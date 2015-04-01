/*
 * TwoLayerWithContinuousUpdate.cpp
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

#include "TwoLayerWithContinuousUpdate.h"

namespace SedFlow {

TwoLayerWithContinuousUpdate::TwoLayerWithContinuousUpdate(double layerThickness, bool dynamicLayerThickness, double layerThicknessFactor, double referenceGrainSizePercentile):
	StrataSorting(layerThickness),
	dynamicLayerThickness(dynamicLayerThickness),
	layerThicknessFactor(layerThicknessFactor),
	referenceGrainSizePercentile(referenceGrainSizePercentile)
{
	if ( this->layerThickness <= 0 )
	{
		const char *const layerThicknessErrorMessage = "For TwoLayerWithContinuousUpdate a positive, non-zero layerThickness is needed.";
		throw(layerThicknessErrorMessage);
	}

	if ( this->layerThicknessFactor <= 0 )
	{
		const char *const layerThicknessFactorErrorMessage = "For TwoLayerWithContinuousUpdate a positive, non-zero layerThicknessFactor is needed.";
		throw(layerThicknessFactorErrorMessage);
	}
}

TwoLayerWithContinuousUpdate::~TwoLayerWithContinuousUpdate(){}

StrataSorting* TwoLayerWithContinuousUpdate::createStrataSortingPointerCopy() const
{
	StrataSorting* result = new TwoLayerWithContinuousUpdate(this->layerThickness, this->dynamicLayerThickness, this->layerThicknessFactor, this->referenceGrainSizePercentile);
	return result;
}

ConstructionVariables TwoLayerWithContinuousUpdate::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::StrataSorting;
	result.realisationType = CombinerVariables::typeOfStrataSortingToString(CombinerVariables::TwoLayerWithContinuousUpdate);
	std::vector<double> doubleVector;
	doubleVector.push_back(layerThickness);
	result.labelledDoubles["layerThickness"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(dynamicLayerThickness);
	result.labelledBools["dynamicLayerThickness"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(layerThicknessFactor);
	result.labelledDoubles["layerThicknessFactor"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(referenceGrainSizePercentile);
	result.labelledDoubles["referenceGrainSizePercentile"] = doubleVector;
	return result;
}

void TwoLayerWithContinuousUpdate::updateLayerThickness (const RegularRiverReachProperties& regularRiverReachProperties)
{
	if( dynamicLayerThickness && ((regularRiverReachProperties.strataPerUnitBedSurface.front().getOverallVolume()) > 0.0) )
	{
		layerThickness = layerThicknessFactor * regularRiverReachProperties.strataPerUnitBedSurface.front().getPercentileGrainDiameter((regularRiverReachProperties.getOverallParameters())->getFractionalGrainDiameters(), referenceGrainSizePercentile);
	}
}

void TwoLayerWithContinuousUpdate::sortMaterialUpward (std::vector<Grains>& strata, double activeThickness, double sublayerThickness, const RegularRiverReachProperties& regularRiverReachProperties, int cellID)
{
	Grains updateGrains = (layerThickness - activeThickness) * strata.back() / sublayerThickness;
	updateGrains = (strata.back()).subtract(updateGrains);
	(strata.front()).add(updateGrains);
}

void TwoLayerWithContinuousUpdate::sortMaterialDownward (std::vector<Grains>& strata, double activeThickness, double sublayerThickness, int cellID)
{
	Grains updateGrains = (activeThickness - layerThickness) * strata.front() / activeThickness;
	updateGrains = (strata.front()).subtract(updateGrains);
	(strata.back()).add(updateGrains);
}

bool TwoLayerWithContinuousUpdate::sortStrata (RegularRiverReachProperties& regularRiverReachProperties, int cellID)
{
	std::vector<Grains>& strata = regularRiverReachProperties.strataPerUnitBedSurface;

	if ( strata.size() != 2 )
	{
		//Otherwise undefined behaviour for the for-loops.
		const char *const errorMessage = "Action on strata consisting of any other number than two layers.";
		throw(errorMessage);
	}

	bool result = false;
	double activeThickness = strata.front().getOverallVolume();
	double sublayerThickness = strata.back().getOverallVolume();

	updateLayerThickness(regularRiverReachProperties);

	if( (activeThickness < layerThickness) && (sublayerThickness > 0.0) )
	{
		this->sortMaterialUpward(strata,activeThickness,sublayerThickness,regularRiverReachProperties,cellID);
		result = true;
	}
	else
	{
		if(activeThickness > layerThickness)
		{
			this->sortMaterialDownward(strata,activeThickness,sublayerThickness,cellID);
			result = true;
		}
	}

	return result;
}

}
