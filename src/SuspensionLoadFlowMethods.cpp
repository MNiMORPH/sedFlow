/*
 * SuspensionLoadFlowMethods.cpp
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

#include "SuspensionLoadFlowMethods.h"

	//Up to now this class is just an empty place holder.

namespace SedFlow {

SuspensionLoadFlowMethods::SuspensionLoadFlowMethods(double maximumFractionOfActiveLayerToBeEroded, bool preventZeroOrNegativeBedSlopes, double maximumRelativeTwoCellBedSlopeChange, const OverallMethods& overallMethods):
	SedimentFlowTypeMethods(maximumFractionOfActiveLayerToBeEroded,preventZeroOrNegativeBedSlopes,maximumRelativeTwoCellBedSlopeChange,overallMethods)
{
	this->typeOfSedimentFlowMethods = CombinerVariables::SuspensionLoadFlowMethods;
}

SedimentFlowTypeMethods* SuspensionLoadFlowMethods::createSedimentFlowTypeMethodsPointerCopy() const
{
	SedimentFlowTypeMethods* result = new SuspensionLoadFlowMethods(this->maximumFractionOfActiveLayerToBeEroded, this->preventZeroOrNegativeBedSlopes, this->maximumRelativeTwoCellBedSlopeChange, this->overallMethods);
	return result;
}

SuspensionLoadFlowMethods::~SuspensionLoadFlowMethods()
{
	// The line { delete bedSlopeCalculationMethod; } has already been called in SedimentFlowTypeMethods::~SedimentFlowTypeMethods()
}

ConstructionVariables SuspensionLoadFlowMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::SedimentFlowTypeMethods;
	result.realisationType = CombinerVariables::typeOfSedimentFlowMethodsToString(CombinerVariables::SuspensionLoadFlowMethods);
	std::vector<double> doubleVector;
	doubleVector.push_back(maximumFractionOfActiveLayerToBeEroded);
	result.labelledDoubles["maximumFractionOfActiveLayerToBeEroded"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(preventZeroOrNegativeBedSlopes);
	result.labelledBools["preventZeroOrNegativeBedSlopes"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(maximumRelativeTwoCellBedSlopeChange);
	result.labelledDoubles["maximumRelativeTwoCellBedSlopeChange"] = doubleVector;
	return result;
}

void SuspensionLoadFlowMethods::calculateChangeRate(RiverReachProperties& riverReachProperties) const {}

}
