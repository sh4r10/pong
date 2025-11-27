#include "raylib.h"

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

const int BALL_RADIUS = 10;
const int BALL_SPEED = 15;
const int MAX_BALL_SLOPE = 15;

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

int main () {

	// use vsync of high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// create window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong in C");

	// game state
	Vector2 ai_paddle = {
		(float)PADDLE_OFFSET,
		(WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0)
	};

	Vector2 player_paddle = {
		(float)WINDOW_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET,
		(WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0)
	};

	Vector2 ball_pos = {
		WINDOW_WIDTH/2.0,
		WINDOW_HEIGHT/2.0
	};

	Vector2 ball_slope = {
		GetRandomValue(0, 1) ? BALL_SPEED : -BALL_SPEED,
		GetRandomValue(-1*MAX_BALL_SLOPE, MAX_BALL_SLOPE)
	};
	
	// game loop, run until close or escape
	while (!WindowShouldClose())
	{
		if(IsKeyDown(KEY_J)){
			// check if would hit border
			if(player_paddle.y < WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT){
				bool hitting_edge = player_paddle.y + PLAYER_MOVEMENT_INTERVAL > WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT;
				player_paddle.y = hitting_edge ? WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT : player_paddle.y + PLAYER_MOVEMENT_INTERVAL;
			}
		}
		if(IsKeyDown(KEY_K)){
			// check if would hit border
			if(player_paddle.y > BORDER_THICKNESS){
				bool hitting_edge = player_paddle.y - PLAYER_MOVEMENT_INTERVAL < BORDER_THICKNESS;
				player_paddle.y = hitting_edge ? BORDER_THICKNESS : player_paddle.y - PLAYER_MOVEMENT_INTERVAL;
			}
		}

		// move ai paddle
		if(ball_pos.y > ai_paddle.y + PADDLE_HEIGHT){
			bool hitting_edge = ai_paddle.y + AI_MOVEMENT_INTERVAL > WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT;
			ai_paddle.y = hitting_edge ? WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT : ai_paddle.y + AI_MOVEMENT_INTERVAL;
		} else if (ball_pos.y < ai_paddle.y) {
			bool hitting_edge = ai_paddle.y - AI_MOVEMENT_INTERVAL < BORDER_THICKNESS;
			ai_paddle.y = hitting_edge ? BORDER_THICKNESS : ai_paddle.y - AI_MOVEMENT_INTERVAL;
		}


		ball_pos.x += ball_slope.x;
		ball_pos.y += ball_slope.y;

		// check border collisions
		bool is_colliding_top_border = is_ball_colliding_with_box(
			(Vector2){0.0,0.0}, 
			(Vector2){WINDOW_WIDTH, BORDER_THICKNESS}, 
			ball_pos
		);

		bool is_colliding_bottom_border = is_ball_colliding_with_box(
			(Vector2) {0.0, (float)WINDOW_HEIGHT-BORDER_THICKNESS},
			(Vector2){WINDOW_WIDTH, BORDER_THICKNESS}, 
			ball_pos
		);

		bool is_colliding_player_paddle = is_ball_colliding_with_box(
			player_paddle,
			(Vector2) {2, PADDLE_HEIGHT},
			ball_pos
		); 

		bool is_colliding_ai_paddle = is_ball_colliding_with_box(
			ai_paddle,
			(Vector2) {2, PADDLE_HEIGHT},
			ball_pos
		); 
		
		if(is_colliding_top_border || is_colliding_bottom_border){
			ball_slope.y *= -1;
		}

		if(is_colliding_player_paddle){
			ball_slope.y = get_collision_angle(player_paddle, ball_pos);
			ball_slope.x *= -1;
		}

		if(is_colliding_ai_paddle){
			ball_slope.y = get_collision_angle(ai_paddle, ball_pos);
			ball_slope.x *= -1;
		}

		// frame starts
		BeginDrawing();
		ClearBackground(BLACK);

		// draw stage
		DrawRectangle(0, 0, WINDOW_WIDTH, BORDER_THICKNESS, WHITE);
		DrawRectangle(0, WINDOW_HEIGHT-BORDER_THICKNESS, WINDOW_WIDTH, BORDER_THICKNESS, WHITE);
		DrawLineDashed((Vector2) {WINDOW_WIDTH/2.0, 0.0}, (Vector2) {WINDOW_WIDTH/2.0, WINDOW_HEIGHT*1.0}, DASH_SIZE, DASH_SPACE, WHITE);
		
		// draw player paddles
		DrawRectangle(ai_paddle.x, ai_paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
		DrawRectangle(player_paddle.x, player_paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);

		// draw ball
		DrawCircle(ball_pos.x, ball_pos.y, (float)BALL_RADIUS, WHITE);

		// frame ends
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
