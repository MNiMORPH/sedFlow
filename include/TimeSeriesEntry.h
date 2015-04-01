/*
 * TimeSeriesEntry.h
 *
 *   Copyright (C) 2014 Swiss Federal Research Institute WSL (http://www.wsl.ch)
 *   Developed by F.U.M. Heimann
 *   Published by the Swiss Federal Research Institute WSL
 *   
 *   This program is free software: you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License version 3
 *   as published by the Free Software Foundation.
 *   
 *   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *   without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *   See the GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see http://www.gnu.org/licenses
 *   
 *   This software is part of the model sedFlow,
 *   which is intended for the simulation of bedload dynamics in mountain streams.
 *   
 *   For details on sedFlow see http://www.wsl.ch/sedFlow
 */

#ifndef TIMESERIESENTRY_H_
#define TIMESERIESENTRY_H_

#include "Grains.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class TimeSeriesEntry {
public:
	TimeSeriesEntry(){}
	TimeSeriesEntry(double elapsedSeconds, double doubleValue);
	TimeSeriesEntry(double elapsedSeconds, Grains grainsValue);
	virtual ~TimeSeriesEntry();

	double elapsedSeconds;
	double doubleValue;
	Grains grainsValue;

	ConstructionVariables createConstructionVariables() const;

	friend bool operator < (const TimeSeriesEntry& timeSeriesEntry1, const TimeSeriesEntry& timeSeriesEntry2)
	{
		return (timeSeriesEntry1.elapsedSeconds < timeSeriesEntry2.elapsedSeconds);
	}

	friend bool operator == (const TimeSeriesEntry& timeSeriesEntry1, const TimeSeriesEntry& timeSeriesEntry2)
	{
		return (timeSeriesEntry1.elapsedSeconds == timeSeriesEntry2.elapsedSeconds);
	}

};

}

#endif /* TIMESERIESENTRY_H_ */
