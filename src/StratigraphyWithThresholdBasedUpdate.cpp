/*
 * StratigraphyWithThresholdBasedUpdate.cpp
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

#include "StratigraphyWithThresholdBasedUpdate.h"

#include <algorithm>
#include <math.h>

namespace SedFlow {

void StratigraphyWithThresholdBasedUpdate::checkAndUpdateValuesForConstructor()
{
	if ( this->incrementLayerThickness <= 0 )
	{
		const char *const incrementLayerThicknessErrorMessage = "For StratigraphyWithThresholdBasedUpdate a positive, non-zero incrementLayerThickness is needed.";
		throw(incrementLayerThicknessErrorMessage);
	}

	if ( (this->referenceGrainSizePercentile < 0.0) || (this->referenceGrainSizePercentile > 100.0) )
	{
		const char *const referenceGrainSizePercentileErrorMessage = "For StratigraphyWithThresholdBasedUpdate the referenceGrainSizePercentile has to be a floating point number between 0 and 100.";
		throw(referenceGrainSizePercentileErrorMessage);
	}

	if ( this->valueOrFactorForLowerThresholdForActiveLayerThickness <= 0 )
	{
		const char *const valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage = "For StratigraphyWithThresholdBasedUpdate a positive, non-zero valueOrFactorForLowerThresholdForActiveLayerThickness is needed.";
		throw(valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage);
	}

	if ( this->valueOrFactorForUpperThresholdForActiveLayerThickness <= this->valueOrFactorForLowerThresholdForActiveLayerThickness )
	{
		const char *const valueOrFactorForUpperThresholdForActiveLayerThicknessErrorMessage = "For StratigraphyWithThresholdBasedUpdate the valueOrFactorForUpperThresholdForActiveLayerThickness has to be larger than the valueOrFactorForLowerThresholdForActiveLayerThickness.";
		throw(valueOrFactorForUpperThresholdForActiveLayerThicknessErrorMessage);
	}

	if ( this->minimumLowerThresholdForActiveLayerThickness <= 0 )
	{
		const char *const minimumLowerThresholdForActiveLayerThicknessErrorMessage = "For StratigraphyWithThresholdBasedUpdate a positive, non-zero minimumLowerThresholdForActiveLayerThickness is needed.";
		throw(minimumLowerThresholdForActiveLayerThicknessErrorMessage);
	}

	if( this->minimumDifferenceBetweenThresholdsForActiveLayerThickness <= this->incrementLayerThickness )
	{
		const char *const minimumDifferenceErrorMessage = "For StratigraphyWithThresholdBasedUpdate the minimumDifferenceBetweenThresholdsForActiveLayerThickness has to be larger than the incrementLayerThickness.";
		throw(minimumDifferenceErrorMessage);
	}

	if( !dynamicThresholds )
	{
		this->lowerThresholdForActiveLayerThickness = this->valueOrFactorForLowerThresholdForActiveLayerThickness;
		this->upperThresholdForActiveLayerThickness = this->valueOrFactorForUpperThresholdForActiveLayerThickness;
		if( (this->upperThresholdForActiveLayerThickness - this->lowerThresholdForActiveLayerThickness) < this->minimumDifferenceBetweenThresholdsForActiveLayerThickness )
		{
			this->lowerThresholdForActiveLayerThickness = 0.5 * ( this->lowerThresholdForActiveLayerThickness + this->upperThresholdForActiveLayerThickness - this->minimumDifferenceBetweenThresholdsForActiveLayerThickness );
		}
	}
	this->lowerThresholdForActiveLayerThickness = std::max(this->lowerThresholdForActiveLayerThickness,this->minimumLowerThresholdForActiveLayerThickness);
	this->upperThresholdForActiveLayerThickness = std::max(this->upperThresholdForActiveLayerThickness,(this->lowerThresholdForActiveLayerThickness + this->minimumDifferenceBetweenThresholdsForActiveLayerThickness));

	this->layerThickness = 0.5 * (this->lowerThresholdForActiveLayerThickness + this->upperThresholdForActiveLayerThickness);
}

StratigraphyWithThresholdBasedUpdate::StratigraphyWithThresholdBasedUpdate(double incrementLayerThickness, bool dynamicThresholds, double valueOrFactorForLowerThresholdForActiveLayerThickness, double valueOrFactorForUpperThresholdForActiveLayerThickness):
	StrataSorting(0.4),//This is just a dummy layer thickness.
	incrementLayerThickness(incrementLayerThickness),
	dynamicThresholds(dynamicThresholds),
	referenceGrainSizePercentile(44.4),//This is just a dummy percentile.
	valueOrFactorForLowerThresholdForActiveLayerThickness(valueOrFactorForLowerThresholdForActiveLayerThickness),
	valueOrFactorForUpperThresholdForActiveLayerThickness(valueOrFactorForUpperThresholdForActiveLayerThickness),
	minimumDifferenceBetweenThresholdsForActiveLayerThickness((1.1 * incrementLayerThickness)),
	lowerThresholdForActiveLayerThickness(-1.0),
	upperThresholdForActiveLayerThickness(-0.5)
{
	if(this->dynamicThresholds)
	{
		const char *const missingReferenceGrainSizePercentileErrorMessage = "For StratigraphyWithThresholdBasedUpdate with dynamicThresholds a referenceGrainSizePercentile is needed.";
		throw(missingReferenceGrainSizePercentileErrorMessage);
	}

	this->minimumLowerThresholdForActiveLayerThickness = this->minimumDifferenceBetweenThresholdsForActiveLayerThickness * this->valueOrFactorForLowerThresholdForActiveLayerThickness / (this->valueOrFactorForUpperThresholdForActiveLayerThickness - this->valueOrFactorForLowerThresholdForActiveLayerThickness);
	this->checkAndUpdateValuesForConstructor();
}

StratigraphyWithThresholdBasedUpdate::StratigraphyWithThresholdBasedUpdate(double incrementLayerThickness, bool dynamicThresholds, double referenceGrainSizePercentile, double valueOrFactorForLowerThresholdForActiveLayerThickness, double valueOrFactorForUpperThresholdForActiveLayerThickness):
	StrataSorting(0.4),//This is just a dummy layer thickness.
	incrementLayerThickness(incrementLayerThickness),
	dynamicThresholds(dynamicThresholds),
	referenceGrainSizePercentile(referenceGrainSizePercentile),
	valueOrFactorForLowerThresholdForActiveLayerThickness(valueOrFactorForLowerThresholdForActiveLayerThickness),
	valueOrFactorForUpperThresholdForActiveLayerThickness(valueOrFactorForUpperThresholdForActiveLayerThickness),
	minimumLowerThresholdForActiveLayerThickness((0.1 * incrementLayerThickness)),
	minimumDifferenceBetweenThresholdsForActiveLayerThickness((1.1 * incrementLayerThickness)),
	lowerThresholdForActiveLayerThickness(-1.0),
	upperThresholdForActiveLayerThickness(-0.5)
{
	this->minimumLowerThresholdForActiveLayerThickness = this->minimumDifferenceBetweenThresholdsForActiveLayerThickness * this->valueOrFactorForLowerThresholdForActiveLayerThickness / (this->valueOrFactorForUpperThresholdForActiveLayerThickness - this->valueOrFactorForLowerThresholdForActiveLayerThickness);
	this->checkAndUpdateValuesForConstructor();
}

StratigraphyWithThresholdBasedUpdate::StratigraphyWithThresholdBasedUpdate(double incrementLayerThickness, bool dynamicThresholds, double referenceGrainSizePercentile, double valueOrFactorForLowerThresholdForActiveLayerThickness, double valueOrFactorForUpperThresholdForActiveLayerThickness, double minimumLowerThresholdForActiveLayerThickness, double minimumDifferenceBetweenThresholdsForActiveLayerThickness):
	StrataSorting(0.4),//This is just a dummy layer thickness.
	incrementLayerThickness(incrementLayerThickness),
	dynamicThresholds(dynamicThresholds),
	referenceGrainSizePercentile(referenceGrainSizePercentile),
	valueOrFactorForLowerThresholdForActiveLayerThickness(valueOrFactorForLowerThresholdForActiveLayerThickness),
	valueOrFactorForUpperThresholdForActiveLayerThickness(valueOrFactorForUpperThresholdForActiveLayerThickness),
	minimumLowerThresholdForActiveLayerThickness(minimumLowerThresholdForActiveLayerThickness),
	minimumDifferenceBetweenThresholdsForActiveLayerThickness(minimumDifferenceBetweenThresholdsForActiveLayerThickness),
	lowerThresholdForActiveLayerThickness(-1.0),
	upperThresholdForActiveLayerThickness(-0.5)
{
	this->checkAndUpdateValuesForConstructor();
}

StratigraphyWithThresholdBasedUpdate::~StratigraphyWithThresholdBasedUpdate() {}

StrataSorting* StratigraphyWithThresholdBasedUpdate::createStrataSortingPointerCopy() const
{
	StrataSorting* result = new StratigraphyWithThresholdBasedUpdate(this->incrementLayerThickness, this->dynamicThresholds, this->referenceGrainSizePercentile, this->valueOrFactorForLowerThresholdForActiveLayerThickness, this->valueOrFactorForUpperThresholdForActiveLayerThickness, this->minimumLowerThresholdForActiveLayerThickness, this->minimumDifferenceBetweenThresholdsForActiveLayerThickness);
	return result;
}

ConstructionVariables StratigraphyWithThresholdBasedUpdate::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::StrataSorting;
	result.realisationType = CombinerVariables::typeOfStrataSortingToString(CombinerVariables::StratigraphyWithThresholdBasedUpdate);
	std::vector<double> doubleVector;
	doubleVector.push_back(layerThickness);
	result.labelledDoubles["layerThickness"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(incrementLayerThickness);
	result.labelledDoubles["incrementLayerThickness"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(dynamicThresholds);
	result.labelledBools["dynamicThresholds"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(referenceGrainSizePercentile);
	result.labelledDoubles["referenceGrainSizePercentile"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(valueOrFactorForLowerThresholdForActiveLayerThickness);
	result.labelledDoubles["valueOrFactorForLowerThresholdForActiveLayerThickness"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(valueOrFactorForUpperThresholdForActiveLayerThickness);
	result.labelledDoubles["valueOrFactorForUpperThresholdForActiveLayerThickness"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(minimumLowerThresholdForActiveLayerThickness);
	result.labelledDoubles["minimumLowerThresholdForActiveLayerThickness"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(minimumDifferenceBetweenThresholdsForActiveLayerThickness);
	result.labelledDoubles["minimumDifferenceBetweenThresholdsForActiveLayerThickness"] = doubleVector;
	return result;
}

void StratigraphyWithThresholdBasedUpdate::updateThresholds(const Grains& activeLayer, const std::vector<double>& fractionalGrainDiameters)
{
	double referenceDiameter = activeLayer.getPercentileGrainDiameter(fractionalGrainDiameters, referenceGrainSizePercentile);
	lowerThresholdForActiveLayerThickness = referenceDiameter * valueOrFactorForLowerThresholdForActiveLayerThickness;
	upperThresholdForActiveLayerThickness = referenceDiameter * valueOrFactorForUpperThresholdForActiveLayerThickness;

	lowerThresholdForActiveLayerThickness = std::max(lowerThresholdForActiveLayerThickness,minimumLowerThresholdForActiveLayerThickness);
	upperThresholdForActiveLayerThickness = std::max(upperThresholdForActiveLayerThickness,(lowerThresholdForActiveLayerThickness + minimumDifferenceBetweenThresholdsForActiveLayerThickness));
}

void StratigraphyWithThresholdBasedUpdate::updateLayerThickness (const RegularRiverReachProperties& regularRiverReachProperties)
{
	if( dynamicThresholds && ((regularRiverReachProperties.strataPerUnitBedSurface.front().getOverallVolume()) > 0.0) )
	{
		this->updateThresholds(regularRiverReachProperties.strataPerUnitBedSurface.front(),(regularRiverReachProperties.getOverallParameters())->getFractionalGrainDiameters());
		layerThickness = 0.5 * (lowerThresholdForActiveLayerThickness + upperThresholdForActiveLayerThickness);
	}
}

bool StratigraphyWithThresholdBasedUpdate::sortStrata (RegularRiverReachProperties& regularRiverReachProperties, int cellID)
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

	updateLayerThickness(regularRiverReachProperties);

	if(activeThickness < lowerThresholdForActiveLayerThickness)
	{
		if( strata.at(1).getOverallVolume() > 0.0 )
		{
			do
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

				double incrementLayerThicknessThicknessPerUnitBaseLayer = incrementLayerThickness / (*baseLayer).getOverallVolume();
				if ( incrementLayerThicknessThicknessPerUnitBaseLayer > 1.0 ) { incrementLayerThicknessThicknessPerUnitBaseLayer = 1.0; }
				Grains layerUpdate = (*baseLayer) * incrementLayerThicknessThicknessPerUnitBaseLayer;
				(*(baseLayer-1)) = layerUpdate;
				(*baseLayer).subtract(layerUpdate);

				strata.front() += previousActiveLayer;

			} while ( ( fabs((strata.at(0).getOverallVolume() - layerThickness)) > (0.5 * incrementLayerThickness) ) && ( strata.at(1).getOverallVolume() > 0.0 ) );

			result = true;
		}
	}
	else
	{
		if( activeThickness > upperThresholdForActiveLayerThickness )
		{
			Grains layerUpdate = strata.front() * (  incrementLayerThickness / activeThickness );
			Grains previousBaseLayer;
			do
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
			} while ( fabs((strata.at(0).getOverallVolume() - layerThickness)) > (0.5 * incrementLayerThickness) );

			result = true;
		}
	}

	return result;
}

std::pair<double,double> StratigraphyWithThresholdBasedUpdate::getThresholdsForActiveLayerThickness (bool dynamic, const Grains& activeLayer, const std::vector<double>& fractionalGrainDiameters)
{
	if(dynamic) { this->updateThresholds(activeLayer,fractionalGrainDiameters); }
	return std::pair<double,double>(this->lowerThresholdForActiveLayerThickness, this->upperThresholdForActiveLayerThickness);
}

}
