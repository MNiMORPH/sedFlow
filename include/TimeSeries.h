/*
 * TimeSeries.h
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

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <vector>

#include "TimeSeriesEntry.h"
#include "CombinerVariables.h"
#include "RegularRiverSystemProperties.h"

namespace SedFlow {

class TimeSeries {
private:
	int cellID;
	int userCellID;
	bool margin;

public:
	TimeSeries(){}
	TimeSeries(int userCellID, bool margin, CombinerVariables::TypesOfRegularRiverReachProperties property, const std::vector<TimeSeriesEntry>& timeSeries, const RegularRiverSystemProperties& regularRiverSystemProperties);
	TimeSeries(int userCellID, bool margin, CombinerVariables::TypesOfRegularRiverReachProperties property, const std::vector<TimeSeriesEntry>& timeSeries, bool surplusInsteadOfAbsoluteValues, bool finalInsteadOfInitialAdding, const RegularRiverSystemProperties& regularRiverSystemProperties);
	virtual ~TimeSeries();

	CombinerVariables::TypesOfRegularRiverReachProperties property;
	std::vector<TimeSeriesEntry> actualTimeSeries;
	bool surplusInsteadOfAbsoluteValues;
	bool finalInsteadOfInitialAdding; //Needed when a change rate itself and not a contributing property is modified.

	ConstructionVariables createConstructionVariables() const;

	inline int getCellID() const { return this->cellID; }
	void setCellID(int cellID, const RegularRiverSystemProperties& regularRiverSystemProperties);

	friend bool operator < (const TimeSeries& timeSeries1, const TimeSeries& timeSeries2)
	{
		return (timeSeries1.cellID < timeSeries2.cellID);
	}

};

}

#endif /* TIMESERIES_H_ */
