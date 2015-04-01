/*
 * SimpleDownstreamTwoCellGradientWithCenteredValues.cpp
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

#include "SimpleDownstreamTwoCellGradientWithCenteredValues.h"

#include <limits>
#include <iostream>

namespace SedFlow {

SimpleDownstreamTwoCellGradientWithCenteredValues::SimpleDownstreamTwoCellGradientWithCenteredValues(CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest):
	propertyOfInterest(propertyOfInterest)
{
	this->typeOfGradientCalculationMethod = CombinerVariables::SimpleDownstreamTwoCellGradientWithCenteredValues;
	this->isDependentOnBedslope = false;
	this->isDependentOnWaterEnergyslope = false;
}


CalcGradient* SimpleDownstreamTwoCellGradientWithCenteredValues::createCalcGradientMethodPointerCopy() const
{
	CalcGradient* result = new SimpleDownstreamTwoCellGradientWithCenteredValues(this->propertyOfInterest);
	return result;
}

ConstructionVariables SimpleDownstreamTwoCellGradientWithCenteredValues::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::SimpleDownstreamTwoCellGradientWithCenteredValues);
	std::vector<std::string> stringVector;
	stringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(propertyOfInterest) );
	result.labelledStrings["propertyOfInterest"] = stringVector;
	return result;
}

double SimpleDownstreamTwoCellGradientWithCenteredValues::calculate (double localValue, double downstreamValue, double localLength, double downstreamLength) const
{
	return ( (localValue-downstreamValue) / (0.5 * (localLength + downstreamLength) ) );
}

double SimpleDownstreamTwoCellGradientWithCenteredValues::calculate (const RiverReachProperties& riverReachProperties) const
{
	if ( riverReachProperties.isDownstreamMargin() || ( ( propertyOfInterest == CombinerVariables::hydraulicHead || propertyOfInterest == CombinerVariables::waterLevel ) && (riverReachProperties.getDownstreamCellPointer())->isDownstreamMargin() ) ) { return riverReachProperties.regularRiverReachProperties.bedslope; }
	else
	{
		const RiverReachProperties& downstreamRiverReachProperties ( (*( riverReachProperties.getDownstreamCellPointer() )) );
		double downstreamValue = downstreamRiverReachProperties.regularRiverReachProperties.getDoubleProperty(propertyOfInterest);
		if( downstreamRiverReachProperties.regularRiverReachProperties.sillOccurence )
		{
			switch (propertyOfInterest)
			{
			case CombinerVariables::elevation:
				downstreamValue = std::max( downstreamValue, downstreamRiverReachProperties.regularRiverReachProperties.sillTopEdgeElevation );
				break;

			case CombinerVariables::hydraulicHead:
				downstreamValue = std::max( downstreamValue, downstreamRiverReachProperties.sillProperties->calculateFreeOverfallHydraulicHeadAtSill(downstreamRiverReachProperties.regularRiverReachProperties.discharge,downstreamRiverReachProperties.regularRiverReachProperties,downstreamRiverReachProperties.additionalRiverReachProperties));
				break;

			case CombinerVariables::waterLevel:
			case CombinerVariables::maximumWaterdepth:
			case CombinerVariables::flowVelocity:
				{ const char *const sillsErrorMessage = "SimpleDownstreamTwoCellGradientWithCenteredValues: Gradient calculation based on waterLevel, maximumWaterdepth or flowVelocity is not defined at sills yet.";
				throw(sillsErrorMessage); }
				break;

			default:
				break;
			}
		}

		return this->calculate( riverReachProperties.regularRiverReachProperties.getDoubleProperty(propertyOfInterest), downstreamValue, riverReachProperties.regularRiverReachProperties.length, (riverReachProperties.getDownstreamCellPointer())->regularRiverReachProperties.length);
	}

}


std::vector<double> SimpleDownstreamTwoCellGradientWithCenteredValues::calculate (const RiverSystemProperties& riverSystem) const
{
	std::vector<double> result;

	for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = riverSystem.regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < riverSystem.regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachProperties)
	{
		result.push_back( this->calculate(*currentRiverReachProperties) );
	}

	return result;

}

double SimpleDownstreamTwoCellGradientWithCenteredValues::calculate (const std::vector<double>& values, const std::vector<double>& lengths, int cellID) const
{
	if (values.size() != lengths.size())
	{
		const char *const errorMessage = "Not corresponding vector sizes";
		throw(errorMessage);
	}

	if (cellID == values.size())
	{
		return std::numeric_limits<double>::quiet_NaN();

	} else {
		// All other cells
		return this->calculate(values.at(cellID), values.at((cellID+1)), lengths.at(cellID), lengths.at((cellID+1)));
	}
}

std::vector<double> SimpleDownstreamTwoCellGradientWithCenteredValues::calculate (const std::vector<double>& values, const std::vector<double>& lengths) const
{
	if (values.size() != lengths.size())
	{
		const char *const errorMessage = "Not corresponding vector sizes";
		throw(errorMessage);
	}

	std::vector<double> result;

	for(int cellID = 0; cellID < values.size(); ++cellID)
	{
		result.push_back( this->calculate(values.at(cellID), values.at((cellID+1)), lengths.at(cellID), lengths.at((cellID+1))) );
	}

	return result;
}

}
