/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Tue Sep 19 17:16:48 PDT 2000
    copyright            : (C) 2000 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: main.cpp,v 1.28 2003/07/18 03:50:00 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/*Feeling left out? Get included!*/
#include <iostream>
#include <list>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SDL/SDL_image.h"
#include <SDL/SDL_thread.h>
#include "dfont.h"
#include "gameclock.h"
#include "submarine.h"
#include "towedarray.h"
#include "targetmotionanalysis.h"
#include "message.h"
#include "sonar.h"
#include "radar.h"
#include "files.h"
#include "esm.h"
#include "control.h"
#include "main.h"
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

//#######GRAPHIC PRIMITIVES#############
void SetupScreen(bool full_screen){
	//Initialize the screen and some default colors
	//string name = "LinuxSSN 0.0pre_alpha.2.8";


	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0 ) {
		cerr << "Couldn't initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}
	atexit(SDL_Quit);
	SDL_InitSubSystem(SDL_INIT_TIMER);
	if(full_screen){
		//Create main screen surface
		screen = SDL_SetVideoMode(1024, 768, 16, SDL_FULLSCREEN);
	}else{
		screen = SDL_SetVideoMode(1024, 768, 16, SDL_SWSURFACE);
	}

	if ( screen == NULL ) {
		cerr << "Couldn't set video mode..." << endl
		<< "OpenSSN requires 1024x768 @ 16 bpp" << endl
		<< SDL_GetError() << endl;
		exit(1);
	}
	SDL_WM_SetCaption("OpenSSN http://openssn.sf.net", "OpenSSN");

	//define some standard colors
	textcolor = SDL_MapRGB(screen->format, 192, 255, 6);
	black = SDL_MapRGB(screen->format, 0, 0, 0);
	white = SDL_MapRGB(screen->format, 255, 255, 255);
	red = SDL_MapRGB(screen->format, 255, 99, 20);
        dark_red = SDL_MapRGB(screen->format, 140, 80, 10);
	green = SDL_MapRGB(screen->format, 0, 255, 0);
        dark_green = SDL_MapRGB(screen->format, 0, 120, 0);
	yellow = SDL_MapRGB(screen->format, 216, 255, 0);
        brown = SDL_MapRGB(screen->format, 120, 140, 0);
        grey = SDL_MapRGB(screen->format, 180, 180, 180);
        dark_grey = SDL_MapRGB(screen->format, 100, 100, 100);
	// mapcolor = SDL_MapRGB(screen->format, 130, 201, 225);
        mapcolor = SDL_MapRGB(screen->format, 10, 10, 100);
}

void MapIcon(int x, int y, int ShipType, int Friend, Uint32 color){
  // Display NTDS symbols according to the following Legend
  // Hostile:  Submarine = Red Bottom Half Diamond, Surface = Red Full Diamond,
  //           Aircraft = Red Top Half Diamond

  // Friendly: Submarine = green Bottom SemiCircle, Surface = green Circle,
  //           Aircraft = green Top SemiCircle

  // Unknown:  Submarine = grey Bottom Half Box, Surface = grey Box,
  //           Aircraft = grey Top Half Box

  // Neutral: Submarine = Yellow Bottom Half box, Surface = Yellow box,
  //           Aircraft = Yellow Top Half box

  // Is it a submarine?
  if(ShipType == 0) {
    switch(Friend){
    case 0: //Foe??
      DrawDiamond(screen, x-2, y+2, 7, 'B', black);  //Bottom Half Diamond
      DrawDiamond(screen, x, y, 7, 'B', color);  //Bottom Half Diamond
      break;
    case 1: //Friend??
      DrawArc(screen, x-2,y+2, 7, 180, 360, black); //Bottom SemiCircle
      DrawArc(screen, x-2, y+3, 7, 180, 360, black); // thickness
      DrawArc(screen, x ,y, 7, 180, 360, color); //Bottom SemiCircle
      DrawArc(screen, x, y + 1, 7, 180, 360, color); // thickness
      break;
    case 2://Unknown??
      DrawBox(screen, x-2 ,y+2, 7, 'B', black); //Bottom Half Box
      DrawBox(screen, x ,y, 7, 'B', color); //Bottom Half Box
      break;
    default: //Neutral
       DrawBox(screen, x-2 ,y+2, 7, 'B', black); //Bottom Half Box
       DrawBox(screen, x ,y, 7, 'B', color); //Bottom Half Box
    }
  }
 // Is it a surface ship?
    else if(ShipType == 1) {
   switch(Friend){
   case 0: //Foe??
     DrawDiamond(screen, x-2 ,y+2, 7, 'F', black); //Diamond
     DrawDiamond(screen, x ,y, 7, 'F', color); //Diamond
     break;
   case 1: //Friend??
     DrawCircle(screen, x-2, y+2, 7, black); //Circle
     DrawCircle(screen, x, y, 7, color); //Circle
     break;
   case 2: //Unknown??
      DrawBox(screen, x-2 ,y+2, 7, 'F', black); //Box
      DrawBox(screen, x ,y, 7, 'F', color); //Box
      break;
   default: //Neutral
     DrawBox(screen, x-2 ,y+2, 7, 'F', black); //Cross
     DrawBox(screen, x ,y, 7, 'F', color); //Cross
    }
  }

  // Is it an airplane or Helo?
  else if(ShipType == 2) {
    switch(Friend){
    case 0: //Foe??
      DrawDiamond(screen, x, y, 7, 'T', color);  //Top Half Diamond
      break;
    case 1: //Friend??
      DrawArc(screen, x ,y, 7, 0, 180, color); //Top SemiCircle
      break;
    case 2: //Unknown??
      DrawBox(screen, x ,y, 7, 'T', color); //Top Half Box
      break;
    default: //Neutral
      DrawBox(screen, x ,y, 7, 'T', color); //Top Half Cross
    }
  }
  else if (ShipType == TYPE_TORPEDO)
  {
      DrawDiamond(screen, x, y, 7, 'B', color);
      DrawLine(screen, x, y-5, x, y, color);
  }
}


void DirectionalPointer(int x, int y, int heading, int speed,Uint32 color){
	//Draws a directional vane on a ships map icon.
	//the length will vary depending on the speed.

	double var1; //just a place to hold a value so we don't have to calc it twice.
	double destinationx; //the dest x&y point for
	double destinationy; //the drawn line.

	if((speed > 1) || (speed < -1)) speed=speed/2; //allow for 0 speed.
	var1 = heading * 0.017453; //convert degrees to radians.
	destinationx = (sin(var1) * speed) + x;
	destinationy = (-1) * (cos(var1) * speed) + y; 
	DrawLine(screen, x, y, (int)destinationx, (int)destinationy, color);
}

inline void DrawPixel(SDL_Surface *screen, int x, int y, Uint32 color){
	//this only works for 16bpp screens
	//are we outside the screen?????
	//If we are bail out now before it's too late!

	if (x > 1023 || x < 0 || y > 759 || y < 0) {
		return;
	}

	//place the pixel on the screen
	Uint16 *pixel_location;
	pixel_location = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
	*pixel_location = color;
}

void DrawLine(SDL_Surface *screen, int X1, int Y1, int X2, int Y2, Uint32 Color){

//don't even ask me about this stuff All I know is it works
//and thats ALL I care about...

	int dx,dy,sdx,sdy,py,px,x,y;
	dx = X2 - X1;
	dy = Y2 - Y1;
	if (dx < 0) sdx = -1;
	else sdx = 1;
	if (dy < 0) sdy = -1;
	else sdy = 1;
	dx = sdx * dx + 1;
	dy = sdy * dy + 1;
	x = 0;
	y = 0;
	px = X1;
	py = Y1;
	if (dx >= dy){
		for (int x = 0; x < dx; x++){
			DrawPixel(screen, px, py, Color);
			y = y + dy;
			if (y >= dx){
				y = y - dx;
				py = py + sdy;
			}
			px = px + sdx;
		}
	}else{
		for (int y = 0; y < dy; y++){
			DrawPixel(screen, px, py, Color);
			x = x + dx;
			if (x >= dy){
				x = x - dy;
				px = px + sdx;
			}
			py = py + sdy;
		}
	}
}


void DrawArc(SDL_Surface *screen, int X1, int Y1, int Radius, int Theta1, int Theta2, Uint32 Color){

//Draw an arc  at (X1,Y1) of a given radius from theta1 to theta2 using specified Color.
	int x, y, xc, yc, radius;
	int theta, theta1, theta2;
	xc = X1;
	yc = Y1;
	radius = Radius;
	theta1 = Theta1;
	theta2 = Theta2;

	for(theta=theta1;theta<=theta2;theta+=5) {
	  x = xc + int(radius*cos(theta*3.14/180.0));
	  y = yc - int(radius*sin(theta*3.14/180.0));
	  DrawPixel(screen, x, y, Color);
	}
}

void DrawCircle(SDL_Surface *screen, int X1, int Y1, int Radius, Uint32 Color){

//Draw a circle  at (X1,Y1) of a given radius using specified Color.
	int xc, yc, radius;
	xc = X1;
	yc = Y1;
	radius = Radius;
	DrawArc(screen, xc, yc, radius, 0, 360, Color);
}

void DrawDiamond(SDL_Surface *screen, int X1, int Y1, int Size, char Direction, Uint32 Color){

  //Draw a diamond  at (X1,Y1) of Size using specified Color.
  // Direction: T = Top, B = Bottom, F = Full
	int xc, yc;
	int size;
	char direction;
	xc = X1;
	yc = Y1;
	size = Size;
	direction = Direction;

	switch(direction){

	case 'B':
	  DrawLine(screen, xc-size, yc, xc, yc+size, Color); //bottomleft
	  DrawLine(screen, xc+size, yc, xc, yc+size, Color); //bottomright
	  break;

	case 'T':
	  DrawLine(screen, xc, yc-size, xc-size, yc, Color); //topleft
	  DrawLine(screen, xc, yc-size, xc+size, yc, Color); //topright
	  break;

	default:
	DrawLine(screen, xc-size, yc, xc, yc+size, Color); //bottomleft
	DrawLine(screen, xc+size, yc, xc, yc+size, Color); //bottomright
	DrawLine(screen, xc, yc-size, xc-size, yc, Color); //topleft
	DrawLine(screen, xc, yc-size, xc+size, yc, Color); //topright
	}
}

void DrawBox(SDL_Surface *screen, int X1, int Y1, int Size, char Direction, Uint32 Color){

  //Draw a Box  at (X1,Y1) of Size using specified Color.
  // Direction: T = Top, B = Bottom, F = Full
	int xc, yc;
	int size;
	char direction;
	xc = X1;
	yc = Y1;
	size = Size;
	direction = Direction;

	switch(direction){

	case 'T':
	  DrawLine(screen, xc-size, yc-size, xc-size, yc, Color);
	  DrawLine(screen, xc-size, yc-size, xc+size, yc-size, Color);
	  DrawLine(screen, xc+size, yc-size, xc+size, yc, Color);
	  break;

	case 'B':
	  DrawLine(screen, xc-size, yc, xc-size, yc+size, Color);
	  DrawLine(screen, xc-size, yc+size, xc+size, yc+size, Color);
	  DrawLine(screen, xc+size, yc, xc+size, yc+size, Color);
	  break;

	default:
	DrawLine(screen, xc-size, yc-size, xc+size, yc-size, Color);
	DrawLine(screen, xc+size, yc-size, xc+size, yc+size, Color);
	DrawLine(screen, xc-size, yc-size, xc-size, yc+size, Color);
	DrawLine(screen, xc-size, yc+size, xc+size, yc+size, Color);
	}
}


void DrawRectangle(SDL_Surface *screen, int x1, int y1, int x2, int y2, Uint32 color)
{
   DrawLine(screen, x1, y1, x1, y2, color);
   DrawLine(screen, x1, y1, x2, y1, color);
   DrawLine(screen, x1, y2, x2, y2, color);
   DrawLine(screen, x2, y1, x2, y2, color);
}


 
void DrawCross(SDL_Surface *screen, int X1, int Y1, int Size, char Direction, Uint32 Color){

  //Draw a Cross  at (X1,Y1) of Size using specified Color.
  // Direction: T = Top, B = Bottom, F = Full
	int xc, yc;
	int size;
	char direction;
	xc = X1;
	yc = Y1;
	size = Size;
	direction = Direction;

	switch(direction){

	case 'T':
	  DrawLine(screen, xc-size, yc-size/2, xc-size, yc, Color);
	  DrawLine(screen, xc+size, yc-size/2, xc+size, yc, Color);
	  DrawLine(screen, xc-size/2, yc-size, xc+size/2, yc-size, Color);
	  DrawLine(screen, xc-size/2, yc-size, xc-size/2, yc-size/2, Color);
	  DrawLine(screen, xc+size/2, yc-size, xc+size/2, yc-size/2, Color);
	  DrawLine(screen, xc+size/2, yc-size/2, xc+size, yc-size/2, Color);
	  DrawLine(screen, xc-size, yc-size/2, xc-size/2, yc-size/2, Color);
	  break;

	case 'B':
	  DrawLine(screen, xc-size, yc, xc-size, yc+size/2, Color);
	  DrawLine(screen, xc+size, yc, xc+size, yc+size/2, Color);
	  DrawLine(screen, xc-size/2, yc+size, xc+size/2, yc+size, Color);
	  DrawLine(screen, xc-size/2, yc+size/2, xc-size/2, yc+size, Color);
	  DrawLine(screen, xc+size/2, yc+size/2, xc+size/2, yc+size, Color);
	  DrawLine(screen, xc-size, yc+size/2, xc-size/2, yc+size/2, Color);
	  DrawLine(screen, xc+size/2, yc+size/2, xc+size, yc+size/2, Color);
	  break;

	default:
	  DrawLine(screen, xc-size, yc-size/2, xc-size, yc+size/2, Color);
	  DrawLine(screen, xc+size, yc-size/2, xc+size, yc+size/2, Color);
	  DrawLine(screen, xc-size/2, yc-size, xc+size/2, yc-size, Color);
	  DrawLine(screen, xc-size/2, yc+size, xc+size/2, yc+size, Color);
	  DrawLine(screen, xc-size/2, yc-size, xc-size/2, yc-size/2, Color);
	  DrawLine(screen, xc-size/2, yc+size/2, xc-size/2, yc+size, Color);
	  DrawLine(screen, xc+size/2, yc-size, xc+size/2, yc-size/2, Color);
	  DrawLine(screen, xc+size/2, yc+size/2, xc+size/2, yc+size, Color);
	  DrawLine(screen, xc-size, yc+size/2, xc-size/2, yc+size/2, Color);
	  DrawLine(screen, xc+size/2, yc-size/2, xc+size, yc-size/2, Color);
	  DrawLine(screen, xc-size, yc-size/2, xc-size/2, yc-size/2, Color);
	  DrawLine(screen, xc+size/2, yc+size/2, xc+size, yc+size/2, Color);
	}
}


void LoadScreen(int item){
	//Loads a PN file into an SDL surface then
	//blits it to the display surface.
	//then deletes the temporary surface.


	SDL_Surface *IMGFile=NULL; // Get our selfs a surface to work with..
	SDL_Rect dest; //A rectangle structure to define a blit area
	switch (item)
	{
	case 0:
		IMGFile = Load_Image("images/tittle.png"); //fill the surface w/ the bmp
		break;
	case 1:
		IMGFile = Load_Image("images/Sonarscreen.png"); //fill the surface w/ the bmp
		break;
	case 2:
		IMGFile = Load_Image("images/Mapscreen.png"); //fill the surface w/ the bmp
		break;
	case 3:
		IMGFile = Load_Image("images/Mapscreen.png"); //fill the surface w/ the bmp
		break;
	case 4:
		IMGFile = Load_Image("images/ControlScreen.png"); //fill the surface w/ the bmp
		break;
	case 5:
		IMGFile = Load_Image("images/RadarScreen.png"); //fill the surface w/ the bmp
		break;
	case 6:
		IMGFile = Load_Image("images/ESMScreen.png"); //fill the surface w/ the bmp
		break;
	default:
		cerr << "Unknown screen item" << endl;
		break;
	}
	if ( IMGFile == NULL ) { // Are we screwed??
		cerr << "Function LoadScreen()" << endl
				<< "Couldn't load screen item# " << item <<endl
				<< "SDL Error: " << SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
		return; // Whine Snivel and Bail..
	}
	dest.x = 0; // Blit destination x&y to the upper left
	dest.y = 0;
	dest.w = IMGFile->w; // Height and width equal to the
	dest.h = IMGFile->h;  // source images....
	SDL_BlitSurface(IMGFile, NULL, screen, &dest); // Do the actual Blit
	SDL_UpdateRects(screen, 1, &dest); //Show the screen...
	SDL_FreeSurface(IMGFile); //Free up the surface memory..
	return;
}

void LoadWidgets(){
	SDL_Surface *temp;

	temp = Load_Image("images/mapup.png");
	if(temp != NULL) navbuttonup = SDL_DisplayFormat(temp);
	if ( navbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/mapdown.png");
	if(temp != NULL) navbuttondown = SDL_DisplayFormat(temp);
	if ( navbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/tmaup.png");
	if(temp != NULL) tmabuttonup = SDL_DisplayFormat(temp);
	if ( tmabuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/tmadown.png");
	if(temp != NULL) tmabuttondown = SDL_DisplayFormat(temp);
	if ( tmabuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/shipcontrolup.png");
	if(temp != NULL) shipcontrolbuttonup = SDL_DisplayFormat(temp);
	if ( shipcontrolbuttonup == NULL ) { // Are we screwed??

		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/shipcontroldown.png");
	if(temp != NULL) shipcontrolbuttondown = SDL_DisplayFormat(temp);
	if ( shipcontrolbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/esmup.png");
	if(temp != NULL) esmbuttonup = SDL_DisplayFormat(temp);
	if ( esmbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/esmdown.png");
	if(temp != NULL) esmbuttondown = SDL_DisplayFormat(temp);
	if ( esmbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/radarup.png");
	if(temp != NULL) radarbuttonup = SDL_DisplayFormat(temp);
	if ( radarbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/radardown.png");
	if(temp != NULL) radarbuttondown = SDL_DisplayFormat(temp);
	if ( radarbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/quitbuttonup.png");
	if(temp != NULL) quitbuttonup = SDL_DisplayFormat(temp);
	if ( quitbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/quitbuttondown.png");
	if(temp != NULL) quitbuttondown = SDL_DisplayFormat(temp);
	if ( quitbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/plusbuttonup.png");
	if(temp != NULL) plusbuttonup = SDL_DisplayFormat(temp);
	if ( quitbuttonup == NULL ) { // Are we screwed??

		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/plusbuttondown.png");
	if(temp != NULL) plusbuttondown = SDL_DisplayFormat(temp);
	if ( quitbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/minusbuttonup.png");
	if(temp != NULL) minusbuttonup = SDL_DisplayFormat(temp);
	if ( minusbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/minusbuttondown.png");
	if(temp != NULL) minusbuttondown = SDL_DisplayFormat(temp);
	if ( minusbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/centerbuttondown.png");
	if(temp != NULL) centerbuttondown = SDL_DisplayFormat(temp);
	if ( centerbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/centerbuttonup.png");
	if(temp != NULL) centerbuttonup = SDL_DisplayFormat(temp);
	if ( centerbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/upbuttondown.png");
	if(temp != NULL) upbuttondown = SDL_DisplayFormat(temp);
	if ( upbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/upbuttonup.png");
	if(temp != NULL) upbuttonup = SDL_DisplayFormat(temp);
	if ( upbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/downbuttondown.png");
	if(temp != NULL) downbuttondown = SDL_DisplayFormat(temp);
	if ( centerbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/downbuttonup.png");
	if(temp != NULL) downbuttonup = SDL_DisplayFormat(temp);
	if ( downbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/leftbuttondown.png");
	if(temp != NULL) leftbuttondown = SDL_DisplayFormat(temp);
	if ( leftbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/leftbuttonup.png");
	if(temp != NULL) leftbuttonup = SDL_DisplayFormat(temp);
	if ( leftbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/rightbuttondown.png");
	if(temp != NULL) rightbuttondown = SDL_DisplayFormat(temp);
	if ( rightbuttondown == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

	temp = Load_Image("images/rightbuttonup.png");
	if(temp != NULL) rightbuttonup = SDL_DisplayFormat(temp);
	if ( rightbuttonup == NULL ) { // Are we screwed??
		cerr<<"Function LoadWidgets()" << endl
		<< SDL_GetError() << endl;
		SDL_Quit();
		exit(0);
	}
	SDL_FreeSurface(temp);

        temp = Load_Image("images/noisemaker.png");
        if (temp)
        {
           noisemaker_image = SDL_DisplayFormat(temp);
           if (! noisemaker_image)
             cerr << "Error loading noisemaker image." << endl;
           SDL_FreeSurface(temp);
        }

        temp = Load_Image("images/torpedo.png");
        if (temp)
        {
           torpedo_image = SDL_DisplayFormat(temp);
           if (! noisemaker_image)
             cerr << "Error loading torpedo image." << endl;
           SDL_FreeSurface(temp);
        }

}



void UnLoadWidgets(){
	
	//free the surfaces

	SDL_FreeSurface(sonarbuttonup);
	SDL_FreeSurface(sonarbuttondown); 
	SDL_FreeSurface(navbuttonup);
	SDL_FreeSurface(navbuttondown);
	SDL_FreeSurface(tmabuttonup);
	SDL_FreeSurface(tmabuttondown);
	SDL_FreeSurface(quitbuttonup);
	SDL_FreeSurface(quitbuttondown); 
	SDL_FreeSurface(plusbuttonup);
	SDL_FreeSurface(plusbuttondown); 
	SDL_FreeSurface(minusbuttonup);
	SDL_FreeSurface(minusbuttondown); 
        SDL_FreeSurface(torpedo_image);
        SDL_FreeSurface(noisemaker_image);
}

void DisplayNavigationWidgets(){	
	if (navwidget){ // is the nav button down?
		destination_rectangle.x = 280; //upper left corner to
		destination_rectangle.y = 710; //place the button.
		destination_rectangle.h = navbuttondown->h; //height &
		destination_rectangle.w = navbuttondown->w;//width of button.
		SDL_BlitSurface(navbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
		SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		if(mapcenter){
			destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = centerbuttondown->h; //height &
			destination_rectangle.w = centerbuttondown->w;//width of button.
			SDL_BlitSurface(centerbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		}else{
			destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = centerbuttonup->h; //height &
			destination_rectangle.w = centerbuttonup->w;//width of button.
			SDL_BlitSurface(centerbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		}
		 	destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 219; //place the button.
			destination_rectangle.h = upbuttonup->h; //height &
			destination_rectangle.w = upbuttonup->w;//width of button.
			SDL_BlitSurface(upbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		 	destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 319; //place the button.
			destination_rectangle.h = downbuttonup->h; //height &
			destination_rectangle.w = downbuttonup->w;//width of button.
			SDL_BlitSurface(downbuttonup, NULL, screen, &destination_rectangle); // Do the Blit
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen..
		 	destination_rectangle.x = 175; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = leftbuttonup->h; //height &
			destination_rectangle.w = leftbuttonup->w;//width of button.
			SDL_BlitSurface(leftbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
		 	destination_rectangle.x = 275; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = rightbuttonup->h; //height &
			destination_rectangle.w = rightbuttonup->w;//width of button.
			SDL_BlitSurface(rightbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		 	destination_rectangle.x = 175; //upper left corner to
			destination_rectangle.y = 359; //place the button.
			destination_rectangle.h = plusbuttonup->h; //height &
			destination_rectangle.w = plusbuttonup->w;//width of button.
			SDL_BlitSurface(plusbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
		 	destination_rectangle.x = 275; //upper left corner to
			destination_rectangle.y = 359; //place the button.
			destination_rectangle.h = minusbuttonup->h; //height &
			destination_rectangle.w = minusbuttonup->w;//width of button.
			SDL_BlitSurface(minusbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
	}else{ // our button must be up...
 		destination_rectangle.x = 280; //upper left corner to
		destination_rectangle.y = 710; //place the button.
		destination_rectangle.h = navbuttonup->h; //height &
		destination_rectangle.w = navbuttonup->w;//width of button.
		SDL_BlitSurface(navbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
		SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
	}
}

void DisplayTMAWidgets(){
	if (tmawidget){ // is the TMA button down?
	 	destination_rectangle.x = 380; //upper left corner to
		destination_rectangle.y = 710; //place the button.
		destination_rectangle.h = tmabuttondown->h; //height &
		destination_rectangle.w = tmabuttondown->w;//width of button.
		SDL_BlitSurface(tmabuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
		SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		if(Tma.centerGeoPlot){
			destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = centerbuttondown->h; //height &
			destination_rectangle.w = centerbuttondown->w;//width of button.
			SDL_BlitSurface(centerbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		}else{
			destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = centerbuttonup->h; //height &
			destination_rectangle.w = centerbuttonup->w;//width of button.
			SDL_BlitSurface(centerbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		}
		 	destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 219; //place the button.
			destination_rectangle.h = upbuttonup->h; //height &
			destination_rectangle.w = upbuttonup->w;//width of button.
			SDL_BlitSurface(upbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		 	destination_rectangle.x = 225; //upper left corner to
			destination_rectangle.y = 319; //place the button.
			destination_rectangle.h = downbuttonup->h; //height &
			destination_rectangle.w = downbuttonup->w;//width of button.
			SDL_BlitSurface(downbuttonup, NULL, screen, &destination_rectangle); // Do the Blit
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen..
		 	destination_rectangle.x = 175; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = leftbuttonup->h; //height &
			destination_rectangle.w = leftbuttonup->w;//width of button.
			SDL_BlitSurface(leftbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
		 	destination_rectangle.x = 275; //upper left corner to
			destination_rectangle.y = 269; //place the button.
			destination_rectangle.h = rightbuttonup->h; //height &
			destination_rectangle.w = rightbuttonup->w;//width of button.
			SDL_BlitSurface(rightbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
		 	destination_rectangle.x = 175; //upper left corner to
			destination_rectangle.y = 359; //place the button.
			destination_rectangle.h = plusbuttonup->h; //height &
			destination_rectangle.w = plusbuttonup->w;//width of button.
			SDL_BlitSurface(plusbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
		 	destination_rectangle.x = 275; //upper left corner to
			destination_rectangle.y = 359; //place the button.
			destination_rectangle.h = minusbuttonup->h; //height &
			destination_rectangle.w = minusbuttonup->w;//width of button.
			SDL_BlitSurface(minusbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
			SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
	}else{ // our button must be up...
		destination_rectangle.x = 380; //upper left corner to
		destination_rectangle.y = 710; //place the button.
		destination_rectangle.h = tmabuttonup->h; //height &
		destination_rectangle.w = tmabuttonup->w;//width of button.
		SDL_BlitSurface(tmabuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
		SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
	}
}

void DisplayESMWidgets(){
 	destination_rectangle.x = 580; //upper left corner to
	destination_rectangle.y = 710; //place the button.

	if (esmwidget){ // is the nav button down?
		destination_rectangle.h =esmbuttondown->h; //height &
		destination_rectangle.w = esmbuttondown->w;//width of button.
		SDL_BlitSurface(esmbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
	}else{ // our button must be up...
		destination_rectangle.h = esmbuttonup->h; //height &
		destination_rectangle.w = esmbuttonup->w;//width of button.
		SDL_BlitSurface(esmbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
	}
	SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
}

void DisplayRADARWidgets(){
 	destination_rectangle.x = 680; //upper left corner to
	destination_rectangle.y = 710; //place the button.

	if (radarwidget){ // is the nav button down?
		destination_rectangle.h =radarbuttondown->h; //height &
		destination_rectangle.w = radarbuttondown->w;//width of button.
		SDL_BlitSurface(radarbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
	}else{ // our button must be up...
		destination_rectangle.h = radarbuttonup->h; //height &
		destination_rectangle.w = radarbuttonup->w;//width of button.
		SDL_BlitSurface(radarbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
	}
	SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
}

void DisplayShipControlWidgets(){
 	destination_rectangle.x = 480; //upper left corner to
	destination_rectangle.y = 710; //place the button.

	if (shipcontrolwidget){ // is the nav button down?
		destination_rectangle.h = shipcontrolbuttondown->h; //height &
		destination_rectangle.w = shipcontrolbuttondown->w;//width of button.
		SDL_BlitSurface(shipcontrolbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
	}else{ // our button must be up...
		destination_rectangle.h = shipcontrolbuttonup->h; //height &
		destination_rectangle.w = shipcontrolbuttonup->w;//width of button.
		SDL_BlitSurface(shipcontrolbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
	}
	SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
}
void DisplayWidgets(){
	SonarStation.DisplaySonarWidgets();
	DisplayNavigationWidgets();
	DisplayTMAWidgets();
	DisplayShipControlWidgets();
	DisplayESMWidgets();
	DisplayRADARWidgets();

	destination_rectangle.x = 780; //upper left corner to
	destination_rectangle.y = 710; //place the button.
	if (quitwidget){ // is the quit button down?
		destination_rectangle.h = quitbuttondown->h; //height &
		destination_rectangle.w = quitbuttondown->w;//width of button.
		SDL_BlitSurface(quitbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
	}else{ // our button must be up...
		destination_rectangle.h = quitbuttonup->h; //height &
		destination_rectangle.w = quitbuttonup->w;//width of button.
		SDL_BlitSurface(quitbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
	}
	SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...

	destination_rectangle.x = 880; //upper left corner to
	destination_rectangle.y = 710; //place the button.
	destination_rectangle.h = plusbuttonup->h; //height &
	destination_rectangle.w = plusbuttonup->w;//width of button.
	SDL_BlitSurface(plusbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
	SDL_UpdateRects(screen, 1, &destination_rectangle); //Show button on the screen...
	destination_rectangle.x = 970; //upper left corner to
	destination_rectangle.y = 710; //place the button.
	destination_rectangle.h = minusbuttonup->h; //height &
	destination_rectangle.w = minusbuttonup->w;//width of button.
	SDL_BlitSurface(minusbuttonup, NULL, screen, &destination_rectangle); // Do the Blit.
	SDL_UpdateRects(screen, 1, &destination_rectangle); //Show button on the screen...

}



// This function adds a torpedo to the linked-list of torpedoes
// The returned value is the new linked-list pointer, in case
// the new_torpedo is the first in the list.
Submarine *Add_Torpedo(Submarine *all_torp, Submarine *new_torp)
{
   Submarine *my_torp;

   // we do not have a torpedo to add
   if (! new_torp)
     return all_torp;

   // empty list
   if (! all_torp)
       return new_torp;

   // add one to an existing list
   my_torp = all_torp;
   while (my_torp->next)
      my_torp = my_torp->next;

   // add to end of list
   my_torp->next = new_torp;
   
   return all_torp;
}


// This function removes a torpedo from the list of all
// torpedoes. It returns the first item in the torpedo linked-list.
Submarine *Remove_Torpedo(Submarine *all_torpedoes, Submarine *old_torpedo)
{
   Submarine *my_torp, *previous;

   if (! all_torpedoes)
      return NULL;
   if (! old_torpedo)
      return all_torpedoes;

   my_torp = all_torpedoes;
   previous = NULL;
   while (my_torp)
   {
      // found match and it is the first in the list
      if ( (my_torp == old_torpedo) && (! previous) )
      {
          previous = my_torp->next;
          free(my_torp);
          return previous;
      }
      // found a match and it isn't the first in the list
      else if (my_torp == old_torpedo)
      {
         previous->next = my_torp->next;
         free(my_torp);
         return all_torpedoes;
      }
      // no match found
      else
      {
          previous = my_torp;
          my_torp = my_torp->next;
      }
      
   }
   // item wasn't found, just return the complete list
   return all_torpedoes;
}





//######################################
void ShipHandeling(){
       Submarine *my_torp, *temp_torp;
       int status;

       // see if we can use radar, esm, etc
       if ( Subs[0].Depth > PERISCOPE_DEPTH)
       {
           EsmStation.LowerMast();
           RadarStation.LowerMast();
       }

	for (int x = 0; x < ships; x++){
		Subs[x].UpdateLatLon();	//Move all the ships
                // see what the AI wants to do
                if (x > 0)
                {
                  if (Subs[x].ShipType == TYPE_SUB)
                    Subs[x].Sub_AI(torpedoes);
                  else if (Subs[x].ShipType == TYPE_SHIP)
                    Subs[x].Ship_AI(torpedoes);
                }
		Subs[x].Handeling();	//Steer, Change Depth etc...
	}
	for (int x = 0; x < 19; x++){
		Contacts[x].UpdateContact();	
	}

        // move some torps
        my_torp = torpedoes;
        while (my_torp)
        {
           my_torp->UpdateLatLon();  
           my_torp->Torpedo_AI();   // see where we should be going
           my_torp->Handeling();    // change heading and depth
           status = my_torp->Check_Status();  // see if we ran into something
                                              // or we are out of fuel
           if (status == OUT_OF_FUEL)
           {
               temp_torp = my_torp->next;
               torpedoes = Remove_Torpedo(torpedoes, my_torp);
               my_torp = temp_torp;
               Message.post_message("A torpedo ran out of fuel.");
               Message.display_message();
           }
           else if (status == HIT_TARGET)
           {
               int target_status;
               // damage target
               target_status = my_torp->target->Take_Damage();
               if (target_status == DAMAGE_SINK)
               {
                   Remove_Inactive_Ship(my_torp->target);
               }
               temp_torp = my_torp->next;
               torpedoes = Remove_Torpedo(torpedoes, my_torp);
               my_torp = temp_torp;
               Message.post_message("A torpedo hit its target!");
               if (target_status == DAMAGE_SINK)
                  Message.post_message("Target is sinking!");
               Message.display_message();
           }
           else
              my_torp = my_torp->next;
        }   // end of all torpedoes
}



// This function may get a little messy, we need to do a few things
// to remove a ship completely from the scenario.
// 1. We find the index number of the inactive ship.
// 2. Any torpedoes shooting at this target have to have their
//    target variable set to NULL
// 3. Any ships/subs who were tracking this ship need to stop
//    (we reset the targets and target_strength variables)
// 4. We move all ships _later_ in the list down one place in the Subs index
void Remove_Inactive_Ship(Submarine *victim)
{
    int ship_index = 0, found = FALSE;
    Submarine *torpedo;
    int index;

    // 1. find the ship's index
    while ( (ship_index < MAX_SUBS) && (!found) )
    {
        if ( Subs[ship_index].Active )
          ship_index++;
        else
           found = TRUE;
    }

    if (! found)   // couldn't find victim, this should never happen, quit
        return;

    // 2. Cancel torpedoes targetting this ship
    torpedo = torpedoes;
    while (torpedo)
    {
        if (torpedo->target == victim)
          torpedo->target = NULL;
        torpedo = torpedo->next;
    }

    // 3. Ships who were tracking this ship need to stop
    for (index = 0; index < MAX_SUBS; index++)
    {
        if ( Subs[index].Active )
           Subs[index].Cancel_Target(ship_index);
    }

    // 4. Move all ships down onein the list
    index = ship_index;
    while (index < (MAX_SUBS - 1) )
    {
       memcpy( &(Subs[index]), &(Subs[index + 1]), sizeof(Submarine) );
       index++;
    }
    memset(&(Subs[MAX_SUBS - 1]), 0, sizeof(Submarine));
    ships--;
}






double RelativeBearing(int observer, int target){
	// Returns the relative bearing of target to observer. This bearing is
	//measured clockwise from on ships heading to target bearing.
	double relative_bearing;
	int observer_heading, bearing_to_target;
	bearing_to_target = (int)Subs[observer].BearingToTarget(& (Subs[target]) );
	observer_heading = (int)Subs[observer].Heading;
	if(observer_heading > bearing_to_target) bearing_to_target += 360;
	relative_bearing = bearing_to_target - observer_heading;
	return relative_bearing;
}

int ReciprocalBearing(int bearing){
	// returns the 180 degree opposite of the bearing given to it..
	int recipbearing;
	if (bearing >= 180){
		 recipbearing = bearing - 180;
	}else{
		recipbearing = bearing + 180;
	}
	return recipbearing;
}

double AngleOnBow(int observer, int target){
	// Returns the Angle on the Bow (AOB) of target relative to observer.
	// Note: -180 < Angle on Bow < 180
	// Positive AOB = Port AOB, Negative AOB = Starboard AOB
	// Examples:
	// target AOB port 30 -> +30
	// target AOB starboard 30 -> -30
	// target pointing at observer -> 0
	// target pointing away from observer -> -180
	double aob=0;
	aob = Subs[target].Heading - Subs[observer].Heading
	-	RelativeBearing(observer,target) - 180;
	if (aob > 180) aob = 360 - aob;
	if (aob < -180) aob = 360 + aob;
	return aob;
}

int minimize360 ( int compass_bearing ){
	// If course is bigger than 360 degrees, then subtract 360 degrees from it
	// and keep doing so until it is less than 360 degrees.
	while ( compass_bearing > 360 )
	{
		compass_bearing -= 360;
	}
	return compass_bearing;
}

int maximize360 ( int compass_bearing ){
	// If course is negative then, then add 360 degrees to it
	// and keep doing so until it is positive.
	while ( compass_bearing < 0 )
	{
		compass_bearing += 360;
	}
	return compass_bearing;
}

double CalculateRange(int observer, int target){
	//Calculates the distance in yards from the observer, to the target.

	double latdif = 0, londif = 0; //sqrt needs doubles
	LatLonDifference( observer,  target,  &latdif,  &londif );
	return sqrt((latdif * latdif) + (londif * londif));
}

void LatLonDifference(int x, int y, double *platdif, double *plondif){ // CHANGE TO REFERENCES
	//Returns in yards the difference between object x and object y

	if (Subs[x].Lat_TotalYards > Subs[y].Lat_TotalYards){
		*platdif = Subs[x].Lat_TotalYards - Subs[y].Lat_TotalYards;
	}
	else{
		*platdif = Subs[y].Lat_TotalYards - Subs[x].Lat_TotalYards;
	}

	if (Subs[x].Lon_TotalYards > Subs[y].Lon_TotalYards){
		*plondif = Subs[x].Lon_TotalYards - Subs[y].Lon_TotalYards;
	}
	else{
		*plondif = Subs[y].Lon_TotalYards - Subs[x].Lon_TotalYards;
	}
}

void CreateShips(int mission_number){
  char *ship_file, *mission_name;
  char filename[128];
  char line[256], *status;
  int i;
  FILE *my_file, *mission_file;

  snprintf(filename, 128, "data/ships%d.dat", mission_number);
  // ifstream constructor opens file
  ship_file = Find_Data_File(filename);
  my_file = fopen(ship_file, "r");
  if ( (ship_file) && (ship_file != filename) )
     free(ship_file);

  // idiot check
  if(!my_file){
    cerr << "Create ships file missing \n";
    exit(1);
  }

  snprintf(filename, 128, "data/mission%d.dat", mission_number);
  mission_name = Find_Data_File(filename);
  mission_file = fopen(mission_name, "r");
  if ( (mission_name) && (mission_name != filename) )
    free(mission_name);

  
  // init all ships
  for (i = 0; i < MAX_SUBS; i++)
  {
      Subs[i].Init();
      Subs[i].Load_Mission(mission_file);
  }

  // Read in the data from the ship file
  // Format of ship file: 1 row of data per ship
  // Active flag, Ship Type, Friend flag, Speed, DesiredSpeed, Depth, 
  // Desired depth, Heading, Desired
  // Heading, Rudder, Lat_TotalYards, Lon_TotalYards, PSCS

  // Notes: Ship Type: Sub=0, Surface=1, Aircraft=2
  //        Friend: Foe=0, Friend=1, Unknown=2, Neutral=3
  i = 0;
  status = fgets(line, 256, my_file);
  while (status)
  {
    sscanf(line, "%d %d %d %d %f %f %f %f %f %f",
                  &(Subs[i].Active), &(Subs[i].ShipType),
                  &(Subs[i].ShipClass), &(Subs[i].Friend),
                  &(Subs[i].Speed), &(Subs[i].Depth),
                  &(Subs[i].Heading), &(Subs[i].Lat_TotalYards),
                  &(Subs[i].Lon_TotalYards), &(Subs[i].PSCS) );

    /*
    inClientFile >> Subs[i].Active >> Subs[i].ShipType 
                 >> Subs[i].ShipClass >> Subs[i].Friend
		 >> Subs[i].Speed >> Subs[i].Depth
		 >> Subs[i].Heading
		 >> Subs[i].Lat_TotalYards >> Subs[i].Lon_TotalYards
		 >> Subs[i].PSCS;
     */
     Subs[i].DesiredSpeed = (int) Subs[i].Speed;
     Subs[i].DesiredDepth = (int) Subs[i].Depth;
     Subs[i].DesiredHeading = (int) Subs[i].Heading;
     sprintf(filename, "ships/class%d.shp", Subs[i].ShipClass);
     ship_file = Find_Data_File(filename);
     Subs[i].Load_Class(ship_file);
     if ( (ship_file) && (ship_file != filename) )
        free(ship_file);
     i+=1;
     status = fgets(line, 256, my_file);
  }
  // inClientFile.close();
  if (my_file)
     fclose(my_file);
  if (mission_file)
     fclose(mission_file);

  // ships = i - 1;
  ships = i;
  // rdm 5/15/01 testing to be sure correct number of ships being read
  // cout << " Number of ships = " <<  i-1 << endl;
}

void UpdateSensors(){
//Added code to place our boats position into a FIFO so we
//can calculate where the sonar array is at any time.
//the cable on the TB16 is 2600ft. wich is 866yds. The FIFO is
//100 elements holding 2 double precision floats. So 866/100
//Is 8.66, Which is the resolution of our placement of the array.
	static float distance_traveled;
	distance_traveled += Subs[0].Speed * 0.555;
	if(distance_traveled >= 8.66){ //if we've gone 8.66yds record our loc in the FIFO
		TB16.RecordPos(Subs[0].Lat_TotalYards,  Subs[0].Lon_TotalYards);
		distance_traveled = distance_traveled - 8.66;
	}
	TB16.OperateWinch();//extend/retract the array as needed.
	SoundEnvironment(); //lets give a listen...
	SonarStation.Sonar(SonarStation.GetNorthCenterState());
	SonarStation.TowedSonar(SonarStation.GetNorthCenterState());
}

void UpdateDisplay(){
	if (drawmap){
                Display_Target();
		DrawMap(); // fix me to do something useful!
	}
	if (drawsonar){
		SonarStation.UpdateDisplay();
	}
	if (drawtma){
		DisplayTMA();
	}
        if (drawweapons)
            DisplayWeapons();
	if (drawradar){
		RadarStation.DisplayContacts(ships);	
	}
	if (drawesm){
		EsmStation.DisplayContacts(ships);			
	}
	if (drawcontrol){
		ControlStation.Display();			
	}

}



// Show information on the currently selected target
void Display_Target()
{
   char buffer[256];
   SDL_Rect rectangle;
   DFont fnt("images/font.png", "data/font.dat");
   float range;
   double bearing;

   if (current_target == -1)
      return;

   if (! Subs[current_target].Active)
      return;

   // make empty box to the side of the screen
   rectangle.x = 120;
   rectangle.y = 400;
   rectangle.w = 200;
   rectangle.h = 160;
   SDL_FillRect(screen, &rectangle, black);
 
   // fill in data
   sprintf(buffer, "   Target");
   fnt.PutString(screen, 150, 400, buffer);
   sprintf(buffer, "Heading: %d", (int) Subs[current_target].Heading);
   fnt.PutString(screen, 140, 424, buffer);
   
   sprintf(buffer, "Spead: %d knots", (int) Subs[current_target].Speed);
   fnt.PutString(screen, 140, 436, buffer);

   range = Subs[0].DistanceToTarget(& (Subs[current_target]) );
   // range *= 0.000568;
   range *= YARDS_TO_MILES;
   sprintf(buffer, "Range: %2.1f miles", range);
   fnt.PutString(screen, 140, 448, buffer);

   bearing = Subs[0].BearingToTarget(& (Subs[current_target]) );
   sprintf(buffer, "Bearing: %2.0lf", bearing);
   fnt.PutString(screen, 140, 460, buffer); 

   sprintf(buffer, "Depth: %d feet", (int) Subs[current_target].Depth);
   fnt.PutString(screen, 140, 472, buffer);

   sprintf(buffer, "Type: %s", Subs[current_target].ClassName );
   fnt.PutString(screen, 140, 484, buffer);

   SDL_UpdateRects(screen, 1, &rectangle); 
}




void DrawMap(){
	//Future home of an actual map display routine, right now just a blank field with dots

	SDL_Rect rectangle;
	rectangle.x=374;
	rectangle.y=145;   //define a rectangle on the screen and make it blue
	rectangle.h=500;
	rectangle.w=501;
	SDL_FillRect(screen, &rectangle, mapcolor);

	for (int x=374; x<874; x+= 10){ //Make meaningless dots grid on "Map"
		for (int y=145; y<637; y+= 10){
			DrawPixel(screen, x+5, y+5, white);
		}
	}
	PlaceShips(mapscale, 0, 0, current_target);
	SDL_UpdateRects(screen, 1, &rectangle);

}

void PlaceShips(int scale, int change_scrollx, int change_scrolly, int current_target){
	//Places all Ships onto the map. Soon to change, so only registered contacts appear.
	//scale is in YDS per pixel
        int status, fresh;
	int x, y; //where to place the ships
	int xoffset = 374;  //offsets to move the ships to
	int yoffset = 145;  //the defined place on map screen
	static int scrolloffsetx=0; //offset to center map
	static int scrolloffsety=0;
	Uint32 color;
        Submarine *a_torp;

	scale = scale * MAP_FACTOR;
	if(mapcenter){ //center map onto our own ntds symbol
		scrolloffsetx = 250 - ((int)Subs[0].Lat_TotalYards / scale);
		scrolloffsety = 250 - ((int)Subs[0].Lon_TotalYards / scale);
	}else{
		scrolloffsetx += change_scrollx;
		scrolloffsety += change_scrolly;
	}
	for (int shipnum = 0; shipnum<ships; shipnum++)
        {
             status = Subs[0].Can_Detect(shipnum);
             if ( (status >= 0) || (shipnum == 0) )
             {
		x = 500 - ((int)Subs[shipnum].Lat_TotalYards / scale);
		x = x - scrolloffsetx;
		y = 500 - ((int)Subs[shipnum].Lon_TotalYards / scale);
		y = y - scrolloffsety;
		if (x>10 && x<490 && y>10 && y<490){ //are we going to fall off the damn map???
			x = x + xoffset;
			y = y + yoffset;
                        // not only do we need friend/foe, but
                        // also fresh/old contact
                        fresh = Subs[0].target_strength[status];
			switch(Subs[shipnum].Friend){
				case 0: //Foe??
                                        if (fresh >= CONTACT_SOLID)
					  color = red;
                                        else
                                          color = dark_red;
					break;
				case 1: //Friend??
                                        if (fresh >= CONTACT_SOLID)
					  color = green;
                                        else
                                          color = dark_green;
					break;
				case 2: // Neither???
                                        if (fresh >= CONTACT_SOLID)
                       			   color = yellow;
                                        else
                                           color = brown;
                                        break;
				default: // Unknown
                                        if (fresh >= CONTACT_SOLID)
                                           color = grey;
                                        else
                                           color = dark_grey;
					break;
			}
			if (shipnum == 0){  // Is it me???
				color = green;
			}
                        if ( (fresh >= CONTACT_WEAK) || (shipnum == 0) )
                        {
			  MapIcon(x, y, (int)Subs[shipnum].ShipType, (int)Subs[shipnum].Friend, color); //Draw the NTDS symbol.
                          // check to see if we should highlight
                          if (current_target == shipnum)
                             MapIcon(x, y+1, (int)Subs[shipnum].ShipType, (int)Subs[shipnum].Friend, color);
			  DirectionalPointer(x-2, y+2, (int)Subs[shipnum].Heading, (int)Subs[shipnum].Speed, black);
			  DirectionalPointer(x, y, (int)Subs[shipnum].Heading, (int)Subs[shipnum].Speed, color);
			// Add pointer the show heading.
                        }   // end of we can hear you ok
		}   // end of we are on the map
             }    // end of able to detect
	}

        // now place torpedoes
        a_torp = torpedoes;
        while (a_torp)
        {
           x = 500 - ((int)a_torp->Lat_TotalYards / scale);
           x = x - scrolloffsetx;
           y = 500 - ((int)a_torp->Lon_TotalYards / scale);
           y = y - scrolloffsety;
           if (x>10 && x<490 && y>10 && y<490)  //are we going to fall off  map??
           {
              x = x + xoffset;
              y = y + yoffset;
              switch(a_torp->Friend){
                 case 0: //Foe??
                         color = red;
                         break;
                 case 1: //Friend??
                         color = green;
                         break;
                 case 2: // Neither???
                         color = yellow;
                         break;
                 default: // Unknown
                          color = grey;
                          break;
               }
               MapIcon(x, y, (int)a_torp->ShipType, (int)a_torp->Friend, color); 
           }   // within map limits
             
           a_torp = a_torp->next;
        }  // end of displaying torpedoes
}



// calls the other detection functions to see if there is
// any way we can pick up the target
float Any_Detection(double Range, int observer, int target)
{
    float status;

    status = Radar_Detection(Range, observer, target);
    if (status)
    {
      // printf("I can see on radar target %d\n", target);
      return 2.0;
    }
    status = Esm_Detection(Range, observer, target);
    if (status)
    {
      // printf("I can pick up on esm target %d\n", target);
      return 1.0;
    }
    // this one takes the most math, so we do it last
    // status = Sonar_Detection(Range, observer, target);
    status = Sonar_Detection_New(Range, &(Subs[observer]), &(Subs[target]) );
    // if (status)
    //   printf("I can hear target %d\n", target);
    return status;
}


// Can I see you on radar?
float Radar_Detection(double Range, int observer, int target)
{
   int depth, range;
   bool result;

   depth = (int) Subs[observer].Depth;
   range = (int) Subs[observer].DistanceToTarget( & (Subs[target]) );
   result = RadarStation.isTargetVisible(target, range, depth,
                         SHIP_HEIGHT, DEFAULT_SEA_STATE);
   if (result)
       return 1.0;
   else
      return 0.0;   
}


// Can I detect your radar?
float Esm_Detection(double Range, int observer, int target)
{
   int depth, range;
   bool result;

   depth = (int) Subs[observer].Depth;
   range = (int) Subs[observer].DistanceToTarget(& (Subs[target]) );
   result = EsmStation.isTargetVisible(target, range, depth,
                       SHIP_HEIGHT, TRUE, DEFAULT_SEA_STATE);
   if (result)
     return 1.0;
   else
     return 0.0;

}



/// Can the observer hear the target?
float Sonar_Detection_New(double Range, Submarine *observer, Submarine *target)
{

        float NauticalMiles = (float)Range / 2000.0;
        float HisPassiveSonarCrosssection = target->PSCS;
        float EffectiveTargetSpeed;
        float AmbientNoise;
        float OwnShipNoise;
        float TotalNoise;
        float TargetNoise;
        float Gb;
        float Lbp;
        float NoiseFromSpeed;
        float BasisNoiseLevel;
        float value;
        float SeaState = 3.0; // Anyone want to model the weather.

        if (target->Speed <= 5.0){
             EffectiveTargetSpeed = 0.0;
        }else{
             EffectiveTargetSpeed = target->Speed - 5.0;
        }

        if (target->Speed < 20.0){
             NoiseFromSpeed = 1.30;
             BasisNoiseLevel = 0.0;
        }else{
             NoiseFromSpeed = 0.65;
             BasisNoiseLevel = 9.75;
        }
        AmbientNoise = 89.0 + (5.0 * SeaState);
        OwnShipNoise = observer->RadiatedNoise();
        TotalNoise = 10.0 * log10(pow(10.0,OwnShipNoise/10.0) + pow(10.0,AmbientNoise/10.0));
        Gb = (TotalNoise - AmbientNoise) / 2.9;
        Lbp = AmbientNoise + Gb;
        TargetNoise = HisPassiveSonarCrosssection +
        ((NoiseFromSpeed * EffectiveTargetSpeed) + BasisNoiseLevel);
        value = TargetNoise - (20.0 * log10(NauticalMiles) + 1.1 * NauticalMiles) - Lbp;
        if (!observer) 
              SonarStation.flowandambientnoise = (Lbp - 34);
        if (value > -45.0){
                return (value - -45.0) + 1.0;
        }else{
                return 0.0;
        }
}



// Re-writing this Sonar function above to accept submarines
// passed in, rather than indexes. Should make things more
// flexible.

// Can I hear you?
// float Sonar_Detection(double Range, int observer, int target){

	/*********************************************
	This is the current incarnation of our detection
	routines. It seems to work well, and any further
	changes will be slight, unless I get a wild hair
	up my butt....

	Pass it the Range of the two objects and the
	ship ID's and it will spit back if the target
	ship should be detectable.
	*********************************************/
/*
	float NauticalMiles = (float)Range / 2000.0;
	float HisPassiveSonarCrosssection = Subs[target].PSCS;
	float EffectiveTargetSpeed;
	float AmbientNoise;
	float OwnShipNoise;
	float TotalNoise;
	float TargetNoise;
	float Gb;
	float Lbp;
	float NoiseFromSpeed;
	float BasisNoiseLevel;
	float value;
	float SeaState = 3.0; // Anyone want to model the weather.

	if (Subs[target].Speed <= 5.0){
	     EffectiveTargetSpeed = 0.0;
	}else{
	     EffectiveTargetSpeed = Subs[target].Speed - 5.0;
	}

	if (Subs[target].Speed < 20.0){
	     NoiseFromSpeed = 1.30;
	     BasisNoiseLevel = 0.0;
	}else{
	     NoiseFromSpeed = 0.65;
	     BasisNoiseLevel = 9.75;
	}

	AmbientNoise = 89.0 + (5.0 * SeaState);
	OwnShipNoise = Subs[observer].RadiatedNoise();
	TotalNoise = 10.0 * log10(pow(10.0,OwnShipNoise/10.0) + pow(10.0,AmbientNoise/10.0));
	Gb = (TotalNoise - AmbientNoise) / 2.9;
	Lbp = AmbientNoise + Gb;
	TargetNoise = HisPassiveSonarCrosssection +
	((NoiseFromSpeed * EffectiveTargetSpeed) + BasisNoiseLevel);
	value = TargetNoise - (20.0 * log10(NauticalMiles) + 1.1 * NauticalMiles) - Lbp;
	if (!observer) SonarStation.flowandambientnoise = (Lbp - 34);
	if (value > -45.0){
		return (value - -45.0) + 1.0;
	}else{
		return 0.0;
	}
}
*/


void SoundEnvironment(){
	///********************************************************************
	//This will gather sound information from all platforms and place
	//messages into each platforms sonar queue.
	//*********************************************************************

	int bearing;
	double Range;

	//loop through each ship and let them listen
	//for the other ships...
	Subs[0].AdvanceSonarHistory(); //advance sonar queue by 1/5th of a second
	TB16.AdvanceSonarHistory(); //advance sonar queue by 1/5th of a second
	for (int target=0; target<ships; target++){
		if (!InBaffles(0, target, 1)){ //I'm not deaf?
			Range = CalculateRange(target, 0);
			float signal;
			signal = Any_Detection(Range, 0, target);
			if (signal){ // Are we audible?
				bearing = (int)Subs[0].BearingToTarget(& (Subs[target]) );  //Change me to float for better tma
				Subs[0].RegisterEvent(bearing,signal,target);
                                // printf("Adding target %d to list.\n", target);
                                Subs[0].Add_Target(target, signal);
			}
                        else {
                          // printf("Lost target %d from list.\n", target);
                          Subs[0].Remove_Target(target);
                        }
		}
		if (!InBaffles(0, target, 2) && TB16.GetLength() >240){ // do the same for towed array
			Range = TB16.RangeToTarget(Subs[target].Lat_TotalYards, Subs[target].Lon_TotalYards);
			float signal;
			// signal = Sonar_Detection(Range, 0, target);
                        signal = Sonar_Detection_New(Range, &(Subs[0]), &(Subs[target]));
			if (signal){ // Are we audible?
				bearing = (int)TB16.BearingToTarget(Subs[target].Lat_TotalYards, Subs[target].Lon_TotalYards);  //Change me to float for better tma
				TB16.RegisterEvent(bearing,signal,target);
                                // printf("Adding target %d to list.\n", target);
                                Subs[0].Add_Target(target, signal);
			}
                        else {
                          // printf("Lost target %d from list.\n", target);
                          Subs[0].Remove_Target(target);
                        }
		}
	}
}

int InBaffles(int observer, int target, int sensor){
/*********************************************
	This function will return if a target is in
	the observers Baffles and therefore not
	detectable. Values for 'int sensor' are 1 for
	spherical array, 2 for towed array, 3 for port
	hull array and 4 for starboard hull array.

	Might want to move the calculations of the
	baffle angles to the Coord class so they don't
	have to be calculated all the time.
*********************************************/
	int array_heading;
	int relative_bearing;
	int sensordeaf=1;
	int bearing_to_target;

	switch(sensor){
		case 1:	//Spherical
			sensordeaf = 0;
			array_heading = (int)Subs[observer].Heading;
			bearing_to_target = (int)Subs[observer].BearingToTarget( &( Subs[target]) );
			if(array_heading > bearing_to_target) bearing_to_target += 360;
			relative_bearing = bearing_to_target - array_heading;
			if(relative_bearing > 150 && relative_bearing < 210) sensordeaf = 1;
			if(target == observer) sensordeaf = 1;
			break;
		case 2:	//Towed
			sensordeaf = 0;
			array_heading = (int)TB16.ReturnHeading();
			bearing_to_target = (int)TB16.BearingToTarget(Subs[target].Lat_TotalYards, Subs[target].Lon_TotalYards);
			if(array_heading > bearing_to_target) bearing_to_target += 360;
			relative_bearing = bearing_to_target - array_heading;
			if(relative_bearing < 30 || relative_bearing > 330) sensordeaf = 1;
			break;
		case 3: //port hull
		case 4: //sb hull
		default:
			break;
	}
	return sensordeaf;
}


void DisplayTMA(int xoffset,int yoffset){
 	SDL_Rect source, destination;
	source.x = 0;
	source.y = 0;
	source.w = 500;
	source.h = 500;
	destination.x = 374;
	destination.y = 145;
	destination.w = 500;
	destination.h = 500;
	Tma.Lock();
	Tma.our_heading = (double)Subs[0].Heading;
	Tma.our_speed = (float)Subs[0].Speed;
	Tma.target_heading = (double)Subs[1].Heading;
	Tma.target_speed = (float)Subs[1].Speed;
	Tma.DisplayGeoPlot(xoffset,yoffset);
	//Tma.DisplayLOS();
	SDL_BlitSurface(Tma.GeoPlotScreen, &source, screen, &destination);
	SDL_UpdateRects(screen, 1, &destination);
	Tma.UnLock();
}

void DisplayWeapons()
{
    SDL_Rect weapons, tubes;
    DFont fnt("images/font.png", "data/font.dat");
    char text[256];
    int index, y1, y2;

    if (! update_weapons_screen)
        return;

    weapons.x = 150;
    weapons.y = 145;
    weapons.w = 200;
    weapons.h = 300;
    tubes.x = 374;
    tubes.y = 145;
    tubes.h = 500;
    tubes.w = 501;

    SDL_FillRect(screen, &weapons, black);
    sprintf(text, "WEAPONS");
    fnt.PutString(screen, 190, 150, text);
    sprintf(text, "     Torpedos: %d", Subs[0].TorpedosOnBoard);
    fnt.PutString(screen, 160, 174, text);
    sprintf(text, "Noise Makers: %d", Subs[0].NoiseMakers);
    fnt.PutString(screen, 160, 196, text);
    sprintf(text, "'T' to load torpedo");
    fnt.PutString(screen, 160, 300, text);
    sprintf(text, "'N' to load noise maker");
    fnt.PutString(screen, 160, 312, text);
    sprintf(text, "'U' to unload device");
    fnt.PutString(screen, 160, 324, text);
    sprintf(text, "'F' to fire");
    fnt.PutString(screen, 160, 336, text);
    SDL_UpdateRects(screen, 1, &weapons);

    SDL_FillRect(screen, &tubes, black);
    y1 = 150; y2 = 190;
    for (index = 0; index < MAX_TUBES; index++)
    {
        // draw tube
        DrawRectangle(screen, 390, y1, 500, y2, green);
        // draw buttons
        fnt.PutString(screen, 520, y1 + 5, "  Load");
        fnt.PutString(screen, 520, y1 + 18, "Torpedo");
        DrawRectangle(screen, 520, y1, 590, y2, green);
        fnt.PutString(screen, 600, y1 + 5, "   Load");
        fnt.PutString(screen, 600, y1 + 18, "Noise Maker");
        DrawRectangle(screen, 600, y1, 690, y2, green);
        fnt.PutString(screen, 700, y1 + 10, "Unload");
        DrawRectangle(screen, 700, y1, 760, y2, green);
        fnt.PutString(screen, 770, y1 + 10, "Fire!");
        DrawRectangle(screen, 770, y1, 815, y2, green);
        y1 += 50; y2 += 50;
    }
    SDL_UpdateRects(screen, 1, &tubes);

    // put stuff in the tubes
    y1 = 151;
    for (index = 0; index < MAX_TUBES; index++)
    {
       tubes.x = 391;
       tubes.y = y1;
       tubes.w = 105;
       tubes.h = 35;
       if (Subs[0].torpedo_tube[index] == TUBE_TORPEDO)
          SDL_BlitSurface(torpedo_image, NULL, screen, &tubes);
       else if (Subs[0].torpedo_tube[index] == TUBE_NOISEMAKER)
          SDL_BlitSurface(noisemaker_image, NULL, screen, &tubes);
       else
       {
         SDL_FillRect(screen, &tubes, black); 
         sprintf(text, "Tube %d", index + 1);
         fnt.PutString(screen, 420, y1 + 10, text);
       }
       SDL_UpdateRects(screen, 1, &tubes);
       y1 += 50;
    }
    update_weapons_screen = FALSE;
}



inline int RandInt(int TO){ //Returns a random interger...TO is upper limit
	return (rand()%TO);
}

inline int Clamp(int sample){ //holds a value to be between 0 and 255
	if (sample < 0){
		sample = 0;
	}
	if (sample > 255){
		sample = 255;
	}
	return sample;
}

inline double Clamp(double sample){ //Overloaded for floats
	if (sample < 0){
		sample = 0.0;
	}
	if (sample > 255){
		sample = 255.0;
	}
	return sample;
}

void ResetWidgetFlags(){
	//turns off all the widget redraw flags
	SonarStation.sonarwidget=false;
	navwidget=0;
	quitwidget=0;
	tmawidget=0;
	esmwidget=0;
	radarwidget=0;
	shipcontrolwidget=0;
}

void ShowStation(int station){

 // init everything to avoid overlap
 drawmap = drawweapons = drawsonar = drawradar = drawesm = drawcontrol = 0;
 navwidget = shipcontrolwidget = radarwidget = esmwidget = 0;
	switch (station){// which station are we at?
		case 1: //sonar screen
			LoadScreen(1); //load in the screen for the SONAR station
			ResetWidgetFlags(); //self explanitory
			SonarStation.sonarwidget = true; //depress the widget
			DisplayWidgets(); // display the widgets
			drawsonar = 1; //tell the sonar it's ok to draw itself
			break;
		case 2: //nav screen
			LoadScreen(2); //load in the screen for the NAV station
			ResetWidgetFlags(); //self explanitory
			navwidget = 1; //depress the widget
			DisplayWidgets(); // display the widgets
			drawmap = 1; //tell the Map it's ok to draw itself
			break;
		case 3: //tma screen
			LoadScreen(3); //load in the screen for the TMA station
			ResetWidgetFlags(); //self explanitory
			// tmawidget = 1; //depress the widget
			DisplayWidgets(); // display the widgets
			drawweapons = 1; //Turn on TMA screen updates
			break;
		case 4: //shipcontrol screen
			LoadScreen(4); //load in the screen for the shipcontrol station
			ResetWidgetFlags(); //self explanitory
			shipcontrolwidget = 1; //depress the widget
			DisplayWidgets(); // display the widgets
			drawcontrol = 1;
			ControlStation.InitGraphics(screen, controlscreen);
			break;
		case 5: //RADAR screen
			LoadScreen(5); //load in the screen for the RADAR station
			ResetWidgetFlags(); //self explanitory
			radarwidget = 1; //depress the widget
			DisplayWidgets(); // display the widgets
			drawradar = 1; //Turn on Radar screen updates
			RadarStation.InitGraphics(screen, radarscreen);
			break;
		case 6: //ESM screen
			LoadScreen(6); //load in the screen for the ESM station
			ResetWidgetFlags(); //self explanitory
			esmwidget = 1; //depress the widget
			DisplayWidgets(); // display the widgets
			drawesm = 1; //Turn on ESM screen updates
			EsmStation.InitGraphics(screen, esmscreen);
			break;
	}
}

/**************************************************
This is our callback function to handle
time critical Functions
**************************************************/
Uint32 timerfunc(Uint32 interval, void *param){
	param = NULL;
	Clock.UpdateTime();
	ShipHandeling();
	UpdateSensors();
	return interval;
}

/***************************************************
Another callback to handle the 60 second
resolution TMA stuff.
****************************************************/
Uint32 TmaTimer(Uint32 interval, void *param){
	static Uint32 tick = 0;
	Tma.Lock(); //Lock Tma access mutex
	param = NULL; //Quites error messages.
	tick ++; //record the time of the tma record.
	Tma.RecordBoatPosition(Subs[0].Lat_TotalYards, Subs[0].Lon_TotalYards, Subs[0].BearingToTarget(& (Subs[1])) , tick);
	Tma.UnLock(); //Unlock mutex
	return interval;
}

int HandleInput(SDL_Event &event, int &mousex, int &mousey){
	static string textline;
        int y1, y2, x_checks_out;
        int index;

	switch (event.type){
		case SDL_MOUSEBUTTONDOWN:
			mousex = event.button.x;
			mousey = event.button.y;
                        // This is debug stuff we can get rid of later
                        /*
			char cmousex[10];
			char cmousey[10];
			sprintf(cmousex, "%i", mousex);
			sprintf(cmousey, "%i", mousey);
			textline="Mouse Event: X:";
			textline.append(cmousex);
			textline.append(" Y:");
			textline.append(cmousey);
			Message.post_message(textline);
			Message.display_message();
                        */
			if (mousey<740 && mousey>711){
				if (mousex <212 && mousex>180){   //SONAR widget
					return SONAR;
					break;
				}
				if (mousex <312 && mousex>280){  //Map Widget
					return NAVMAP;
					break;
				}
				if (mousex <412 && mousex>380){  //TMA Widget
				 	return WEAPONS;
					break;
				}
				if (mousex <512 && mousex>480){  //Ship Control Widget
					return SHIPCONTROL;
					break;
				}
				if (mousex <612 && mousex>580){  //ESM Widget
					return ESM;
					break;
				}
				if (mousex <712 && mousex>680){ //RADAR Widget
					return RADAR;
					break;
				}
				if (mousex <812 && mousex>780){ //Quit Widget
					return QUIT;
					break;
				}
				if (mousex < 908 && mousex > 884){ //Time Compression +
					return COMPRESSTIME;
					break;
				}
			
				if (mousex < 1000 && mousex > 972){  //Time Compression -
					return UNCOMPRESSTIME;
					break;
				}
			}
			if(drawmap || drawtma){
				if(mousex > 224 && mousex < 257){
					if(mousey >270 && mousey < 300){
						return CENTERDISPLAY;
						break;
					}
				}
				if(mousex > 224 && mousex < 257){
					if(mousey >220 && mousey < 245){
						return SCROLLMAPUP;
						break;
					}
				}
				if(mousex > 224 && mousex < 257){
					if(mousey >320 && mousey < 345){
						return SCROLLMAPDOWN;
						break;
					}
				}
				if(mousex > 175 && mousex < 210){
					if(mousey >270 && mousey < 300){
						return SCROLLMAPLEFT;
						break;
					}
				}
				if(mousex > 275 && mousex < 310){
					if(mousey >270 && mousey < 300){
						return SCROLLMAPRIGHT;
						break;
					}
				}
				if(mousex >175 && mousex < 210){
					if(mousey > 360 && mousey < 390){
						return INCREASEMAPSCALE;
						break;
					}
				}
				if(mousex >275 && mousex < 310){
					if(mousey > 360 && mousey < 390){
						return DECREASEMAPSCALE;
						break;
					}
				}
			}
			// Radar events
			if(drawradar){
				if(mousex > 105 && mousex < 512){
					if(mousey >156 && mousey < 566){
					RadarStation.ShowData(screen, mousex, mousey);
						break;
					}
				}

				if(mousex > 748 && mousex < 795){
					if(mousey >320 && mousey < 367){
						return TOGGLER10;
						break;
					}
				}
				
				if(mousex > 795 && mousex < 842){
					if(mousey >320 && mousey < 367){
						return TOGGLER20;
						break;
					}
				}
				
				if(mousex > 842 && mousex < 889){
					if(mousey >320 && mousey < 367){
						return TOGGLER30;
						break;
					}
				}

				if(mousex > 748 && mousex < 795){
					if(mousey >367 && mousey < 414){
						return TOGGLER40;
						break;
					}
				}
				
				if(mousex > 795 && mousex < 842){
					if(mousey >367 && mousey < 414){
						return TOGGLER50;
						break;
					}
				}

				if(mousex > 842 && mousex < 889){
					if(mousey >367 && mousey < 414){
						return TOGGLER60;
						break;
					}
				}

				if(mousex > 744 && mousex < 791){
					if(mousey >185 && mousey < 232){
						return RADARDOWN;
						break;
					}
				}

				if(mousex > 850 && mousex < 897){
					if(mousey >185 && mousey < 232){
						return RADARUP;
						break;
					}
				}

				if(mousex > 749 && mousex < 796){
					if(mousey >501 && mousey < 548){
						return RING0;
						break;
					}
				}
				
				if(mousex > 796 && mousex < 843){
					if(mousey >501 && mousey < 548){
					  return RING5;
					  break;
					}
				}

				if(mousex > 843 && mousex < 889){
				        if(mousey >501 && mousey < 548){
					  return RING10;
					  break;
					}
				}
				
				if(mousex > 548 && mousex < 796){
				        if(mousey >548 && mousey < 594){
					  return RING15;
					  break;
					}
				}

				if(mousex > 796 && mousex < 843){
				        if(mousey >548 && mousey < 594){
					  return RING20;
					  break;
					}
				}
				
				if(mousex > 843 && mousex < 889){
				        if(mousey >548 && mousey < 594){
					  return RING25;
					  break;
					}
				}
			}

                        // Weapon console events
                        if (drawweapons)
                        {
                            x_checks_out = TRUE;
                            y1 = 150; y2 = 190;
                            // check all x1-x2 possiblities
                            if ( (mousex > 520) && (mousex < 590) )
                               tube_action = LOAD_TORPEDO;
                            else if ( (mousex > 600) && (mousex < 680) )
                               tube_action = LOAD_NOISEMAKER;
                            else if ( (mousex > 700) && (mousex < 760) )
                               tube_action = UNLOAD_TUBE;
                            else if ( (mousex > 770) && (mousex < 815) )
                               tube_action = FIRE_TUBE;
                            else
                            {
                               tube_action = 0;
                               x_checks_out = FALSE;
                            }
                            // look at all y1-y2 button possibilities
                            if (x_checks_out)
                            {
                                index = 0;
                                tube_to_use = -1;
                                while ( (index < MAX_TUBES) && 
                                        (tube_to_use == -1) )
                                {
                                    if ((mousey > y1) && (mousey < y2) )
                                      tube_to_use = index;
                                    else
                                    {
                                        index++;
                                        y1 += 50; y2 += 50;
                                    }
                                }    // end of checking buttons vertically
                                if (tube_to_use == -1)
                                   tube_action = 0;
                            }
                            else
                              tube_action = 0;
                            if ( (tube_action) && (tube_to_use > -1) )
                              return USE_TUBE;
                                 

                        }   // end of weapons screen
			// ESM events
			if(drawesm){
				if(mousex > 744 && mousex < 791){
					if(mousey >185 && mousey < 232){
						return ESMDOWN;
						break;
					}
				}

				if(mousex > 850 && mousex < 897){
					if(mousey >185 && mousey < 232){
						return ESMUP;
						break;
					}
				}
			}

			// Control events
			if(drawcontrol){
			       if(mousex > 158 && mousex < 347){
					if(mousey >250 && mousey < 430){
					ControlStation.AdjustHeading(mousex, mousey);
						break;
					}
				}
				if(mousex > 112 && mousex < 126){
					if(mousey >254 && mousey < 293){
					  ControlStation.AdjustDepth(mousex);
						break;
					}
				}

				if(mousex > 385 && mousex < 401){
					if(mousey >257 && mousey < 296){
					  ControlStation.AdjustDepth(mousex);
						break;
					}
				}
				if(mousex > 364 && mousex < 404){
					if(mousey >403 && mousey < 418){
						return TURNSTARBOARD;
						break;
					}
				}

				if(mousex > 108 && mousex < 147){
					if(mousey >405 && mousey < 421){
						return TURNPORT;
						break;
					}
				}
				if(mousex > 232 && mousex < 252){
					if(mousey >581 && mousey < 607){
						return ASTOP;
						break;
					}
				}
				if(mousex > 263 && mousex < 287){
					if(mousey >581 && mousey < 607){
						return A13;
						break;
					}
				}
				if(mousex > 296 && mousex < 320){
				        if(mousey >581 && mousey < 607){
				                return A23;
				                break;
					}
				}
				if(mousex > 329 && mousex < 352){
					if(mousey >581 && mousey < 607){
						return ASTD;
						break;
					}
				}
				if(mousex > 364 && mousex < 384){
				        if(mousey >581 && mousey < 607){
				                return AFULL;
				                break;
					}
				}
				if(mousex > 397 && mousex < 416){
					if(mousey >581 && mousey < 607){
						return AFLK;
						break;
					}
				}
				if(mousex > 200 && mousex < 218){
					if(mousey >581 && mousey < 607){
						return B13;
						break;
					}
				}
				if(mousex > 166 && mousex < 185){
				        if(mousey >581 && mousey < 607){
				                return B23;
				                break;
					}
				}
				if(mousex > 134 && mousex < 152){
					if(mousey >581 && mousey < 607){
						return BSTD;
						break;
					}
				}
				if(mousex > 100 && mousex < 118){
					if(mousey >581 && mousey < 607){
						return BEMER;
						break;
					}
				}
				if(mousex > 717 && mousex < 757){
					if(mousey >585 && mousey < 601){
					  return EXTENDARRAY;
						break;
					}
				}
				if(mousex > 717 && mousex < 757){
					if(mousey >612 && mousey < 628){
					  return RETRACTARRAY;
						break;
					}
				}
			}

			if(drawsonar){
				if(mousex > 615 && mousex < 657){
					if(mousey > 162 && mousey < 210){
						return TOGGLETRUERELATIVE;
						break;
					}
				}
				if(mousex > 474 && mousex < 519){
					if(mousey > 358 && mousey < 401){
						return DESIGNATECONTACT;
						break;
					}
				}
				if(mousex > 615 && mousex < 657){
					if(mousey > 215 && mousey < 261){
						return TOGGLESPHERICALTOWED;
						break;
					}
				}
				if(mousex > 663 && mousex < 706){
					if(mousey > 162 && mousey < 210){
						return UPPERCRTBUTTON;
						break;
					}
				}
				if(mousex > 663 && mousex < 706){
					if(mousey > 215 && mousey < 261){
						return LOWERCRTBUTTON;
						break;
					}
				}
				if(mousex >569  && mousex < 614){
					if(mousey > 593 && mousey < 638){
						return STOPWINCH;
						break;
					}
				}
				if(mousex > 617 && mousex < 660){
					if(mousey > 593 && mousey < 638){
						return EXTENDARRAY;
						break;
					}
				}
				if(mousex > 663 && mousex < 706){
					if(mousey > 593 && mousey < 638){
						return RETRACTARRAY;
						break;
					}
				}
				if(mousex > 476 && mousex < 517){
					if(mousey > 406 && mousey < 447){
						return ASSIGNTRACKER;
						break;
					}
				}
				if(mousex > 523 && mousex < 564){
					if(mousey > 406 && mousey < 447){
						return TRACKER1;
						break;
					}
				}
				if(mousex > 570 && mousex < 611){
					if(mousey > 406 && mousey < 447){
						return TRACKER2;
						break;
					}
				}
				if(mousex > 617 && mousex < 658){
					if(mousey > 406 && mousey < 447){
						return TRACKER3;
						break;
					}
				}
				if(mousex > 663 && mousex < 706){
					if(mousey > 406 && mousey < 447){
						return TRACKER4;
						break;
					}
				}
			}
			break;
		case SDL_QUIT:
			return QUIT;
			break;
		case SDL_KEYDOWN:  //Process key presses
			switch(event.key.keysym.sym){
				case SDLK_PAGEUP:  // PgUp   Make Einstein proud.
					return COMPRESSTIME;
					break;
				case SDLK_PAGEDOWN: // PgDown
					return UNCOMPRESSTIME;
					break;
				case SDLK_KP_PLUS:
				case SDLK_PLUS:  // Speed Up
					return INCREASESPEED;
					break;
				case SDLK_KP_MINUS:
				case SDLK_MINUS:  // Slow Down
					return DECREASESPEED;
					break;
				// case SDLK_n: //  Navigation
                                case SDLK_F2:
					return NAVMAP;
					break;
				// case SDLK_s: //  Sonar
                                case SDLK_F1:
					return SONAR;
					break;
				// case SDLK_t: //  TMA
                                case SDLK_F3:
					return WEAPONS;
				 	break;
				case SDLK_ESCAPE: // Quit...
					return QUIT;
					break;
                                case SDLK_TAB:  // switch target
                                        return SWITCHTARGET;
                                        break;
				case SDLK_F10: //  Dump the screen
					return DUMPSCREEN;
					break;
                                case SDLK_F4:
                                        return SHIPCONTROL;
                                        break;
                                case SDLK_F5:
                                        return ESM;
                                        break;
                                case SDLK_F6:
                                        return RADAR;
                                        break;
				case SDLK_KP4: // Turn Left
					return TURNPORT;
					break;
				case SDLK_KP6: // Turn Right
					return TURNSTARBOARD;
					break;
				case SDLK_KP2: // Dive
					return INCREASEDEPTH;
					break;
				case SDLK_KP8: //Surface
					return DECREASEDEPTH;
					break;
				case SDLK_INSERT:
					return INCREASEMAPSCALE;
					break;
				case SDLK_DELETE: 
					return DECREASEMAPSCALE;
					break;
				case SDLK_UP :
					return SCROLLMAPUP;
					break;
 				case SDLK_DOWN:
					return SCROLLMAPDOWN;
					break;
				case SDLK_LEFT:
					return SCROLLMAPLEFT;
					break;
				case SDLK_RIGHT:
					return SCROLLMAPRIGHT;
					break;
				case SDLK_c:
					return CENTERDISPLAY;
					break;
				case SDLK_e:
					return EXTENDARRAY;
					break;
				case SDLK_r:
					return RETRACTARRAY;
					break;
				case SDLK_w:
					return STOPWINCH;
					break;
				// case 19: // Pause The Game
				case SDLK_p: //  Pause The Game
					return PAUSEGAME;
					break;
                                case SDLK_t:
                                         tube_action = LOAD_TORPEDO;
                                         tube_to_use = -1;
                                        return WHICHTUBE;
                                case SDLK_n:
                                         tube_action = LOAD_NOISEMAKER;
                                         tube_to_use = -1;
                                         return WHICHTUBE;
                                case SDLK_u:
                                         tube_action = UNLOAD_TUBE;
                                         tube_to_use = -1;
                                         return WHICHTUBE;
                                case SDLK_f:
                                         tube_action = FIRE_TUBE;
                                         tube_to_use = -1;
                                         return WHICHTUBE;
                                case SDLK_1:
                                         tube_to_use = 0;
                                         return USE_TUBE;
                                case SDLK_2:
                                         tube_to_use = 1;
                                         return USE_TUBE;
                                case SDLK_3:
                                         tube_to_use = 2;
                                         return USE_TUBE;
                                case SDLK_4:
                                         tube_to_use = 3;
                                         return USE_TUBE;
                                case SDLK_5:
                                         tube_to_use = 4;
                                         return USE_TUBE;
                                case SDLK_6:
                                         tube_to_use = 5;
                                         return USE_TUBE;

				default:	
					return 0;
					break;
				}
		default:
			return 0;
			break;
	}
	return 0;
}

int main(int argc, char **argv){
	static char text[120];
        int status;
	int hours=0;
	int minutes=0;
	int seconds=0;
	int option_choice;//a place to put command line switches
	int screendumpcount = 0;
	int mousex = 0, mousey = 0; //where is the mouse?
	bool quit = false;  //Quit flag Duh!
	int station; //flag to decide which work station to display
	bool full_screen = false;
	char *textline = "";
	char file1[] = "images/font.png";
	char file2[] = "data/font.dat";
	char file3[] = "images/largefont.png";
	char file4[] = "data/largefont.dat";
	SDL_Event event; //a typedef to hold events
	drawsonar = 0; // draw the sonar flag
	drawmap = 1; // draw the map flag
	drawradar = 0;
	drawesm = 0;
	drawcontrol = 0;
	northcenter = true; //make the sonar display N. centered
	Uint32 timer1; // our event timers....
	SDL_TimerID timer_id, timer_id2;
        torpedoes = NULL;
	tmamutex = SDL_CreateMutex();
	srand(time(NULL)); //Seed the random generator

	//Process commandline options.
	sprintf(text,"vwfkh");
	while ((option_choice = getopt(argc, argv, text)) != -1){
		switch (option_choice){
			case 'w': //they passed the '-w' flag.
				full_screen = false;
				break;
                        case 'f': // full screen
                                full_screen = true;
                                break;
			case 'k': //This option is just for me cause when using kdevelop
							//I don't start in the right directory.
				chdir("./subsim");
				break;
                        case 'v':
                                cout << "OpenSSN version " << VERSION << endl;
                                return 0;
			case 'h':
				cout << "Usage:" << endl
                                << "-f For full screen mode." << endl
				<< "-w For Windowed Mode." << endl
                                << "-v For version." << endl
				<< "-h For this message." << endl;
				return 0;
				break;
			default:
				cout << "Unknown command-line argument" << endl
				<< "Please use -h for a list of commands." << endl;
				return 1;
		}
	}
	SetupScreen(full_screen);
	CreateShips(0);
	Tma.InitGraphics();
	SonarStation.InitGraphics();
//	msg Message;
	Message.InitGraphics();
	Clock.InitTime(12,15,0);
	SonarStation.LoadWidgets();
	SDL_EnableKeyRepeat(150,100);
	LoadWidgets();
	LoadScreen(0); //Display intro screen
	DFont fnt(file1, file2);
	static DFont fnt2(file3,file4);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	timer1 = SDL_GetTicks();// initialize the timer
	while (!quit){
		if (SDL_GetTicks() - timer1 > 5000) quit = true; // if 5 seconds has passed, exit intro
		while (SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_MOUSEBUTTONDOWN: //is the mouse button pressed?
				case SDL_KEYDOWN: //or has a key been pressed?
					quit = true; // exit the intro screen
					break;
				default:
					break;
			}
		}
	}

	//LoadWidgets(); //load up the buttons
	SDL_Rect rectangle;
	rectangle.x = 0;
	rectangle.y = 0;
	rectangle.w = 1024; // This block of code clears the screen
	rectangle.h = 768;
	SDL_FillRect(screen, &rectangle, black);
	timecompression = 1;
	station = 2; //default station
	ShowStation(station);
	textline="OpenSSN VERSION 0.2";
	Message.post_message(textline);
	textline="http://openssn.sourceforge.net";
	Message.post_message(textline);
	Message.display_message();
	sprintf(text, "[%i] ", timecompression);
	fnt.PutString(screen, 933, 718, text);
	quit = false; //reset loop exit flag
	timer_id = SDL_AddTimer(1000, timerfunc, NULL);
	timer_id2 = SDL_AddTimer(60000, TmaTimer, NULL);
	timer1 = SDL_GetTicks();
	const int frameperiod = 100;	//how fast we want the displays to update (in milliseconds)... this allows for a fixed frame rate
	while (!quit)
        { 	//This is the main loop.....
		if (timer1 + (frameperiod) < SDL_GetTicks()){
			timer1 = SDL_GetTicks();
			RadarStation.Sweep(frameperiod*timecompression);
			UpdateDisplay();
			
			SDL_Rect rectangle;
			rectangle.x=16;
			rectangle.y=14;   //define a rectangle on the screen and make it black
			rectangle.h=72;
			rectangle.w=126;
			SDL_FillRect(screen, &rectangle, black);
                        sprintf(text, "Now - Wanted");
                        fnt.PutString(screen, 30, 20, text);
			sprintf(text, "S: [%3i]  -   [%3i]", (int)Subs[0].Speed, Subs[0].DesiredSpeed);
			fnt.PutString(screen, 30, 31, text);
			sprintf(text, "H: [%3i]  -   [%3i]", (int)Subs[0].Heading, Subs[0].DesiredHeading);
			fnt.PutString(screen, 30, 42, text);
			sprintf(text, "D: [%4i]  -  [%4i]", (int)Subs[0].Depth, Subs[0].DesiredDepth);
			fnt.PutString(screen, 30, 53, text);
			// sprintf(text, "ARRAY [%4i]", TB16.GetLength());
			// fnt.PutString(screen, 40, 53, text);
			Clock.GetTime(hours,minutes,seconds);
			sprintf(text, "%.2i:%.2i:%.2i", hours, minutes, seconds);
			fnt.PutString(screen, 40, 64, text);
			SDL_UpdateRects(screen, 1, &rectangle);			
			}

		while (SDL_PollEvent(&event)){
		//If there are events waiting take care of them
			switch (HandleInput(event, mousex, mousey)){
				case SONAR:
						ShowStation(1);
						UpdateDisplay();
                                                Message.post_message("Sonar station");
						Message.display_message();
						sprintf(text, "[%i] ", timecompression);
						fnt.PutString(screen, 933, 718, text);
					break;
				case NAVMAP:
					ShowStation(2);
					UpdateDisplay();
                                        Message.post_message("Naviation display");
					Message.display_message();
					sprintf(text, "[%i] ", timecompression);
					fnt.PutString(screen, 933, 718, text);
					break;
				case WEAPONS:
                                        update_weapons_screen = TRUE;
					ShowStation(3);
					UpdateDisplay();
                                        Message.post_message("Weapons console");
					Message.display_message();
					sprintf(text, "[%i] ", timecompression);
					fnt.PutString(screen, 933, 718, text);
					break;
                                case WHICHTUBE:
                                        update_weapons_screen = TRUE;
                                        UpdateDisplay();
                                        Message.post_message("Which tube (1-6)?");
                                        Message.display_message();
                                        break;
				case SHIPCONTROL:
					ShowStation(4);
					UpdateDisplay();
                                        Message.post_message("Helm control");
					Message.display_message();
					sprintf(text, "[%i] ", timecompression);
					fnt.PutString(screen, 933, 718, text);
					break;
				case ESM:
					ShowStation(6);
					UpdateDisplay();
                                        Message.post_message("ESM station");
					Message.display_message();
					sprintf(text, "[%i] ", timecompression);
					fnt.PutString(screen, 933, 718, text);
					break;
				case RADAR:
					ShowStation(5);
					UpdateDisplay();
                                        Message.post_message("Radar console");
					Message.display_message();
					sprintf(text, "[%i] ", timecompression);
					fnt.PutString(screen, 933, 718, text);
					break;
                                case SWITCHTARGET:
	                                current_target = Subs[0].Next_Target();
                                        if (drawmap)
                                        {
                                          DrawMap();
                                          // PlaceShips(mapscale, 0, -10, current_target);
                                        }
                                        SDL_Delay(100);
                                        break;
				case QUIT:
					ResetWidgetFlags();
					quitwidget=1;
					DisplayWidgets();
					SDL_Delay(200);
					quitwidget=0;
					DisplayWidgets();
					SDL_Delay(500);
					quit = true;
					break;
				case COMPRESSTIME:  //  Make Einstein proud.
					destination_rectangle.x = 880; //upper left corner to
					destination_rectangle.y = 710; //place the button.
					destination_rectangle.h = plusbuttondown->h; //height &
					destination_rectangle.w = plusbuttondown->w;//width of button.
					SDL_BlitSurface(plusbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
					SDL_UpdateRects(screen, 1, &destination_rectangle); //Show button on the screen...
					timecompression++;
					if(timecompression > 8){
						timecompression = 8;
					}else{
						if (!pause_game){
							SDL_RemoveTimer(timer_id);
							SDL_RemoveTimer(timer_id2);
							timer_id = SDL_AddTimer(1000/timecompression, timerfunc, NULL);
							timer_id2 = SDL_AddTimer(60000/timecompression, TmaTimer, NULL);
						}
						sprintf(text, "[%i] ", timecompression);
						fnt.PutString(screen, 933, 718, text);
					}
					SDL_Delay(150);
					DisplayWidgets();
					break;
				case UNCOMPRESSTIME: // Take A Downer
					destination_rectangle.x = 970; //upper left corner to
					destination_rectangle.y = 710; //place the button.
					destination_rectangle.h = minusbuttondown->h; //height &
					destination_rectangle.w = minusbuttondown->w;//width of button.
					SDL_BlitSurface(minusbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
					SDL_UpdateRects(screen, 1, &destination_rectangle); //Show button on the screen...
					timecompression--;
					if(timecompression < 1){
						timecompression = 1;
					}else{
						if (!pause_game){
							SDL_RemoveTimer(timer_id);
							SDL_RemoveTimer(timer_id2);
							timer_id = SDL_AddTimer(1000/timecompression, timerfunc, NULL);
							timer_id2 = SDL_AddTimer(60000/timecompression, TmaTimer, NULL);
						}
						sprintf(text, "[%i] ", timecompression);
						fnt.PutString(screen, 933, 718, text);
						}
						SDL_Delay(150);
						DisplayWidgets();
						break;
				case INCREASESPEED:
					Subs[0].DesiredSpeed++;
					break;
				case DECREASESPEED:
					Subs[0].DesiredSpeed--;
					break;
				case DUMPSCREEN:
					screendumpcount++;
					sprintf(text, "screendump%i.bmp", screendumpcount );
					SDL_SaveBMP(screen,text); //screen dumps
					cerr << "Screen Dump" << endl;
					break;
				case TURNPORT:
					Subs[0].DesiredHeading--;
					if(Subs[0].DesiredHeading < 0){
						Subs[0].DesiredHeading += 360;
					}
					break;
				case TURNSTARBOARD:
					Subs[0].DesiredHeading++;
					if(Subs[0].DesiredHeading > 359){
						Subs[0].DesiredHeading -= 360;
					}
					break;
				case INCREASEDEPTH:
					Subs[0].DesiredDepth++;
                                        if (Subs[0].DesiredDepth > Subs[0].MaxDepth)
                                           Subs[0].DesiredDepth = Subs[0].MaxDepth;
					break;
				case DECREASEDEPTH:
					Subs[0].DesiredDepth--;
					if(Subs[0].DesiredDepth < 0){
						Subs[0].DesiredDepth = 0;
					}
					break;
				case INCREASEMAPSCALE:
					if (drawmap){
						mapscale++;
						if(mapscale>MAX_MAP_SCALE) mapscale=MAX_MAP_SCALE;
					 	destination_rectangle.x = 175; //upper left corner to
						destination_rectangle.y = 359; //place the button.
						destination_rectangle.h = plusbuttondown->h; //height &
						destination_rectangle.w = plusbuttondown->w;//width of button.
						SDL_BlitSurface(plusbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
						DrawMap();
						//UpdateDisplay();
						SDL_Delay(100);
						DisplayNavigationWidgets();
					}
					if(drawtma){
					 	destination_rectangle.x = 175; //upper left corner to
						destination_rectangle.y = 359; //place the button.
						destination_rectangle.h = plusbuttondown->h; //height &
						destination_rectangle.w = plusbuttondown->w;//width of button.
						SDL_BlitSurface(plusbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
						Tma.IncreasePlotScale();
						DisplayTMA();
						SDL_Delay(100);
						DisplayTMAWidgets();
					}
					break;
				case DECREASEMAPSCALE:
					if(drawmap){
						mapscale--;
						if(mapscale<1) mapscale = 1;
					 	destination_rectangle.x = 275; //upper left corner to
						destination_rectangle.y = 359; //place the button.
						destination_rectangle.h = minusbuttondown->h; //height &
						destination_rectangle.w = minusbuttondown->w;//width of button.
						SDL_BlitSurface(minusbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
						DrawMap();
						//UpdateDisplay();
						SDL_Delay(100);
						DisplayNavigationWidgets();
					}
					if(drawtma){
						Tma.DecreasePlotScale();
					 	destination_rectangle.x = 275; //upper left corner to
						destination_rectangle.y = 359; //place the button.
						destination_rectangle.h = minusbuttondown->h; //height &
						destination_rectangle.w = minusbuttondown->w;//width of button.
						SDL_BlitSurface(minusbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
						DisplayTMA();
						SDL_Delay(100);
						DisplayTMAWidgets();
					}
					break;
				case SCROLLMAPUP:
					if(drawmap){
					 	destination_rectangle.x = 225; //upper left corner to
						destination_rectangle.y = 219; //place the button.
						destination_rectangle.h = upbuttondown->h; //height &
						destination_rectangle.w = upbuttondown->w;//width of button.
						SDL_BlitSurface(upbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
						PlaceShips(mapscale, 0, -10, current_target);
						DrawMap();
						SDL_Delay(100);
						DisplayNavigationWidgets();
					}
					if(drawtma){
					 	destination_rectangle.x = 225; //upper left corner to
						destination_rectangle.y = 219; //place the button.
						destination_rectangle.h = upbuttondown->h; //height &
						destination_rectangle.w = upbuttondown->w;//width of button.
						SDL_BlitSurface(upbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
						PlaceShips(mapscale, 0, -10, current_target);
						DisplayTMA(0,-10);
						SDL_Delay(100);
						DisplayTMAWidgets();
					}
					break;
				case SCROLLMAPDOWN:
					if(drawmap){
					 	destination_rectangle.x = 225; //upper left corner to
						destination_rectangle.y = 319; //place the button.
						destination_rectangle.h = downbuttondown->h; //height &
						destination_rectangle.w = downbuttondown->w;//width of button.
						SDL_BlitSurface(downbuttondown, NULL, screen, &destination_rectangle); // Do the Blit
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen..
						PlaceShips(mapscale, 0, 10, current_target);
						DrawMap();
						SDL_Delay(100);
						DisplayNavigationWidgets();
					}
					if(drawtma){
					 	destination_rectangle.x = 225; //upper left corner to
						destination_rectangle.y = 319; //place the button.
						destination_rectangle.h = downbuttondown->h; //height &
						destination_rectangle.w = downbuttondown->w;//width of button.
						SDL_BlitSurface(downbuttondown, NULL, screen, &destination_rectangle); // Do the Blit
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen..
						PlaceShips(mapscale, 0, 10, current_target);
						DisplayTMA(0,10);
						SDL_Delay(100);
						DisplayTMAWidgets();
					}
					break;
				case SCROLLMAPLEFT:
					if(drawmap){
				 		destination_rectangle.x = 175; //upper left corner to
						destination_rectangle.y = 269; //place the button.
						destination_rectangle.h = leftbuttondown->h; //height &
						destination_rectangle.w = leftbuttondown->w;//width of button.
						SDL_BlitSurface(leftbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
						 PlaceShips(mapscale, -10, 0, current_target);
						DrawMap();
						SDL_Delay(100);
						DisplayNavigationWidgets();
					}
					if(drawtma){
				 		destination_rectangle.x = 175; //upper left corner to
						destination_rectangle.y = 269; //place the button.
						destination_rectangle.h = leftbuttondown->h; //height &
						destination_rectangle.w = leftbuttondown->w;//width of button.
						SDL_BlitSurface(leftbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen....
						 PlaceShips(mapscale, -10, 0, current_target);
						DisplayTMA(-10,0);
						SDL_Delay(100);
						DisplayTMAWidgets();
					}
					break;
				case SCROLLMAPRIGHT:
					if(drawmap){
				 		destination_rectangle.x = 275; //upper left corner to
						destination_rectangle.y = 269; //place the button.
						destination_rectangle.h = rightbuttondown->h; //height &
						destination_rectangle.w = rightbuttondown->w;//width of button.
						SDL_BlitSurface(rightbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
						 PlaceShips(mapscale, 10, 0, current_target);
						DrawMap();
						SDL_Delay(100);
						DisplayNavigationWidgets();
					}
					if(drawtma){
				 		destination_rectangle.x = 275; //upper left corner to
						destination_rectangle.y = 269; //place the button.
						destination_rectangle.h = rightbuttondown->h; //height &
						destination_rectangle.w = rightbuttondown->w;//width of button.
						SDL_BlitSurface(rightbuttondown, NULL, screen, &destination_rectangle); // Do the Blit.
						SDL_UpdateRects(screen, 1, &destination_rectangle); //Show the screen...
						DisplayTMA(10,0);
						SDL_Delay(100);
						DisplayTMAWidgets();
					}
					break;
				case TOGGLESPHERICALTOWED:
					SonarStation.ToggleArrayChoice();
					SonarStation.DisplaySonarWidgets();
					break;
				case TOGGLETRUERELATIVE:
					SonarStation.ToggleTrueRel();
					SonarStation.DisplaySonarWidgets();
					break;
				case UPPERCRTBUTTON:
					SonarStation.UpperCRT_Button();
					break;
				case LOWERCRTBUTTON:
					SonarStation.LowerCRT_Button();
					break;
				case CENTERDISPLAY:
					if(drawmap){
						if(mapcenter) {
							mapcenter = 0;
							DisplayNavigationWidgets();
						}else{
							mapcenter = 1;
							DisplayNavigationWidgets();
						}
						UpdateDisplay();
					}
					if(drawsonar){
						SonarStation.ToggleNorthCenter();
						SonarStation.ClearSonarData();
						UpdateDisplay();
						Message.display_message();
					}
					if(drawtma){
						Tma.ToggleGeoPlotCenter();
						DisplayTMAWidgets();
						UpdateDisplay();
						Message.display_message();
					}
					break;
				case EXTENDARRAY:
					TB16.Extend();
					SonarStation.DisplaySonarWidgets();
					break;
				case RETRACTARRAY:
					TB16.ReelIn();
					SonarStation.DisplaySonarWidgets();
					break;
				case STOPWINCH:
					SonarStation.StopWinch();
					break;
				case ASSIGNTRACKER:
					SonarStation.ToggleAssignTracker();
					SonarStation.DisplaySonarWidgets();
					break;
				case TRACKER1:
					if(SonarStation.assigntracker){
						SonarStation.ToggleAssignTracker();
						Tma.AssignTracker(0,0);
						SonarStation.DisplaySonarWidgets();
					}
					break;
				case TRACKER2:
					if(SonarStation.assigntracker){
						SonarStation.ToggleAssignTracker();
						Tma.AssignTracker(1,0);
						SonarStation.DisplaySonarWidgets();
					}
					break;
				case TRACKER3:
					if(SonarStation.assigntracker){
						SonarStation.ToggleAssignTracker();
						Tma.AssignTracker(2,0);
						SonarStation.DisplaySonarWidgets();
					}
					break;
				case TRACKER4:
					if(SonarStation.assigntracker){
						SonarStation.ToggleAssignTracker();
						Tma.AssignTracker(3,0);
						SonarStation.DisplaySonarWidgets();
					}
					break;
				case DESIGNATECONTACT:
					// textline="CON, SONAR, NEW CONTACT BEARING 123. DESIGNATEING CONTACT S1.";
					// Message.post_message(textline);
					break;
				case TOGGLER10:
					RadarStation.ToggleRangeScale10();
					RadarStation.DisplayWidgets();
					break;
				case TOGGLER20:
					RadarStation.ToggleRangeScale20();
					RadarStation.DisplayWidgets();
					break;
				case TOGGLER30:
					RadarStation.ToggleRangeScale30();
					RadarStation.DisplayWidgets();
					break;
				case TOGGLER40:
					RadarStation.ToggleRangeScale40();
					RadarStation.DisplayWidgets();
					break;
				case TOGGLER50:
					RadarStation.ToggleRangeScale50();
					RadarStation.DisplayWidgets();
					break;
				case TOGGLER60:
					RadarStation.ToggleRangeScale60();
					RadarStation.DisplayWidgets();
					break;
				case RING0:
					RadarStation.ToggleRangeRing0();
					RadarStation.DisplayWidgets();
					break;
				case RING5:
					RadarStation.ToggleRangeRing5();
					RadarStation.DisplayWidgets();
					break;
			        case RING10:
					RadarStation.ToggleRangeRing10();
					RadarStation.DisplayWidgets();
					break;
			        case RING15:
					RadarStation.ToggleRangeRing15();
					RadarStation.DisplayWidgets();
					break;
			        case RING20:
					RadarStation.ToggleRangeRing20();
					RadarStation.DisplayWidgets();
					break;
			     	case RING25:
					RadarStation.ToggleRangeRing25();
					RadarStation.DisplayWidgets();
					break;
				case RADARDOWN:
					RadarStation.LowerMast();
					RadarStation.DisplayWidgets();
					RadarStation.ClearScreen();
					break;
				case RADARUP:
                                        if (Subs[0].Depth < PERISCOPE_DEPTH)
                                        {
					  RadarStation.RaiseMast();
					  RadarStation.DisplayWidgets();
                                        }
					break;
				case ESMDOWN:
					EsmStation.LowerMast();
					EsmStation.DisplayWidgets();
					EsmStation.ClearScreen();
					break;
				case ESMUP:
                                        if (Subs[0].Depth < PERISCOPE_DEPTH)
                                        {
					  EsmStation.RaiseMast();
					  EsmStation.DisplayWidgets();
                                        }
					break;
			        case ASTOP:
					ControlStation.ToggleASTOP();
					ControlStation.DisplayWidgets();
					break;
			        case A13:
					ControlStation.ToggleA13();
					ControlStation.DisplayWidgets();
					break;
			        case A23:
					ControlStation.ToggleA23();
					ControlStation.DisplayWidgets();
					break;
			        case ASTD:
					ControlStation.ToggleASTD();
					ControlStation.DisplayWidgets();
					break;
				case AFULL:
					ControlStation.ToggleAFULL();
					ControlStation.DisplayWidgets();
					break;
			        case AFLK:
					ControlStation.ToggleAFLK();
					ControlStation.DisplayWidgets();
					break;
			        case B13:
					ControlStation.ToggleB13();
					ControlStation.DisplayWidgets();
					break;
				case B23:
					ControlStation.ToggleB23();
					ControlStation.DisplayWidgets();
					break;
			        case BSTD:
					ControlStation.ToggleBSTD();
					ControlStation.DisplayWidgets();
					break;
				case BEMER:
					ControlStation.ToggleBEMER();
					ControlStation.DisplayWidgets();
					break;
                                case USE_TUBE:
                                        // load, fire or unload a tube
                                        status = Subs[0].Use_Tube(tube_action, tube_to_use);
                                        if (status == TUBE_ERROR_FIRE_NOISEMAKER)
                                        {
                                            Submarine *new_torpedo;
                                            char *ship_file, filename[] = "ships/class5.shp";
                                            ship_file = Find_Data_File(filename);
                                            new_torpedo = Subs[0].Fire_Tube(NULL, ship_file);
                                            if ( (ship_file) && (ship_file != filename) )
                                                free(ship_file);
                                            if (new_torpedo)
                                            {
                                                new_torpedo->Friend = FRIEND;
                                                torpedoes = Add_Torpedo(torpedoes, new_torpedo);
                                                Message.post_message("Noise maker launched!");
                                                Message.display_message();
                                            }
                                        }
                                        else if ( (status == TUBE_ERROR_FIRE_SUCCESS) && (current_target > -1) )
                                        {
                                           char *ship_file, filename[] = "ships/class5.shp";
                                           ship_file = Find_Data_File(filename);
                                           if (current_target > -1)
                                           {
                                              Submarine *new_torpedo;
                                              new_torpedo = Subs[0].Fire_Tube( &(Subs[current_target]), ship_file );
                                              if ( (ship_file) && (ship_file != filename) )
                                                free(ship_file);
                                              if (new_torpedo)
                                              {
                                                  new_torpedo->Friend = FRIEND;
                                                  torpedoes = Add_Torpedo(torpedoes, new_torpedo);
                                                  Message.post_message("Torpedo launched!");
                                                  Message.display_message();
                                              }
                                           }
                                          }
                                          else if (status == TUBE_ERROR_FIRE_SUCCESS)
                                           {
                                               Message.post_message("Torpedo has no target.");
                                               Message.display_message();
                                               Subs[0].TorpedosOnBoard++;
                                           }
                                        

                                        tube_action = 0;
                                        tube_to_use = -1;
                                        update_weapons_screen = TRUE;
                                        UpdateDisplay();
                                        break;
				case PAUSEGAME:
					if (pause_game){ //UnPause
						pause_game = !pause_game;
						timer_id = SDL_AddTimer(1000/timecompression, timerfunc, NULL);
						timer_id2 = SDL_AddTimer(60000/timecompression, TmaTimer, NULL);
					}else{  //Pause
						pause_game = !pause_game;
						SDL_RemoveTimer(timer_id);
						SDL_RemoveTimer(timer_id2);
					}
					break;
				default:
					break;
				}
			}
                SDL_Delay(GAME_DELAY);
		}   // end of main loop
	UnLoadWidgets();
        // get rid of torpedoes
        while (torpedoes)
            torpedoes = Remove_Torpedo(torpedoes, torpedoes);
	SDL_Quit();
}

