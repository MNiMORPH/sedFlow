/*
 * ParallelShiftOfBasicGeometry.cpp
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

#include "ParallelShiftOfBasicGeometry.h"

namespace SedFlow {

ParallelShiftOfBasicGeometry::ParallelShiftOfBasicGeometry(const ChannelGeometry* alluviumChannel, bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate):
		GeometricalChannelBehaviour(alluviumChannel),
		upstreamOfSillsWedgeShapedInsteadOfParallelUpdate(upstreamOfSillsWedgeShapedInsteadOfParallelUpdate)
{
	this->typeOfGeometricalChannelBehaviour = CombinerVariables::ParallelShiftOfBasicGeometry;
	this->doesNOTchangeTypeOfChannelGeometry = true;
}

ParallelShiftOfBasicGeometry::ParallelShiftOfBasicGeometry(const ChannelGeometry* alluviumChannel, bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate, std::vector<double> additionalParameters):
	GeometricalChannelBehaviour(alluviumChannel),
	upstreamOfSillsWedgeShapedInsteadOfParallelUpdate(upstreamOfSillsWedgeShapedInsteadOfParallelUpdate)
{
	this->typeOfGeometricalChannelBehaviour = CombinerVariables::ParallelShiftOfBasicGeometry;
	this->doesNOTchangeTypeOfChannelGeometry = true;
}

GeometricalChannelBehaviour* ParallelShiftOfBasicGeometry::createGeometricalChannelBehaviourPointerCopy() const
{
	GeometricalChannelBehaviour* result = new ParallelShiftOfBasicGeometry(this->alluviumChannel,this->upstreamOfSillsWedgeShapedInsteadOfParallelUpdate);
	return result;
}

ConstructionVariables ParallelShiftOfBasicGeometry::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::GeometricalChannelBehaviour;
	result.realisationType = CombinerVariables::typeOfGeometricalChannelBehaviourToString(CombinerVariables::ParallelShiftOfBasicGeometry);
	std::vector<bool> boolVector;
	boolVector.push_back(upstreamOfSillsWedgeShapedInsteadOfParallelUpdate);
	result.labelledBools["upstreamOfSillsWedgeShapedInsteadOfParallelUpdate"] = boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( alluviumChannel->createConstructionVariables() );
	result.labelledObjects["alluviumChannel"] = constructionVariablesVector;
	return result;
}

double ParallelShiftOfBasicGeometry::convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementIncludingAlluviumChannelUpdate (double activeWidth, double reachLength, double sedimentVolumeIncrement)
{
	return convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel(activeWidth,reachLength,sedimentVolumeIncrement);
}

double ParallelShiftOfBasicGeometry::convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel (double activeWidth, double reachLength, double sedimentVolumeIncrement) const
{
	switch ( this->alluviumChannel->getTypeOfChannelGeometry() )
	{
	case CombinerVariables::InfinitelyDeepRectangularChannel:
		break;
	case CombinerVariables::InfinitelyDeepVShapedChannel:
		break;
	default:
		//TODO Less Important: Think about the why.
		const char *const firstErrorMessage = "The GeometricalChannelBehaviour 'ParallelShiftOfBasicGeometry' can only be used in combination with the ChannelGeometry 'InfinitelyDeepRectangularChannel' or 'InfinitelyDeepRectangularChannel'.";
		throw(firstErrorMessage);
	}

	double activePerimeter = this->alluviumChannel->convertActiveWidthIntoActivePerimeter(activeWidth);

	double result;
	if (activePerimeter > 0.0)
	{
			result = (sedimentVolumeIncrement / (activePerimeter*reachLength) );
			if(upstreamOfSillsWedgeShapedInsteadOfParallelUpdate)
			{
				result *= 2.0;
			}
	}
	else
	{
		if (activePerimeter == 0.0 )
			{ result = 0.0; }
		else
		{
			const char *const secondErrorMessage = "Negative active perimeter.";
			throw(secondErrorMessage);
		}
	}
	return result;
}

std::pair<bool,double> ParallelShiftOfBasicGeometry::getLinearConversionFactorFromVolumeToHeight(double activeWidth, double reachLength) const
{
	switch ( this->alluviumChannel->getTypeOfChannelGeometry() )
	{
	case CombinerVariables::InfinitelyDeepRectangularChannel:
		break;
	case CombinerVariables::InfinitelyDeepVShapedChannel:
		break;
	default:
		//TODO Less Important: Think about the why.
		const char *const firstErrorMessage = "The GeometricalChannelBehaviour 'ParallelShiftOfBasicGeometry' can only be used in combination with the ChannelGeometry 'InfinitelyDeepRectangularChannel' or 'InfinitelyDeepRectangularChannel'.";
		throw(firstErrorMessage);
	}

	double activePerimeter = this->alluviumChannel->convertActiveWidthIntoActivePerimeter(activeWidth);

	double conversionFactor;
	std::pair<bool,double> result;
	if (activePerimeter > 0.0)
	{
		conversionFactor = 1.0 / (activePerimeter*reachLength);
		if(upstreamOfSillsWedgeShapedInsteadOfParallelUpdate)
		{
			conversionFactor *= 2.0;
		}
		result = std::make_pair(true,conversionFactor);
	}
	else
	{
		if( activePerimeter == 0.0 )
		{
			result = std::make_pair(true,0.0);
		}
		else
		{
			const char *const secondErrorMessage = "Negative active perimeter.";
			throw(secondErrorMessage);
		}
	}
	return result;
}

std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > ParallelShiftOfBasicGeometry::getInternalParameters() const
{
	std::vector< std::vector<double> > empty;
	return std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > (CombinerVariables::ParallelShiftOfBasicGeometry, empty);
}

void ParallelShiftOfBasicGeometry::setInternalParameters( std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > newParameters )
{
	if (newParameters.first != CombinerVariables::ParallelShiftOfBasicGeometry)
	{
		const char *const errorMessage = "Action on not matching type of geometrical channel behaviour";
		throw(errorMessage);
	}
}


}
