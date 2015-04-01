/*
 * DarcyWeisbachFlowResistance.cpp
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

#include "DarcyWeisbachFlowResistance.h"

#include <math.h>
#include <limits>

#include "BedrockRoughnessContribution.h"

namespace SedFlow {

DarcyWeisbachFlowResistance::DarcyWeisbachFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, double darcyWeisbachFrictionFactorForBedrock, double darcyWeisbachFrictionFactorForGravelbed):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder),
	darcyWeisbachFrictionFactorForGravelbed(darcyWeisbachFrictionFactorForGravelbed),
	darcyWeisbachFrictionFactorForBedrock(darcyWeisbachFrictionFactorForBedrock)
{
	this->typeOfFlowResistance = CombinerVariables::DarcyWeisbachFlowResistance;
	#if defined _DEBUG || defined DEBUG
		if ( darcyWeisbachFrictionFactorForGravelbed <= 0.0 || darcyWeisbachFrictionFactorForBedrock <= 0.0 )
		{
			const char *const errorMessage = "Negative or zero Darcy-Weisbach friction factor.";
			throw(errorMessage);
		}
	#endif
}

DarcyWeisbachFlowResistance::DarcyWeisbachFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, double darcyWeisbachFrictionFactorForBedrock, double darcyWeisbachFrictionFactorForGravelbed, const std::set<int>& everTreatedCellIDs):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,everTreatedCellIDs),
	darcyWeisbachFrictionFactorForGravelbed(darcyWeisbachFrictionFactorForGravelbed),
	darcyWeisbachFrictionFactorForBedrock(darcyWeisbachFrictionFactorForBedrock)
{
	this->typeOfFlowResistance = CombinerVariables::DarcyWeisbachFlowResistance;
	#if defined _DEBUG || defined DEBUG
		if ( darcyWeisbachFrictionFactorForGravelbed <= 0.0 || darcyWeisbachFrictionFactorForBedrock <= 0.0 )
		{
			const char *const errorMessage = "Negative or zero Darcy-Weisbach friction factor.";
			throw(errorMessage);
		}
	#endif
}

DarcyWeisbachFlowResistance::~DarcyWeisbachFlowResistance() {}

FlowResistance* DarcyWeisbachFlowResistance::createFlowResistancePointerCopy() const
{
	FlowResistance* result = new DarcyWeisbachFlowResistance(this->startingValueForIteration,this->accuracyForTerminatingIteration,this->maximumNumberOfIterations,this->useApproximationsForHydraulicRadius,this->maximumFroudeNumber,this->minimumHydraulicSlope,this->typeOfNumericRootFinder,this->darcyWeisbachFrictionFactorForBedrock,this->darcyWeisbachFrictionFactorForGravelbed,this->everTreatedCellIDs);
	return result;
}

double DarcyWeisbachFlowResistance::getInterpolatedFrictionFactor(const RiverReachProperties& riverReachProperties) const
{
	double bedrockRoughnessContribution = BedrockRoughnessContribution::getBedrockRoughnessContribution(riverReachProperties);
	return ( ( darcyWeisbachFrictionFactorForGravelbed * (1.0-bedrockRoughnessContribution) ) + ( darcyWeisbachFrictionFactorForBedrock * bedrockRoughnessContribution ) );
}

ConstructionVariables DarcyWeisbachFlowResistance::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowResistance;
	result.realisationType = CombinerVariables::typeOfFlowResistanceToString(CombinerVariables::DarcyWeisbachFlowResistance);
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
	doubleVector.push_back(darcyWeisbachFrictionFactorForBedrock);
	result.labelledDoubles["darcyWeisbachFrictionFactorForBedrock"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(darcyWeisbachFrictionFactorForGravelbed);
	result.labelledDoubles["darcyWeisbachFrictionFactorForGravelbed"] = doubleVector;
	return result;
}


PowerLawRelation DarcyWeisbachFlowResistance::dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const
{
	PowerLawRelation result;

	double sqrtEightOverF = sqrt( ( 8.0 / this->getInterpolatedFrictionFactor(riverReachProperties) ) );

	result = this->dischargeAsPowerLawFunctionOfWaterVolumeInReachForGivenSquareRootEightOverDarcyWeisbachFrictionFactor(riverReachProperties, sqrtEightOverF);

	return result;
}

PowerLawRelation DarcyWeisbachFlowResistance::dischargeAsPowerLawFunctionOfWaterVolumeInReachForGivenSquareRootEightOverDarcyWeisbachFrictionFactor(const RiverReachProperties& riverReachProperties, double sqrtEightOverF) const
{
	PowerLawRelation result;

	PowerLawRelation hydraulicRadiusAsFunctionOfCrossSectionalArea = this->hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea(riverReachProperties);

	if (hydraulicRadiusAsFunctionOfCrossSectionalArea.powerLawCheck() && hydraulicRadiusAsFunctionOfCrossSectionalArea.addition() == 0.0)
	{
		double resultExponent = 1.0 + ( hydraulicRadiusAsFunctionOfCrossSectionalArea.exponent() * 0.5 );
		double resultFactor = sqrtEightOverF * sqrt( ((riverReachProperties.getOverallParameters())->getGravityAcceleration() * hydraulicRadiusAsFunctionOfCrossSectionalArea.factor() * riverReachProperties.regularRiverReachProperties.waterEnergyslope  / pow(riverReachProperties.regularRiverReachProperties.length,hydraulicRadiusAsFunctionOfCrossSectionalArea.exponent()) ) )  / riverReachProperties.regularRiverReachProperties.length;
		result = PowerLawRelation(0.0,resultFactor,resultExponent);
	}
	else
	{ result = PowerLawRelation(false); }

	return result;
}

double DarcyWeisbachFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	return this->getInterpolatedFrictionFactor(riverReachProperties);
}

PowerLawRelation DarcyWeisbachFlowResistance::darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const
{
	return PowerLawRelation(0.0,(this->getInterpolatedFrictionFactor(riverReachProperties)),0.0);
}

double DarcyWeisbachFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity (double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	return this->getInterpolatedFrictionFactor(riverReachProperties);
}

std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > DarcyWeisbachFlowResistance::getInternalParameters() const
{
	std::vector<double> resultVector (1,darcyWeisbachFrictionFactorForGravelbed);
	resultVector.push_back(darcyWeisbachFrictionFactorForBedrock);
	return std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > (CombinerVariables::DarcyWeisbachFlowResistance, resultVector);
}

void DarcyWeisbachFlowResistance::setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters)
{
	if( newParameters.first != CombinerVariables::DarcyWeisbachFlowResistance)
	{
		const char *const errorMessage = "Action on not matching types of flow resistance";
		throw(errorMessage);
	}
	if( newParameters.second.size() != 2 )
	{
		const char *const errorMessage = "DarcyWeisbachFlowResistance only accepts new parametersets of the size two.";
		throw(errorMessage);
	}

	this->darcyWeisbachFrictionFactorForGravelbed = newParameters.second.at(0);
	this->darcyWeisbachFrictionFactorForBedrock = newParameters.second.at(1);
}

}
