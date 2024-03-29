/***************************************************************************
                         esm.cpp  -  description
                            -------------------
   begin                : Wed August 15 2001
   copyright            : (C) 2001 by Rick McDaniel
   email                : rickeym@swbell.net
$Id: esm.cpp,v 1.5 2003/04/14 05:51:03 mbridak Exp $
***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#include "esm.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include "dfont.h"
#include "draw.h"
#include "files.h"

// rdm windows
#include <iostream>

Esm::Esm()
{
    // Default values: mast down, power off
    MastHeight = 68;  // Max height of esm mast
    Mast = false;
}

Esm::~Esm()
{
}

void Esm::setSubs(Submarine *Subs)
{
    this->Subs = Subs;
}

void Esm::InitGraphics(SDL_Surface *screen)
{
    this->screen = screen;

    orange = SDL_MapRGB(screen->format, 238, 118, 0);
}

void Esm::LoadWidgets()
{
    esmconsole.load("images/ESMScreen.png");
    mastdownoff.load("images/mastdownoff.png");
    mastdownon.load("images/mastdownon.png");
    mastupoff.load("images/mastupoff.png");
    mastupon.load("images/mastupon.png");
    styllus.load("images/styllus.png");
}

void Esm::UnLoadWidgets()
{
    // free the surfaces
    esmconsole.unload();
    mastdownoff.unload();
    mastdownon.unload();
    mastupoff.unload();
    mastupon.unload();
    styllus.unload();
}

void Esm::DisplayWidgets()
{
    // ESM console
    esmconsole.draw(screen, 0, 0);

    // Center of ESM screen at (316,325)
    // styllus.draw(screen, 316, 325);

    // Mast up/down buttons
    Mast ? mastupon.draw(screen, 845, 187) : mastupoff.draw(screen, 845, 187);
    Mast ? mastdownoff.draw(screen, 740, 187) : mastdownon.draw(screen, 740, 187);
}

void Esm::LowerMast()
{
    Mast = false;
}

void Esm::RaiseMast()
{
    Mast = true;
}

void Esm::ToggleMast()
{
    Mast = !Mast;
}

bool Esm::getMastStatus()const
{
    return Mast;
}

int Esm::getAntennaHeight(int Depth, int SeaState) const
{
    // Effective height of esm mast above water.
    switch (SeaState) {
        case 0: // case 0 and case 1 are same
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
            return (MastHeight - Depth - 67);
    }
}

bool Esm::isTargetVisible(Submarine *target, int TargetRange, int ObserverDepth,
                          int TargetHeight, bool TargetRadar, int SeaState)
{
    // Determines if observer's esm can detect target radar.
    // Returns a 1 if true or a 0 if false.

    // TargetRange is the true range to the target.

    // TargetHeight is the height of the tallest mast on a surface ship
    // (usually 50 ft - 175 ft), height of conning tower for a submarine
    // (usually 40-50 ft), or altitude for airplane.

    /* To see Target, Range < EsmHorizon, Targets radar must be on.

       Esm horizon is the earliest posssible target detection due to
       the Earth-Curvature. Esm horizon = 3600(sqrt(ha)+ sqrt(ht))
       ha = antenna height(ft), ht = target height(ft), EsmHorizon(yds)

       Note: ESM horizon > Radar Horizon since can detect targets radar
       long before he can detect esm mast.
    */

    bool boolean = false;
    // we cannot detect subs on esm
    if (target->Depth > 0.0)
        return false;

    CurrentAntennaHeight = getAntennaHeight(ObserverDepth, SeaState);
    if (CurrentAntennaHeight <= 0) return boolean; // Mast is below water!

    if (TargetRadar == 1 && getMastStatus() == 1
            && TargetRange <= getEsmHorizon(ObserverDepth, TargetHeight, SeaState))
        boolean = true;
    return boolean;
}

int Esm::getBearing(int TargetBearing, int ObserverDepth,
                    int TargetHeight, int SeaState)
{
    // Returns the bearing to the target based on accuracy of esm mast.

    // The greater the EsmHorizon, the greater the error.

    EsmHorizon = getEsmHorizon(ObserverDepth, TargetHeight, SeaState);

    if (EsmHorizon > 30000)
        bearing = TargetBearing - 5 + rand() % 10;
    else if (EsmHorizon > 20000 && EsmHorizon < 30000)
        bearing = TargetBearing - 3 + rand() % 6;
    else if (EsmHorizon > 15000 && EsmHorizon < 20000)
        bearing = TargetBearing - 2 + rand() % 4;
    else
        bearing = TargetBearing - 1 + rand() % 2;

    return bearing;
}

int Esm::getSignalStrength(Submarine *Target, int TargetRange, int ObserverDepth,
                           int TargetHeight, bool TargetRadar, int SeaState)
{
    // Returns the signal strength of the target radar.
    // The signal strength is determined by the Detection Probability.

    // The detection probability is a function of Target Range, Antenna
    // Height, and TargetHeight

    if (isTargetVisible(Target, TargetRange, ObserverDepth, TargetHeight,
                        TargetRadar, SeaState) == 0) {
        SignalStrength = 0;
        return SignalStrength;
    } else {
        DetectProb = TargetRange - 600 * getAntennaHeight(ObserverDepth, SeaState)
                     - 20 * TargetHeight;
        if (DetectProb > 35000)
            SignalStrength =  1;
        else if (DetectProb > 25000 && DetectProb < 35000)
            SignalStrength = 2;
        else if (DetectProb > 18000 && DetectProb < 25000)
            SignalStrength = 3;
        else if (DetectProb > 14000 && DetectProb < 18000)
            SignalStrength = 4;
        else
            SignalStrength = 5;
    }
    return SignalStrength;
}

int Esm::getEsmHorizon(int ObserverDepth, int TargetHeight, int SeaState)
{
    CurrentAntennaHeight = getAntennaHeight(ObserverDepth, SeaState);
    if (CurrentAntennaHeight <= 0) return 0; // Mast is below water!

    EsmHorizon = 3600 * (int)(sqrt(float(CurrentAntennaHeight)) +
                              sqrt(float(TargetHeight)));

    return EsmHorizon;
}

void Esm::DisplayContacts()
{
    static char text[120];
    static DFont fnt("images/font.png", "images/font.dat");
    float radians;
    int x, y;
    int bearing, range, depth;
    Submarine *target;
    int count;

    // Note: Center of radar screen at (x,y) = (323,330)
    // set dx = 144 dy = 144

    target = Subs->next;
    count = 0;
    while (target) {
        bearing = (int)Subs->BearingToTarget(target);
        range = (int)Subs->DistanceToTarget(target);
        depth = (int)Subs->Depth;

        radians = float(bearing) * (3.14 / 180.0);  // degrees to radians

        if (isTargetVisible(target, range, depth, 100, true, 3)) {
            count++;
            // Plot a line at the correct bearing
            x = int(326.0 + (460.0 - 326.0) * cos(1.57 - radians));
            y = int(383.0 - (460.0 - 326.0) * sin(1.57 - radians));
            DrawLine(screen, 326, 383, x, y, orange);

            sprintf(text, "BEARING  %i   SS %i", bearing,
                    getSignalStrength(target, range, 60, 100, true, 3));
            fnt.PutString(screen, 698, 300 + 11 * count, text);
        }

        target = target->next;
    }
}

void Esm::UpdateDisplay()
{
    DisplayWidgets();
    DisplayContacts();
}
