/***************************************************************************
                          radar.cpp  -  description
                             -------------------
    begin                : Fri August 2 2001
    copyright            : (C) 2001 by Rick McDaniel
    email                : rickeym@swbell.net
$Id: radar.cpp,v 1.16 2003/05/17 22:25:34 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#include <string>
#include "math.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_thread.h"
#include "dfont.h"
#include "draw.h"
#include "files.h"
#include "submarine.h"

// VENZON: need rotozoom now
#include "SDL_rotozoom.h"

// rdm windows
#include <iostream>
using namespace std;

#include "radar.h"

// VENZON: I've replaced all occurrences of 10*tick with tick, since I upped
// the resolution by a factor of 10

Radar::Radar(Submarine *temp): Subs(temp)
{
    // Default values: Mast down, RangeRing off, RangeScale = 10

    MastHeight = 59;  // Max height of radar mast
    Mast = false;
    RangeRing = 0;
    RangeScale = 10;
    rangescale10 = true;
    rangescale20 = false;
    rangescale30 = false;
    rangescale40 = false;
    rangescale50 = false;
    rangescale60 = false;

    rangering0 = true;
    rangering10 = false;
    rangering15 = false;
    rangering20 = false;
    rangering25 = false;

    // radar sweep counter
    tick = 0;
    ALPHA = 255;
}

Radar::~Radar()
{
}

void Radar::InitGraphics(SDL_Surface *temp, SDL_Surface *tempradarscreen)
{
    screen = temp;
    radarscreen = tempradarscreen;

    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 420, 490, 32,
                                screen->format->Rmask,
                                screen->format->Gmask,
                                screen->format->Bmask,
                                screen->format->Amask);
    tempscreen = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    LoadWidgets();
    DisplayWidgets();

    orange = SDL_MapRGB(screen->format, 238, 118, 0);
}

void Radar::LoadWidgets()
{
    // Load Range Scale Widgets
    range10off = Load_Image("images/range10off.png");
    range10on = Load_Image("images/range10on.png");
    range20off = Load_Image("images/range20off.png");
    range20on = Load_Image("images/range20on.png");
    range30off = Load_Image("images/range30off.png");
    range30on = Load_Image("images/range30on.png");
    range40off = Load_Image("images/range40off.png");
    range40on = Load_Image("images/range40on.png");
    range50off = Load_Image("images/range50off.png");
    range50on = Load_Image("images/range50on.png");
    range60off = Load_Image("images/range60off.png");
    range60on = Load_Image("images/range60on.png");

    // Load Range Ring Widgets
    ring0off = Load_Image("images/ring0off.png");
    ring0on = Load_Image("images/ring0on.png");
    ring5off = Load_Image("images/ring5off.png");
    ring5on = Load_Image("images/ring5on.png");
    ring10off = Load_Image("images/ring10off.png");
    ring10on = Load_Image("images/ring10on.png");
    ring15off = Load_Image("images/ring15off.png");
    ring15on = Load_Image("images/ring15on.png");
    ring20off = Load_Image("images/ring20off.png");
    ring20on = Load_Image("images/ring20on.png");
    ring25off = Load_Image("images/ring25off.png");
    ring25on = Load_Image("images/ring25on.png");

    // Load Mast Widgets
    mastdownoff = Load_Image("images/mastdownoff.png");
    mastdownon = Load_Image("images/mastdownon.png");
    mastupoff = Load_Image("images/mastupoff.png");
    mastupon = Load_Image("images/mastupon.png");

    // VENZON: only need 1 sweep image now
    sweep[0] = Load_Image("images/sweep0.png");

    // Load the picture of a blip
    blip = Load_Image("images/blip.png");
    SDL_SetAlpha(blip, SDL_SRCALPHA, 128);

    // Clear Widgets
    ClearRadar = Load_Image("images/ClearRadar.png");
    ClearRadar2 = Load_Image("images/ClearRadar2.png");
}

void Radar::UnLoadWidgets()
{
    // free the surfaces
    SDL_FreeSurface(range10off);
    SDL_FreeSurface(range10on);
    SDL_FreeSurface(range20off);
    SDL_FreeSurface(range20on);
    SDL_FreeSurface(range30off);
    SDL_FreeSurface(range30on);
    SDL_FreeSurface(range40off);
    SDL_FreeSurface(range40on);
    SDL_FreeSurface(range50off);
    SDL_FreeSurface(range50on);
    SDL_FreeSurface(range60off);
    SDL_FreeSurface(range60on);

    SDL_FreeSurface(ring0off);
    SDL_FreeSurface(ring0on);
    SDL_FreeSurface(ring5off);
    SDL_FreeSurface(ring5on);
    SDL_FreeSurface(ring10off);
    SDL_FreeSurface(ring10on);
    SDL_FreeSurface(ring15off);
    SDL_FreeSurface(ring15on);
    SDL_FreeSurface(ring20off);
    SDL_FreeSurface(ring20on);
    SDL_FreeSurface(ring25off);
    SDL_FreeSurface(ring25on);

    SDL_FreeSurface(mastdownoff);
    SDL_FreeSurface(mastdownon);
    SDL_FreeSurface(mastupoff);
    SDL_FreeSurface(mastupon);

    SDL_FreeSurface(sweep[0]);

    SDL_FreeSurface(blip);

    SDL_FreeSurface(ClearRadar);
    SDL_FreeSurface(ClearRadar2);
}

void Radar::DisplayWidget(SDL_Surface *dest, int x, int y, SDL_Surface *source)
{
    SDL_Rect rect;

    // Blit destination x & y to the upper left
    rect.x = x;
    rect.y = y;
    // Height and width equal to the source images...
    rect.h = source->h;
    rect.w = source->w;
    // Do the actual blit
    SDL_BlitSurface(source, NULL, dest, &rect);
    // Show the screen...
    SDL_UpdateRects(dest, 1, &rect);
}

void Radar::DisplayWidgets()
{
    // Range Scale Widgets
    DisplayWidget(screen, 748, 320, rangescale10 ? range10on : range10off);
    DisplayWidget(screen, 795, 320, rangescale20 ? range20on : range20off);
    DisplayWidget(screen, 842, 320, rangescale30 ? range30on : range30off);
    DisplayWidget(screen, 748, 367, rangescale40 ? range40on : range40off);
    DisplayWidget(screen, 795, 367, rangescale50 ? range50on : range50off);
    DisplayWidget(screen, 842, 367, rangescale60 ? range60on : range60off);

    // Mast Widgets
    DisplayWidget(screen, 850, 185, Mast ? mastupon : mastupoff);
    DisplayWidget(screen, 744, 185, Mast ? mastdownoff : mastdownon);

    // Range Ring Widgets
    DisplayWidget(screen, 749, 501, rangering0 ? ring0on : ring0off);
    DisplayWidget(screen, 796, 501, rangering5 ? ring5on : ring5off);
    DisplayWidget(screen, 843, 501, rangering10 ? ring10on : ring10off);
    DisplayWidget(screen, 749, 548, rangering15 ? ring15on : ring15off);
    DisplayWidget(screen, 796, 548, rangering20 ? ring20on : ring20off);
    DisplayWidget(screen, 843, 548, rangering25 ? ring25on : ring25off);

    // Need this for the time compression??
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Radar::ClearScreen()
{
    // Clear the whole radar area (scope + text boxes) on the screen
    DisplayWidget(screen, 98, 154, ClearRadar);

    DisplayWidgets();
}

void Radar::ClearTextBox()
{
    // Clear the text boxes area on the screen
    DisplayWidget(screen, 139, 572, ClearRadar2);

    DisplayWidgets();
}

void Radar::Sweep(float gametime)
{
    if (getMastStatus()) {  // If the mast is up start sweep
        tick += gametime / 25.0f;

        if (tick > 360) tick -= 360;
    }
}

void Radar::DisplaySweep()
{
    SDL_Rect dest;

    if (getMastStatus()) {  // If the mast is up start sweep

        dest.x = 193;
        dest.y = 245;
        dest.w = sweep[0]->w;
        dest.h = sweep[0]->h;

        // VENZON: some rotozooming!
        SDL_SetAlpha(sweep[0], SDL_SRCALPHA, 128);
        SDL_FillRect(screen, &dest, black);
        SDL_Surface *sweeprot;

        // set the last argument of this function to 0 for a bit faster
        // (but much crappier looking) rotozooming
        sweeprot = rotozoomSurface(sweep[0], -tick, 1.0, 1);

        // I'm not sure what this alpha stuff is for, but hopefully
        // the rotozoomer won't mess it up
        SDL_SetAlpha(sweeprot, SDL_SRCALPHA, 128);

        // the rotozoomer seems to create a surface with whatever width and
        // height necessary, so we have to recompute the destination coords
        DisplayWidget(screen,
                      sweep[0]->w / 2 + dest.x - sweeprot->w / 2,
                      sweep[0]->h / 2 + dest.y - sweeprot->h / 2,
                      sweeprot);
        SDL_FreeSurface(sweeprot);
    }
}

void Radar::DisplayRings()
{
    int number;

    if (RangeRing == 0) {
        number = 0;
    } else {
        number = int(float(RangeScale) / float(RangeRing));
    }

    for (int i = 0; i < number; i++) {
        DrawCircle(screen, 320, 371, int((144.0 / getRangeScale())*RangeRing * (i + 1)), orange);
    }
}

void Radar::DisplayContacts()
{
    float radians;
    int bearing, range, depth;
    Submarine *target;
    SDL_Rect dest;

    DisplaySweep();  // Start the radar sweep

    DisplayRings();  // Draw Range Rings

    // Note: Center of radar screen @ (x,y) = (316,374)
    // set dx = 144   dy = 144

    target = Subs->next;
    while (target) {
        bearing = (int) Subs->BearingToTarget(target);
        range = (int) Subs->DistanceToTarget(target);
        depth = (int) Subs->Depth;

        radians = float(bearing) * (3.14 / 180.0);  // degrees to radians

        if (isTargetVisible(target, range, depth, 100, 3)) {
            // Find where we plot the dot
            x = 316 + int((460 - 316) * (range / float(1000 * RangeScale)) * cos(1.57 - radians));
            y = 374 - int((374 - 230) * (range / float(1000 * RangeScale)) * sin(1.57 - radians));

            dest.x = x;
            dest.y = y;
            dest.w = blip->w;
            dest.h = blip->h;

            // Here's where we set the alpha level

            if (bearing <= 180) {
                if (tick >= bearing && tick <= ReciprocalBearing(bearing)) {
                    ALPHA = int(-(255.0 / 180.0) * float(DeltaBearing((int) tick, bearing)) + 255.0);
                } else {

                    ALPHA = 0;
                }
            }

            if (bearing >= 180) {
                if (tick <= bearing && tick >= ReciprocalBearing(bearing)) {
                    ALPHA = 0;
                } else {

                    ALPHA = int(-(255.0 / 180.0) * float(DeltaBearing((int) tick, bearing)) + 255.0);
                }
            }

            SDL_SetAlpha(blip, SDL_SRCALPHA, ALPHA);
            SDL_FillRect(screen, &dest, black);

            SDL_BlitSurface(blip, NULL, screen, &dest);  // Do the actual blit

        }  // if visible
        target = target->next;
    }  // end of while
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Radar::ClearRangeScale()
{
    rangescale10 = false;
    rangescale20 = false;
    rangescale30 = false;
    rangescale40 = false;
    rangescale50 = false;
    rangescale60 = false;
    ClearScreen();
}

void Radar::ClearRangeRing()
{
    rangering0 = false;
    rangering5 = false;
    rangering10 = false;
    rangering15 = false;
    rangering20 = false;
    rangering25 = false;
    ClearScreen();
}

// Toggle Range Scale
void Radar::ToggleRangeScale10()
{
    ClearRangeScale();
    rangescale10 = !rangescale10;
    setRangeScale(10);
}

void Radar::ToggleRangeScale20()
{
    ClearRangeScale();
    rangescale20 = !rangescale20;
    setRangeScale(20);
}

void Radar::ToggleRangeScale30()
{
    ClearRangeScale();
    rangescale30 = !rangescale30;
    setRangeScale(30);
}

void Radar::ToggleRangeScale40()
{
    ClearRangeScale();
    rangescale40 = !rangescale40;
    setRangeScale(40);
}

void Radar::ToggleRangeScale50()
{
    ClearRangeScale();
    rangescale50 = !rangescale50;
    setRangeScale(50);
}

void Radar::ToggleRangeScale60()
{
    ClearRangeScale();
    rangescale60 = !rangescale60;
    setRangeScale(60);
}

// Toggle Range Rings
void Radar::ToggleRangeRing0()
{
    ClearRangeRing();
    rangering0 = !rangering0;
    setRangeRing(0);
}

void Radar::ToggleRangeRing5()
{
    ClearRangeRing();
    rangering5 = !rangering5;
    setRangeRing(5);
}

void Radar::ToggleRangeRing10()
{
    ClearRangeRing();
    rangering10 = !rangering10;
    setRangeRing(10);
}

void Radar::ToggleRangeRing15()
{
    ClearRangeRing();
    rangering15 = !rangering15;
    setRangeRing(15);
}

void Radar::ToggleRangeRing20()
{
    ClearRangeRing();
    rangering20 = !rangering20;
    setRangeRing(20);
}

void Radar::ToggleRangeRing25()
{
    ClearRangeRing();
    rangering25 = !rangering25;
    setRangeRing(25);
}

void Radar::RaiseMast()
{
    Mast = true;
}

void Radar::LowerMast()
{
    Mast = false;
}

void Radar::ToggleMast()
{
    // ClearScreen();
    Mast = !Mast;
}

void Radar::setRangeScale(int RS)
{
    // Set Radar RangeScale
    RangeScale = (RS == 10 || RS == 20 || RS == 30 ||
                  RS == 40 || RS == 50 || RS == 60) ? RS : 10;
}

void Radar::setRangeRing(int RR)
{
    // Set Radar RangeRing
    RangeRing = (RR == 0 || RR == 5 || RR == 10 ||
                 RR == 15 || RR == 20 || RR == 25) ? RR : 0;
}

int Radar::getRangeScale() const
{
    return RangeScale;
}

int Radar::getRangeRing() const
{
    return RangeRing;
}

bool Radar::getMastStatus()const
{
    return Mast;
}

int Radar::getAntennaHeight(int Depth, int SeaState) const
{
    // Effective Height of radar mast above water.
    // Note at SeaState > 5, must be on the surface to use radar
    // Also at SeaState > 5, effective length on mast = 1 ft.
    switch (SeaState) {
        case 0:
        case 1:
            return (MastHeight - Depth);
        case 2:
            return (MastHeight - Depth - 1);
        case 3:
            return (MastHeight - Depth - 3);
        case 4:
            return (MastHeight - Depth - 5);
        case 5:
            return (MastHeight - Depth - 8);
        default:
            return (MastHeight - Depth - 58);
    }
}

bool Radar::isTargetVisible(Submarine *target, int TargetRange,
                            int ObserverDepth,
                            int TargetHeight, int SeaState)
{
    // Determines if observer's radar can detect target.
    // Returns a 1 if true or a 0 if false.

    // TargetRange is the true range to the target.

    // TargetHeight is the height of the tallest mast on a surface ship
    // (usually 50 ft - 175 ft), height of conning tower for a submarine
    // (usually 40-50 ft), or altitude for airplane.

    /* To see Target, Mast must be up, Range < RadarHorizon, Range < RangeScale.

       Radar horizon is the earliest posssible target detection due to
       the Earth-Curvature. Radar horizon = 2400(sqrt(ha)+ sqrt(ht))
       ha = antenna height(ft), ht = target height(ft), RadarHorizon(yds)

       When sea state (SS) <= 1, Depth should be < 58 ft or mast will be below
       water.  At SS = 2, depth <= 57 ft.  At SS = 3, depth <= 55 ft.
       At SS = 4, depth <= 53 ft.   At SS = 5, depth <= 50 ft.
    */

    bool boolean = false;

    // if target is under water we cannot see them
    if (target->Depth > 0.0)
        return false;

    CurrentAntennaHeight = getAntennaHeight(ObserverDepth, SeaState);
    if (CurrentAntennaHeight <= 0) return boolean;  // Mast is below water!

    RadarHorizon = 2400 * (int) (sqrt(float(CurrentAntennaHeight)) + sqrt(float(TargetHeight)));

    if (target->Depth <= 0 && int(TargetRange) <= 1000 * getRangeScale() && getMastStatus() == 1 &&
            TargetRange <= RadarHorizon) boolean = true;
    return boolean;
}

float Radar::getRadarHorizon(int ObserverDepth, int TargetHeight, int SeaState)
{
    /* Radar horizon is the earliest possible target detection due to
       the Earth-Curvature. Radar horizon = 2400(sqrt(ha)+ sqrt(ht))
       ha = antenna height(ft), ht = target height(ft), RadarHorizon(yds)

       When sea state (SS) <= 1, Depth should be < 58 ft or mast will be below
       water.  At SS = 2, depth <= 57 ft.  At SS = 3, depth <= 55 ft.
       At SS = 4, depth <= 53 ft.   At SS = 5, depth <= 50 ft.
    */

    CurrentAntennaHeight = getAntennaHeight(ObserverDepth, SeaState);
    if (CurrentAntennaHeight <= 0) return 0;  // Mast is below water!

    return (2400.0 * (sqrt(float(CurrentAntennaHeight)) + sqrt(float(TargetHeight))));
}

int Radar::getBearing(int TargetBearing)
{
    // Returns the bearing to the target.
    bearing = TargetBearing;
    return bearing;
}

int Radar::getRange(int TargetRange)
{
    // Returns the range to the target.
    range = TargetRange;
    return range;
}

void Radar::ShowData(SDL_Surface *screen, int x, int y)
{
    static char text[120];
    // string file1 = "font.png";
    // string file2 = "font.dat";
    char filename[] = "images/largefont.png";
    char filename2[] = "data/largefont.dat";
    static DFont largeFont(filename, filename2);

    double c1, c2, c3, c4, c5, c6;
    int bearing;
    int range;

    c1 = double(316);
    c2 = double((460 - 316)) / double(RangeScale);
    c3 = double(374);
    c4 = double((374 - 230)) / double(RangeScale);

    c5 = c2 * (double(y) - c3);
    c6 = c4 * (double(x) - c1);

    ClearTextBox();

    if (x >= 316) {
        bearing = int(90.0 + (180 / 3.14) * atan(c5 / c6));
    } else {
        bearing = int(270.0 + (180 / 3.14) * atan(c5 / c6));
    }

    range = int(1000.0 * sqrt(pow(((double(x) - c1) / c2), 2) + pow(((double(y) - c3) / c4), 2)));

    sprintf(text, "  %i  ", bearing);
    largeFont.PutString(screen, 170, 615, text);

    sprintf(text, "  %i  ", range);
    largeFont.PutString(screen, 370, 615, text);

    return;
}

int Radar::DeltaBearing(int bearing1, int bearing2)
{
    // Difference between two bearings
    int deltabearing;
    // for POSIX systems
#ifndef WIN32
    if (fabs(bearing1 - bearing2) < 180) {
        deltabearing = (int) fabs(bearing1 - bearing2);
    } else {
        deltabearing = (int) (360 - fabs(bearing1 - bearing2));
    }
    // WIN32 prefers abs when dealing with int
#else
    if (abs(bearing1 - bearing2) < 180) {
        deltabearing = (int) abs(bearing1 - bearing2);
    } else {
        deltabearing = (int) (360 - abs(bearing1 - bearing2));
    }
#endif
    return deltabearing;
}

int  Radar::ReciprocalBearing(int bearing)
{
    // returns the 180 degree opposite of the bearing given to it...
    int recipbearing;
    if (bearing >= 180) {
        recipbearing = bearing - 180;
    } else {
        recipbearing = bearing + 180;
    }
    return recipbearing;
}
