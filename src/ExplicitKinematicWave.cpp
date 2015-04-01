/*
 * ExplicitKinematicWave.cpp
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

#include "ExplicitKinematicWave.h"

#include <algorithm>
#include <math.h>
#if defined TIMESTEPANALYSIS
	#include <iostream>
	#include <sstream>
	#include <cstring>
#endif
//TODO Delete this debugging line.
#include <iostream>

namespace SedFlow {

ExplicitKinematicWave::ExplicitKinematicWave(const OverallMethods& overallMethods):
	FlowTypeMethods(CombinerVariables::ExplicitKinematicWave,CombinerVariables::WaterFlowMethodsInGeneral),
	overallMethods(overallMethods)
{}

FlowTypeMethods* ExplicitKinematicWave::createFlowTypeMethodsPointerCopy() const
{
	FlowTypeMethods* result = new ExplicitKinematicWave(this->overallMethods);
	return result;
}

ConstructionVariables ExplicitKinematicWave::createConstructionVariables() const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;
	result.realisationType = CombinerVariables::typeOfFlowMethodsToString(CombinerVariables::ExplicitKinematicWave);
	return result;
}

//TODO Less Important: Check why this method gets into trouble, when there are more than two upstream cell pointers.
void ExplicitKinematicWave::calculateChangeRate (RiverReachProperties& riverReachProperties) const
{
	if ( !(riverReachProperties.isMargin()) )
	{
		double cumulativeInputDischarges = 0.0;
		std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
		for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
				{ cumulativeInputDischarges += (*(*currentUpstreamCellPointer)).regularRiverReachProperties.discharge; }
		riverReachProperties.regularRiverReachProperties.waterVolumeChangeRate = cumulativeInputDischarges - riverReachProperties.regularRiverReachProperties.discharge;
	}
}

void ExplicitKinematicWave::handDownChangeRate (RiverReachProperties& riverReachProperties) const
{
	// There is nothing to hand down
}

void ExplicitKinematicWave::updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const
{
	//At the moment there are no ChangeRateDependingParameters. This may be overwritten in child classes related to AdditionalParameters.
}

double ExplicitKinematicWave::calculateTimeStep (const RiverSystemProperties& riverSystem) const
{
	std::vector<double> allCalculatedTimeSteps;
	allCalculatedTimeSteps.reserve( ( 3 * riverSystem.regularRiverSystemProperties.cellProperties.size() ) );

	RiverReachProperties* currentDownstreamCellPointer;
	//All following variables are for checking not to create too large RelativeTwoCellDischargeGradientChange
	double tempTimeStep;
	double currentDischargeGradient;
	double currentLocalVolume;
	double currentDownstreamVolume;
	double localVolumeChangeRate;
	double downstreamVolumeChangeRate;
	double localNewVolume;
	double downstreamNewVolume;
	double localNewWaterdepth;
	double downstreamNewWaterdepth;
	double localNewDischarge;
	double downstreamNewDischarge;
	double newDischargeGradient;
	double dischargeGradientChange;

#if defined TIMESTEPANALYSIS
	double thresholdForOutputtingTimeSteps = TIMESTEPANALYSIS;
	std::ostringstream oStringStream;
#endif

	double timeStepEntry;
	const RiverReachProperties* currentRiverReachProperties;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP ExplicitKinematicWave::calculateTimeStep" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentRiverReachProperties,timeStepEntry,currentDownstreamCellPointer,tempTimeStep,currentDischargeGradient,currentLocalVolume,currentDownstreamVolume,localVolumeChangeRate,downstreamVolumeChangeRate,localNewVolume,downstreamNewVolume,localNewWaterdepth,downstreamNewWaterdepth,localNewDischarge,downstreamNewDischarge,newDischargeGradient,dischargeGradientChange) default(shared)
	for(int i = 0; i < (riverSystem.regularRiverSystemProperties.cellProperties.size() - 1); ++i)
	{
		currentRiverReachProperties = &(riverSystem.regularRiverSystemProperties.cellProperties[i]);
		if ( !( currentRiverReachProperties->isMargin() ) )
		{
			currentDownstreamCellPointer = (*currentRiverReachProperties).getDownstreamCellPointer();
			localVolumeChangeRate = (*currentRiverReachProperties).regularRiverReachProperties.waterVolumeChangeRate;
			downstreamVolumeChangeRate = (*currentDownstreamCellPointer).regularRiverReachProperties.waterVolumeChangeRate;

			//Check for not emptying complete cell.
			if ( localVolumeChangeRate < 0.0)
			{
				timeStepEntry = ( ((*currentRiverReachProperties).geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea((*currentRiverReachProperties).regularRiverReachProperties.maximumWaterdepth)) * (*currentRiverReachProperties).regularRiverReachProperties.length ) * 0.75  / fabs(localVolumeChangeRate);
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
						oStringStream << " due to ExplicitKinematicWave check for not emptying complete cell." << std::flush;
						#pragma omp critical
						{ std::cout << std::endl << "===============================================================" << oStringStream.str() << std::endl << std::flush; }

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
					oStringStream << " due to ExplicitKinematicWave check for Courant-Friedrichs-Lewy." << std::flush;
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

			// TODO Think about the necessity of and alternatives for the following lines!
			// This To Do should especially consider the case of (currentDischargeGradient ~ 0.0) i.e. two succeeding cells have very similar discharge.
			// This very common (?) case would produce extremely small time steps.
			/*
			//Check for not creating too large RelativeTwoCellDischargeGradientChange
			tempTimeStep = allCalculatedTimeSteps.back();
			currentDischargeGradient = (currentRiverReachProperties->regularRiverReachProperties.discharge - currentDownstreamCellPointer->regularRiverReachProperties.discharge) / currentRiverReachProperties->regularRiverReachProperties.length;
			currentLocalVolume = currentRiverReachProperties->regularRiverReachProperties.length * currentRiverReachProperties->geometricalChannelBehaviour->alluviumChannel->basicGeometry.convertMaximumFlowDepthIntoCrossSectionalArea(currentRiverReachProperties->regularRiverReachProperties.maximumWaterdepth);
			currentDownstreamVolume = currentDownstreamCellPointer->regularRiverReachProperties.length * currentDownstreamCellPointer->geometricalChannelBehaviour->alluviumChannel->basicGeometry.convertMaximumFlowDepthIntoCrossSectionalArea(currentDownstreamCellPointer->regularRiverReachProperties.maximumWaterdepth);
			do {
				localNewVolume = currentLocalVolume + localVolumeChangeRate * tempTimeStep;
				downstreamNewVolume = currentDownstreamVolume + downstreamVolumeChangeRate * tempTimeStep;
				localNewWaterdepth = (*currentRiverReachProperties).geometricalChannelBehaviour->alluviumChannel->basicGeometry.convertCrossSectionalAreaIntoMaximumFlowDepth( (localNewVolume / (*currentRiverReachProperties).regularRiverReachProperties.length));
				downstreamNewWaterdepth = (*currentDownstreamCellPointer).geometricalChannelBehaviour->alluviumChannel->basicGeometry.convertCrossSectionalAreaIntoMaximumFlowDepth( (downstreamNewVolume / (*currentDownstreamCellPointer).regularRiverReachProperties.length));
				localNewDischarge = (overallMethods.flowResistance->calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(localNewWaterdepth, *currentRiverReachProperties)).first;
				downstreamNewDischarge = (overallMethods.flowResistance->calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(downstreamNewWaterdepth, *currentDownstreamCellPointer)).first;
				newDischargeGradient = (localNewDischarge - downstreamNewDischarge) / currentRiverReachProperties->regularRiverReachProperties.length;
				dischargeGradientChange = newDischargeGradient - currentDischargeGradient;

				tempTimeStep /= 2.0;
			} while (fabs((dischargeGradientChange/currentDischargeGradient)) > maximumRelativeTwoCellDischargeGradientChange);

			timeStepEntry = tempTimeStep * 2.0;
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
						oStringStream << " due to ExplicitKinematicWave check for not creating too large RelativeTwoCellDischargeGradientChange." << std::flush;
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
			*/
		}

	}
	return *( std::min_element( allCalculatedTimeSteps.begin(), allCalculatedTimeSteps.end() ) );
}

void ExplicitKinematicWave::calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const
{
	riverReachProperties.regularRiverReachProperties.waterVolumeChange = riverReachProperties.regularRiverReachProperties.waterVolumeChangeRate * timeStep;
}

void ExplicitKinematicWave::handDownChange (RiverReachProperties& riverReachProperties) const
{
	//There is nothing to hand down.
}

void ExplicitKinematicWave::applyChange (RiverReachProperties& riverReachProperties) const
{
	if (!(riverReachProperties.isUpstreamMargin()))
	{
	double newVolume = (riverReachProperties.regularRiverReachProperties.length * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(riverReachProperties.regularRiverReachProperties.maximumWaterdepth)) + riverReachProperties.regularRiverReachProperties.waterVolumeChange;
	riverReachProperties.regularRiverReachProperties.maximumWaterdepth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertCrossSectionalAreaIntoMaximumFlowDepth( (newVolume / riverReachProperties.regularRiverReachProperties.length));
	}
}

void ExplicitKinematicWave::updateOtherParameters (RiverReachProperties& riverReachProperties) const
{
	if (!(riverReachProperties.isUpstreamMargin()))
	{
	overallMethods.flowResistance->calculateAndUpdateDischargeAndFlowVelocityUsingFlowDepthAsInput(riverReachProperties);
	}
}

void ExplicitKinematicWave::handDownOtherParameters (RiverReachProperties& riverReachProperties) const
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
