#include "raylib.h"
#include <stdio.h>

// configs
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 800;

const int BORDER_THICKNESS = 10;
const int DASH_SIZE = 10;
const int DASH_SPACE = 10;

const int PADDLE_OFFSET = 20;
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 120;
const int PLAYER_MOVEMENT_INTERVAL = 15;
const int AI_MOVEMENT_INTERVAL = 5;
const int POINTS_TO_WIN = 3;

const int BALL_RADIUS = 10;
const int BALL_SPEED = 15;
const int MAX_BALL_SLOPE = 15;

const int FONT_SIZE = 20;
const int TEXT_DISTANCE = 150;

enum GameStatus {
	RUNNING,
	RESET,
	OVER
};

struct GameState {
	Vector2 player_paddle;
	Vector2 ai_paddle;
	Vector2 ball_pos;
	Vector2 ball_slope;
	int player_score;
	int ai_score;
	enum GameStatus game_status;
	char score_string[2];
};

struct GameState get_initial_state(){
	struct GameState game_state;

	game_state.ai_score = 0;
	game_state.player_score = 0;
	game_state.game_status= RUNNING;

	game_state.ai_paddle = (Vector2){
		(float)PADDLE_OFFSET,
		(WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0)
	};

	game_state.player_paddle = (Vector2){
		(float)WINDOW_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET,
		(WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0)
	};

	game_state.ball_pos = (Vector2){
		WINDOW_WIDTH/2.0,
		WINDOW_HEIGHT/2.0
	};

	game_state.ball_slope = (Vector2){
		GetRandomValue(0, 1) ? BALL_SPEED : -BALL_SPEED,
		GetRandomValue(-1*MAX_BALL_SLOPE/4, MAX_BALL_SLOPE/4)
	};

	return game_state;
}

struct GameState reset_state_for_new_point(struct GameState game_state){
	game_state.game_status= RUNNING;

	game_state.ai_paddle = (Vector2){
		(float)PADDLE_OFFSET,
		(WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0)
	};

	game_state.player_paddle = (Vector2){
		(float)WINDOW_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET,
		(WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0)
	};

	game_state.ball_pos = (Vector2){
		WINDOW_WIDTH/2.0,
		WINDOW_HEIGHT/2.0
	};

	game_state.ball_slope = (Vector2){
		GetRandomValue(0, 1) ? BALL_SPEED : -BALL_SPEED,
		GetRandomValue(-1*MAX_BALL_SLOPE/4, MAX_BALL_SLOPE/4)
	};

	return game_state;
}

bool is_ball_colliding_with_box(Vector2 box_pos, Vector2 box_dimensions, Vector2 ball_pos){
	bool is_within_x_left = ball_pos.x + BALL_RADIUS > box_pos.x;
	bool is_within_x_right = ball_pos.x - BALL_RADIUS < box_pos.x + box_dimensions.x;
	bool is_within_y_left = ball_pos.y + BALL_RADIUS > box_pos.y;
	bool is_within_y_right = ball_pos.y - BALL_RADIUS < box_pos.y + box_dimensions.y;
	return is_within_x_left && is_within_x_right && is_within_y_left && is_within_y_right;
}

float get_collision_angle(Vector2 paddle_pos, Vector2 ball_pos){
	float py = paddle_pos.y + (PADDLE_HEIGHT/2.0);
	float diff_from_paddle_center = py - ball_pos.y;
	float percentage_diff = (diff_from_paddle_center/PADDLE_HEIGHT);
	float variation = GetRandomValue(0, 50)/1000.0;
	return (percentage_diff+variation) * -MAX_BALL_SLOPE;
}

void draw_game_over_screen(int player_score, int ai_score){
	BeginDrawing();
	ClearBackground(BLACK);

	DrawText("GAME OVER", (WINDOW_WIDTH/2) - 240, (WINDOW_HEIGHT/2)-140, 80, WHITE);
	if(ai_score == POINTS_TO_WIN){
		DrawText("AI Wins", (WINDOW_WIDTH/2) - 120, (WINDOW_HEIGHT/2) - 40, 60, WHITE);
	} else {
		DrawText("You Win", (WINDOW_WIDTH/2) - 120, (WINDOW_HEIGHT/2) - 40, 60, WHITE);
	}
	DrawText("Press esc to exit or enter to play again", (WINDOW_WIDTH/2) - 200, (WINDOW_HEIGHT/2) + 40, 20, GRAY);
	
	EndDrawing();
}

int main () {
	// use vsync of high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(60);

	// Debugging
	// SetTraceLogLevel(LOG_DEBUG);

	// create window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong in C");

	// audio configs
	InitAudioDevice();
	Sound ball_hit_sound = LoadSound("resources/ball_hit.ogg");
	Sound point_scored_sound = LoadSound("resources/point_scored.ogg");

	struct GameState game_state = get_initial_state();

	// game loop, run until close or escape
	while (!WindowShouldClose())
	{
		if(game_state.game_status == OVER && IsKeyDown(KEY_ENTER)){
			TraceLog(LOG_DEBUG, "Game state: Over, User pressed Enter, resetting state");
			game_state = get_initial_state();
			continue;
		} else if(game_state.game_status == OVER) {
			TraceLog(LOG_DEBUG, "Game state: Over, Awaiting user input");
			draw_game_over_screen(game_state.player_score, game_state.ai_score);
			continue;
		}

		if(game_state.ai_score == POINTS_TO_WIN || game_state.player_score == POINTS_TO_WIN){
			TraceLog(LOG_DEBUG, "One of the players has won, showing game over screen");
			draw_game_over_screen(game_state.player_score, game_state.ai_score);
			game_state.game_status = OVER;
			continue;
		}

		if(game_state.game_status == RESET){
			TraceLog(LOG_DEBUG, "Point scored, waiting 1 second to reset stage");
			WaitTime(1);
			game_state = reset_state_for_new_point(game_state);
			continue;
		}

		if(IsKeyDown(KEY_J)){
			// check if would hit border
			if(game_state.player_paddle.y < WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT){
				bool hitting_edge = game_state.player_paddle.y + PLAYER_MOVEMENT_INTERVAL > WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT;
				game_state.player_paddle.y = hitting_edge ? WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT : game_state.player_paddle.y + PLAYER_MOVEMENT_INTERVAL;
			}
		}
		if(IsKeyDown(KEY_K)){
			// check if would hit border
			if(game_state.player_paddle.y > BORDER_THICKNESS){
				bool hitting_edge = game_state.player_paddle.y - PLAYER_MOVEMENT_INTERVAL < BORDER_THICKNESS;
				game_state.player_paddle.y = hitting_edge ? BORDER_THICKNESS : game_state.player_paddle.y - PLAYER_MOVEMENT_INTERVAL;
			}
		}

		// move ai paddle
		if(game_state.ball_pos.y > game_state.ai_paddle.y + PADDLE_HEIGHT){
			bool hitting_edge = game_state.ai_paddle.y + AI_MOVEMENT_INTERVAL > WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT;
			game_state.ai_paddle.y = hitting_edge ? WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT : game_state.ai_paddle.y + AI_MOVEMENT_INTERVAL;
		} else if (game_state.ball_pos.y < game_state.ai_paddle.y) {
			bool hitting_edge = game_state.ai_paddle.y - AI_MOVEMENT_INTERVAL < BORDER_THICKNESS;
			game_state.ai_paddle.y = hitting_edge ? BORDER_THICKNESS : game_state.ai_paddle.y - AI_MOVEMENT_INTERVAL;
		}

		// move the ball in the direction of the slope
		game_state.ball_pos.x += game_state.ball_slope.x;
		game_state.ball_pos.y += game_state.ball_slope.y;

		// check border collisions
		bool is_colliding_top_border = is_ball_colliding_with_box(
			(Vector2){0.0,0.0}, 
			(Vector2){WINDOW_WIDTH, BORDER_THICKNESS}, 
			game_state.ball_pos
		);

		bool is_colliding_bottom_border = is_ball_colliding_with_box(
			(Vector2) {0.0, (float)WINDOW_HEIGHT-BORDER_THICKNESS},
			(Vector2){WINDOW_WIDTH, BORDER_THICKNESS}, 
			game_state.ball_pos
		);

		bool is_colliding_player_paddle = is_ball_colliding_with_box(
			game_state.player_paddle,
			(Vector2) {PADDLE_WIDTH, PADDLE_HEIGHT},
			game_state.ball_pos
		); 

		bool is_colliding_ai_paddle = is_ball_colliding_with_box(
			game_state.ai_paddle,
			(Vector2) {PADDLE_WIDTH, PADDLE_HEIGHT},
			game_state.ball_pos
		); 
		
		if(is_colliding_top_border || is_colliding_bottom_border){
			PlaySound(ball_hit_sound);
			game_state.ball_slope.y *= -1;
		}

		if(is_colliding_player_paddle){
			PlaySound(ball_hit_sound);
			game_state.ball_slope.y = get_collision_angle(game_state.player_paddle, game_state.ball_pos);
			game_state.ball_slope.x *= -1;
		}

		if(is_colliding_ai_paddle){
			PlaySound(ball_hit_sound);
			game_state.ball_slope.y = get_collision_angle(game_state.ai_paddle, game_state.ball_pos);
			game_state.ball_slope.x *= -1;
		}

		// check point scored
		if(game_state.ball_pos.x > WINDOW_WIDTH + BALL_RADIUS){
			PlaySound(point_scored_sound);
			game_state.ai_score += 1;
			game_state.game_status = RESET;
			TraceLog(LOG_DEBUG, "Point scored by AI, setting game_state to RESET");
		}

		if(game_state.ball_pos.x < -BALL_RADIUS){
			PlaySound(point_scored_sound);
			game_state.player_score += 1;
			game_state.game_status = RESET;
			TraceLog(LOG_DEBUG, "Point scored by Player, setting game_state to RESET");
		}

		// frame starts
		BeginDrawing();
		ClearBackground(BLACK);

		// draw stage
		DrawRectangle(0, 0, WINDOW_WIDTH, BORDER_THICKNESS, WHITE);
		DrawRectangle(0, WINDOW_HEIGHT-BORDER_THICKNESS, WINDOW_WIDTH, BORDER_THICKNESS, WHITE);
		DrawLineDashed((Vector2) {WINDOW_WIDTH/2.0, 0.0}, (Vector2) {WINDOW_WIDTH/2.0, WINDOW_HEIGHT*1.0}, DASH_SIZE, DASH_SPACE, WHITE);

		// draw scoreboard
		sprintf(game_state.score_string, "%d", game_state.ai_score);
		DrawText("COMPUTER", (WINDOW_WIDTH/2) - TEXT_DISTANCE, BORDER_THICKNESS*4, FONT_SIZE, GRAY);
		DrawText(game_state.score_string, (WINDOW_WIDTH/2) - TEXT_DISTANCE + 45, BORDER_THICKNESS*8, FONT_SIZE*4, GRAY);

		sprintf(game_state.score_string, "%d", game_state.player_score);
		DrawText("PLAYER", (WINDOW_WIDTH/2) + TEXT_DISTANCE/4, BORDER_THICKNESS*4, FONT_SIZE, GRAY);
		DrawText(game_state.score_string, (WINDOW_WIDTH/2) + TEXT_DISTANCE/4 + 25, BORDER_THICKNESS*8, FONT_SIZE*4, GRAY);

		// draw player paddles
		DrawRectangle(game_state.ai_paddle.x, game_state.ai_paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
		DrawRectangle(game_state.player_paddle.x, game_state.player_paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

		// draw ball
		DrawCircle(game_state.ball_pos.x, game_state.ball_pos.y, (float)BALL_RADIUS, WHITE);

		// frame ends
		EndDrawing();
	}
	// destroy the window and cleanup the OpenGL context
	UnloadSound(ball_hit_sound);
	UnloadSound(point_scored_sound);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
