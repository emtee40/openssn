/***************************************************************************
                          sonar.cpp  -  description
                             -------------------
    begin                : Fri Apr 11 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: sonar.cpp,v 1.14 2003/09/21 21:52:40 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#include "sonar.h"
#include <cmath>
#include <cstring>
#include "SDL/SDL.h"
#include "dfont.h"
#include "draw.h"
#include "files.h"

AnBqq5::AnBqq5(TowedArray &tb16, TargetMotionAnalysis &tma, msg &msg)
    : TB16(tb16), Tma(tma), Message(msg)
{
    northcenter = true;
    arraychoice5by6 = true;
    bearingdisplay5by6 = true;
    cursorBearing = 0;
}

AnBqq5::~AnBqq5() {}

void AnBqq5::setSubs(Submarine *Subs)
{
    this->Subs = Subs;
}

void AnBqq5::setFlowandambientnoise(float flowandambientnoise)
{
    this->flowandambientnoise = flowandambientnoise;
}

void AnBqq5::InitGraphics(SDL_Surface *screen)
{
    this->screen = screen;
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 368, 280, 32,
                                                screen->format->Rmask,
                                                screen->format->Gmask,
                                                screen->format->Bmask,
                                                screen->format->Amask);
    sonarscreen = SDL_DisplayFormat(surface);
    towedarrayscreen = SDL_DisplayFormat(surface);
    SDL_FreeSurface(surface);
    uppersonarcrt = sonarscreen;
    lowersonarcrt = towedarrayscreen;
    if (screen == NULL || sonarscreen == NULL || towedarrayscreen == NULL) {
        std::cerr << "OOPS" << SDL_GetError() << std::endl;
        exit(1);
    }
    black = SDL_MapRGB(screen->format, 0, 0, 0);
    white = SDL_MapRGB(screen->format, 255, 255, 255);
    green = SDL_MapRGB(screen->format, 0, 255, 0);
}

void AnBqq5::LoadWidgets()
{
    sonarconsole.load("images/Sonarscreen.png");
    ncscale.load("images/nc_scale.png");
    scscale.load("images/sc_scale.png");
    truerel[0].load("images/rel_true.png");
    truerel[1].load("images/true_rel.png");
    sphertowed[0].load("images/towedspherical.png");
    sphertowed[1].load("images/sphericaltowed.png");
    uppercrtoff.load("images/uppercrtoff.png");
    uppercrton.load("images/uppercrton.png");
    lowercrtoff.load("images/lowercrtoff.png");
    lowercrton.load("images/lowercrton.png");
    tb16winchon.load("images/tb16winchon.png");
    tb16winchoff.load("images/tb16winchoff.png");
    extendtb16[0].load("images/extendtb16off.png");
    extendtb16[1].load("images/extendtb16on.png");
    retracttb16[0].load("images/retracttb16off.png");
    retracttb16[1].load("images/retracttb16on.png");
    cutarray.load("images/cutarray.png");
    sendping.load("images/ping_button.png");
    assigntrackerwidget[0].load("images/assigntrackeroff.png");
    assigntrackerwidget[1].load("images/assigntrackeron.png");
    tracker1[0].load("images/track1.png");
    tracker1[1].load("images/track1assigned.png");
    tracker2[0].load("images/track2.png");
    tracker2[1].load("images/track2assigned.png");
    tracker3[0].load("images/track3.png");
    tracker3[1].load("images/track3assigned.png");
    tracker4[0].load("images/track4.png");
    tracker4[1].load("images/track4assigned.png");
}

void AnBqq5::UnLoadWidgets()
{
    // free the surfaces
    sonarconsole.unload();
    ncscale.unload();
    scscale.unload();
    truerel[0].unload();
    truerel[1].unload();
    sphertowed[0].unload();
    sphertowed[1].unload();
    uppercrtoff.unload();
    uppercrton.unload();
    lowercrtoff.unload();
    lowercrton.unload();
    tb16winchon.unload();
    tb16winchoff.unload();
    extendtb16[0].unload();
    extendtb16[1].unload();
    retracttb16[0].unload();
    retracttb16[1].unload();
    cutarray.unload();
    sendping.unload();
    assigntrackerwidget[0].unload();
    assigntrackerwidget[1].unload();
    tracker1[0].unload();
    tracker1[1].unload();
    tracker2[0].unload();
    tracker2[1].unload();
    tracker3[0].unload();
    tracker3[1].unload();
    tracker4[0].unload();
    tracker4[1].unload();
}

void AnBqq5::DisplayWidgets()
{
    // Sonar console
    sonarconsole.draw(screen, 0, 0);
    // True/relative bearing / Spherical/towed array
    truerel[bearingdisplay5by6].draw(screen, 614, 161);
    sphertowed[arraychoice5by6].draw(screen, 614, 215);
    // Upper CRT / Lower CRT
    uppercrtoff.draw(screen, 661, 161);
    lowercrtoff.draw(screen, 661, 215);
    // Assign tracker / tracker 1 / tracker 2 / tracker 3 / tracker 4
    assigntrackerwidget[assigntracker].draw(screen, 473, 403);
    tracker1[Tma.GetTrackerState(0)].draw(screen, 520, 403);
    tracker2[Tma.GetTrackerState(1)].draw(screen, 567, 403);
    tracker3[Tma.GetTrackerState(2)].draw(screen, 614, 403);
    tracker4[Tma.GetTrackerState(3)].draw(screen, 661, 403);
    // Send ping / Cut array / Stop winch / Extend / Retract
    sendping.draw(screen, 472, 590);
    cutarray.draw(screen, 520, 590);
    tb16winchoff.draw(screen, 567, 590);
    extendtb16[TB16.winch == 1].draw(screen, 614, 590);
    retracttb16[TB16.winch == 2].draw(screen, 661, 590);
}

void AnBqq5::ClearSonarData()  // when the display is switched from north centered
{
    // to south centered, we must erase the old display data
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.h = 200;
    rectangle.w = 368;
    SDL_FillRect(sonarscreen, &rectangle, black);
    SDL_FillRect(towedarrayscreen, &rectangle, black);
}

void AnBqq5::Sonar(bool center)
/********************************************
    This routine checks the sonar event queue
    and if there is an event, plots the sound
    source on the waterfall display. It's called
    every 1/5 of a second. and in turn calls
    AdvanceSonarScreen() every second.
*********************************************/
{
    Uint32 tracecolor;  // We have to put the color somewhere
    int direction;
    float signal;
    int ship_type;
    double combined_sound;

    if (Subs->GetCount() > 0) {
        // Are there any sound events at this time index to worry about?
        for (int event = 1; event <= Subs->GetCount(); event++) {
            // If so step through the events
            Subs->GetEvent(event, &direction, &signal, &ship_type);
            // TargetId not used now but maybe later in TMA

            // visible mono color from 70 - 255
            combined_sound = (signal * 4) + flowandambientnoise;
            if (combined_sound > 255.0)
                combined_sound = 255.0;
            switch (ship_type) {
                case TYPE_SHIP:
                    tracecolor = SDL_MapRGB(sonarscreen->format, 0, (int) combined_sound, 0);
                    break;
                case TYPE_TORPEDO:
                    tracecolor = SDL_MapRGB(sonarscreen->format, (int) combined_sound, 0, 0);
                    break;
                case TYPE_SUB:
                default:
                    tracecolor = SDL_MapRGB(sonarscreen->format, 0, 0, (int) combined_sound);
                    break;
            }
            // Change brightness based on strength of signal
            int tdirection = direction;
            for (int xx = 1; xx < 6; xx++) {
                direction = (-3 + RandInt(6)) + tdirection;
                // throw some variances in the bearing nothing is perfekt!...
                if (direction < 0) direction += 360;    // Christ I hate roll overs in degrees
                if (direction > 359) direction -= 360;

                if (!center) {  // Do we want the sonar display to be north centered ?
                    DrawPixel(sonarscreen, direction, 0, tracecolor);   // plots on 0-60 sec display
                    DrawPixel(sonarscreen, direction, 70, tracecolor);  // plots on 0-30 min display
                    DrawPixel(sonarscreen, direction, 140, tracecolor); // plots on 0-2 hr display
                } else {  // draw traces for south centered display
                    DrawPixel(sonarscreen, ReciprocalBearing(direction),
                           0, tracecolor); //plots on 0-60 sec display
                    DrawPixel(sonarscreen, ReciprocalBearing(direction),
                           70, tracecolor); //plots on 0-30 min display
                    DrawPixel(sonarscreen, ReciprocalBearing(direction),
                           140, tracecolor); //plots on 0-2 hr display
                }
            }
        }
    }
    if (!center) {
        DrawPixel(sonarscreen, (int) Subs->Heading, 0, white);  // plot our heading
    } else {
        DrawPixel(sonarscreen, ReciprocalBearing((int)Subs->Heading),
               0, white);  // plots our heading
    }
    AdvanceSonarDisplay();  // advance the screen
    UpdateCursor();
}

void AnBqq5::DisplaySonar()
{
    SDL_Rect destination_rectangle;
    SDL_Rect source_rectangle;
    DisplayBearingScale(northcenter);
    DisplayCursor();

    if (uppersonarcrt != NULL) {
        source_rectangle.x = 0;
        source_rectangle.y = 0;
        source_rectangle.w = 360;
        source_rectangle.h = 200;
        destination_rectangle.x = 52;
        destination_rectangle.y = 150;
        destination_rectangle.w = 360;
        destination_rectangle.h = 200;
        SDL_BlitSurface(uppersonarcrt, &source_rectangle, screen, &destination_rectangle);
    }

    if (lowersonarcrt != NULL) {
        source_rectangle.x = 0;
        source_rectangle.y = 0;
        source_rectangle.w = 360;
        source_rectangle.h = 200;
        destination_rectangle.x = 52;
        destination_rectangle.y = 450;
        destination_rectangle.w = 360;
        destination_rectangle.h = 200;
        SDL_BlitSurface(lowersonarcrt, &source_rectangle, screen, &destination_rectangle);
    }
}

void AnBqq5::UpdateDisplay(Submarine *current_target)
{
    static char text[120];
    static DFont largeFont("images/largefont.png", "data/largefont.dat");
    int tempint;
    // Display console and widgets
    DisplayWidgets();
    // Draw sonar screen
    DisplaySonar();
    // Display bearing and deangle of current target
    if (current_target) {
        tempint = (int) Subs->BearingToTarget(current_target);
        sprintf(text, "%4i", tempint);
        largeFont.PutString(screen, 840, 412, text);
        Subs->DEAngle(current_target);
        if (deAngle > 0) {
            sprintf(text, "+%3.1f", deAngle);
        } else {
            sprintf(text, "%3.1f", deAngle);
        }
        largeFont.PutString(screen, 840, 498, text);
    }
}

void AnBqq5::TowedSonar(bool center)
/********************************************
    This routine checks the sonar event queue
    and if there is an event, plots the sound
    source on the waterfall display. It's called
    every 1/5 of a second. and in turn calls
    AdvanceSonarScreen() every second.
*********************************************/
{
    Uint32 tracecolor;  // We have to put the color somewhere
    int direction, ambiguous_direction, bearing, ambiguous_relative_bearing;
    int ship_type;
    float signal;
    double combined_sound;
    int TB16_Count = TB16.GetCount();
    if (TB16_Count > 0) {
        // Are there any sound events at this time index to worry about?
        for (int event = 1; event <= TB16_Count; event++) {
            // If so step through the events
            TB16.GetEvent(event, &bearing, &signal, &ship_type);
            ambiguous_relative_bearing = (int) TB16.BearingAmbiguity((float)bearing);
            // TargetId not used now but maybe later in TMA
            // visible mono color from 70 - 255
            combined_sound = (signal * 4) + flowandambientnoise;
            if (combined_sound > 255.0)
                combined_sound = 255.0;
            switch (ship_type) {
                case TYPE_SHIP:
                    tracecolor = SDL_MapRGB(sonarscreen->format, 0, (int) combined_sound, 0);
                    break;
                case TYPE_TORPEDO:
                    tracecolor = SDL_MapRGB(sonarscreen->format, (int) combined_sound, 0, 0);
                    break;
                case TYPE_SUB:
                default:
                    tracecolor = SDL_MapRGB(sonarscreen->format, 0, 0, (int) combined_sound);
                    break;
            }

            // Change brightness based on strength of signal
            for (int xx = 1; xx < 6; xx++) {
                direction = bearing;
                // if (direction >= 360) direction -= 360;
                direction = (-3 + RandInt(6)) + direction;
                // throw some variances in the bearing nothing is perfekt!...
                if (direction < 0) direction += 360;
                if (direction > 359) direction -= 360;
                ambiguous_direction = ambiguous_relative_bearing;
                // if (ambiguous_direction >= 360) ambiguous_direction -= 360;
                ambiguous_direction = (-3 + RandInt(6)) + ambiguous_direction;
                if (ambiguous_direction < 0) ambiguous_direction += 360;
                if (ambiguous_direction > 359) ambiguous_direction -= 360;
                if (!center) {  // Do we want the sonar display to be north centered?
                    DrawPixel(towedarrayscreen, direction, 0, tracecolor);    // plots on 0-60 sec display
                    DrawPixel(towedarrayscreen, direction, 70, tracecolor);   // plots on 0-30 min display
                    DrawPixel(towedarrayscreen, direction, 140, tracecolor);  // plots on 0-2 hr display
                    DrawPixel(towedarrayscreen, ambiguous_direction, 0, tracecolor);    // Do the same for the
                    DrawPixel(towedarrayscreen, ambiguous_direction, 70, tracecolor);   // ambiguous bearing
                    DrawPixel(towedarrayscreen, ambiguous_direction, 140, tracecolor);  // returned from towed array
                } else {  // draw traces for south centered display
                    DrawPixel(towedarrayscreen, ReciprocalBearing(direction),
                           0, tracecolor);    // plots on 0-60 sec display
                    DrawPixel(towedarrayscreen, ReciprocalBearing(direction),
                           70, tracecolor);   // plots on 0-30 min display
                    DrawPixel(towedarrayscreen, ReciprocalBearing(direction),
                           140, tracecolor);  // plots on 0-2 hr display

                    DrawPixel(towedarrayscreen, ReciprocalBearing(ambiguous_direction),
                           0, tracecolor);    // plots on 0-60 sec display
                    DrawPixel(towedarrayscreen, ReciprocalBearing(ambiguous_direction),
                           70, tracecolor);   // plots on 0-30 min display
                    DrawPixel(towedarrayscreen, ReciprocalBearing(ambiguous_direction),
                           140, tracecolor);  // plots on 0-2 hr display
                }
            }
        }
    }
    if (!center) {
        DrawPixel(towedarrayscreen, (int) Subs->Heading, 0, white);  // plot our heading
    } else {
        DrawPixel(towedarrayscreen, ReciprocalBearing((int)Subs->Heading),
               0, white);  // plots our heading
    }
    AdvanceTB16Screen();  // advance the screen
}

void AnBqq5::AdvanceSonarDisplay()
{
    SDL_Rect destination_rectangle;
    SDL_Rect source_rectangle;
    static int count = 0;
    static int count2 = 0;
    int my_noise;
    Uint32 noisecolor;
    // move the minute display down
    source_rectangle.x = 0;  // sonar screen minus last line
    source_rectangle.y = 0;
    source_rectangle.w = 360;
    source_rectangle.h = 59;
    destination_rectangle.x = 0;
    destination_rectangle.y = 1;
    destination_rectangle.w = 360;
    destination_rectangle.h = 59;
    // cut sonarscreen and paste it back one line lower
    SDL_BlitSurface(sonarscreen, &source_rectangle, sonarscreen, &destination_rectangle);
    // Fill first line with background noise
    for (int nbp = 0; nbp < 360; nbp++) {
        bool sensordeaf = false;
        int array_heading;
        if (northcenter) {
            array_heading = (int) Subs->Heading;
        } else {
            array_heading = ReciprocalBearing((int)Subs->Heading);
        }
        int recipbearing;
        if (array_heading >= 180) {
            recipbearing = array_heading - 180;
        } else {
            recipbearing = array_heading + 180;
        }
        int bearing_to_target = nbp;
        if (recipbearing > bearing_to_target) bearing_to_target += 360;
        int relative_bearing = bearing_to_target - recipbearing;
        if (relative_bearing > 150 && relative_bearing < 210) sensordeaf = true;
        if (!sensordeaf) {
            // noisecolor = SDL_MapRGB(sonarscreen->format, 0, (int) fabs((flowandambientnoise - RandInt(40))), 0);
            my_noise = (int) fabs((flowandambientnoise - RandInt(40))) / 2;
            noisecolor = SDL_MapRGB(sonarscreen->format, my_noise, my_noise, my_noise);
            DrawPixel(sonarscreen, nbp, 0, noisecolor);
        } else {
            DrawPixel(sonarscreen, nbp, 0, black);
        }
    }

    // move the 0-30 min display down
    ++count;
    if (count > 29) {
        count = 0;
        source_rectangle.x = 0;  // sonar screen minus last line
        source_rectangle.y = 70;
        source_rectangle.w = 360;
        source_rectangle.h = 59;
        destination_rectangle.x = 0;
        destination_rectangle.y = 71;
        destination_rectangle.w = 360;
        destination_rectangle.h = 59;
        // cut sonarscreen and paste it to buffer
        SDL_BlitSurface(sonarscreen, &source_rectangle, sonarscreen, &destination_rectangle);
        for (int nbp = 0; nbp < 360; nbp++) {
            bool sensordeaf = false;
            int array_heading;
            if (northcenter) {
                array_heading = (int) Subs->Heading;
            } else {
                array_heading = ReciprocalBearing((int)Subs->Heading);
            }
            int recipbearing;
            if (array_heading >= 180) {
                recipbearing = array_heading - 180;
            } else {
                recipbearing = array_heading + 180;
            }
            int bearing_to_target = nbp;
            if (recipbearing > bearing_to_target) bearing_to_target += 360;
            int relative_bearing = bearing_to_target - recipbearing;
            if (relative_bearing > 150 && relative_bearing < 210) sensordeaf = true;
            if (!sensordeaf) {
                // noisecolor = SDL_MapRGB(sonarscreen->format, 0, (int) fabs((flowandambientnoise - RandInt(40))), 0);
                my_noise = (int) fabs((flowandambientnoise - RandInt(40))) / 2;
                noisecolor = SDL_MapRGB(sonarscreen->format, my_noise, my_noise, my_noise);
                DrawPixel(sonarscreen, nbp, 70, noisecolor);
            } else {
                DrawPixel(sonarscreen, nbp, 70, black);
            }
        }
    }
	
    // move the 0-2 hour display down
    ++count2;
    if (count2 > 119) {
        count2 = 0;
        source_rectangle.x = 0;  // sonar screen minus last line
        source_rectangle.y = 140;
        source_rectangle.w = 360;
        source_rectangle.h = 59;
        destination_rectangle.x = 0;
        destination_rectangle.y = 141;
        destination_rectangle.w = 360;
        destination_rectangle.h = 59;
        // cut sonarscreen and paste it to buffer
        SDL_BlitSurface(sonarscreen, &source_rectangle, sonarscreen, &destination_rectangle);
        for (int nbp = 0; nbp < 360; nbp++) {
            bool sensordeaf = false;
            int array_heading;
            if (northcenter) {
                array_heading = (int) Subs->Heading;
            } else {
                array_heading = ReciprocalBearing((int)Subs->Heading);
            }
            int recipbearing;
            if (array_heading >= 180) {
                recipbearing = array_heading - 180;
            } else {
                recipbearing = array_heading + 180;
            }
            int bearing_to_target = nbp;
            if (recipbearing > bearing_to_target) bearing_to_target += 360;
            int relative_bearing = bearing_to_target - recipbearing;
            if (relative_bearing > 150 && relative_bearing < 210) sensordeaf = true;
            if (!sensordeaf) {
                // noisecolor = SDL_MapRGB(sonarscreen->format, 0, (int) fabs((flowandambientnoise - RandInt(40))), 0);

                my_noise = (int) fabs((flowandambientnoise - RandInt(40))) / 2;
                noisecolor = SDL_MapRGB(sonarscreen->format, my_noise, my_noise, my_noise);
                DrawPixel(sonarscreen, nbp, 140, noisecolor);
            } else {
                DrawPixel(sonarscreen, nbp, 140, black);
            }
        }
    }
}


void AnBqq5::AdvanceTB16Screen()
/******************************************
    This routine advances the waterfall display
    on the TB16 towed array screen.
*******************************************/
{
    static int count = 0;
    static int count2 = 0;
    Uint32 noisecolor;
    int my_noise;

    // move the minute display down
    SDL_Rect source, destination;
    source.x = 0;  // sonar screen minus last line
    source.y = 0;
    source.w = 360;
    source.h = 59;

    destination.x = 0;
    destination.y = 1;
    destination.w = 360;
    destination.h = 59;

    // cut towedarrayscreen and paste it to buffer
    SDL_BlitSurface(towedarrayscreen, &source, towedarrayscreen, &destination);

    // erase portion of screen
    DrawLine(towedarrayscreen, 0, 0, 360, 0, black);
    if (TB16.GetLength() > 240) {
        for (int nbp = 0; nbp < 360; nbp++) {
            bool sensordeaf = false;
            int array_heading;
            if (northcenter) {
                array_heading = (int) TB16.ReturnHeading();
            } else {
                array_heading = ReciprocalBearing((int)TB16.ReturnHeading());
            }
            int bearing_to_target = nbp;
            if (array_heading > bearing_to_target) bearing_to_target += 360;
            int relative_bearing = bearing_to_target - array_heading;
            if (relative_bearing > 150 && relative_bearing < 210) sensordeaf = true;
            if (!sensordeaf) {
                // noisecolor = SDL_MapRGB(towedarrayscreen->format, 0, (int) fabs((flowandambientnoise - RandInt(40))), 0);
                my_noise = (int) fabs((flowandambientnoise - RandInt(40))) / 2;
                noisecolor = SDL_MapRGB(towedarrayscreen->format, my_noise, my_noise, my_noise);
                DrawPixel(towedarrayscreen, nbp, 0, noisecolor);
            } else {
                DrawPixel(towedarrayscreen, nbp, 0, black);
            }
        }
    }

    // move the 0-30 min display down
    ++count;
    if (count > 29) {
        count = 0;

        source.x = 0;  // sonar screen minus last line
        source.y = 70;
        source.w = 360;
        source.h = 59;

        destination.x = 0;
        destination.y = 71;
        destination.w = 360;
        destination.h = 59;

        // cut towedarrayscreen and paste it to buffer
        SDL_BlitSurface(towedarrayscreen, &source, towedarrayscreen, &destination);
        if (TB16.GetLength() > 240) {
            for (int nbp = 0; nbp < 360; nbp++) {
                bool sensordeaf = false;
                int array_heading;
                if (northcenter) {
                    array_heading = (int) TB16.ReturnHeading();
                } else {
                    array_heading = ReciprocalBearing((int)TB16.ReturnHeading());
                }
                int bearing_to_target = nbp;
                if (array_heading > bearing_to_target) bearing_to_target += 360;
                int relative_bearing = bearing_to_target - array_heading;
                if (relative_bearing > 150 && relative_bearing < 210) sensordeaf = true;
                if (!sensordeaf) {
                    // noisecolor = SDL_MapRGB(towedarrayscreen->format, 0, (int) fabs((flowandambientnoise - RandInt(40))), 0);
                    my_noise = (int) fabs((flowandambientnoise - RandInt(40))) / 2;
                    noisecolor = SDL_MapRGB(towedarrayscreen->format, my_noise, my_noise, my_noise);
                    DrawPixel(towedarrayscreen, nbp, 70, noisecolor);
                } else {
                    DrawPixel(towedarrayscreen, nbp, 70, black);
                }
            }
        }
    }

    // move the 0-2 hour display down
    ++count2;
    if (count2 > 119) {
        count2 = 0;

        source.x = 0;  // sonar screen minus last line
        source.y = 140;
        source.w = 360;
        source.h = 59;

        destination.x = 0;  // temp buffer
        destination.y = 141;
        destination.w = 360;
        destination.h = 59;

        // cut towedarrayscreen and paste it to buffer
        SDL_BlitSurface(towedarrayscreen, &source, towedarrayscreen, &destination);
        if (TB16.GetLength() > 240) {
            for (int nbp = 0; nbp < 360; nbp++) {
                bool sensordeaf = false;
                int array_heading;
                if (northcenter) {
                    array_heading = (int) TB16.ReturnHeading();
                } else {
                    array_heading = ReciprocalBearing((int)TB16.ReturnHeading());
                }
                int bearing_to_target = nbp;
                if (array_heading > bearing_to_target) bearing_to_target += 360;
                int relative_bearing = bearing_to_target - array_heading;
                if (relative_bearing > 150 && relative_bearing < 210) sensordeaf = true;
                if (!sensordeaf) {
                    // noisecolor = SDL_MapRGB(towedarrayscreen->format, 0, (int) fabs((flowandambientnoise - RandInt(40))), 0);
                    my_noise = (int) fabs((flowandambientnoise - RandInt(40))) / 2;
                    noisecolor = SDL_MapRGB(towedarrayscreen->format, my_noise, my_noise, my_noise);
                    DrawPixel(towedarrayscreen, nbp, 140, noisecolor);
                } else {
                    DrawPixel(towedarrayscreen, nbp, 140, black);
                }
            }
        }
    }
}

void AnBqq5::DisplayBearingScale(bool center)
{
    center ? ncscale.draw(screen, 50, 130) : scscale.draw(screen, 50, 130);
    center ? ncscale.draw(screen, 50, 430) : scscale.draw(screen, 50, 430);
}

void AnBqq5::UpperCRT_Spherical()
{
    uppersonarcrt = sonarscreen;
}

void AnBqq5::LowerCRT_Spherical()
{
    lowersonarcrt = sonarscreen;
}

void AnBqq5::UpperCRT_TowedArray()
{
    uppersonarcrt = towedarrayscreen;
}

void AnBqq5::LowerCRT_TowedArray()
{
    lowersonarcrt = towedarrayscreen;
}

void AnBqq5::ToggleArrayChoice()
{
    arraychoice5by6 = !arraychoice5by6;
}

void AnBqq5::ToggleTrueRel()
{
    bearingdisplay5by6 = !bearingdisplay5by6;
}

bool AnBqq5::GetAssignTrackerState()
{
    return assigntracker;
}

void AnBqq5::ToggleAssignTracker()
{
    assigntracker = !assigntracker;
}

bool AnBqq5::GetNorthCenterState()
{
    return northcenter;
}

void AnBqq5::ToggleNorthCenter()
{
    northcenter = !northcenter;
}

void AnBqq5::UpperCRT_Button()
{
    uppercrton.draw(screen, 661, 161, true);

    SDL_Delay(200);
    if (arraychoice5by6) {
        UpperCRT_Spherical();
    } else {
        UpperCRT_TowedArray();
    }
}

void AnBqq5::LowerCRT_Button()
{
    lowercrton.draw(screen, 661, 215, true);

    SDL_Delay(200);
    if (arraychoice5by6) {
        LowerCRT_Spherical();
    } else {
        LowerCRT_TowedArray();
    }
}

void AnBqq5::StopWinch()
{
    tb16winchon.draw(screen, 567, 590, true);
    TB16.Stop();
    SDL_Delay(200);
}

int AnBqq5::RandInt(int TO)  // Returns a random integer...TO is upper limit
{
    return (rand() % TO);
}

int AnBqq5::ReciprocalBearing(int bearing)
{
    // returns the 180 degree opposite of the bearing given to it
    int recipbearing;
    if (bearing >= 180) {
        recipbearing = bearing - 180;
    } else {
        recipbearing = bearing + 180;
    }
    return recipbearing;
}

void AnBqq5::DisplayCursor()
{
    // Draw Cursor
    if (!northcenter) {
        DrawLine(screen, 51 + cursorBearing, 140, 51 + cursorBearing, 147, green);
    } else {
        int recipCursorBearing = ReciprocalBearing(cursorBearing);
        DrawLine(screen, 51 + recipCursorBearing, 140, 51 + recipCursorBearing, 147, green);
    }
}

void AnBqq5::UpdateCursor()
{
    int direction, temp;
    int ship_type;
    float signal;
    if (Subs->GetCount() > 0) {
        // Are there any sound events at this time index to worry about?
        for (int event = 1; event <= Subs->GetCount(); event++) {
            // If so step through the events
            Subs->GetEvent(event, &direction, &signal, &ship_type);
            // Okay this used to go through each bearing one by one and
            // check to see if there was a sonar contact. But this is slow...
            // So we are now incrementing by 5 degrees to speed thing along
            // In doing so we now have to check relative_bearings because
            // the contact might get passed by if it's on a 0/359 deg boundary.
            temp = direction;
            if (cursorBearing > temp) temp += 360;
            // relative_bearing = temp - cursorBearing;
            // if (relative_bearing < 3 || relative_bearing >357) {
            // std::cout << "TargetId: " << TargetId << " - Bearing: " << direction << std::endl;
            /*
            static char text[120];
            sprintf(text, "TARGET-ID %3i - BEARING %3i", TargetId, direction);
            string textline = text;
            Message.post_message(textline);
            Message.display_message();
            */
            //}
        }
    }
    // If cursor located on undesignated contact, Halt on contact.
    // else
    cursorBearing += 5;
    if (cursorBearing > 360) cursorBearing -= 360;
    // Some debug crap
    /* 
    if (cursorBearing > 360) std::cout << "cursor over " << cursorBearing << std::endl;
    if (cursorBearing < 0) std::cout << "cursor under " << cursorBearing << std::endl;
	*/
}
