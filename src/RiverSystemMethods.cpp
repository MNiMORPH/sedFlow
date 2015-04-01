/*
 * RiverSystemMethods.cpp
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

#include "RiverSystemMethods.h"

namespace SedFlow {

RiverSystemMethods::RiverSystemMethods(RegularRiverSystemMethods regularRiverSystemMethods, AdditionalRiverSystemMethods additionalRiverSystemMethods, RiverSystemProperties& riverSystemProperties, const OverallMethods& overallMethods):
	regularRiverSystemMethods(regularRiverSystemMethods),
	additionalRiverSystemMethods(additionalRiverSystemMethods),
	riverSystemProperties(riverSystemProperties),
	overallMethods(overallMethods)
{}

ConstructionVariables RiverSystemMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RiverSystemMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( regularRiverSystemMethods.createConstructionVariables(riverSystemProperties) );
	result.labelledObjects["regularRiverSystemMethods"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( additionalRiverSystemMethods.createConstructionVariables() );
	result.labelledObjects["additionalRiverSystemMethods"] = constructionVariablesVector;
	return result;
}

void RiverSystemMethods::throwExceptionIfWaterFlowIsNotHighestOrderFlowMethod()
{
	regularRiverSystemMethods.throwExceptionIfWaterFlowIsNotHighestOrderFlowMethod();
}

void RiverSystemMethods::calculateAndModifyChangeRates()
{
	regularRiverSystemMethods.calculateAndModifyChangeRates(riverSystemProperties,overallMethods);
}

double RiverSystemMethods::calculateTimeStep()
{
	return regularRiverSystemMethods.calculateTimeStep(riverSystemProperties);
}

void RiverSystemMethods::calculateAndHandDownChanges()
{
	regularRiverSystemMethods.calculateAndHandDownChanges(riverSystemProperties);
}

void RiverSystemMethods::performAdditionalReachActions()
{
	regularRiverSystemMethods.performAdditionalReachActions();
}

void RiverSystemMethods::applyChanges()
{
	regularRiverSystemMethods.applyChanges(riverSystemProperties);
}
void RiverSystemMethods::updateRegularProperties()
{
	regularRiverSystemMethods.updateBedSlopes(riverSystemProperties, overallMethods);
	regularRiverSystemMethods.updateRegularProperties(riverSystemProperties);
	regularRiverSystemMethods.updateWaterEnergySlopes(riverSystemProperties, overallMethods);
	regularRiverSystemMethods.updateSedimentEnergySlopes(riverSystemProperties, overallMethods);
	regularRiverSystemMethods.updateTaus();
	regularRiverSystemMethods.updateActiveWidths();
}

void RiverSystemMethods::updateAdditionalRiverReachProperties()
{
	regularRiverSystemMethods.updateAdditionalRiverReachProperties();
}

int RiverSystemMethods::getNumberOfCells() const
{
	return riverSystemProperties.regularRiverSystemProperties.numberOfCells;
}

void RiverSystemMethods::updateBedSlopes()
{
	regularRiverSystemMethods.updateBedSlopes(riverSystemProperties,overallMethods);
}

void RiverSystemMethods::updateEnergySlopes()
{
	regularRiverSystemMethods.updateWaterEnergySlopes(riverSystemProperties,overallMethods);
	regularRiverSystemMethods.updateSedimentEnergySlopes(riverSystemProperties,overallMethods);
}

void RiverSystemMethods::updateAdditionalRiverSystemProperties()
{
	additionalRiverSystemMethods.updateAdditionalRiverSystemProperties(riverSystemProperties.regularRiverSystemProperties, riverSystemProperties.overallParameters, regularRiverSystemMethods);
}

void RiverSystemMethods::performAdditionalRiverSystemActions()
{
	additionalRiverSystemMethods.performAdditionalRiverSystemActions(riverSystemProperties, regularRiverSystemMethods);
}

}
