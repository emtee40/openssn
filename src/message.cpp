/***************************************************************************
                          msg.cpp  -  description
                             -------------------
    begin                : Monday Jan 13 2003
    copyright            : (C) 2003 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: message.cpp,v 1.7 2003/09/21 21:52:40 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#include <string>
#include "files.h"
#include "message.h"

msg::msg()
{
    // clear out garbage from buffer.
    for (int x = 0; x < 20; x++) {
        buffer[x][0] = '\0';
    }
}

msg::~msg()
{
}

void msg::InitGraphics()
{
    screen = SDL_GetVideoSurface();
    SDL_Surface *temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 800, 72, 32,
                        screen->format->Rmask,
                        screen->format->Gmask,
                        screen->format->Bmask,
                        screen->format->Amask);
    messagewindow = SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);
    black = SDL_MapRGB(screen->format, 0, 0, 0);
}

void msg::post_message(const char *message)
{
    for (int x = 1; x < 20; x++) {
        strcpy(buffer[x - 1], buffer[x]);
    }
    strcpy(buffer[19], message);
}

void msg::display_message()
{
    static DFont font("images/font.png", "data/font.dat");
    // Clear out window contents
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.h = 72;
    rectangle.w = 800;
    SDL_FillRect(messagewindow, &rectangle, black);

    // print bottom 4 lines of buffer[]
    for (int x = 16; x < 20; x++) {
        font.PutString(messagewindow, 5, (15 * (x - 15)) - 6, buffer[x]);
    }

    // paste surface into screen
    rectangle.x = 175;
    rectangle.y = 15;
    rectangle.w = 800;
    rectangle.h = 72;
    SDL_BlitSurface(messagewindow, NULL, screen, &rectangle);
    SDL_UpdateRects(screen, 1, &rectangle);
}
