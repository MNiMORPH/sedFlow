/*
 * BedloadFlowMethods.cpp
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

#include "BedloadFlowMethods.h"

namespace SedFlow {

BedloadFlowMethods::BedloadFlowMethods(double maximumFractionOfActiveLayerToBeEroded, bool preventZeroOrNegativeBedSlopes, double maximumRelativeTwoCellBedSlopeChange, const OverallMethods& overallMethods, const CalcBedloadCapacity* bedLoadCapacityCalculationMethod):
	SedimentFlowTypeMethods(maximumFractionOfActiveLayerToBeEroded,preventZeroOrNegativeBedSlopes,maximumRelativeTwoCellBedSlopeChange,overallMethods),
	bedLoadCapacityCalculationMethod(bedLoadCapacityCalculationMethod->createCalcBedloadCapacityMethodPointerCopy())
{
	this->typeOfSedimentFlowMethods = CombinerVariables::BedloadFlowMethods;
}

BedloadFlowMethods::~BedloadFlowMethods()
{
	delete bedLoadCapacityCalculationMethod;
}

SedimentFlowTypeMethods* BedloadFlowMethods::createSedimentFlowTypeMethodsPointerCopy() const
{
	SedimentFlowTypeMethods* result = new BedloadFlowMethods(this->maximumFractionOfActiveLayerToBeEroded, this->preventZeroOrNegativeBedSlopes, this->maximumRelativeTwoCellBedSlopeChange, this->overallMethods, this->bedLoadCapacityCalculationMethod);
	return result;
}

ConstructionVariables BedloadFlowMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::SedimentFlowTypeMethods;
	result.realisationType = CombinerVariables::typeOfSedimentFlowMethodsToString(CombinerVariables::BedloadFlowMethods);
	std::vector<double> doubleVector;
	doubleVector.push_back(maximumFractionOfActiveLayerToBeEroded);
	result.labelledDoubles["maximumFractionOfActiveLayerToBeEroded"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(preventZeroOrNegativeBedSlopes);
	result.labelledBools["preventZeroOrNegativeBedSlopes"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(maximumRelativeTwoCellBedSlopeChange);
	result.labelledDoubles["maximumRelativeTwoCellBedSlopeChange"] = doubleVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( bedLoadCapacityCalculationMethod->createConstructionVariables() );
	result.labelledObjects["bedLoadCapacityCalculationMethod"] = constructionVariablesVector;
	return result;
}

void BedloadFlowMethods::calculateChangeRate(RiverReachProperties& riverReachProperties)const
{
	riverReachProperties.regularRiverReachProperties.erosionRate = (*(this->bedLoadCapacityCalculationMethod)).calculate(riverReachProperties,this->overallMethods);
}

}
