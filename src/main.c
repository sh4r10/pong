#include "raylib.h"

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

		// frame ends
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
