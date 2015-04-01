/*
 * SedimentFlowTypeMethods.cpp
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

#include <vector>
#include <math.h>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
#include <limits>
#if defined TIMESTEPANALYSIS
	#include <iostream>
	#include <sstream>
	#include <cstring>
#endif

#include "SedimentFlowTypeMethods.h"
#include "RiverReachMethods.h"
#include "FlowTypeMethods.h"

namespace SedFlow {

SedimentFlowTypeMethods::SedimentFlowTypeMethods(double maximumFractionOfActiveLayerToBeEroded, bool preventZeroOrNegativeBedSlopes, double maximumRelativeTwoCellBedSlopeChange, const OverallMethods& overallMethods):
	maximumFractionOfActiveLayerToBeEroded(maximumFractionOfActiveLayerToBeEroded),
	preventZeroOrNegativeBedSlopes(preventZeroOrNegativeBedSlopes),
	maximumRelativeTwoCellBedSlopeChange(maximumRelativeTwoCellBedSlopeChange),
	overallMethods(overallMethods)
{}

void SedimentFlowTypeMethods::handDownChangeRate (RiverReachProperties& riverReachProperties) const
{
	riverReachProperties.regularRiverReachProperties.depositionRate.zeroFractions();
	std::vector<RiverReachProperties*> upstreamCells = riverReachProperties.getUpstreamCellPointers();
	for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCell = upstreamCells.begin(); currentUpstreamCell < upstreamCells.end(); ++currentUpstreamCell)
	{
		riverReachProperties.regularRiverReachProperties.depositionRate += (*currentUpstreamCell)->regularRiverReachProperties.erosionRate;
	}
}

void SedimentFlowTypeMethods::updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const
{
	//For Sediment flow there are no change rate depending parameters.
}

double SedimentFlowTypeMethods::calculateTimeStep (const RiverSystemProperties& riverSystem) const
{
	//TODO Less Important: Check for not too intense update of Channel Geometry.
	std::vector<double> allCalculatedTimeSteps (1,(std::numeric_limits<double>::max() - 4.0));
	allCalculatedTimeSteps.reserve( ( (3 * riverSystem.regularRiverSystemProperties.cellProperties.size()) + 1 ) );

	RiverReachProperties* currentDownstreamCellPointer;
	double currentLocalActiveOverallVolume;
	double currentLocalSedimentVelocity;
	std::pair<bool,double> localLinearConversion;
	std::pair<bool,double> downstreamLinearConversion;
	double bedslopeChangeRate;
	double bedslopeChange;
	double currentBedslope;
	double tempTimeStep;
	std::vector<double> localOverallDepositionRate;
	std::vector<double> localOverallErosionRate;
	std::vector<double> downstreamOverallDepositionRate;
	std::vector<double> downstreamOverallErosionRate;
	std::vector<double> localDepositionAndErosionRate;
	std::vector<double> downstreamDepositionAndErosionRate;
	std::vector<double> localDepositionAndErosion;
	std::vector<double> downstreamDepositionAndErosion;
	double localDepositionAndErosionVolume;
	double downstreamDepositionAndErosionVolume;

	#if defined TIMESTEPANALYSIS
		double thresholdForOutputtingTimeSteps = TIMESTEPANALYSIS;
		std::ostringstream oStringStream;
	#endif

	double timeStepEntry;
	const RiverReachProperties* currentRiverReachProperties;
	#pragma omp parallel for private(currentRiverReachProperties,timeStepEntry,currentDownstreamCellPointer,localLinearConversion,downstreamLinearConversion,bedslopeChangeRate,bedslopeChange,currentBedslope,tempTimeStep,localOverallDepositionRate,localOverallErosionRate,downstreamOverallDepositionRate,downstreamOverallErosionRate,localDepositionAndErosionRate,downstreamDepositionAndErosionRate,localDepositionAndErosion,downstreamDepositionAndErosion,localDepositionAndErosionVolume,downstreamDepositionAndErosionVolume) default(shared)
	for(int i = 0; i < (riverSystem.regularRiverSystemProperties.cellProperties.size() - 1); ++i)
	{
		currentRiverReachProperties = &(riverSystem.regularRiverSystemProperties.cellProperties[i]);
		if( !(currentRiverReachProperties->isMargin()) )
		{
			currentDownstreamCellPointer = (*currentRiverReachProperties).getDownstreamCellPointer();
			//Check for Courant-Friedrichs-Lewy.
			currentLocalSedimentVelocity = overallMethods.bedloadVelocityCalculationMethod->calculate( (*currentRiverReachProperties) );
			if( currentLocalSedimentVelocity > 0.0 )
			{
				timeStepEntry = ( riverSystem.overallParameters.getCourantFriedrichsLewyNumber() * (std::min(((*currentRiverReachProperties).regularRiverReachProperties.length),((*currentDownstreamCellPointer).regularRiverReachProperties.length))) / currentLocalSedimentVelocity );
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
						oStringStream << " due to SedimentFlowTypeMethods check for Courant-Friedrichs-Lewy." << std::flush;
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

			//Check for not Eroding more than active Layer
			double currentLocalActiveOverallVolume = ((*currentRiverReachProperties).regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getOverallVolume();
			if( (*currentRiverReachProperties).regularRiverReachProperties.strataPerUnitBedSurface.size() > 1 && ((*currentRiverReachProperties).regularRiverReachProperties.strataPerUnitBedSurface.at(1)).getOverallVolume() > 0.0000001 )
			{
				timeStepEntry = currentLocalActiveOverallVolume * maximumFractionOfActiveLayerToBeEroded /  ( (*currentRiverReachProperties).geometricalChannelBehaviour->convertActiveWidthAndOverallSedimentVolumeIncrementIntoSedimentVolumeIncrementPerUnitBedSurface((*currentRiverReachProperties).regularRiverReachProperties.activeWidth, (*currentRiverReachProperties).regularRiverReachProperties.erosionRate)  ).getOverallVolume();
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
						oStringStream << " due to SedimentFlowTypeMethods check for not eroding more than active layer." << std::flush;
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

			currentBedslope = ((*currentRiverReachProperties).regularRiverReachProperties.elevation - (*currentDownstreamCellPointer).regularRiverReachProperties.elevation) / (*currentRiverReachProperties).regularRiverReachProperties.length;
			if( (this->preventZeroOrNegativeBedSlopes) && (currentBedslope > 0.0) )
			{
				//Check for not creating negative slopes
				localLinearConversion = (*currentRiverReachProperties).geometricalChannelBehaviour->getLinearConversionFactorFromVolumeToHeight( (*currentRiverReachProperties).regularRiverReachProperties.activeWidth, (*currentRiverReachProperties).regularRiverReachProperties.length );
				downstreamLinearConversion = (*currentDownstreamCellPointer).geometricalChannelBehaviour->getLinearConversionFactorFromVolumeToHeight( (*currentDownstreamCellPointer).regularRiverReachProperties.activeWidth, (*currentDownstreamCellPointer).regularRiverReachProperties.length );
				if(localLinearConversion.first && downstreamLinearConversion.first)
				{
					bedslopeChangeRate = (1.0 / (1.0 - riverSystem.overallParameters.getPoreVolumeFraction()) ) * fabs( ( (localLinearConversion.second * ((*currentRiverReachProperties).regularRiverReachProperties.depositionRate.getOverallVolume()-(*currentRiverReachProperties).regularRiverReachProperties.erosionRate.getOverallVolume())) - (downstreamLinearConversion.second * ((*currentDownstreamCellPointer).regularRiverReachProperties.depositionRate.getOverallVolume()-(*currentDownstreamCellPointer).regularRiverReachProperties.erosionRate.getOverallVolume())) ) / (*currentRiverReachProperties).regularRiverReachProperties.length  );
					timeStepEntry = maximumRelativeTwoCellBedSlopeChange * currentBedslope /  bedslopeChangeRate ;
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
							oStringStream << " due to SedimentFlowTypeMethods check for not creating negative slopes." << std::flush;
							#pragma omp critical
							{ std::cout << std::endl << "===============================================================" << std::endl  << oStringStream.str() << std::endl << std::flush; }

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
				}else
				{
					tempTimeStep = *( std::min_element( allCalculatedTimeSteps.begin(), allCalculatedTimeSteps.end() ) );
					localOverallDepositionRate = (*currentRiverReachProperties).regularRiverReachProperties.depositionRate.getOverallFractionalAbundance();
					localOverallErosionRate = (*currentRiverReachProperties).regularRiverReachProperties.erosionRate.getOverallFractionalAbundance();
					downstreamOverallDepositionRate = (*currentDownstreamCellPointer).regularRiverReachProperties.depositionRate.getOverallFractionalAbundance();
					downstreamOverallErosionRate = (*currentDownstreamCellPointer).regularRiverReachProperties.erosionRate.getOverallFractionalAbundance();
					localDepositionAndErosionRate.clear();
					downstreamDepositionAndErosionRate.clear();
					for(std::vector<double>::const_iterator currentFractionLocalDeposition = localOverallDepositionRate.begin(), currentFractionLocalErosion = localOverallErosionRate.begin(), currentFractionalDownstreamDeposition = downstreamOverallDepositionRate.begin(), currentFractionalDownstreamErosion = downstreamOverallErosionRate.begin(); currentFractionLocalDeposition < localOverallDepositionRate.end(); ++currentFractionLocalDeposition, ++currentFractionLocalErosion, ++currentFractionalDownstreamDeposition, ++currentFractionalDownstreamErosion)
					{
						localDepositionAndErosionRate.push_back( ( *currentFractionLocalDeposition - *currentFractionLocalErosion ) );
						downstreamDepositionAndErosionRate.push_back( ( *currentFractionalDownstreamDeposition - *currentFractionalDownstreamErosion ) );
					}

					localDepositionAndErosion.clear();
					downstreamDepositionAndErosion.clear();
					for(std::vector<double>::const_iterator currentLocalDERate = localDepositionAndErosionRate.begin(), currentDownstreamDERate = downstreamDepositionAndErosionRate.begin(); currentLocalDERate < localDepositionAndErosionRate.end(); ++currentLocalDERate, ++currentDownstreamDERate)
					{
						localDepositionAndErosion.push_back( (*currentLocalDERate * tempTimeStep) );
						downstreamDepositionAndErosion.push_back( (*currentDownstreamDERate * tempTimeStep) );
					}

					localDepositionAndErosionVolume = std::accumulate(localDepositionAndErosion.begin(),localDepositionAndErosion.end(),0.0);
					localDepositionAndErosionVolume /= (1.0 - riverSystem.overallParameters.getPoreVolumeFraction());
					downstreamDepositionAndErosionVolume = std::accumulate(downstreamDepositionAndErosion.begin(),downstreamDepositionAndErosion.end(),0.0);
					downstreamDepositionAndErosionVolume /= (1.0 - riverSystem.overallParameters.getPoreVolumeFraction());

					bedslopeChange = fabs( ( (*currentRiverReachProperties).geometricalChannelBehaviour->convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel((*currentRiverReachProperties).regularRiverReachProperties.activeWidth, (*currentRiverReachProperties).regularRiverReachProperties.length, localDepositionAndErosionVolume) -  (*currentDownstreamCellPointer).geometricalChannelBehaviour->convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel((*currentDownstreamCellPointer).regularRiverReachProperties.activeWidth, (*currentDownstreamCellPointer).regularRiverReachProperties.length, downstreamDepositionAndErosionVolume) ) / (*currentRiverReachProperties).regularRiverReachProperties.length );
					while((bedslopeChange/currentBedslope) > maximumRelativeTwoCellBedSlopeChange)
					{
						tempTimeStep /= 2.0;
						for(std::vector<double>::iterator currentLocalDE = localDepositionAndErosion.begin(), currentDownstreamDE = downstreamDepositionAndErosion.begin(); currentLocalDE < localDepositionAndErosion.end(); ++currentLocalDE, ++currentDownstreamDE)
						{
							*currentLocalDE /= 2.0;
							*currentDownstreamDE /= 2.0;
						}
						localDepositionAndErosionVolume = std::accumulate(localDepositionAndErosion.begin(),localDepositionAndErosion.end(),0.0);
						localDepositionAndErosionVolume /= (1.0 - riverSystem.overallParameters.getPoreVolumeFraction());
						downstreamDepositionAndErosionVolume = std::accumulate(downstreamDepositionAndErosion.begin(),downstreamDepositionAndErosion.end(),0.0);
						downstreamDepositionAndErosionVolume /= (1.0 - riverSystem.overallParameters.getPoreVolumeFraction());

						bedslopeChange = fabs( ( (*currentRiverReachProperties).geometricalChannelBehaviour->convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel((*currentRiverReachProperties).regularRiverReachProperties.activeWidth, (*currentRiverReachProperties).regularRiverReachProperties.length, localDepositionAndErosionVolume) -  (*currentDownstreamCellPointer).geometricalChannelBehaviour->convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel((*currentDownstreamCellPointer).regularRiverReachProperties.activeWidth, (*currentDownstreamCellPointer).regularRiverReachProperties.length, downstreamDepositionAndErosionVolume) ) / (*currentRiverReachProperties).regularRiverReachProperties.length );
					}
					timeStepEntry = tempTimeStep;
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
							oStringStream << " due to SedimentFlowTypeMethods check for not creating negative slopes." << std::flush;
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
		}

	}
	return *( std::min_element( allCalculatedTimeSteps.begin(), allCalculatedTimeSteps.end() ) );
}

void SedimentFlowTypeMethods::calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const
{
	riverReachProperties.regularRiverReachProperties.deposition = riverReachProperties.regularRiverReachProperties.depositionRate * timeStep;
	riverReachProperties.regularRiverReachProperties.erosion = riverReachProperties.regularRiverReachProperties.erosionRate * timeStep;
}

void SedimentFlowTypeMethods::handDownChange (RiverReachProperties& riverReachProperties) const
{
	// Scale down overall erosion and deposition down to unit active width
	riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface = (*(riverReachProperties.geometricalChannelBehaviour)).convertActiveWidthAndOverallSedimentVolumeIncrementIntoSedimentVolumeIncrementPerUnitBedSurface( riverReachProperties.regularRiverReachProperties.activeWidth, riverReachProperties.regularRiverReachProperties.erosion );
	riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface = (*(riverReachProperties.geometricalChannelBehaviour)).convertActiveWidthAndOverallSedimentVolumeIncrementIntoSedimentVolumeIncrementPerUnitBedSurface( riverReachProperties.regularRiverReachProperties.activeWidth, riverReachProperties.regularRiverReachProperties.deposition );

	// Scale down erosion and deposition to unit bed surface
	riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface /= riverReachProperties.regularRiverReachProperties.length;
	riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface /= riverReachProperties.regularRiverReachProperties.length;

	Grains erosionDifference = riverReachProperties.regularRiverReachProperties.erosion;

	if ( !(riverReachProperties.isMargin()) )
	{
		// Set tempStrataPerUnitBedSurface and add deposition to it
		std::vector<Grains> tempStrataPerUnitBedSurface = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface;
		riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface.deposit(tempStrataPerUnitBedSurface);
		// Make sure that it is not more eroded than available
		riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface = riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface.getPotentialErosion(tempStrataPerUnitBedSurface);
		// Scale up erosion per unit bed surface to overall erosion.
		riverReachProperties.regularRiverReachProperties.erosion = (*(riverReachProperties.geometricalChannelBehaviour)).convertActiveWidthAndSedimentVolumePerUnitBedSurfaceIntoOverallActiveSedimentVolume( riverReachProperties.regularRiverReachProperties.activeWidth, riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface );
		riverReachProperties.regularRiverReachProperties.erosion *= riverReachProperties.regularRiverReachProperties.length;
	}

	erosionDifference.subtract(riverReachProperties.regularRiverReachProperties.erosion);

	if ( !(riverReachProperties.isDownstreamMargin()) )
	{
		(*(riverReachProperties.getDownstreamCellPointer())).regularRiverReachProperties.deposition.subtract(erosionDifference);
	}
}

void SedimentFlowTypeMethods::applyChange (RiverReachProperties& riverReachProperties) const
{
	riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface.deposit(riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface);
	riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface.erode(riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface);

	std::vector<double> overallDeposition = riverReachProperties.regularRiverReachProperties.deposition.getOverallFractionalAbundance();
	std::vector<double> overallErosion = riverReachProperties.regularRiverReachProperties.erosion.getOverallFractionalAbundance();
	double overallDepositionVolume = std::accumulate(overallDeposition.begin(),overallDeposition.end(),0.0);
	double overallErosionVolume = std::accumulate(overallErosion.begin(),overallErosion.end(),0.0);
	double overallVolumeIncrement = ( overallDepositionVolume - overallErosionVolume ) / ( 1.0 - (riverReachProperties.getOverallParameters())->getPoreVolumeFraction() );
	riverReachProperties.regularRiverReachProperties.elevation += (*(riverReachProperties.geometricalChannelBehaviour)).convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementIncludingAlluviumChannelUpdate( riverReachProperties.regularRiverReachProperties.activeWidth, riverReachProperties.regularRiverReachProperties.length, overallVolumeIncrement );
}

void SedimentFlowTypeMethods::updateOtherParameters (RiverReachProperties& riverReachProperties) const
{
	riverReachProperties.strataSorting->sortStrata(riverReachProperties.regularRiverReachProperties,riverReachProperties.getCellID());
}

void SedimentFlowTypeMethods::handDownOtherParameters (RiverReachProperties& riverReachProperties) const
{
	riverReachProperties.regularRiverReachProperties.bedslope = overallMethods.bedSlopeCalculationMethod->calculate(riverReachProperties);


	if( overallMethods.waterEnergySlopeCalculationMethod->dependsOnBedslope() )
	{
		riverReachProperties.regularRiverReachProperties.waterEnergyslope = overallMethods.waterEnergySlopeCalculationMethod->calculate(riverReachProperties);

		if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnWaterEnergyslope() )
		{
			std::pair<double,double> reducedAndUnreducedSlope =  overallMethods.sedimentEnergySlopeCalculationMethod->calculateReducedAndUnreducedSedimentEnergyslope(riverReachProperties);
			riverReachProperties.regularRiverReachProperties.sedimentEnergyslope = reducedAndUnreducedSlope.first;
			riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope = reducedAndUnreducedSlope.second;
		}

	}

	if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnBedslope() )
	{
		std::pair<double,double> reducedAndUnreducedSlope =  overallMethods.sedimentEnergySlopeCalculationMethod->calculateReducedAndUnreducedSedimentEnergyslope(riverReachProperties);
		riverReachProperties.regularRiverReachProperties.sedimentEnergyslope = reducedAndUnreducedSlope.first;
		riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope = reducedAndUnreducedSlope.second;
	}
}

std::string SedimentFlowTypeMethods::getTypeOfSedimentFlowMethodsAsString() const
{
	return CombinerVariables::typeOfSedimentFlowMethodsToString(this->typeOfSedimentFlowMethods);
}

CombinerVariables::TypesOfSedimentFlowMethods SedimentFlowTypeMethods::getTypeOfSedimentFlowMethods() const
{
	return this->typeOfSedimentFlowMethods;
}

}
