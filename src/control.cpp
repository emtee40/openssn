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

#include "control.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "SDL/SDL_gfxPrimitives.h"  // rdm 9/11
#include "files.h"

Control::Control()
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

void Control::setSubs(Submarine *Subs)
{
    this->Subs = Subs;
}

void Control::InitGraphics(SDL_Surface *screen)
{
    this->screen = screen;
}

void Control::LoadWidgets()
{
    controlconsole.load("images/ControlScreen.png");
    STOPoff.load("images/STOPoff.png");
    STOPon.load("images/STOPon.png");
    A13off.load("images/A13off.png");
    A13on.load("images/A13on.png");
    A23off.load("images/A23off.png");
    A23on.load("images/A23on.png");
    ASTDoff.load("images/ASTDoff.png");
    ASTDon.load("images/ASTDon.png");
    AFULLoff.load("images/AFULLoff.png");
    AFULLon.load("images/AFULLon.png");
    AFLKoff.load("images/AFLKoff.png");
    AFLKon.load("images/AFLKon.png");
    depthupoff.load("images/depthupoff.png");
    depthupon.load("images/depthupon.png");
    depthdownoff.load("images/depthdownoff.png");
    depthdownon.load("images/depthdownon.png");
}

void Control::UnLoadWidgets()
{
    // free the surfaces
    controlconsole.unload();
    STOPoff.unload();
    STOPon.unload();
    A13off.unload();
    A13on.unload();
    A23off.unload();
    A23on.unload();
    ASTDoff.unload();
    ASTDon.unload();
    AFULLoff.unload();
    AFULLon.unload();
    AFLKoff.unload();
    AFLKon.unload();
    depthupoff.unload();
    depthupon.unload();
    depthdownoff.unload();
    depthdownon.unload();
}

void Control::DisplayWidgets()
{
    // Control console
    controlconsole.draw(screen, 0, 0);
    // Engine Order Telegraph
    ASTOP ? STOPon.draw(screen, 231, 582) : STOPoff.draw(screen, 231, 582);
    A13 ? A13on.draw(screen, 264, 582) : A13off.draw(screen, 264, 582);
    A23 ? A23on.draw(screen, 297, 581) : A23off.draw(screen, 297, 581);
    ASTD ? ASTDon.draw(screen, 330, 581) : ASTDoff.draw(screen, 330, 581);
    AFULL ? AFULLon.draw(screen, 363, 581) : AFULLoff.draw(screen, 363, 581);
    AFLK ? AFLKon.draw(screen, 396, 581) : AFLKoff.draw(screen, 396, 581);
    B13 ? A13on.draw(screen, 198, 582) : A13off.draw(screen, 198, 582);
    B23 ? A23on.draw(screen, 165, 581) : A23off.draw(screen, 165, 581);
    BSTD ? ASTDon.draw(screen, 132, 581) : ASTDoff.draw(screen, 132, 581);
    BEMER ? AFLKon.draw(screen, 99, 581) : AFLKoff.draw(screen, 99, 581);
    // Depth up/down buttons
    depthdown ? depthdownon.draw(screen, 111, 253) : depthdownoff.draw(screen, 111, 253);
    depthup ? depthupon.draw(screen, 384, 256) : depthupoff.draw(screen, 384, 256);
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
    static DFont fnt("images/font.png", "data/font.dat");
    int x, y;
    double x1, y1, x2, y2;

    if (depthup) Subs->DesiredDepth--;    // Take her up!!
    if (depthdown) Subs->DesiredDepth++;  // Take her down!!

    // Heading Screen
    sprintf(text, "%i", (int)Subs[0].Heading);
    fnt.PutString(screen, 145, 144, text);

    // Ordered Heading Screen
    sprintf(text, "%i", (int)Subs[0].DesiredHeading);
    fnt.PutString(screen, 145, 198, text);

    // Desired heading compass (if applicable)
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

    // Draw Heading Compass  // rdm 9/11
    // Draw a orange filled triangle with vertices (x1, y1), (x2, y2), (x3, y3) and RGBA color (r, g, b, a)
    x = int(251.0 + 58.0 * cos(1.57 - Subs->Heading * 3.14 / 180.0));
    y = int(344.0 - 58.0 * sin(1.57 - Subs->Heading * 3.14 / 180.0));

    x1 = int(251.0 + 4.0 * sin(1.57 - Subs->Heading * 3.14 / 180.0));
    y1 = int(344.0 + 4.0 * cos(1.57 - Subs->Heading * 3.14 / 180.0));
    x2 = int(251.0 - 4.0 * sin(1.57 - Subs->Heading * 3.14 / 180.0));
    y2 = int(344.0 - 4.0 * cos(1.57 - Subs->Heading * 3.14 / 180.0));

    filledTrigonRGBA(screen, x1, y1, x2, y2, x, y, 238, 118, 0, 255);

    // Depth Screen
    sprintf(text, "%i", (int)Subs[0].Depth);
    fnt.PutString(screen, 247, 144, text);

    // Desired Depth Screen
    sprintf(text, "%i", (int)Subs[0].DesiredDepth);
    fnt.PutString(screen, 247, 199, text);

    // Speed Screen
    sprintf(text, "%d", int(Subs->Speed));
    fnt.PutString(screen, 355, 144, text);

    // Desired Speed Screen
    sprintf(text, "%i", (int)Subs->DesiredSpeed);
    fnt.PutString(screen, 355, 199, text);
}

void Control::UpdateDisplay()
{
    DisplayWidgets();
    Display();
    Subs->map->Draw_Depth_Meter(Subs->Depth, SCREEN_HELM);
}

void Control::AdjustHeading(int x, int y)
{

    double c1, c3, c5, c6;
    int heading;

    c1 = double(251);
    c3 = double(344);

    c5 = 58.0 * (double(y) - c3);
    c6 = 58.0 * (double(x) - c1);

    if (x >= 251) {
        heading = int(90.0 + (180 / 3.14) * atan(c5 / c6));
    } else {
        heading = int(270.0 + (180 / 3.14) * atan(c5 / c6));
    }

    Subs->DesiredHeading = heading;
}

void Control::AdjustDepth(int x)
{
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
}
