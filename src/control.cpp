/***************************************************************************
                          control.cpp  -  description
                             -------------------
    begin                : Tue June 11 2002
    copyright            : (C) 2002 by Rick McDaniel
    email                : rickeym@swbell.net
$Id: control.cpp,v 1.3 2003/04/20 19:30:29 anoncvs_pygsl Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_gfxPrimitives.h"  // rdm 9/11
#include "dfont.h"
#include "submarine.h"
#include "files.h"

#include "control.h"
#include "dstack.h"
#include <cmath>
#include <cstdlib>

#include <iostream>
using namespace std;

// Use these for the display
DStack HeadingStack, SpeedStack, DepthStack;

// Used to remember last ordered heading, to clear display.
float old_heading;

Control::Control(Submarine *temp): Subs(temp)
{
    // Default Values
    BEMER = false;
    BSTD = false;
    B23 = false;
    B13 = false;
    ASTOP = false;
    A13 = true;
    A23 = false;
    ASTD = false;
    AFULL = false;
    AFLK = false;
    depthup = false;
    depthdown = false;
}

Control::~Control()
{
}

void Control::InitGraphics(SDL_Surface *temp, SDL_Surface *tempcontrolscreen)
{
    screen = temp;
    controlscreen = tempcontrolscreen;

    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 420, 490, 32,
                                screen->format->Rmask,
                                screen->format->Gmask,
                                screen->format->Bmask,
                                screen->format->Amask);
    tempscreen = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    LoadWidgets();
    DisplayWidgets();

// orange = SDL_MapRGB(screen->format, 238, 118, 0);  // rdm 9/11 not needed
// green = SDL_MapRGB(screen->format, 0, 128, 0);
// black = SDL_MapRGB(screen->format, 0, 0, 0);
}

void Control::LoadWidgets()
{
    STOPoff = Load_Image("images/STOPoff.png");
    STOPon = Load_Image("images/STOPon.png");
    A13off = Load_Image("images/A13off.png");
    A13on = Load_Image("images/A13on.png");
    A23off = Load_Image("images/A23off.png");
    A23on = Load_Image("images/A23on.png");
    ASTDoff = Load_Image("images/ASTDoff.png");
    ASTDon = Load_Image("images/ASTDon.png");
    AFULLoff = Load_Image("images/AFULLoff.png");
    AFULLon = Load_Image("images/AFULLon.png");
    AFLKoff = Load_Image("images/AFLKoff.png");
    AFLKon = Load_Image("images/AFLKon.png");
    depthupoff = Load_Image("images/depthupoff.png");
    depthupon = Load_Image("images/depthupon.png");
    depthdownoff = Load_Image("images/depthdownoff.png");
    depthdownon = Load_Image("images/depthdownon.png");

    ClearControl = Load_Image("images/ClearControl.png");
}

void Control::UnLoadWidgets()
{
    // free the surfaces
    SDL_FreeSurface(STOPoff);
    SDL_FreeSurface(STOPon);
    SDL_FreeSurface(A13off);
    SDL_FreeSurface(A13on);
    SDL_FreeSurface(A23off);
    SDL_FreeSurface(A23on);
    SDL_FreeSurface(ASTDoff);
    SDL_FreeSurface(ASTDon);
    SDL_FreeSurface(AFULLoff);
    SDL_FreeSurface(AFULLon);
    SDL_FreeSurface(AFLKoff);
    SDL_FreeSurface(AFLKon);
    SDL_FreeSurface(depthupoff);
    SDL_FreeSurface(depthupon);
    SDL_FreeSurface(depthdownoff);
    SDL_FreeSurface(depthdownon);

    SDL_FreeSurface(ClearControl);
}

void Control::DisplayWidget(SDL_Surface *dest, int x, int y, SDL_Surface *source)
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

void Control::DisplayWidgets()
{
    // Engine Order Telegraph
    DisplayWidget(screen, 231, 582, ASTOP ? STOPon : STOPoff);
    DisplayWidget(screen, 264, 582, A13 ? A13on : A13off);
    DisplayWidget(screen, 297, 581, A23 ? A23on : A23off);
    DisplayWidget(screen, 330, 581, ASTD ? ASTDon : ASTDoff);
    DisplayWidget(screen, 363, 581, AFULL ? AFULLon : AFULLoff);
    DisplayWidget(screen, 396, 581, AFLK ? AFLKon : AFLKoff);
    DisplayWidget(screen, 198, 582, B13 ? A13on : A13off);
    DisplayWidget(screen, 165, 581, B23 ? A23on : A23off);
    DisplayWidget(screen, 132, 581, BSTD ? ASTDon : ASTDoff);
    DisplayWidget(screen, 99, 581, BEMER ? AFLKon : AFLKoff);
    // Depth up/down buttons
    DisplayWidget(screen, 111, 253, depthdown ? depthdownon : depthdownoff);
    DisplayWidget(screen, 384, 256, depthup ? depthupon : depthupoff);
}

void Control::ClearHeading()
{
    // Clear the heading area on the screen
    DisplayWidget(screen, 107, 140, ClearControl);
}

void Control::ClearOrdHeading()
{
    int x, y;
    double x1, y1, x2, y2;//rdm 9/11

    // Clear Last Ordered Heading Compass  // rdm 9/11
    x = int(251.0 + 58.0 * cos(1.57 - old_heading * 3.14 / 180.0));
    y = int(344.0 - 58.0 * sin(1.57 - old_heading * 3.14 / 180.0));
    //  filledTrigonRGBA(screen, 251, 341, 251, 347, x, y, 0, 0, 0, 255);  // rdm 9/11

    x1 = int(251.0 + 4.0 * sin(1.57 - old_heading * 3.14 / 180.0));
    y1 = int(344.0 + 4.0 * cos(1.57 - old_heading * 3.14 / 180.0));
    x2 = int(251.0 - 4.0 * sin(1.57 - old_heading * 3.14 / 180.0));
    y2 = int(344.0 - 4.0 * cos(1.57 - old_heading * 3.14 / 180.0));

    filledTrigonRGBA(screen, x1, y1, x2, y2, x, y, 0, 0, 0, 255);

    // Clear the ordered heading area on the screen
    DisplayWidget(screen, 107, 195, ClearControl);

    DisplayWidgets();
}

void Control::ClearDepth()
{
    // Clear the depth area on the screen
    DisplayWidget(screen, 211, 140, ClearControl);
}

void Control::ClearOrdDepth()
{
    // Clear the ordered depth area on the screen
    DisplayWidget(screen, 211, 195, ClearControl);

    DisplayWidgets();
}

void Control::ClearOrdSpeed()
{
    BEMER = false;
    BSTD = false;
    B23 = false;
    B13 = false;
    ASTOP = false;
    A13 = false;
    A23 = false;
    ASTD = false;
    AFULL = false;
    AFLK = false;

    // Clear the ordered speed area on the screen
    DisplayWidget(screen, 317, 194, ClearControl);

    DisplayWidgets();
}

void Control::ClearSpeed()
{
    // Clear the speed area on the screen
    DisplayWidget(screen, 317, 139, ClearControl);
}

void Control::ToggleBEMER()
{
    ClearOrdSpeed();
    BEMER = !BEMER;
    Subs->DesiredSpeed = -16;
}

void Control::ToggleBSTD()
{
    ClearOrdSpeed();
    BSTD = !BSTD;
    Subs->DesiredSpeed = -12;
}

void Control::ToggleB23()
{
    ClearOrdSpeed();
    B23 = !B23;
    Subs->DesiredSpeed = -8;
}

void Control::ToggleB13()
{
    ClearOrdSpeed();
    B13 = !B13;
    Subs->DesiredSpeed = -4;
}

void Control::ToggleASTOP()
{
    ClearOrdSpeed();
    ASTOP = !ASTOP;
    Subs->DesiredSpeed = 0;
}

void Control::ToggleA13()
{
    ClearOrdSpeed();
    A13 = !A13;
    Subs->DesiredSpeed = 5;
}

void Control::ToggleA23()
{
    ClearOrdSpeed();
    A23 = !A23;
    Subs->DesiredSpeed = 10;
}

void Control::ToggleASTD()
{
    ClearOrdSpeed();
    ASTD = !ASTD;
    Subs->DesiredSpeed = 15;
}

void Control::ToggleAFULL()
{
    ClearOrdSpeed();
    AFULL = !AFULL;
    Subs->DesiredSpeed = 20;
}

void Control::ToggleAFLK()
{
    ClearOrdSpeed();
    AFLK = !AFLK;
    Subs->DesiredSpeed = 32;
}

void Control::Display()
{
    static char text[120];
    char file1[] = "images/font.png";
    char file2[] = "data/font.dat";
    DFont fnt(file1, file2);
    float radians, previous_radians;
    float previous_speed, previous_depth;
    double x1, y1, x2, y2;

    if (depthup) Subs->DesiredDepth--;    // Take her up!!
    if (depthdown) Subs->DesiredDepth++;  // Take her down!!

    // Heading Screen
    sprintf(text, "%i", (int)Subs[0].Heading);
    fnt.PutString(screen, 145, 144, text);

    // Ordered Heading Screen
    sprintf(text, "%i", (int)Subs[0].DesiredHeading);
    fnt.PutString(screen, 145, 198, text);

    // Replot desired heading compass if we left control screen before turn completed  // rdm 9/11
    if (Subs->DesiredHeading != Subs->Heading) {
        // Draw a green filled triangle with vertices (x1, y1), (x2, y2), (x3, y3) and RGBA color (r, g, b, a)
        x = int(251.0 + 58.0 * cos(1.57 - Subs[0].DesiredHeading * 3.14 / 180.0));
        y = int(344.0 - 58.0 * sin(1.57 - Subs[0].DesiredHeading * 3.14 / 180.0));

        x1 = int(251.0 + 4.0 * sin(1.57 - Subs[0].DesiredHeading * 3.14 / 180.0));
        y1 = int(344.0 + 4.0 * cos(1.57 - Subs[0].DesiredHeading * 3.14 / 180.0));
        x2 = int(251.0 - 4.0 * sin(1.57 - Subs[0].DesiredHeading * 3.14 / 180.0));
        y2 = int(344.0 - 4.0 * cos(1.57 - Subs[0].DesiredHeading * 3.14 / 180.0));

        filledTrigonRGBA(screen, x1, y1, x2, y2, x, y, 0, 128, 0, 255);
    }

    // Depth Screen
    sprintf(text, "%i", (int)Subs[0].Depth);
    fnt.PutString(screen, 247, 144, text);

    // Desired Depth Screen
    sprintf(text, "%i", (int)Subs[0].DesiredDepth);
    fnt.PutString(screen, 247, 199, text);

    // Clear the Ordered Depth Boxes if needed.
    if (Subs->DesiredDepth == 100.0) ClearOrdDepth();
    if (Subs->DesiredDepth == 1000.0) ClearOrdDepth();

    // Speed Screen
    sprintf(text, "%d", int(Subs->Speed));
    fnt.PutString(screen, 355, 144, text);

    // Desired Speed Screen
    sprintf(text, "%i", (int)Subs->DesiredSpeed);
    fnt.PutString(screen, 355, 199, text);

    radians = Subs->Heading * (3.14 / 180.0); // degrees to radians

    if (!HeadingStack.empty()) {  // Is there data on the stack?
        previous_radians = HeadingStack.pop();
        previous_speed = SpeedStack.pop();
        previous_depth = DepthStack.pop();

        // Next lines clear the display as nec to prevent garbage on screen
        if (Subs->Heading <= 10.0 && previous_radians * (180.0 / 3.14) > 10.0) ClearHeading();
        if (Subs->Heading <= 100.0 && previous_radians * (180.0 / 3.14) > 100.0) ClearHeading();
        if (Subs->Speed <= 10.0 && previous_speed > 10.0) ClearSpeed();
        if (Subs->Speed >= -10.0 && previous_speed < -10.0) ClearSpeed();
        if (Subs->Speed >= 0.0 && previous_speed < 0.0) ClearSpeed();

        if (Subs->Depth <= 100.0 && previous_depth > 100.0) ClearDepth();
        if (Subs->Depth <= 1000.0 && previous_depth > 1000.0) ClearDepth();


        // Clear Old Data from Heading Compass  // rdm 9/11
        // Draw a orange filled triangle with vertices (x1, y1), (x2, y2), (x3, y3) and RGBA color (r, g, b, a)
        x = int(251.0 + 58.0 * cos(1.57 - previous_radians));
        y = int(344.0 - 58.0 * sin(1.57 - previous_radians));

        x1 = int(251.0 + 4.0 * sin(1.57 - previous_radians));
        y1 = int(344.0 + 4.0 * cos(1.57 - previous_radians));
        x2 = int(251.0 - 4.0 * sin(1.57 - previous_radians));
        y2 = int(344.0 - 4.0 * cos(1.57 - previous_radians));

        filledTrigonRGBA(screen, x1, y1, x2, y2, x, y, 0, 0, 0, 255);
    }

    // Draw Desired Heading Compass  // rdm 9/11
    // Draw a orange filled triangle with vertices (x1, y1), (x2, y2), (x3, y3) and RGBA color (r, g, b, a)
    x = int(251.0 + 58.0 * cos(1.57 - radians));
    y = int(344.0 - 58.0 * sin(1.57 - radians));

    x1 = int(251.0 + 4.0 * sin(1.57 - radians));
    y1 = int(344.0 + 4.0 * cos(1.57 - radians));
    x2 = int(251.0 - 4.0 * sin(1.57 - radians));
    y2 = int(344.0 - 4.0 * cos(1.57 - radians));

    filledTrigonRGBA(screen, x1, y1, x2, y2, x, y, 238, 118, 0, 255);

    // Push the data on the stack
    HeadingStack.push(radians);
    SpeedStack.push(Subs->Speed);
    DepthStack.push(Subs->Depth);

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void Control::AdjustHeading(int x, int y)
{

    double c1, c3, c5, c6;
    double x1, y1, x2, y2;
    int heading;

    c1 = double(251);
    c3 = double(344);

    // c5 = 60.0*(double(y)-c3);
    // c6 = 60.0*(double(x)-c1);

    c5 = 58.0 * (double(y) - c3);
    c6 = 58.0 * (double(x) - c1);


    ClearOrdHeading();

    if (x >= 251) {
        heading = int(90.0 + (180 / 3.14) * atan(c5 / c6));
    } else {
        heading = int(270.0 + (180 / 3.14) * atan(c5 / c6));
    }

    Subs->DesiredHeading = heading;
    old_heading = heading;

    // Draw Desired Heading Compass  // rdm 9/11
    // Draw a green filled triangle with vertices (x1, y1), (x2, y2), (x3, y3) and RGBA color (r, g, b, a)
    x = int(251.0 + 58.0 * cos(1.57 - heading * 3.14 / 180.0));
    y = int(344.0 - 58.0 * sin(1.57 - heading * 3.14 / 180.0));
    // filledTrigonRGBA(screen, 251, 341, 251, 347, x, y, 0, 128, 0, 255);  // rdm 9/11

    x1 = int(251.0 + 4.0 * sin(1.57 - heading * 3.14 / 180.0));
    y1 = int(344.0 + 4.0 * cos(1.57 - heading * 3.14 / 180.0));
    x2 = int(251.0 - 4.0 * sin(1.57 - heading * 3.14 / 180.0));
    y2 = int(344.0 - 4.0 * cos(1.57 - heading * 3.14 / 180.0));

    filledTrigonRGBA(screen, x1, y1, x2, y2, x, y, 0, 128, 0, 255);
    return;
}

void Control::AdjustDepth(int x)
{
    ClearOrdDepth();
    if (x > 112 && x < 126) {
        if (depthup) {
            depthup = false;
        } else {
            depthdown = true;
            depthup = false;
        }
    }

    if (x > 385 && x < 401) {
        if (depthdown) {
            depthdown = false;
        } else {
            depthdown = false;
            depthup = true;
        }
    }
    DisplayWidgets();
    return;
}
