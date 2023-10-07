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

/**
  * @author Rick McDaniel
  */


class Control
{
public:
    Submarine *Subs;

    Control(Submarine *temp);
    ~Control();

    void  InitGraphics(SDL_Surface *temp, SDL_Surface *tempcontrolscreen);

    void ClearHeading();
    void ClearOrdHeading();

    void ClearSpeed();
    void ClearOrdSpeed();

    void ClearDepth();
    void ClearOrdDepth();

    void LoadWidgets();
    void UnLoadWidgets();

    void DisplayWidget(SDL_Surface *dest, int x, int y, SDL_Surface *source);
    void DisplayWidgets();

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

    void AdjustHeading(int x, int y);

    void AdjustDepth(int x);

private:
    SDL_Surface *controlscreen;
    SDL_Surface *tempscreen;

    SDL_Surface *ClearControl;

    bool A13, A23, ASTD, AFULL, AFLK;
    bool ASTOP, B13, B23, BSTD, BEMER;

    bool depthup, depthdown;

    SDL_Surface *screen;
    SDL_Surface *STOPoff;
    SDL_Surface *STOPon;
    SDL_Surface *A13off;
    SDL_Surface *A13on;
    SDL_Surface *A23off;
    SDL_Surface *A23on;
    SDL_Surface *ASTDoff;
    SDL_Surface *ASTDon;
    SDL_Surface *AFULLoff;
    SDL_Surface *AFULLon;
    SDL_Surface *AFLKoff;
    SDL_Surface *AFLKon;
    SDL_Surface *depthupoff;
    SDL_Surface *depthupon;
    SDL_Surface *depthdownoff;
    SDL_Surface *depthdownon;

    int x;
    int y;
};

#endif  // CONTROL_H
