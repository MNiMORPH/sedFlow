/*
 * FlowResistance.cpp
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

#include "FlowResistance.h"

#include <iostream>
#include <math.h>
#include <limits>
#include <sstream>

#include "BasicCalculations.h"

namespace SedFlow {

//////////////////////////////////////////////////////////////////////////////////////////////////////
//TODO Remove these debugging lines
/*
std::pair<double,double> Debugging_FlowResistance_findPositiveBracketsStartingFromBoundary(const std::binder2nd< FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead>& function, double boundary, bool isLowerBoundary, int maximumNumberOfIterations)
{
	double factor,currentBracket,currentBracketValue;
	if(isLowerBoundary) { factor = 2.0; } else { factor = 0.5; }
	double previousBracket (boundary);
	double previousBracketValue (BasicCalculations::signum(function(boundary)));
	for(int i = 1; i <= maximumNumberOfIterations; ++i)
	{
		currentBracket = previousBracket * factor;
		currentBracketValue = BasicCalculations::signum(function(currentBracket));
		if(currentBracketValue != previousBracketValue)
		{
			if(isLowerBoundary)
				{ return std::pair<double,double> (previousBracket,currentBracket); }
			else
				{ return std::pair<double,double> (currentBracket,previousBracket); }
		}
		previousBracket = currentBracket;
		previousBracketValue = currentBracketValue;
	}


	const char *const maxIterationErrorMessage = "Debug_BasicCalculations::findPositiveBracketsStartingFromBoundary: Maximum number of iterations exceeded.";
	throw(maxIterationErrorMessage);
	return std::pair<double,double>(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN());
}

std::pair<double,double> Debugging_FlowResistance_findPositiveBracketsStartingFromExpectedValue(const std::binder2nd< FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero >& function, double expectedValue, bool abortAtAmbiguity, int maximumNumberOfIterations, bool useLowerBracketsInCaseOfAmbiguity)
{
	double  lowerBracket, upperBracket, previousLowerBracketValue, previousUpperBracketValue, lowerBracketValue, upperBracketValue;
	double previousLowerBracket (expectedValue), previousUpperBracket (expectedValue);
	previousLowerBracketValue = previousUpperBracketValue = BasicCalculations::signum(function(expectedValue));
	for(int i = 1; i <= maximumNumberOfIterations; ++i)
	{
		lowerBracket = previousLowerBracket * 0.5;
		upperBracket = previousUpperBracket * 2.0;
		lowerBracketValue = BasicCalculations::signum(function(lowerBracket));
		upperBracketValue = BasicCalculations::signum(function(upperBracket));
		if( lowerBracketValue != upperBracketValue || lowerBracketValue != previousLowerBracketValue || upperBracketValue != previousUpperBracketValue )
		{
			if( lowerBracketValue == upperBracketValue )//Ambiguity!!
			{
				if(abortAtAmbiguity)
				{
					const char *const ambiguityErrorMessage = "BasicCalculations::findPositiveBracketsStartingFromExpectedValue: Ambiguous result.";
					throw(ambiguityErrorMessage);
					return std::pair<double,double>(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN());
				}
				else
				{
					if(useLowerBracketsInCaseOfAmbiguity)
						{ return std::pair<double,double>(lowerBracket,previousLowerBracket); }
					else
						{ return std::pair<double,double>(previousUpperBracket,upperBracket); }
				}
			}

			if( lowerBracketValue != previousLowerBracketValue ) { return std::pair<double,double>(lowerBracket,previousLowerBracket); }
			if( upperBracketValue != previousUpperBracketValue ) { return std::pair<double,double>(previousUpperBracket,upperBracket); }
			return std::pair<double,double>(lowerBracket,upperBracket);
		}
		previousLowerBracket = lowerBracket;
		previousUpperBracket = upperBracket;
		previousLowerBracketValue = lowerBracketValue;
		previousUpperBracketValue = upperBracketValue;
	}


	const char *const maxIterationErrorMessage = "BasicCalculations::findPositiveBracketsStartingFromExpectedValue: Maximum number of iterations exceeded.";
	throw(maxIterationErrorMessage);
	return std::pair<double,double>(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN());
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////

PowerLawRelation FlowResistance::hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea(const RiverReachProperties& riverReachProperties) const
{
	PowerLawRelation result;
	if(useApproximationsForHydraulicRadius)
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->hydraulicRadiusAsPowerLawFunctionOfCrossSectionalAreaUsingApproximations();
	}
	else
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea();
	}
	return result;
}

PowerLawRelation FlowResistance::hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth(const RiverReachProperties& riverReachProperties) const
{
	PowerLawRelation result;
	if(useApproximationsForHydraulicRadius)
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepthUsingApproximations();
	}
	else
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth();
	}
	return result;
}

double FlowResistance::convertMaximumFlowDepthIntoHydraulicRadius(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	double result;
	if(useApproximationsForHydraulicRadius)
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoHydraulicRadiusUsingApproximations(flowDepth);
	}
	else
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth);
	}
	return result;
}

double FlowResistance::convertHydraulicRadiusIntoMaximumFlowDepth(double hydraulicRadius, const RiverReachProperties& riverReachProperties) const
{
	double result;
	if(useApproximationsForHydraulicRadius)
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertHydraulicRadiusIntoMaximumFlowDepthUsingApproximations(hydraulicRadius);
	}
	else
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertHydraulicRadiusIntoMaximumFlowDepth(hydraulicRadius);
	}
	return result;
}

double FlowResistance::getHydraulicRadiusForActiveWidth(double maximumFlowDepth, double activeWidth, const RiverReachProperties& riverReachProperties) const
{
	double result;
	if(useApproximationsForHydraulicRadius)
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->getHydraulicRadiusForActiveWidthUsingApproximations(maximumFlowDepth,activeWidth);
	}
	else
	{
		result = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->getHydraulicRadiusForActiveWidth(maximumFlowDepth,activeWidth);
	}
	return result;
}

double FlowResistance::getStartingFlowDepthForIteration(const RiverReachProperties& riverReachProperties) const
{
	double result;
	if( this->everTreatedCellIDs.find(riverReachProperties.getCellID()) != this->everTreatedCellIDs.end() )
	{
		result = riverReachProperties.regularRiverReachProperties.maximumWaterdepth;
	}
	else
	{
		if(riverReachProperties.isDownstreamMargin())
		{
			result = this->startingValueForIteration;
		}
		else
		{
			result  = (riverReachProperties.getDownstreamCellPointer())->regularRiverReachProperties.maximumWaterdepth;
		}
		(const_cast<FlowResistance*>(this))->everTreatedCellIDs.insert( riverReachProperties.getCellID() );
	}
	return result;
}

std::pair<double,double> FlowResistance::getFlowDepthAndVelocityEnsuringMinimumHydraulicSlope(std::pair<double,double> previouslyCalculatedFlowDepthAndVelocity, double discharge, double minimumFlowDepth, double gravityAcceleration, const RiverReachProperties& riverReachProperties)const
{
	std::pair<double,double> result = previouslyCalculatedFlowDepthAndVelocity;
	double outputFlowDepth = std::max(result.first,minimumFlowDepth);

	if( (!(riverReachProperties.isDownstreamMargin())) )
	{
		double length = riverReachProperties.regularRiverReachProperties.length;
		double localElevation = riverReachProperties.regularRiverReachProperties.elevation;
		const RiverReachProperties& downstreamRiverReachProperties ( (*(riverReachProperties.getDownstreamCellPointer())) );
		bool downstreamSillOccurence = downstreamRiverReachProperties.regularRiverReachProperties.sillOccurence;
		double downstreamHydraulicHead = downstreamRiverReachProperties.regularRiverReachProperties.hydraulicHead();
		if( downstreamSillOccurence )
		{
			downstreamHydraulicHead = std::max(downstreamHydraulicHead,downstreamRiverReachProperties.sillProperties->calculateFreeOverfallHydraulicHeadAtSill(downstreamRiverReachProperties.regularRiverReachProperties.discharge, downstreamRiverReachProperties.regularRiverReachProperties, downstreamRiverReachProperties.additionalRiverReachProperties));
		}
		else
		{
			//The minimum water surface slope can only be ensured without downstream sills. Otherwise the downstream water level is unknown.
			double minimumFlowDepth = downstreamRiverReachProperties.regularRiverReachProperties.waterLevel() + (this->minimumHydraulicSlope * length) - localElevation;
			outputFlowDepth = std::max(outputFlowDepth,minimumFlowDepth);
		}

		double gravityAcceleration = (riverReachProperties.getOverallParameters())->getGravityAcceleration();
		double minimumHydraulicHead = downstreamHydraulicHead + (this->minimumHydraulicSlope * length);
		FlowResistance_BoundaryConditionsForCurrentHydraulicHeadMinusMinimumHydraulicHead boundaryConditions (minimumHydraulicHead,localElevation,discharge,gravityAcceleration,riverReachProperties.geometricalChannelBehaviour->alluviumChannel);
		std::binder2nd< FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead > currentMinusMinimumHydraulicHead (FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead(),boundaryConditions);

		if( currentMinusMinimumHydraulicHead(outputFlowDepth) < 0.0 )
		{
			double criticalFlowDepth = BasicCalculations::calculateMaximumFlowDepthForGivenFroudeAndDischarge(1.0,discharge,gravityAcceleration,riverReachProperties.geometricalChannelBehaviour->alluviumChannel,this->startingValueForIteration,this->accuracyForTerminatingIteration,this->maximumNumberOfIterations,this->maximumFroudeSolver);
			////////////////////////////////////////////////////////////////////////////
			//May be switched for Debugging.
			std::pair<double,double> brackets = BasicCalculations::findPositiveBracketsStartingFromBoundary< std::binder2nd< FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead > >(currentMinusMinimumHydraulicHead,criticalFlowDepth,true,maximumNumberOfIterations);
			//std::pair<double,double> brackets = Debugging_FlowResistance_findPositiveBracketsStartingFromBoundary(currentMinusMinimumHydraulicHead,criticalFlowDepth,true,maximumNumberOfIterations);
			////////////////////////////////////////////////////////////////////////////
			outputFlowDepth = this->minimumHydraulicSlopeSolver->findRoot(currentMinusMinimumHydraulicHead,brackets.first,brackets.second,accuracyForTerminatingIteration,maximumNumberOfIterations);
		}
	}

	if(outputFlowDepth != result.first)
	{
		double flowVelocity = discharge / riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(outputFlowDepth);
		result = std::pair<double,double>(outputFlowDepth,flowVelocity);
	}
	return result;
}

FlowResistance::FlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder):
	startingValueForIteration(startingValueForIteration),
	accuracyForTerminatingIteration(accuracyForTerminatingIteration),
	maximumNumberOfIterations(maximumNumberOfIterations),
	useApproximationsForHydraulicRadius(useApproximationsForHydraulicRadius),
	maximumFroudeNumber(maximumFroudeNumber),
	minimumHydraulicSlope(minimumHydraulicSlope),
	typeOfNumericRootFinder(typeOfNumericRootFinder)
{
	this->flowResistanceSolver = this->flowResistanceSolverBuilder.numericRootFinderBuilder(typeOfNumericRootFinder);
	this->maximumFroudeSolver = this->maximumFroudeSolverBuilder.numericRootFinderBuilder(typeOfNumericRootFinder);
	this->minimumHydraulicSlopeSolver = this->minimumHydraulicSlopeSolverBuilder.numericRootFinderBuilder(typeOfNumericRootFinder);
	if(this->startingValueForIteration <= 0.0)
	{
		const char *const startingValueForIterationErrorMessage = "startingValueForIteration needs to be positive. (FlowResistance)";
		throw(startingValueForIterationErrorMessage);
	}

	if(this->accuracyForTerminatingIteration <= 0.0)
	{
		const char *const accuracyForTerminatingIterationErrorMessage = "accuracyForTerminatingIteration needs to be positive. (FlowResistance)";
		throw(accuracyForTerminatingIterationErrorMessage);
	}

	if(this->maximumNumberOfIterations <= 0)
	{
		const char *const maximumNumberOfIterationsErrorMessage = "maximumNumberOfIterations needs to be positive. (FlowResistance)";
		throw(maximumNumberOfIterationsErrorMessage);
	}

	if( this->maximumFroudeNumber < 0.5 )
	{
				const char *const maximumFroudeNumberErrorMessage = "The maximum Froude number cannot be smaller than 0.5. (FlowResistance)";
				throw(maximumFroudeNumberErrorMessage);
	}
}

FlowResistance::FlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const std::set<int>& everTreatedCellIDs):
	startingValueForIteration(startingValueForIteration),
	accuracyForTerminatingIteration(accuracyForTerminatingIteration),
	maximumNumberOfIterations(maximumNumberOfIterations),
	useApproximationsForHydraulicRadius(useApproximationsForHydraulicRadius),
	maximumFroudeNumber(maximumFroudeNumber),
	minimumHydraulicSlope(minimumHydraulicSlope),
	typeOfNumericRootFinder(typeOfNumericRootFinder),
	everTreatedCellIDs(everTreatedCellIDs)
{
	this->flowResistanceSolver = this->flowResistanceSolverBuilder.numericRootFinderBuilder(typeOfNumericRootFinder);
	this->maximumFroudeSolver = this->maximumFroudeSolverBuilder.numericRootFinderBuilder(typeOfNumericRootFinder);
	this->minimumHydraulicSlopeSolver = this->minimumHydraulicSlopeSolverBuilder.numericRootFinderBuilder(typeOfNumericRootFinder);
	if(this->startingValueForIteration <= 0.0)
	{
		const char *const startingValueForIterationErrorMessage = "startingValueForIteration needs to be positive. (FlowResistance)";
		throw(startingValueForIterationErrorMessage);
	}

	if(this->accuracyForTerminatingIteration <= 0.0)
	{
		const char *const accuracyForTerminatingIterationErrorMessage = "accuracyForTerminatingIteration needs to be positive. (FlowResistance)";
		throw(accuracyForTerminatingIterationErrorMessage);
	}

	if(this->maximumNumberOfIterations <= 0)
	{
		const char *const maximumNumberOfIterationsErrorMessage = "maximumNumberOfIterations needs to be positive. (FlowResistance)";
		throw(maximumNumberOfIterationsErrorMessage);
	}

	if( this->maximumFroudeNumber < 0.5 )
	{
				const char *const maximumFroudeNumberErrorMessage = "The maximum Froude number cannot be smaller than 0.5. (FlowResistance)";
				throw(maximumFroudeNumberErrorMessage);
	}
}

FlowResistance::~FlowResistance()
{
	delete flowResistanceSolver;
	delete maximumFroudeSolver;
	delete minimumHydraulicSlopeSolver;
}

std::pair<double,double> FlowResistance::calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(double discharge, const RiverReachProperties& riverReachProperties) const
{
	std::pair<double,double> result;
	double outputFlowDepth = this->getStartingFlowDepthForIteration(riverReachProperties);
	FlowResistance_BoundaryConditionsForEquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero boundaryConditions (discharge,riverReachProperties,this);
	std::binder2nd< FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero > unaryFunctionObject (FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero(),boundaryConditions);
	std::pair<double,double> brackets = BasicCalculations::findPositiveBracketsStartingFromExpectedValue< std::binder2nd< FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero > >(unaryFunctionObject,outputFlowDepth,false,maximumNumberOfIterations,true);
	outputFlowDepth = this->flowResistanceSolver->findRoot(unaryFunctionObject,brackets.first,brackets.second,accuracyForTerminatingIteration,maximumNumberOfIterations);
	double flowVelocity = discharge / riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(outputFlowDepth);
	return (std::pair<double,double>(outputFlowDepth,flowVelocity));
}

std::pair<double,double> FlowResistance::calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(double discharge, const RiverReachProperties& riverReachProperties) const
{
	double outputFlowDepth = 0.0;
	double flowVelocity = 0.0;
	std::pair<double,double> result (outputFlowDepth,flowVelocity);
	if ( discharge > 0.0 )
	{

		double gravityAcceleration = (riverReachProperties.getOverallParameters())->getGravityAcceleration();
		double minimumFlowDepthForFroudeCriterion = BasicCalculations::calculateMaximumFlowDepthForGivenFroudeAndDischarge(this->maximumFroudeNumber,discharge,gravityAcceleration,riverReachProperties.geometricalChannelBehaviour->alluviumChannel,this->startingValueForIteration,this->accuracyForTerminatingIteration,this->maximumNumberOfIterations,this->maximumFroudeSolver);

		if(riverReachProperties.regularRiverReachProperties.waterEnergyslope > 0.0)
			{ result = calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(discharge,riverReachProperties); }
		else
			{ outputFlowDepth = minimumFlowDepthForFroudeCriterion; flowVelocity = discharge / riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(outputFlowDepth); result = std::pair<double,double>(outputFlowDepth,flowVelocity); }

		result = this->getFlowDepthAndVelocityEnsuringMinimumHydraulicSlope(result,discharge,minimumFlowDepthForFroudeCriterion,gravityAcceleration,riverReachProperties);

	}
	return result;
}

std::pair<double,double> FlowResistance::calculateDischargeAndFlowVelocityUsingFlowDepthAsInputBasedOnDarcyWeisbachFrictionFactor(double flowDepth, const RiverReachProperties& riverReachProperties, double darcyWeisbachFrictionFactor) const
{
	double hydraulicRadius = this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties);
	double flowVelocity = sqrt( (8.0 * (riverReachProperties.getOverallParameters())->getGravityAcceleration() * hydraulicRadius * riverReachProperties.regularRiverReachProperties.waterEnergyslope / darcyWeisbachFrictionFactor) );
	double discharge = flowVelocity * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
	std::pair<double,double> result (discharge,flowVelocity);
	return result;
}

std::pair<double,double> FlowResistance::calculateDischargeAndFlowDepthUsingFlowVelocityAsInputBasedOnDarcyWeisbachFrictionFactor(double flowVelocity, const RiverReachProperties& riverReachProperties, double darcyWeisbachFrictionFactor) const
{
	double hydraulicRadius = 0.125 * flowVelocity * flowVelocity * darcyWeisbachFrictionFactor / ( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * riverReachProperties.regularRiverReachProperties.waterEnergyslope );
	double flowDepth = this->convertHydraulicRadiusIntoMaximumFlowDepth(hydraulicRadius,riverReachProperties);
	double discharge = flowVelocity * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
	std::pair<double,double> result (discharge,flowDepth);
	return result;
}

std::pair<double,double> FlowResistance::calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	return calculateDischargeAndFlowVelocityUsingFlowDepthAsInputBasedOnDarcyWeisbachFrictionFactor(flowDepth,riverReachProperties, (returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(flowDepth,riverReachProperties)) );
}

std::pair<double,double> FlowResistance::calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	return calculateDischargeAndFlowDepthUsingFlowVelocityAsInputBasedOnDarcyWeisbachFrictionFactor(flowVelocity, riverReachProperties, (returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(flowVelocity,riverReachProperties)) );
}

double FlowResistance::returnlocalFrictionSlopeNotUsingLocalFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	double flowVelocity = riverReachProperties.regularRiverReachProperties.discharge / riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
	return ( returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(flowDepth,riverReachProperties) * flowVelocity * flowVelocity / ( 8.0 * riverReachProperties.getOverallParameters()->getGravityAcceleration() * this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties) ) );
}

double FlowResistance::returnlocalFrictionSlopeNotUsingLocalFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	double flowDepth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertCrossSectionalAreaIntoMaximumFlowDepth( (riverReachProperties.regularRiverReachProperties.discharge / flowVelocity) );
	return ( returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(flowVelocity,riverReachProperties) * flowVelocity * flowVelocity / ( 8.0 * riverReachProperties.getOverallParameters()->getGravityAcceleration() * this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties) ) );
}


double FlowResistance::returnlocalFrictionSlope(const RiverReachProperties& riverReachProperties) const
{
	return ( returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth,riverReachProperties) * riverReachProperties.regularRiverReachProperties.flowVelocity * riverReachProperties.regularRiverReachProperties.flowVelocity / ( 8.0 * riverReachProperties.getOverallParameters()->getGravityAcceleration() * this->convertMaximumFlowDepthIntoHydraulicRadius(riverReachProperties.regularRiverReachProperties.maximumWaterdepth,riverReachProperties) ) );
}


}
