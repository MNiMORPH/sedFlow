/*
 * StratigraphyWithOLDConstantThresholdBasedUpdate.cpp
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

#include "StratigraphyWithOLDConstantThresholdBasedUpdate.h"

namespace SedFlow {

StratigraphyWithOLDConstantThresholdBasedUpdate::StratigraphyWithOLDConstantThresholdBasedUpdate(double layerThickness, double minimumLayerThicknessPerUnitLayerThickness, double maximumLayerThicknessPerUnitLayerThickness):
	StrataSorting(layerThickness),
	minimumLayerThicknessPerUnitLayerThickness(minimumLayerThicknessPerUnitLayerThickness),
	maximumLayerThicknessPerUnitLayerThickness(maximumLayerThicknessPerUnitLayerThickness)
{
	if (maximumLayerThicknessPerUnitLayerThickness < 1.0)
	{
		const char *const errorMessage = "Maximum layer thickness per unit layer thickness is smaller than 1";
		throw(errorMessage);
	}
	if (minimumLayerThicknessPerUnitLayerThickness < 0.0)
	{
		const char *const errorMessage = "Negative minimum layer thickness";
		throw(errorMessage);
	}
	if (minimumLayerThicknessPerUnitLayerThickness > 1.0)
	{
		const char *const errorMessage = "Minimum layer thickness per unit layer thickness is larger than 1";
		throw(errorMessage);
	}
	if ( (minimumLayerThicknessPerUnitLayerThickness + 1.0) > maximumLayerThicknessPerUnitLayerThickness)
	{
		// This check is important as it prevents overcorrection.
		const char *const errorMessage = "Minimum layer thickness plus layer thickness is larger than maximum layer thickness";
		throw(errorMessage);
	}
}

StratigraphyWithOLDConstantThresholdBasedUpdate::~StratigraphyWithOLDConstantThresholdBasedUpdate(){}

StrataSorting* StratigraphyWithOLDConstantThresholdBasedUpdate::createStrataSortingPointerCopy() const
{
	StrataSorting* result = new StratigraphyWithOLDConstantThresholdBasedUpdate(this->layerThickness, this->minimumLayerThicknessPerUnitLayerThickness, this->maximumLayerThicknessPerUnitLayerThickness);
	return result;
}

ConstructionVariables StratigraphyWithOLDConstantThresholdBasedUpdate::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::StrataSorting;
	result.realisationType = CombinerVariables::typeOfStrataSortingToString(CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate);
	std::vector<double> doubleVector;
	doubleVector.push_back(layerThickness);
	result.labelledDoubles["layerThickness"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(minimumLayerThicknessPerUnitLayerThickness);
	result.labelledDoubles["minimumLayerThicknessPerUnitLayerThickness"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(maximumLayerThicknessPerUnitLayerThickness);
	result.labelledDoubles["maximumLayerThicknessPerUnitLayerThickness"] = doubleVector;
	return result;
}

bool StratigraphyWithOLDConstantThresholdBasedUpdate::sortStrata (RegularRiverReachProperties& regularRiverReachProperties, int cellID)
{
	std::vector<Grains>& strata = regularRiverReachProperties.strataPerUnitBedSurface;

	if ( strata.size() < 2 )
	{
		//Otherwise undefined behaviour for the for-loops.
		const char *const errorMessage = "Action on strata consisting of less than two layers.";
		throw(errorMessage);
	}

	bool result = false;
	double activeThickness = strata.at(0).getOverallVolume();

	if(activeThickness < (layerThickness * minimumLayerThicknessPerUnitLayerThickness) )
	{
		Grains previousActiveLayer = strata.front();
		std::vector<Grains>::iterator baseLayer = (strata.end()-1);
		if(strata.size() > 2)
		{
			for(std::vector<Grains>::iterator currentLayer = strata.begin(); currentLayer < (baseLayer-1); ++currentLayer)
			{
				(*currentLayer) = (*(currentLayer+1));
			}
		}

		double baseLayerThicknessPerUnitLayerThickness = (*baseLayer).getOverallVolume() / layerThickness;
		if ( baseLayerThicknessPerUnitLayerThickness < 1.0 ) { baseLayerThicknessPerUnitLayerThickness = 1.0; }
		Grains layerUpdate = (*baseLayer) / baseLayerThicknessPerUnitLayerThickness;
		(*(baseLayer-1)) = layerUpdate;
		(*baseLayer).subtract(layerUpdate);

		strata.front() += previousActiveLayer;

		result = true;
	}

	if(activeThickness > (layerThickness * maximumLayerThicknessPerUnitLayerThickness) )
	{
		int numberOfNewLayers = static_cast<int>( (activeThickness / layerThickness) );
		if ( (activeThickness - (static_cast<double>(numberOfNewLayers) * layerThickness)) < (minimumLayerThicknessPerUnitLayerThickness * layerThickness) ) { numberOfNewLayers -= 1; }
		Grains layerUpdate = strata.front() * (  layerThickness / (strata.front()).getOverallVolume() );
		Grains previousBaseLayer;
		for(int i = 0; i < numberOfNewLayers; ++i)
		{
			previousBaseLayer = strata.back();
			if (strata.size() > 2)
			{
				for(std::vector<Grains>::reverse_iterator currentLayer = strata.rbegin(); currentLayer < (strata.rend() - 2); ++currentLayer)
				{
					(*currentLayer) = (*(currentLayer + 1));
				}
			}
			strata.at(1) = layerUpdate;
			strata.at(0).subtract(layerUpdate);
			strata.back() += previousBaseLayer;
		}
		result = true;
	}

	return result;
}

}
