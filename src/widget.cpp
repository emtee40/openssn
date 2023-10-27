#include "widget.h"
#include "files.h"

Widget::Widget() : surface(NULL)
{
}

Widget::~Widget()
{
}

void Widget::load(const char *filename)
{
    surface = Load_Image(filename);
}

void Widget::unload()
{
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

void Widget::draw(SDL_Surface *dest, int x, int y, bool update)
{
    SDL_Rect rect;

    // Blit destination x & y to the upper left
    rect.x = x;
    rect.y = y;
    // Height and width equal to the source images...
    rect.h = surface->h;
    rect.w = surface->w;
    // Do the actual blit
    SDL_BlitSurface(surface, NULL, dest, &rect);
    if (update) {
        // Show the screen...
        SDL_UpdateRects(dest, 1, &rect);
    }
}

