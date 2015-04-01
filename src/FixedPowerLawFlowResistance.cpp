/*
 * FixedPowerLawFlowResistance.cpp
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

#include "FixedPowerLawFlowResistance.h"
#include <math.h>
#include <limits>

#include "BedrockRoughnessContribution.h"

namespace SedFlow {

FixedPowerLawFlowResistance::FixedPowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, double factor, double grainsFactor, double grainsPercentile, double exponent):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder),
	factor(factor),
	grainsFactor(grainsFactor),
	grainsPercentile(grainsPercentile),
	exponent(exponent)
{
	this->typeOfFlowResistance = CombinerVariables::FixedPowerLawFlowResistance;
}

FixedPowerLawFlowResistance::FixedPowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, double factor, double grainsFactor, double grainsPercentile, double exponent, const std::set<int>& everTreatedCellIDs):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,everTreatedCellIDs),
	factor(factor),
	grainsFactor(grainsFactor),
	grainsPercentile(grainsPercentile),
	exponent(exponent)
{
	this->typeOfFlowResistance = CombinerVariables::FixedPowerLawFlowResistance;
}

FixedPowerLawFlowResistance::~FixedPowerLawFlowResistance()
{}

FlowResistance* FixedPowerLawFlowResistance::createFlowResistancePointerCopy() const
{
	FlowResistance* result = new FixedPowerLawFlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,factor,grainsFactor,grainsPercentile,exponent,everTreatedCellIDs);
	return result;
}

double FixedPowerLawFlowResistance::getInterpolatedRepresentativeGrainDiameter(const RiverReachProperties& riverReachProperties) const
{
	double representativeGrainDiameter = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(),this->grainsPercentile);
	double bedrockContribution = BedrockRoughnessContribution::getBedrockRoughnessContribution(riverReachProperties);
	if( bedrockContribution > 0.0 )
	{
		double bedrockRepresentativeGrainDiameter = BedrockRoughnessContribution::getBedrockRoughnessEquivalentRepresentativeGrainDiameterValue(riverReachProperties);
		if( representativeGrainDiameter != representativeGrainDiameter )
		{
			representativeGrainDiameter = bedrockRepresentativeGrainDiameter;
		}
		else
		{
			representativeGrainDiameter = ( (representativeGrainDiameter * (1.0-bedrockContribution) ) + (bedrockRepresentativeGrainDiameter * bedrockContribution) );
		}
	}
	return representativeGrainDiameter;
}


ConstructionVariables FixedPowerLawFlowResistance::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowResistance;
	result.realisationType = CombinerVariables::typeOfFlowResistanceToString(CombinerVariables::FixedPowerLawFlowResistance);
	std::vector<double> doubleVector;
	doubleVector.push_back(startingValueForIteration);
	result.labelledDoubles["startingValueForIteration"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(accuracyForTerminatingIteration);
	result.labelledDoubles["accuracyForTerminatingIteration"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(maximumNumberOfIterations);
	result.labelledInts["maximumNumberOfIterations"] = intVector;
	std::vector<bool> boolVector;
	boolVector.push_back(useApproximationsForHydraulicRadius);
	result.labelledBools["useApproximationsForHydraulicRadius"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(maximumFroudeNumber);
	result.labelledDoubles["maximumFroudeNumber"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(minimumHydraulicSlope);
	result.labelledDoubles["minimumHydraulicSlope"] = doubleVector;
	std::vector<std::string> stringVector;
	stringVector.push_back(getTypeOfNumericRootFinderAsString());
	result.labelledStrings["typeOfNumericRootFinder"] = stringVector;
	doubleVector.clear();
	doubleVector.push_back(factor);
	result.labelledDoubles["factor"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(grainsFactor);
	result.labelledDoubles["grainsFactor"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(grainsPercentile);
	result.labelledDoubles["grainsPercentile"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(exponent);
	result.labelledDoubles["exponent"] = doubleVector;
	return result;
}

PowerLawRelation FixedPowerLawFlowResistance::dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const
{
	PowerLawRelation result;
	double representativeGrainDiameter = this->getInterpolatedRepresentativeGrainDiameter(riverReachProperties);

	PowerLawRelation hydraulicRadiusAsFunctionOfCrossSectionalArea = this->hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea(riverReachProperties);

	if (hydraulicRadiusAsFunctionOfCrossSectionalArea.powerLawCheck() && hydraulicRadiusAsFunctionOfCrossSectionalArea.addition() == 0.0)
	{
		double resultExponent = 1.0 + ( hydraulicRadiusAsFunctionOfCrossSectionalArea.exponent() * (this->exponent + 0.5) );
		double exponentiatedLength = pow(riverReachProperties.regularRiverReachProperties.length,hydraulicRadiusAsFunctionOfCrossSectionalArea.exponent());
		double resultFactor = sqrt( ((riverReachProperties.getOverallParameters())->getGravityAcceleration() * riverReachProperties.regularRiverReachProperties.waterEnergyslope * hydraulicRadiusAsFunctionOfCrossSectionalArea.factor() / exponentiatedLength) ) * pow( (hydraulicRadiusAsFunctionOfCrossSectionalArea.factor() / (exponentiatedLength * this->grainsFactor * representativeGrainDiameter)) , this->exponent ) * (this->factor / riverReachProperties.regularRiverReachProperties.length);
		result = PowerLawRelation(0.0,resultFactor,resultExponent);
	}
	else
	{ result = PowerLawRelation(false); }

	return result;
}

std::pair<double,double> FixedPowerLawFlowResistance::calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	double representativeGrainDiameter = this->getInterpolatedRepresentativeGrainDiameter(riverReachProperties);
	double hydraulicRadius = this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties);

	double flowVelocity = sqrt( ((riverReachProperties.getOverallParameters())->getGravityAcceleration() * hydraulicRadius * riverReachProperties.regularRiverReachProperties.waterEnergyslope) ) * this->factor * pow( (hydraulicRadius/(this->grainsFactor*representativeGrainDiameter)) ,this->exponent);
	double discharge = flowVelocity * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
	std::pair<double,double> result (discharge,flowVelocity);
	return result;
}

std::pair<double,double> FixedPowerLawFlowResistance::calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	double representativeGrainDiameter = this->getInterpolatedRepresentativeGrainDiameter(riverReachProperties);
	double hydraulicRadius = pow( ( flowVelocity * pow((this->grainsFactor*representativeGrainDiameter),this->exponent) / (this->factor * sqrt(((riverReachProperties.getOverallParameters())->getGravityAcceleration() * riverReachProperties.regularRiverReachProperties.waterEnergyslope))) ) , (1 / (this->exponent+0.5)) );
	double flowDepth = this->convertHydraulicRadiusIntoMaximumFlowDepth(hydraulicRadius,riverReachProperties);
	double discharge = flowVelocity * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
	std::pair<double,double> result (discharge,flowVelocity);
	return result;
}

double FixedPowerLawFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	double representativeGrainDiameter = this->getInterpolatedRepresentativeGrainDiameter(riverReachProperties);
	double hydraulicRadius = this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties);
	double sqrtEightOverF = this->factor * pow((hydraulicRadius / (this->grainsFactor * representativeGrainDiameter)),this->exponent);
	return (8.0 / (sqrtEightOverF * sqrtEightOverF));
}

PowerLawRelation FixedPowerLawFlowResistance::darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const
{
	PowerLawRelation result (false);
	PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfFlowDepth = this->hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth(riverReachProperties);
	if( hydraulicRadiusAsPowerLawFunctionOfFlowDepth.powerLawCheck() && (hydraulicRadiusAsPowerLawFunctionOfFlowDepth.addition() == 0.0) )
	{
		double representativeGrainDiameter = this->getInterpolatedRepresentativeGrainDiameter(riverReachProperties);
		double resultFactor = pow( (this->grainsFactor * representativeGrainDiameter / hydraulicRadiusAsPowerLawFunctionOfFlowDepth.factor()) , (2.0 * this->exponent) ) * 8.0 / (this->factor * this->factor);
		double resultExponent = hydraulicRadiusAsPowerLawFunctionOfFlowDepth.exponent() - (2.0 * this->exponent);
		result = PowerLawRelation(0.0,resultFactor,resultExponent);
	}
	return result;
}

double FixedPowerLawFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method FixedPowerLawFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity has not been implemented yet.";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > FixedPowerLawFlowResistance::getInternalParameters() const
{
	std::vector<double> resultVector;
	resultVector.reserve(4);
	resultVector.push_back(this->factor);
	resultVector.push_back(this->grainsFactor);
	resultVector.push_back(this->grainsPercentile);
	resultVector.push_back(this->exponent);
	return std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > (CombinerVariables::FixedPowerLawFlowResistance, resultVector);
}

void FixedPowerLawFlowResistance::setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters)
{
	if( newParameters.first != CombinerVariables::VariablePowerLawFlowResistance)
	{
		const char *const errorMessage = "Action on not matching types of flow resistance";
		throw(errorMessage);
	}
	if( newParameters.second.size() != 4 )
	{
		const char *const errorMessage = "FixedPowerLawFlowResistance only accepts new parametersets of the size four.";
		throw(errorMessage);
	}

	this->factor = newParameters.second.at(0);
	this->grainsFactor = newParameters.second.at(1);
	this->grainsPercentile = newParameters.second.at(2);
	this->exponent = newParameters.second.at(3);
}

}
