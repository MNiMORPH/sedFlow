/*
 * RickenmannBedloadCapacityBasedOnTheta.cpp
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

#include "RickenmannBedloadCapacityBasedOnTheta.h"

#include <math.h>
#include <numeric>
#include <algorithm>
#include <functional>

#include "BasicCalculations.h"
#include "ReducedWaterEnergyslope.h"

namespace SedFlow {

RickenmannBedloadCapacityBasedOnTheta::RickenmannBedloadCapacityBasedOnTheta(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, const CalcHidingFactors* hidingFactorsCalculationMethod, bool useOnePointOneAsExponentForFroudeNumber, bool simplifiedEquation, bool thetaCriticalBasedOnConstantSred):
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	hidingFactorsCalculationMethod(hidingFactorsCalculationMethod->createCalcHidingFactorsMethodPointerCopy()),
	useOnePointOneAsExponentForFroudeNumber(useOnePointOneAsExponentForFroudeNumber),
	simplifiedEquation(simplifiedEquation),
	thetaCriticalBasedOnConstantSred(thetaCriticalBasedOnConstantSred)
{}

RickenmannBedloadCapacityBasedOnTheta::~RickenmannBedloadCapacityBasedOnTheta()
{
	delete thresholdCalculationMethod;
	delete hidingFactorsCalculationMethod;
}

CalcBedloadCapacity* RickenmannBedloadCapacityBasedOnTheta::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new RickenmannBedloadCapacityBasedOnTheta(thresholdCalculationMethod,hidingFactorsCalculationMethod,useOnePointOneAsExponentForFroudeNumber,simplifiedEquation,thetaCriticalBasedOnConstantSred);
	return result;
}

ConstructionVariables RickenmannBedloadCapacityBasedOnTheta::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::RickenmannBedloadCapacityBasedOnTheta);
	std::vector<bool> boolVector;
	boolVector.push_back(useOnePointOneAsExponentForFroudeNumber);
	result.labelledBools["useOnePointOneAsExponentForFroudeNumber"] = boolVector;
	boolVector.clear();
	boolVector.push_back(simplifiedEquation);
	result.labelledBools["simplifiedEquation"] = boolVector;
	boolVector.clear();
	boolVector.push_back(thetaCriticalBasedOnConstantSred);
	result.labelledBools["thetaCriticalBasedOnConstantSred"] = boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( thresholdCalculationMethod->createConstructionVariables() );
	result.labelledObjects["thresholdCalculationMethod"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( hidingFactorsCalculationMethod->createConstructionVariables() );
	result.labelledObjects["hidingFactorsCalculationMethod"] = constructionVariablesVector;
	return result;
}

Grains RickenmannBedloadCapacityBasedOnTheta::calculate(const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	const Grains* erosionBase = &(riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0));
	std::vector<double> resultOverallFractionalAbundances ( erosionBase->getNumberOfFractions(), 0.0 );
	double overallVolume = erosionBase->getOverallVolume();
	Grains* inputFromUpstream = NULL;

	if( overallVolume >= 0.0 )
	{
		if( overallVolume <= 1e-9 ) // In this case use the sediment coming from upstream as distribution base. This enables bypassing of sediment.
		{
			inputFromUpstream = new Grains(*erosionBase);
			int localCellID = riverReachProperties.getCellID();
			std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
			for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamPointerIterator = upstreamCellPointers.begin(); currentUpstreamPointerIterator < upstreamCellPointers.end(); ++currentUpstreamPointerIterator)
			{
				#if !defined SEDFLOWPARALLEL
				if( (*currentUpstreamPointerIterator)->getCellID() < localCellID )
					{ *inputFromUpstream += (*currentUpstreamPointerIterator)->regularRiverReachProperties.erosionRate; }
				else
				#endif
					{ *inputFromUpstream += this->calculate( (*(*currentUpstreamPointerIterator)), overallMethods ); }
			}
			erosionBase = inputFromUpstream;
			overallVolume = erosionBase->getOverallVolume();
		}

		if( overallVolume > 1e-9 ) // If there is no sediment in the reach and there is nothing coming from upstream. Do nothing and return zeros.
		{
			std::vector<double> overallFractionalAbundances = erosionBase->getOverallFractionalAbundance();
			std::vector<double> fractionalGrainDiameters = (riverReachProperties.getOverallParameters())->getFractionalGrainDiameters();
			std::vector<double> theta = BasicCalculations::calculateDimensionlessShearStress(riverReachProperties.regularRiverReachProperties);

			#if defined _DEBUG || defined DEBUG
				if (overallFractionalAbundances.size() != fractionalGrainDiameters.size() || overallFractionalAbundances.size() != theta.size())
				{
					const char *const secondErrorMessage = "Action on not matching number of grain fractions";
					throw(secondErrorMessage);
				}
			#endif

			double thetaCriticalNotCorrectedForHiding = thresholdCalculationMethod->calculate(riverReachProperties.regularRiverReachProperties);
			if(thetaCriticalBasedOnConstantSred)
			{
				CombinerVariables::TypesOfGradientCalculationMethod typeOfSedimentEnergyslopeCalculationMethod = overallMethods.sedimentEnergySlopeCalculationMethod->getTypeOfGradientCalculationMethod();
				if((typeOfSedimentEnergyslopeCalculationMethod == CombinerVariables::ReducedWaterEnergyslope) || (typeOfSedimentEnergyslopeCalculationMethod == CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable))
				{
					double criticalFlowDepth = BasicCalculations::calculateHydraulicRadiusFromDimensionlessShearStress(thetaCriticalNotCorrectedForHiding,(erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,50.0)),riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope,riverReachProperties);
					double constantReducedSlope = (dynamic_cast<ReducedWaterEnergyslope*>(overallMethods.sedimentEnergySlopeCalculationMethod))->convertSimpleToReducedSlope(riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope,criticalFlowDepth,riverReachProperties,false);
					thetaCriticalNotCorrectedForHiding *= constantReducedSlope / riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope;
				}
			}
			else
			{
				thetaCriticalNotCorrectedForHiding *=  riverReachProperties.regularRiverReachProperties.sedimentEnergyslope / riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope;
			}

			std::vector<double> thetaCritical = hidingFactorsCalculationMethod->calculateFractionalHidingFactors(fractionalGrainDiameters,overallFractionalAbundances,riverReachProperties);
			std::transform(thetaCritical.begin(),thetaCritical.end(),thetaCritical.begin(),std::bind1st(std::multiplies<double>(),thetaCriticalNotCorrectedForHiding));
			double froudeFactor = BasicCalculations::calculateFroude(riverReachProperties);
			if(useOnePointOneAsExponentForFroudeNumber)
				{ froudeFactor = pow(froudeFactor,1.1); }

			double constantFactors = (1/overallVolume) * froudeFactor * sqrt((riverReachProperties.getOverallParameters())->getGravityAcceleration()) * riverReachProperties.regularRiverReachProperties.activeWidth ;

			if(simplifiedEquation)
			{
				constantFactors *= 2.5 * sqrt( ( ((riverReachProperties.getOverallParameters())->getSedimentDensity()/(riverReachProperties.getOverallParameters())->getWaterDensity()) - 1.0 ) );
			}
			else
			{
				double dNinetyOverDThirty = erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,90.0) / erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,30.0);
				constantFactors *= 3.1 * pow(dNinetyOverDThirty,0.2);
			}

			std::vector<double>::iterator resultIterator = resultOverallFractionalAbundances.begin();
			for(std::vector<double>::const_iterator currentFractionalAbundance = overallFractionalAbundances.begin(), currentGrainDiameter = fractionalGrainDiameters.begin(), currentTheta = theta.begin(), currentThetaCritical = thetaCritical.begin() ; currentFractionalAbundance < overallFractionalAbundances.end() ; ++resultIterator, ++currentFractionalAbundance, ++currentGrainDiameter, ++currentTheta, ++currentThetaCritical)
			{
				if( (*currentTheta) > (*currentThetaCritical) )
					{ *resultIterator = ( (*currentFractionalAbundance) * constantFactors * sqrt((*currentTheta)) * ((*currentTheta) - (*currentThetaCritical)) * pow((*currentGrainDiameter), 1.5) ); }
			}
		}
	}

	Grains result = Grains::distributeFractionalAbundance(resultOverallFractionalAbundances,*erosionBase);

	delete inputFromUpstream;

	return result;

}

}
