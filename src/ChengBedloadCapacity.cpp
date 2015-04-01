/*
 * ChengBedloadCapacity.cpp
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

#include "ChengBedloadCapacity.h"

#include <algorithm>
#include <math.h>

#include "BasicCalculations.h"
#include "ReducedWaterEnergyslope.h"

namespace SedFlow {

ChengBedloadCapacity::ChengBedloadCapacity(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, const CalcHidingFactors* hidingFactorsCalculationMethod, bool thetaCriticalBasedOnConstantSred, double factor):
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	hidingFactorsCalculationMethod(hidingFactorsCalculationMethod->createCalcHidingFactorsMethodPointerCopy()),
	thetaCriticalBasedOnConstantSred(thetaCriticalBasedOnConstantSred),
	factor(factor)
{}

ChengBedloadCapacity::ChengBedloadCapacity(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, const CalcHidingFactors* hidingFactorsCalculationMethod, bool thetaCriticalBasedOnConstantSred):
	thresholdCalculationMethod(thresholdCalculationMethod->createCalcThresholdForInitiationOfBedloadMotionPointerCopy()),
	hidingFactorsCalculationMethod(hidingFactorsCalculationMethod->createCalcHidingFactorsMethodPointerCopy()),
	thetaCriticalBasedOnConstantSred(thetaCriticalBasedOnConstantSred),
	factor(13.0)
{}

ChengBedloadCapacity::~ChengBedloadCapacity()
{
	delete thresholdCalculationMethod;
	delete hidingFactorsCalculationMethod;
}

CalcBedloadCapacity* ChengBedloadCapacity::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new ChengBedloadCapacity(thresholdCalculationMethod,hidingFactorsCalculationMethod,thetaCriticalBasedOnConstantSred,factor);
	return result;
}

ConstructionVariables ChengBedloadCapacity::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::ChengBedloadCapacity);
	std::vector<bool> boolVector;
	boolVector.push_back(thetaCriticalBasedOnConstantSred);
	result.labelledBools["thetaCriticalBasedOnConstantSred"] = boolVector;
	std::vector<double> doubleVector;
	doubleVector.push_back(factor);
	result.labelledDoubles["factor"] = doubleVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( thresholdCalculationMethod->createConstructionVariables() );
	result.labelledObjects["thresholdCalculationMethod"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( hidingFactorsCalculationMethod->createConstructionVariables() );
	result.labelledObjects["hidingFactorsCalculationMethod"] = constructionVariablesVector;
	return result;
}

Grains ChengBedloadCapacity::calculate(const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	//TODO Remove this debugging line.
	double thetaCriticalNotCorrectedForHiding,constantFactors;

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

			thetaCriticalNotCorrectedForHiding = thresholdCalculationMethod->calculate(riverReachProperties.regularRiverReachProperties);
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

			constantFactors = factor *  (1/overallVolume) * sqrt(((riverReachProperties.getOverallParameters())->getGravityAcceleration() * (((riverReachProperties.getOverallParameters())->getSedimentDensity()/(riverReachProperties.getOverallParameters())->getWaterDensity())-1))) * riverReachProperties.regularRiverReachProperties.activeWidth ;

			std::vector<double>::iterator resultIterator = resultOverallFractionalAbundances.begin();
			for(std::vector<double>::const_iterator currentFractionalAbundance = overallFractionalAbundances.begin(), currentGrainDiameter = fractionalGrainDiameters.begin(), currentTheta = theta.begin(), currentThetaCritical = thetaCritical.begin() ; currentFractionalAbundance < overallFractionalAbundances.end() ; ++resultIterator, ++currentFractionalAbundance, ++currentGrainDiameter, ++currentTheta, ++currentThetaCritical)
			{
				*resultIterator = ( (*currentFractionalAbundance) * constantFactors * exp(((-(*currentThetaCritical)) / pow((*currentTheta),1.5))) * pow(((*currentGrainDiameter) * (*currentTheta)), 1.5) );
			}
		}
	}

	Grains result = Grains::distributeFractionalAbundance(resultOverallFractionalAbundances,*erosionBase);

	delete inputFromUpstream;

	return result;

}

}
