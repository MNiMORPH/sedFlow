/*
 * RiverReachMethods.cpp
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

#include "RiverReachMethods.h"

namespace SedFlow {

RiverReachMethods::RiverReachMethods(const OverallMethods* overallMethods, const RegularRiverReachMethods& regularRiverReachMethods, const AdditionalRiverReachMethods& additionalRiverReachMethods, RiverReachProperties& riverReachProperties):
	overallMethods(overallMethods),
	regularRiverReachMethods(regularRiverReachMethods),
	additionalRiverReachMethods(additionalRiverReachMethods),
	riverReachProperties(&riverReachProperties)
{}

RiverReachMethods::RiverReachMethods(const RiverReachMethods& toCopy):
	overallMethods(toCopy.overallMethods),
	regularRiverReachMethods(toCopy.regularRiverReachMethods),
	additionalRiverReachMethods(toCopy.additionalRiverReachMethods),
	riverReachProperties(toCopy.riverReachProperties)
{}

ConstructionVariables RiverReachMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RiverReachMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( regularRiverReachMethods.createConstructionVariables() );
	result.labelledObjects["regularRiverReachMethods"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( additionalRiverReachMethods.createConstructionVariables() );
	result.labelledObjects["additionalRiverReachMethods"] = constructionVariablesVector;
	std::vector<int> intVector;
	intVector.push_back( riverReachProperties->getCellID() );
	result.labelledInts["internalCellID"] = intVector;
	return result;
}

void RiverReachMethods::calculateFlowVelocityAndWaterdepth()const
{
	regularRiverReachMethods.calculateFlowVelocityAndWaterdepth(*riverReachProperties, *overallMethods);
}

double RiverReachMethods::getCurrentDarcyWeisbachFrictionFactorF()const
{
	return regularRiverReachMethods.getCurrentDarcyWeisbachFrictionFactorF(*riverReachProperties, *overallMethods);
}

void RiverReachMethods::calculateTau()const
{
	regularRiverReachMethods.calculateTau(*riverReachProperties, *overallMethods);
}

void RiverReachMethods::calculateActiveWidth(const SedimentFlowMethods& sedimentFlowMethods)const
{
	regularRiverReachMethods.calculateActiveWidth(*riverReachProperties, sedimentFlowMethods, *overallMethods);
}


void RiverReachMethods::updateAdditionalRiverReachProperties()
{
	additionalRiverReachMethods.updateAdditionalRiverReachProperties(riverReachProperties->regularRiverReachProperties, riverReachProperties->geometricalChannelBehaviour, regularRiverReachMethods);
}

void RiverReachMethods::performAdditionalRiverReachActions()
{
	additionalRiverReachMethods.performTypeSpecificActions(*riverReachProperties, regularRiverReachMethods);
}


}
