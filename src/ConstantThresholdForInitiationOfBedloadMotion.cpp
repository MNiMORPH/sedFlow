/*
 * ConstantThresholdForInitiationOfBedloadMotion.cpp
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

#include "ConstantThresholdForInitiationOfBedloadMotion.h"

#include "CorrectionForBedloadWeightAtSteepSlopes.h"

namespace SedFlow {

ConstantThresholdForInitiationOfBedloadMotion::ConstantThresholdForInitiationOfBedloadMotion(double constantThreshold, bool correctionForBedloadWeightAtSteepCounterSlopes):
	constantThreshold(constantThreshold),
	correctionForBedloadWeightAtSteepCounterSlopes(correctionForBedloadWeightAtSteepCounterSlopes)
{}

ConstantThresholdForInitiationOfBedloadMotion::~ConstantThresholdForInitiationOfBedloadMotion(){}

CalcThresholdForInitiationOfBedloadMotion* ConstantThresholdForInitiationOfBedloadMotion::createCalcThresholdForInitiationOfBedloadMotionPointerCopy() const
{
	CalcThresholdForInitiationOfBedloadMotion* result = new ConstantThresholdForInitiationOfBedloadMotion(constantThreshold,correctionForBedloadWeightAtSteepCounterSlopes);
	return result;
}

ConstructionVariables ConstantThresholdForInitiationOfBedloadMotion::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcThresholdForInitiationOfBedloadMotion;
	result.realisationType = CombinerVariables::typeOfCalculationMethodForBedloadMotionInitiationThresholdToString(CombinerVariables::ConstantThresholdForInitiationOfBedloadMotion);
	std::vector<double> doubleVector;
	doubleVector.push_back(constantThreshold);
	result.labelledDoubles["constantThreshold"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(correctionForBedloadWeightAtSteepCounterSlopes);
	result.labelledBools["correctionForBedloadWeightAtSteepCounterSlopes"] = boolVector;
	return result;
}

double ConstantThresholdForInitiationOfBedloadMotion::calculate (const RegularRiverReachProperties& regularRiverReachProperties) const
{
	double result = constantThreshold;
	if(correctionForBedloadWeightAtSteepCounterSlopes && regularRiverReachProperties.bedslope < 0.0)
	{
		result = CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedDimensionlessCriticalShearStressAtSteepCounterSlopes(result,(regularRiverReachProperties.getOverallParameters())->getAngleOfReposeInRadians(),regularRiverReachProperties.bedslope);
	}
	return result;
}

}
