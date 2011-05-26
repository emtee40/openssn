/***************************************************************************
                          towedarray.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                :michael.bridak@verizon.net
$Id: towedarray.h,v 1.3 2003/04/14 05:52:40 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TOWEDARRAY_H
#define TOWEDARRAY_H

#include "stack.h"

/**
  *@author Michael Bridak
  */

class TowedArray : public Stack  {
public:
	TowedArray();
	TowedArray(double, double);
	~TowedArray();
	void RecordPos(double lat, double lon);
	void Extend();
	void Stop();
	void ReelIn();
	int GetLength();
	void OperateWinch();
	void ReturnLatLon(double &latitude, double &longitude);
	float ReturnHeading();
	float BearingAmbiguity(float);
	float BearingToTarget(double target_Latitude, double target_longitude);
	float CalcBearing(double observer_latitude, double observer_longitude ,double target_latitude,double target_longitude);
	double RangeToTarget(double target_latitude, double target_longitude);
	double latitude_array[100], longitude_array[100];
	float length;
	int winch;
};

#endif
