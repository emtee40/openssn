/***************************************************************************
                          targetmotionanalysis.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: targetmotionanalysis.cpp,v 1.7 2003/05/17 22:27:28 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#include "targetmotionanalysis.h"
#include <cmath>
#include "draw.h"
#include "files.h"

TargetMotionAnalysis::TargetMotionAnalysis()
{
    MutexLock = SDL_CreateMutex();
    plotscale = 1;
    centerGeoPlot = true;
    scrolloffsetx = 0;
    scrolloffsety = 0;
}

TargetMotionAnalysis::~TargetMotionAnalysis()
{
    SDL_DestroyMutex(MutexLock);
}

/** Update the subs list */
void TargetMotionAnalysis::setSubs(Submarine *Subs)
{
    this->Subs = Subs;
}

/** Initialize the graphics */
void TargetMotionAnalysis::InitGraphics(SDL_Surface *screen)
{
    this->screen = screen;
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 500, 500, 32,
                                                screen->format->Rmask,
                                                screen->format->Gmask,
                                                screen->format->Bmask,
                                                screen->format->Amask);
    GeoPlotScreen = SDL_DisplayFormat(surface);
    SDL_FreeSurface(surface);
    black     = SDL_MapRGB(screen->format, 0, 0, 0);
    white     = SDL_MapRGB(screen->format, 255, 255, 255);
    red       = SDL_MapRGB(screen->format, 255, 99, 20);
    green     = SDL_MapRGB(screen->format, 0, 255, 0);
    yellow    = SDL_MapRGB(screen->format, 216, 255, 0);
    mapcolor  = SDL_MapRGB(screen->format, 130, 201, 225);
    ClearGeoPlot();
}

void TargetMotionAnalysis::LoadWidgets()
{
    tmaconsole.load("images/Mapscreen.png");
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

void TargetMotionAnalysis::UnLoadWidgets()
{
    // free the surfaces
    tmaconsole.unload();
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

void TargetMotionAnalysis::DisplayWidgets()
{
    tmaconsole.draw(screen, 0, 0);
    centerGeoPlot ? centerbuttondown.draw(screen, 225, 269) : centerbuttonup.draw(screen, 225, 269);
    upbuttonup.draw(screen, 225, 219);
    downbuttonup.draw(screen, 225, 319);
    leftbuttonup.draw(screen, 175, 269);
    rightbuttonup.draw(screen, 275, 269);
    plusbuttonup.draw(screen, 175, 359);
    minusbuttonup.draw(screen, 275, 359);
}

/** Returns TRUE if tracker is in use */
bool TargetMotionAnalysis::GetTrackerState(int tracker) const
{
    return trackerstate[tracker];
}

/** Frees a tracker to be used later. */
void TargetMotionAnalysis::ClearTracker(int tracker)
{
    trackerstate[tracker] = false;
}

/** Assign a tracker to a target */
void TargetMotionAnalysis::AssignTracker(int tracker, int target)
{
    trackerstate[tracker] = true;
}

/** Assign Target designation and Tracker to trace currently under
cursor on SONAR screen. */
void TargetMotionAnalysis::DesignateContact()
{
}

/** Display a GeoPlot for the designated contact based on the
position of the boat and the bearing history of the contact. */
void TargetMotionAnalysis::DisplayGeoPlot()
{
    ClearGeoPlot();
    if (size != 0) {
        PlotHistory();
    }
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.h = 500;
    rectangle.w = 500;
    SDL_UpdateRects(GeoPlotScreen, 1, &rectangle);
}

/** Display the LOS Diagram */
void TargetMotionAnalysis::DisplayLOS()
{

    double bearing_to_target;
    double relative_bearing;
    double recipbearing;
    double var1;  // a place to hold a value so we don't have to calc it twice
    double destinationx;  // the dest x&y point for
    double destinationy;  // the drawn line
    double direction;
    int x, y;

    bearing_to_target = storage[size - 1].bearing;
    ClearGeoPlot();

    // Draw LOS
    DrawLine(GeoPlotScreen, 250, 292, 250, 100, white);

    // Draw our ships angle relative to the LOS
    if (our_heading > bearing_to_target) bearing_to_target += 360;
    relative_bearing = bearing_to_target - our_heading;

    x = 250;
    y = 292;
    direction = 360 - relative_bearing;
    our_speed = (our_speed * 3);
    var1 = direction * 0.017453;  // convert degrees to radians
    destinationx = (sin(var1) * our_speed) + x;
    destinationy = (-1) * (cos(var1) * our_speed) + y;
    DrawLine(GeoPlotScreen, x, y, (int)destinationx, (int)destinationy, yellow);

    // Draw targets angle relative to the LOS
    x = 250;
    y = 100;
    if (bearing_to_target >= 180) {
        recipbearing = bearing_to_target - 180;
    } else {
        recipbearing = bearing_to_target + 180;
    }
    direction = target_heading + (180 - recipbearing);
    target_speed = (target_speed * 3);
    var1 = direction * 0.017453;  // convert degrees to radians
    destinationx = (sin(var1) * target_speed) + x;
    destinationy = (-1) * (cos(var1) * target_speed) + y;
    DrawLine(GeoPlotScreen, x, y, (int)destinationx, (int)destinationy, red);

    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.h = 500;
    rectangle.w = 500;
    SDL_UpdateRects(GeoPlotScreen, 1, &rectangle);
}

/** Creates a speed ruler which we will use to drag around the
GeoPlot to test solutions.
Extend ruler backwards in time by plotting course using reciprocal
bearings. */
void TargetMotionAnalysis::CreateSpeedRuler()
{
}

/** Plots the amount of error from the expected bearing to target
and the actual bearing received. */
void TargetMotionAnalysis::BearingDeviationPlot()
{
}

/** Record the boats current position */
void TargetMotionAnalysis::RecordBoatPosition(float latitude, float longitude, double bearing, Uint32 tick)
{
    Lock();
    position temp;
    temp.lat = latitude;
    temp.lon = longitude;
    temp.bearing = bearing;
    temp.tick = tick;
    RecBoatPos(temp);
    UnLock();
}

/** Lock Mutex */
void TargetMotionAnalysis::Lock()
{
    SDL_mutexP(MutexLock);
}

/** Unlock Mutex */
void TargetMotionAnalysis::UnLock()
{
    SDL_mutexV(MutexLock);
}

/** erases the work screen */
void TargetMotionAnalysis::ClearGeoPlot()
{
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;  // define a rectangle on the screen and make it black
    rectangle.h = 500;
    rectangle.w = 500;
    SDL_FillRect(GeoPlotScreen, &rectangle, black);
    SDL_UpdateRects(GeoPlotScreen, 1, &rectangle);
}

/** Plot the boats history */
void TargetMotionAnalysis::PlotHistory()
{
    // scale is in YDS per pixel
    int x, y;  // where to place the ships
    int xoffset = 0;  // offsets to move the ships to
    int yoffset = 0;  // the defined place on map screen
    double var1;  // just a place to hold a value so we don't have to calc it twice
    double destinationx;  // the dest X&Y point for
    double destinationy;  // the drawn line
    int length = 2000;

    int scale = plotscale * 5;
    if (centerGeoPlot) {  // center map onto our own ships location
        scrolloffsetx = 250 - (int)(storage[size - 1].lat / scale);
        scrolloffsety = 250 - (int)(storage[size - 1].lon / scale);
    }

    for (int index = 0; index < size; index ++) {
        x = 500 - (int)(storage[index].lat / scale);
        x = x - scrolloffsetx;
        y = 500 - (int)(storage[index].lon / scale);
        y = y - scrolloffsety;
        if (x > 10 && x < 490 && y > 10 && y < 490) {
            x = x + xoffset;
            y = y + yoffset;
            DrawCircle(GeoPlotScreen, x, y, 2, yellow);
        }
        var1 = storage[index].bearing * 0.017453;  // convert degrees to radians
        destinationx = (sin(var1) * length) + x;
        destinationy = (-1) * (cos(var1) * (length / .75)) + y;  // adjust to account for 4:3 screen ratio
        DrawLine(GeoPlotScreen, x, y, (int)destinationx, (int)destinationy, white);
    }
}

void TargetMotionAnalysis::DisplayTMA()
{
    SDL_Rect rect;
    Lock();
    our_heading = (double) Subs->Heading;
    our_speed = (float) Subs->Speed;
    target_heading = (double) Subs->Heading;
    target_speed = (float) Subs->Speed;
    DisplayGeoPlot();
    // DisplayLOS();
    rect.x = 374;
    rect.y = 145;
    rect.h = GeoPlotScreen->h;
    rect.w = GeoPlotScreen->w;
    SDL_BlitSurface(GeoPlotScreen, NULL, screen, &rect);
    UnLock();
}

void TargetMotionAnalysis::UpdateDisplay()
{
    DisplayWidgets();
    DisplayTMA();
}

/** Increments plotscale to zoom in on the GeoPlot */
void TargetMotionAnalysis::IncreasePlotScale()
{
    plotscale++;
    if (plotscale > 15) {
        plotscale = 15;
    }
    plusbuttondown.draw(screen, 175, 359, true);
    SDL_Delay(100);
}

/** Decrement the plotscale to zoom out of GeoPlot */
void TargetMotionAnalysis::DecreasePlotScale()
{
    plotscale--;
    if (plotscale < 1) {
        plotscale = 1;
    }
    minusbuttondown.draw(screen, 275, 359, true);
    SDL_Delay(100);
}

/** No descriptions */
void TargetMotionAnalysis::ToggleGeoPlotCenter()
{
    centerGeoPlot = !centerGeoPlot;
}

/** Scroll the plot history up */
void TargetMotionAnalysis::ScrollPlotUp(int change_scrolly)
{
    scrolloffsety -= change_scrolly;
    upbuttondown.draw(screen, 225, 219, true);
    SDL_Delay(100);
}

/** Scroll the plot history down */
void TargetMotionAnalysis::ScrollPlotDown(int change_scrolly)
{
    scrolloffsety += change_scrolly;
    downbuttondown.draw(screen, 225, 319, true);
    SDL_Delay(100);
}

/** Scroll the plot history left */
void TargetMotionAnalysis::ScrollPlotLeft(int change_scrollx)
{
    scrolloffsetx -= change_scrollx;
    leftbuttondown.draw(screen, 175, 269, true);
    SDL_Delay(100);
}

/** Scroll the plot history right */
void TargetMotionAnalysis::ScrollPlotRight(int change_scrollx)
{
    scrolloffsetx += change_scrollx;
    rightbuttondown.draw(screen, 275, 269, true);
    SDL_Delay(100);
}

/** Fill in the initial values for the solution. */
void TargetMotionAnalysis::InitializeSolution(float LatYards, float LonYards, int Speed, int Heading)
{
    Solution[0].Lat_TotalYards = LatYards;
    Solution[0].Lon_TotalYards = LonYards;
    Solution[0].Speed = Speed;
    Solution[0].Heading = Heading;
}
