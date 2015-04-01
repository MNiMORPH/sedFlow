/*
 * RiverReachProperties.cpp
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

#include "RiverReachProperties.h"

#include <limits>

namespace SedFlow {

RiverReachProperties::RiverReachProperties(int cellID, RegularRiverReachProperties regularRiverReachProperties,	AdditionalRiverReachProperties additionalRiverPropertiesAndMethods, const StrataSorting* strataSorting, const GeometricalChannelBehaviour* geometricalChannelBehaviour, const SillProperties* sillProperties, OverallParameters* overallParameters):
	cellID(cellID),
	downstreamCellID((cellID+1)),
	upstreamMarginCell(false),
	downstreamMarginCell(false),
	regularRiverReachProperties(regularRiverReachProperties),
	additionalRiverReachProperties(additionalRiverPropertiesAndMethods),
	strataSorting( (*strataSorting).createStrataSortingPointerCopy() ),
	geometricalChannelBehaviour( (*geometricalChannelBehaviour).createGeometricalChannelBehaviourPointerCopy() ),
	sillProperties( (*sillProperties).createSillPropertiesPointerCopy() ),
	mapFromRealCellIDtoUserCellID(NULL),
	overallParameters(overallParameters)
{
	if( cellID == std::numeric_limits<int>::min() )
	{
		const char *const errorMessage = "One of the cellID's (large negative number) is not allowed.";
		throw(errorMessage);
	}
}

RiverReachProperties::RiverReachProperties(int cellID, int downstreamCellID, RegularRiverReachProperties regularRiverReachProperties, AdditionalRiverReachProperties additionalRiverPropertiesAndMethods, const StrataSorting* strataSorting, const GeometricalChannelBehaviour* geometricalChannelBehaviour, const SillProperties* sillProperties, OverallParameters* overallParameters):
	cellID(cellID),
	downstreamCellID(downstreamCellID),
	upstreamMarginCell(false),
	downstreamMarginCell(false),

	regularRiverReachProperties(regularRiverReachProperties),
	additionalRiverReachProperties(additionalRiverPropertiesAndMethods),
	strataSorting( (*strataSorting).createStrataSortingPointerCopy() ),
	geometricalChannelBehaviour( (*geometricalChannelBehaviour).createGeometricalChannelBehaviourPointerCopy() ),
	sillProperties( (*sillProperties).createSillPropertiesPointerCopy() ),
	mapFromRealCellIDtoUserCellID(NULL),
	overallParameters(overallParameters)
{
	if( cellID >= downstreamCellID )
	{
		const char *const firstErrorMessage = "Cell-IDs have to be ordered from upstream to downstream.";
		throw(firstErrorMessage);
	}
	if( cellID == std::numeric_limits<int>::min() )
	{
		const char *const secondErrorMessage = "One of the cellID's (large negative number) is not allowed.";
		throw(secondErrorMessage);
	}
}

RiverReachProperties::RiverReachProperties(const RiverReachProperties& toCopy):
	cellID(toCopy.cellID),
	downstreamCellID(toCopy.downstreamCellID),
	numberOfUpstreamCells(toCopy.numberOfUpstreamCells),
	upstreamCellIDs(toCopy.upstreamCellIDs),
	upstreamMarginCell(toCopy.upstreamMarginCell),
	downstreamMarginCell(toCopy.downstreamMarginCell),
	regularRiverReachProperties(toCopy.regularRiverReachProperties),
	additionalRiverReachProperties(toCopy.additionalRiverReachProperties),
	strataSorting( toCopy.strataSorting->createStrataSortingPointerCopy() ),
	geometricalChannelBehaviour( toCopy.geometricalChannelBehaviour->createGeometricalChannelBehaviourPointerCopy() ),
	sillProperties( toCopy.sillProperties->createSillPropertiesPointerCopy() ),
	mapFromRealCellIDtoUserCellID(NULL),
	overallParameters(toCopy.overallParameters)
{
	if(!(this->downstreamMarginCell))
		{ if( cellID >= downstreamCellID )
			{
			//TODO Remove these debugging lines
			////////////////////////////////////////////////////////
			/*
			std::cout << "After de-referencing within copy-constructor just before crashing:" << std::endl;
			std::cout << "toCopy.cellID: " << toCopy.cellID << ";" << std::endl;
			std::cout << "toCopy.downstreamCellID: " << toCopy.downstreamCellID << ";" << std::endl;
			std::cout << "toCopy.upstreamCellIDs.size(): " << toCopy.upstreamCellIDs.size() << ";" << std::endl;
			*/
			////////////////////////////////////////////////////////
				const char *const firstErrorMessage = "Cell-IDs have to be ordered from upstream to downstream.";
				throw(firstErrorMessage);
			}
		}

	if(!(this->upstreamMarginCell))
	{
		for(std::vector<int>::const_iterator currentUpstreamCellID = upstreamCellIDs.begin(); currentUpstreamCellID < upstreamCellIDs.end(); ++currentUpstreamCellID)
		{
			if( *currentUpstreamCellID >= cellID )
			{
				const char *const secondErrorMessage = "Cell-IDs have to be ordered from upstream to downstream.";
				throw(secondErrorMessage);
			}
		}
	}
}

RiverReachProperties::~RiverReachProperties()
{
	delete strataSorting;
	delete geometricalChannelBehaviour;
	delete sillProperties;
}

RiverReachProperties RiverReachProperties::createCopyAsUpstreamMarginCell(const RiverReachProperties& toCopy)
{
	RiverReachProperties result = RiverReachProperties(toCopy);
	result.upstreamMarginCell = true;
	result.downstreamMarginCell = false;
	return result;
}

RiverReachProperties RiverReachProperties::createCopyAsDownstreamMarginCell(const RiverReachProperties& toCopy)
{
	RiverReachProperties result = RiverReachProperties(toCopy);
	result.upstreamMarginCell = false;
	result.downstreamMarginCell = true;
	return result;
}

RiverReachProperties RiverReachProperties::createCopyAsNonMarginCell(const RiverReachProperties& toCopy)
{
	RiverReachProperties result = RiverReachProperties(toCopy);
	result.upstreamMarginCell = false;
	result.downstreamMarginCell = false;
	return result;
}

std::vector<RiverReachProperties> RiverReachProperties::createVectorCopy (const std::vector<RiverReachProperties>& toCopy)
{
	std::vector<RiverReachProperties> result;
	result.reserve( toCopy.size() );
	for(std::vector<RiverReachProperties>::const_iterator currentItem = toCopy.begin(); currentItem < toCopy.end(); ++currentItem)
		{ result.push_back( RiverReachProperties( *currentItem ) ); }
	return result;
}

ConstructionVariables RiverReachProperties::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RiverReachProperties;
	std::vector<int> intVector;
	std::vector<bool> boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	intVector.push_back( cellID );
	result.labelledInts["internalCellID"] = intVector;
	intVector.clear();
	intVector.push_back( downstreamCellID );
	result.labelledInts["internalDownstreamCellID"] = intVector;
	intVector.clear();
	intVector.push_back( numberOfUpstreamCells );
	result.labelledInts["numberOfUpstreamCells"] = intVector;
	intVector.clear();
	for(std::vector<int>::const_iterator currentUpstreamCellID = upstreamCellIDs.begin(); currentUpstreamCellID < upstreamCellIDs.end(); ++currentUpstreamCellID)
			{ intVector.push_back( *currentUpstreamCellID ); }
	result.labelledInts["internalUpstreamCellIDs"] = intVector;
	intVector.clear();
	boolVector.push_back( upstreamMarginCell );
	result.labelledBools["upstreamMarginCell"] = boolVector;
	boolVector.clear();
	boolVector.push_back( downstreamMarginCell );
	result.labelledBools["downstreamMarginCell"] = boolVector;
	boolVector.clear();
	constructionVariablesVector.push_back( regularRiverReachProperties.createConstructionVariables() );
	result.labelledObjects["regularRiverReachProperties"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( additionalRiverReachProperties.createConstructionVariables() );
	result.labelledObjects["additionalRiverReachProperties"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( strataSorting->createConstructionVariables() );
	result.labelledObjects["strataSorting"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( geometricalChannelBehaviour->createConstructionVariables() );
	result.labelledObjects["geometricalChannelBehaviour"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( sillProperties->createConstructionVariables() );
	result.labelledObjects["sillProperties"] = constructionVariablesVector;
	return result;
}

}
