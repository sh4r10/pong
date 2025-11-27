#include "raylib.h"

bool is_ball_colliding(Vector2 box_pos, Vector2 box_dimensions, Vector2 ball_pos, int ball_radius){
	bool is_within_x_left = ball_pos.x + ball_radius > box_pos.x;
	bool is_within_x_right = ball_pos.x - ball_radius < box_pos.x + box_dimensions.x;
	bool is_within_y_left = ball_pos.y + ball_radius > box_pos.y;
	bool is_within_y_right = ball_pos.y - ball_radius < box_pos.y + box_dimensions.y;
	return is_within_x_left && is_within_x_right && is_within_y_left && is_within_y_right;
}

int main () {
	// configs
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 800;

	const int BORDER_THICKNESS = 10;
	const int DASH_SIZE = 10;
	const int DASH_SPACE = 10;

	const int PADDLE_OFFSET = 20;
	const int PADDLE_WIDTH = 20;
	const int PADDLE_HEIGHT = 100;
	const int PADDLE_MOVEMENT_INTERVAL = 20;

	const int BALL_RADIUS = 10;
	const int BALL_SPEED = 5;

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
		GetRandomValue(-2, 2),
		GetRandomValue(-2, 2)
	};
	
	// game loop, run until close or escape
	while (!WindowShouldClose())
	{
		if(IsKeyDown(KEY_J)){
			// check if would hit border
			if(player_paddle.y + PADDLE_MOVEMENT_INTERVAL <= WINDOW_HEIGHT - BORDER_THICKNESS - PADDLE_HEIGHT){
				player_paddle.y += PADDLE_MOVEMENT_INTERVAL;
			}
		}
		if(IsKeyDown(KEY_K)){
			// check if would hit border
			if(player_paddle.y - PADDLE_MOVEMENT_INTERVAL >= BORDER_THICKNESS){
				player_paddle.y -= PADDLE_MOVEMENT_INTERVAL;
			}
		}
		
		ball_pos.x += ball_slope.x * BALL_SPEED;
		ball_pos.y += ball_slope.y * BALL_SPEED;

		// check border collisions
		bool is_colliding_top_border = is_ball_colliding(
			(Vector2){0.0,0.0}, 
			(Vector2){WINDOW_WIDTH, BORDER_THICKNESS}, 
			ball_pos, 
			BALL_RADIUS
		);

		bool is_colliding_bottom_border = is_ball_colliding(
			(Vector2) {0.0, (float)WINDOW_HEIGHT-BORDER_THICKNESS},
			(Vector2){WINDOW_WIDTH, BORDER_THICKNESS}, 
			ball_pos,
			BALL_RADIUS
		);

		bool is_colliding_player_paddle = is_ball_colliding(
			player_paddle,
			(Vector2) {PADDLE_WIDTH, PADDLE_HEIGHT},
			ball_pos,
			BALL_RADIUS
		); 

		bool is_colliding_ai_paddle = is_ball_colliding(
			ai_paddle,
			(Vector2) {PADDLE_WIDTH, PADDLE_HEIGHT},
			ball_pos,
			BALL_RADIUS
		); 
		
		if(is_colliding_top_border || is_colliding_bottom_border){
			ball_slope.y *= -1;
		}

		if(is_colliding_player_paddle || is_colliding_ai_paddle){
			ball_slope.x *= -1;
		}

		// move ai paddle
		if(ball_pos.y > ai_paddle.y + PADDLE_HEIGHT){
			if(ai_paddle.y + PADDLE_MOVEMENT_INTERVAL < WINDOW_HEIGHT-BORDER_THICKNESS){
				ai_paddle.y += PADDLE_MOVEMENT_INTERVAL;
			}
		} else if (ball_pos.y < ai_paddle.y) {
			if(ai_paddle.y - PADDLE_MOVEMENT_INTERVAL > BORDER_THICKNESS){
				ai_paddle.y -= PADDLE_MOVEMENT_INTERVAL;
			}
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
