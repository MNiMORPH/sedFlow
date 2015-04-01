/*
 * TwoLayerWithShearStressBasedUpdate.cpp
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

#include "TwoLayerWithShearStressBasedUpdate.h"

#include <math.h>

#include "BasicCalculations.h"

namespace SedFlow {

TwoLayerWithShearStressBasedUpdate::TwoLayerWithShearStressBasedUpdate(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, bool dynamicBreakUpConditions, bool usePredefinedBreakUpConditions, double layerThickness, bool dynamicLayerThickness, double layerThicknessFactor, double referenceGrainSizePercentile):
	TwoLayerWithContinuousUpdate(layerThickness,dynamicLayerThickness,layerThicknessFactor,referenceGrainSizePercentile),
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	dynamicBreakUpConditions(dynamicBreakUpConditions),
	usePredefinedBreakUpConditions(usePredefinedBreakUpConditions)
{
	if( this->usePredefinedBreakUpConditions )
	{
		const char *const missingValuesErrorMessage = "For TwoLayerWithShearStressBasedUpdate the variables referenceMedianDiameter, thetaCriticalForActiveLayer and thetaCriticalForSublayer are needed, if usePredefinedBreakUpConditions.";
		throw(missingValuesErrorMessage);
	}
}

TwoLayerWithShearStressBasedUpdate::TwoLayerWithShearStressBasedUpdate(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, bool dynamicBreakUpConditions, bool usePredefinedBreakUpConditions, double referenceMedianDiameter, double thetaCriticalForActiveLayer, double thetaCriticalForSublayer, double layerThickness, bool dynamicLayerThickness, double layerThicknessFactor, double referenceGrainSizePercentile):
	TwoLayerWithContinuousUpdate(layerThickness,dynamicLayerThickness,layerThicknessFactor,referenceGrainSizePercentile),
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	dynamicBreakUpConditions(dynamicBreakUpConditions),
	usePredefinedBreakUpConditions(usePredefinedBreakUpConditions)
{
	if ( referenceMedianDiameter <= 0 )
	{
		const char *const referenceMedianDiameterErrorMessage = "For TwoLayerWithShearStressBasedUpdate a positive, non-zero referenceMedianDiameter is needed.";
		throw(referenceMedianDiameterErrorMessage);
	}

	if ( thetaCriticalForSublayer <= 0 )
	{
		const char *const thetaCriticalForSublayerErrorMessage = "For TwoLayerWithShearStressBasedUpdate a positive, non-zero thetaCriticalForSublayer is needed.";
		throw(thetaCriticalForSublayerErrorMessage);
	}

	if ( thetaCriticalForActiveLayer <= thetaCriticalForSublayer )
	{
		const char *const thetaCriticalForActiveLayerErrorMessage = "For TwoLayerWithShearStressBasedUpdate the thetaCriticalForActiveLayer needs to be larger than the thetaCriticalForSublayer.";
		throw(thetaCriticalForActiveLayerErrorMessage);
	}

	this->predefinedBaseDataForSublayerInfluence.medianDiameterForActiveLayer = referenceMedianDiameter;
	this->predefinedBaseDataForSublayerInfluence.thetaCriticalForSublayer = thetaCriticalForSublayer;
	this->predefinedBaseDataForSublayerInfluence.oneOverDifferenceBetweenThetaCriticalForActiveAndSublayer = 1.0 / (thetaCriticalForActiveLayer - thetaCriticalForSublayer);
}

TwoLayerWithShearStressBasedUpdate::TwoLayerWithShearStressBasedUpdate(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, bool dynamicBreakUpConditions, bool usePredefinedBreakUpConditions, const TwoLayerWithShearStressBasedUpdate_BaseDataForSublayerInfluence& predefinedBaseDataForSublayerInfluence, double layerThickness, bool dynamicLayerThickness, double layerThicknessFactor, double referenceGrainSizePercentile, const std::map<int,Grains>& mapFromCellIDToSublayerComposition, const std::map<int,TwoLayerWithShearStressBasedUpdate_BaseDataForSublayerInfluence>& mapFromCellIDToBaseDataForSublayerInfluence):
	TwoLayerWithContinuousUpdate(layerThickness,dynamicLayerThickness,layerThicknessFactor,referenceGrainSizePercentile),
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	dynamicBreakUpConditions(dynamicBreakUpConditions),
	usePredefinedBreakUpConditions(usePredefinedBreakUpConditions),
	mapFromCellIDToSublayerComposition(mapFromCellIDToSublayerComposition),
	mapFromCellIDToBaseDataForSublayerInfluence(mapFromCellIDToBaseDataForSublayerInfluence),
	predefinedBaseDataForSublayerInfluence(predefinedBaseDataForSublayerInfluence)
{}

TwoLayerWithShearStressBasedUpdate::~TwoLayerWithShearStressBasedUpdate()
{
	delete thresholdCalculationMethod;
}

StrataSorting* TwoLayerWithShearStressBasedUpdate::createStrataSortingPointerCopy() const
{
	StrataSorting* result = new TwoLayerWithShearStressBasedUpdate(this->thresholdCalculationMethod, this->dynamicBreakUpConditions, this->usePredefinedBreakUpConditions, this->predefinedBaseDataForSublayerInfluence, this->layerThickness, this->dynamicLayerThickness, this->layerThicknessFactor, this->referenceGrainSizePercentile, this->mapFromCellIDToSublayerComposition, this->mapFromCellIDToBaseDataForSublayerInfluence);
	return result;
}

ConstructionVariables TwoLayerWithShearStressBasedUpdate::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::StrataSorting;
	result.realisationType = CombinerVariables::typeOfStrataSortingToString(CombinerVariables::TwoLayerWithShearStressBasedUpdate);
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
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( thresholdCalculationMethod->createConstructionVariables() );
	result.labelledObjects["thresholdCalculationMethod"] = constructionVariablesVector;
	boolVector.clear();
	boolVector.push_back(dynamicBreakUpConditions);
	result.labelledBools["dynamicBreakUpConditions"] = boolVector;
	boolVector.clear();
	boolVector.push_back(usePredefinedBreakUpConditions);
	result.labelledBools["usePredefinedBreakUpConditions"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(predefinedBaseDataForSublayerInfluence.medianDiameterForActiveLayer);
	result.labelledDoubles["referenceMedianDiameter"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back( ((1.0 / predefinedBaseDataForSublayerInfluence.oneOverDifferenceBetweenThetaCriticalForActiveAndSublayer) + predefinedBaseDataForSublayerInfluence.thetaCriticalForSublayer) );
	result.labelledDoubles["thetaCriticalForActiveLayer"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(predefinedBaseDataForSublayerInfluence.thetaCriticalForSublayer);
	result.labelledDoubles["thetaCriticalForSublayer"] = doubleVector;
	return result;
}

void TwoLayerWithShearStressBasedUpdate::sortMaterialUpward (std::vector<Grains>& strata, double activeThickness, double sublayerThickness, const RegularRiverReachProperties& regularRiverReachProperties, int cellID)
{
	double sublayerInfluence;
	if(this->usePredefinedBreakUpConditions)
	{
		double currentTheta = BasicCalculations::calculateDimensionlessShearStress(this->predefinedBaseDataForSublayerInfluence.medianDiameterForActiveLayer,regularRiverReachProperties);
		sublayerInfluence = (currentTheta - this->predefinedBaseDataForSublayerInfluence.thetaCriticalForSublayer) * this->predefinedBaseDataForSublayerInfluence.oneOverDifferenceBetweenThetaCriticalForActiveAndSublayer;
		sublayerInfluence = std::max(0.0,sublayerInfluence);
		sublayerInfluence = std::min(1.0,sublayerInfluence);
	}
	else
	{
		std::map<int,TwoLayerWithShearStressBasedUpdate_BaseDataForSublayerInfluence>::const_iterator baseDataEntry = this->mapFromCellIDToBaseDataForSublayerInfluence.find(cellID);
		if( ( this->dynamicBreakUpConditions || (baseDataEntry == this->mapFromCellIDToBaseDataForSublayerInfluence.end()) ) && (activeThickness > 0.0) )
		{
			TwoLayerWithShearStressBasedUpdate_BaseDataForSublayerInfluence tempBaseData;

			std::vector<double> fractionalGrainDiameters = (regularRiverReachProperties.getOverallParameters())->getFractionalGrainDiameters();
			double meanDiameterForActiveLayer = (strata.front()).getArithmeticMeanGrainDiameter(fractionalGrainDiameters);
			tempBaseData.medianDiameterForActiveLayer = (strata.front()).getPercentileGrainDiameter(fractionalGrainDiameters,50.0);
			double meanDiameterForSublayer = (strata.back()).getArithmeticMeanGrainDiameter(fractionalGrainDiameters);
			meanDiameterForSublayer = std::min(meanDiameterForSublayer,meanDiameterForActiveLayer);
			tempBaseData.thetaCriticalForSublayer = this->thresholdCalculationMethod->calculate(regularRiverReachProperties);
			double thetaCriticalForActiveLayer = pow( (meanDiameterForActiveLayer / meanDiameterForSublayer), (2.0/3.0) ) * tempBaseData.thetaCriticalForSublayer;
			tempBaseData.oneOverDifferenceBetweenThetaCriticalForActiveAndSublayer = 1.0 / ( thetaCriticalForActiveLayer - tempBaseData.thetaCriticalForSublayer );

			this->mapFromCellIDToBaseDataForSublayerInfluence[cellID] = tempBaseData;
			baseDataEntry = this->mapFromCellIDToBaseDataForSublayerInfluence.find(cellID);
		}

		if(baseDataEntry != this->mapFromCellIDToBaseDataForSublayerInfluence.end())
		{
			double currentTheta = BasicCalculations::calculateDimensionlessShearStress(baseDataEntry->second.medianDiameterForActiveLayer,regularRiverReachProperties);
			sublayerInfluence = (currentTheta - baseDataEntry->second.thetaCriticalForSublayer) * baseDataEntry->second.oneOverDifferenceBetweenThetaCriticalForActiveAndSublayer;
			sublayerInfluence = std::max(0.0,sublayerInfluence);
			sublayerInfluence = std::min(1.0,sublayerInfluence);
		}
		else
		{
			sublayerInfluence = 1.0;
		}
	}

	double updateThickness = std::min((this->layerThickness - activeThickness),sublayerThickness);
	if(updateThickness == sublayerThickness)
	{
		this->mapFromCellIDToSublayerComposition[cellID] = strata.back();
	}
	Grains updateGrains = updateThickness * ( ( (1.0 - sublayerInfluence) * strata.front() / activeThickness ) + ( sublayerInfluence * strata.back() / sublayerThickness ));
	(strata.back()).subtract((updateThickness * strata.back() / sublayerThickness));
	(strata.front()).add(updateGrains);
}

void TwoLayerWithShearStressBasedUpdate::sortMaterialDownward (std::vector<Grains>& strata, double activeThickness, double sublayerThickness, int cellID)
{
	strata.front() *= layerThickness / activeThickness;
	if(sublayerThickness > 0.0)
	{
		strata.back() *= (sublayerThickness + activeThickness - layerThickness) / sublayerThickness;
	}
	else
	{
		const Grains* sublayerBase = &(strata.front());
		std::map<int,Grains>::const_iterator sublayerEntry = this->mapFromCellIDToSublayerComposition.find(cellID);
		if(sublayerEntry != this->mapFromCellIDToSublayerComposition.end()) { sublayerBase = &(sublayerEntry->second); }
		strata.back() = *sublayerBase * (sublayerThickness + activeThickness - layerThickness) / sublayerBase->getOverallVolume();
	}
}

}
