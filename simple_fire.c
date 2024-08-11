#include "random_table.h"
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

int main(void)
{

	SetConfigFlags(
		FLAG_BORDERLESS_WINDOWED_MODE |
		FLAG_WINDOW_UNDECORATED |
		FLAG_WINDOW_TRANSPARENT |
		FLAG_WINDOW_TOPMOST |
		FLAG_WINDOW_MOUSE_PASSTHROUGH
	);

	const int width = 1920;
	const int height = 1080;
	const int scaledWidth = width / 4;
	const int scaledHeight = height / 4;
	const int bottom = scaledHeight - 1;

	InitWindow(width+1, height+1, "");

	random_table* rng1 = random_table_ctor(25, 0, 1);
	random_table* rng2 = random_table_ctor(25, -1, 1);
	random_table* rng3 = random_table_ctor(25, -1, 3);
	
	int* matrix = (int*)calloc(scaledWidth * scaledHeight, sizeof(int));

	Image buffer = GenImageColor(scaledWidth, scaledHeight, BLANK);
	Texture render = LoadTextureFromImage(buffer);
	Image colorData = LoadImage("E:/Projects/SimpleFireC/x64/Debug/firetable.png");
	
	Color colorTable[64];
	
	Rectangle srcRec = { 0, 0, scaledWidth, scaledHeight };
	Rectangle dstRec = { 0, 0, width, height };

	Vector2 zero = { 0,0 };
	
	int rand3;
	
	for (int i = 0; i < 64; i++)
	{
		colorTable[i] = GetImageColor(colorData, i, 0);
	}


	UnloadImage(colorData);

	while (!WindowShouldClose())
	{

		for (int x = 0; x < scaledWidth; x++)
		{
			matrix[(bottom * scaledWidth) + x] = 63;
			for (int y = 2; y < scaledHeight; y++)
			{
			
				int pos = (y * scaledWidth) + x;
				
				if (matrix[pos] <= 0) {
					continue;
				}
					
				if (matrix[pos] < 52) {
					rand3 = next_value(rng1);
				}
					
				else {
					rand3 = next_value(rng3);
				}
					
				int mX = x + next_value(rng2);
				
				if (mX >= scaledWidth) {
					mX -= scaledWidth;
				}
					
				else if (mX < 0) {
					mX += scaledWidth;
				}
					

				int nextPos = ((y - next_value(rng1)) * scaledWidth) + mX;

				matrix[nextPos] = Clamp(matrix[pos] - rand3, 0, 63);
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
	free(rng3);
	CloseWindow();
	return 0;
}




