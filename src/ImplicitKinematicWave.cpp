/*
 * ImplicitKinematicWave.cpp
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

#include "ImplicitKinematicWave.h"
#include <math.h>
#include <algorithm>
#include <complex>
#if defined TIMESTEPANALYSIS
	#include <iostream>
	#include <sstream>
	#include <cstring>
#endif
//TODO Delete this debugging line.
#include <iostream>

namespace SedFlow {

ImplicitKinematicWave::ImplicitKinematicWave(double maximumTimeStep, bool checkForCourantFriedrichsLewy, const OverallMethods& overallMethods):
	FlowTypeMethods(CombinerVariables::ImplicitKinematicWave,CombinerVariables::WaterFlowMethodsInGeneral),
	maximumTimeStep(maximumTimeStep),
	checkForCourantFriedrichsLewy(checkForCourantFriedrichsLewy),
	overallMethods(overallMethods)
{}

FlowTypeMethods* ImplicitKinematicWave::createFlowTypeMethodsPointerCopy() const
{
	FlowTypeMethods* result = new ImplicitKinematicWave(this->maximumTimeStep,this->checkForCourantFriedrichsLewy,this->overallMethods);
	return result;
}

ConstructionVariables ImplicitKinematicWave::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;
	result.realisationType = CombinerVariables::typeOfFlowMethodsToString(CombinerVariables::ImplicitKinematicWave);
	std::vector<double> doubleVector;
	doubleVector.push_back(maximumTimeStep);
	result.labelledDoubles["maximumTimeStep"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(checkForCourantFriedrichsLewy);
	result.labelledBools["checkForCourantFriedrichsLewy"] = boolVector;
	return result;
}


void ImplicitKinematicWave::calculateChangeRate (RiverReachProperties& riverReachProperties) const
{
	//This whole block is not necessary and the zero discharge causes trouble in RickenmannBedloadCapacityBasedOnq.
	/*
	if (!(riverReachProperties.isUpstreamMargin()))
	{
		riverReachProperties.regularRiverReachProperties.discharge = 0.0;
		riverReachProperties.regularRiverReachProperties.waterVolumeChangeRate = 0.0;
		riverReachProperties.regularRiverReachProperties.waterVolumeChange = 0.0;
	}
	*/
}

void ImplicitKinematicWave::handDownChangeRate (RiverReachProperties& riverReachProperties) const
{
	// There is nothing to hand down
}

void ImplicitKinematicWave::updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const
{
	//At the moment there are no ChangeRateDependingParameters. This may be overwritten in child classes related to AdditionalParameters.
}

double ImplicitKinematicWave::calculateTimeStep (const RiverSystemProperties& riverSystem) const
{
	double result = maximumTimeStep;
	RiverReachProperties* currentDownstreamCellPointer;
	if(checkForCourantFriedrichsLewy)
	{
		std::vector<double> allCalculatedTimeSteps (1,maximumTimeStep);
		allCalculatedTimeSteps.reserve(riverSystem.regularRiverSystemProperties.cellProperties.size());

		#if defined TIMESTEPANALYSIS
			double thresholdForOutputtingTimeSteps = TIMESTEPANALYSIS;
			std::ostringstream oStringStream;
		#endif

		double timeStepEntry;
		const RiverReachProperties* currentRiverReachProperties;
		#pragma omp parallel for private(currentRiverReachProperties,timeStepEntry) default(shared)
		for(int i = 0; i < (riverSystem.regularRiverSystemProperties.cellProperties.size() - 1); ++i)
		{
			currentRiverReachProperties = &(riverSystem.regularRiverSystemProperties.cellProperties[i]);
			if ( !( currentRiverReachProperties->isMargin() ) )
			{
				currentDownstreamCellPointer = (*currentRiverReachProperties).getDownstreamCellPointer();
				//Check for Courant-Friedrichs-Lewy.
				timeStepEntry = riverSystem.overallParameters.getCourantFriedrichsLewyNumber() * (std::min(((*currentRiverReachProperties).regularRiverReachProperties.length),((*currentDownstreamCellPointer).regularRiverReachProperties.length))) / (*currentRiverReachProperties).regularRiverReachProperties.flowVelocity ;
				#if defined TIMESTEPANALYSIS
					if( timeStepEntry < thresholdForOutputtingTimeSteps )
					{
						oStringStream.str("");
						oStringStream.clear();
						oStringStream << "Time step of " << timeStepEntry << " seconds at ";
						if(currentRiverReachProperties->isMargin())
						{
							if(currentRiverReachProperties->isDownstreamMargin())
							{
								oStringStream << "downstream margin";
							}
							else
							{
								oStringStream << "upstream margin at reach " << currentRiverReachProperties->getDownstreamUserCellID();
							}
						}
						else
						{
							oStringStream << "reach " << currentRiverReachProperties->getUserCellID();
						}
						oStringStream << " due to ImplicitKinematicWave check for Courant-Friedrichs-Lewy." << std::flush;
						#pragma omp critical
						{ std::cout << std::endl << "===============================================================" << std::endl << oStringStream.str() << std::endl << std::flush; }

						if(riverSystem.overallParameters.incrementNumberOfTimeStepOutputsAndCheckIfReachedMaximum())
						{
							oStringStream.str("");
							oStringStream.clear();
							oStringStream << "Number of time step length outputs reached its maximum of " << riverSystem.overallParameters.getMaximumNumberOfTimeStepOutputs() << "." << std::flush;
							char* tmpChar = new char [(oStringStream.str()).size()+1];
							std::strcpy(tmpChar, (oStringStream.str()).c_str());
							const char *const maximumNumberOfTimeStepOutputsErrorMessage = tmpChar;
							#pragma omp critical
							{ throw(maximumNumberOfTimeStepOutputsErrorMessage); }
						}
					}
				#endif
				#pragma omp critical
				{ allCalculatedTimeSteps.push_back(timeStepEntry); }
			}

		}
		result = (*( std::min_element( allCalculatedTimeSteps.begin(), allCalculatedTimeSteps.end() ) ));
	}
	return result;
	//TODO Less Important: Introduce supra-timesteps.
}

void ImplicitKinematicWave::calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const
{
	if (!(riverReachProperties.isUpstreamMargin()))
	{
		PowerLawRelation dischargeAsFunctionOfWaterVolume = overallMethods.flowResistance->dischargeAsPowerLawFunctionOfWaterVolumeInReach(riverReachProperties);

		if ( !(dischargeAsFunctionOfWaterVolume.powerLawCheck()) )
		{
			const char *const firstErrorMessage = "For ImplicitKinematicWave discharge needs to be a power law function of water volume within the reach.";
			throw(firstErrorMessage);
		}

		double upstreamDischargeInputs = 0.0;
		std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
		for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
				{ upstreamDischargeInputs += (*currentUpstreamCellPointer)->regularRiverReachProperties.discharge; }

		double a = upstreamDischargeInputs - dischargeAsFunctionOfWaterVolume.addition();
		double b = dischargeAsFunctionOfWaterVolume.factor();
		double c = dischargeAsFunctionOfWaterVolume.exponent();

		double newWaterVolume = 0.0;
		double currentWaterVolume = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea( riverReachProperties.regularRiverReachProperties.maximumWaterdepth ) * riverReachProperties.regularRiverReachProperties.length;
		double currentTimeStepLength = timeStep; //TODO Less important: Adjust this for supra-timesteps

		if ( a == 0.0 )
		{
			newWaterVolume = pow( ( pow(currentWaterVolume,(1.0-c)) + (b*(c-1.0)*currentTimeStepLength) ) , (1.0/(1.0-c)) ); // p. 29 Eq. (48)

		} else // i.e. (a != 0.0)
		{
			if ( 1.0 >= c || c >= 10.0 )
			{
				const char *const secondErrorMessage = "For ImplicitKinematicWave the exponent c has to be between 1.0 and 10.0.";
				throw(secondErrorMessage);
			}

			double yCircumflex = currentWaterVolume; // p. 30 Eq. (50); y_0 represents the currentWaterVolume; the second term including the dt can be neglected as it tends to zero.
			double AA = 0.5 * (-b) * c * (c-1) * pow( yCircumflex, (c-2) ); // p. 30 Eq. (51); The 0.5 has been found to be needed, when recalculating the Taylors series expansion.
			double BB = (-1.0) * 2.0 * yCircumflex * (c-2) / (c-1); // p. 30 Eq. (51); The (-1) has been found to be needed, when recalculating the Taylors series expansion.
			double CC = ( a / AA ) + ( yCircumflex * yCircumflex * (c-2) / c ); // p. 30 Eq. (51);

			std::complex<double> p1 = 0.5 * ( -BB + sqrt( ( (BB*BB) - (4*CC) ) ) ); // p. 30 Eq. (53)
			std::complex<double> p2 = 0.5 * ( -BB - sqrt( ( (BB*BB) - (4*CC) ) ) ); // p. 30 Eq. (54)

			std::complex<double> X = ( (currentWaterVolume-p1) / (currentWaterVolume-p2) ) * exp( ( AA * (p1-p2) * currentTimeStepLength ) ); // p. 30 Eq. (58)

			std::complex<double> complexNewWaterVolume = ( p1 - (p2*X) ) / ( 1.0 - X ); // p. 30 Eq. (57)
			newWaterVolume = complexNewWaterVolume.real(); // p. 31 Eq. (59)

		}

		riverReachProperties.regularRiverReachProperties.waterVolumeChange = newWaterVolume - currentWaterVolume;
		riverReachProperties.regularRiverReachProperties.waterVolumeChangeRate = riverReachProperties.regularRiverReachProperties.waterVolumeChange / currentTimeStepLength;
		riverReachProperties.regularRiverReachProperties.discharge = upstreamDischargeInputs - riverReachProperties.regularRiverReachProperties.waterVolumeChangeRate;
	}
}

void ImplicitKinematicWave::handDownChange (RiverReachProperties& riverReachProperties) const
{
	//There is nothing to hand down.
}

void ImplicitKinematicWave::applyChange (RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Avoid nonsense updates, when supra-timesteps are active.
	if (!(riverReachProperties.isUpstreamMargin()))
	{
	double newVolume = (riverReachProperties.regularRiverReachProperties.length * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(riverReachProperties.regularRiverReachProperties.maximumWaterdepth)) + riverReachProperties.regularRiverReachProperties.waterVolumeChange;
	riverReachProperties.regularRiverReachProperties.maximumWaterdepth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertCrossSectionalAreaIntoMaximumFlowDepth( (newVolume / riverReachProperties.regularRiverReachProperties.length));
	}
}

void ImplicitKinematicWave::updateOtherParameters (RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Avoid nonsense updates, when supra-timesteps are active.
	if (!(riverReachProperties.isUpstreamMargin()))
	{
	riverReachProperties.regularRiverReachProperties.flowVelocity = (overallMethods.flowResistance->calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(riverReachProperties)).second;
	}
}

void ImplicitKinematicWave::handDownOtherParameters (RiverReachProperties& riverReachProperties) const
{
	riverReachProperties.regularRiverReachProperties.waterEnergyslope = overallMethods.waterEnergySlopeCalculationMethod->calculate(riverReachProperties);

	if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnWaterEnergyslope() )
	{
		std::pair<double,double> reducedAndUnreducedSlope = overallMethods.sedimentEnergySlopeCalculationMethod->calculateReducedAndUnreducedSedimentEnergyslope(riverReachProperties);
		riverReachProperties.regularRiverReachProperties.sedimentEnergyslope = reducedAndUnreducedSlope.first;
		riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope = reducedAndUnreducedSlope.second;
	}
}

}
