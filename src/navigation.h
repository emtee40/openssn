#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "SDL/SDL.h"
#include "submarine.h"
#include "helicopter.h"
#include "widget.h"

#define MAP_FACTOR 20
#define MAX_MAP_SCALE 500

class Navigation
{
public:
    Navigation();
    ~Navigation();

    void setShips(Submarine *Subs, Submarine *torpedoes, Helicopter *helicopters);
    void setDepthMap(MAP *my_map);

    void InitGraphics(SDL_Surface *screen);

    void LoadWidgets();
    void UnLoadWidgets();

    void DisplayWidgets();

    void MapIcon(int x, int y, int ShipType, int Friend, Uint32 color);
    void DirectionalPointer(int X, int Y, int Heading, int speed, Uint32 Color);
    void PlaceShips(Submarine *target = NULL);

    void DrawMap(Submarine *current_target);

    void Display_Target(Submarine *current_target);

    void UpdateDisplay(Submarine *current_target);

    void ToggleMapCenter();

    void IncreaseMapScale();
    void DecreaseMapScale();

    void ScrollMapUp(int change_scrolly);
    void ScrollMapDown(int change_scrolly);
    void ScrollMapLeft(int change_scrollx);
    void ScrollMapRight(int change_scrollx);

private:
    Submarine *Subs;
    Submarine *torpedoes;
    Helicopter *helicopters;

    // Depth map with thermal layers
    MAP *my_map;

    int mapcenter;
    int mapscale;
    int scrolloffsetx;  // offset to center map
    int scrolloffsety;

    SDL_Surface *screen;

    Widget mapconsole;
    Widget centerbuttonup;
    Widget centerbuttondown;
    Widget leftbuttonup;
    Widget leftbuttondown;
    Widget rightbuttonup;
    Widget rightbuttondown;
    Widget upbuttonup;
    Widget upbuttondown;
    Widget downbuttonup;
    Widget downbuttondown;
    Widget plusbuttonup;
    Widget plusbuttondown;
    Widget minusbuttonup;
    Widget minusbuttondown;

    Uint32 black, white, red, dark_red, green, dark_green, yellow, brown;
    Uint32 grey, dark_grey, mapcolor;
};

#endif  // NAVIGATION_H
