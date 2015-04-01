/*
 * RegularRiverReachMethods.cpp
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

#include "RegularRiverReachMethods.h"

namespace SedFlow {

RegularRiverReachMethods::RegularRiverReachMethods():
		tauCalculationMethod(NULL),
		activeWidthCalculationMethod(NULL)
{}

RegularRiverReachMethods::RegularRiverReachMethods(const CalcTau* tauCalculationMethod, const CalcActiveWidth* activeWidthCalculationMethod):
	tauCalculationMethod( tauCalculationMethod->createCalcTauPointerCopy() ),
	activeWidthCalculationMethod( activeWidthCalculationMethod->createCalcActiveWidthPointerCopy() )
{}

RegularRiverReachMethods::RegularRiverReachMethods(const RegularRiverReachMethods& toCopy):
	tauCalculationMethod( toCopy.tauCalculationMethod->createCalcTauPointerCopy() ),
	activeWidthCalculationMethod( toCopy.activeWidthCalculationMethod->createCalcActiveWidthPointerCopy() )
{}

RegularRiverReachMethods::~RegularRiverReachMethods()
{
	delete tauCalculationMethod;
	delete activeWidthCalculationMethod;
}

ConstructionVariables RegularRiverReachMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RegularRiverReachMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( tauCalculationMethod->createConstructionVariables() );
	result.labelledObjects["tauCalculationMethod"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( activeWidthCalculationMethod->createConstructionVariables() );
	result.labelledObjects["activeWidthCalculationMethod"] = constructionVariablesVector;
	return result;
}

void RegularRiverReachMethods::calculateFlowVelocityAndWaterdepth(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	overallMethods.flowResistance->calculateAndUpdateFlowDepthAndFlowVelocityUsingDischargeAsInput(riverReachProperties);
}

void RegularRiverReachMethods::calculateTau(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	riverReachProperties.regularRiverReachProperties.bedShearStress = (*tauCalculationMethod).calculate(riverReachProperties,overallMethods.flowResistance);
}

double RegularRiverReachMethods::getCurrentDarcyWeisbachFrictionFactorF(const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	return overallMethods.flowResistance->returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth,riverReachProperties);
}

void RegularRiverReachMethods::calculateActiveWidth(RiverReachProperties& riverReachProperties, const SedimentFlowMethods& sedimentFlowMethods, const OverallMethods& overallMethods) const
{
	riverReachProperties.regularRiverReachProperties.activeWidth = (*activeWidthCalculationMethod).calculateActiveWidth(riverReachProperties, sedimentFlowMethods, overallMethods);
}
}
