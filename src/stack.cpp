/***************************************************************************
                          stack.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: stack.cpp,v 1.5 2003/04/14 05:51:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/

 #include "stack.h"
#include <iostream>

Stack::Stack(){
	counter=0;
}
Stack::~Stack(){
}

void Stack::RegisterEvent(int direction, float signal, int ship_type){

	++counter;
	stack[counter][0]=(float)direction;
	stack[counter][1]=signal;
        stack[counter][2]=(float)ship_type;

}

void Stack::GetEvent(int event, int &rdirection, float &rsignal, int &ship_type){

	rdirection=(int)stack[event][0];
	rsignal=stack[event][1];
        ship_type = (int) stack[event][2];

}

int Stack::GetCount(){
	return (counter);
}

void Stack::AdvanceSonarHistory(){
/*
	for (int second=29; second>0; second--){
		counter[second]=counter[second-1];
		for (int event=counter[second-1]; event>0; event--){
			stack[second][event][0]=stack[second-1][event][0];
			stack[second][event][1]=stack[second-1][event][1];
			stack[second][event][2]=stack[second-1][event][2];
		}
	}
*/
	counter=0;
	stack[0][0]=0;
	stack[0][1]=0;
	stack[0][2]=0;
}


