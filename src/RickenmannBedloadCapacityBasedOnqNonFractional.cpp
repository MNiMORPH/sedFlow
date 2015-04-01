/*
 * RickenmannBedloadCapacityBasedOnq.cpp
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

#include "RickenmannBedloadCapacityBasedOnqNonFractional.h"

#include <math.h>
#include <numeric>
#include <algorithm>
#include <functional>

#include "CorrectionForBedloadWeightAtSteepSlopes.h"

namespace SedFlow {

RickenmannBedloadCapacityBasedOnqNonFractional::RickenmannBedloadCapacityBasedOnqNonFractional(bool correctionForBedloadWeightAtSteepSlopes, bool takeArmourLayerIntoAccount):
	correctionForBedloadWeightAtSteepSlopes(correctionForBedloadWeightAtSteepSlopes),
	takeArmourLayerIntoAccount(takeArmourLayerIntoAccount)
{}

CalcBedloadCapacity* RickenmannBedloadCapacityBasedOnqNonFractional::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new RickenmannBedloadCapacityBasedOnqNonFractional(this->correctionForBedloadWeightAtSteepSlopes,this->takeArmourLayerIntoAccount);
	return result;
}

ConstructionVariables RickenmannBedloadCapacityBasedOnqNonFractional::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::RickenmannBedloadCapacityBasedOnqNonFractional);
	std::vector<bool> boolVector;
	boolVector.push_back(correctionForBedloadWeightAtSteepSlopes);
	result.labelledBools["correctionForBedloadWeightAtSteepSlopes"] = boolVector;
	boolVector.clear();
	boolVector.push_back(takeArmourLayerIntoAccount);
	result.labelledBools["takeArmourLayerIntoAccount"] = boolVector;
	return result;
}

Grains RickenmannBedloadCapacityBasedOnqNonFractional::calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	Grains erosionBase = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0);
	double overallVolume = erosionBase.getOverallVolume();
	double bedloadCapacityOverallVolume = 0.0;

	if( overallVolume >= 0.0 )
	{
		if( overallVolume <= 1e-9 ) // In this case use the sediment coming from upstream as distribution base. This enables bypassing of sediment.
		{
			int localCellID = riverReachProperties.getCellID();
			std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
			for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamPointerIterator = upstreamCellPointers.begin(); currentUpstreamPointerIterator < upstreamCellPointers.end(); ++currentUpstreamPointerIterator)
			{
				#if !defined SEDFLOWPARALLEL
				if( (*currentUpstreamPointerIterator)->getCellID() < localCellID )
					{ erosionBase += (*currentUpstreamPointerIterator)->regularRiverReachProperties.erosionRate; }
				else
				#endif
					{ erosionBase += this->calculate( (*(*currentUpstreamPointerIterator)), overallMethods ); }
			}
			overallVolume = erosionBase.getOverallVolume();
		}

		if( overallVolume > 1e-9 ) // If there is no sediment in the reach and there is nothing coming from upstream. Do nothing and return zeros.
		{
			std::vector<double> fractionalGrainDiameters = (riverReachProperties.getOverallParameters())->getFractionalGrainDiameters();
			double dNinety = erosionBase.getPercentileGrainDiameter(fractionalGrainDiameters,90.0);
			double dNinetyOverDThirty = dNinety / erosionBase.getPercentileGrainDiameter(fractionalGrainDiameters,30.0);
			double dFifty = erosionBase.getPercentileGrainDiameter(fractionalGrainDiameters,50.0);
			double sMinusOne = ( (riverReachProperties.getOverallParameters())->getSedimentDensity() / (riverReachProperties.getOverallParameters())->getWaterDensity() ) - 1;
			double qCritical = 0.065 * pow(sMinusOne,1.67) * sqrt( (riverReachProperties.getOverallParameters())->getGravityAcceleration() ) * dFifty * sqrt(dFifty) * pow( riverReachProperties.regularRiverReachProperties.bedslope, (-1.12) );
			if(takeArmourLayerIntoAccount)
			{
				double dMean = erosionBase.getArithmeticMeanGrainDiameter(fractionalGrainDiameters);
				qCritical *= pow( (dNinety/dMean) , (10.0/9.0) );
			}

			// The following two lines calculate flow depth and width of a virtual rectangular channel corresponding to the mean flow depth within the active width.
			double flowDepth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->getMeanFlowDepthForActiveWidth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth, riverReachProperties.regularRiverReachProperties.activeWidth);
			double flowWidth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(riverReachProperties.regularRiverReachProperties.maximumWaterdepth) / flowDepth;

			double dischargePerUnitFlowWidth = riverReachProperties.regularRiverReachProperties.discharge / flowWidth;

			double correctedSedimentEnergySlope = riverReachProperties.regularRiverReachProperties.sedimentEnergyslope;
			if ( correctionForBedloadWeightAtSteepSlopes && (riverReachProperties.regularRiverReachProperties.bedslope > 0.0) )
			{
				correctedSedimentEnergySlope = CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedSedimentEnergySlope(correctedSedimentEnergySlope,(riverReachProperties.getOverallParameters())->getAngleOfReposeInRadians(), riverReachProperties.regularRiverReachProperties.bedslope);
			}

			if( dischargePerUnitFlowWidth > qCritical )
			{
				bedloadCapacityOverallVolume = 3.1 * pow(sMinusOne,(-1.5)) * pow(dNinetyOverDThirty,0.2) * (dischargePerUnitFlowWidth - qCritical) * pow(correctedSedimentEnergySlope,1.5) * riverReachProperties.regularRiverReachProperties.activeWidth ;
			}

			erosionBase /= overallVolume;
		}
	}

	erosionBase *= bedloadCapacityOverallVolume;

	return erosionBase;

}


}
