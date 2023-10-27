/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Tue Sep 19 17:16:48 PDT 2000
    copyright            : (C) 2000 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: main.cpp,v 1.28 2003/07/18 03:50:00 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 
     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.
 
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "main.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include "dfont.h"
#include "draw.h"
#include "menu.h"
#include "mission.h"
#include "files.h"
#include "sound.h"
#include "winfunctions.h"

//#######GRAPHIC PRIMITIVES#############
void SetupScreen(bool full_screen)
{
    // Initialize the screen and some default colors

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    atexit(SDL_Quit);
    SDL_InitSubSystem(SDL_INIT_TIMER);

    if (full_screen) {
        // Create main screen surface
        screen = SDL_SetVideoMode(1024, 768, 32, SDL_FULLSCREEN);
    } else {
        screen = SDL_SetVideoMode(1024, 768, 32, SDL_SWSURFACE);
    }

    if (screen == NULL) {
        std::cerr << "Couldn't set video mode..." << std::endl
                  << "OpenSSN requires 1024x768 @ 16 bpp" << std::endl
                  << SDL_GetError() << std::endl;
        exit(1);
    }
	
    SDL_WM_SetCaption("OpenSSN http://openssn.sf.net", "OpenSSN");

    // define some standard colors
    black      = SDL_MapRGB(screen->format, 0, 0, 0);
    white      = SDL_MapRGB(screen->format, 255, 255, 255);
    red        = SDL_MapRGB(screen->format, 255, 99, 20);
    dark_red   = SDL_MapRGB(screen->format, 140, 80, 10);
    green      = SDL_MapRGB(screen->format, 0, 255, 0);
    dark_green = SDL_MapRGB(screen->format, 0, 120, 0);
    yellow     = SDL_MapRGB(screen->format, 216, 255, 0);
    brown      = SDL_MapRGB(screen->format, 120, 140, 0);
    grey       = SDL_MapRGB(screen->format, 180, 180, 180);
    dark_grey  = SDL_MapRGB(screen->format, 100, 100, 100);
    mapcolor   = SDL_MapRGB(screen->format, 10, 10, 100);
}

// Let all stations initialize their graphics
void InitGraphics()
{
    SonarStation.InitGraphics(screen);
    NavigationStation.InitGraphics(screen);
    WeaponsStation.InitGraphics(screen);
    TmaStation.InitGraphics(screen);
    ControlStation.InitGraphics(screen);
    EsmStation.InitGraphics(screen);
    RadarStation.InitGraphics(screen);
}

void LoadWidgets()
{
    titlescreen.load("images/tittle.png");
    successscreen.load("images/sub_surfacing.png");
    failurescreen.load("images/sub_rising.png");
    menuscreen.load("images/sub_menu.png");

    sonarbuttonup.load("images/sonarup.png");
    sonarbuttondown.load("images/sonardown.png");
    navbuttonup.load("images/mapup.png");
    navbuttondown.load("images/mapdown.png");
    tmabuttonup.load("images/tmaup.png");
    tmabuttondown.load("images/tmadown.png");
    shipcontrolbuttonup.load("images/shipcontrolup.png");
    shipcontrolbuttondown.load("images/shipcontroldown.png");
    esmbuttonup.load("images/esmup.png");
    esmbuttondown.load("images/esmdown.png");
    radarbuttonup.load("images/radarup.png");
    radarbuttondown.load("images/radardown.png");
    quitbuttonup.load("images/quitbuttonup.png");
    quitbuttondown.load("images/quitbuttondown.png");
    plusbuttonup.load("images/plusbuttonup.png");
    plusbuttondown.load("images/plusbuttondown.png");
    minusbuttonup.load("images/minusbuttonup.png");
    minusbuttondown.load("images/minusbuttondown.png");

    SonarStation.LoadWidgets();
    NavigationStation.LoadWidgets();
    WeaponsStation.LoadWidgets();
    TmaStation.LoadWidgets();
    ControlStation.LoadWidgets();
    EsmStation.LoadWidgets();
    RadarStation.LoadWidgets();
}

void UnLoadWidgets()
{
    // free the surfaces
    titlescreen.unload();
    successscreen.unload();
    failurescreen.unload();
    menuscreen.unload();

    sonarbuttonup.unload();
    sonarbuttondown.unload();
    navbuttonup.unload();
    navbuttondown.unload();
    tmabuttonup.unload();
    tmabuttondown.unload();
    shipcontrolbuttonup.unload();
    shipcontrolbuttondown.unload();
    esmbuttonup.unload();
    esmbuttondown.unload();
    radarbuttonup.unload();
    radarbuttondown.unload();
    quitbuttonup.unload();
    quitbuttondown.unload();
    plusbuttonup.unload();
    plusbuttondown.unload();
    minusbuttonup.unload();
    minusbuttondown.unload();

    SonarStation.UnLoadWidgets();
    NavigationStation.UnLoadWidgets();
    WeaponsStation.UnLoadWidgets();
    TmaStation.UnLoadWidgets();
    ControlStation.UnLoadWidgets();
    EsmStation.UnLoadWidgets();
    RadarStation.UnLoadWidgets();
}

void DisplayScreen(int item)
{
    switch (item) {
        case SCREEN_TITLE:
            titlescreen.draw(screen, 0, 0);
            break;
        case SCREEN_SUCCESS:
            successscreen.draw(screen, 0, 0);
            break;
        case SCREEN_FAILURE:
            failurescreen.draw(screen, 0, 0);
            break;
        case SCREEN_MENU:
            menuscreen.draw(screen, 0, 0);
            break;
        default:
            std::cerr << "Unknown screen item" << std::endl;
            break;
    }
}

void DisplayStationWidgets()
{
    station == STN_SONAR ? sonarbuttondown.draw(screen, 180, 710) : sonarbuttonup.draw(screen, 180, 710);
    station == STN_NAV ? navbuttondown.draw(screen, 280, 710) : navbuttonup.draw(screen, 280, 710);
    station == STN_WEAPONS ? tmabuttondown.draw(screen, 380, 710) : tmabuttonup.draw(screen, 380, 710);
    station == STN_SHIPCONTROL ? shipcontrolbuttondown.draw(screen, 480, 710) : shipcontrolbuttonup.draw(screen, 480, 710);
    station == STN_ESM ? esmbuttondown.draw(screen, 580, 710) : esmbuttonup.draw(screen, 580, 710);
    station == STN_RADAR ? radarbuttondown.draw(screen, 680, 710) : radarbuttonup.draw(screen, 680, 710);
    quit ? quitbuttondown.draw(screen, 780, 710) : quitbuttonup.draw(screen, 780, 710);
}

// This function adds a torpedo to the linked-list of torpedoes
// The returned value is the new linked-list pointer, in case
// the new_torpedo is the first in the list.
Submarine *Add_Ship(Submarine *all_torp, Submarine *new_torp)
{
    Submarine *my_torp;

    // we do not have a torpedo to add
    if (! new_torp)
        return all_torp;

    // empty list
    if (!all_torp)
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
Submarine *Remove_Ship(Submarine *all_torpedoes, Submarine *old_torpedo)
{
    Submarine *my_torp, *previous;

    if (!all_torpedoes)
        return NULL;
    if (!old_torpedo)
        return all_torpedoes;

    my_torp = all_torpedoes;
    previous = NULL;
    while (my_torp) {
        // found match and it is the first in the list
        if ((my_torp == old_torpedo) && (!previous)) {
            previous = my_torp->next;
            delete my_torp;
            return previous;
        }
        // found a match and it isn't the first in the list
        else if (my_torp == old_torpedo) {
            previous->next = my_torp->next;
            delete my_torp;
            return all_torpedoes;
        }
        // this one does not match, go to next node
        else {
            previous = my_torp;
            my_torp = my_torp->next;
        }

    }
    // item wasn't found, just return the complete list
    return all_torpedoes;
}

Helicopter *Add_Helicopter(Helicopter *all_helicopters, Helicopter *new_helicopter)
{
    Helicopter *current;

    if (!new_helicopter)
        return all_helicopters;
    if (!all_helicopters)
        return new_helicopter;

    current = all_helicopters;
    while (current->next)
        current = current->next;
    current->next = new_helicopter;

    return all_helicopters;
}

Helicopter *Remove_Helicopter(Helicopter *all_helicopters, Helicopter *old_helicopter)
{
    Helicopter *current, *previous = NULL;

    if (!all_helicopters)
        return NULL;
    if (!old_helicopter)
        return all_helicopters;

    current = all_helicopters;
    while (current) {
        // first in list
        if ((current == old_helicopter) && (!previous)) {
            previous = current->next;
            delete current;
            return previous;
        }
        // not first in list
        else if (current == old_helicopter) {
            previous->next = current->next;
            delete current;
            return all_helicopters;
        }
        // this is not our node
        else {
            previous = current;
            current = current->next;
        }
    }
    return all_helicopters;
}

//######################################
void ShipHandeling()
{
    Submarine *my_torp, *temp_torp;
    int status;
    Submarine *ship;
    Helicopter *helicopter;

    // see if we can use radar, esm, etc
    if (player->Depth > PERISCOPE_DEPTH) {
        EsmStation.LowerMast();
        RadarStation.LowerMast();
        player->using_radar = FALSE;
    }
    // if array is out, limit speed
    if (TB16.GetLength() > 0) {
        int max_speed = Subs->MaxSpeed / 2;
        if (Subs->Speed > max_speed)
            Subs->Speed = max_speed;
    }

    ship = Subs;
    while (ship) {
        ship->UpdateLatLon();  // Move all the ships
        // see what the AI wants to do
        if (ship != Subs) {
            /*
            if (ship->ShipType == TYPE_SUB)
              torpedoes = ship->Sub_AI(Subs, torpedoes);
            else if (ship->ShipType == TYPE_SHIP)
              torpedoes = ship->Ship_AI(Subs, torpedoes);
            */
            torpedoes = ship->Sub_AI(Subs, torpedoes, helicopters);
        }
        ship->Handeling();  // Steer, Change Depth etc...

        // we were pinging, but it fades quickly
        if (ship->pinging > 0)
            ship->pinging--;
        ship = ship->next;
    }
    helicopter = helicopters;
    while (helicopter) {
        helicopter->UpdateLatLon();
        torpedoes = helicopter->Helicopter_AI(Subs, torpedoes);
        #ifdef DEBUG_HELICOPTER
        printf("Course: %d\nSpeed: %d\nHeight: %d\n",
               (int) helicopter->Heading, (int) helicopter->Speed,
               (int) helicopter->Depth);
        printf("DX: %d DY: %d\nX: %d  Y: %d\n",
               helicopter->destination_x, helicopter->destination_y,
               (int) helicopter->Lat_TotalYards,
               (int) helicopter->Lon_TotalYards);
        #endif
        helicopter->Handle();
        helicopter = helicopter->next;
    }

    for (int x = 0; x < MAX_SUBS; x++) {
        Contacts[x].UpdateContact();
    }

    // move some torps
    my_torp = torpedoes;
    while (my_torp) {
        // check to see if this torpedo is chasing the player
        if ((my_torp->target == player) &&
                (my_torp->fuel_remaining == (TORPEDO_FUEL - 30))) {
            Message.post_message("Torpedo coming our way!");
        }
        my_torp->UpdateLatLon();
        my_torp->Torpedo_AI(Subs);  // see where we should be going
        my_torp->Handeling();  // change heading and depth
        status = my_torp->Check_Status();  // see if we ran into something
        // or we are out of fuel
        if (status == OUT_OF_FUEL) {
            if (my_torp->ShipType == TYPE_TORPEDO)
                Message.post_message("A torpedo ran out of fuel.");
            else
                Message.post_message("Noisemaker stopped running.");
            temp_torp = my_torp->next;
            if (current_target == my_torp)
                current_target = NULL;
            Subs->Cancel_Target(my_torp);
            torpedoes = Remove_Ship(torpedoes, my_torp);
            my_torp = temp_torp;
        } else if (status == HIT_TARGET) {
            int target_status = DAMAGE_OK;
            // damage target
            // we should always have a target, but just in case...
            if (my_torp->target) {
                if (my_torp->target == player)
                    Message.post_message("We have been hit!");
                else
                    Message.post_message("A torpedo hit its target!");
                // radio warning to others
                if ((my_torp->target->ShipType == TYPE_SHIP) &&
                        (my_torp->target->mood == MOOD_CONVOY)) {
                    my_torp->target->Radio_Signal(Subs, RADIO_UNDER_ATTACK);
                }
                target_status = my_torp->target->Take_Damage();
                if (target_status == DAMAGE_SINK) {
                    if (my_torp->target == player)
                        player = NULL;
                    if (my_torp->target->mission_status == MISSION_ALIVE)
                        my_mission_status = MISSION_FAILED;
                    Remove_Inactive_Ship(my_torp->target);
                }
            }
            if (current_target == my_torp)
                current_target = NULL;
            Subs->Cancel_Target(my_torp);
            temp_torp = my_torp->next;
            torpedoes = Remove_Ship(torpedoes, my_torp);
            my_torp = temp_torp;
            // Message.post_message("A torpedo hit its target!");
            if (target_status == DAMAGE_SINK)
                Message.post_message("Target is sinking!");
        } else
            my_torp = my_torp->next;
    }  // end of all torpedoes

    // see if we can still detect the current target
    if (current_target) {
        status = player->Can_Detect(current_target);
        if (status < CONTACT_WEAK)
            current_target = NULL;
    }
}

// This function may get a little messy, we need to do a few things
// to remove a ship completely from the scenario.
// 1. Any torpedoes shooting at this target have to have their
//    target variable set to NULL
// 2. Any ships/subs who were tracking this ship need to stop
void Remove_Inactive_Ship(Submarine *victim)
{
    Submarine *torpedo;
    Submarine *ship;

    if (!victim)  // couldn't find victim, this should never happen, quit
        return;

    // 1. Cancel torpedoes targetting this ship
    torpedo = torpedoes;
    while (torpedo) {
        if (torpedo->target == victim)
            torpedo->target = NULL;
        torpedo = torpedo->next;
    }

    // 2. Ships who were tracking this ship need to stop
    ship = Subs;
    while (ship) {
        ship->Cancel_Target(victim);
        ship = ship->next;
    }

    Subs->last_target = NULL;
    Subs = Remove_Ship(Subs, victim);
    if (victim == current_target)
        current_target = NULL;
}

double RelativeBearing(Submarine *observer, Submarine *target)
{
    // Returns the relative bearing of target to observer. This bearing is
    // measured clockwise from on ships heading to target bearing.
    double relative_bearing;
    int observer_heading, bearing_to_target;
    bearing_to_target = (int) observer->BearingToTarget(target);
    observer_heading = (int) observer->Heading;
    if (observer_heading > bearing_to_target) bearing_to_target += 360;
    relative_bearing = bearing_to_target - observer_heading;
    return relative_bearing;
}

int ReciprocalBearing(int bearing)
{
    // returns the 180 degree opposite of the bearing given to it..
    int recipbearing;
    if (bearing >= 180) {
        recipbearing = bearing - 180;
    } else {
        recipbearing = bearing + 180;
    }
    return recipbearing;
}

double AngleOnBow(Submarine *observer, Submarine *target)
{
    // Returns the Angle on the Bow (AOB) of target relative to observer.
    // Note: -180 < Angle on Bow < 180
    // Positive AOB = Port AOB, Negative AOB = Starboard AOB
    // Examples:
    // target AOB port 30 -> +30
    // target AOB starboard 30 -> -30
    // target pointing at observer -> 0
    // target pointing away from observer -> -180
    double aob = 0;
    aob = target->Heading - observer->Heading
          -   RelativeBearing(observer, target) - 180;
    if (aob > 180) aob = 360 - aob;
    if (aob < -180) aob = 360 + aob;
    return aob;
}

int minimize360(int compass_bearing)
{
    // If course is bigger than 360 degrees, then subtract 360 degrees from it
    // and keep doing so until it is less than 360 degrees.
    while (compass_bearing > 360) {
        compass_bearing -= 360;
    }
    return compass_bearing;
}

int maximize360(int compass_bearing)
{
    // If course is negative then, then add 360 degrees to it
    // and keep doing so until it is positive.
    while (compass_bearing < 0) {
        compass_bearing += 360;
    }
    return compass_bearing;
}

double CalculateRange(Submarine *observer, Submarine *target)
{
    // Calculates the distance in yards from the observer, to the target.

    double latdif = 0, londif = 0;  // sqrt needs doubles
    LatLonDifference(observer, target, &latdif, &londif);
    return sqrt((latdif * latdif) + (londif * londif));
}

void LatLonDifference(Submarine *x, Submarine *y, double *platdif, double *plondif)  // CHANGE TO REFERENCES
{
    // Returns in yards the difference between object x and object y

    if (x->Lat_TotalYards > y->Lat_TotalYards) {
        *platdif = x->Lat_TotalYards - y->Lat_TotalYards;
    } else {
        *platdif = y->Lat_TotalYards - x->Lat_TotalYards;
    }

    if (x->Lon_TotalYards > y->Lon_TotalYards) {
        *plondif = x->Lon_TotalYards - y->Lon_TotalYards;
    } else {
        *plondif = y->Lon_TotalYards - x->Lon_TotalYards;
    }
}

void CreateShips(int mission_number, MAP *map)
{
    char *ship_file, *mission_name;
    char filename[128];
    char line[256], *status;
    // int i;
    FILE *my_file, *mission_file;
    Submarine *new_ship;
    Helicopter *new_helicopter;
    int ship_type, ship_class;

#ifndef WIN32
    snprintf(filename, 128, "data/ships%d.dat", mission_number);
#else
    sprintf(filename, "data/ships%d.dat", mission_number);
#endif

    ship_file = Find_Data_File(filename);
    my_file = fopen(ship_file, "r");
    free(ship_file);

    // idiot check
    if (!my_file) {
        printf("Create ships: file missing\n");
        exit(1);
    }

#ifndef WIN32
    snprintf(filename, 128, "data/mission%d.dat", mission_number);
#else
    sprintf(filename, "data/mission%d.dat", mission_number);
#endif
    mission_name = Find_Data_File(filename);
    mission_file = fopen(mission_name, "r");
    free(mission_name);

    // Read in the data from the ship file
    // Format of ship file: 1 row of data per ship
    // Active flag, Ship Type, Friend flag, Speed, DesiredSpeed, Depth,
    // Desired depth, Heading, Desired
    // Heading, Rudder, Lat_TotalYards, Lon_TotalYards, PSCS

    // Notes: Ship Type: Sub=0, Surface=1, Aircraft=2
    //        Friend: Foe=0, Friend=1, Unknown=2, Neutral=3
    // i = 0;
    status = fgets(line, 256, my_file);
    while (status) {
        sscanf(line, "%d %d", &ship_type, &ship_class);
        if (ship_class == CLASS_HELICOPTER) {
            #ifdef DEBUG
            printf("Creating new helicopter.\n");
            #endif
            new_helicopter = new Helicopter();
            if (new_helicopter) {
                new_helicopter->map = map;
                sscanf(line, "%d %d %d %d %d %d %f %f %d",
                       &(new_helicopter->ShipType),
                       &(new_helicopter->ShipClass),
                       &(new_helicopter->Friend),
                       &(new_helicopter->DesiredSpeed),
                       &(new_helicopter->DesiredDepth),
                       &(new_helicopter->DesiredHeading),
                       &(new_helicopter->Lat_TotalYards),
                       &(new_helicopter->Lon_TotalYards),
                       &(new_helicopter->has_sonar));
                // check mood
#ifndef WIN32
                if (strcasestr(line, "convoy"))
                    new_helicopter->mood = MOOD_CONVOY;
                else if (strcasestr(line, "passive"))
                    new_helicopter->mood = MOOD_PASSIVE;
                else if (strcasestr(line, "attack"))
                    new_helicopter->mood = MOOD_ATTACK;
#else
                if (my_strcasestr(line, "convoy"))
                    new_helicopter->mood = MOOD_CONVOY;
                else if (my_strcasestr(line, "passive"))
                    new_helicopter->mood = MOOD_PASSIVE;
                else if (my_strcasestr(line, "attack"))
                    new_helicopter->mood = MOOD_ATTACK;
#endif
                new_helicopter->Speed = new_helicopter->DesiredSpeed;
                new_helicopter->Heading = new_helicopter->DesiredHeading;
                new_helicopter->Depth = new_helicopter->DesiredDepth;
                // load mission
                new_helicopter->Load_Mission(mission_file);
                // load class
                sprintf(filename, "ships/class%d.shp", new_helicopter->ShipClass);
                ship_file = Find_Data_File(filename);
                new_helicopter->Load_Class(ship_file);
                free(ship_file);
                // add helicopter to list
                helicopters = Add_Helicopter(helicopters, new_helicopter);
            }  // end of created a helicopter
        }  // end of helicopter

        else { // all other classes
            #ifdef DEBUG
            printf("Creating new ship\n");
            #endif
            new_ship = new Submarine();
            if (new_ship) {
                #ifdef DEBUG
                printf("Added new ship to list of ships.\n");
                #endif
                new_ship->Init();
                new_ship->map = map;
                #ifdef DEBUG
                printf("Loaded: %s", line);
                #endif
                sscanf(line, "%d %d %d %d %d %d %f %f",
                       &(new_ship->ShipType),
                       &(new_ship->ShipClass), &(new_ship->Friend),
                       &(new_ship->DesiredSpeed), &(new_ship->DesiredDepth),
                       &(new_ship->DesiredHeading), &(new_ship->Lat_TotalYards),
                       &(new_ship->Lon_TotalYards));  // , &(new_ship->PSCS) );
                // check mood
#ifndef WIN32
                if (strcasestr(line, "convoy"))
                    new_ship->mood = MOOD_CONVOY;
                else if (strcasestr(line, "passive"))
                    new_ship->mood = MOOD_PASSIVE;
                else if (strcasestr(line, "attack"))
                    new_ship->mood = MOOD_ATTACK;
#else
                if (my_strcasestr(line, "convoy"))
                    new_ship->mood = MOOD_CONVOY;
                else if (my_strcasestr(line, "passive"))
                    new_ship->mood = MOOD_PASSIVE;
                else if (my_strcasestr(line, "attack"))
                    new_ship->mood = MOOD_ATTACK;
#endif

                #ifdef DEBUG
                printf("%d %d %d %d %d %d %f %f %d\n",
                       (new_ship->ShipType),
                       (new_ship->ShipClass), (new_ship->Friend),
                       (new_ship->DesiredSpeed), (new_ship->DesiredDepth),
                       (new_ship->DesiredHeading), (new_ship->Lat_TotalYards),
                       (new_ship->Lon_TotalYards), new_ship->mood);
                // , (new_ship->PSCS) );
                #endif
                new_ship->Speed = new_ship->DesiredSpeed;
                new_ship->Depth = new_ship->DesiredDepth;
                new_ship->Heading = new_ship->DesiredHeading;
                #ifdef DEBUG
                printf("Set S: %f H: %f D: %f\n", new_ship->Speed,
                       new_ship->Heading, new_ship->Depth);
                #endif
                new_ship->Load_Mission(mission_file);
                #ifdef DEBUG
                printf("Ship has mission flag: %d\n", new_ship->mission_status);
                #endif
                sprintf(filename, "ships/class%d.shp", new_ship->ShipClass);
                ship_file = Find_Data_File(filename);
                new_ship->Load_Class(ship_file);
                Subs = Add_Ship(Subs, new_ship);
                free(ship_file);
                // i+=1;
            }  // end of successfully created new ship
        }  // end of other classes

        status = fgets(line, 256, my_file);
    }
	
    // inClientFile.close();
    if (my_file)
        fclose(my_file);
    if (mission_file)
        fclose(mission_file);

    // ships = i - 1;
    // ships = i;
    // rdm 5/15/01 testing to be sure correct number of ships being read
    // std::cout << " Number of ships = " <<  i-1 << std::endl;
    if (Subs) {
        SonarStation.setSubs(Subs);
        NavigationStation.setShips(Subs, torpedoes, helicopters);
        NavigationStation.setDepthMap(map);
        TmaStation.setSubs(Subs);
        WeaponsStation.setSubs(Subs);
        ControlStation.setSubs(Subs);
        EsmStation.setSubs(Subs);
        RadarStation.setSubs(Subs);
        player = Subs;
    }
}

void UpdateSensors()
{
// Added code to place our boats position into a FIFO so we
// can calculate where the sonar array is at any time.
// the cable on the TB16 is 2600ft. wich is 866yds. The FIFO is
// 100 elements holding 2 double precision floats. So 866/100
// Is 8.66, Which is the resolution of our placement of the array.
    #ifdef DEBUG
    printf("Updating sensors and stuff\n");
    #endif
    static float distance_traveled;
    distance_traveled += Subs->Speed * 0.555;
    if (distance_traveled >= 8.66) {  // if we've gone 8.66yds record our loc in the FIFO
        TB16.RecordPos(Subs->Lat_TotalYards, Subs->Lon_TotalYards);
        distance_traveled = distance_traveled - 8.66;
    }
    TB16.OperateWinch();  // extend/retract the array as needed.
    SoundEnvironment();  // lets give a listen...
    SonarStation.Sonar(SonarStation.GetNorthCenterState());
    SonarStation.TowedSonar(SonarStation.GetNorthCenterState());
}

// calls the other detection functions to see if there is
// any way we can pick up the target
float Any_Detection(double Range, Submarine *observer, Submarine *target)
{
    float status;

    #ifdef DEBUG
    printf("Attempting all forms of detection.\n");
    #endif
    status = Radar_Detection(Range, observer, target);
    if (status) {
        // printf("I can see on radar target %d\n", target);
        return 2.0;
    }
    status = Esm_Detection(Range, observer, target);
    if (status) {
        // printf("I can pick up on esm target %d\n", target);
        return 1.0;
    }
    // this one takes the most math, so we do it last
    // status = Sonar_Detection(Range, observer, target);
    status = Sonar_Detection_New(Range, observer, target);
    // if (status)
    //     printf("I can hear target %d\n", target);
    return status;
}

// Can I see you on radar?
float Radar_Detection(double Range, Submarine *observer, Submarine *target)
{
    int depth, range;
    bool result;

    depth = (int) observer->Depth;
    range = (int) observer->DistanceToTarget(target);
    result = RadarStation.isTargetVisible(target, range, depth,
                                          SHIP_HEIGHT, DEFAULT_SEA_STATE);
    if (result)
        return 1.0;
    else
        return 0.0;
}

// Can I detect your radar?
float Esm_Detection(double Range, Submarine *observer, Submarine *target)
{
    int depth, range;
    bool result;

    depth = (int) observer->Depth;
    range = (int) observer->DistanceToTarget(target);
    result = EsmStation.isTargetVisible(target, range, depth,
                                        SHIP_HEIGHT, TRUE, DEFAULT_SEA_STATE);
    if (result)
        return 1.0;
    else
        return 0.0;
}

// Can the observer hear the target?
float Sonar_Detection_New(double Range, Submarine *observer, Submarine *target)
{
    float NauticalMiles = (float) Range / 2000.0;
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
    float SeaState = 3.0;  // Anyone want to model the weather?
    int thermal_layers = 0;

    if (target->Speed <= 5.0) {
        EffectiveTargetSpeed = 0.0;
    } else {
        EffectiveTargetSpeed = target->Speed - 5.0;
    }

    if (target->Speed < 20.0) {
        NoiseFromSpeed = 1.30;
        BasisNoiseLevel = 0.0;
    } else {
        NoiseFromSpeed = 0.65;
        BasisNoiseLevel = 9.75;
    }

    thermal_layers = my_map.Thermals_Between(observer->Depth, target->Depth);
    #ifdef DEBUGMAP
    printf("There are %d thermal layers between us.\n", thermal_layers);
    #endif
    AmbientNoise = 89.0 + (5.0 * SeaState);
    OwnShipNoise = observer->RadiatedNoise();
    TotalNoise = 10.0 * log10(pow(10.0, OwnShipNoise / 10.0) + pow(10.0, AmbientNoise / 10.0));
    Gb = (TotalNoise - AmbientNoise) / 2.9;
    Lbp = AmbientNoise + Gb;
    TargetNoise = HisPassiveSonarCrosssection +
                  ((NoiseFromSpeed * EffectiveTargetSpeed) + BasisNoiseLevel);
    #ifdef DEBUGMAP
    printf("Old target noise: %f\n", TargetNoise);
    #endif
    if (thermal_layers)
        TargetNoise -= TargetNoise * (thermal_layers * THERMAL_FILTER);
    #ifdef DEBUGMAP
    printf("New target noise: %f\n", TargetNoise);
    #endif
    value = TargetNoise - (20.0 * log10(NauticalMiles) + 1.1 * NauticalMiles) - Lbp;
    if (!observer)
        SonarStation.setFlowandambientnoise(Lbp - 34);
    if (value > -45.0) {
        return (value - -45.0) + 1.0;
    } else {
        return 0.0;
    }
}

void SoundEnvironment()
{
    //*********************************************************************
    // This will gather sound information from all platforms and place
    // messages into each platforms sonar queue.
    //*********************************************************************

    int bearing;
    double Range;
    Submarine *target;
    int line = FALSE;

    // loop through each ship and let them listen
    // for the other ships...
    #ifdef DEBUG
    printf("Advancing sonar.\n");
    #endif
    Subs->AdvanceSonarHistory();  // advance sonar queue by 1/5th of a second
    TB16.AdvanceSonarHistory();  // advance sonar queue by 1/5th of a second
    // for (target = Subs->next; target; target = target->next) {
    target = Subs->next;
    while (target) {
        #ifdef DEBUG
        printf("Checking if we can hear other ships.\n");
        #endif
        if (!player->InBaffles(target, 1, &TB16)) {  // I'm not deaf?
            Range = CalculateRange(target, Subs);
            float signal;
            signal = Any_Detection(Range, Subs, target);
            if (signal) {  // Are we audible?
                #ifdef DEBUG
                printf("Heard you, adding to list.\n");
                #endif
                bearing = (int) Subs->BearingToTarget(target);  // Change me to float for better tma
                // this weird check prevents us from
                // erasing sonar data with radar/esm data
                if ((signal == 1.0) || (signal == 2.0)) {
                    float temp_signal;
                    temp_signal = Sonar_Detection_New(Range, Subs, target);
                    Subs->RegisterEvent(bearing, temp_signal, target->ShipType);
                } else
                    Subs->RegisterEvent(bearing, signal, target->ShipType);
                // printf("Adding target %d to list.\n", target);
                Subs->Add_Target(target, signal);
            } else {
                // printf("Lost target %d from list.\n", target);
                Subs->Remove_Target(target);
            }
        } else if (!player->InBaffles(target, 2, &TB16) && TB16.GetLength() > 240) {  // do the same for towed array
            Range = TB16.RangeToTarget(target->Lat_TotalYards, target->Lon_TotalYards);
            float signal;
            // signal = Sonar_Detection(Range, 0, target);
            signal = Sonar_Detection_New(Range, Subs, target);
            if (signal) {  // Are we audible?
                bearing = (int) TB16.BearingToTarget(target->Lat_TotalYards, target->Lon_TotalYards);  // Change me to float for better tma
                TB16.RegisterEvent(bearing, signal, target->ShipType);
                // printf("Adding target %d to list.\n", target);
                Subs->Add_Target(target, signal);
            } else {
                // printf("Lost target %d from list.\n", target);
                Subs->Remove_Target(target);
            }
        } else  // can't hear target with either sonar device
            Subs->Remove_Target(target);

        target = target->next;
        if ((!target) && (!line)) {
            target = torpedoes;
            line = TRUE;
        }
    }
}

inline int RandInt(int TO)  // Returns a random integer... TO is upper limit
{
    return (rand() % TO);
}

inline int Clamp(int sample)  // holds a value to be between 0 and 255
{
    if (sample < 0) {
        sample = 0;
    }
    if (sample > 255) {
        sample = 255;
    }
    return sample;
}

inline double Clamp(double sample)  // Overloaded for floats
{
    if (sample < 0) {
        sample = 0.0;
    }
    if (sample > 255) {
        sample = 255.0;
    }
    return sample;
}

// Display the status of the sub: speed / ordered speed, heading / ordered
// heading, depth / ordered depth, and time
void DisplaySubStatus()
{
    int hours, minutes, seconds;
    SDL_Rect rectangle;
    static DFont fnt("images/font.png", "data/font.dat");
    static char text[120];
    rectangle.x = 16;
    rectangle.y = 14;  // define a rectangle on the screen and make it black
    rectangle.h = 72;
    rectangle.w = 126;
    SDL_FillRect(screen, &rectangle, black);
    sprintf(text, "Now - Wanted");
    fnt.PutString(screen, 30, 20, text);
    sprintf(text, "S: [%3i]  -   [%3i]", (int) Subs->Speed, Subs->DesiredSpeed);
    fnt.PutString(screen, 30, 31, text);
    sprintf(text, "H: [%3i]  -   [%3i]", (int) Subs->Heading, Subs->DesiredHeading);
    fnt.PutString(screen, 30, 42, text);
    sprintf(text, "D: [%4i]  -  [%4i]", (int) Subs->Depth, Subs->DesiredDepth);
    fnt.PutString(screen, 30, 53, text);
    // sprintf(text, "ARRAY [%4i]", TB16.GetLength());
    // fnt.PutString(screen, 40, 53, text);
    Clock.GetTime(hours, minutes, seconds);
    sprintf(text, "%.2i:%.2i:%.2i", hours, minutes, seconds);
    fnt.PutString(screen, 40, 64, text);
}

// Display the current time compression factor and the plus/minus buttons
void DisplayTimeBox()
{
    static char text[120];
    static DFont fnt("images/font.png", "data/font.dat");

    // Display the time compression factor
    sprintf(text, "[%i] ", timecompression);
    fnt.PutString(screen, 933, 718, text);

    // Display the plus/minus buttons
    plusbuttonup.draw(screen, 880, 710);
    minusbuttonup.draw(screen, 970, 710);
}

// Update the whole screen: station, status, messages, icons & time compression
void UpdateDisplay()
{
    // Display the current station:
    // - background screen
    // - widgets (buttons)
    // - content (sonar screens, map, esm & radar scopes)
    switch (station) {
        case STN_SONAR:
            SonarStation.UpdateDisplay(current_target);
            break;
        case STN_NAV:
            NavigationStation.UpdateDisplay(current_target);
            break;
        case STN_TMA:
            TmaStation.UpdateDisplay();
            break;
        case STN_WEAPONS:
            WeaponsStation.UpdateDisplay();
            break;
        case STN_SHIPCONTROL:
            ControlStation.UpdateDisplay();
            break;
        case STN_ESM:
            EsmStation.UpdateDisplay();
            break;
        case STN_RADAR:
            RadarStation.UpdateDisplay();
            break;
    }

    // Display the sub status
    DisplaySubStatus();

    // Display the messages
    Message.display_message();

    // Station icons and quit button
    DisplayStationWidgets();

    // Time compression factor and plus/minus buttons
    DisplayTimeBox();
}

/**************************************************
This is our callback function to handle
time critical Functions
**************************************************/
Uint32 timerfunc(Uint32 interval, void *param)
{
    should_update_everything = TRUE;
    return interval;
}

int Update_Everything()
{
    // param = NULL;
    #ifdef DEBUG
    printf("In timer function\n");
    #endif
    Clock.UpdateTime();
    ShipHandeling();
    if (player) {
        UpdateSensors();
        // see if the mission is over
        Check_Find(Subs);
        Check_Alive(Subs);
        my_mission_status = Mission_Status(Subs, my_mission_status);
    }  // player is dead, we should end the mission
    else
        my_mission_status = MISSION_FAILED;
    return TRUE;
}

/***************************************************
Another callback to handle the 60 second
resolution TMA stuff.
****************************************************/
Uint32 TmaTimer(Uint32 interval, void *param)
{
    static Uint32 tick = 0;
    TmaStation.Lock();  // Lock Tma access mutex
    param = NULL;  // Quites error messages.
    tick ++;  // record the time of the tma record.
    TmaStation.RecordBoatPosition(Subs->Lat_TotalYards, Subs->Lon_TotalYards, Subs->BearingToTarget(& (Subs[1])), tick);
    TmaStation.UnLock();  // Unlock mutex
    return interval;
}

int HandleInput(SDL_Event &event)
{
    int mousex = 0, mousey = 0;  // where is the mouse?
    int y1, y2, x_checks_out;
    int index;

    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            mousex = event.button.x;
            mousey = event.button.y;
            if (mousey < 740 && mousey > 711) {
                if (mousex < 212 && mousex > 180) {  // SONAR widget
                    return SONAR;
                    break;
                }
                if (mousex < 312 && mousex > 280) {  // Map Widget
                    return NAVMAP;
                    break;
                }
                if (mousex < 412 && mousex > 380) {  // TMA Widget
                    return WEAPONS;
                    break;
                }
                if (mousex < 512 && mousex > 480) {  // Ship Control Widget
                    return SHIPCONTROL;
                    break;
                }
                if (mousex < 612 && mousex > 580) {  // ESM Widget
                    return ESM;
                    break;
                }
                if (mousex < 712 && mousex > 680) {  // RADAR Widget
                    return RADAR;
                    break;
                }
                if (mousex < 812 && mousex > 780) {  // Quit Widget
                    return QUIT;
                    break;
                }
                if (mousex < 908 && mousex > 884) {  // Time Compression +
                    return COMPRESSTIME;
                    break;
                }

                if (mousex < 1000 && mousex > 972) {  // Time Compression -
                    return UNCOMPRESSTIME;
                    break;
                }
            }
            if (station == STN_NAV || station == STN_TMA) {
                if (mousex > 224 && mousex < 257) {
                    if (mousey > 270 && mousey < 300) {
                        return CENTERDISPLAY;
                        break;
                    }
                }
                if (mousex > 224 && mousex < 257) {
                    if (mousey > 220 && mousey < 245) {
                        return SCROLLMAPUP;
                        break;
                    }
                }
                if (mousex > 224 && mousex < 257) {
                    if (mousey > 320 && mousey < 345) {
                        return SCROLLMAPDOWN;
                        break;
                    }
                }
                if (mousex > 175 && mousex < 210) {
                    if (mousey > 270 && mousey < 300) {
                        return SCROLLMAPLEFT;
                        break;
                    }
                }
                if (mousex > 275 && mousex < 310) {
                    if (mousey > 270 && mousey < 300) {
                        return SCROLLMAPRIGHT;
                        break;
                    }
                }
                if (mousex > 175 && mousex < 210) {
                    if (mousey > 360 && mousey < 390) {
                        return INCREASEMAPSCALE;
                        break;
                    }
                }
                if (mousex > 275 && mousex < 310) {
                    if (mousey > 360 && mousey < 390) {
                        return DECREASEMAPSCALE;
                        break;
                    }
                }
            }
            // Radar events
            if (station == STN_RADAR) {
                if (mousex > 105 && mousex < 512) {
                    if (mousey > 156 && mousey < 566) {
                        RadarStation.selectContact(mousex, mousey);
                        break;
                    }
                }

                if (mousex > 748 && mousex < 795) {
                    if (mousey > 320 && mousey < 367) {
                        return TOGGLER10;
                        break;
                    }
                }

                if (mousex > 795 && mousex < 842) {
                    if (mousey > 320 && mousey < 367) {
                        return TOGGLER20;
                        break;
                    }
                }

                if (mousex > 842 && mousex < 889) {
                    if (mousey > 320 && mousey < 367) {
                        return TOGGLER30;
                        break;
                    }
                }

                if (mousex > 748 && mousex < 795) {
                    if (mousey > 367 && mousey < 414) {
                        return TOGGLER40;
                        break;
                    }
                }

                if (mousex > 795 && mousex < 842) {
                    if (mousey > 367 && mousey < 414) {
                        return TOGGLER50;
                        break;
                    }
                }

                if (mousex > 842 && mousex < 889) {
                    if (mousey > 367 && mousey < 414) {
                        return TOGGLER60;
                        break;
                    }
                }

                if (mousex > 744 && mousex < 791) {
                    if (mousey > 185 && mousey < 232) {
                        return RADARDOWN;
                        break;
                    }
                }

                if (mousex > 850 && mousex < 897) {
                    if (mousey > 185 && mousey < 232) {
                        return RADARUP;
                        break;
                    }
                }

                if (mousex > 749 && mousex < 796) {
                    if (mousey > 501 && mousey < 548) {
                        return RING0;
                        break;
                    }
                }

                if (mousex > 796 && mousex < 843) {
                    if (mousey > 501 && mousey < 548) {
                        return RING5;
                        break;
                    }
                }

                if (mousex > 843 && mousex < 889) {
                    if (mousey > 501 && mousey < 548) {
                        return RING10;
                        break;
                    }
                }

                if (mousex > 548 && mousex < 796) {
                    if (mousey > 548 && mousey < 594) {
                        return RING15;
                        break;
                    }
                }

                if (mousex > 796 && mousex < 843) {
                    if (mousey > 548 && mousey < 594) {
                        return RING20;
                        break;
                    }
                }

                if (mousex > 843 && mousex < 889) {
                    if (mousey > 548 && mousey < 594) {
                        return RING25;
                        break;
                    }
                }
            }

            // Weapon console events
            if (station == STN_WEAPONS) {
                x_checks_out = TRUE;
                y1 = 150;
                y2 = 190;
                // check all x1-x2 possiblities
                if ((mousex > 520) && (mousex < 590))
                    tube_action = LOAD_TORPEDO;
                else if ((mousex > 600) && (mousex < 680))
                    tube_action = LOAD_NOISEMAKER;
                else if ((mousex > 700) && (mousex < 760))
                    tube_action = UNLOAD_TUBE;
                else if ((mousex > 770) && (mousex < 815))
                    tube_action = FIRE_TUBE;
                else {
                    tube_action = 0;
                    x_checks_out = FALSE;
                }
                // look at all y1-y2 button possibilities
                if (x_checks_out) {
                    index = 0;
                    tube_to_use = -1;
                    while ((index < MAX_TUBES) &&
                            (tube_to_use == -1)) {
                        if ((mousey > y1) && (mousey < y2))
                            tube_to_use = index;
                        else {
                            index++;
                            y1 += 50;
                            y2 += 50;
                        }
                    }  // end of checking buttons vertically
                    if (tube_to_use == -1)
                        tube_action = 0;
                } else
                    tube_action = 0;
                if ((tube_action) && (tube_to_use > -1))
                    return USE_TUBE;


            }  // end of weapons screen
            // ESM events
            if (station == STN_ESM) {
                if (mousex > 744 && mousex < 791) {
                    if (mousey > 185 && mousey < 232) {
                        return ESMDOWN;
                        break;
                    }
                }

                if (mousex > 850 && mousex < 897) {
                    if (mousey > 185 && mousey < 232) {
                        return ESMUP;
                        break;
                    }
                }
            }

            // Control events
            if (station == STN_SHIPCONTROL) {
                if (mousex > 158 && mousex < 347) {
                    if (mousey > 250 && mousey < 430) {
                        ControlStation.AdjustHeading(mousex, mousey);
                        break;
                    }
                }
                if (mousex > 112 && mousex < 126) {
                    if (mousey > 254 && mousey < 293) {
                        ControlStation.AdjustDepth(mousex);
                        break;
                    }
                }

                if (mousex > 385 && mousex < 401) {
                    if (mousey > 257 && mousey < 296) {
                        ControlStation.AdjustDepth(mousex);
                        break;
                    }
                }
                if (mousex > 364 && mousex < 404) {
                    if (mousey > 403 && mousey < 418) {
                        return TURNSTARBOARD;
                        break;
                    }
                }

                if (mousex > 108 && mousex < 147) {
                    if (mousey > 405 && mousey < 421) {
                        return TURNPORT;
                        break;
                    }
                }
                if (mousex > 232 && mousex < 252) {
                    if (mousey > 581 && mousey < 607) {
                        return ASTOP;
                        break;
                    }
                }
                if (mousex > 263 && mousex < 287) {
                    if (mousey > 581 && mousey < 607) {
                        return A13;
                        break;
                    }
                }
                if (mousex > 296 && mousex < 320) {
                    if (mousey > 581 && mousey < 607) {
                        return A23;
                        break;
                    }
                }
                if (mousex > 329 && mousex < 352) {
                    if (mousey > 581 && mousey < 607) {
                        return ASTD;
                        break;
                    }
                }
                if (mousex > 364 && mousex < 384) {
                    if (mousey > 581 && mousey < 607) {
                        return AFULL;
                        break;
                    }
                }
                if (mousex > 397 && mousex < 416) {
                    if (mousey > 581 && mousey < 607) {
                        return AFLK;
                        break;
                    }
                }
                if (mousex > 200 && mousex < 218) {
                    if (mousey > 581 && mousey < 607) {
                        return B13;
                        break;
                    }
                }
                if (mousex > 166 && mousex < 185) {
                    if (mousey > 581 && mousey < 607) {
                        return B23;
                        break;
                    }
                }
                if (mousex > 134 && mousex < 152) {
                    if (mousey > 581 && mousey < 607) {
                        return BSTD;
                        break;
                    }
                }
                if (mousex > 100 && mousex < 118) {
                    if (mousey > 581 && mousey < 607) {
                        return BEMER;
                        break;
                    }
                }
                if (mousex > 717 && mousex < 757) {
                    if (mousey > 585 && mousey < 601) {
                        return EXTENDARRAY;
                        break;
                    }
                }
                if (mousex > 717 && mousex < 757) {
                    if (mousey > 612 && mousey < 628) {
                        return RETRACTARRAY;
                        break;
                    }
                }
            }

            if (station == STN_SONAR) {
                if (mousex > 615 && mousex < 657) {
                    if (mousey > 162 && mousey < 210) {
                        return TOGGLETRUERELATIVE;
                        break;
                    }
                }
                if (mousex > 474 && mousex < 519) {
                    if (mousey > 358 && mousey < 401) {
                        return DESIGNATECONTACT;
                        break;
                    }
                }
                if (mousex > 615 && mousex < 657) {
                    if (mousey > 215 && mousey < 261) {
                        return TOGGLESPHERICALTOWED;
                        break;
                    }
                }
                if (mousex > 663 && mousex < 706) {
                    if (mousey > 162 && mousey < 210) {
                        return UPPERCRTBUTTON;
                        break;
                    }
                }
                if (mousex > 663 && mousex < 706) {
                    if (mousey > 215 && mousey < 261) {
                        return LOWERCRTBUTTON;
                        break;
                    }
                }
                if (mousex > 472 && mousex < 520) {
                    if (mousey > 590 && mousey < 638)
                        return SEND_PING;
                }
                if (mousex > 520 && mousex < 567) {
                    if (mousey > 590 && mousey < 638) {
                        return CUTARRAY;
                        break;
                    }
                }
                if (mousex > 569  && mousex < 614) {
                    if (mousey > 593 && mousey < 638) {
                        return STOPWINCH;
                        break;
                    }
                }
                if (mousex > 617 && mousex < 660) {
                    if (mousey > 593 && mousey < 638) {
                        return EXTENDARRAY;
                        break;
                    }
                }
                if (mousex > 663 && mousex < 706) {
                    if (mousey > 593 && mousey < 638) {
                        return RETRACTARRAY;
                        break;
                    }
                }
                if (mousex > 476 && mousex < 517) {
                    if (mousey > 406 && mousey < 447) {
                        return ASSIGNTRACKER;
                        break;
                    }
                }
                if (mousex > 523 && mousex < 564) {
                    if (mousey > 406 && mousey < 447) {
                        return TRACKER1;
                        break;
                    }
                }
                if (mousex > 570 && mousex < 611) {
                    if (mousey > 406 && mousey < 447) {
                        return TRACKER2;
                        break;
                    }
                }
                if (mousex > 617 && mousex < 658) {
                    if (mousey > 406 && mousey < 447) {
                        return TRACKER3;
                        break;
                    }
                }
                if (mousex > 663 && mousex < 706) {
                    if (mousey > 406 && mousey < 447) {
                        return TRACKER4;
                        break;
                    }
                }
            }
            break;
        case SDL_QUIT:
            return QUIT;
            break;
        case SDL_KEYDOWN:  // Process key presses
            switch (event.key.keysym.sym) {
                case SDLK_PAGEUP:  // PgUp   Make Einstein proud.
                    return COMPRESSTIME;
                    break;
                case SDLK_PAGEDOWN:  // PgDown
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
                // case SDLK_n:  // Navigation
                case SDLK_F2:
                    return NAVMAP;
                    break;
                // case SDLK_s:  // Sonar
                case SDLK_F1:
                    return SONAR;
                    break;
                // case SDLK_t:  // TMA
                case SDLK_F3:
                    return WEAPONS;
                    break;
                case SDLK_ESCAPE:  // Quit...
                    return QUIT;
                    break;
                case SDLK_TAB:  // switch target
                    return SWITCHTARGET;
                    break;
                case SDLK_F10:  // Dump the screen
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
                case SDLK_KP4:  // Turn Left
                    return TURNPORT;
                    break;
                case SDLK_KP6:  // Turn Right
                    return TURNSTARBOARD;
                    break;
                case SDLK_KP2:  // Dive
                    return INCREASEDEPTH;
                    break;
                case SDLK_KP8:  // Surface
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
                // case 19:  // Pause The Game
                case SDLK_p:  // Pause The Game
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
                case SDLK_g:
                    return SEND_PING;
                case SDLK_q:
                    return UP_THERMAL;
                case SDLK_a:
                    return HOLD_DEPTH;
                case SDLK_z:
                    return DOWN_THERMAL;
                case SDLK_s:
                    return GO_SURFACE;
                case SDLK_x:
                    return GO_PERISCOPE_DEPTH;
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

int main(int argc, char **argv)
{
    static char text[120];
    int status;
    int option_choice;  // a place to put command line switches
    int screendumpcount = 0;
    bool full_screen = false;
    int mission_number = 0;
    int enable_sound = FALSE;
    SDL_Event event;  // a typedef to hold events
    Uint32 ticks;  // our tick counter
    SDL_TimerID timer_id, timer_id2;
    torpedoes = NULL;
    helicopters = NULL;

    srand(time(NULL));  // Seed the random generator

    // Process command line options
    status = 1;
    while (status < argc) {
        if (argv[status][0] == '-')
            option_choice = argv[status][1];
        else
            option_choice = '\0';
        switch (option_choice) {
            case 'm':
                // mission_number = atoi(optarg);
                mission_number = atoi(argv[status + 1]);
                status++;
                break;
            case 'w':  // they passed the '-w' flag.
                full_screen = false;
                break;
            case 'f':  // full screen
                full_screen = true;
                break;
            /*
            case 'k':  // This option is just for me cause when using kdevelop
                       // I don't start in the right directory.
                chdir("./subsim");
                break;
            */
            case 'v':
                std::cout << "OpenSSN version " << VERSION << std::endl;
                return 0;
            case 'h':
                std::cout << "Usage:" << std::endl
                          << "-m <mission> Select specific mission." << std::endl
                          << "-f For full screen mode." << std::endl
                          << "-s Enable sound effects." << std::endl
                          << "-w For Windowed Mode." << std::endl
                          << "-v For version." << std::endl
                          << "-h For this message." << std::endl;
                return 0;
                break;
            case 's':
                enable_sound = TRUE;
                break;
            default:
                std::cout << "Unknown command-line argument" << std::endl
                          << "Please use -h for a list of commands." << std::endl;
                return 1;
        }  // end of switch
        status++;
    }  // end of while

    SetupScreen(full_screen);
    Init_Audio(enable_sound);
    InitGraphics();
    Message.InitGraphics(screen);
    my_map.InitGraphics(screen);
    Clock.InitTime(12, 15, 0);
    SDL_EnableKeyRepeat(150, 100);
    LoadWidgets();  //load up the screens and buttons
    DisplayScreen(SCREEN_TITLE);  // Display intro screen
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    #ifdef DEBUGMAP
    my_map.Test_Map();
    #endif
    ticks = SDL_GetTicks();  // initialize the tick counter
    SDL_Delay(1000);  // show splash screen for one second
    quit = false;  // reset loop exit flag
    DisplayScreen(SCREEN_MENU);
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    // main menu stuff goes here
    status = Main_Menu(&mission_number, screen);
    if (status == ACTION_QUIT)
        quit = true;

    CreateShips(mission_number, &my_map);
    timecompression = 1;
    station = STN_NAV;  // default station
    sprintf(text, "OpenSSN version %2.1f", VERSION);
    Message.post_message(text);
    Message.post_message("http://openssn.sourceforge.net");
    Message.display_message();
    my_mission_status = MISSION_STARTED;
    timer_id = SDL_AddTimer(1000, timerfunc, NULL);
    timer_id2 = SDL_AddTimer(60000, TmaTimer, NULL);
    const int frameperiod = 1000 / GAME_FRAMERATE;  // how fast we want the displays to update (in milliseconds)... this allows for a fixed frame rate

    // This is the main loop...
    while (!quit) {
        // Events handling
        while (SDL_PollEvent(&event)) {
            // If there are events waiting take care of them
            switch (HandleInput(event)) {
                case SONAR:
                    station = STN_SONAR;
                    Message.post_message("Sonar station");
                    break;
                case NAVMAP:
                    station = STN_NAV;
                    Message.post_message("Navigation display");
                    break;
                case WEAPONS:
                    station = STN_WEAPONS;
                    Message.post_message("Weapons console");
                    break;
                case WHICHTUBE:
                    Message.post_message("Which tube (1-6)?");
                    break;
                case SHIPCONTROL:
                    station = STN_SHIPCONTROL;
                    Message.post_message("Helm control");
                    break;
                case ESM:
                    station = STN_ESM;
                    Message.post_message("ESM station");
                    break;
                case RADAR:
                    station = STN_RADAR;
                    Message.post_message("Radar console");
                    break;
                case SWITCHTARGET:
                    current_target = Subs->Next_Target();
                    break;
                case QUIT:
                    printf("Got quit signal.\n");
                    quit = true;
                    break;
                case COMPRESSTIME:  // Make Einstein proud.
                    plusbuttondown.draw(screen, 880, 710, true);
                    timecompression++;
                    if (timecompression > 8) {
                        timecompression = 8;
                    } else {
                        if (!pause_game) {
                            SDL_RemoveTimer(timer_id);
                            SDL_RemoveTimer(timer_id2);
                            timer_id = SDL_AddTimer(1000 / timecompression, timerfunc, NULL);
                            timer_id2 = SDL_AddTimer(60000 / timecompression, TmaTimer, NULL);
                        }
                    }
                    SDL_Delay(150);
                    break;
                case UNCOMPRESSTIME:  // Take A Downer
                    minusbuttondown.draw(screen, 970, 710, true);
                    timecompression--;
                    if (timecompression < 1) {
                        timecompression = 1;
                    } else {
                        if (!pause_game) {
                            SDL_RemoveTimer(timer_id);
                            SDL_RemoveTimer(timer_id2);
                            timer_id = SDL_AddTimer(1000 / timecompression, timerfunc, NULL);
                            timer_id2 = SDL_AddTimer(60000 / timecompression, TmaTimer, NULL);
                        }
                    }
                    SDL_Delay(150);
                    break;
                case INCREASESPEED:
                    Subs->DesiredSpeed++;
                    break;
                case DECREASESPEED:
                    Subs->DesiredSpeed--;
                    break;
                case DUMPSCREEN:
                    screendumpcount++;
                    sprintf(text, "screendump%i.bmp", screendumpcount);
                    SDL_SaveBMP(screen, text);  // screen dumps
                    std::cerr << "Screen Dump" << std::endl;
                    break;
                case TURNPORT:
                    Subs->DesiredHeading--;
                    if (Subs->DesiredHeading < 0) {
                        Subs->DesiredHeading += 360;
                    }
                    break;
                case TURNSTARBOARD:
                    Subs->DesiredHeading++;
                    if (Subs->DesiredHeading > 359) {
                        Subs->DesiredHeading -= 360;
                    }
                    break;
                case INCREASEDEPTH:
                    Subs->DesiredDepth++;
                    if (Subs->DesiredDepth > Subs->MaxDepth)
                        Subs->DesiredDepth = Subs->MaxDepth;
                    break;
                case DECREASEDEPTH:
                    Subs->DesiredDepth--;
                    if (Subs->DesiredDepth < 0) {
                        Subs->DesiredDepth = 0;
                    }
                    break;
                case INCREASEMAPSCALE:
                    if (station == STN_NAV) {
                        NavigationStation.IncreaseMapScale();
                    }
                    if (station == STN_TMA) {
                        TmaStation.IncreasePlotScale();
                    }
                    break;
                case DECREASEMAPSCALE:
                    if (station == STN_NAV) {
                        NavigationStation.DecreaseMapScale();
                    }
                    if (station == STN_TMA) {
                        TmaStation.DecreasePlotScale();
                    }
                    break;
                case SCROLLMAPUP:
                    if (station == STN_NAV) {
                        NavigationStation.ScrollMapUp(10);
                    }
                    if (station == STN_TMA) {
                        TmaStation.ScrollPlotUp(10);
                    }
                    break;
                case SCROLLMAPDOWN:
                    if (station == STN_NAV) {
                        NavigationStation.ScrollMapDown(10);
                    }
                    if (station == STN_TMA) {
                        TmaStation.ScrollPlotDown(10);
                    }
                    break;
                case SCROLLMAPLEFT:
                    if (station == STN_NAV) {
                        NavigationStation.ScrollMapLeft(10);
                    }
                    if (station == STN_TMA) {
                        TmaStation.ScrollPlotLeft(10);
                    }
                    break;
                case SCROLLMAPRIGHT:
                    if (station == STN_NAV) {
                        NavigationStation.ScrollMapRight(10);
                    }
                    if (station == STN_TMA) {
                        TmaStation.ScrollPlotRight(10);
                    }
                    break;
                case UP_THERMAL:
                    if (player) {
                        status = my_map.Next_Up(player->Depth);
                        if (status != player->Depth) {
                            player->DesiredDepth = status - 25;
                            Message.post_message("Going up one thermal.");
                        }
                    }
                    break;
                case DOWN_THERMAL:
                    if (player) {
                        status = my_map.Next_Down(player->Depth);
                        if (status != player->Depth) {
                            player->DesiredDepth = status + 25;
                            Message.post_message("Going down one thermal.");
                        }
                    }
                    break;
                case HOLD_DEPTH:
                    if (player)
                        player->DesiredDepth = player->Depth;
                    Message.post_message("Holding depth, Captain.");
                    break;
                case GO_SURFACE:
                    if (player)
                        player->DesiredDepth = 0;
                    Message.post_message("Surfacing, Captain!");
                    break;
                case GO_PERISCOPE_DEPTH:
                    if (player)
                        player->DesiredDepth = PERISCOPE_DEPTH;
                    Message.post_message("Heading to periscope depth.");
                    break;

                case TOGGLESPHERICALTOWED:
                    SonarStation.ToggleArrayChoice();
                    break;
                case TOGGLETRUERELATIVE:
                    SonarStation.ToggleTrueRel();
                    break;
                case UPPERCRTBUTTON:
                    SonarStation.UpperCRT_Button();
                    break;
                case LOWERCRTBUTTON:
                    SonarStation.LowerCRT_Button();
                    break;
                case CENTERDISPLAY:
                    if (station == STN_NAV) {
                        NavigationStation.ToggleMapCenter();
                    }
                    if (station == STN_SONAR) {
                        SonarStation.ToggleNorthCenter();
                        SonarStation.ClearSonarData();
                    }
                    if (station == STN_TMA) {
                        TmaStation.ToggleGeoPlotCenter();
                    }
                    break;
                case EXTENDARRAY:
                    status = TB16.Extend();
                    if (status) {
                        Message.post_message("Extending sonar array.");
                    } else
                        Message.post_message("Unable to extend array.");
                    break;
                case RETRACTARRAY:
                    status = TB16.ReelIn();
                    if (status) {
                        Message.post_message("Retrieving sonar array.");
                    } else
                        Message.post_message("Unable to retrieve array.");
                    break;
                case CUTARRAY:
                    status = TB16.CutArray();
                    if (status)
                        Message.post_message("Cut towed array.");
                    else
                        Message.post_message("Cannot cut array.");
                    break;
                case STOPWINCH:
                    SonarStation.StopWinch();
                    Message.post_message("Stopping sonar array.");
                    break;
                case ASSIGNTRACKER:
                    SonarStation.ToggleAssignTracker();
                    break;
                case TRACKER1:
                    if (SonarStation.GetAssignTrackerState()) {
                        SonarStation.ToggleAssignTracker();
                        TmaStation.AssignTracker(0, 0);
                    }
                    break;
                case TRACKER2:
                    if (SonarStation.GetAssignTrackerState()) {
                        SonarStation.ToggleAssignTracker();
                        TmaStation.AssignTracker(1, 0);
                    }
                    break;
                case TRACKER3:
                    if (SonarStation.GetAssignTrackerState()) {
                        SonarStation.ToggleAssignTracker();
                        TmaStation.AssignTracker(2, 0);
                    }
                    break;
                case TRACKER4:
                    if (SonarStation.GetAssignTrackerState()) {
                        SonarStation.ToggleAssignTracker();
                        TmaStation.AssignTracker(3, 0);
                    }
                    break;
                case DESIGNATECONTACT:
                    // Message.post_message("CON, SONAR, NEW CONTACT "
                    //                      "BEARING 123. DESIGNATING CONTACT S1.");
                    break;
                case TOGGLER10:
                    RadarStation.ToggleRangeScale10();
                    break;
                case TOGGLER20:
                    RadarStation.ToggleRangeScale20();
                    break;
                case TOGGLER30:
                    RadarStation.ToggleRangeScale30();
                    break;
                case TOGGLER40:
                    RadarStation.ToggleRangeScale40();
                    break;
                case TOGGLER50:
                    RadarStation.ToggleRangeScale50();
                    break;
                case TOGGLER60:
                    RadarStation.ToggleRangeScale60();
                    break;
                case RING0:
                    RadarStation.ToggleRangeRing0();
                    break;
                case RING5:
                    RadarStation.ToggleRangeRing5();
                    break;
                case RING10:
                    RadarStation.ToggleRangeRing10();
                    break;
                case RING15:
                    RadarStation.ToggleRangeRing15();
                    break;
                case RING20:
                    RadarStation.ToggleRangeRing20();
                    break;
                case RING25:
                    RadarStation.ToggleRangeRing25();
                    break;
                case RADARDOWN:
                    RadarStation.LowerMast();
                    if (player)
                        player->using_radar = FALSE;
                    break;
                case RADARUP:
                    if (Subs->Depth <= PERISCOPE_DEPTH) {
                        RadarStation.RaiseMast();
                        if (player)
                            player->using_radar = TRUE;
                    }
                    break;
                case ESMDOWN:
                    EsmStation.LowerMast();
                    break;
                case ESMUP:
                    if (Subs->Depth <= PERISCOPE_DEPTH) {
                        EsmStation.RaiseMast();
                    }
                    break;
                case ASTOP:
                    ControlStation.ToggleASTOP();
                    break;
                case A13:
                    ControlStation.ToggleA13();
                    break;
                case A23:
                    ControlStation.ToggleA23();
                    break;
                case ASTD:
                    ControlStation.ToggleASTD();
                    break;
                case AFULL:
                    ControlStation.ToggleAFULL();
                    break;
                case AFLK:
                    ControlStation.ToggleAFLK();
                    break;
                case B13:
                    ControlStation.ToggleB13();
                    break;
                case B23:
                    ControlStation.ToggleB23();
                    break;
                case BSTD:
                    ControlStation.ToggleBSTD();
                    break;
                case BEMER:
                    ControlStation.ToggleBEMER();
                    break;
                case SEND_PING:
                    player->Send_Ping(Subs);
                    Message.post_message("Sending active ping, sir.");
                    break;
                case USE_TUBE:
                    // check to see if we already have too
                    // many items out there
                    status = player->Count_Torpedoes(torpedoes);
                    status += player->Count_Noisemakers(torpedoes);
                    if (status >= MAX_PLAYER_WEAPONS) {
                        Message.post_message("Tracking computer full, Captain.");
                        break;
                    }
                    // load, fire or unload a tube
                    status = Subs->Use_Tube(tube_action, tube_to_use);
                    if (status == TUBE_ERROR_FIRE_NOISEMAKER) {
                        Submarine *new_torpedo, *my_torp;
                        char *ship_file, filename[] = "ships/class6.shp";
                        ship_file = Find_Data_File(filename);
                        new_torpedo = Subs->Fire_Tube(NULL, ship_file);
                        free(ship_file);
                        if (new_torpedo) {
                            new_torpedo->Friend = player->Friend;
                            new_torpedo->owner = player;
                            torpedoes = Add_Ship(torpedoes, new_torpedo);
                            Message.post_message("Noise maker launched!");
                            my_torp = torpedoes;
                            while (my_torp) {
                                if (my_torp->target == player)
                                    my_torp->Is_Distracted_By_Noisemaker(new_torpedo);
                                my_torp = my_torp->next;
                            }
                        }
                    } else if ((status == TUBE_ERROR_FIRE_SUCCESS) && (current_target)) {
                        char *ship_file, filename[] = "ships/class5.shp";
                        if (current_target) {
                            Submarine *new_torpedo;
                            ship_file = Find_Data_File(filename);
                            new_torpedo = Subs->Fire_Tube(current_target, ship_file);
                            free(ship_file);
                            if (new_torpedo) {
                                char *sound_file;
                                const char *file_name = "sounds/torpedo-launch.ogg";
                                sound_file = Find_Data_File(file_name);
                                Play_Sound(sound_file);
                                free(sound_file);
                                new_torpedo->Friend = FRIEND;
                                new_torpedo->owner = Subs;
                                torpedoes = Add_Ship(torpedoes, new_torpedo);
                                Message.post_message("Torpedo launched!");
                            }
                        }
                    } else if (status == TUBE_ERROR_FIRE_SUCCESS) {
                        Message.post_message("Torpedo has no target.");
                        Subs->TorpedosOnBoard++;
                    }

                    tube_action = 0;
                    tube_to_use = -1;
                    break;
                case PAUSEGAME:
                    if (pause_game) {  // UnPause
                        pause_game = !pause_game;
                        timer_id = SDL_AddTimer(1000 / timecompression, timerfunc, NULL);
                        timer_id2 = SDL_AddTimer(60000 / timecompression, TmaTimer, NULL);
                    } else {  // Pause
                        pause_game = !pause_game;
                        SDL_RemoveTimer(timer_id);
                        SDL_RemoveTimer(timer_id2);
                    }
                    break;
                default:
                    break;
            }
        }

        // Update sensors
        if (should_update_everything) {
            should_update_everything = FALSE;
            Update_Everything();
        }

        // Render - if time elapsed is more than frame period (or quit)
        if ((SDL_GetTicks() > ticks + frameperiod) || quit) {
            // Update tick counter for current frame
            ticks = SDL_GetTicks();
            RadarStation.Sweep(frameperiod * timecompression);
            // Redraw the whole screen
            UpdateDisplay();
            // Update the display
            SDL_UpdateRect(screen, 0, 0, 0, 0);
        }

        if (my_mission_status == MISSION_SUCCESS) {
            printf("Mission completed successfully!\n");
            DisplayScreen(SCREEN_SUCCESS);
            SDL_Delay(5000);
            quit = true;
        } else if (my_mission_status == MISSION_FAILED) {
            printf("Mission failed.\n");
            DisplayScreen(SCREEN_FAILURE);
            SDL_Delay(5000);
            quit = true;
        }
    }  // end of main loop

    SDL_Delay(200);

    #ifdef DEBUG
    printf("Unloading widgets.\n");
    #endif
    UnLoadWidgets();
    // get rid of torpedoes
    #ifdef DEBUG
    printf("Destroying torpedoes\n");
    #endif
    while (torpedoes)
        torpedoes = Remove_Ship(torpedoes, torpedoes);
    #ifdef DEBUG
    printf("Destroying subs\n");
    #endif
    while (Subs)
        Subs = Remove_Ship(Subs, Subs);
    #ifdef DEBUG
    printf("Removing helicopters.\n");
    #endif
    while (helicopters)
        helicopters = Remove_Helicopter(helicopters, helicopters);
    #ifdef DEBUG
    printf("Killing SDL\n");
    #endif
    Clean_Up_Audio();
    SDL_Quit();
    return 0;  // just to make the compiler happy
}
