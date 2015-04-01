/*
 * PowerLawHidingFunction.cpp
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

#include <math.h>

#include "PowerLawHidingFunction.h"

#include "BedrockRoughnessContribution.h"

namespace SedFlow {

PowerLawHidingFunction::PowerLawHidingFunction(double referenceDiameterPercentile, double exponent):
	referenceDiameterPercentile(referenceDiameterPercentile),
	exponent(exponent)
{
	if( (exponent > 0.0) || (exponent < (-2.5)) )
	{
		const char *const exponentOutOfRangeErrorMessage = "No values smaller than -2.5 or larger than 0.0 are allowed for the exponent in PowerLawHidingFunction. (The value of -1.0 represents equal mobility.)";
		throw(exponentOutOfRangeErrorMessage);
	}
}

PowerLawHidingFunction::~PowerLawHidingFunction() {}

CalcHidingFactors* PowerLawHidingFunction::createCalcHidingFactorsMethodPointerCopy() const
{
	CalcHidingFactors* result = new PowerLawHidingFunction(this->referenceDiameterPercentile, this->exponent);
	return result;
}


ConstructionVariables PowerLawHidingFunction::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcHidingFactors;
	result.realisationType = CombinerVariables::typeOfHidingFactorsCalculationMethodToString(CombinerVariables::PowerLawHidingFunction);
	std::vector<double> doubleVector;
	doubleVector.push_back(referenceDiameterPercentile);
	result.labelledDoubles["referenceDiameterPercentile"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(exponent);
	result.labelledDoubles["exponent"] = doubleVector;
	return result;
}

double PowerLawHidingFunction::getInterpolatedReferenceGrainDiameter (const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, const RiverReachProperties& riverReachProperties) const
{
	double referenceGrainDiameter = Grains::getPercentileGrainDiameter(fractionalGrainDiameters,fractionalAbundances,referenceDiameterPercentile);
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

std::vector<double> PowerLawHidingFunction::calculateFractionalHidingFactors (const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, const RiverReachProperties& riverReachProperties) const
{
	std::vector<double>fractionalHidingFactors(fractionalGrainDiameters);
	double referenceGrainDiameter = this->getInterpolatedReferenceGrainDiameter(fractionalGrainDiameters,fractionalAbundances,riverReachProperties);
	for(std::vector<double>::iterator currentHidingFactor = fractionalHidingFactors.begin(); currentHidingFactor < fractionalHidingFactors.end(); ++currentHidingFactor)
	{
		*currentHidingFactor = pow( (*currentHidingFactor/referenceGrainDiameter), this->exponent );
	}
	return fractionalHidingFactors;
}

}
