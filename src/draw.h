#ifndef DRAW_HEADER_FILE__
#define DRAW_HEADER_FILE__

#include <SDL.h>
#include <SDL/SDL.h>

void DrawPixel(SDL_Surface *screen, int x, int y, Uint32 color);

void DrawLine(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color);

void DrawArc(SDL_Surface *screen, int x, int y, int radius, int theta1, int theta2, Uint32 color);

void DrawCircle(SDL_Surface *screen, int x, int y, int radius, Uint32 color);

void FillCircle(SDL_Surface *screen, int x, int y, int radius, Uint32 color);

void DrawDiamond(SDL_Surface *screen, int x, int y, int size, char direction, Uint32 color);

void DrawBox(SDL_Surface *screen, int x, int y, int size, char direction, Uint32 color);

void DrawRectangle(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color);

void FillRectangle(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color);

void DrawCross(SDL_Surface *screen, int x, int y, int size, char direction, Uint32 color);

#endif  // DRAW_HEADER_FILE__
