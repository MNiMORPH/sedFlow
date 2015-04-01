/*
 * ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.cpp
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

#include "ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.h"

#include <math.h>
#include <iostream>
#include <cstdlib>

namespace SedFlow {

ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable(CalcGradient* simpleWaterEnergyslopeCalculationMethod, double stressPartitioningExponent, bool calculationBasedOnqInsteadOfh, double maximumFroudeNumber, bool ensureMinimumInputSlope, double minimumInputSlope):
	ReducedWaterEnergyslope(stressPartitioningExponent, calculationBasedOnqInsteadOfh, maximumFroudeNumber, ensureMinimumInputSlope, minimumInputSlope),
	simpleWaterEnergyslopeCalculationMethod(simpleWaterEnergyslopeCalculationMethod->createCalcGradientMethodPointerCopy())
{
	this->typeOfGradientCalculationMethod = CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable;
	this->isDependentOnWaterEnergyslope = false;
}

ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::~ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable()
{
	delete simpleWaterEnergyslopeCalculationMethod;
}

CalcGradient* ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::createCalcGradientMethodPointerCopy() const
{
	CalcGradient* result = new ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable(this->simpleWaterEnergyslopeCalculationMethod, this->stressPartitioningExponent, this->calculationBasedOnqInsteadOfh, this->maximumFroudeNumber, this->ensureMinimumInputSlope, this->minimumInputSlope);
	return result;
}

ConstructionVariables ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable);
	std::vector<double> doubleVector;
	doubleVector.push_back(stressPartitioningExponent);
	result.labelledDoubles["stressPartitioningExponent"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(calculationBasedOnqInsteadOfh);
	result.labelledBools["calculationBasedOnqInsteadOfh"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(maximumFroudeNumber);
	result.labelledDoubles["maximumFroudeNumber"] = doubleVector;
	boolVector.clear();
	boolVector.push_back(ensureMinimumInputSlope);
	result.labelledBools["ensureMinimumInputSlope"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(minimumInputSlope);
	result.labelledDoubles["minimumInputSlope"] = doubleVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( simpleWaterEnergyslopeCalculationMethod->createConstructionVariables() );
	result.labelledObjects["simpleWaterEnergyslopeCalculationMethod"] = constructionVariablesVector;
	return result;
}

double ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::calculate (const RiverReachProperties& riverReachProperties) const
{
	return convertSimpleToReducedSlope( simpleWaterEnergyslopeCalculationMethod->calculate(riverReachProperties), riverReachProperties );
}

double ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::calculate (const RiverSystemProperties& river, int cellID) const
{
	return convertSimpleToReducedSlope( simpleWaterEnergyslopeCalculationMethod->calculate(river,cellID), (river.regularRiverSystemProperties.cellProperties.at(cellID)) );
}

std::vector<double> ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::calculate (const RiverSystemProperties& river) const
{
	return convertSimpleToReducedSlope(simpleWaterEnergyslopeCalculationMethod->calculate(river), river);
}

std::pair<double,double> ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::calculateReducedAndUnreducedSedimentEnergyslope (const RiverReachProperties& riverReachProperties) const
{
	double simpleSlope = simpleWaterEnergyslopeCalculationMethod->calculate(riverReachProperties);
	return std::pair<double,double>(convertSimpleToReducedSlope(simpleSlope,riverReachProperties),simpleSlope);
}

std::pair<double,double> ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::calculateReducedAndUnreducedSedimentEnergyslope (const RiverSystemProperties& river, int cellID) const
{
	double simpleSlope = simpleWaterEnergyslopeCalculationMethod->calculate(river,cellID);
	return std::pair<double,double>(convertSimpleToReducedSlope( simpleSlope, (river.regularRiverSystemProperties.cellProperties.at(cellID)) ),simpleSlope);
}

std::pair< std::vector<double>, std::vector<double> > ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable::calculateReducedAndUnreducedSedimentEnergyslope (const RiverSystemProperties& river) const
{
	std::vector<double> simpleSlopes = simpleWaterEnergyslopeCalculationMethod->calculate(river);
	return std::pair< std::vector<double>, std::vector<double> > (convertSimpleToReducedSlope(simpleSlopes, river),simpleSlopes);
}


}
