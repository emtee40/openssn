/***************************************************************************
                          targetmotionanalysis.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: targetmotionanalysis.h,v 1.6 2003/04/20 16:55:49 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef TARGETMOTIONANALYSIS_H
#define TARGETMOTIONANALYSIS_H

#define MAXTRACKERS 13

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "boatpositionlog.h"
#include "contact.h"
#include "submarine.h"
#include "widget.h"

/**
  * @author Michael Bridak
  */

class TargetMotionAnalysis : public BoatPositionLog
{
public:
    Contact Solution[12];
    Uint32 black, white, red, green, yellow, mapcolor;  // Place to hold color info

    double our_heading;
    double target_heading;

    float our_speed;
    float target_speed;

    /** an SDL surface to plot our GeoPlot on */
    SDL_Surface *GeoPlotScreen;

    /**  */
    int plotscale;

    /** Geoplot centering flag */
    bool centerGeoPlot;

    TargetMotionAnalysis();
    ~TargetMotionAnalysis();

    /** Update the subs list */
    void setSubs(Submarine *Subs);

    /** Initialize the graphics */
    void InitGraphics(SDL_Surface *screen);

    /** Load the widgets */
    void LoadWidgets();

    /** Free the widgets */
    void UnLoadWidgets();

    /** Display all TMA widgets */
    void DisplayWidgets();

    /** Frees a tracker to be used later */
    void ClearTracker(int tracker);

    /** Returns the TRUE if tracker is in use */
    bool GetTrackerState(int tracker) const;

    /** Assign a tracker to a target */
    void AssignTracker(int tracker, int target);

    /** Assign Target designation and Tracker to trace currently under
    cursor on SONAR screen.
    */
    void DesignateContact();

    /** Display a GeoPlot for the designated contact based on the
    position of the boat and the bearing history of the contact.
    */
    void DisplayGeoPlot();

    /** Display the LOS Diagram
    */
    void DisplayLOS();

    /** Creates a speed ruler which we will use to drag around the
    GeoPlot to test solutions.
    */
    void CreateSpeedRuler();

    /** Plots the amount of error from the expected bearing to target
    and the actual bearing recieved.
    */
    void BearingDeviationPlot();

    /** Record the boats current position
    */
    void RecordBoatPosition(float latitude, float longitude, double bearing, Uint32 tick);

    /** Lock Mutex */
    void Lock();

    /** Unlock Mutex */
    void UnLock();

    /** erases the work screen */
    void ClearGeoPlot();

    /** Plot the boats history */
    void PlotHistory();

    /** Display the TMA */
    void DisplayTMA();

    /** Display of the TMA console, widgets and screen */
    void UpdateDisplay();

    /** Increments plotscale to zoom in on the GeoPlot */
    void IncreasePlotScale();

    /** Decrement the plotscale to zoom out of GeoPlot */
    void DecreasePlotScale();

    /** No descriptions */
    void ToggleGeoPlotCenter();

    /** Scroll the plot history up */
    void ScrollPlotUp(int change_scrolly);

    /** Scroll the plot history down */
    void ScrollPlotDown(int change_scrolly);

    /** Scroll the plot history left */
    void ScrollPlotLeft(int change_scrollx);

    /** Scroll the plot history right */
    void ScrollPlotRight(int change_scrollx);

    /** Fill in the initial values for the solution. */
    void InitializeSolution(float LatYards, float LonYards, int Speed, int Heading);

private:  // Private attributes
    Submarine *Subs;

    /** Pointer to the display surface */
    SDL_Surface *screen;

    Widget tmaconsole;
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

    int scrolloffsetx;  // offset to center map
    int scrolloffsety;

    /**  */
    bool trackerstate[MAXTRACKERS];
    struct packet
    {
        float LatYards;
        float LonYards;
        float BearingToTarget;
    };

    SDL_mutex *MutexLock;
};

#endif  // TARGETMOTIONANALYSIS_H
