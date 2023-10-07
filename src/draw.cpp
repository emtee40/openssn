#include "draw.h"
#include <cmath>

void DrawPixel(SDL_Surface *screen, int x, int y, Uint32 color)
{
    // this only works for 32bpp screens
    // are we outside the screen?????
    // If we are bail out now before it's too late!

    if (x >= screen->w || x < 0 || y >= screen->h || y < 0) {
        return;
    }

    // place the pixel on the screen
    Uint32 *pixel_location;
    pixel_location = (Uint32 *)screen->pixels + y * screen->pitch / 4 + x;
    *pixel_location = color;
}

void DrawLine(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
    // don't even ask me about this stuff all I know is it works
    // and thats ALL I care about...

    int dx, dy, sdx, sdy, py, px, x, y;
    dx = x2 - x1;
    dy = y2 - y1;
    if (dx < 0) sdx = -1;
    else sdx = 1;
    if (dy < 0) sdy = -1;
    else sdy = 1;
    dx = sdx * dx + 1;
    dy = sdy * dy + 1;
    x = 0;
    y = 0;
    px = x1;
    py = y1;
    if (dx >= dy) {
        for (int x = 0; x < dx; x++) {
            DrawPixel(screen, px, py, color);
            y = y + dy;
            if (y >= dx) {
                y = y - dx;
                py = py + sdy;
            }
            px = px + sdx;
        }
    } else {
        for (int y = 0; y < dy; y++) {
            DrawPixel(screen, px, py, color);
            x = x + dx;
            if (x >= dy) {
                x = x - dy;
                px = px + sdx;
            }
            py = py + sdy;
        }
    }
}

void DrawArc(SDL_Surface *screen, int x, int y, int radius, int theta1, int theta2, Uint32 color)
{
    // Draw an arc at (x,y) of a given radius from theta1 to theta2 using specified color.
    int xa, ya;
    int theta;

    for (theta = theta1; theta <= theta2; theta += 5) {
        xa = x + int(radius * cos(theta * 3.14 / 180.0));
        ya = y - int(radius * sin(theta * 3.14 / 180.0));
        DrawPixel(screen, xa, ya, color);
    }
}

void DrawCircle(SDL_Surface *screen, int x, int y, int radius, Uint32 color)
{
    // Draw a circle at (x,y) of a given radius using specified color.
    DrawArc(screen, x, y, radius, 0, 360, color);
}

void FillCircle(SDL_Surface *screen, int x, int y, int radius, Uint32 color)
{
    while (radius > 1) {
        DrawCircle(screen, x, y, radius, color);
        radius--;
    }
}

void DrawDiamond(SDL_Surface *screen, int x, int y, int size, char direction, Uint32 color)
{
    // Draw a diamond at (x,y) of size using specified color.
    // Direction: T = Top, B = Bottom, F = Full
    switch (direction) {
        case 'B':
            DrawLine(screen, x - size, y, x, y + size, color);  // bottomleft
            DrawLine(screen, x + size, y, x, y + size, color);  // bottomright
            break;

        case 'T':
            DrawLine(screen, x, y - size, x - size, y, color);  // topleft
            DrawLine(screen, x, y - size, x + size, y, color);  // topright
            break;

        default:
            DrawLine(screen, x - size, y, x, y + size, color);  // bottomleft
            DrawLine(screen, x + size, y, x, y + size, color);  // bottomright
            DrawLine(screen, x, y - size, x - size, y, color);  // topleft
            DrawLine(screen, x, y - size, x + size, y, color);  // topright
    }
}

void DrawBox(SDL_Surface *screen, int x, int y, int size, char direction, Uint32 color)
{
    // Draw a box at (x,y) of size using specified color.
    // Direction: T = Top, B = Bottom, F = Full
    switch (direction) {
        case 'T':
            DrawLine(screen, x - size, y - size, x - size, y, color);
            DrawLine(screen, x - size, y - size, x + size, y - size, color);
            DrawLine(screen, x + size, y - size, x + size, y, color);
            break;

        case 'B':
            DrawLine(screen, x - size, y, x - size, y + size, color);
            DrawLine(screen, x - size, y + size, x + size, y + size, color);
            DrawLine(screen, x + size, y, x + size, y + size, color);
            break;

        default:
            DrawLine(screen, x - size, y - size, x + size, y - size, color);
            DrawLine(screen, x + size, y - size, x + size, y + size, color);
            DrawLine(screen, x - size, y - size, x - size, y + size, color);
            DrawLine(screen, x - size, y + size, x + size, y + size, color);
    }
}

void DrawRectangle(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
    DrawLine(screen, x1, y1, x1, y2, color);
    DrawLine(screen, x1, y1, x2, y1, color);
    DrawLine(screen, x1, y2, x2, y2, color);
    DrawLine(screen, x2, y1, x2, y2, color);
}

void FillRectangle(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
    int x, y;

    for (y = y1; y <= y2; y++) {
        for (x = x1; x <= x2; x++) {
            DrawPixel(screen, x, y, color);
        }
    }
}

void DrawCross(SDL_Surface *screen, int x, int y, int size, char direction, Uint32 color)
{
    // Draw a Cross at (x,y) of size using specified color.
    // Direction: T = Top, B = Bottom, F = Full
    switch (direction) {
        case 'T':
            DrawLine(screen, x - size, y - size / 2, x - size, y, color);
            DrawLine(screen, x + size, y - size / 2, x + size, y, color);
            DrawLine(screen, x - size / 2, y - size, x + size / 2, y - size, color);
            DrawLine(screen, x - size / 2, y - size, x - size / 2, y - size / 2, color);
            DrawLine(screen, x + size / 2, y - size, x + size / 2, y - size / 2, color);
            DrawLine(screen, x + size / 2, y - size / 2, x + size, y - size / 2, color);
            DrawLine(screen, x - size, y - size / 2, x - size / 2, y - size / 2, color);
            break;

        case 'B':
            DrawLine(screen, x - size, y, x - size, y + size / 2, color);
            DrawLine(screen, x + size, y, x + size, y + size / 2, color);
            DrawLine(screen, x - size / 2, y + size, x + size / 2, y + size, color);
            DrawLine(screen, x - size / 2, y + size / 2, x - size / 2, y + size, color);
            DrawLine(screen, x + size / 2, y + size / 2, x + size / 2, y + size, color);
            DrawLine(screen, x - size, y + size / 2, x - size / 2, y + size / 2, color);
            DrawLine(screen, x + size / 2, y + size / 2, x + size, y + size / 2, color);
            break;

        default:
            DrawLine(screen, x - size, y - size / 2, x - size, y + size / 2, color);
            DrawLine(screen, x + size, y - size / 2, x + size, y + size / 2, color);
            DrawLine(screen, x - size / 2, y - size, x + size / 2, y - size, color);
            DrawLine(screen, x - size / 2, y + size, x + size / 2, y + size, color);
            DrawLine(screen, x - size / 2, y - size, x - size / 2, y - size / 2, color);
            DrawLine(screen, x - size / 2, y + size / 2, x - size / 2, y + size, color);
            DrawLine(screen, x + size / 2, y - size, x + size / 2, y - size / 2, color);
            DrawLine(screen, x + size / 2, y + size / 2, x + size / 2, y + size, color);
            DrawLine(screen, x - size, y + size / 2, x - size / 2, y + size / 2, color);
            DrawLine(screen, x + size / 2, y - size / 2, x + size, y - size / 2, color);
            DrawLine(screen, x - size, y - size / 2, x - size / 2, y - size / 2, color);
            DrawLine(screen, x + size / 2, y + size / 2, x + size, y + size / 2, color);
    }
}
