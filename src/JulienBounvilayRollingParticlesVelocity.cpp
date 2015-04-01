/*
 * JulienBounvilayRollingParticlesVelocity.cpp
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

#include "JulienBounvilayRollingParticlesVelocity.h"

#include <math.h>
#include <algorithm>

#include "BasicCalculations.h"
#include "BedrockRoughnessContribution.h"

namespace SedFlow {

JulienBounvilayRollingParticlesVelocity::JulienBounvilayRollingParticlesVelocity(double roughnessDiameterPercentile, double roughnessDiameterFactor):
	roughnessDiameterPercentile(roughnessDiameterPercentile),
	roughnessDiameterFactor(roughnessDiameterFactor)
{}

JulienBounvilayRollingParticlesVelocity::~JulienBounvilayRollingParticlesVelocity(){}

CalcBedloadVelocity* JulienBounvilayRollingParticlesVelocity::createCalcBedloadVelocityPointerCopy() const
{
	CalcBedloadVelocity* result = new JulienBounvilayRollingParticlesVelocity(this->roughnessDiameterPercentile,this->roughnessDiameterFactor);
	return result;
}

ConstructionVariables JulienBounvilayRollingParticlesVelocity::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadVelocity;
	result.realisationType = CombinerVariables::typeOfBedloadVelocityCalculationMethodToString(CombinerVariables::JulienBounvilayRollingParticlesVelocity);
	std::vector<double> doubleVector;
	doubleVector.push_back(roughnessDiameterPercentile);
	result.labelledDoubles["roughnessDiameterPercentile"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(roughnessDiameterFactor);
	result.labelledDoubles["roughnessDiameterFactor"] = doubleVector;
	return result;
}

double JulienBounvilayRollingParticlesVelocity::calculate (const RiverReachProperties& riverReachProperties) const
{
	double result = 0.0;
	if( riverReachProperties.regularRiverReachProperties.erosionRate.getOverallVolume() > 0.0 )
	{
		const OverallParameters* overallParameters = riverReachProperties.getOverallParameters();
		double roughnessDiameter = roughnessDiameterFactor * Grains::getPercentileGrainDiameter( overallParameters->getFractionalGrainDiameters() , (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getOverallFractionalAbundance() , roughnessDiameterPercentile);
		double bedrockContribution = BedrockRoughnessContribution::getBedrockRoughnessContribution(riverReachProperties);
		if( bedrockContribution > 0.0 )
		{
			double bedrockRoughnessDiameter = BedrockRoughnessContribution::getBedrockRoughnessEquivalentRepresentativeGrainDiameterValue(riverReachProperties);
			if( roughnessDiameter != roughnessDiameter )
			{
				roughnessDiameter = bedrockRoughnessDiameter;
			}
			else
			{
				roughnessDiameter = ( (roughnessDiameter * (1.0-bedrockContribution) ) + (bedrockRoughnessDiameter * bedrockContribution) );
			}
		}
		double roughnessTheta = BasicCalculations::calculateDimensionlessShearStress(roughnessDiameter,riverReachProperties.regularRiverReachProperties);
		double shearVelocity = sqrt( (overallParameters->getGravityAcceleration() * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoHydraulicRadius(riverReachProperties.regularRiverReachProperties.maximumWaterdepth) * riverReachProperties.regularRiverReachProperties.sedimentEnergyslope) );
		// Equation 11
		result = std::max( 0.0, (shearVelocity * ( ( 3.3 * log(roughnessTheta) ) + 17.7 )) );
	}
	return result;
}

}
