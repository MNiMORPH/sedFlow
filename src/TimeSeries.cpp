/*
 * TimeSeries.cpp
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

#include "TimeSeries.h"

#include <algorithm>

namespace SedFlow {

TimeSeries::~TimeSeries(){}

TimeSeries::TimeSeries(int userCellID, bool margin, CombinerVariables::TypesOfRegularRiverReachProperties property, const std::vector<TimeSeriesEntry>& timeSeries, const RegularRiverSystemProperties& regularRiverSystemProperties):
	userCellID(userCellID),
	margin(margin),
	property(property),
	actualTimeSeries(timeSeries),
	surplusInsteadOfAbsoluteValues(false),
	finalInsteadOfInitialAdding(false)
{
	std::sort(this->actualTimeSeries.begin(),this->actualTimeSeries.end());
	if(this->margin)
	{
		const RiverReachProperties* localCell = regularRiverSystemProperties.getReachPropertiesConstPointerCorrespondingToUserCellID(userCellID);
		const RiverReachProperties* upstreamCell = (localCell->getUpstreamCellPointers()).at(0);
		const RiverReachProperties* downstreamCell = localCell->getDownstreamCellPointer();
		if ( upstreamCell->isUpstreamMargin() )
		{
			this->cellID = upstreamCell->getCellID();
		}
		else
		{
			if ( downstreamCell->isDownstreamMargin() )
			{
				this->cellID = downstreamCell->getCellID();
			}
			else
			{
				const char *const errorMessage = "No margin could be found for given time series.";
				throw(errorMessage);
			}
		}

	}
	else
	{
		this->cellID = regularRiverSystemProperties.getRealCellIDcorrespondingToUserCellID(userCellID);
	}
}

TimeSeries::TimeSeries(int userCellID, bool margin, CombinerVariables::TypesOfRegularRiverReachProperties property, const std::vector<TimeSeriesEntry>& timeSeries, bool surplusInsteadOfAbsoluteValues, bool finalInsteadOfInitialAdding, const RegularRiverSystemProperties& regularRiverSystemProperties):
	userCellID(userCellID),
	margin(margin),
	property(property),
	actualTimeSeries(timeSeries),
	surplusInsteadOfAbsoluteValues(surplusInsteadOfAbsoluteValues),
	finalInsteadOfInitialAdding(finalInsteadOfInitialAdding)
{
	std::sort(this->actualTimeSeries.begin(),this->actualTimeSeries.end());
	if(this->margin)
	{
		const RiverReachProperties* localCell = regularRiverSystemProperties.getReachPropertiesConstPointerCorrespondingToUserCellID(userCellID);
		const RiverReachProperties* upstreamCell = (localCell->getUpstreamCellPointers()).at(0);
		const RiverReachProperties* downstreamCell = localCell->getDownstreamCellPointer();
		if ( upstreamCell->isUpstreamMargin() )
		{
			this->cellID = upstreamCell->getCellID();
		}
		else
		{
			if ( downstreamCell->isDownstreamMargin() )
			{
				this->cellID = downstreamCell->getCellID();
			}
			else
			{
				const char *const errorMessage = "No margin could be found for given time series.";
				throw(errorMessage);
			}
		}

	}
	else
	{
		this->cellID = regularRiverSystemProperties.getRealCellIDcorrespondingToUserCellID(userCellID);
	}
}

ConstructionVariables TimeSeries::createConstructionVariables() const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::TimeSeries;
	std::vector<int> intVector;
	intVector.push_back(this->userCellID);
	result.labelledInts["userCellID"] = intVector;
	std::vector<bool> boolVector;
	boolVector.push_back( this->margin );
	result.labelledBools["margin"] = boolVector;
	boolVector.clear();
	boolVector.push_back( this->surplusInsteadOfAbsoluteValues );
	result.labelledBools["surplusInsteadOfAbsoluteValues"] = boolVector;
	boolVector.clear();
	boolVector.push_back( this->finalInsteadOfInitialAdding );
	result.labelledBools["finalInsteadOfInitialAdding"] = boolVector;
	std::vector<std::string> stringVector;
	stringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(property) );
	result.labelledStrings["property"] = stringVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	for(std::vector<TimeSeriesEntry>::const_iterator iterator = this->actualTimeSeries.begin(); iterator < this->actualTimeSeries.end(); ++iterator)
			{ constructionVariablesVector.push_back( iterator->createConstructionVariables() ); }
	result.labelledObjects["timeSeries"] = constructionVariablesVector;
	return result;
}

void TimeSeries::setCellID(int cellID, const RegularRiverSystemProperties& regularRiverSystemProperties)
{
	const RiverReachProperties* localCell = &(regularRiverSystemProperties.cellProperties.at(cellID));
	if( localCell->isMargin() )
	{
		this->margin = true;

		if( localCell->isUpstreamMargin() )
		{
			this->cellID = cellID;
			this->userCellID = regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( (localCell->getDownstreamCellPointer())->getCellID() );

		}
		else
		{
			this->cellID = cellID;
			this->userCellID = regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( ((localCell->getUpstreamCellPointers()).at(0))->getCellID() );
		}

	}
	else
	{
		this->margin = false;
		this->cellID = cellID;
		this->userCellID = regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID(cellID);
	}
}

}
