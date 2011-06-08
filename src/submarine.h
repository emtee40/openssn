/***************************************************************************
                          submarine.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: submarine.h,v 1.5 2003/04/14 05:51:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "coord.h"
#include "stack.h"


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define TYPE_SUB 0
#define TYPE_SHIP 1
#define TYPE_AIR 2
#define TYPE_TORPEDO 3
#define TYPE_NOISEMAKER 4


#define FRIEND 1
#define FOE 0
#define UNKNOWN 2
#define NEUTRAL 3

#define CONTACT_SOLID 100
#define CONTACT_WEAK 25

#define CLASS_NAME_SIZE 32
#define CLASS_LOSANGELES 1
#define CLASS_AKULA 2
#define CLASS_DESTROYER 3
#define CLASS_MERCHANT 4
#define CLASS_TORPEDO 5
#define CLASS_NOISEMAKER 6

// status of tubes
#define MAX_TUBES 6
#define TUBE_EMPTY 0
#define TUBE_TORPEDO 1
#define TUBE_NOISEMAKER 2


// Actions to do with tubes
// these have weird nmbers for legacy reasons
#define         LOAD_TORPEDO 66
#define         LOAD_NOISEMAKER 67
#define         UNLOAD_TUBE 68
#define         FIRE_TUBE 69

// torpedo tube status codes
#define TUBE_ERROR_NUMBER  1 // we were passed an invalid tube number
#define TUBE_ERROR_FULL 2    // tube has something in it
#define TUBE_ERROR_ACTION 3  // we were passed an invalid action
#define TUBE_ERROR_NO_TORPEDO 4  // we are out of torpedos
#define TUBE_ERROR_NO_NOISE 5    // we are out of noise makers
#define TUBE_ERROR_TORPEDO_SUCCESS 6  // tube loaded successfully
#define TUBE_ERROR_NOISEMAKER_SUCCESS 7  // tube loaded successfully
#define TUBE_ERROR_UNLOAD_SUCCESS 8   // we cleared the tube ok
#define TUBE_ERROR_FIRE_FAIL 9      // could not fire torpedo
#define TUBE_ERROR_FIRE_SUCCESS 10  // we fired a torpedo
#define TUBE_ERROR_FIRE_NOISEMAKER 11   // we fired a noisemaker

#define TORPEDO_FUEL 600
#define HITTING_RANGE 50

#define OUT_OF_FUEL 1
#define HIT_TARGET 2
#define STATUS_OK 3

#define DAMAGE_OK 1
#define DAMAGE_SINK 2

#define CHANCE_COURSE 1000

#define MISSION_NONE 0
#define MISSION_SINK 1
#define MISSION_FIND 2
#define MISSION_ALIVE 3

#define YARDS_TO_MILES 0.000568
#define MILES_TO_YARDS 1760
#define PERISCOPE_DEPTH 50


/**
  *@author Michael Bridak
  */

typedef struct
{
   void *sub;
   void *next;
   int contact_strength;
} Target;


class Submarine : public Coord,  public Stack  {
public:
	float NauticalMiles;
	float HisPassiveSonarCrosssection;
	float EffectiveTargetSpeed;
	float AmbientNoise;
	float OwnShipNoise;
	float flowandambientnoise;
	float TotalNoise;
	float TargetNoise;
	float Gb;
	float Lbp;
	float NoiseFromSpeed;
	float BasisNoiseLevel;
	float value;
	float SeaState; // Anyone want to model the weather.
        int current_target;   // who are we tracking?
	int Active; /*Flag to see if ship is in the world.*/
	int ShipType; /*SubSurface, Surface, Air */
        int ShipClass;  // specific type (Los Angels, Akula, etc)
        char ClassName[CLASS_NAME_SIZE];
	int Friend; /*Flag to mark if ship is a friend/foe/neither*/
	float Depth; /*How deep are we?*/
	int DesiredDepth; /*You want to go how deep!*/
	int MaxDepth; /*Why is my hull crushing!*/
	int DesiredSpeed; /*Scotty warp speed!*/
	int MaxSpeed; /*Captin I've given her all she's got!*/
	int MinSpeed; /*You want to go how fast in reverse!?*/
	int DesiredHeading; 
	int Rudder; /*bend me and she will turn*/
	int TorpedosOnBoard; /*Where are all the fish!*/
        int NoiseMakers;    // to destract the torpedos
	int CavitationFlag; /*Ok Who Farted!*/
	int BaseNoise; /*how noisey are we just sittin' here*/
	float PSCS; /*Passive Sonar Crosssection*/
	float RadiatedNoise(); /*Tell 'em how loud we are.*/
	void CheckForCavitation(); /*self explanitory */
	void Handeling(); /*Turn Left/Right go Up/Down Etc...*/
	float CheckNegSpeed(float); /*if Speed is negative return inverse value*/
	double DistanceToTarget(Submarine *Target);
	double BearingToTarget(Submarine *Target);
	float DEAngle(Submarine *Target);
        int hull_strength;
        int mission_status;
        int has_sonar;
        int torpedo_tube[MAX_TUBES];
        Submarine *next;     // also for torpedos, though could be used
                             // later for ships
        Submarine *target;   // for torpedoes
        Target *last_target;
        int fuel_remaining;  // mostly for torpedos, 
                             // but maybe for electric subs later
        
	Submarine();
	~Submarine();
        void Init();

        // We should be able to track targets
        Target *targets;
        int Add_Target(Submarine *new_sub, float contact_strength);
        void Remove_Target(Submarine *old_sub);
        void Cancel_Target(Submarine *old_sub);
        int Can_Detect(Submarine *a_sub);

        // returns target index or -1 if none is available
        Submarine *Next_Target();

        // Load information on this type of ship/sub
        int Load_Class(char *from_file);
        int Load_Mission(FILE *from_file);
        int Use_Tube(int action, int tube_number);

        Submarine *Fire_Tube(Submarine *target, char *ship_file);
        int Can_Hear(Submarine *target);
        int Torpedo_AI();
        int Ship_AI(Submarine *all_torpedoes);  // figure out what to do with surface ships
        int Sub_AI(Submarine *all_torpedoes);   // like ship AI in 3D
        int Check_Status();         // see if our torpedo is ok
        int Take_Damage();         // we were hit!
};

#endif

