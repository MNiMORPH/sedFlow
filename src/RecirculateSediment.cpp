/*
 * RecirculateSediment.cpp
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

#include "RecirculateSediment.h"

#include <limits>

namespace SedFlow {

RecirculateSediment::RecirculateSediment(double inputDivisor, bool ensureModificationAgainstOtherChangeRateModifiers):
		ChangeRateModifiersType(CombinerVariables::RecirculateSediment, CombinerVariables::SedimentFlowMethodsInGeneral),
		inputDivisor(inputDivisor),
		ensureModificationAgainstOtherChangeRateModifiers(ensureModificationAgainstOtherChangeRateModifiers),
		downstreamMarginCellPointer(NULL),
		firstTime(true),
		currentlyElapsedSeconds(std::numeric_limits<double>::quiet_NaN())
{}

RecirculateSediment::RecirculateSediment(double inputDivisor, bool ensureModificationAgainstOtherChangeRateModifiers, RiverReachProperties* downstreamMarginCellPointer, double currentlyElapsedSeconds, const Grains& inputErosionRate, const Grains& inputErosionRateForNextTimeStep):
	ChangeRateModifiersType(CombinerVariables::RecirculateSediment, CombinerVariables::SedimentFlowMethodsInGeneral),
	inputDivisor(inputDivisor),
	ensureModificationAgainstOtherChangeRateModifiers(ensureModificationAgainstOtherChangeRateModifiers),
	firstTime(true),
	downstreamMarginCellPointer(downstreamMarginCellPointer),
	currentlyElapsedSeconds(currentlyElapsedSeconds),
	inputErosionRate(inputErosionRate),
	inputErosionRateForNextTimeStep(inputErosionRateForNextTimeStep)
{}

ChangeRateModifiersType* RecirculateSediment::createChangeRateModifiersTypePointerCopy() const
{
	ChangeRateModifiersType* result = new RecirculateSediment(this->inputDivisor, this->ensureModificationAgainstOtherChangeRateModifiers, this->downstreamMarginCellPointer, this->currentlyElapsedSeconds, this->inputErosionRate, this->inputErosionRateForNextTimeStep);
	return result;
}

ConstructionVariables RecirculateSediment::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
	result.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::RecirculateSediment);
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

void RecirculateSediment::modificationBeforeUpdates(RiverReachProperties& riverReachProperties)
{
	if(firstTime) { this->updateDownstreamMarginCellPointer(riverReachProperties); firstTime = false; }
	if( !( downstreamMarginCellPointer->isDownstreamMargin() ) ) { this->updateDownstreamMarginCellPointer(riverReachProperties); }

	if ( currentlyElapsedSeconds != (riverReachProperties.getOverallParameters())->getElapsedSeconds() )
	{
		if(inputErosionRateForNextTimeStep.getNumberOfGrainTypes() == 0) { inputErosionRateForNextTimeStep = downstreamMarginCellPointer->regularRiverReachProperties.depositionRate / inputDivisor; }
		inputErosionRate = inputErosionRateForNextTimeStep;
		inputErosionRateForNextTimeStep = downstreamMarginCellPointer->regularRiverReachProperties.depositionRate / inputDivisor;
		currentlyElapsedSeconds = (riverReachProperties.getOverallParameters())->getElapsedSeconds();
	}

	if ( riverReachProperties.isUpstreamMargin() )
	{
		riverReachProperties.regularRiverReachProperties.erosionRate = inputErosionRate;
		(*(riverReachProperties.getDownstreamCellPointer() )).regularRiverReachProperties.depositionRate  = inputErosionRate;
	}
}

void RecirculateSediment::updateDownstreamMarginCellPointer(RiverReachProperties& riverReachProperties)
{
	RiverReachProperties* currentCellPointer = &riverReachProperties;
	while ( !(currentCellPointer->isDownstreamMargin()) )
				{ currentCellPointer = currentCellPointer->getDownstreamCellPointer(); }
	this->downstreamMarginCellPointer = currentCellPointer;
	this->inputCellPointersForDownstreamMargin.clear();
	this->inputCellPointersForDownstreamMargin = this->downstreamMarginCellPointer->getUpstreamCellPointers();
}

}
