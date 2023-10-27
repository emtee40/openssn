/***************************************************************************
                          Sonar.h  -  description
                             -------------------
    begin                : Fri Apr 11 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: sonar.h,v 1.10 2003/09/20 22:53:15 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef SONAR_H
#define SONAR_H

#include "message.h"
#include "submarine.h"
#include "targetmotionanalysis.h"
#include "towedarray.h"
#include "widget.h"

/**
  * @author Michael Bridak
  */

class AnBqq5
{
public:
    AnBqq5(TowedArray &tb16, TargetMotionAnalysis &tma, msg &msg);
    ~AnBqq5();

    /** Update the subs list */
    void setSubs(Submarine *Subs);

    /** Set the flow and ambiant noise */
    void setFlowandambientnoise(float flowandambientnoise);

    /** Initialize the graphic for this class, *screen is the main
        display screen passed in from main. */
    void InitGraphics(SDL_Surface *screen);

    /** Loads in the widgets */
    void LoadWidgets();

    /** Unloads the widgets */
    void UnLoadWidgets();

    /** Displays the widgets needed for the sonar display */
    void DisplayWidgets();

    void Sonar(bool center);

    void DisplaySonar();

    /** The main entry point to do a gross update of all the sonar
        stations sub screen. */
    void UpdateDisplay(Submarine *target);
    void ClearSonarData();

    void TowedSonar(bool center);

    void AdvanceSonarDisplay();

    void AdvanceTB16Screen();

    /** Displays the degree bearing scale in the waterfall displays
        the bool center flag is to check if we want north (0 degree)
        or south (180 degree) as the scales center. */
    void DisplayBearingScale(bool center);

    /** Switches the upper sonar waterfall display screen to show the
        history of the spherical array. */
    void UpperCRT_Spherical();

    /** Switches the lower sonar waterfall display screen to show the
        history of the spherical array. */
    void LowerCRT_Spherical();

    /** Switches the upper sonar waterfall display screen to show the
        history of the TB16 array. */
    void UpperCRT_TowedArray();

    /** Switches the lower sonar waterfall display screen to show the
        history of the TB16 array. */
    void LowerCRT_TowedArray();

    /** Toggles the bool variable arraychoice which affects the sonar
        widget display and decides which array, spherical or the towed
        TB16, will appear when either the upper crt or lower crt widget
        is clicked. */

    void ToggleArrayChoice();
    /** Toggles the truerel bool variable. At this time, only affects
        it to the sonar widget display. But will soon determine if the
        waterfall display shows us true or relative traces. */
    void ToggleTrueRel();

    /** Called when the user clicks the "Upper CRT" widget on the sonar
        screen. When called, it will switch the upper waterfall display
        to show the desired sonar sensors history. */
    void UpperCRT_Button();

    /** Called when the user clicks the "Lower CRT" widget on the sonar
        screen. When called, it will switch the lower waterfall display
        to show the desired sonar sensors history. */
    void LowerCRT_Button();

    /** Called to checked whether the user has clicked on the assign
        tracker widget on the sonar screen. */
    bool GetAssignTrackerState();

    /** Called when the user clicks on the assign tracker widget on the
        sonar screen. Toggles the assigntracker bool variable. When the
        flag is true, and the user clicks on one of the tracker widgets,
        that tracker will be assigned to the sound source currently
        under the sonar cursor. */
    void ToggleAssignTracker();

    bool GetNorthCenterState();

    void ToggleNorthCenter();

    /** Called when the user clicks on the "Stop TB16 Winch" widget on
        the sonar screen. This unhighlights the widget and calls the
        TB16.Stop() routine. */
    void StopWinch();

    int RandInt(int TO);

    int ReciprocalBearing(int bearing);

    void DisplayCursor();
    void UpdateCursor();

private:
    Submarine *Subs;

    int cursorBearing;
    bool bearingdisplay5by6;
    bool arraychoice5by6;
    bool assigntracker;
    float flowandambientnoise;

    SDL_Surface *screen;

    SDL_Surface *sonarscreen;
    SDL_Surface *towedarrayscreen;
    SDL_Surface *uppersonarcrt;
    SDL_Surface *lowersonarcrt;

    Widget sonarconsole;
    Widget ncscale;
    Widget scscale;
    Widget truerel[2];
    Widget sphertowed[2];
    Widget uppercrtoff;
    Widget uppercrton;
    Widget lowercrtoff;
    Widget lowercrton;
    Widget tb16winchoff;
    Widget tb16winchon;
    Widget extendtb16[2];
    Widget retracttb16[2];
    Widget cutarray;
    Widget sendping;
    Widget assigntrackerwidget[2];
    Widget tracker1[2];
    Widget tracker2[2];
    Widget tracker3[2];
    Widget tracker4[2];

    TowedArray &TB16;
    TargetMotionAnalysis &Tma;
    msg &Message;
    float deAngle;
    Uint32 black;
    Uint32 white;
    Uint32 green;
    bool northcenter;
};

#endif  // SONAR_H
