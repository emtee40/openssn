/***************************************************************************
                          submarine.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: submarine.cpp,v 1.6 2003/04/14 05:51:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include <fstream>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "submarine.h"
#include "files.h"

using namespace std;

Submarine::Submarine(){
    Init();
/*
Moved all of this to init()

        int index;

	//Passive Sonar Cross Section
	//in future make different pscs's
	//based on angle of view, front
	//side and rear.

	PSCS=83.0;
        Active = FALSE;
	CavitationFlag=0;
	Depth=100;
	MaxDepth=1200;
	Speed=5.0;
	MaxSpeed=32;
	MinSpeed=-32;
	DesiredSpeed=0;
	Rudder=10;
	TorpedosOnBoard=23;
        // clear targets
        for (index = 0; index < MAX_SUBS; index++)
        {
           targets[index] = -1;
           target_strength[index] = 0;
        }
        current_target = -1;
*/
}

Submarine::~Submarine()
{
}



void Submarine::Init()
{
        int index;

        //Passive Sonar Cross Section
        //in future make different pscs's
        //based on angle of view, front
        //side and rear.

        PSCS=83.0;
        Active = FALSE;
        CavitationFlag=0;
        Depth=0;
        MaxDepth=999;
        Speed=5.0;
        MaxSpeed=32;
        MinSpeed=-12;
        DesiredSpeed=0;
        Rudder=10;
        TorpedosOnBoard=24;
        NoiseMakers = 12;
        ShipClass = CLASS_MERCHANT;
        // clear targets
        targets = NULL;
        last_target = NULL;
        current_target = -1;
        for (index = 0; index < MAX_TUBES; index++)
           torpedo_tube[index] = TUBE_EMPTY;
        next = NULL;
        target = NULL;
        owner = NULL;
        fuel_remaining = INT_MAX; 
        hull_strength = 1;
        has_sonar = 1;
        mission_status = MISSION_NONE;
}



float Submarine::RadiatedNoise(){
	//This will return how noisey you are

	float	unit=40;
	float	multiplier=4.2;
	float	value=0;

	CheckForCavitation();
	value = PSCS + (unit * CavitationFlag) + (((CheckNegSpeed(Speed)>4.9)*multiplier) * CheckNegSpeed(Speed));
	return value;
}

float Submarine::CheckNegSpeed(float value){
	if(value < 0){
		return 100.0 - (100.0 + value);
	}else{
		return value;
	}
}
void Submarine::CheckForCavitation(){

	//Check to see if we are cavitating...
	// 4-5kts @ 0-100ft.
	// 5-14kts @ 100-150ft
	// 14-35kts @ 150-600ft

	CavitationFlag = 0;

	if ((Depth <100) && (CheckNegSpeed(Speed) > 4.0)){
		if ((CheckNegSpeed(Speed) - 4.0) > (float) Depth / 100.0){
			CavitationFlag = 1;
		}
		return;
	}

	if ((Depth < 150) && (CheckNegSpeed(Speed) > 5.0)){
		if (((CheckNegSpeed(Speed) - 5.0) * 5.555) > (Depth -100)){
			CavitationFlag = 1;
		}
		return;
	}

	if ((Depth < 600) && (CheckNegSpeed(Speed) > 14.0)){
		if (((CheckNegSpeed(Speed) - 14.0) * 21.428) > (Depth - 150)){
			CavitationFlag = 1;
		}
		return;
	}
}
double Submarine::BearingToTarget(Submarine *Target){
	double latdif=0, londif=0, bearing = 0; //atan() needs doubles
	//LatLonDifference(observer, target, &latdif, &londif);

        if (! Target)
           return 0;
	if (Lat_TotalYards > Target->Lat_TotalYards){
		latdif = Lat_TotalYards - Target->Lat_TotalYards;
	}
	else{
		latdif = Target->Lat_TotalYards - Lat_TotalYards;
	}

	if (Lon_TotalYards > Target->Lon_TotalYards){
		londif = Lon_TotalYards - Target->Lon_TotalYards;
	}
	else{
		londif = Target->Lon_TotalYards - Lon_TotalYards;
	}

	if ((Lon_TotalYards < Target->Lon_TotalYards) &&
	(Lat_TotalYards < Target->Lat_TotalYards)){
		bearing = (360 - ((atan(latdif / londif) * 360) / 6.28318530717958647692));
	}
	if ((Lon_TotalYards < Target->Lon_TotalYards) &&
	(Lat_TotalYards > Target->Lat_TotalYards)){
		bearing = (0 + ((atan(latdif / londif) * 360) / 6.28318530717958647692));
	}
	if ((Lon_TotalYards > Target->Lon_TotalYards) &&
	(Lat_TotalYards < Target->Lat_TotalYards)){
		bearing = (180 + ((atan(latdif / londif) * 360) / 6.28318530717958647692));
	}
	if ((Lon_TotalYards > Target->Lon_TotalYards) &&
	(Lat_TotalYards > Target->Lat_TotalYards)){
		bearing = (180 - ((atan(latdif / londif) * 360) / 6.28318530717958647692));
	}
	if (londif == 0){
		if (Lat_TotalYards > Target->Lat_TotalYards){
			bearing = 90;
		}else{
			bearing = 270;
		}
	}
	if (latdif == 0){
		if (Lon_TotalYards > Target->Lon_TotalYards){
			bearing = 180;
		}else{
			bearing = 0;
		}
	}
	return bearing;
}

double Submarine::DistanceToTarget(Submarine *Target){
	double latdif = 0, londif = 0; //sqrt needs doubles
	//LatLonDifference( observer,  target,  &latdif,  &londif );

        if (! Target)
            return 0;

	if (Lat_TotalYards > Target->Lat_TotalYards){
		latdif = Lat_TotalYards - Target->Lat_TotalYards;
	}
	else{
		latdif = Target->Lat_TotalYards - Lat_TotalYards;
	}

	if (Lon_TotalYards > Target->Lon_TotalYards){
		londif = Lon_TotalYards - Target->Lon_TotalYards;
	}
	else{
		londif = Target->Lon_TotalYards - Lon_TotalYards;
	}
	return sqrt((latdif * latdif) + (londif * londif));
}

float Submarine::DEAngle(Submarine *Target){
	double distance = 0.0;
	int depthDifference = 0;
	double deang = 0.0;

	distance = DistanceToTarget(Target);

	if (Depth < Target->Depth){
		depthDifference = (int)(Target->Depth - Depth);
	}else{
		depthDifference = (int)(Depth - Target->Depth);
	}

     if (depthDifference != 0) depthDifference = depthDifference * 3;

	if (distance > depthDifference){
		deang =  (atan(distance / depthDifference) * 360) / 6.283185307179;
	}else{
		deang =  (atan(depthDifference / distance) * 360) / 6.283185307179;
	}

	if (Depth > Target->Depth){
		deang = 90.0 - deang;
	}else{
		deang = deang - 90.0;
	}

	return (float)deang;

}



void Submarine::Handeling(){

	//This is where we change the ships characteristics based on the desired variables
	//First we'll do depth

	float UpBubble; //The greater the amount the faster the change.
	float DownBubble;
	float AmountOfChange; //How much to turn the boat.

        // keep desired depth sane
        if (DesiredDepth < 0)
            DesiredDepth = 0;
        else if (DesiredDepth > MaxDepth)
            DesiredDepth = MaxDepth;

	if (DesiredDepth > Depth){  //Do we need to go up?
		UpBubble = 0.05; //5% up bubble if we're close
		if ((DesiredDepth - Depth) > 20){
			UpBubble = 0.1; //10% Up Bubble
		}
                UpBubble *= 4;
		Depth += ((Speed * 0.185) * UpBubble); //feet per second @ 1kt = 0.185
		if (Depth > DesiredDepth){         //if we have risen past the desired depth
			Depth = DesiredDepth;     //Flatten us out
		}
	}

	if (DesiredDepth < Depth){  //Do we need to go down?
		DownBubble= 0.05; //5% Down Bubble if we're close
		if ((Depth - DesiredDepth) >20){
			DownBubble = 0.1; //10% down bubble
		}
                UpBubble *= 4;
		Depth -= ((Speed * 0.185) * DownBubble);
		if (Depth < DesiredDepth){
			Depth = DesiredDepth;
		}
	}

	//Change Heading

	// AmountOfChange = (Rudder * Speed) * 0.012;
        AmountOfChange = (Rudder * Speed) * 0.025;
        #ifdef DEBUG
        printf("Rudder change %f\n", AmountOfChange);
        #endif
	if (Heading > DesiredHeading){
		if ((Heading - DesiredHeading) < 180){
			Heading = Heading - AmountOfChange;
			if ((Heading < DesiredHeading) &&
			((DesiredHeading - Heading) < AmountOfChange)){
				Heading = (float)DesiredHeading;
			}
		}
		else{
			Heading = Heading + AmountOfChange;
			if ((Heading > DesiredHeading) &&
			((Heading - DesiredHeading) < AmountOfChange)){
				Heading = (float)DesiredHeading;
			}
		}
	}
	else{
		if (Heading < DesiredHeading){
			if ((DesiredHeading - Heading) < 180){
				Heading += AmountOfChange;
				if ((Heading > DesiredHeading) &&
				((Heading - DesiredHeading) < AmountOfChange)){
					Heading = (float)DesiredHeading;
				}
			}
			else{
				Heading = Heading - AmountOfChange;
				if ((Heading < DesiredHeading) &&
				((DesiredHeading - Heading) < AmountOfChange)){
					Heading = (float)DesiredHeading;
				}
			}
		}
	}

	if (Heading > 360){
		Heading = Heading - 360.0;
	}
	else{
		if (Heading < 0){
			Heading = Heading + 360.0;
		}
	}

	// Change Speed

	if(DesiredSpeed > MaxSpeed) DesiredSpeed = MaxSpeed;
	if(DesiredSpeed < MinSpeed) DesiredSpeed = MinSpeed;
	if (DesiredSpeed > Speed){ //Speed Up
		Speed += 0.45; //a little less than 1/2 a knot per second..
		if (Speed > DesiredSpeed) Speed = DesiredSpeed; // Did we go past target speed?
	}

	if (Speed > DesiredSpeed){ //Slow Down
		Speed -= 0.45;
		if (Speed < DesiredSpeed) Speed=DesiredSpeed; // Did we slow too much?
	}
}



// We can detect a new target
// Make sure it is on the list.
// Function returns TRUE on success or FALSE on error
int Submarine::Add_Target(Submarine *new_sub, float signal_strength)
{
   int add_contact, found = FALSE;
   Target *my_target, *new_target, *last_target = NULL;

   if (signal_strength == 2.0)
      add_contact = CONTACT_SOLID;
   else if (signal_strength == 1.0)
      add_contact = CONTACT_WEAK;
   else
      add_contact = 1;

   // check to see if target is already in the list
   my_target = targets;
   while ( (my_target) && (! found) )
   {
       if (my_target->sub == new_sub)
          found = TRUE;
       else
       {
          last_target = my_target;
          my_target = (Target *) my_target->next;
       }
   }   // end of looking for existing target

   if (found)
   {
      my_target->contact_strength += add_contact;   
      if (my_target->contact_strength > CONTACT_SOLID)
         my_target->contact_strength = CONTACT_SOLID;
      return TRUE;
   }
   else // need new contact
   {
      #ifdef DEBUG
      printf("Adding new target\n");
      #endif
      new_target = (Target *) calloc( 1, sizeof(Target) );
      if (! new_target)
          return FALSE;
      new_target->sub = new_sub;
      new_target->contact_strength = add_contact;
      new_target->next = NULL;   // not needed, but let's be careful
      if (last_target)
         last_target->next = new_target;
      else
         targets = new_target;
   }
   return TRUE;
}



// Find and reduce a target from the list
void Submarine::Remove_Target(Submarine *old_sub)
{
   int found = FALSE;
   Target *my_target;

   my_target = targets;
   while ( (! found) && (my_target) )
   {
       if (my_target->sub == old_sub)
       {
          if (my_target->contact_strength < 1)
             Cancel_Target(old_sub);
          else
             my_target->contact_strength -= 2;
          found = TRUE;
       }
       else
          my_target = (Target *)my_target->next;
   }
}

// Find and completely remove a target from the list
void Submarine::Cancel_Target(Submarine *old_sub)
{
   int found = FALSE;
   Target *my_target, *previous = NULL;

   #ifdef DEBUG
   printf("Removing target we can't hear.\n");
   #endif
   my_target = targets;
   while ( (!found) && (my_target) )
   {
       if (my_target->sub == old_sub)
       {
         if (previous)
            previous->next = my_target->next;
         else   // first item in the list
           targets = (Target *) my_target->next;

         free(my_target);
         found = TRUE;
       }
       else
       {
          previous = my_target;
          my_target = (Target *)my_target->next;
       }
   }
}




// This function will let us know if a ship is on our target list
// Returns the signal strength if the ship exists or FALSE if it does not.
int Submarine::Can_Detect(Submarine *a_sub)
{
    int found = FALSE;
    Target *my_target;

    my_target = (Target *)targets;
    while ( (my_target) && (! found) )
    {
       if (my_target->sub == a_sub)
          found = TRUE;
       else
          my_target = (Target *)my_target->next;
    }
    if (found)
      return my_target->contact_strength;
    else 
      return FALSE;
}




// Find the next available target. Return the target
// pointer or NULL if no target is found
Submarine *Submarine::Next_Target()
{
     Target *original = last_target;
     Target *my_target;

     if (! targets)
     {
         last_target = NULL;
         return NULL;    // no possible targets
     }
     if (! original)     // nothing selected before, return first item
     {
        last_target = targets;
        return (Submarine *)last_target->sub;
     }
     
     my_target = (Target *) original->next;
     if (my_target)
     {
        last_target = my_target;
        return (Submarine *)last_target->sub;
     }
     else
     {
        last_target = targets;
        return (Submarine *)last_target->sub;
     }
}



// This function should be called right after the ship is created. It
// will load the ship's class specific information from the appropriate
// data file.
// The data file's format will be
// maxspeed maxdepth name
int Submarine::Load_Class(char *my_file)
{
    // confirm file exists
    if (! my_file)
      return FALSE;
    ifstream infile(my_file, ios::in);
    if (! infile)
       return FALSE;

    // load data
    infile >> MaxSpeed >> MaxDepth >> Rudder >> TorpedosOnBoard >> hull_strength >> has_sonar >> PSCS >> ClassName;
    infile.close();
    return TRUE;
}


// This function tries to load a single line from the passed file
// and translates the contents to the ship's mission.
// The function returns TRUE on success and FALSE if an
// error occurs.
int Submarine::Load_Mission(FILE *from_file)
{
    char line[256];
    
    if (! from_file)
       return FALSE;

    memset(line, '\0', 256);
    fgets(line, 256, from_file);
    if (! strncasecmp(line, "sink", 4) )
        mission_status = MISSION_SINK;
    else if (! strncasecmp(line, "find", 4) )
        mission_status = MISSION_FIND;
    else if (! strncasecmp(line, "alive", 5) )
        mission_status = MISSION_ALIVE;
    else
        mission_status = MISSION_NONE;
    return TRUE;
}




// This function will attempt to manipulate a torpedo tube. We can
// load a torpedo into a tube, load a noisemaker, empty a tube or
// fire the contents of the tube.
// The function returns a status number based on what happened or
// did not happen.
int Submarine::Use_Tube(int action, int tube_number)
{
   if ( (tube_number < 0) || (tube_number >= MAX_TUBES) )
      return TUBE_ERROR_NUMBER;

   switch (action)
   {
     case LOAD_TORPEDO:
          if (TorpedosOnBoard <= 0)
              return TUBE_ERROR_NO_TORPEDO;

          if (torpedo_tube[tube_number] == TUBE_EMPTY)
	  {
              torpedo_tube[tube_number] = TUBE_TORPEDO;
              TorpedosOnBoard--;
              return TUBE_ERROR_TORPEDO_SUCCESS;
          }
          else
            return TUBE_ERROR_FULL;
          break;
     case LOAD_NOISEMAKER:
          if (NoiseMakers <= 0)
               return TUBE_ERROR_NO_NOISE;
          if (torpedo_tube[tube_number] == TUBE_EMPTY)
          {
              torpedo_tube[tube_number] = TUBE_NOISEMAKER;
              NoiseMakers--;
              return TUBE_ERROR_NOISEMAKER_SUCCESS;
          }
          else
             return TUBE_ERROR_FULL;
          break;
      case UNLOAD_TUBE:
            if (torpedo_tube[tube_number] == TUBE_TORPEDO)
                TorpedosOnBoard++;
            else if (torpedo_tube[tube_number] == TUBE_NOISEMAKER)
                NoiseMakers++;
            torpedo_tube[tube_number] = TUBE_EMPTY;
            return TUBE_ERROR_UNLOAD_SUCCESS;
      case FIRE_TUBE:
            if (torpedo_tube[tube_number] == TUBE_EMPTY)
               return TUBE_ERROR_FIRE_FAIL;
            else if (torpedo_tube[tube_number] == TUBE_TORPEDO)
            {
               torpedo_tube[tube_number] = TUBE_EMPTY;
               return TUBE_ERROR_FIRE_SUCCESS;
            }
            else if (torpedo_tube[tube_number] == TUBE_NOISEMAKER)
            {
               torpedo_tube[tube_number] = TUBE_EMPTY;
               return TUBE_ERROR_FIRE_NOISEMAKER;
            } 
            break;

     default: 
          return TUBE_ERROR_ACTION;
   }

   return TRUE;     // this shouldn't happen, but just in case
}




// This function launches a torpedo or noise maker. If the
// passed target is NULL, the new torp/noisemaker is given a
// random course and no target, leaving it to run in a striaght line.
// On success the function passes back a pointer to the new torpedo
// and on failure NULL is returned.
Submarine *Submarine::Fire_Tube(Submarine *target, char *ship_file)
{
   Submarine *my_torp;
   my_torp = new Submarine();
   if (! my_torp)
       return NULL;

   my_torp->Load_Class(ship_file);
   if (target)
   {
       my_torp->target = target;
       // set heading and desired depth
       my_torp->DesiredHeading = my_torp->Heading = BearingToTarget(target);
       my_torp->DesiredDepth = target->Depth;
   }
   else // no target, noisemaker
   {
       my_torp->target = NULL;
       // set random heading
       my_torp->Heading = rand() % 360;
       my_torp->DesiredHeading = my_torp->Heading;
       
   }
   // set current position 
   my_torp->Lat_TotalYards = Lat_TotalYards;
   my_torp->Lon_TotalYards = Lon_TotalYards;
   // set fuel
   my_torp->fuel_remaining = TORPEDO_FUEL;
   // set depth 
   my_torp->Depth = Depth;
   // set speed and desired speed
   my_torp->Speed = Speed;
   my_torp->DesiredSpeed = my_torp->MaxSpeed;
   my_torp->ShipType = TYPE_TORPEDO;
   my_torp->Friend = FOE;
   return my_torp;
}



// This function lets us know if we can hear another ship/sub/
// It returns TRUE if we can hear it and FALSE if we can not.
int Submarine::Can_Hear(Submarine *target)
{

        float Range = DistanceToTarget(target);
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
        float minimum_sound = -45.0;

        if (ShipType == TYPE_TORPEDO)
            minimum_sound *= 2.5;

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
        OwnShipNoise = RadiatedNoise();
        TotalNoise = 10.0 * log10(pow(10.0,OwnShipNoise/10.0) + pow(10.0,AmbientNoise/10.0));
        Gb = (TotalNoise - AmbientNoise) / 2.9;
        Lbp = AmbientNoise + Gb;
        TargetNoise = HisPassiveSonarCrosssection +
        ((NoiseFromSpeed * EffectiveTargetSpeed) + BasisNoiseLevel);
        value = TargetNoise - (20.0 * log10(NauticalMiles) + 1.1 * NauticalMiles) - Lbp;
        // if (!observer)
        //      SonarStation.flowandambientnoise = (Lbp - 34);
        if (value > minimum_sound){
                return TRUE;
        }else{
                return FALSE;
        }

}




// This function will simply return if we do not have a target.
// If we do have a target, and we can hear it, this will adjust the
// torpedo's desired heading and desired depth to match the target.
// The function returns TRUE.
int Submarine::Torpedo_AI()
{
   int can_hear_target;

   if (! target)
      return TRUE;

   // check to see if we can hear the target
   can_hear_target = Can_Hear(target);
   if (can_hear_target)
   {
     DesiredHeading = BearingToTarget(target);
     DesiredDepth = target->Depth;
     DesiredSpeed = MaxSpeed;
   }
   // what if we can't hear the target, go into search mode
   else
   {
      DesiredHeading = Heading + 90;
      DesiredSpeed = MaxSpeed / 2;
   }
   return TRUE;
}


// This function lets us figure out what to do with surface
// ships. By default a surface ships just wanders around
// and make the occasional turn.
// Note: Later we will add hunting, running and shooting at stuff here.
// This function returns TRUE
int Submarine::Ship_AI(Submarine *all_ships, Submarine *all_torpedoes)
{
   int change;
   Submarine *torpedo;
   int can_hear_torpedo;
   double distance;
   int bearing;

   // most important thing we can do is run away from torpedoes
   if (has_sonar)
   {
       // go through all torpedoes and see if any of them
       // chasing us
       torpedo = all_torpedoes;
       while (torpedo)
       {
           if (torpedo->ShipType == TYPE_TORPEDO)
           {
           can_hear_torpedo = Can_Hear(torpedo);
           distance = DistanceToTarget(torpedo);
           if ( (can_hear_torpedo) && (distance < (10 * MILES_TO_YARDS) ) )
           {
               bearing = (int) BearingToTarget(torpedo);
               bearing += 180;
               if (bearing >= 360)
                  bearing = bearing % 360;
               DesiredHeading = bearing;
               DesiredSpeed = MaxSpeed;
               return TRUE;
           }
           }   // end of this really is a torp
           torpedo = torpedo->next;
      }
      // if we got this far we cannot hear a torpedo coming at us
      if (Speed == MaxSpeed)
         DesiredSpeed = MaxSpeed / 2;
   }

   // nothing is going on, we have a 1/100 chance of making a turn
   change = rand() % CHANCE_COURSE;
   if (! change)
   {
      DesiredHeading = Heading + ((rand() % 180) - 90);
      if (DesiredHeading >= 360)
        DesiredHeading = DesiredHeading % 360;
   }

   return TRUE;
}


// This function tells us what AI submarines will do.
// Right now they just make the occasional turn. Later we
// will add depth/speed and combat changed in here.
Submarine *Submarine::Sub_AI(Submarine *all_ships, Submarine *all_torpedoes)
{
   int change;
   Submarine *torpedo;
   int can_hear_torpedo;
   double distance;
   int bearing;
   Submarine *target, *my_torpedoes;
   int status;

   // most important thing we can do is run away from torpedoes
   if (has_sonar)
   {
       // go through all torpedoes and see if any of them
       // chasing us
       torpedo = all_torpedoes;
       while (torpedo)
       {
           if (torpedo->ShipType == TYPE_TORPEDO)
           {
           can_hear_torpedo = Can_Hear(torpedo);
           distance = DistanceToTarget(torpedo);
           if ( (can_hear_torpedo) && (distance < (10 * MILES_TO_YARDS) ) )
           {
               bearing = (int) BearingToTarget(torpedo);
               bearing += 180;
               if (bearing >= 360)
                  bearing = bearing % 360;
               DesiredHeading = bearing;
               DesiredSpeed = MaxSpeed;
               // subs are dive too
               if (torpedo->Depth <= Depth)  // it is above us
                  DesiredDepth = MaxDepth;
               else if (torpedo->Depth > Depth)  // below us
                  DesiredDepth = PERISCOPE_DEPTH;
               return all_torpedoes;
           }
           }   // end of this is a torpedo
           torpedo = torpedo->next;
      }

      // see if we can hear a nearby enemy to shoot at
      target = Have_Enemy_Target(all_ships);
      if (target)
      {
          torpedo_tube[0] = TUBE_TORPEDO;
          status = Use_Tube(FIRE_TUBE, 0);
          if (status == TUBE_ERROR_FIRE_SUCCESS)
          {
              char *ship_file, filename[] = "ships/class5.shp";
              ship_file = Find_Data_File(filename);
              torpedo = Fire_Tube(target, ship_file );
              if ( (ship_file) && (ship_file != filename) )
                   free(ship_file);
              if (torpedo)
              {
                 torpedo->Friend = Friend;
                 torpedo->owner = this;
                 // all_torpedoes = Add_Ship(all_torpedoes, torpedo);
                 if (! all_torpedoes)
                    return torpedo;
                 my_torpedoes = all_torpedoes;
                 while (my_torpedoes)
                 {
                     if (my_torpedoes->next)
                        my_torpedoes = my_torpedoes->next;
                     else
                     {
                       my_torpedoes->next = torpedo;
                       return all_torpedoes;
                     }
                 }
              }
          }
      }

      // if we got this far we cannot hear a torpedo coming at us
      if (Speed == MaxSpeed)
         DesiredSpeed = MaxSpeed / 2;
   }

   // got nothing to do, but perhaps change course
   change = rand() % CHANCE_COURSE;
   if (! change)
   {
     DesiredHeading = Heading + ( (rand() % 100) - 90);
     if (DesiredHeading >= 360)
       DesiredHeading = DesiredHeading % 360;
   }
   return all_torpedoes;
}




// Thisfunction checks on our torpedo to see how it
// is doing. If it has hit its target, we return HIT_TARGET.
// If we run out of fuel, we return OUT_OF_FUEL. Otherwise
// we return STATUS_OK.
// This function just checks status changes, it doesn't do
// anything about them.
int Submarine::Check_Status()
{
   double range;

   // first see if we ran out of fuel
   fuel_remaining--;
   if (fuel_remaining < 1)
      return OUT_OF_FUEL;

   if (target)
   {
      range = DistanceToTarget(target);
      if (range < HITTING_RANGE)
         return HIT_TARGET;
   }

   return STATUS_OK;
}



// This function reduces the remaining strength of the ship's hull.
// If the ship/sub can still float, we return DAMAGE_OK, but if
// the ship is sinking, we return DAMAGE_SINK.
// If the ship sinks, we set Active to FALSE.
int Submarine::Take_Damage()
{
   hull_strength--;
   if (hull_strength < 1)
   {
      Active = FALSE;
      return DAMAGE_SINK;
   }
   else
     return DAMAGE_OK;
}



// This function counts how many torpedoes we have active
// in the water (that we fired).
int Submarine::Count_Torpedoes(Submarine *all_torp)
{
    int count = 0;
    Submarine *torp = all_torp;

    while (torp)
    {
       if (torp->owner == this)
         count++;
       torp = torp->next;
    }
    return count;
}
   

// This function checks the list of ships to see if we have any enemies
// to shoot at.
// To get results, we must have torpedoes on board,
// We must be able to hear them clearly.
// They must be an enemy (Friend must be opposite of our FOE or FRIEND).
// In the case of multiple matches, the closest target is returned.
Submarine *Submarine::Have_Enemy_Target(Submarine *all_ships)
{
    Submarine *current, *min = NULL;
    int current_distance, min_distance = INT_MAX;
    int making_noise;

    // can we shoot?
    if (TorpedosOnBoard < 1)
        return NULL;
    // do we even have enemies?
    if ( (Friend != FRIEND) && (Friend != FOE) )
        return NULL;

    current = all_ships;
    while (current)
    {
       if ( ( (current->Friend == FOE) && (Friend == FRIEND) ) ||
            ( (current->Friend == FRIEND) && (Friend == FOE) ) )
       {
           // we do not like this ship, are they closer than our current min?
           current_distance = DistanceToTarget(current);
           if (current_distance < min_distance)
           {
               // they are close, can we hear them?
               making_noise = Can_Hear(current); 
               if (making_noise)
               {
                   min = current;
                   min_distance = current_distance;
               }
           } 
       }
       current = current->next;
    }
    return min;
}

