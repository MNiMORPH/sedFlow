/*
 * SternbergAbrasionIncludingFining.cpp
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

#include "SternbergAbrasionIncludingFining.h"

#include <iostream>
#include <math.h>
#include <limits>
#include <algorithm>
#include <functional>
#include <numeric>

namespace SedFlow {

SternbergAbrasionIncludingFining::SternbergAbrasionIncludingFining(double sternbergAbrasionCoefficient):
			sternbergAbrasionCoefficient(sternbergAbrasionCoefficient),
			treatedCellID(std::numeric_limits<int>::min()),
			singleUpstreamReach(false),
			abrasionLossFactor(0.0)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::SternbergAbrasionIncludingFining;
	if (this->sternbergAbrasionCoefficient < 0.0)
	{
		const char *const errorMessage = "The sternbergAbrasionCoefficient cannot have a negativeValue.";
		throw(errorMessage);
	}
}

SternbergAbrasionIncludingFining::SternbergAbrasionIncludingFining(double sternbergAbrasionCoefficient, int treatedCellID, bool singleUpstreamReach, double abrasionLossFactor, std::vector<double> singleAbrasionLossFactors, std::vector<double> grainDiameters, std::vector<double> diameterSpansPerFraction):
	sternbergAbrasionCoefficient(sternbergAbrasionCoefficient),
	treatedCellID(treatedCellID),
	singleUpstreamReach(singleUpstreamReach),
	abrasionLossFactor(abrasionLossFactor),
	singleAbrasionLossFactors(singleAbrasionLossFactors),
	grainDiameters(grainDiameters),
	diameterSpansPerFraction(diameterSpansPerFraction)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::SternbergAbrasionIncludingFining;
}


SternbergAbrasionIncludingFining::~SternbergAbrasionIncludingFining(){}

ConstructionVariables SternbergAbrasionIncludingFining::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
	result.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::SternbergAbrasionIncludingFining);
	std::vector<double> doubleVector (1,this->sternbergAbrasionCoefficient);
	result.labelledDoubles["sternbergAbrasionCoefficient"] = doubleVector;
	return result;
}

AdditionalRiverReachMethodType* SternbergAbrasionIncludingFining::createAdditionalRiverReachMethodTypeCopy() const
{
	AdditionalRiverReachMethodType* result = new SternbergAbrasionIncludingFining(this->sternbergAbrasionCoefficient,this->treatedCellID,this->singleUpstreamReach,this->abrasionLossFactor,this->singleAbrasionLossFactors,this->grainDiameters,this->diameterSpansPerFraction);
	return result;
}

void SternbergAbrasionIncludingFining::typeSpecificAction (RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods)
{
	if(singleUpstreamReach && riverReachProperties.getCellID()==treatedCellID)
	{
		riverReachProperties.regularRiverReachProperties.deposition *= abrasionLossFactor;
		riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface *= abrasionLossFactor;
		this->applyFining(riverReachProperties);
	}
	else
	{
		if( !(riverReachProperties.isUpstreamMargin()) )
		{
			std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
			if(upstreamCellPointers.size()==1)
			{
				treatedCellID = riverReachProperties.getCellID();
				singleUpstreamReach = true;
				abrasionLossFactor = exp( (-1.0) * sternbergAbrasionCoefficient * (upstreamCellPointers.at(0))->regularRiverReachProperties.length * 0.001 );
			}
			else
			{
				if(riverReachProperties.getCellID() != treatedCellID)
				{
					treatedCellID = riverReachProperties.getCellID();
					singleAbrasionLossFactors.clear();
					for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
					{
						singleAbrasionLossFactors.push_back( (exp( (-1.0) * sternbergAbrasionCoefficient * (*currentUpstreamCellPointer)->regularRiverReachProperties.length * 0.001 )) );
					}
				}
				singleUpstreamErosions.clear();
				std::vector<double> singleUpstreamErosionVolumes;
				double overallUpstreamErosion = 0.0;
				for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
				{
					singleUpstreamErosions.push_back( (*currentUpstreamCellPointer)->regularRiverReachProperties.erosion );
					singleUpstreamErosionVolumes.push_back( (singleUpstreamErosions.back()).getOverallVolume() );
					overallUpstreamErosion += singleUpstreamErosionVolumes.back();
				}
				//Convert singleUpstreamErosionVolumes from absolute to relative values
				std::transform(singleUpstreamErosionVolumes.begin(),singleUpstreamErosionVolumes.end(),singleUpstreamErosionVolumes.begin(),std::bind2nd(std::divides<double>(),overallUpstreamErosion));
				//Calculate weightedAbrasionLossFactors using singleUpstreamErosionVolumes as container
				std::transform(singleUpstreamErosionVolumes.begin(),singleUpstreamErosionVolumes.end(),singleAbrasionLossFactors.begin(),singleUpstreamErosionVolumes.begin(),std::multiplies<double>());
				abrasionLossFactor = std::accumulate(singleUpstreamErosionVolumes.begin(),singleUpstreamErosionVolumes.end(),0.0);
			}
			riverReachProperties.regularRiverReachProperties.deposition *= abrasionLossFactor;
			riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface *= abrasionLossFactor;
			this->applyFining(riverReachProperties);
		}
	}
}

void SternbergAbrasionIncludingFining::applyFining(RiverReachProperties& riverReachProperties)
{
	if(diameterSpansPerFraction.size() < 1)
	{
		grainDiameters = (riverReachProperties.getOverallParameters())->getFractionalGrainDiameters();
		diameterSpansPerFraction.clear();
		if(grainDiameters.size() == 1)
		{
			diameterSpansPerFraction.push_back((grainDiameters[0] * 2.0));
		}
		else
		{
			diameterSpansPerFraction.push_back( 0.5 * ( grainDiameters[0] + grainDiameters[1] ) );
			for(std::vector<double>::const_iterator currentDiameter = (grainDiameters.begin()+1); currentDiameter < (grainDiameters.end()-1); ++currentDiameter)
			{
				diameterSpansPerFraction.push_back(  0.5 * ( (*(currentDiameter+1)) - (*(currentDiameter-1)) ) );
			}
			diameterSpansPerFraction.push_back( ( grainDiameters.back() - (*(grainDiameters.end()-2)) ) );
		}
	}

	std::vector<double> currentErosionOverallAbundances;
	std::vector< std::vector<double> > upstreamErosionsPerFraction (grainDiameters.size(),currentErosionOverallAbundances);
	std::vector< std::vector<double> >::iterator currentFractionContainer;
	for(std::vector<Grains>::const_iterator currentErosion = singleUpstreamErosions.begin(); currentErosion < singleUpstreamErosions.end(); ++currentErosion)
	{
		currentErosionOverallAbundances.clear();
		currentErosionOverallAbundances = currentErosion->getOverallFractionalAbundance();
		currentFractionContainer = upstreamErosionsPerFraction.begin();
		for(std::vector<double>::const_iterator currentErosionFraction = currentErosionOverallAbundances.begin(); currentErosionFraction < currentErosionOverallAbundances.end(); ++currentErosionFraction, ++currentFractionContainer)
		{
			currentFractionContainer->push_back(*currentErosionFraction);
		}
	}

	std::vector<double> singleDiameterLossFactors = singleAbrasionLossFactors;
	for(std::vector<double>::iterator currentDiameterLossFactor = singleDiameterLossFactors.begin(); currentDiameterLossFactor < singleDiameterLossFactors.end(); ++currentDiameterLossFactor)
		{ *currentDiameterLossFactor = pow(*currentDiameterLossFactor,(1.0/3.0)); }

	std::vector<double> fractionalFiningLosses (1,0.0);
	std::vector<double> singleErosionsForCurrentFraction;
	double currentFractionalFiningLoss;
	double currentDiameter;
	double currentDiameterSpanPerFraction;
	double currentDiameterLoss;
	for(int currentFractionID = 1; currentFractionID < grainDiameters.size(); ++currentFractionID)
	{
		currentFractionalFiningLoss = 0.0;
		currentDiameter = grainDiameters.at(currentFractionID);
		currentDiameterSpanPerFraction = diameterSpansPerFraction.at(currentFractionID);
		singleErosionsForCurrentFraction.clear();
		singleErosionsForCurrentFraction = upstreamErosionsPerFraction.at(currentFractionID);
		for(int currentUpstreamCellIdentifier = 0; currentUpstreamCellIdentifier < singleErosionsForCurrentFraction.size(); ++currentUpstreamCellIdentifier)
		{
			currentDiameterLoss = currentDiameter * singleDiameterLossFactors.at(currentUpstreamCellIdentifier);
			currentFractionalFiningLoss += singleErosionsForCurrentFraction.at(currentUpstreamCellIdentifier) * std::min(1.0, (currentDiameterLoss/currentDiameterSpanPerFraction) );
		}
		fractionalFiningLosses.push_back(currentFractionalFiningLoss);
	}
	Grains finingLosses = Grains::distributeFractionalAbundance(fractionalFiningLosses,riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface);
	Grains actualFiningLosses = riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface.subtract(finingLosses);
	fractionalFiningLosses.clear();
	fractionalFiningLosses = actualFiningLosses.getOverallFractionalAbundance();
	fractionalFiningLosses.push_back(0.0);
	fractionalFiningLosses.erase(fractionalFiningLosses.begin());
	Grains finingWins = Grains::distributeFractionalAbundance(fractionalFiningLosses,riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface);
	riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface.add(finingWins);
}

}
