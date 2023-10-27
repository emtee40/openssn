/***************************************************************************
                             radar.h  -  description
                             -------------------
    begin                : Fri August 2 2001
    copyright            : (C) 2001 by Rick McDaniel
    email                : rickeym@swbell.net
$Id: radar.h,v 1.8 2003/04/14 05:51:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef RADAR_H
#define RADAR_H

#include "SDL/SDL.h"
#include "submarine.h"
#include "widget.h"

// rdm windows
// #include <iostream>

/** Usage:

1. Make depth < 58 feet (for sea state <= 1).  Must be shallower at higher
   sea states.
2. Raise mast:  radar.RaiseMast()

  *@author Rick McDaniel
*/

class Radar
{
public:
    Radar();
    ~Radar();

    void setSubs(Submarine *Subs);

    /** Initialize the graphic for this class, *screen is the main
        display screen passed in from main. */
    void InitGraphics(SDL_Surface *screen);

    void LoadWidgets();
    void UnLoadWidgets();

    void DisplayWidgets();

    void DisplaySweep();

    void DisplayRings();

    void DisplayContacts();

    void ShowContactData();

    void UpdateDisplay();

    void Sweep(float gametime);

    void ClearRangeScale();

    void ClearRangeRing();

    void selectContact(int x, int y);

    int DeltaBearing(int bearing1, int bearing2);
    int ReciprocalBearing(int bearing1);

    // Toggle Range Scale
    void ToggleRangeScale10();
    void ToggleRangeScale20();
    void ToggleRangeScale30();
    void ToggleRangeScale40();
    void ToggleRangeScale50();
    void ToggleRangeScale60();

    // Toggle Ring Ring
    void ToggleRangeRing0();
    void ToggleRangeRing5();
    void ToggleRangeRing10();
    void ToggleRangeRing15();
    void ToggleRangeRing20();
    void ToggleRangeRing25();

    void ToggleMast();  // Toggle Mast Status
    void RaiseMast();
    void LowerMast();

    // Set functions
    void setRangeScale(int);  // How far do we want to look!
    void setRangeRing(int);   // Used on the display panel

    // Get functions
    int getRangeScale() const;   // Get current RangeScale
    int getRangeRing() const;    // Get current RangeRing
    bool getMastStatus() const;  // Get the current mast position
    int getAntennaHeight(int, int) const;  // Get current mast height
    int getBearing(int);  // Get target bearing
    int getRange(int);    // Get target range
    float getRadarHorizon(int ObserverDepth, int TargetHeight, int SeaState);

    bool isTargetVisible(Submarine *, int, int, int, int);  // Can we see the target

private:
    Submarine *Subs;

    bool rangescale10, rangescale20, rangescale30;
    bool rangescale40, rangescale50, rangescale60;

    bool rangering0, rangering5, rangering10;
    bool rangering15, rangering20, rangering25;

    float tick;  // our radar sweep angle
    int ALPHA;

    SDL_Surface *screen;

    Widget radarconsole;

    Widget range10off;
    Widget range10on;
    Widget range20off;
    Widget range20on;
    Widget range30off;
    Widget range30on;
    Widget range40off;
    Widget range40on;
    Widget range50off;
    Widget range50on;
    Widget range60off;
    Widget range60on;

    Widget ring0off;
    Widget ring0on;
    Widget ring5off;
    Widget ring5on;
    Widget ring10off;
    Widget ring10on;
    Widget ring15off;
    Widget ring15on;
    Widget ring20off;
    Widget ring20on;
    Widget ring25off;
    Widget ring25on;

    Widget mastdownoff;
    Widget mastdownon;

    Widget mastupoff;
    Widget mastupon;

    SDL_Surface *blip;
    SDL_Surface *sweep;

    bool Mast;
    int RangeScale;
    int RangeRing;
    int MastHeight;
    int CurrentAntennaHeight;
    int RadarHorizon;
    int bearing;
    int range;
    bool contact_selected;
    int contact_x;
    int contact_y;

    Uint32 orange;
};

#endif  // RADAR_H
