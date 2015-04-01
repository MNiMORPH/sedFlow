/*
 * AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.cpp
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

#include "AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.h"

#include <limits>

namespace SedFlow {

AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(double minimumBedSlope, double maximumBedSlope, double minimumWaterSurfaceSlope, double maximumWaterSurfaceSlope, bool ensureMinimumSlopesAgainstOtherChangeRateModifiers, CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod):
	ChangeRateModifiersType(CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins, correspondingGeneralFlowMethod),
	minimumBedSlope(minimumBedSlope),
	maximumBedSlope(maximumBedSlope),
	minimumWaterSurfaceSlope(minimumWaterSurfaceSlope),
	maximumWaterSurfaceSlope(maximumWaterSurfaceSlope),
	ensureModificationAgainstOtherChangeRateModifiers(ensureMinimumSlopesAgainstOtherChangeRateModifiers),
	currentlyElapsedSeconds(std::numeric_limits<double>::quiet_NaN()),
	newTimeStep(true)
{}

AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(double minimumBedSlope, double maximumBedSlope, double minimumWaterSurfaceSlope, double maximumWaterSurfaceSlope, bool ensureMinimumSlopesAgainstOtherChangeRateModifiers, CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod, double currentlyElapsedSeconds, bool newTimeStep):
	ChangeRateModifiersType(CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins, correspondingGeneralFlowMethod),
	minimumBedSlope(minimumBedSlope),
	maximumBedSlope(maximumBedSlope),
	minimumWaterSurfaceSlope(minimumWaterSurfaceSlope),
	maximumWaterSurfaceSlope(maximumWaterSurfaceSlope),
	ensureModificationAgainstOtherChangeRateModifiers(ensureMinimumSlopesAgainstOtherChangeRateModifiers),
	currentlyElapsedSeconds(currentlyElapsedSeconds),
	newTimeStep(newTimeStep)
{}

ChangeRateModifiersType* AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins::createChangeRateModifiersTypePointerCopy() const
{
	ChangeRateModifiersType* result = new AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(this->minimumBedSlope, this->maximumBedSlope, this->minimumWaterSurfaceSlope, this->maximumWaterSurfaceSlope, this->ensureModificationAgainstOtherChangeRateModifiers, this->correspondingGeneralFlowMethod, this->currentlyElapsedSeconds, this->newTimeStep);
	return result;
}

ConstructionVariables AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
	result.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins);
	std::vector<double> doubleVector;
	doubleVector.push_back(minimumBedSlope);
	result.labelledDoubles["minimumBedSlope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(maximumBedSlope);
	result.labelledDoubles["maximumBedSlope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(minimumWaterSurfaceSlope);
	result.labelledDoubles["minimumWaterSurfaceSlope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(maximumWaterSurfaceSlope);
	result.labelledDoubles["maximumWaterSurfaceSlope"] = doubleVector;
	doubleVector.clear();
	std::vector<bool> boolVector;
	boolVector.push_back(ensureModificationAgainstOtherChangeRateModifiers);
	result.labelledBools["ensureModificationAgainstOtherChangeRateModifiers"] = boolVector;
	boolVector.clear();
	std::vector<std::string> stringVector;
	stringVector.push_back( CombinerVariables::typeOfGeneralFlowMethodsToString(correspondingGeneralFlowMethod) );
	result.labelledStrings["correspondingGeneralFlowMethod"] = stringVector;
	stringVector.clear();
	return result;
}

void AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins::modificationBeforeUpdates(RiverReachProperties& riverReachProperties)
{
	double systemElapsedSeconds = (riverReachProperties.getOverallParameters())->getElapsedSeconds();
	newTimeStep = (currentlyElapsedSeconds != systemElapsedSeconds);
	if (newTimeStep) { currentlyElapsedSeconds = systemElapsedSeconds; }
	this->adjustSlopes(riverReachProperties);
}

void AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins::adjustSlopes(RiverReachProperties& riverReachProperties)
{
	if(riverReachProperties.isMargin())
	{
		double localElevation = riverReachProperties.regularRiverReachProperties.elevation;
		double localWaterLevel = localElevation + riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
		double distance;
		if(riverReachProperties.isUpstreamMargin())
		{
			RiverReachProperties* downstreamCellPointer = riverReachProperties.getDownstreamCellPointer();
			double downstreamElevation = downstreamCellPointer->regularRiverReachProperties.elevation;
			double downstreamWaterLevel = downstreamElevation + downstreamCellPointer->regularRiverReachProperties.maximumWaterdepth;
			distance = 0.5 * ( riverReachProperties.regularRiverReachProperties.length + downstreamCellPointer->regularRiverReachProperties.length );

			if ( ( (localElevation - downstreamElevation) / distance ) < minimumBedSlope )
			{
				riverReachProperties.regularRiverReachProperties.elevation = (minimumBedSlope * distance) + downstreamElevation;

				localElevation = riverReachProperties.regularRiverReachProperties.elevation;
				localWaterLevel = localElevation + riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
			}

			if ( !(minimumBedSlope > maximumBedSlope) && ( (localElevation - downstreamElevation) / distance ) > maximumBedSlope )
			{
				riverReachProperties.regularRiverReachProperties.elevation = (maximumBedSlope * distance) + downstreamElevation;

				localElevation = riverReachProperties.regularRiverReachProperties.elevation;
				localWaterLevel = localElevation + riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
			}

			if ( ( (localWaterLevel - downstreamWaterLevel) / distance ) < minimumWaterSurfaceSlope )
			{
				riverReachProperties.regularRiverReachProperties.maximumWaterdepth = ( (minimumWaterSurfaceSlope * distance) + downstreamWaterLevel ) - localElevation;
				localWaterLevel = localElevation + riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
			}

			if ( !(minimumWaterSurfaceSlope > maximumWaterSurfaceSlope) && ( (localWaterLevel - downstreamWaterLevel) / distance ) > maximumWaterSurfaceSlope )
			{
				riverReachProperties.regularRiverReachProperties.maximumWaterdepth = ( (minimumWaterSurfaceSlope * distance) + downstreamWaterLevel ) - localElevation;
			}
		}

		if(riverReachProperties.isDownstreamMargin())
		{
			RiverReachProperties* upstreamCellPointer = (riverReachProperties.getUpstreamCellPointers()).at(0);
			double upstreamElevation = upstreamCellPointer->regularRiverReachProperties.elevation;
			double upstreamWaterLevel = upstreamElevation + upstreamCellPointer->regularRiverReachProperties.maximumWaterdepth;
			distance = 0.5 * ( riverReachProperties.regularRiverReachProperties.length + upstreamCellPointer->regularRiverReachProperties.length );

			if ( ( (upstreamElevation - localElevation) / distance ) < minimumBedSlope )
			{
				riverReachProperties.regularRiverReachProperties.elevation = upstreamElevation - (minimumBedSlope * distance);

				localElevation = riverReachProperties.regularRiverReachProperties.elevation;
				localWaterLevel = localElevation + riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
			}

			if ( !(minimumBedSlope > maximumBedSlope) && ( (upstreamElevation - localElevation) / distance ) > maximumBedSlope )
			{
				riverReachProperties.regularRiverReachProperties.elevation = upstreamElevation - (maximumBedSlope * distance);

				localElevation = riverReachProperties.regularRiverReachProperties.elevation;
				localWaterLevel = localElevation + riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
			}

			if ( ( (upstreamWaterLevel - localWaterLevel) / distance ) < minimumWaterSurfaceSlope )
			{
				localWaterLevel = upstreamWaterLevel - (minimumWaterSurfaceSlope * distance);
				riverReachProperties.regularRiverReachProperties.maximumWaterdepth = localWaterLevel - localElevation;
			}

			if ( !(minimumWaterSurfaceSlope > maximumWaterSurfaceSlope) && ( (upstreamWaterLevel - localWaterLevel) / distance ) > maximumWaterSurfaceSlope )
			{
				localWaterLevel = upstreamWaterLevel - (maximumWaterSurfaceSlope * distance);
				riverReachProperties.regularRiverReachProperties.maximumWaterdepth = localWaterLevel - localElevation;
			}

		}
	}
}


}
