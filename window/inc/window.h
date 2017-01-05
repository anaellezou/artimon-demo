#ifndef __WINDOW_H
# define __WINDOW_H

# include "/Library/Frameworks/SDL2.framework/Versions/A/Headers/SDL.h" 

# define SDL_WINDOW			window->window
# define SDL_SCREEN			window->screen
# define SDL_IMAGE			window->image
# define SDL_FRAME_BUFFER	window->frameBuffer
# define SDL_WIDTH			window->width
# define SDL_HEIGHT			window->height
# define SDL_DEPTH			window->depth
# define SDL_BPL			window->bytesPerLine
# define SDL_EVENT			window->event
# define KEY				SDL_EVENT.key.keysym.sym				

typedef	struct
{
	SDL_Window		*window;
	SDL_Surface		*screen;
	SDL_Surface		*image;
	SDL_Event		event;
	char			*frameBuffer;
	int				width;
	int				height;
	int				depth;
	int				bytesPerLine;
	int				flags;
}					Window;

void				putPixel(Window *window, int x, int y, int r, int g, int b);
void				putImage(Window *window);
Window				*newWindow(char *title, int width, int height);
void				killSDL(Window *window);
void				hang(Window *window);

#endif
