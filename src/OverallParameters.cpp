/*
 * OverallParameters.cpp
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

#include "OverallParameters.h"

#include <math.h>
#define _USE_MATH_DEFINES

namespace SedFlow {

OverallParameters::OverallParameters(double densityWater, double densitySediment, double poreVolumeFraction, double gravityAcceleration, double angleOfReposeInDegree, std::vector<double> fractionalGrainDiameters, double currentTimeStepLengthInSeconds, double elapsedSeconds, double finishSeconds, double courantFriedrichsLewyNumber, double timeStepThresholdForTerminatingSimulation, double timeStepFactor, bool updateRegularPropertiesAfterInitialisation, bool updateAdditionalRiverReachPropertiesAfterInitialisation, bool updateOutputMethodsAfterInitialisation):
	densityWater(densityWater),
	densitySediment(densitySediment),
	poreVolumeFraction(poreVolumeFraction),
	gravityAcceleration(gravityAcceleration),
	angleOfReposeInDegree(angleOfReposeInDegree),
	fractionalGrainDiameters(fractionalGrainDiameters),
	currentTimeStepLengthInSeconds(currentTimeStepLengthInSeconds),
	elapsedSeconds(elapsedSeconds),
	finishSeconds(finishSeconds),
	courantFriedrichsLewyNumber(courantFriedrichsLewyNumber),
	timeStepThresholdForTerminatingSimulation(timeStepThresholdForTerminatingSimulation),
	timeStepFactor(timeStepFactor),
#if defined TIMESTEPANALYSIS
	numberOfTimeStepOutputs(0),
	maximumNumberOfTimeStepOutputs(1000),
#endif
	updateRegularPropertiesAfterInitialisation(updateRegularPropertiesAfterInitialisation),
	updateAdditionalRiverReachPropertiesAfterInitialisation(updateRegularPropertiesAfterInitialisation),
	updateOutputMethodsAfterInitialisation(updateRegularPropertiesAfterInitialisation)
{
	this->angleOfReposeInRadians = this->angleOfReposeInDegree * M_PI / 180.0;
}

OverallParameters::~OverallParameters() {}

ConstructionVariables OverallParameters::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OverallParameters;
	std::vector<double> doubleVector;
	doubleVector.push_back(densityWater);
	result.labelledDoubles["densityWater"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(densitySediment);
	result.labelledDoubles["densitySediment"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(poreVolumeFraction);
	result.labelledDoubles["poreVolumeFraction"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(gravityAcceleration);
	result.labelledDoubles["gravityAcceleration"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(angleOfReposeInDegree);
	result.labelledDoubles["angleOfReposeInDegree"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(angleOfReposeInRadians);
	result.labelledDoubles["angleOfReposeInRadians"] = doubleVector;
	doubleVector.clear();
	doubleVector.reserve( fractionalGrainDiameters.size() );
	for(std::vector<double>::const_iterator iterator = fractionalGrainDiameters.begin(); iterator < fractionalGrainDiameters.end(); ++iterator)
			{ doubleVector.push_back(*iterator); }
	result.labelledDoubles["fractionalGrainDiameters"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(currentTimeStepLengthInSeconds);
	result.labelledDoubles["currentTimeStepLengthInSeconds"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(elapsedSeconds);
	result.labelledDoubles["elapsedSeconds"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(finishSeconds);
	result.labelledDoubles["finishSeconds"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(courantFriedrichsLewyNumber);
	result.labelledDoubles["courantFriedrichsLewyNumber"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(timeStepThresholdForTerminatingSimulation);
	result.labelledDoubles["timeStepThresholdForTerminatingSimulation"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(timeStepFactor);
	result.labelledDoubles["timeStepFactor"] = doubleVector;
#if defined TIMESTEPANALYSIS
	std::vector<int> intVector;
	intVector.push_back(numberOfTimeStepOutputs);
	result.labelledInts["numberOfTimeStepOutputs"] = intVector;
	intVector.clear();
	intVector.push_back(maximumNumberOfTimeStepOutputs);
	result.labelledInts["maximumNumberOfTimeStepOutputs"] = intVector;
#endif
	std::vector<bool> boolVector;
	boolVector.push_back(updateRegularPropertiesAfterInitialisation);
	result.labelledBools["updateRegularPropertiesAfterInitialisation"] = boolVector;
	boolVector.clear();
	boolVector.push_back(updateAdditionalRiverReachPropertiesAfterInitialisation);
	result.labelledBools["updateAdditionalRiverReachPropertiesAfterInitialisation"] = boolVector;
	boolVector.clear();
	boolVector.push_back(updateOutputMethodsAfterInitialisation);
	result.labelledBools["updateOutputMethodsAfterInitialisation"] = boolVector;
	boolVector.clear();
	return result;
}

}
