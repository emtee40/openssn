/***************************************************************************
                          stack.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: stack.h,v 1.4 2003/04/14 05:51:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STACK_H
#define STACK_H

/**Holds sound information for the SONAR
  *@author Michael Bridak
  */

class Stack {
public:
	/** how many events for this second*/
	int counter;
	/** storage space */
	float stack[20][2];
	/** Advance sound history, drop old events */
	void AdvanceSonarHistory();
	/** Place a sound event on in the Queue */
	void RegisterEvent(int direction, float signalstrength);
	/** Get how many events are in the Queue for a particular second */
	int  GetCount();
	/** Get sound information */
	void GetEvent(int event, int &Direction, float &SignalStrength);
	Stack();
	~Stack();
};

#endif
