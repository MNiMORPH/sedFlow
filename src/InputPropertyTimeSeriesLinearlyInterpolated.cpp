/*
 * InputPropertyTimeSeriesLinearlyInterpolated.cpp
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

#include "InputPropertyTimeSeriesLinearlyInterpolated.h"

#include <algorithm>
#include <map>

namespace SedFlow {

InputPropertyTimeSeriesLinearlyInterpolated::InputPropertyTimeSeriesLinearlyInterpolated(CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod, std::vector<TimeSeries> timeSeriesInputs, bool ensureModificationAgainstOtherChangeRateModifiers, const OverallMethods& overallMethods):
		ChangeRateModifiersType(CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated, correspondingGeneralFlowMethod),
		ensureModificationAgainstOtherChangeRateModifiers(ensureModificationAgainstOtherChangeRateModifiers),
		overallMethods(overallMethods)
{
	std::map<int,std::vector<TimeSeries> >::iterator iteratorForTimeSeriesInputsByCellID;
	std::vector<TimeSeries> emptyVector;
	for(std::vector<TimeSeries>::iterator currentTimeSeries = timeSeriesInputs.begin(); currentTimeSeries < timeSeriesInputs.end(); ++currentTimeSeries)
	{
		iteratorForTimeSeriesInputsByCellID = this->timeSeriesInputsByCellID.find(currentTimeSeries->getCellID());
		if (iteratorForTimeSeriesInputsByCellID == this->timeSeriesInputsByCellID.end())
		{
			this->timeSeriesInputsByCellID[ currentTimeSeries->getCellID() ] = emptyVector;
			iteratorForTimeSeriesInputsByCellID = this->timeSeriesInputsByCellID.find(currentTimeSeries->getCellID());
		}
		iteratorForTimeSeriesInputsByCellID->second.push_back( *currentTimeSeries );
	}
}

InputPropertyTimeSeriesLinearlyInterpolated::~InputPropertyTimeSeriesLinearlyInterpolated(){}

ChangeRateModifiersType* InputPropertyTimeSeriesLinearlyInterpolated::createChangeRateModifiersTypePointerCopy() const
{
	std::vector<TimeSeries> timeSeriesInputs = this->createSingleTimeSeriesVectorCopy();
	ChangeRateModifiersType* result = new InputPropertyTimeSeriesLinearlyInterpolated(this->correspondingGeneralFlowMethod,timeSeriesInputs,this->ensureModificationAgainstOtherChangeRateModifiers,this->overallMethods);
	return result;
}

ConstructionVariables InputPropertyTimeSeriesLinearlyInterpolated::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
	result.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated);
	std::vector<std::string> stringVector;
	stringVector.push_back( CombinerVariables::typeOfGeneralFlowMethodsToString(correspondingGeneralFlowMethod) );
	result.labelledStrings["correspondingGeneralFlowMethod"] = stringVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	std::vector<TimeSeries> timeSeriesInputs = this->createSingleTimeSeriesVectorCopy();
	constructionVariablesVector.reserve( timeSeriesInputs.size() );
	for(std::vector<TimeSeries>::const_iterator iterator = timeSeriesInputs.begin(); iterator < timeSeriesInputs.end(); ++iterator)
			{ constructionVariablesVector.push_back( iterator->createConstructionVariables() ); }
	result.labelledObjects["timeSeriesInputs"] = constructionVariablesVector;
	std::vector<bool> boolVector;
	boolVector.push_back(ensureModificationAgainstOtherChangeRateModifiers);
	result.labelledBools["ensureModificationAgainstOtherChangeRateModifiers"] = boolVector;
	return result;
}

void InputPropertyTimeSeriesLinearlyInterpolated::inputModification(RiverReachProperties& riverReachProperties)
{
	currentModificationPhase = InputModification;
	applyModification(riverReachProperties);
}

void InputPropertyTimeSeriesLinearlyInterpolated::modificationBeforeUpdates(RiverReachProperties& riverReachProperties)
{
	currentModificationPhase = ModificationBeforeUpdates;
	if(this->ensureModificationAgainstOtherChangeRateModifiers)
	{
		applyModification(riverReachProperties);
	}
}

void InputPropertyTimeSeriesLinearlyInterpolated::finalModification(RiverReachProperties& riverReachProperties)
{
	currentModificationPhase = FinalModification;
	if(this->ensureModificationAgainstOtherChangeRateModifiers)
	{
		applyModification(riverReachProperties);
	}
}

void InputPropertyTimeSeriesLinearlyInterpolated::applyModification(RiverReachProperties& riverReachProperties)
{
	std::map< std::vector<TimeSeries>*, std::vector<TimeSeries>::iterator > toErase;

	std::map<int,std::vector<TimeSeries> >::iterator timeSeriesInputsIterator = this->timeSeriesInputsByCellID.find(riverReachProperties.getCellID());
	if(timeSeriesInputsIterator != this->timeSeriesInputsByCellID.end()) //Continue only if the current Cell is treated
	{
		double reallyElapsedSeconds = (riverReachProperties.getOverallParameters())->getElapsedSeconds();
		std::vector<TimeSeriesEntry>::const_iterator firstTimeSeriesEntry, secondTimeSeriesEntry;
		CombinerVariables::TypesOfRegularRiverReachProperties currentProperty;
		double doubleUpdate;
		Grains grainsUpdate;
		//Run through all time series for the current Cell
		for(std::vector<TimeSeries>::iterator currentTimeSeries = timeSeriesInputsIterator->second.begin(); currentTimeSeries < timeSeriesInputsIterator->second.end(); ++currentTimeSeries)
		{
			if ( !( currentTimeSeries->surplusInsteadOfAbsoluteValues && ( currentModificationPhase==ModificationBeforeUpdates || (currentModificationPhase==InputModification && currentTimeSeries->finalInsteadOfInitialAdding) || (currentModificationPhase==FinalModification && (!(currentTimeSeries->finalInsteadOfInitialAdding))) ) ) )
			//Addition shall only be performed in inputModification!!
			{
				if( currentTimeSeries->actualTimeSeries.size() < 2 ) { toErase[ &(timeSeriesInputsIterator->second) ] = currentTimeSeries; }
				else // Continue only if time series is potentially usable and delete unusable time series
				{
					secondTimeSeriesEntry = currentTimeSeries->actualTimeSeries.begin() + 1;
					// Remove the historic past from the time series
					while( secondTimeSeriesEntry != currentTimeSeries->actualTimeSeries.end() && secondTimeSeriesEntry->elapsedSeconds <= reallyElapsedSeconds)
					{
						currentTimeSeries->actualTimeSeries.erase(currentTimeSeries->actualTimeSeries.begin());
						secondTimeSeriesEntry = currentTimeSeries->actualTimeSeries.begin() + 1;
					}
					if (secondTimeSeriesEntry == currentTimeSeries->actualTimeSeries.end()) { toErase[ &(timeSeriesInputsIterator->second) ] = currentTimeSeries; }
					else // Continue only if time series is still potentially usable and delete unusable time series
					{
						firstTimeSeriesEntry = currentTimeSeries->actualTimeSeries.begin();
						// Continue only if we have already entered the range of the time series
						if ( firstTimeSeriesEntry->elapsedSeconds <= reallyElapsedSeconds )
						{
							currentProperty = currentTimeSeries->property;
							//If everything went right apply the interpolated time series
							if(CombinerVariables::regularRiverReachPropertyIsGrains(currentProperty))
							{
								grainsUpdate = Grains::interpolateLinearly(firstTimeSeriesEntry->grainsValue, secondTimeSeriesEntry->grainsValue, ((reallyElapsedSeconds - firstTimeSeriesEntry->elapsedSeconds) / (secondTimeSeriesEntry->elapsedSeconds - firstTimeSeriesEntry->elapsedSeconds)) );
								if(currentTimeSeries->surplusInsteadOfAbsoluteValues) { grainsUpdate += riverReachProperties.regularRiverReachProperties.getGrainsProperty(currentProperty); }
								riverReachProperties.regularRiverReachProperties.setGrainsProperty(currentProperty, grainsUpdate);
							}
							else
							{
								doubleUpdate = firstTimeSeriesEntry->doubleValue + ( (secondTimeSeriesEntry->doubleValue - firstTimeSeriesEntry->doubleValue) * (reallyElapsedSeconds - firstTimeSeriesEntry->elapsedSeconds) / (secondTimeSeriesEntry->elapsedSeconds - firstTimeSeriesEntry->elapsedSeconds) );
								if(currentTimeSeries->surplusInsteadOfAbsoluteValues) { doubleUpdate += riverReachProperties.regularRiverReachProperties.getDoubleProperty(currentProperty); }
								riverReachProperties.regularRiverReachProperties.setDoubleProperty(currentProperty, doubleUpdate);
							}
							//After application of interpolated time series update depending parameters.
							switch (currentProperty)
							{
							case CombinerVariables::maximumWaterdepth:
								overallMethods.flowResistance->calculateAndUpdateDischargeAndFlowVelocityUsingFlowDepthAsInput(riverReachProperties);
								break;

							case CombinerVariables::discharge:
								overallMethods.flowResistance->calculateAndUpdateFlowDepthAndFlowVelocityUsingDischargeAsInput(riverReachProperties);
								break;

							case CombinerVariables::flowVelocity:
								overallMethods.flowResistance->calculateAndUpdateDischargeAndFlowDepthUsingFlowVelocityAsInput(riverReachProperties);
								break;

							//TODO Less important: Think of any other depending parameters to be updated.

							//default:
								//In any other case there are no depending parameters to update.
							}

						}
					}
				}
			}
		}
	}

	for(std::map< std::vector<TimeSeries>*, std::vector<TimeSeries>::iterator >::iterator currentEraseEntry = toErase.begin(); currentEraseEntry != toErase.end(); ++currentEraseEntry)
	{
		currentEraseEntry->first->erase(currentEraseEntry->second);
	}
}

std::vector<TimeSeries> InputPropertyTimeSeriesLinearlyInterpolated::createSingleTimeSeriesVectorCopy()const
{
	std::vector<TimeSeries> result;
	for( std::map< int,std::vector<TimeSeries> >::const_iterator currentTimeSeriesVector = this->timeSeriesInputsByCellID.begin(); currentTimeSeriesVector != this->timeSeriesInputsByCellID.end(); ++currentTimeSeriesVector)
	{
		for(std::vector<TimeSeries>::const_iterator currentTimeSeries = currentTimeSeriesVector->second.begin(); currentTimeSeries < currentTimeSeriesVector->second.end(); ++currentTimeSeries)
				{ result.push_back( *currentTimeSeries ); }
	}
	return result;
}

}
