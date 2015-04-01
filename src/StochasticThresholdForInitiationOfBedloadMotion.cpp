/*
 * StochasticThresholdForInitiationOfBedloadMotion.cpp
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

#include <stdlib.h>
#include <algorithm>    // std::max
#include <math.h>
#include <limits>

#include "StochasticThresholdForInitiationOfBedloadMotion.h"

#include "CorrectionForBedloadWeightAtSteepSlopes.h"

namespace SedFlow {


StochasticThresholdForInitiationOfBedloadMotion::StochasticThresholdForInitiationOfBedloadMotion(double minimumThresholdValue, double miu, double beta, int seed, double weightForCurrent, double weightForPrevious, double weightForPrePrevious, bool correctionForBedloadWeightAtSteepCounterSlopes, std::vector<double> widthsForSpecialValues, std::vector<double> miuSpecialValues, std::vector<double> betaSpecialValues):
	minimumThresholdValue(minimumThresholdValue),
	miu(miu),
	beta(beta),
	seed(abs(seed)),
	weightForCurrent(weightForCurrent),
	weightForPrevious(weightForPrevious),
	weightForPrePrevious(weightForPrePrevious),
	correctionForBedloadWeightAtSteepCounterSlopes(correctionForBedloadWeightAtSteepCounterSlopes),
	widthsForSpecialValues(widthsForSpecialValues),
	miuSpecialValues(miuSpecialValues),
	betaSpecialValues(betaSpecialValues)
{
	srand(this->seed);
	double weightSum = this->weightForCurrent + this->weightForPrevious + this->weightForPrePrevious;
	this->weightForCurrent /= weightSum;
	this->weightForPrevious /= weightSum;
	this->weightForPrePrevious /= weightSum;

	int specialValuesSize = this->widthsForSpecialValues.size();
	if(specialValuesSize > 0)
	{
		if( (specialValuesSize != this->miuSpecialValues.size()) || (this->miuSpecialValues.size() != this->betaSpecialValues.size()) )
		{
			const char *const sizeErrorMessage = "The variables widthsForSpecialValues, miuSpecialValues and betaSpecialValues need to have all the same length for StochasticThresholdForInitiationOfBedloadMotion.";
			throw(sizeErrorMessage);
		}
		else
		{
			for(std::vector<double>::const_iterator currentWidth = this->widthsForSpecialValues.begin(), currentMiu = this->miuSpecialValues.begin(), currentBeta = this->betaSpecialValues.begin(); currentWidth < this->widthsForSpecialValues.end(); ++currentWidth, ++currentMiu, ++currentBeta)
			{
				this->mapFromWidthToSpecialValues[*currentWidth] = std::pair<double,double>(*currentMiu,*currentBeta);
			}
		}
	}
}

StochasticThresholdForInitiationOfBedloadMotion::StochasticThresholdForInitiationOfBedloadMotion(double minimumThresholdValue, double miu, double beta, int seed, double weightForCurrent, double weightForPrevious, double weightForPrePrevious, bool correctionForBedloadWeightAtSteepCounterSlopes, std::map<double,std::vector<double> > mapFromWidthToValues, std::vector<double> widthsForSpecialValues, std::vector<double> miuSpecialValues, std::vector<double> betaSpecialValues, std::map<double,std::pair<double,double> > mapFromWidthToSpecialValues):
	minimumThresholdValue(minimumThresholdValue),
	miu(miu),
	beta(beta),
	seed(abs(seed)),
	weightForCurrent(weightForCurrent),
	weightForPrevious(weightForPrevious),
	weightForPrePrevious(weightForPrePrevious),
	correctionForBedloadWeightAtSteepCounterSlopes(correctionForBedloadWeightAtSteepCounterSlopes),
	mapFromWidthToValues(mapFromWidthToValues),
	widthsForSpecialValues(widthsForSpecialValues),
	miuSpecialValues(miuSpecialValues),
	betaSpecialValues(betaSpecialValues),
	mapFromWidthToSpecialValues(mapFromWidthToSpecialValues)
{}

StochasticThresholdForInitiationOfBedloadMotion::~StochasticThresholdForInitiationOfBedloadMotion(){}

CalcThresholdForInitiationOfBedloadMotion* StochasticThresholdForInitiationOfBedloadMotion::createCalcThresholdForInitiationOfBedloadMotionPointerCopy() const
{
	CalcThresholdForInitiationOfBedloadMotion* result = new StochasticThresholdForInitiationOfBedloadMotion(minimumThresholdValue,miu,beta,seed,weightForCurrent,weightForPrevious,weightForPrePrevious,correctionForBedloadWeightAtSteepCounterSlopes,mapFromWidthToValues,widthsForSpecialValues,miuSpecialValues,betaSpecialValues,mapFromWidthToSpecialValues);
	return result;
}

ConstructionVariables StochasticThresholdForInitiationOfBedloadMotion::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcThresholdForInitiationOfBedloadMotion;
	result.realisationType = CombinerVariables::typeOfCalculationMethodForBedloadMotionInitiationThresholdToString(CombinerVariables::StochasticThresholdForInitiationOfBedloadMotion);
	std::vector<double> doubleVector;
	doubleVector.push_back(minimumThresholdValue);
	result.labelledDoubles["minimumThresholdValue"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(miu);
	result.labelledDoubles["miu"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(beta);
	result.labelledDoubles["beta"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(seed);
	result.labelledInts["seed"] = intVector;
	doubleVector.clear();
	doubleVector.push_back(weightForCurrent);
	result.labelledDoubles["weightForCurrent"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(weightForPrevious);
	result.labelledDoubles["weightForPrevious"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(weightForPrePrevious);
	result.labelledDoubles["weightForPrePrevious"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(correctionForBedloadWeightAtSteepCounterSlopes);
	result.labelledBools["correctionForBedloadWeightAtSteepCounterSlopes"] = boolVector;
	doubleVector.clear();
	doubleVector = widthsForSpecialValues;
	result.labelledDoubles["widthsForSpecialValues"] = doubleVector;
	doubleVector.clear();
	doubleVector = miuSpecialValues;
	result.labelledDoubles["miuSpecialValues"] = doubleVector;
	doubleVector.clear();
	doubleVector = betaSpecialValues;
	result.labelledDoubles["betaSpecialValues"] = doubleVector;
	return result;
}

double StochasticThresholdForInitiationOfBedloadMotion::calculate (const RegularRiverReachProperties& regularRiverReachProperties) const
{
	return ( (const_cast<StochasticThresholdForInitiationOfBedloadMotion*>(this))->calculateNotConst(regularRiverReachProperties) );
}

double StochasticThresholdForInitiationOfBedloadMotion::calculateNotConst (const RegularRiverReachProperties& regularRiverReachProperties)
{
	double result = std::numeric_limits<double>::quiet_NaN();

	std::map<double,std::vector<double> >::iterator currentValues = mapFromWidthToValues.find(regularRiverReachProperties.activeWidth);
	if(currentValues == mapFromWidthToValues.end())
	{
		double currentMiu (miu);
		double currentBeta (beta);

		std::map<double,std::pair<double,double> >::iterator currentSpecialValues = mapFromWidthToSpecialValues.find(regularRiverReachProperties.activeWidth);
		if(currentSpecialValues != mapFromWidthToSpecialValues.end())
		{
			currentMiu = currentSpecialValues->second.first;
			currentBeta = currentSpecialValues->second.second;
		}

		std::vector<double> tmpValues (6,currentMiu); //miu, currentValue, previousValue, prePreviousValue
		tmpValues.at(0) = std::numeric_limits<double>::quiet_NaN(); //discharge
		tmpValues.at(2) = currentBeta; //beta

		mapFromWidthToValues[regularRiverReachProperties.activeWidth] = tmpValues;
		currentValues = mapFromWidthToValues.find(regularRiverReachProperties.activeWidth);
	}

	/*
	if(regularRiverReachProperties.discharge == currentValues->second.at(0))
	{
		result = currentValues->second.at(3);
	}
	else*/
	{
		currentValues->second.at(0) = regularRiverReachProperties.discharge;
		currentValues->second.at(5) = currentValues->second.at(4); // prePreviousValue = previousValue
		currentValues->second.at(4) = currentValues->second.at(3); // previousValue = currentValue

		double randomNumber = (1.0+rand()) / (2.0+RAND_MAX); //Uniform Distribution
		randomNumber = currentValues->second.at(1) - (currentValues->second.at(2) * log( (-1.0) * log(randomNumber) ) ); //Gumbel Distribution

		result = (randomNumber * weightForCurrent) + (currentValues->second.at(4) * weightForPrevious) + (currentValues->second.at(5) * weightForPrePrevious);

		result = std::max(result,minimumThresholdValue);
		if(correctionForBedloadWeightAtSteepCounterSlopes && regularRiverReachProperties.bedslope < 0.0)
		{
			result = CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedDimensionlessCriticalShearStressAtSteepCounterSlopes(result,(regularRiverReachProperties.getOverallParameters())->getAngleOfReposeInRadians(),regularRiverReachProperties.bedslope);
		}

		currentValues->second.at(3) = result;
	}

	return result;
}

}
