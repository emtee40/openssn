#include "navigation.h"
#include <cmath>
#include "dfont.h"
#include "draw.h"
#include "files.h"

Navigation::Navigation()
{
    mapcenter = 1;  // by default, center the map on our ship
    mapscale = 25;
    scrolloffsetx = 0;
    scrolloffsety = 0;
    my_map = NULL;
}

Navigation::~Navigation()
{
}

// Set pointers to the shared lists of subs, torpedoes and helicopters
void Navigation::setShips(Submarine *Subs, Submarine *torpedoes, Helicopter *helicopters)
{
    this->Subs = Subs;
    this->torpedoes = torpedoes;
    this->helicopters = helicopters;
}

// Set the pointer to the shared depth map
void Navigation::setDepthMap(MAP *my_map)
{
    this->my_map = my_map;
}

void Navigation::InitGraphics(SDL_Surface *screen)
{
    this->screen = screen;

    black      = SDL_MapRGB(screen->format, 0, 0, 0);
    white      = SDL_MapRGB(screen->format, 255, 255, 255);
    red        = SDL_MapRGB(screen->format, 255, 99, 20);
    dark_red   = SDL_MapRGB(screen->format, 140, 80, 10);
    green      = SDL_MapRGB(screen->format, 0, 255, 0);
    dark_green = SDL_MapRGB(screen->format, 0, 120, 0);
    yellow     = SDL_MapRGB(screen->format, 216, 255, 0);
    brown      = SDL_MapRGB(screen->format, 120, 140, 0);
    grey       = SDL_MapRGB(screen->format, 180, 180, 180);
    dark_grey  = SDL_MapRGB(screen->format, 100, 100, 100);
    mapcolor   = SDL_MapRGB(screen->format, 10, 10, 100);
}

void Navigation::LoadWidgets()
{
    mapconsole.load("images/Mapscreen.png");
    centerbuttonup.load("images/centerbuttonup.png");
    centerbuttondown.load("images/centerbuttondown.png");
    upbuttonup.load("images/upbuttonup.png");
    upbuttondown.load("images/upbuttondown.png");
    downbuttonup.load("images/downbuttonup.png");
    downbuttondown.load("images/downbuttondown.png");
    leftbuttonup.load("images/leftbuttonup.png");
    leftbuttondown.load("images/leftbuttondown.png");
    rightbuttonup.load("images/rightbuttonup.png");
    rightbuttondown.load("images/rightbuttondown.png");
    plusbuttonup.load("images/plusbuttonup.png");
    plusbuttondown.load("images/plusbuttondown.png");
    minusbuttonup.load("images/minusbuttonup.png");
    minusbuttondown.load("images/minusbuttondown.png");
}

void Navigation::UnLoadWidgets()
{
    // free the surfaces
    mapconsole.unload();
    centerbuttonup.unload();
    centerbuttondown.unload();
    upbuttonup.unload();
    upbuttondown.unload();
    downbuttonup.unload();
    downbuttondown.unload();
    leftbuttonup.unload();
    leftbuttondown.unload();
    rightbuttonup.unload();
    rightbuttondown.unload();
    plusbuttonup.unload();
    plusbuttondown.unload();
    minusbuttonup.unload();
    minusbuttondown.unload();
}

void Navigation::DisplayWidgets()
{
    mapconsole.draw(screen, 0, 0);
    mapcenter ? centerbuttondown.draw(screen, 225, 269) : centerbuttonup.draw(screen, 225, 269);
    upbuttonup.draw(screen, 225, 219);
    downbuttonup.draw(screen, 225, 319);
    leftbuttonup.draw(screen, 175, 269);
    rightbuttonup.draw(screen, 275, 269);
    plusbuttonup.draw(screen, 175, 359);
    minusbuttonup.draw(screen, 275, 359);
}

void Navigation::MapIcon(int x, int y, int ShipType, int Friend, Uint32 color)
{
    // Display NTDS symbols according to the following Legend
    // Hostile:  Submarine = Red Bottom Half Diamond, Surface = Red Full Diamond,
    //           Aircraft = Red Top Half Diamond

    // Friendly: Submarine = green Bottom SemiCircle, Surface = green Circle,
    //           Aircraft = green Top SemiCircle

    // Unknown:  Submarine = grey Bottom Half Box, Surface = grey Box,
    //           Aircraft = grey Top Half Box

    // Neutral:  Submarine = Yellow Bottom Half box, Surface = Yellow box,
    //           Aircraft = Yellow Top Half box

    // Is it a submarine?
    if (ShipType == TYPE_SUB) {
        switch (Friend) {
            case 0:  // Foe??
                DrawDiamond(screen, x - 2, y + 2, 7, 'B', black);  // Bottom Half Diamond
                DrawDiamond(screen, x, y, 7, 'B', color);  // Bottom Half Diamond
                break;
            case 1:  // Friend??
                DrawArc(screen, x - 2, y + 2, 7, 180, 360, black);  // Bottom SemiCircle
                DrawArc(screen, x - 2, y + 3, 7, 180, 360, black);  // thickness
                DrawArc(screen, x, y, 7, 180, 360, color);  // Bottom SemiCircle
                DrawArc(screen, x, y + 1, 7, 180, 360, color);  // thickness
                break;
            case 2:  // Unknown??
                DrawBox(screen, x - 2, y + 2, 7, 'B', black);  // Bottom Half Box
                DrawBox(screen, x, y, 7, 'B', color);  // Bottom Half Box
                break;
            default:  // Neutral
                DrawBox(screen, x - 2, y + 2, 7, 'B', black);  // Bottom Half Box
                DrawBox(screen, x, y, 7, 'B', color);  // Bottom Half Box
        }
    }

    // Is it a surface ship?
    else if (ShipType == TYPE_SHIP) {
        switch (Friend) {
            case 0:  // Foe??
                DrawDiamond(screen, x - 2, y + 2, 7, 'F', black);  // Diamond
                DrawDiamond(screen, x, y, 7, 'F', color);  // Diamond
                break;
            case 1:  // Friend??
                DrawCircle(screen, x - 2, y + 2, 7, black);  // Circle
                DrawCircle(screen, x, y, 7, color);  // Circle
                break;
            case 2:  // Unknown??
                DrawBox(screen, x - 2, y + 2, 7, 'F', black);  // Box
                DrawBox(screen, x, y, 7, 'F', color);  // Box
                break;
            default:  // Neutral
                DrawBox(screen, x - 2, y + 2, 7, 'F', black);  // Cross
                DrawBox(screen, x, y, 7, 'F', color);  // Cross
        }
    }

    // Is it an airplane or Helo?
    else if (ShipType == TYPE_HELICOPTER) {
        switch (Friend) {
            case 0:  // Foe??
                DrawDiamond(screen, x, y, 7, 'T', color);  // Top Half Diamond
                break;
            case 1:  // Friend??
                DrawArc(screen, x, y, 7, 0, 180, color);  // Top SemiCircle
                break;
            case 2:  // Unknown??
                DrawBox(screen, x, y, 7, 'T', color);  // Top Half Box
                break;
            default:  // Neutral
                DrawBox(screen, x, y, 7, 'T', color);  // Top Half Cross
        }
    } else if (ShipType == TYPE_TORPEDO) {
        DrawDiamond(screen, x, y, 7, 'B', color);
        DrawLine(screen, x, y - 5, x, y, color);
    } else if (ShipType == TYPE_NOISEMAKER) {
        DrawBox(screen, x, y, 7, 'B', color);
    }
}

void Navigation::DirectionalPointer(int x, int y, int heading, int speed, Uint32 color)
{
    // Draws a directional vane on a ships map icon.
    // the length will vary depending on the speed.

    double var1;  // just a place to hold a value so we don't have to calc it twice.
    double destinationx; // the dest x & y point for
    double destinationy; // the drawn line.

    if ((speed > 1) || (speed < -1)) speed = speed / 2;  // allow for 0 speed.
    var1 = heading * 0.017453;  // convert degrees to radians.
    destinationx = (sin(var1) * speed) + x;
    destinationy = (-1) * (cos(var1) * speed) + y;
    DrawLine(screen, x, y, (int) destinationx, (int) destinationy, color);
}

void Navigation::PlaceShips(Submarine *current_target)
{
    // Places all Ships onto the map. Soon to change, so only registered contacts appear.
    // scale is in YDS per pixel
    int fresh;
    int x, y;  // where to place the ships
    int xoffset = 374;  // offsets to move the ships to
    int yoffset = 145;  // the defined place on map screen
    Uint32 color;
    Submarine *a_torp;
    Submarine *target_ship;
    Helicopter *a_helicopter;
    int layers;

    int scale = mapscale * MAP_FACTOR;
    if (mapcenter) {  // center map onto our own ntds symbol
        scrolloffsetx = 250 - ((int) Subs->Lat_TotalYards / scale);
        scrolloffsety = 250 - ((int) Subs->Lon_TotalYards / scale);
    }

    target_ship = Subs;
    #ifdef DEBUG
    printf("About to place ships on map.\n");
    #endif
    while (target_ship) {
        fresh = Subs->Can_Detect(target_ship);
        #ifdef DEBUG
        printf("Freshness factor %d\n", fresh);
        #endif
        if ((fresh) || (target_ship == Subs)) {
            x = 500 - ((int) target_ship->Lat_TotalYards / scale);
            x = x - scrolloffsetx;
            y = 500 - ((int) target_ship->Lon_TotalYards / scale);
            y = y - scrolloffsety;
            if (x > 10 && x < 490 && y > 10 && y < 490) {  // are we going to fall off the damn map???
                x = x + xoffset;
                y = y + yoffset;
                // not only do we need friend/foe, but
                // also fresh/old contact
                // fresh = Subs->Can_Detect(target_ship);
                switch (target_ship->Friend) {
                    case 0:  // Foe??
                        if (fresh >= CONTACT_SOLID)
                            color = red;
                        else
                            color = dark_red;
                        break;
                    case 1:  // Friend??
                        if (fresh >= CONTACT_SOLID)
                            color = green;
                        else
                            color = dark_green;
                        break;
                    case 2:  // Neither???
                        if (fresh >= CONTACT_SOLID)
                            color = yellow;
                        else
                            color = brown;
                        break;
                    default:  // Unknown
                        if (fresh >= CONTACT_SOLID)
                            color = grey;
                        else
                            color = dark_grey;
                        break;
                }
                if (target_ship == Subs) {  // Is it me???
                    color = green;
                }
                #ifdef DEBUG
                printf("Deciding if to draw clearly.\n");
                #endif
                if ((fresh >= CONTACT_WEAK) || (Subs == target_ship)) {
                    MapIcon(x, y, (int) target_ship->ShipType, (int) target_ship->Friend, color);  // Draw the NTDS symbol.
                    // check to see if we should highlight
                    if (current_target == target_ship)
                        MapIcon(x, y + 1, (int) target_ship->ShipType, (int) target_ship->Friend, color);
                    DirectionalPointer(x - 2, y + 2, (int) target_ship->Heading, (int) target_ship->Speed, black);
                    DirectionalPointer(x, y, (int) target_ship->Heading, (int) target_ship->Speed, color);
                    // Add pointer the show heading.
                }  // end of we can hear you ok
            }  // end of we are on the map
        }  // end of able to detect
        target_ship = target_ship->next;
    }

    // now place torpedoes
    #ifdef DEBUG
    printf("About to draw torpedoes on map\n");
    #endif
    a_torp = torpedoes;
    while (a_torp) {
        x = 500 - ((int) a_torp->Lat_TotalYards / scale);
        x = x - scrolloffsetx;
        y = 500 - ((int) a_torp->Lon_TotalYards / scale);
        y = y - scrolloffsety;
        if (x > 10 && x < 490 && y > 10 && y < 490) {  // are we going to fall off map??
            x = x + xoffset;
            y = y + yoffset;
            switch (a_torp->Friend) {
                case 0:  // Foe??
                    color = red;
                    break;
                case 1:  // Friend??
                    color = green;
                    break;
                case 2:  // Neither???
                    color = yellow;
                    break;
                default:  // Unknown
                    color = grey;
                    break;
            }
            MapIcon(x, y, (int) a_torp->ShipType, (int) a_torp->Friend, color);
        }  // within map limits

        a_torp = a_torp->next;
    }  // end of displaying torpedoes

    #ifdef DEBUG
    printf("About to draw helicopters on map.\n");
    #endif
    // we only draw helicopters if we are near the surface
    // and moving slow. This is crude, but it'll do for now
    Submarine *player = Subs;  // first sub is the player
    layers = my_map->Thermals_Between(0, player->Depth);
    if ((layers < 1) && (player->Speed < (player->MaxSpeed / 2))) {
        a_helicopter = helicopters;
        while (a_helicopter) {
            x = 500 - ((int) a_helicopter->Lat_TotalYards / scale);
            x = x - scrolloffsetx;
            y = 500 - ((int) a_helicopter->Lon_TotalYards / scale);
            y = y - scrolloffsety;
            if (x > 10 && x < 490 && y > 10 && y < 490) {  // are we on the map
                x = x + xoffset;
                y = y + yoffset;
                switch (a_helicopter->Friend) {
                    case FOE:
                        color = red;
                        break;
                    case FRIEND:
                        color = green;
                        break;
                    case NEUTRAL:
                        color = grey;
                        break;
                    case UNKNOWN:
                    default:
                        color = yellow;
                        break;
                }  // end of friend/foe switch
                MapIcon(x, y, a_helicopter->ShipType,
                        a_helicopter->Friend, color);
            }  // within map limits
            a_helicopter = a_helicopter->next;
        }  // end of drawing helicopters
    }  // end of if near surface
}

// Future home of an actual map display routine, right now just a blank field with dots
void Navigation::DrawMap(Submarine *current_target)
{
    // define a rectangle on the screen and make it blue
    SDL_Rect rectangle;
    rectangle.x = 374;
    rectangle.y = 145;
    rectangle.h = 500;
    rectangle.w = 501;
    SDL_FillRect(screen, &rectangle, mapcolor);

    // Make meaningless dots grid on "Map"
    for (int x = 374; x < 874; x += 10) {
        for (int y = 145; y < 637; y += 10) {
            DrawPixel(screen, x + 5, y + 5, white);
        }
    }

    // Place ships
    #ifdef DEBUG
    printf("About to place ships.\n");
    #endif
    PlaceShips(current_target);
}

// Show information on the currently selected target
void Navigation::Display_Target(Submarine *current_target)
{
    char buffer[256];
    SDL_Rect rectangle;
    static DFont fnt("images/font.png", "data/font.dat");
    float range;
    double bearing;

    // if (!current_target)
    //     return;

    // make empty box to the side of the screen
    rectangle.x = 120;
    rectangle.y = 400;
    rectangle.w = 200;
    rectangle.h = 160;
    SDL_FillRect(screen, &rectangle, black);

    if (current_target) {
        // fill in data
        sprintf(buffer, "   Target");
        fnt.PutString(screen, 150, 400, buffer);
        sprintf(buffer, "Heading: %d", (int) current_target->Heading);
        fnt.PutString(screen, 140, 424, buffer);

        sprintf(buffer, "Speed: %d knots", (int) current_target->Speed);
        fnt.PutString(screen, 140, 436, buffer);

        range = Subs->DistanceToTarget(current_target);
        // range *= 0.000568;
        range *= YARDS_TO_MILES;
        sprintf(buffer, "Range: %2.1f miles", range);
        fnt.PutString(screen, 140, 448, buffer);

        bearing = Subs->BearingToTarget(current_target);
        sprintf(buffer, "Bearing: %2.0lf", bearing);
        fnt.PutString(screen, 140, 460, buffer);

        sprintf(buffer, "Depth: %d feet", (int) current_target->Depth);
        fnt.PutString(screen, 140, 472, buffer);

        sprintf(buffer, "Type: %s ", current_target->ClassName);
        if (current_target->ClassType[0])
            strcat(buffer, current_target->ClassType);
        fnt.PutString(screen, 140, 484, buffer);
    }  // end of valid target
}

void Navigation::UpdateDisplay(Submarine *current_target)
{
    DisplayWidgets();
    DrawMap(current_target);  // fix me to do something useful!
    my_map->Draw_Depth_Meter(Subs->Depth, SCREEN_NAV);
    Display_Target(current_target);
}

void Navigation::ToggleMapCenter()
{
    if (mapcenter) {
        mapcenter = 0;
    } else {
        mapcenter = 1;
    }
}

void Navigation::IncreaseMapScale()
{
    mapscale++;
    if (mapscale > MAX_MAP_SCALE) mapscale = MAX_MAP_SCALE;
    plusbuttondown.draw(screen, 175, 359, true);
    SDL_Delay(100);
}

void Navigation::DecreaseMapScale()
{
    mapscale--;
    if (mapscale < 1) mapscale = 1;
    minusbuttondown.draw(screen, 275, 359, true);
    SDL_Delay(100);
}

void Navigation::ScrollMapUp(int change_scrolly)
{
    scrolloffsety -= change_scrolly;
    upbuttondown.draw(screen, 225, 219, true);
    SDL_Delay(100);
}

void Navigation::ScrollMapDown(int change_scrolly)
{
    scrolloffsety += change_scrolly;
    downbuttondown.draw(screen, 225, 319, true);
    SDL_Delay(100);
}

void Navigation::ScrollMapLeft(int change_scrollx)
{
    scrolloffsetx -= change_scrollx;
    leftbuttondown.draw(screen, 175, 269, true);
    SDL_Delay(100);
}

void Navigation::ScrollMapRight(int change_scrollx)
{
    scrolloffsetx += change_scrollx;
    rightbuttondown.draw(screen, 275, 269, true);
    SDL_Delay(100);
}
