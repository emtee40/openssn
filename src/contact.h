/***************************************************************************
                          contact.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: contact.h,v 1.4 2003/05/20 17:36:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License.

 ***************************************************************************/

#ifndef CONTACT_H
#define CONTACT_H

#include "coord.h"

/** This is our picture of what is outside of our Sub.
  * @author Michael Bridak
  */

class Contact : public Coord
{
public:
    Contact();
    ~Contact();
    void UpdateContact();

private:
    // Flag to see if ship is in the world
    bool Active;
    // Sub = 0, Surface = 1, Air = 2
    int ShipType;
    // Foe = 0, Friend = 1, Unknown = 2, Neutral = 3
    int Friend;
};

#endif  // CONTACT_H
