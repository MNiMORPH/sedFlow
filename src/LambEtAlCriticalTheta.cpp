/*
 * LambEtAlCriticalTheta.cpp
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

#include "LambEtAlCriticalTheta.h"

#include <math.h>
#include <algorithm>

#include "CorrectionForBedloadWeightAtSteepSlopes.h"

namespace SedFlow {

LambEtAlCriticalTheta::LambEtAlCriticalTheta(double minimumCriticalDimensionlessShearStress, bool correctionForBedloadWeightAtSteepCounterSlopes):
	minimumCriticalDimensionlessShearStress(minimumCriticalDimensionlessShearStress),
	correctionForBedloadWeightAtSteepCounterSlopes(correctionForBedloadWeightAtSteepCounterSlopes)
{
	if(minimumCriticalDimensionlessShearStress < 0.0)
	{
		const char *const negativeMinimumForCriticalDimensionlessShearStressErrorMessage = "Negative values are not allowed for minimumCriticalDimensionlessShearStress in LambEtAlCriticalTheta.";
		throw(negativeMinimumForCriticalDimensionlessShearStressErrorMessage);
	}
}

LambEtAlCriticalTheta::~LambEtAlCriticalTheta() {}

CalcThresholdForInitiationOfBedloadMotion* LambEtAlCriticalTheta::createCalcThresholdForInitiationOfBedloadMotionPointerCopy() const
{
	CalcThresholdForInitiationOfBedloadMotion* result = new LambEtAlCriticalTheta(this->minimumCriticalDimensionlessShearStress,this->correctionForBedloadWeightAtSteepCounterSlopes);
	return result;
}

ConstructionVariables LambEtAlCriticalTheta::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcThresholdForInitiationOfBedloadMotion;
	result.realisationType = CombinerVariables::typeOfCalculationMethodForBedloadMotionInitiationThresholdToString(CombinerVariables::LambEtAlCriticalTheta);
	std::vector<double> doubleVector;
	doubleVector.push_back(minimumCriticalDimensionlessShearStress);
	result.labelledDoubles["minimumCriticalDimensionlessShearStress"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(correctionForBedloadWeightAtSteepCounterSlopes);
	result.labelledBools["correctionForBedloadWeightAtSteepCounterSlopes"] = boolVector;
	return result;
}

double LambEtAlCriticalTheta::calculate (const RegularRiverReachProperties& regularRiverReachProperties) const
{
	double result = minimumCriticalDimensionlessShearStress;
	if(regularRiverReachProperties.bedslope >= 0.0)
	{
		result = std::max( result , (0.15 * pow(regularRiverReachProperties.bedslope,0.25)) );
	}
	else
	{
		if(correctionForBedloadWeightAtSteepCounterSlopes)
		{
			result = CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedDimensionlessCriticalShearStressAtSteepCounterSlopes(result,(regularRiverReachProperties.getOverallParameters())->getAngleOfReposeInRadians(),regularRiverReachProperties.bedslope);
		}
	}
	return result;
}

}
