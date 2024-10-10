#include "display.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// use SDl to puery the fullscreen width and height maximums
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create SDL Window
	window =
		SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
						 window_width, window_height, SDL_WINDOW_BORDERLESS);

	if (!window) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}

	// Create an SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	if(((x + width) >= window_width) || ((y + height) >= window_height))
	{
		return;
	}

	for(int j = y; j < (y + height); j++)
	{
		for(int i = x; i < (x + width); i++)
		{
			color_buffer[(j*window_width) + i] = color;
		}
	}
}

void draw_grid(void)
{
	for(int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)  
		{
			if( (x % 10 == 0) || (y % 10 == 0))
			{
				color_buffer[(y*window_width) + x] = 0x00000000;
			}
		}
	}
}

void clear_color_buffer(uint32_t color)
{
	for(int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)  
		{
			color_buffer[(y*window_width) + x] = color;
		}
	}
}

void render_color_buffer(void)
{
	SDL_UpdateTexture(color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width * sizeof(uint32_t)));

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

