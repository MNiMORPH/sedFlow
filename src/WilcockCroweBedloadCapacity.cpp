/*
 * WilcockCroweBedload.cpp
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

#include "WilcockCroweBedloadCapacity.h"

#include <math.h>
#include <numeric>
#include <algorithm>
#include <functional>

#include "BedrockRoughnessContribution.h"
#include "BasicCalculations.h"

//This class is based on:
//			Wilcock & Crowe (2003): Surface-based Transport Model for Mixed-Size Sediment. JOURNAL OF HYDRAULIC ENGINEERING, Vol. 129, No. 2, 120–128;

namespace SedFlow {

WilcockCroweBedloadCapacity::WilcockCroweBedloadCapacity():
	useConstantSandFraction(false),
	constantSandFraction(0.4),
	breakPoint(1.35),
	firstExponentForRelativeShearStress(7.5),
	secondExponentForRelativeShearStress(0.5)
{}

WilcockCroweBedloadCapacity::WilcockCroweBedloadCapacity(bool useConstantSandFraction, double constantSandFraction):
	useConstantSandFraction(useConstantSandFraction),
	constantSandFraction(constantSandFraction),
	breakPoint(1.35),
	firstExponentForRelativeShearStress(7.5),
	secondExponentForRelativeShearStress(0.5)
{}

CalcBedloadCapacity* WilcockCroweBedloadCapacity::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new WilcockCroweBedloadCapacity(this->useConstantSandFraction,this->constantSandFraction);
	return result;
}

ConstructionVariables WilcockCroweBedloadCapacity::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::WilcockCroweBedloadCapacity);
	std::vector<bool> boolVector;
	boolVector.push_back(useConstantSandFraction);
	result.labelledBools["useConstantSandFraction"] = boolVector;
	std::vector<double> doubleVector;
	doubleVector.push_back(constantSandFraction);
	result.labelledDoubles["constantSandFraction"] = doubleVector;
	return result;
}

double WilcockCroweBedloadCapacity::getInterpolatedMeanDiameterForHiding (double meanDiameter, const std::vector<double>& fractionalGrainDiameters, const RiverReachProperties& riverReachProperties) const
{
	double bedrockContribution = BedrockRoughnessContribution::getBedrockRoughnessContribution(riverReachProperties);
	if( bedrockContribution > 0.0 )
	{
		double bedrockRepresentativeGrainDiameter = fractionalGrainDiameters.front();//On rock all grains are completely exposed, thus the smallest diameter is used as reference.
		if( meanDiameter != meanDiameter )
		{
			meanDiameter = bedrockRepresentativeGrainDiameter;
		}
		else
		{
			meanDiameter = ( (meanDiameter * (1.0-bedrockContribution) ) + (bedrockRepresentativeGrainDiameter * bedrockContribution) );
		}
	}
	return meanDiameter;
}

double WilcockCroweBedloadCapacity::calculateDimensionlessReferenceShearStressForMeanDiameter (const RiverReachProperties& riverReachProperties, const Grains* const erosionBase, const std::vector<double>& fractionalGrainDiameters) const
{
	double sandFraction;
	if(useConstantSandFraction)
		{ sandFraction = constantSandFraction; }
	else
		{ sandFraction = erosionBase->getOverallVolumeWithinDiameterRange(fractionalGrainDiameters, 0.0000625, 0.002) / erosionBase->getOverallVolume(); } //This assumes that grain diameters are expressed in [m]

	return ( 0.021 + (0.015 * exp( (-20 * sandFraction) ) ) ); //tau_rm* from equation (6)
}

Grains WilcockCroweBedloadCapacity::calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	const Grains* erosionBase = &(riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0));
	std::vector<double> sedimentTransportCapacitiesOverallFractionalAbundances ( erosionBase->getNumberOfFractions(), 0.0 );
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
					const char *const errorMessage = "Action on not matching number of grain fractions";
					throw(errorMessage);
				}
			#endif

			double meanDiameter = erosionBase->getGeometricMeanGrainDiameter(fractionalGrainDiameters);
			double oneOverMeanDiameterForHiding = 1.0 / getInterpolatedMeanDiameterForHiding(meanDiameter,fractionalGrainDiameters,riverReachProperties);
			double gravity = (riverReachProperties.getOverallParameters())->getGravityAcceleration();

			double shearVelocity = sqrt( riverReachProperties.regularRiverReachProperties.bedShearStress / (riverReachProperties.getOverallParameters())->getWaterDensity() ); //u* in text below equation (2)

			double dimensionlessReferenceShearStressForMeanDiameter = this->calculateDimensionlessReferenceShearStressForMeanDiameter(riverReachProperties,erosionBase,fractionalGrainDiameters);
			double sedimentDensityPerUnitWaterDensity = (riverReachProperties.getOverallParameters())->getSedimentDensity()/(riverReachProperties.getOverallParameters())->getWaterDensity();
			double referenceShearStressForMeanDiameter = dimensionlessReferenceShearStressForMeanDiameter * (sedimentDensityPerUnitWaterDensity-1) * (riverReachProperties.getOverallParameters())->getWaterDensity() * (riverReachProperties.getOverallParameters())->getGravityAcceleration() * meanDiameter ; //tau_rm from equation (5)
			double tempReferenceShearStress;
			double tempRelativeShearStress;
			double tempDimensionlessSedimentTransportCapacity;
			//double breakPoint; if(useVersionOfSchneiderEtAl) { breakPoint = 1.31; } else { breakPoint = 1.35; }
			//double firstExponentForRelativeShearStress; if(useVersionOfSchneiderEtAl) { firstExponentForRelativeShearStress = 14.1; } else { firstExponentForRelativeShearStress = 7.5; }
			//double secondExponentForRelativeShearStress; if(useVersionOfSchneiderEtAl) { secondExponentForRelativeShearStress = 0.9; } else { secondExponentForRelativeShearStress = 0.5; }
			std::vector<double>::iterator currentResultFraction = sedimentTransportCapacitiesOverallFractionalAbundances.begin();
			for(std::vector<double>::const_iterator currentDiameter = fractionalGrainDiameters.begin(), currentFractionalAbundance = overallFractionalAbundances.begin(); currentFractionalAbundance < overallFractionalAbundances.end(); ++currentResultFraction, ++currentDiameter, ++currentFractionalAbundance)
			{
				tempReferenceShearStress = ( referenceShearStressForMeanDiameter * pow( ((*currentDiameter) * oneOverMeanDiameterForHiding) , ( 0.67 / ( 1+exp(1.5-((*currentDiameter) * oneOverMeanDiameterForHiding)) ) ) ) ); //tau_ri from a combination of the equations (3) and (4)
				tempRelativeShearStress = riverReachProperties.regularRiverReachProperties.bedShearStress / tempReferenceShearStress; //Phi from text below equation (7)
				if( tempRelativeShearStress < breakPoint ) { tempDimensionlessSedimentTransportCapacity = 0.002 * pow( tempRelativeShearStress , firstExponentForRelativeShearStress); } //W_i* from equation (7a)
				else { tempDimensionlessSedimentTransportCapacity = 14 * pow( ( 1-( 0.894/pow(tempRelativeShearStress,secondExponentForRelativeShearStress) ) ) , 4.5); } //W_i* from equation (7b)
				*currentResultFraction = ( riverReachProperties.regularRiverReachProperties.activeWidth * ((*currentFractionalAbundance)/overallVolume) * tempDimensionlessSedimentTransportCapacity * pow(shearVelocity,3) / ( (sedimentDensityPerUnitWaterDensity-1) * gravity )  );//q_bi from equation (2)
			}
		}
	}

	Grains result = Grains::distributeFractionalAbundance(sedimentTransportCapacitiesOverallFractionalAbundances, *erosionBase);

	delete inputFromUpstream;

	return result;
}

}
