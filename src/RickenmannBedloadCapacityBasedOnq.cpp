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

#include "RickenmannBedloadCapacityBasedOnq.h"

#include <math.h>
#include <numeric>
#include <algorithm>
#include <functional>

#include "CorrectionForBedloadWeightAtSteepSlopes.h"

namespace SedFlow {

RickenmannBedloadCapacityBasedOnq::RickenmannBedloadCapacityBasedOnq(bool correctionForBedloadWeightAtSteepSlopes, const CalcHidingFactors* hidingFactorsCalculationMethod):
	correctionForBedloadWeightAtSteepSlopes(correctionForBedloadWeightAtSteepSlopes),
	hidingFactorsCalculationMethod(hidingFactorsCalculationMethod->createCalcHidingFactorsMethodPointerCopy())
{}

RickenmannBedloadCapacityBasedOnq::~RickenmannBedloadCapacityBasedOnq()
{
	delete hidingFactorsCalculationMethod;
}

CalcBedloadCapacity* RickenmannBedloadCapacityBasedOnq::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new RickenmannBedloadCapacityBasedOnq(this->correctionForBedloadWeightAtSteepSlopes,this->hidingFactorsCalculationMethod);
	return result;
}

ConstructionVariables RickenmannBedloadCapacityBasedOnq::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::RickenmannBedloadCapacityBasedOnq);
	std::vector<bool> boolVector;
	boolVector.push_back(correctionForBedloadWeightAtSteepSlopes);
	result.labelledBools["correctionForBedloadWeightAtSteepSlopes"] = boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( hidingFactorsCalculationMethod->createConstructionVariables() );
	result.labelledObjects["hidingFactorsCalculationMethod"] = constructionVariablesVector;
	return result;
}

Grains RickenmannBedloadCapacityBasedOnq::calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
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
			double dNinetyOverDThirty = erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,90.0) / erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,30.0);
			double sMinusOne = ( (riverReachProperties.getOverallParameters())->getSedimentDensity() / (riverReachProperties.getOverallParameters())->getWaterDensity() ) - 1;
			double constantFactorsForQCritical = 0.065 * pow(sMinusOne,1.67) * sqrt( (riverReachProperties.getOverallParameters())->getGravityAcceleration() ) * pow( riverReachProperties.regularRiverReachProperties.bedslope, (-1.12) );
			std::vector<double> hidingFactors = hidingFactorsCalculationMethod->calculateFractionalHidingFactors(fractionalGrainDiameters,overallFractionalAbundances,riverReachProperties);
			std::vector<double> qCritical;
			qCritical.reserve( fractionalGrainDiameters.size() );
			for(std::vector<double>::const_iterator currentDiameter = fractionalGrainDiameters.begin(), currentHidingFactor = hidingFactors.begin(); currentDiameter < fractionalGrainDiameters.end(); ++currentDiameter, ++currentHidingFactor)
			{
				qCritical.push_back( ( constantFactorsForQCritical * (*currentDiameter) * sqrt(*currentDiameter) * (*currentHidingFactor) ) );
			}

			#if defined _DEBUG || defined DEBUG
				if (overallFractionalAbundances.size() != fractionalGrainDiameters.size() || overallFractionalAbundances.size() != qCritical.size())
				{
					const char *const secondErrorMessage = "Action on not matching number of grain fractions";
					throw(secondErrorMessage);
				}
			#endif


			// The following two lines calculate flow depth and width of a virtual rectangular channel corresponding to the mean flow depth within the active width.
			double flowDepth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->getMeanFlowDepthForActiveWidth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth, riverReachProperties.regularRiverReachProperties.activeWidth);
			double flowWidth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(riverReachProperties.regularRiverReachProperties.maximumWaterdepth) / flowDepth;

			double dischargePerUnitFlowWidth = riverReachProperties.regularRiverReachProperties.discharge / flowWidth;

			double correctedSedimentEnergySlope = riverReachProperties.regularRiverReachProperties.sedimentEnergyslope;
			if ( correctionForBedloadWeightAtSteepSlopes && (riverReachProperties.regularRiverReachProperties.bedslope > 0.0) )
			{
				correctedSedimentEnergySlope = CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedSedimentEnergySlope(correctedSedimentEnergySlope,(riverReachProperties.getOverallParameters())->getAngleOfReposeInRadians(), riverReachProperties.regularRiverReachProperties.bedslope);
			}

			double constantFactors = (1/overallVolume) * 3.1 * pow(sMinusOne,(-1.5)) * pow(dNinetyOverDThirty,0.2) * pow(correctedSedimentEnergySlope,1.5) * riverReachProperties.regularRiverReachProperties.activeWidth ;

			std::vector<double>::iterator resultIterator = resultOverallFractionalAbundances.begin();
			for(std::vector<double>::const_iterator currentQCritical = qCritical.begin(), currentFractionalAbundance = overallFractionalAbundances.begin(); currentFractionalAbundance < overallFractionalAbundances.end() ; ++resultIterator, ++currentQCritical, ++currentFractionalAbundance)
			{
				if( dischargePerUnitFlowWidth > (*currentQCritical) )
					{ *resultIterator = ( (*currentFractionalAbundance) * constantFactors * (dischargePerUnitFlowWidth - (*currentQCritical)) ); }
			}
		}
	}

	Grains result = Grains::distributeFractionalAbundance(resultOverallFractionalAbundances, *erosionBase);

	delete inputFromUpstream;

	return result;

}


}
