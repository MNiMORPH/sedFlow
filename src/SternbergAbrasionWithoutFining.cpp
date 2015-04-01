/*
 * SternbergAbrasionWithoutFining.cpp
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

#include "SternbergAbrasionWithoutFining.h"

#include <iostream>
#include <math.h>
#include <limits>
#include <algorithm>
#include <functional>
#include <numeric>

namespace SedFlow {

SternbergAbrasionWithoutFining::SternbergAbrasionWithoutFining(double sternbergAbrasionCoefficient):
	sternbergAbrasionCoefficient(sternbergAbrasionCoefficient),
	treatedCellID(std::numeric_limits<int>::min()),
	singleUpstreamReach(false),
	abrasionLossFactor(0.0)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::SternbergAbrasionWithoutFining;
	if (this->sternbergAbrasionCoefficient < 0.0)
	{
		const char *const errorMessage = "The sternbergAbrasionCoefficient cannot have a negativeValue.";
		throw(errorMessage);
	}
}

SternbergAbrasionWithoutFining::SternbergAbrasionWithoutFining(double sternbergAbrasionCoefficient, int treatedCellID, bool singleUpstreamReach, double abrasionLossFactor, std::vector<double> singleAbrasionLossFactors):
	sternbergAbrasionCoefficient(sternbergAbrasionCoefficient),
	treatedCellID(treatedCellID),
	singleUpstreamReach(singleUpstreamReach),
	abrasionLossFactor(abrasionLossFactor),
	singleAbrasionLossFactors(singleAbrasionLossFactors)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::SternbergAbrasionWithoutFining;
}

SternbergAbrasionWithoutFining::~SternbergAbrasionWithoutFining(){}


ConstructionVariables SternbergAbrasionWithoutFining::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
	result.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::SternbergAbrasionWithoutFining);
	std::vector<double> doubleVector (1,this->sternbergAbrasionCoefficient);
	result.labelledDoubles["sternbergAbrasionCoefficient"] = doubleVector;
	return result;
}

AdditionalRiverReachMethodType* SternbergAbrasionWithoutFining::createAdditionalRiverReachMethodTypeCopy() const
{
	AdditionalRiverReachMethodType* result = new SternbergAbrasionWithoutFining(this->sternbergAbrasionCoefficient,this->treatedCellID,this->singleUpstreamReach,this->abrasionLossFactor,this->singleAbrasionLossFactors);
	return result;
}

void SternbergAbrasionWithoutFining::typeSpecificAction (RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods)
{
	if(singleUpstreamReach && riverReachProperties.getCellID()==treatedCellID)
	{
		riverReachProperties.regularRiverReachProperties.deposition *= abrasionLossFactor;
		riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface *= abrasionLossFactor;
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
				std::vector<double> singleUpstreamErosions;
				double overallUpstreamErosion = 0.0;
				for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamCellPointer = upstreamCellPointers.begin(); currentUpstreamCellPointer < upstreamCellPointers.end(); ++currentUpstreamCellPointer)
				{
					singleUpstreamErosions.push_back( (*currentUpstreamCellPointer)->regularRiverReachProperties.erosion.getOverallVolume() );
					overallUpstreamErosion += singleUpstreamErosions.back();
				}
				abrasionLossFactor = 1.0;
				if(overallUpstreamErosion > 0.0)
				{
					//Convert singleUpstreamErosions from absolute to relative values
					std::transform(singleUpstreamErosions.begin(),singleUpstreamErosions.end(),singleUpstreamErosions.begin(),std::bind2nd(std::divides<double>(),overallUpstreamErosion));
					//Transform singleUpstreamErosions into weightedAbrasionLossFactors
					std::transform(singleUpstreamErosions.begin(),singleUpstreamErosions.end(),singleAbrasionLossFactors.begin(),singleUpstreamErosions.begin(),std::multiplies<double>());
					abrasionLossFactor = std::accumulate(singleUpstreamErosions.begin(),singleUpstreamErosions.end(),0.0);
				}
			}
			riverReachProperties.regularRiverReachProperties.deposition *= abrasionLossFactor;
			riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface *= abrasionLossFactor;
		}
	}
}

}
