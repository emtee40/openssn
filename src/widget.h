#ifndef WIDGET_H
#define WIDGET_H

#include "SDL/SDL.h"

class Widget
{
public:
    Widget();
    ~Widget();

    void load(const char *filename);
    void unload();

    void draw(SDL_Surface *dest, int x, int y, bool update = false);

private:
    SDL_Surface *surface;
};

#endif  // WIDGET_H
