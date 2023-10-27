/***************************************************************************
                          main.h  -  description
                             -------------------
    begin                : Tue Sep 19 2000
    copyright            : (C) 2000 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: main.h,v 1.11 2003/07/18 03:50:00 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <SDL.h>
#include <SDL/SDL.h>
#include "contact.h"
#include "control.h"
#include "esm.h"
#include "gameclock.h"
#include "helicopter.h"
#include "map.h"
#include "message.h"
#include "navigation.h"
#include "radar.h"
#include "sonar.h"
#include "submarine.h"
#include "targetmotionanalysis.h"
#include "towedarray.h"
#include "weapons.h"
#include "widget.h"

#ifndef VERSION
#define VERSION 1.4
#endif

// frame rate in the game loop
#define GAME_FRAMERATE 60

#define PERISCOPE_DEPTH 50
#define MAX_SUBS 20

#define DEFAULT_SEA_STATE 3
#define SHIP_HEIGHT 100


/** Our main program and control loop
  * @author Michael Bridak
  */

// Some defs for easy living
#define SONAR                 1
#define NAVMAP                2
#define TMA                   3
#define QUIT                  4
#define COMPRESSTIME          5
#define UNCOMPRESSTIME        6
#define INCREASESPEED         7
#define DECREASESPEED         8
#define DUMPSCREEN            9
#define TURNPORT             10
#define TURNSTARBOARD        11
#define INCREASEDEPTH        12
#define DECREASEDEPTH        13
#define INCREASEMAPSCALE     14
#define DECREASEMAPSCALE     15
#define CENTERDISPLAY        16
#define EXTENDARRAY          17
#define RETRACTARRAY         18
#define STOPWINCH            19
#define PAUSEGAME            20
#define SHIPCONTROL          21
#define SCROLLMAPUP          22
#define SCROLLMAPDOWN        23
#define SCROLLMAPLEFT        24
#define SCROLLMAPRIGHT       25
#define TOGGLESPHERICALTOWED 26
#define TOGGLETRUERELATIVE   27
#define UPPERCRTBUTTON       28
#define LOWERCRTBUTTON       29
#define ASSIGNTRACKER        30
#define TRACKER1             31
#define TRACKER2             32
#define TRACKER3             33
#define TRACKER4             34
#define ESM                  35
#define RADAR                36

// Radar Event Updates
#define TOGGLER10            37
#define TOGGLER20            38
#define TOGGLER30            39
#define TOGGLER40            40
#define TOGGLER50            41
#define TOGGLER60            42
#define RADARDOWN            43
#define RADARUP              44
#define RING0                45
#define RING5                46
#define RING10               47
#define RING15               48
#define RING20               49
#define RING25               50

// Esm Event Updates
#define ESMDOWN              51
#define ESMUP                52

// Control Station Updates
#define ASTOP                53
#define A13                  54
#define A23                  55
#define ASTD                 56
#define AFULL                57
#define AFLK                 58
#define B13                  59
#define B23                  60
#define BSTD                 61
#define BEMER                62
#define DESIGNATECONTACT     63
#define SWITCHTARGET         64
#define WEAPONS              65
#define LOAD_TORPEDO         66
#define LOAD_NOISEMAKER      67
#define UNLOAD_TUBE          68
#define FIRE_TUBE            69
#define WHICHTUBE            70
#define USE_TUBE             71
#define CUTARRAY             72
#define SEND_PING            73
#define UP_THERMAL           74
#define HOLD_DEPTH           75
#define DOWN_THERMAL         76
#define GO_PERISCOPE_DEPTH   77
#define GO_SURFACE           78

// Stations
#define STN_SONAR             1
#define STN_NAV               2
#define STN_TMA               3
#define STN_WEAPONS           4
#define STN_SHIPCONTROL       5
#define STN_ESM               6
#define STN_RADAR             7

// Screens
#define SCREEN_TITLE          0
#define SCREEN_SUCCESS        1
#define SCREEN_FAILURE        2
#define SCREEN_MENU           3

//globals
SDL_Surface *screen;           // The screen we are going to be looking at

Widget titlescreen;            // screen
Widget successscreen;          // screen
Widget failurescreen;          // screen
Widget menuscreen;             // screen

Widget sonarbuttonup;          // button element
Widget sonarbuttondown;        // button element
Widget navbuttonup;            // button element
Widget navbuttondown;          // button element
Widget tmabuttonup;            // button element
Widget tmabuttondown;          // button element
Widget shipcontrolbuttonup;    // button element
Widget shipcontrolbuttondown;  // button element
Widget esmbuttonup;            // button element
Widget esmbuttondown;          // button element
Widget radarbuttonup;          // button element
Widget radarbuttondown;        // button element
Widget quitbuttonup;           // button element
Widget quitbuttondown;         // button element
Widget plusbuttonup;           // button element
Widget plusbuttondown;         // button element
Widget minusbuttonup;          // button element
Widget minusbuttondown;        // button element

int station;  // flag to decide which work station to display

int tube_action = 0;   // what are we doing with a torpedo tube
int tube_to_use = -1;  // the tube to perform this action on

int timecompression;
Uint32 black, white, red, green, yellow, grey, mapcolor;  // Place to hold color info
Uint32 dark_green, dark_red, brown, dark_grey;
bool quit;  // Quit flag Duh!
bool pause_game = false;
Submarine *current_target = NULL;
int my_mission_status;
int should_update_everything = TRUE;

GameClock Clock;
Submarine *Subs = NULL;      // all subs, ships
Submarine *player = NULL;
Contact Contacts[MAX_SUBS];  // Workable contacts
Submarine *torpedoes;        // a linked-list of torpedoes
Helicopter *helicopters;     // a linked-list of helicopters
TowedArray TB16;
msg Message;
MAP my_map;

TargetMotionAnalysis TmaStation;
AnBqq5 SonarStation(TB16, TmaStation, Message);
Navigation NavigationStation;
Weapons WeaponsStation;
Control ControlStation;
Esm EsmStation;
Radar RadarStation;

// declarations
void SetupScreen(bool);
void CreateShips(int mission_number, MAP *map);
void UpdateSensors(void);
void TakeCommands(void);
double RelativeBearing(Submarine *observer, Submarine *target);
int ReciprocalBearing(int bearing);
double AngleOnBow(Submarine *observer, Submarine *target);
int maximize360(int course);
int minimize360(int course);
double CalculateRange(Submarine *observer, Submarine *target);
void LatLonDifference(Submarine *observer, Submarine *target, double *platdif, double *plondif);
void PositionCursor(int, int);
void InitGraphics();  // init the station graphics
void LoadWidgets(void);
void UnLoadWidgets(void);
void DisplayScreen(int screen_to_display);
void DisplayStationWidgets();
void DisplaySubStatus();
void DisplayTimeBox();
void UpdateDisplay(void);
inline int RandInt(int to);
void SoundEnvironment(void);
float Any_Detection(double Range, Submarine *observer, Submarine *target);
float Radar_Detection(double Range, Submarine *observer, Submarine *target);
float Esm_Detection(double Range, Submarine *observer, Submarine *target);
// float Sonar_Detection(double Range, int ObserverShipId, int TargetShipId);
float Sonar_Detection_New(double Range, Submarine *observer, Submarine *target);
inline int Clamp(int);
inline double Clamp(double);
Uint32 timerfunc(Uint32 interval, void *param);
Uint32 TmaTimer(Uint32 interval, void *param);
int HandleInput(SDL_Event &event);

Submarine *Add_Ship(Submarine *all_torpedoes, Submarine *new_torpedo);
Submarine *Remove_Ship(Submarine *all_torpedoes, Submarine *old_torpedo);
void Remove_Inactive_Ship(Submarine *victim);  // find and remove inactive ship
Helicopter *Add_Helicopter(Helicopter *all_helicopters, Helicopter *new_helicopter);
Helicopter *Remove_Helicopter(Helicopter *all_helicopters, Helicopter*old_helicopter);

#endif  // MAIN_H
