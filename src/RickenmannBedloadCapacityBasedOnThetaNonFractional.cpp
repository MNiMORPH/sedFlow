/*
 * RickenmannBedloadCapacityBasedOnThetaNonFractional.cpp
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

#include "RickenmannBedloadCapacityBasedOnThetaNonFractional.h"

#include <math.h>
#include <numeric>
#include <algorithm>
#include <functional>

#include "BasicCalculations.h"
#include "ReducedWaterEnergyslope.h"

namespace SedFlow {

RickenmannBedloadCapacityBasedOnThetaNonFractional::RickenmannBedloadCapacityBasedOnThetaNonFractional(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, bool takeArmourLayerIntoAccount, bool useOnePointOneAsExponentForFroudeNumber, bool simplifiedEquation, bool thetaCriticalBasedOnConstantSred):
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	takeArmourLayerIntoAccount(takeArmourLayerIntoAccount),
	useOnePointOneAsExponentForFroudeNumber(useOnePointOneAsExponentForFroudeNumber),
	simplifiedEquation(simplifiedEquation),
	thetaCriticalBasedOnConstantSred(thetaCriticalBasedOnConstantSred)
{}

RickenmannBedloadCapacityBasedOnThetaNonFractional::~RickenmannBedloadCapacityBasedOnThetaNonFractional()
{
	delete thresholdCalculationMethod;
}

CalcBedloadCapacity* RickenmannBedloadCapacityBasedOnThetaNonFractional::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new RickenmannBedloadCapacityBasedOnThetaNonFractional(thresholdCalculationMethod,takeArmourLayerIntoAccount,useOnePointOneAsExponentForFroudeNumber,simplifiedEquation,thetaCriticalBasedOnConstantSred);
	return result;
}

ConstructionVariables RickenmannBedloadCapacityBasedOnThetaNonFractional::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::RickenmannBedloadCapacityBasedOnThetaNonFractional);
	std::vector<bool> boolVector;
	boolVector.push_back(useOnePointOneAsExponentForFroudeNumber);
	result.labelledBools["useOnePointOneAsExponentForFroudeNumber"] = boolVector;
	boolVector.clear();
	boolVector.push_back(simplifiedEquation);
	result.labelledBools["simplifiedEquation"] = boolVector;
	boolVector.clear();
	boolVector.push_back(takeArmourLayerIntoAccount);
	result.labelledBools["takeArmourLayerIntoAccount"] = boolVector;
	boolVector.clear();
	boolVector.push_back(thetaCriticalBasedOnConstantSred);
	result.labelledBools["thetaCriticalBasedOnConstantSred"] = boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( thresholdCalculationMethod->createConstructionVariables() );
	result.labelledObjects["thresholdCalculationMethod"] = constructionVariablesVector;
	return result;
}

Grains RickenmannBedloadCapacityBasedOnThetaNonFractional::calculate(const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
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
			double dMean = erosionBase.getArithmeticMeanGrainDiameter(fractionalGrainDiameters);
			double theta = BasicCalculations::calculateDimensionlessShearStress(dMean,riverReachProperties.regularRiverReachProperties);
			double froudeFactor = BasicCalculations::calculateFroude(riverReachProperties);
			if(useOnePointOneAsExponentForFroudeNumber)
				{ froudeFactor = pow(froudeFactor,1.1); }

			double thetaCritical = thresholdCalculationMethod->calculate(riverReachProperties.regularRiverReachProperties);
			if(thetaCriticalBasedOnConstantSred)
			{
				CombinerVariables::TypesOfGradientCalculationMethod typeOfSedimentEnergyslopeCalculationMethod = overallMethods.sedimentEnergySlopeCalculationMethod->getTypeOfGradientCalculationMethod();
				if((typeOfSedimentEnergyslopeCalculationMethod == CombinerVariables::ReducedWaterEnergyslope) || (typeOfSedimentEnergyslopeCalculationMethod == CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable))
				{
					double criticalFlowDepth = BasicCalculations::calculateHydraulicRadiusFromDimensionlessShearStress(thetaCritical,(erosionBase.getPercentileGrainDiameter(fractionalGrainDiameters,50.0)),riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope,riverReachProperties);
					double constantReducedSlope = (dynamic_cast<ReducedWaterEnergyslope*>(overallMethods.sedimentEnergySlopeCalculationMethod))->convertSimpleToReducedSlope(riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope,criticalFlowDepth,riverReachProperties,false);
					thetaCritical *= constantReducedSlope / riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope;
				}
			}
			else
			{
				thetaCritical *=  riverReachProperties.regularRiverReachProperties.sedimentEnergyslope / riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope;
			}

			if(takeArmourLayerIntoAccount)
			{
				thetaCritical *= pow( (dNinety/dMean) , (2.0/3.0) );
			}

			if( theta > thetaCritical )
			{
				bedloadCapacityOverallVolume = sqrt(theta) * (theta - thetaCritical ) * froudeFactor * sqrt((riverReachProperties.getOverallParameters())->getGravityAcceleration()) * dMean * sqrt(dMean) * riverReachProperties.regularRiverReachProperties.activeWidth ;

				if(simplifiedEquation)
				{
					bedloadCapacityOverallVolume *= 2.5 * sqrt( ( ((riverReachProperties.getOverallParameters())->getSedimentDensity()/(riverReachProperties.getOverallParameters())->getWaterDensity()) - 1.0 ) );
				}
				else
				{
					double dNinetyOverDThirty = dNinety / erosionBase.getPercentileGrainDiameter(fractionalGrainDiameters,30.0);
					bedloadCapacityOverallVolume *= 3.1 * pow(dNinetyOverDThirty,0.2);
				}

			}

			erosionBase /= overallVolume;
		}
	}

	erosionBase *= bedloadCapacityOverallVolume;

	return erosionBase;

}

}
