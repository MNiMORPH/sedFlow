/*
 * WilcockCroweHidingFunction.cpp
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

#include "WilcockCroweHidingFunction.h"

#include <math.h>

#include "BedrockRoughnessContribution.h"

namespace SedFlow {

WilcockCroweHidingFunction::WilcockCroweHidingFunction() {}

WilcockCroweHidingFunction::~WilcockCroweHidingFunction() {}

CalcHidingFactors* WilcockCroweHidingFunction::createCalcHidingFactorsMethodPointerCopy() const
{
	CalcHidingFactors* result = new WilcockCroweHidingFunction();
	return result;
}


ConstructionVariables WilcockCroweHidingFunction::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcHidingFactors;
	result.realisationType = CombinerVariables::typeOfHidingFactorsCalculationMethodToString(CombinerVariables::WilcockCroweHidingFunction);
	return result;
}

double WilcockCroweHidingFunction::getInterpolatedReferenceGrainDiameter (const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, const RiverReachProperties& riverReachProperties) const
{
	double referenceGrainDiameter = Grains::getPercentileGrainDiameter(fractionalGrainDiameters,fractionalAbundances,50.0);
	double bedrockContribution = BedrockRoughnessContribution::getBedrockRoughnessContribution(riverReachProperties);
	if( bedrockContribution > 0.0 )
	{
		double bedrockRepresentativeGrainDiameter = fractionalGrainDiameters.front();//On rock all grains are completely exposed, thus the smallest diameter is used as reference.
		if( referenceGrainDiameter != referenceGrainDiameter )
		{
			referenceGrainDiameter = bedrockRepresentativeGrainDiameter;
		}
		else
		{
			referenceGrainDiameter = ( (referenceGrainDiameter * (1.0-bedrockContribution) ) + (bedrockRepresentativeGrainDiameter * bedrockContribution) );
		}
	}
	return referenceGrainDiameter;
}

std::vector<double> WilcockCroweHidingFunction::calculateFractionalHidingFactors (const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, const RiverReachProperties& riverReachProperties) const
{
	std::vector<double>fractionalHidingFactors(fractionalGrainDiameters);
	double oneOverReferenceGrainDiameter = 1.0 / this->getInterpolatedReferenceGrainDiameter(fractionalGrainDiameters,fractionalAbundances,riverReachProperties);
	double exponent;
	for(std::vector<double>::iterator currentHidingFactor = fractionalHidingFactors.begin(); currentHidingFactor < fractionalHidingFactors.end(); ++currentHidingFactor)
	{
		exponent = ( 0.67 / ( 1 + exp(1.5-(*currentHidingFactor * oneOverReferenceGrainDiameter)) ) ) - 1.0;
		*currentHidingFactor = pow( (*currentHidingFactor * oneOverReferenceGrainDiameter), exponent );
	}
	return fractionalHidingFactors;
}

}
