#include "random_table.h"
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"


int main()
{
	const int width = 1920;
	const int height = 1080;

	const int horizontalRepetitions = 16;
	const int scaleFactor = 4;
	const int ratio = horizontalRepetitions / scaleFactor;

	const int scaledWidth = width / horizontalRepetitions;
	const int scaledHeight = height / scaleFactor;

	const int bottom = (scaledHeight - 1) * scaledWidth;

	Rectangle srcRec = { 0, 0, scaledWidth * ratio, scaledHeight };
	Rectangle dstRec = { 0, 0, width, height };

	Vector2 zero = { 0, 0 };

	SetConfigFlags(
		FLAG_BORDERLESS_WINDOWED_MODE |
		FLAG_WINDOW_UNDECORATED |
		FLAG_WINDOW_TRANSPARENT |
		FLAG_WINDOW_TOPMOST |
		FLAG_WINDOW_MOUSE_PASSTHROUGH
	);

	InitWindow(width-2, height-2, "hi");
	
	SetTargetFPS(120);

	random_table* rng1 = random_table_ctor(16, 0, 1);
	random_table* rng2 = random_table_ctor(16, -1, 1);
	
	int* matrix = (int*)calloc(scaledWidth * scaledHeight, sizeof(int));

	Image buffer = GenImageColor(scaledWidth, scaledHeight, BLANK);
	
	Texture render = LoadTextureFromImage(buffer);
	SetTextureWrap(render, TEXTURE_WRAP_REPEAT);

	Image colorData = LoadImage("res/firetable.png");
	Color colorTable[64];
	for (int i = 0; i < 64; i++)
	{
		colorTable[i] = GetImageColor(colorData, i, 0);
	}
	UnloadImage(colorData);

	int decay;
	
	while (!WindowShouldClose())
	{
		for (int x = 0; x < scaledWidth; x++)
		{
			matrix[bottom + x] = 63;
			for (int y = 2; y < scaledHeight; y++)
			{
				int pos;
				pos = (y * scaledWidth) + x;
				
				if (matrix[pos] <= 0)
					continue;

				if (matrix[pos] < 50)
					decay = next_value(rng1) + next_value(rng1);

				else
					decay = next_value(rng1);
					
				int mX = x + next_value(rng2);
				
				if (mX >= scaledWidth) 
					mX -= scaledWidth;
				
				else if (mX < 0) 
					mX += scaledWidth;

				int nextPos = ((y - next_value(rng1)) * scaledWidth) + mX;

				matrix[nextPos] = Clamp(matrix[pos] - decay, 0, 63);
				ImageDrawPixel(&buffer, x, y, colorTable[matrix[pos]]);
			}
		}

		UpdateTexture(render, buffer.data);

		//Draw
		BeginDrawing();
			ClearBackground(BLANK);
			DrawTexturePro(render, srcRec, dstRec, zero, 0, WHITE);
			DrawFPS(100, 100);
		EndDrawing();
	}

	UnloadImage(buffer);
	UnloadTexture(render);

	free(matrix);
	free(rng1);
	free(rng2);

	CloseWindow();
	
	return 0;
}




