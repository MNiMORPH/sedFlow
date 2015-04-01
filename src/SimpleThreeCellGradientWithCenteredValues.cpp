/*
 * SimpleThreeCellGradientWithCenteredValues.cpp
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

#include <iostream>
#include <cstdlib>
#include <numeric>
#include <limits>

#include "SimpleThreeCellGradientWithCenteredValues.h"

namespace SedFlow {

SimpleThreeCellGradientWithCenteredValues::SimpleThreeCellGradientWithCenteredValues(CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest, CombinerVariables::TypesOfRegularRiverReachProperties weightingProperty):
	propertyOfInterest(propertyOfInterest),
	weightingProperty(weightingProperty)
{
	this->typeOfGradientCalculationMethod = CombinerVariables::SimpleThreeCellGradientWithCenteredValues;
	this->isDependentOnBedslope = false;
	this->isDependentOnWaterEnergyslope = false;
}

CalcGradient* SimpleThreeCellGradientWithCenteredValues::createCalcGradientMethodPointerCopy() const
{
	CalcGradient* result = new SimpleThreeCellGradientWithCenteredValues(this->propertyOfInterest, this->weightingProperty);
	return result;
}

ConstructionVariables SimpleThreeCellGradientWithCenteredValues::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::SimpleThreeCellGradientWithCenteredValues);
	std::vector<std::string> stringVector;
	stringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(propertyOfInterest) );
	result.labelledStrings["propertyOfInterest"] = stringVector;
	stringVector.clear();
	stringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(weightingProperty) );
	result.labelledStrings["weightingProperty"] = stringVector;
	return result;
}

double SimpleThreeCellGradientWithCenteredValues::calculate (double upstreamValue, double downstreamValue, double upstreamLength, double localLength, double downstreamLength) const
{
	return ( (upstreamValue-downstreamValue) / (0.5 * upstreamLength + localLength + 0.5 * downstreamLength) );
}

double SimpleThreeCellGradientWithCenteredValues::calculate (const RiverReachProperties& riverReachProperties) const
{
	if(weightingProperty == CombinerVariables::strataPerUnitBedSurface)
	{
		const char *const errorMessage = "SimpleThreeCellGradientWithCenteredValues is not defined for strataPerUnitBedSurface as weightingProperty.";
		throw(errorMessage);
	}

	if ( riverReachProperties.isMargin() || ( ( propertyOfInterest == CombinerVariables::hydraulicHead || propertyOfInterest == CombinerVariables::waterLevel ) && (riverReachProperties.getDownstreamCellPointer())->isDownstreamMargin() ) ) { return riverReachProperties.regularRiverReachProperties.bedslope; }
	else
	{
		const RiverReachProperties& downstreamRiverReachProperties ( (*( riverReachProperties.getDownstreamCellPointer() )) );
		bool downstreamSillOccurence = downstreamRiverReachProperties.regularRiverReachProperties.sillOccurence;

		double downstreamValue = downstreamRiverReachProperties.regularRiverReachProperties.getDoubleProperty(propertyOfInterest);
		if( downstreamSillOccurence )
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
				{ const char *const sillsErrorMessage = "SimpleThreeCellGradientWithCenteredValues: Gradient calculation based on waterLevel, maximumWaterdepth or flowVelocity is not defined at sills yet.";
				throw(sillsErrorMessage); }
				break;

			default:
				break;
			}
		}

		if (riverReachProperties.getNumberOfUpstreamCells() > 1)
		{
			std::vector<double> gradients;
			std::vector<double> upstreamWeightingValues;
			std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
			if ( CombinerVariables::regularRiverReachPropertyIsGrains(weightingProperty) )
			{
				for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
				{
					gradients.push_back( this->calculate( (*(*currentUpstreamCellPointer)).regularRiverReachProperties.getDoubleProperty(propertyOfInterest), downstreamValue, (*(*currentUpstreamCellPointer)).regularRiverReachProperties.length, riverReachProperties.regularRiverReachProperties.length, downstreamRiverReachProperties.regularRiverReachProperties.length) );
					upstreamWeightingValues.push_back( ((*(*currentUpstreamCellPointer)).regularRiverReachProperties.getGrainsProperty(weightingProperty)).getOverallVolume() );
				}
			}
			else
			{
				for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
				{
					gradients.push_back( this->calculate( (*(*currentUpstreamCellPointer)).regularRiverReachProperties.getDoubleProperty(propertyOfInterest), downstreamValue, (*(*currentUpstreamCellPointer)).regularRiverReachProperties.length, riverReachProperties.regularRiverReachProperties.length, downstreamRiverReachProperties.regularRiverReachProperties.length) );
					upstreamWeightingValues.push_back( (*(*currentUpstreamCellPointer)).regularRiverReachProperties.getDoubleProperty(weightingProperty) );
				}
			}
			double weightedMeanGradient = 0.0;
			double weightingValuesSum = accumulate(upstreamWeightingValues.begin(), upstreamWeightingValues.end(), 0.0);
			for(int i = 0; i < gradients.size(); ++i)
			{
				weightedMeanGradient += ( gradients.at(i) * upstreamWeightingValues.at(i) );
			}
			weightedMeanGradient /= weightingValuesSum;
			return weightedMeanGradient;
		}
		else
		{
			double upstreamValue = (( riverReachProperties.getUpstreamCellPointers()).at(0) )->regularRiverReachProperties.getDoubleProperty(propertyOfInterest);
			double upstreamLength = (( riverReachProperties.getUpstreamCellPointers()).at(0) )->regularRiverReachProperties.length;
			double localLength = riverReachProperties.regularRiverReachProperties.length;

			return this->calculate( upstreamValue, downstreamValue, upstreamLength, localLength, downstreamRiverReachProperties.regularRiverReachProperties.length);
		}

	}

}


std::vector<double> SimpleThreeCellGradientWithCenteredValues::calculate (const RiverSystemProperties& riverSystem) const
{
	std::vector<double> result;

	for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = riverSystem.regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < riverSystem.regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachProperties)
	{
		result.push_back( this->calculate(*currentRiverReachProperties) );
	}

	return result;

}

double SimpleThreeCellGradientWithCenteredValues::calculate (const std::vector<double>& values, const std::vector<double>& lengths, int cellID) const
{
	if (values.size() != lengths.size())
	{
		const char *const errorMessage = "Not corresponding vector sizes";
		throw(errorMessage);
	}

	if (cellID == 0)
	{
		return std::numeric_limits<double>::quiet_NaN();

	} else {
		if (cellID == values.size())
		{
			return std::numeric_limits<double>::quiet_NaN();

		} else {
			// All other cells
			return this->calculate(values.at((cellID-1)), values.at((cellID+1)), lengths.at((cellID-1)), lengths.at(cellID), lengths.at((cellID+1)));
		}
	}
}

std::vector<double> SimpleThreeCellGradientWithCenteredValues::calculate (const std::vector<double>& values, const std::vector<double>& lengths) const
{
	if (values.size() != lengths.size())
	{
		const char *const errorMessage = "Not corresponding vector sizes";
		throw(errorMessage);
	}

	std::vector<double> result;

	for(int cellID = 0; cellID < values.size(); ++cellID)
	{
		result.push_back( this->calculate(values.at((cellID-1)), values.at((cellID+1)), lengths.at((cellID-1)), lengths.at(cellID), lengths.at((cellID+1))) );
	}

	return result;
}

}
