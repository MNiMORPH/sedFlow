/*
 * RecirculateWater.cpp
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

#include "RecirculateWater.h"

#include <limits>

namespace SedFlow {

RecirculateWater::RecirculateWater(double inputDivisor, bool ensureModificationAgainstOtherChangeRateModifiers):
		ChangeRateModifiersType(CombinerVariables::RecirculateWater, CombinerVariables::WaterFlowMethodsInGeneral),
		inputDivisor(inputDivisor),
		ensureModificationAgainstOtherChangeRateModifiers(ensureModificationAgainstOtherChangeRateModifiers),
		downstreamMarginCellPointer(NULL),
		firstTime(true),
		currentlyElapsedSeconds(std::numeric_limits<double>::quiet_NaN())
{}

RecirculateWater::RecirculateWater(double inputDivisor, bool ensureModificationAgainstOtherChangeRateModifiers, RiverReachProperties* downstreamMarginCellPointer, double currentlyElapsedSeconds, double inputDischarge):
	ChangeRateModifiersType(CombinerVariables::RecirculateWater, CombinerVariables::WaterFlowMethodsInGeneral),
	inputDivisor(inputDivisor),
	ensureModificationAgainstOtherChangeRateModifiers(ensureModificationAgainstOtherChangeRateModifiers),
	firstTime(true),
	downstreamMarginCellPointer(downstreamMarginCellPointer),
	currentlyElapsedSeconds(currentlyElapsedSeconds),
	inputDischarge(inputDischarge)
{}

ChangeRateModifiersType* RecirculateWater::createChangeRateModifiersTypePointerCopy() const
{
	ChangeRateModifiersType* result = new RecirculateWater(this->inputDivisor, this->ensureModificationAgainstOtherChangeRateModifiers, this->downstreamMarginCellPointer, this->currentlyElapsedSeconds, this->inputDischarge);
	return result;
}

ConstructionVariables RecirculateWater::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
	result.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::RecirculateWater);
	std::vector<double> doubleVector;
	doubleVector.push_back(inputDivisor);
	result.labelledDoubles["inputDivisor"] = doubleVector;
	doubleVector.clear();
	std::vector<bool> boolVector;
	boolVector.push_back(ensureModificationAgainstOtherChangeRateModifiers);
	result.labelledBools["ensureModificationAgainstOtherChangeRateModifiers"] = boolVector;
	boolVector.clear();
	return result;
}

void RecirculateWater::inputModification(RiverReachProperties& riverReachProperties)
{
	if(firstTime) { this->updateDownstreamMarginCellPointer(riverReachProperties); firstTime = false; }
	if( !( downstreamMarginCellPointer->isDownstreamMargin() ) ) { this->updateDownstreamMarginCellPointer(riverReachProperties); }

	if ( currentlyElapsedSeconds != (riverReachProperties.getOverallParameters())->getElapsedSeconds() )
	{
		inputDischarge = 0.0;
		for(std::vector<RiverReachProperties*>::const_iterator currentInputCell = inputCellPointersForDownstreamMargin.begin(); currentInputCell < inputCellPointersForDownstreamMargin.end(); ++currentInputCell)
				{ inputDischarge += (*currentInputCell)->regularRiverReachProperties.discharge; }
		inputDischarge /= inputDivisor;
		currentlyElapsedSeconds = (riverReachProperties.getOverallParameters())->getElapsedSeconds();
	}

	if ( riverReachProperties.isUpstreamMargin() )
	{
		riverReachProperties.regularRiverReachProperties.discharge = inputDischarge;
	}
}

void RecirculateWater::updateDownstreamMarginCellPointer(RiverReachProperties& riverReachProperties)
{
	RiverReachProperties* currentCellPointer = &riverReachProperties;
	while ( !(currentCellPointer->isDownstreamMargin()) )
				{ currentCellPointer = currentCellPointer->getDownstreamCellPointer(); }
	this->downstreamMarginCellPointer = currentCellPointer;
	this->inputCellPointersForDownstreamMargin.clear();
	this->inputCellPointersForDownstreamMargin = this->downstreamMarginCellPointer->getUpstreamCellPointers();
}

}
