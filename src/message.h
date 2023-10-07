/***************************************************************************
                          message.h  -  description
                             -------------------
    begin                : Monday Jan 13 2003
    copyright            : (C) 2003 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: message.h,v 1.3 2003/04/20 16:44:30 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef MESSAGE_H
#define MESSAGE_H

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "dfont.h"

/**
  * @author Michael Bridak
  */

class msg
{
public:
    /** Constructor... */
    msg();

    /** Destructor... */
    ~msg();

    void InitGraphics();

    /** Places a one line message into the message buffer to be later
        displayed by the display_message() method. */
    void post_message(const char *message);

    /** Displays the tail end of the buffer onto the main screen. */
    void display_message();

private:
    /** Our main storage for buffer text. */
    char buffer[20][256];

    SDL_Surface *screen;
    SDL_Surface *messagewindow;
    Uint32  black;
};

#endif  // MESSAGE_H
