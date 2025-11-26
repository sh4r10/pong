#include "raylib.h"
#include "resource_dir.h"

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

	// use vsync of high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// create window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong in C");

	// game state
	float ai_paddle_x = PADDLE_OFFSET;
	float ai_paddle_y = (WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0);

	float player_paddle_x = WINDOW_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET;
	float player_paddle_y = (WINDOW_HEIGHT / 2.0) - (PADDLE_HEIGHT / 2.0);
	
	// game loop, run until close or escape
	while (!WindowShouldClose())
	{
		// frame starts
		BeginDrawing();
		ClearBackground(BLACK);
		// draw stage
		DrawRectangle(0, 0, WINDOW_WIDTH, BORDER_THICKNESS, WHITE);
		DrawRectangle(0, WINDOW_HEIGHT-BORDER_THICKNESS, WINDOW_WIDTH, BORDER_THICKNESS, WHITE);
		DrawLineDashed((Vector2) {WINDOW_WIDTH/2.0, 0.0}, (Vector2) {WINDOW_WIDTH/2.0, WINDOW_HEIGHT*1.0}, DASH_SIZE, DASH_SPACE, WHITE);
		
		DrawRectangle(ai_paddle_x, ai_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
		DrawRectangle(player_paddle_x, player_paddle_y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
		// frame ends
		EndDrawing();
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
