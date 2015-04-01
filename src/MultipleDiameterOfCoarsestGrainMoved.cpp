/*
 * MultipleDiameterOfCoarsestGrainMoved.cpp
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

#include "MultipleDiameterOfCoarsestGrainMoved.h"

namespace SedFlow {

MultipleDiameterOfCoarsestGrainMoved::MultipleDiameterOfCoarsestGrainMoved(double factor, double minimumTransportRateForFractionToBeConsideredMoving):
	factor(factor),
	minimumTransportRateForFractionToBeConsideredMoving(minimumTransportRateForFractionToBeConsideredMoving)
{}

MultipleDiameterOfCoarsestGrainMoved::~MultipleDiameterOfCoarsestGrainMoved(){}

EstimateThicknessOfMovingSedimentLayer* MultipleDiameterOfCoarsestGrainMoved::createEstimateThicknessOfMovingSedimentLayerPointerCopy() const
{
	EstimateThicknessOfMovingSedimentLayer* result = new MultipleDiameterOfCoarsestGrainMoved(this->factor, this->minimumTransportRateForFractionToBeConsideredMoving);
	return result;
}

ConstructionVariables MultipleDiameterOfCoarsestGrainMoved::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::EstimateThicknessOfMovingSedimentLayer;
	result.realisationType = CombinerVariables::typeOfEstimationMethodForThicknessOfMovingSedimentLayerToString(CombinerVariables::MultipleDiameterOfCoarsestGrainMoved);
	std::vector<double> doubleVector;
	doubleVector.push_back(this->factor);
	result.labelledDoubles["factor"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(this->minimumTransportRateForFractionToBeConsideredMoving);
	result.labelledDoubles["minimumTransportRateForFractionToBeConsideredMoving"] = doubleVector;
	return result;
}

double MultipleDiameterOfCoarsestGrainMoved::estimate (const RiverReachProperties& riverReachProperties) const
{
	const std::vector<double> fractionalGrainDiameters = (riverReachProperties.getOverallParameters())->getFractionalGrainDiameters();
	const std::vector<double> fractionalTransportRates = riverReachProperties.regularRiverReachProperties.erosionRate.getOverallFractionalAbundance();
	for(std::vector<double>::const_reverse_iterator currentDiameterBackward = fractionalGrainDiameters.rbegin(), currentTransportRateBackward = fractionalTransportRates.rbegin(); currentTransportRateBackward < fractionalTransportRates.rend(); ++currentDiameterBackward, ++currentTransportRateBackward)
	{
		if( (*currentTransportRateBackward) >= this->minimumTransportRateForFractionToBeConsideredMoving )
		{
			return (*currentDiameterBackward * factor);
		}
	}
	return (fractionalGrainDiameters.front() * factor);
}

}
