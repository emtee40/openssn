#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helicopter.h"


Helicopter::Helicopter()
{
  Init();
}


Helicopter::~Helicopter()
{
  Cleanup();
}



int Helicopter::Init()
{
  MaxSpeed = 0;
  DesiredHeading = 0;
  Speed = 0;
  Heading = 0;
  current_action = ACTION_HOVER;
  mood = MOOD_PASSIVE;
  Friend = FRIEND;
  destination_x = destination_y = -1;
  ShipType = TYPE_HELICOPTER;
  ShipClass = CLASS_HELICOPTER;
  MaxDepth = LISTEN_HEIGHT;
  Rudder = HELICOPTER_RUBBER;
  hull_strength = 1;
  has_sonar = TRUE;
  PSCS = 360.0;
  firing_countdown = 0;
  next = NULL;
  return TRUE;
}



void Helicopter::Cleanup()
{
   // nothing to clean yet
}


// This function should be called right after the helicopter is created. It
// will load the ship's class specific information from the appropriate
// data file.
// The data file's format will be
// maxspeed maxdepth rudder torpedoes-carried hull_strength has_sonar, P
// SCS class-name class-type
int Helicopter::Load_Class(char *from_file)
{
   FILE *my_file;
   char line[256];
   char *status;

   if (! from_file)
      return FALSE;
   my_file = fopen(from_file, "r");
   if (! my_file)
      return FALSE;

   status = fgets(line, 256, my_file);
   if (status)
   {
       sscanf(line, "%d %d %d %d %d %d %f %s %s",
              &MaxSpeed, &MaxDepth, &Rudder, &TorpedoesOnBoard,
              &hull_strength, &has_sonar, &PSCS, ClassName, ClassType);

   }
   fclose(my_file);
   return TRUE;
}


// Helicopters do not receive mission objectives
// yet, but they may have an entry in the mission file
// for formatting. This function simply
// reads one line from the file and returns
int Helicopter::Load_Mission(FILE *from_file)
{
   char line[256];

   if (! from_file)
        return FALSE;
   fgets(line, 256, from_file);
   return TRUE;
}



// This function lets us know if the helicopter is okay
// if it something has happened to make it die.
// Returns TRUE if the helicopter can still fly or
// FALSE if it has died.
// We might add checks here later for running out
// of fuel....
int Helicopter::Check_Status()
{
  if (hull_strength > 0)
    return TRUE;
  else
    return FALSE;

}



// This function will determine what the helicopeter is doing.
// Ideally we want to patrol more or less randomly, looking
// for enemy submarines. When we hear an enemy we
// will move close, confirm we can still hear it and fire.
// When we are out of torpedoes we should probably
// continue to patrol, and radio sightings
int Helicopter::Helicopter_AI(Submarine *all_ships, Submarine *all_torpedoes)
{
  double distance_to_target;
  int range, new_direction;

  switch (current_action)
  {
      case ACTION_HOVER:
            current_action = ACTION_MOVING;
            break;
      case ACTION_MOVING:
            // no destination, set one
            if ( (destination_x < 0) || (destination_y < 0) )
            {
               // destination_x = Lat_TotalYards + ((rand() % 4000) - 2000);
               // destination_y = Lon_TotalYards + ((rand() % 4000) - 2000);
               // pick direction 0-7
               new_direction = rand() % 8;
               range = MILES_TO_YARDS * 3;
               switch (new_direction)
               {
                  case 0: destination_x = Lat_TotalYards + range;  
                          destination_y = Lon_TotalYards;
                          break; // west
                  case 1: destination_x = Lat_TotalYards + range;
                          destination_y = Lon_TotalYards - range;
                          break;  // sw
                  case 2: destination_x = Lat_TotalYards;
                          destination_y = Lon_TotalYards - range;
                          break;  // south
                  case 3: destination_x = Lat_TotalYards;
                          destination_y = Lon_TotalYards + range;
                          break;  // north
                  case 4: destination_x = Lat_TotalYards - range;
                          destination_y = Lon_TotalYards - range;
                          break; // se
                  case 5: destination_x = Lat_TotalYards - range;
                          destination_y = Lon_TotalYards;
                          break;  // east
                  case 6: destination_x = Lat_TotalYards + range;
                          destination_y = Lon_TotalYards + range;
                          break;  // nw
                  case 7: destination_x = Lat_TotalYards - range;
                          destination_y = Lon_TotalYards + range;
                          break;   // ne
               }   // end of pick new destination
               DesiredDepth = MOVE_HEIGHT;
               DesiredSpeed = MaxSpeed;
            }
            // figure out heading here
            DesiredHeading = Bearing_To_Destination();
            #ifdef DEBUG_HELICOPTER
            printf("DesiredHeading: %d\n", (int) DesiredHeading);
            #endif
            // near destination, then switch to listening
            distance_to_target = Distance_To_Destination();
            if (distance_to_target < MILES_TO_YARDS)
            {
               current_action = ACTION_LISTEN;
               DesiredSpeed = 0;
               DesiredDepth = LISTEN_HEIGHT;
            }
            break;

       case ACTION_LISTEN:
            if (destination_x >= 0)
               destination_x = -1;
            if (destination_y >= 0)
               destination_y = -1;
            if ( (Depth == LISTEN_HEIGHT) && (Speed < 1.0) )
            {
               // check all ships subs to find if we can hear a foe

               // we hear a foe and it is close, fire

               // we hear a foe and it is far away, move

               // we do not hear a foe, but hear a friendly ship
               // move to the firendly ship

               // we do not hear anything, move randomly and listen again

               // when in doubt, we are hovering
               current_action = ACTION_HOVER;
            }
            break;
       default:
          current_action = ACTION_HOVER;
      
     }   // end of switch actions

  return TRUE;
}



// This function handles moving the helicopter. It will
// adjust the craft up/down, turn, and adjust speed.
int Helicopter::Handle()
{
  float AmountOfChange;

   // first, handle height
   if (DesiredDepth > LISTEN_HEIGHT)
       DesiredDepth = LISTEN_HEIGHT;
   else if (DesiredDepth < MOVE_HEIGHT)
       DesiredDepth = MOVE_HEIGHT;

   if (DesiredDepth > Depth)
      Depth++;
   else if (DesiredDepth < Depth)
      Depth--;

   AmountOfChange = Rudder / 10;
   // turn toward desired heading
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

   // sanity check on course
   if (Heading < 0)
      Heading += 360;
   else if (Heading > 359)
      Heading -= 360;

   // adjust speed   
   if (DesiredSpeed > Speed)
      Speed += 1.0;
   else if (DesiredSpeed < Speed)
      Speed -= 1.0;
   if (Speed < 0.0)
      Speed = 0.0;
   else if (Speed > MaxSpeed)
      Speed = MaxSpeed;
   
  return TRUE;
}



// This function returns the bearing (in degrees) to
// our destination. If no destination is set, then
// we return zero.
int Helicopter::Bearing_To_Destination()
{
    double delta_x, delta_y, bearing;

    if ( (destination_x < 0) || (destination_y < 0) )
       return 0;

    delta_x = Lat_TotalYards - destination_x;
    if (delta_x < 0)
       delta_x = -delta_x;
    delta_y = Lon_TotalYards - destination_y;
    if (delta_y < 0)
       delta_y = -delta_y;
    if ( (Lon_TotalYards < destination_y) &&
         (Lat_TotalYards < destination_x) )
        bearing = (360 - ((atan(delta_x / delta_y) * 360) / RADIAN_RATIO));
    else if ( (Lon_TotalYards < destination_y) &&
              (Lat_TotalYards > destination_x) )
        bearing = (0 + ((atan(delta_x / delta_y) * 360) / RADIAN_RATIO));
    else if ( (Lon_TotalYards > destination_y) &&
              (Lat_TotalYards < destination_x) )
        bearing = (180 + ((atan(delta_x / delta_y) * 360) / RADIAN_RATIO) );
    else if ( (Lon_TotalYards > destination_y) &&
              (Lat_TotalYards > destination_y) )
        bearing = (180 - ((atan(delta_x / delta_y) * 360) / RADIAN_RATIO) );
    if (delta_y == 0)
    {
        if (Lat_TotalYards > destination_y)
            bearing = 90;
        else
            bearing = 270;
    }
    if (delta_x == 0)
    {
        if (Lon_TotalYards > destination_x)
            bearing = 180;
        else
            bearing = 0;
    }
    return (int) bearing;
}



// This function lets us know how far away
// we are (in yards) from out destination.
// If no destination is set, we return zero.
double Helicopter::Distance_To_Destination()
{
   int delta_x, delta_y;
   if ( (destination_x < 0) || (destination_y < 0) )
      return 0;

   delta_x = destination_x - Lat_TotalYards;
   if (delta_x < 0)
      delta_x = -delta_x;
   delta_y = destination_y - Lon_TotalYards;
   if (delta_y < 0)
      delta_y = -delta_y;
   
   return sqrt( (delta_x * delta_x) + (delta_y * delta_y) );
}

