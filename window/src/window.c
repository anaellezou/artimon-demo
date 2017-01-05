#include "window.h"

void	killSDL(Window *window)
{
	SDL_FreeSurface(window->image);
	SDL_FreeSurface(window->screen);
	SDL_DestroyWindow(SDL_WINDOW);
	SDL_Quit();
	exit(0);
}

void		putPixel(Window *window, int x, int y, int r, int g, int b)
{
	int i;

	if (x < 0 || x > window->width || y < 0 || y > window->height)
		return ;
	i = (x * SDL_DEPTH >> 3) + y * SDL_BPL;
	SDL_FRAME_BUFFER[i] = b;
	SDL_FRAME_BUFFER[i + 1] = g;
	SDL_FRAME_BUFFER[i + 2] = r;
	SDL_FRAME_BUFFER[i + 3] = 1;
}

void hang(Window *window)
{

	int quit;

	quit = 0;
	while (!quit)
	{
		while (SDL_PollEvent(&SDL_EVENT))
		{
			if (SDL_EVENT.type == SDL_QUIT ||
			KEY == SDLK_ESCAPE)
			{
				quit = 1;
				killSDL(window);
			}
		}
	}
}

void		putImage(Window *window)
{
	SDL_BlitSurface(SDL_IMAGE, NULL, SDL_SCREEN, NULL);
	SDL_UpdateWindowSurface(SDL_WINDOW);
	hang(window);
}

Window *newWindow(char *title, int width, int height)                                                        
{                                                                                                              
	Window *window = malloc(sizeof(Window));                                                               
																											   
	SDL_WIDTH = width;                                                                                         
	SDL_HEIGHT = height;                                                                                       
	SDL_DEPTH = 32;                                                                                            
	SDL_BPL = SDL_WIDTH * (SDL_DEPTH / 8);                                                                     
	SDL_Init(SDL_INIT_VIDEO);                                                                                  
	SDL_WINDOW = SDL_CreateWindow(title, 200, 200,                                                             
	SDL_WIDTH, SDL_HEIGHT, 0);
	SDL_SCREEN = SDL_GetWindowSurface(SDL_WINDOW);
	SDL_FRAME_BUFFER = malloc(SDL_WIDTH * SDL_HEIGHT * 4);
	SDL_IMAGE = SDL_CreateRGBSurfaceFrom(SDL_FRAME_BUFFER,
	SDL_WIDTH, SDL_HEIGHT, SDL_DEPTH, SDL_BPL, 0, 0, 0, 0);
	return (window);
}
