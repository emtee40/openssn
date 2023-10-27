/***************************************************************************
                          esm.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: esm.h,v 1.5 2003/04/14 05:51:03 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef ESM_H
#define ESM_H

#include "SDL/SDL.h"
#include "submarine.h"
#include "widget.h"

/**
  * @author Michael Bridak
  */

class Esm
{
public:
    Esm();
    ~Esm();

    void setSubs(Submarine *Subs);

    void InitGraphics(SDL_Surface *screen);

    void LoadWidgets();
    void UnLoadWidgets();

    void DisplayWidgets();

    void LowerMast();   // Lower esm mast
    void RaiseMast();   // Raise esm mast
    void ToggleMast();  // Toggle mast status

    // Set functions
    void setRangeScale(int);  // How far do we want to look!
    void setRangeRing(int);   // Used on the display panel

    // Get functions
    bool getMastStatus() const;           // Get the current mast position
    int getAntennaHeight(int, int) const; // Get current mast height
    int getBearing(int, int, int, int);   // Get the Esm bearing
    int getSignalStrength(Submarine *, int, int, int, bool, int);  // Get the Esm signal strength
    int getEsmHorizon(int, int, int);     // Get the Esm Horizon

    bool isTargetVisible(Submarine *, int TargetRange, int ObserverDepth,
                         int TargetHeight, bool TargetRadar, int SeaState);

    void DisplayContacts();

    void UpdateDisplay();

private:
    Submarine *Subs;

    SDL_Surface *screen;

    Widget esmconsole;

    Widget mastdownoff;
    Widget mastdownon;

    Widget mastupoff;
    Widget mastupon;

    Widget styllus;

    bool Mast;
    int MastHeight;
    int CurrentAntennaHeight;
    int EsmHorizon;
    int bearing;
    int SignalStrength;
    int DetectProb;
    Uint32 orange;
};

#endif  // ESM_H
