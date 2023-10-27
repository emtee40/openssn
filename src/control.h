/***************************************************************************
                          control.h  -  description
                             -------------------
    begin                : Tue June 11 2002
    copyright            : (C) 2002 by Rick McDaniel
    email                : rickeym@swbell.net
$Id: control.h,v 1.3 2003/04/14 05:51:03 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef CONTROL_H
#define CONTROL_H

#include "SDL/SDL.h"
#include "submarine.h"
#include "dfont.h"
#include "widget.h"

/**
  * @author Rick McDaniel
  */


class Control
{
public:
    Control();
    ~Control();

    void setSubs(Submarine *Subs);

    void InitGraphics(SDL_Surface *screen);

    void LoadWidgets();
    void UnLoadWidgets();

    void DisplayWidgets();

    void ClearOrdSpeed();

    void ToggleBEMER();
    void ToggleBSTD();
    void ToggleB23();
    void ToggleB13();
    void ToggleASTOP();
    void ToggleA13();
    void ToggleA23();
    void ToggleASTD();
    void ToggleAFULL();
    void ToggleAFLK();

    void Display();

    void UpdateDisplay();

    void AdjustHeading(int x, int y);

    void AdjustDepth(int x);

private:
    Submarine *Subs;

    bool A13, A23, ASTD, AFULL, AFLK;
    bool ASTOP, B13, B23, BSTD, BEMER;

    bool depthup, depthdown;

    SDL_Surface *screen;

    Widget controlconsole;
    Widget STOPoff;
    Widget STOPon;
    Widget A13off;
    Widget A13on;
    Widget A23off;
    Widget A23on;
    Widget ASTDoff;
    Widget ASTDon;
    Widget AFULLoff;
    Widget AFULLon;
    Widget AFLKoff;
    Widget AFLKon;
    Widget depthupoff;
    Widget depthupon;
    Widget depthdownoff;
    Widget depthdownon;
};

#endif  // CONTROL_H
