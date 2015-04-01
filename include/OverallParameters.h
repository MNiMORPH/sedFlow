/*
 * OverallParameters.h
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

#ifndef OVERALLPARAMETERS_H_
#define OVERALLPARAMETERS_H_

#include <vector>
#include <utility>

#include "CombinerVariables.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class OverallParameters {
	// This friend declaration is necessary to update the current time step length and the elapsed seconds in SedFlow::performTimeStep().
	friend class SedFlowCore; //friend void SedFlow::performTimeStep();

private:
	double densityWater; //Unit: ???
	double densitySediment; //Unit: ???
	double poreVolumeFraction; //Unit: 1
	double gravityAcceleration; //Unit: m / s^2
	double angleOfReposeInDegree; //Unit: Degree
	double angleOfReposeInRadians; //Unit: Radians
	std::vector<double> fractionalGrainDiameters; //Unit: m
	double currentTimeStepLengthInSeconds;
	double elapsedSeconds;
	double finishSeconds;
	double courantFriedrichsLewyNumber;
	double timeStepThresholdForTerminatingSimulation;
	double timeStepFactor;
#if defined TIMESTEPANALYSIS
	int numberOfTimeStepOutputs;
	int maximumNumberOfTimeStepOutputs;
#endif


public:
	OverallParameters(double densityWater,
					double densitySediment,
					double poreVolumeFraction,
					double gravityAcceleration,
					double angleOfReposeInDegree,
					std::vector<double> fractionalGrainDiameters,
					double currentTimeStepLengthInSeconds,
					double elapsedSeconds,
					double finishSeconds,
					double courantFriedrichsLewyNumber,
					double timeStepThresholdForTerminatingSimulation,
					double timeStepFactor,
					bool updateRegularPropertiesAfterInitialisation,
					bool updateAdditionalRiverReachPropertiesAfterInitialisation,
					bool updateOutputMethodsAfterInitialisation);
	virtual ~OverallParameters();

	bool updateRegularPropertiesAfterInitialisation;
	bool updateAdditionalRiverReachPropertiesAfterInitialisation;
	bool updateOutputMethodsAfterInitialisation;

	ConstructionVariables createConstructionVariables()const;

	inline double getWaterDensity() const { return densityWater; }
	inline double getSedimentDensity() const { return densitySediment; }
	inline double getPoreVolumeFraction() const { return poreVolumeFraction; }
	inline double getGravityAcceleration() const { return gravityAcceleration; }
	inline double getAngleOfReposeInDegree() const { return angleOfReposeInDegree; }
	inline double getAngleOfReposeInRadians() const { return angleOfReposeInRadians; }
	inline std::vector<double> getFractionalGrainDiameters() const {return fractionalGrainDiameters; }
	inline double getCurrentTimeStepLengthInSeconds() const { return currentTimeStepLengthInSeconds; }
	inline double getElapsedSeconds() const { return elapsedSeconds; }
	inline double getFinishSeconds() const { return finishSeconds; }
	inline double getCourantFriedrichsLewyNumber() const { return courantFriedrichsLewyNumber; }
	inline double getTimeStepThresholdForTerminatingSimulation() const { return timeStepThresholdForTerminatingSimulation; }
	inline double getTimeStepFactor() const { return timeStepFactor; }
	#if defined TIMESTEPANALYSIS
	inline bool incrementNumberOfTimeStepOutputsAndCheckIfReachedMaximum() const { return ((++((const_cast<OverallParameters*>(this))->numberOfTimeStepOutputs)) >= this->maximumNumberOfTimeStepOutputs); }
	inline int getMaximumNumberOfTimeStepOutputs() const { return maximumNumberOfTimeStepOutputs; }
	#endif
};

}

#endif /* OVERALLPARAMETERS_H_ */
