/*
 * InstantaneousSedimentInputs.cpp
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

#include "InstantaneousSedimentInputs.h"

#include "SedimentFlowTypeMethods.h"

namespace SedFlow {

InstantaneousSedimentInputs::InstantaneousSedimentInputs(const std::vector<double>& elapsedSeconds, const std::vector<Grains>& sedimentInputs)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::InstantaneousSedimentInputs;
	if(elapsedSeconds.size() != sedimentInputs.size())
	{
		const char *const sizeMismatchErrorMessage = "For InstantaneousSedimentInputs the elapsedSeconds and the sedimentInputs have to contain the same number of elements.";
		throw(sizeMismatchErrorMessage);
	}
	std::vector<double>::const_iterator currentSecondsEntry = elapsedSeconds.begin();
	std::vector<Grains>::const_iterator currentInputsEntry = sedimentInputs.begin();
	for(; currentInputsEntry < sedimentInputs.end(); ++currentInputsEntry, ++currentSecondsEntry)
	{
		this->mapFromElapsedSecondsToSedimentInputs[*currentSecondsEntry] = *currentInputsEntry;
	}
}

InstantaneousSedimentInputs::InstantaneousSedimentInputs(const std::map<double,Grains>& mapFromElapsedSecondsToSedimentInputs):
	mapFromElapsedSecondsToSedimentInputs(mapFromElapsedSecondsToSedimentInputs)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::InstantaneousSedimentInputs;
}

InstantaneousSedimentInputs::~InstantaneousSedimentInputs() {}

ConstructionVariables InstantaneousSedimentInputs::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
	result.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::InstantaneousSedimentInputs);
	std::vector<double> doubleVectorForElapsedSeconds;
	std::vector<ConstructionVariables> constructionVariablesVectorForSedimentInputs;
	for(std::map<double,Grains>::const_iterator currentEntry = this->mapFromElapsedSecondsToSedimentInputs.begin(); currentEntry != this->mapFromElapsedSecondsToSedimentInputs.end(); ++currentEntry)
	{
		doubleVectorForElapsedSeconds.push_back(currentEntry->first);
		constructionVariablesVectorForSedimentInputs.push_back((currentEntry->second).createConstructionVariables());
	}
	result.labelledDoubles["elapsedSeconds"] = doubleVectorForElapsedSeconds;
	result.labelledObjects["sedimentInputs"] = constructionVariablesVectorForSedimentInputs;
	return result;
}

AdditionalRiverReachMethodType* InstantaneousSedimentInputs::createAdditionalRiverReachMethodTypeCopy() const
{
	AdditionalRiverReachMethodType* result = new InstantaneousSedimentInputs(this->mapFromElapsedSecondsToSedimentInputs);
	return result;
}

void InstantaneousSedimentInputs::typeSpecificAction (RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods)
{
	std::map<double,Grains>::iterator currentEntry = this->mapFromElapsedSecondsToSedimentInputs.begin();
	if(currentEntry != this->mapFromElapsedSecondsToSedimentInputs.end())
	{
		double currentlyElapsedSeconds = (riverReachProperties.getOverallParameters())->getElapsedSeconds();
		double currentTimeStepLength = (riverReachProperties.getOverallParameters())->getCurrentTimeStepLengthInSeconds();
		if( currentEntry->first <= (currentlyElapsedSeconds + currentTimeStepLength) )
		{
			if(currentEntry->first >= currentlyElapsedSeconds)
			{
				riverReachProperties.regularRiverReachProperties.deposition += currentEntry->second;
				riverReachProperties.regularRiverReachProperties.depositionRate += currentEntry->second / currentTimeStepLength;
				// Scale down overall deposition down to unit active width
				riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface = (*(riverReachProperties.geometricalChannelBehaviour)).convertActiveWidthAndOverallSedimentVolumeIncrementIntoSedimentVolumeIncrementPerUnitBedSurface( riverReachProperties.regularRiverReachProperties.activeWidth, riverReachProperties.regularRiverReachProperties.deposition );
				// Scale down deposition to unit bed surface
				riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface /= riverReachProperties.regularRiverReachProperties.length;
				//Actually, now there is more sediment available for erosion. However, it would be too complex to consider this. (Don't try to do it!! Any attempt will cause trouble. Believe me.)
			}
			this->mapFromElapsedSecondsToSedimentInputs.erase(currentEntry);
			this->typeSpecificAction(riverReachProperties,regularRiverReachMethods);
		}
	}
}

}
