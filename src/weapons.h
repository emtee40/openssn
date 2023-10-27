#ifndef WEAPONS_H
#define WEAPONS_H

#include "SDL/SDL.h"
#include "submarine.h"
#include "widget.h"

class Weapons
{
public:
    Weapons();
    ~Weapons();

    void setSubs(Submarine *Subs);

    void InitGraphics(SDL_Surface *screen);

    void LoadWidgets();
    void UnLoadWidgets();

    void DisplayWidgets();

    void UpdateDisplay();

private:
    Submarine *Subs;

    SDL_Surface *screen;

    Widget weaponsconsole;
    Widget noisemaker_image;
    Widget torpedo_image;

    Uint32 black, green;
};

#endif  // WEAPONS_H
