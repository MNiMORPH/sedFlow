/*
 * OverallMethods.cpp
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

#include "OverallMethods.h"

namespace SedFlow {

OverallMethods::OverallMethods():
	bedSlopeCalculationMethod(NULL),
	waterEnergySlopeCalculationMethod(NULL),
	sedimentEnergySlopeCalculationMethod(NULL),
	flowResistance(NULL),
	bedloadVelocityCalculationMethod(NULL)
{}

OverallMethods::OverallMethods(const CalcGradient* bedslopeCalculationMethod, const CalcGradient* waterEnergyslopeCalculationMethod, const CalcGradient* sedimentEnergyslopeCalculationMethod, const FlowResistance* flowResistance, const CalcBedloadVelocity* bedloadVelocityCalculationMethod):
	bedSlopeCalculationMethod(bedslopeCalculationMethod->createCalcGradientMethodPointerCopy()),
	waterEnergySlopeCalculationMethod(waterEnergyslopeCalculationMethod->createCalcGradientMethodPointerCopy()),
	sedimentEnergySlopeCalculationMethod(sedimentEnergyslopeCalculationMethod->createCalcGradientMethodPointerCopy()),
	flowResistance(flowResistance->createFlowResistancePointerCopy()),
	bedloadVelocityCalculationMethod(bedloadVelocityCalculationMethod->createCalcBedloadVelocityPointerCopy())
{}

OverallMethods::~OverallMethods()
{
	delete bedSlopeCalculationMethod;
	delete waterEnergySlopeCalculationMethod;
	delete sedimentEnergySlopeCalculationMethod;
	delete flowResistance;
	delete bedloadVelocityCalculationMethod;
}

ConstructionVariables OverallMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OverallMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( bedSlopeCalculationMethod->createConstructionVariables() );
	result.labelledObjects["bedSlopeCalculationMethod"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( waterEnergySlopeCalculationMethod->createConstructionVariables() );
	result.labelledObjects["waterEnergySlopeCalculationMethod"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( sedimentEnergySlopeCalculationMethod->createConstructionVariables() );
	result.labelledObjects["sedimentEnergySlopeCalculationMethod"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( flowResistance->createConstructionVariables() );
	result.labelledObjects["flowResistance"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( bedloadVelocityCalculationMethod->createConstructionVariables() );
	result.labelledObjects["bedloadVelocityCalculationMethod"] = constructionVariablesVector;
	return result;
}

}
