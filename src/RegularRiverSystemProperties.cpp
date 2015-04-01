/*
 * RegularRiverSystemProperties.cpp
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

#include "RegularRiverSystemProperties.h"

#include <algorithm>
#include <limits>
#include <map>
#include <numeric>

//Remove this debugging line
#include <iostream>

namespace SedFlow {

//TODO Check for only ONE UserDownstreamMargin!
RegularRiverSystemProperties::RegularRiverSystemProperties(const std::vector<RiverReachProperties>& cellProperties)
{
	std::vector<int> cellIDs;
	std::map<int,std::vector<RiverReachProperties>::const_iterator > mapFromCellIDsToCells;
	for(std::vector<RiverReachProperties>::const_iterator cellIterator = cellProperties.begin(); cellIterator < cellProperties.end(); ++cellIterator)
	{
		cellIDs.push_back( cellIterator->cellID );
		mapFromCellIDsToCells[ cellIterator->cellID ] = cellIterator;
	}
	std::vector<int> sortedCellIDs = cellIDs;
	std::sort(sortedCellIDs.begin(), sortedCellIDs.end());

	int previousUserCellID = std::numeric_limits<int>::min();
	std::map<int,std::vector<int> > mapFromUserCellIDToInternalUpstreamCellIDs;
	std::map<int,std::vector<int> >::iterator iteratorForMapFromUserCellIDToInternalUpstreamCellIDs;
	RiverReachProperties* pointerToCurrentLastCell;
	int runningCellID = 0;
	std::vector<int> emptyIntVector;
	std::vector<RiverReachProperties>::const_iterator currentConstCell;

	std::vector<int>::const_iterator cellIDIterator = sortedCellIDs.begin();
	while(cellIDIterator < sortedCellIDs.end())
	{
		if(*cellIDIterator == previousUserCellID)
		{
			const char *const firstErrorMessage = "River system construction with duplicate cell IDs.";
			throw(firstErrorMessage);
		}
		previousUserCellID = *cellIDIterator;

		currentConstCell = mapFromCellIDsToCells.find(*cellIDIterator)->second;

		iteratorForMapFromUserCellIDToInternalUpstreamCellIDs = mapFromUserCellIDToInternalUpstreamCellIDs.find(currentConstCell->cellID);
		if ( iteratorForMapFromUserCellIDToInternalUpstreamCellIDs == mapFromUserCellIDToInternalUpstreamCellIDs.end() )
		{
			//If there is no upstream cell, create an upstream margin

			// This leads to the confusion...
			///////////////////////////////////////////////////////////////////////
			this->cellProperties.push_back(*currentConstCell);
			pointerToCurrentLastCell = &(this->cellProperties.back());
			///////////////////////////////////////////////////////////////////////
			pointerToCurrentLastCell->downstreamCellID = currentConstCell->cellID;
			pointerToCurrentLastCell->numberOfUpstreamCells = 0;
			pointerToCurrentLastCell->upstreamCellIDs.clear();
			pointerToCurrentLastCell->cellID = runningCellID++;
			pointerToCurrentLastCell->upstreamMarginCell = true;

			//Update the temporary map
			mapFromUserCellIDToInternalUpstreamCellIDs[currentConstCell->cellID] = emptyIntVector;
			iteratorForMapFromUserCellIDToInternalUpstreamCellIDs = mapFromUserCellIDToInternalUpstreamCellIDs.find(currentConstCell->cellID);
			iteratorForMapFromUserCellIDToInternalUpstreamCellIDs->second.push_back(pointerToCurrentLastCell->cellID);
		}
		//Append current cell and convert some IDs into internal IDs
		//TODO !!! Find some workaround this problem
		//TODO Remove these debugging lines
		////////////////////////////////////////////////////////
		/*
		std::cout << "//////////////////////////////////////////////////////" << std::endl;
		std::cout << "currentCell->cellID: " << currentCell->cellID << ";" << std::endl;
		std::cout << "currentCell->downstreamCellID: " << (*currentCell).downstreamCellID << ";" << std::endl;
		std::cout << "currentCell->upstreamCellIDs.size(): " << currentCell->upstreamCellIDs.size() << ";" << std::endl;
		std::cout << "pointerToCurrentLastCell->cellID: " << pointerToCurrentLastCell->cellID << ";" << std::endl;
		std::cout << "pointerToCurrentLastCell->downstreamCellID: " << pointerToCurrentLastCell->downstreamCellID << ";" << std::endl;
		std::cout << "pointerToCurrentLastCell->upstreamCellIDs.size(): " << pointerToCurrentLastCell->upstreamCellIDs.size() << ";" << std::endl;
		std::cout << "--------------------------------------------------------" << std::endl;
		std::cout << "Now calling: this->cellProperties.push_back(RiverReachProperties(*currentCell));" << std::endl;
		*/
		////////////////////////////////////////////////////////

		this->cellProperties.push_back(*currentConstCell);

		//TODO Remove this debugging line
		//std::cout << "--------------------------------------------------------" << std::endl;

		pointerToCurrentLastCell = &(this->cellProperties.back());
		pointerToCurrentLastCell->upstreamCellIDs.clear();
		pointerToCurrentLastCell->upstreamCellIDs = iteratorForMapFromUserCellIDToInternalUpstreamCellIDs->second;
		pointerToCurrentLastCell->numberOfUpstreamCells = pointerToCurrentLastCell->upstreamCellIDs.size();
		mapFromRealCellIDtoUserCellID[runningCellID] = pointerToCurrentLastCell->cellID;
		pointerToCurrentLastCell->cellID = runningCellID++;

		//Update temporary map
		iteratorForMapFromUserCellIDToInternalUpstreamCellIDs = mapFromUserCellIDToInternalUpstreamCellIDs.find( pointerToCurrentLastCell->downstreamCellID );
		if (iteratorForMapFromUserCellIDToInternalUpstreamCellIDs == mapFromUserCellIDToInternalUpstreamCellIDs.end())
		{
			mapFromUserCellIDToInternalUpstreamCellIDs[pointerToCurrentLastCell->downstreamCellID] = emptyIntVector;
			iteratorForMapFromUserCellIDToInternalUpstreamCellIDs = mapFromUserCellIDToInternalUpstreamCellIDs.find( pointerToCurrentLastCell->downstreamCellID );
		}
		iteratorForMapFromUserCellIDToInternalUpstreamCellIDs->second.push_back( pointerToCurrentLastCell->cellID );

		++cellIDIterator;
	}

	//Append downstream margin
	this->cellProperties.push_back( RiverReachProperties(this->cellProperties.back()) );
	pointerToCurrentLastCell = &(this->cellProperties.back());
	pointerToCurrentLastCell->cellID = runningCellID;
	pointerToCurrentLastCell->downstreamMarginCell = true;
	pointerToCurrentLastCell->downstreamCellID = std::numeric_limits<int>::quiet_NaN();
	pointerToCurrentLastCell->upstreamCellIDs.clear();
	pointerToCurrentLastCell->upstreamCellIDs.push_back( (runningCellID-1) );
	pointerToCurrentLastCell->numberOfUpstreamCells = 1;




	//Run through all cells before the last user cell and update the cellIDs and pointers
	std::vector<RiverReachProperties>::iterator currentCell = this->cellProperties.begin();
	while(currentCell < (this->cellProperties.end()-2))
	{
		currentCell->upstreamCellPointers.clear();
		currentCell->upstreamCellPointers.reserve( currentCell->upstreamCellIDs.size() );
		for(std::vector<int>::const_iterator currentUpstreamCellID = currentCell->upstreamCellIDs.begin(); currentUpstreamCellID < currentCell->upstreamCellIDs.end(); ++currentUpstreamCellID)
				{ currentCell->upstreamCellPointers.push_back( &(this->cellProperties.at(*currentUpstreamCellID)) ); }

		currentCell->downstreamCellID = getRealCellIDcorrespondingToUserCellID( currentCell->downstreamCellID );
		currentCell->downstreamCellPointer = &(this->cellProperties.at(currentCell->downstreamCellID));

		++currentCell;
	}

	//Update cellIDs and pointers for the last user cell
	std::vector<RiverReachProperties>::iterator iteratorOfLastUserCell = this->cellProperties.end() - 2;
	iteratorOfLastUserCell->upstreamCellPointers.reserve( iteratorOfLastUserCell->upstreamCellIDs.size() );
	for(std::vector<int>::const_iterator currentUpstreamCellID = iteratorOfLastUserCell->upstreamCellIDs.begin(); currentUpstreamCellID < iteratorOfLastUserCell->upstreamCellIDs.end(); ++currentUpstreamCellID)
			{ iteratorOfLastUserCell->upstreamCellPointers.push_back( &(this->cellProperties.at(*currentUpstreamCellID)) ); }
	iteratorOfLastUserCell->downstreamCellID = runningCellID;
	iteratorOfLastUserCell->downstreamCellPointer = &(this->cellProperties.at(iteratorOfLastUserCell->downstreamCellID));

	//Update pointers for the downstream margin
	pointerToCurrentLastCell = &(this->cellProperties.back());
	pointerToCurrentLastCell->downstreamCellPointer = NULL;
	pointerToCurrentLastCell->upstreamCellPointers.clear();
	pointerToCurrentLastCell->upstreamCellPointers.push_back( &(this->cellProperties.at( (runningCellID-1) )) );

	//Check if everything went right
	this->numberOfCells = this->cellProperties.size();
	if ( this->numberOfCells != (runningCellID+1) )
	{
		const char *const secondErrorMessage = "Something went wrong in the construction of the river system properties.";
		throw(secondErrorMessage);
	}

	//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
	for(std::map<int,int>::iterator currentEntry = this->mapFromRealCellIDtoUserCellID.begin(); currentEntry != this->mapFromRealCellIDtoUserCellID.end(); ++currentEntry)
	{
		currentEntry->second = ( currentEntry->second - 1000 );
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Finally run through all cells and adjust the margin bedslopes
	for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties)
	{
		(*currentRiverReachProperties).mapFromRealCellIDtoUserCellID = &(this->mapFromRealCellIDtoUserCellID);

		if(currentRiverReachProperties->isMargin())
		{
			double slope;
			double localLength = currentRiverReachProperties->regularRiverReachProperties.length;
			if( currentRiverReachProperties->isUpstreamMargin() )
			{
				RiverReachProperties* downstreamCell = currentRiverReachProperties->getDownstreamCellPointer();
				double downstreamElevation = downstreamCell->regularRiverReachProperties.elevation;
				RiverReachProperties* furtherDownstreamCell = downstreamCell->getDownstreamCellPointer();
				double furtherDownstreamElevation = furtherDownstreamCell->regularRiverReachProperties.elevation;
				slope = (downstreamElevation - furtherDownstreamElevation) / localLength;
				currentRiverReachProperties->regularRiverReachProperties.elevation = downstreamElevation + ( slope * localLength );
			}

			if( currentRiverReachProperties->isDownstreamMargin() )
			{
				RiverReachProperties* upstreamCell = (currentRiverReachProperties->getUpstreamCellPointers()).at(0);
				double upstreamElevation = upstreamCell->regularRiverReachProperties.elevation;
				double upstreamTopEdge = upstreamElevation;
				if(upstreamCell->regularRiverReachProperties.sillOccurence) { upstreamTopEdge = std::max(upstreamTopEdge,upstreamCell->regularRiverReachProperties.sillTopEdgeElevation); }
				double currentFurtherUpstreamElevation, currentFurtherUpstreamLength, currentSlope;
				std::vector<double> slopes;
				std::vector<RiverReachProperties*> furtherUpstreamCells = upstreamCell->getUpstreamCellPointers();
				for(std::vector<RiverReachProperties*>::const_iterator currentFurtherUpstreamCell = furtherUpstreamCells.begin(); currentFurtherUpstreamCell < furtherUpstreamCells.end(); ++currentFurtherUpstreamCell)
				{
					currentFurtherUpstreamElevation = (*currentFurtherUpstreamCell)->regularRiverReachProperties.elevation;
					currentFurtherUpstreamLength = (*currentFurtherUpstreamCell)->regularRiverReachProperties.length;
					currentSlope = ( currentFurtherUpstreamElevation - upstreamTopEdge ) / currentFurtherUpstreamLength;
					slopes.push_back( currentSlope );
				}
				slope = std::accumulate(slopes.begin(), slopes.end(), 0.0) / slopes.size();
				currentRiverReachProperties->regularRiverReachProperties.elevation = upstreamElevation - ( slope * localLength );
				currentRiverReachProperties->regularRiverReachProperties.sillOccurence = false;
				currentRiverReachProperties->regularRiverReachProperties.sillTopEdgeElevation = -9999.0;
			}
		}
	}
}

RegularRiverSystemProperties::RegularRiverSystemProperties(const RegularRiverSystemProperties& toCopy):
		mapFromRealCellIDtoUserCellID(toCopy.mapFromRealCellIDtoUserCellID),
		numberOfCells(toCopy.numberOfCells),
		cellProperties(toCopy.cellProperties)
{
	for(std::vector<RiverReachProperties>::iterator currentCell = cellProperties.begin(); currentCell < cellProperties.end(); ++currentCell)
	{
		(*currentCell).mapFromRealCellIDtoUserCellID = &(this->mapFromRealCellIDtoUserCellID);

		currentCell->upstreamCellPointers.clear();
		if( !(currentCell->isUpstreamMargin()) )
		{
			for(std::vector<int>::const_iterator currentUpstreamCellID = currentCell->upstreamCellIDs.begin(); currentUpstreamCellID < currentCell->upstreamCellIDs.end(); ++currentUpstreamCellID)
			{
				currentCell->upstreamCellPointers.push_back( &( cellProperties.at( *currentUpstreamCellID ) ) );
			}
		}

		currentCell->downstreamCellPointer = NULL;
		if( !(currentCell->isDownstreamMargin()) )
		{
			currentCell->downstreamCellPointer = &( cellProperties.at( currentCell->downstreamCellID ) );
		}
	}
}

ConstructionVariables RegularRiverSystemProperties::createConstructionVariables()const
{
	std::map<std::string, std::vector<int> >::iterator labelledIntsIterator;
	std::map<std::string, std::vector<double> >::iterator labelledDoublesIterator;
	std::vector<int> intVector;
	int tmpDownstreamCellID;
	ConstructionVariables* currentConstructionVariablesVectorEnd;
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RegularRiverSystemProperties;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( cellProperties.size() );
	int tmpInt;
	for(std::vector<RiverReachProperties>::const_iterator iterator = cellProperties.begin(); iterator < cellProperties.end(); ++iterator)
	{
		if( !( (*iterator).isMargin() ) )
		{
			constructionVariablesVector.push_back( (*iterator).createConstructionVariables() );
			currentConstructionVariablesVectorEnd = &(constructionVariablesVector.back());
			labelledIntsIterator = currentConstructionVariablesVectorEnd->labelledInts.find("internalCellID");
			tmpInt = (getUserCellIDcorrespondingToRealCellID((labelledIntsIterator->second).at(0)));
			//TODO Remove this bugfix addition by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
			tmpInt += 1000;
			intVector.push_back( tmpInt );
			currentConstructionVariablesVectorEnd->labelledInts["cellID"] = intVector;
			intVector.clear();
			labelledIntsIterator = currentConstructionVariablesVectorEnd->labelledInts.find("internalDownstreamCellID");
			tmpDownstreamCellID = getUserCellIDcorrespondingToRealCellID((labelledIntsIterator->second).at(0));
			if ( tmpDownstreamCellID == std::numeric_limits<int>::min() ) { intVector.push_back( std::numeric_limits<int>::max() ); }
			else
			{
				//TODO Remove this bugfix addition by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
				tmpDownstreamCellID += 1000;
				intVector.push_back( tmpDownstreamCellID );
			}
			currentConstructionVariablesVectorEnd->labelledInts["downstreamCellID"] = intVector;
			intVector.clear();
			labelledIntsIterator = currentConstructionVariablesVectorEnd->labelledInts.find("internalUpstreamCellIDs");
			intVector.reserve( labelledIntsIterator->second.size() );
			for(std::vector<int>::const_iterator iterator = labelledIntsIterator->second.begin(); iterator < labelledIntsIterator->second.end(); ++iterator)
			{
				tmpInt = getUserCellIDcorrespondingToRealCellID(*iterator);
				//TODO Remove this bugfix addition by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
				if ( tmpInt != std::numeric_limits<int>::min() ) { tmpInt += 1000; }
				intVector.push_back( tmpInt );
			}
			currentConstructionVariablesVectorEnd->labelledInts["upstreamCellIDs"] = intVector;
			intVector.clear();
		}
	}
	result.labelledObjects["cellProperties"] = constructionVariablesVector;
	return result;
}

void RegularRiverSystemProperties::setRegularCellParameters (const RegularRiverReachProperties& newParameters, int cellID)
{
	(this->cellProperties.at(cellID)).regularRiverReachProperties = newParameters;

}

std::vector<double> RegularRiverSystemProperties::extractSingleRegularParameter (CombinerVariables::TypesOfRegularRiverReachProperties parameter) const
{
	std::vector<double> result;
	for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties)
	{
		result.push_back( (*currentRiverReachProperties).regularRiverReachProperties.getDoubleProperty(parameter) );
	}
	return result;
}

void RegularRiverSystemProperties::setSingleRegularParameter (CombinerVariables::TypesOfRegularRiverReachProperties parameter, const std::vector<double>& newValues)
{
	if(newValues.size() != this->numberOfCells)
	{
		const char *const firstErrorMessage = "Not matching number of values and cells.";
		throw(firstErrorMessage);
	}
	std::vector<double>::const_iterator currentNewValue = newValues.begin();
	switch (parameter)
	{
	case CombinerVariables::elevation:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.elevation = *currentNewValue;
		}
		break;

	case CombinerVariables::length:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.length = *currentNewValue;
		}
		break;

	case CombinerVariables::discharge:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.discharge = *currentNewValue;
		}
		break;

	case CombinerVariables::flowVelocity:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.flowVelocity = *currentNewValue;
		}
		break;

	case CombinerVariables::maximumWaterdepth:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.maximumWaterdepth = *currentNewValue;
		}
		break;

	case CombinerVariables::bedShearStress:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.bedShearStress = *currentNewValue;
		}
		break;

	case CombinerVariables::activeWidth:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.activeWidth = *currentNewValue;
		}
		break;

	case CombinerVariables::bedslope:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.bedslope = *currentNewValue;
		}
		break;

	case CombinerVariables::waterEnergyslope:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.waterEnergyslope = *currentNewValue;
		}
		break;

	case CombinerVariables::sedimentEnergyslope:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.sedimentEnergyslope = *currentNewValue;
		}
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.unreducedSedimentEnergyslope = *currentNewValue;
		}
		break;

	case CombinerVariables::waterVolumeChangeRate:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.waterVolumeChangeRate = *currentNewValue;
		}
		break;

	case CombinerVariables::waterVolumeChange:
		for(std::vector<RiverReachProperties>::iterator currentRiverReachProperties = this->cellProperties.begin(); currentRiverReachProperties < this->cellProperties.end(); ++currentRiverReachProperties, ++currentNewValue)
		{
			(*currentRiverReachProperties).regularRiverReachProperties.waterVolumeChange = *currentNewValue;
		}
		break;

	default:
		const char *const defaultErrorMessage = "Parameter update has not been defined for this Regular River Parameter";
		throw (defaultErrorMessage);
	}
}

RiverReachProperties* RegularRiverSystemProperties::getReachPropertiesPointerCorrespondingToUserCellID (int userCellID)
{
	int realCellID = getRealCellIDcorrespondingToUserCellID(userCellID);
	return &(cellProperties.at(realCellID));
}

const RiverReachProperties* RegularRiverSystemProperties::getReachPropertiesConstPointerCorrespondingToUserCellID (int userCellID) const
{
	int realCellID = getRealCellIDcorrespondingToUserCellID(userCellID);
	const RiverReachProperties* result = &(cellProperties.at(realCellID));
	return result;
}


}
