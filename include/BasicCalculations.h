/*
 * BasicCalculations.h
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

#ifndef BASICCALCULATIONS_H_
#define BASICCALCULATIONS_H_

#include <vector>
#include <limits>
#include <math.h>

#include "RiverReachProperties.h"
#include "OverallParameters.h"
#include "ConstructionVariables.h"
#include "PowerLawRelation.h"
#include "NumericRootFinder.h"

namespace SedFlow {

class BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero;

class BasicCalculations {

public:

	static int signum (int value) { return (0 < value) - (value < 0 ); }
	static double signum (double value) { return static_cast<double>((0.0 < value) - (value < 0.0)); }

	static double calculateFroude(double meanFlowDepth, double flowVelocity, double gravityAcceleration);
	static double calculateFroude(const RiverReachProperties& riverReachProperties);
	static double calculateMaximumFlowDepthForGivenFroudeAndDischarge(double froudeNumber, double discharge, double gravityAcceleration, const ChannelGeometry* channelGeometry, double expectedFlowDepth, double accuracyForTerminatingIteration, int maximumNumberOfIterations, const NumericRootFinder< std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero > >* numericRootFinder);
	static double calculateFlowVelocityForGivenFroudeAndMeanFlowDepth(double froudeNumber, double meanFlowDepth, double gravityAcceleration);
	static std::vector<double> calculateDimensionlessShearStress(const RegularRiverReachProperties& regularRiverReachProperties);
	static double calculateDimensionlessShearStress(double diameter, const RegularRiverReachProperties& regularRiverReachProperties);
	static double calculateHydraulicRadiusFromDimensionlessShearStress(double dimensionlessShearStress, double diameter, double slope, const RiverReachProperties& riverReachProperties);
	static double calculateDimensionlessDischargePerUnitFlowWidth (const RiverReachProperties& riverReachProperties, double waterEnergyslope, double flowWidth, double grainDiameterPercentileRank);
	static double convertDarcyWeisbachFrictionFactorFIntoManningsN (double darcyWeisbachFrictionFactorF, const RiverReachProperties& riverReachProperties);
	static double hydraulicHeadBernoulliEquation (double elevation, double maximumWaterdepth, double flowVelocity, double gravityAcceleration);
	static double hydraulicHeadBernoulliEquation (const RiverReachProperties& riverReachProperties);
	static double hydraulicHeadBernoulliEquationKineticComponent (double flowVelocity, double gravityAcceleration);
	static double hydraulicHeadBernoulliEquationKineticComponent (const RiverReachProperties& riverReachProperties);
	static double calculateFlowVelocityForGivenHydraulicHeadAndWaterLevel(double hydraulicHead, double maximumWaterdepth, double elevation, double gravityAcceleration);

	template <typename UnaryFunction> static std::pair<double,double> findPositiveBracketsStartingFromExpectedValue(const UnaryFunction& function, double expectedValue, bool abortAtAmbiguity, int maximumNumberOfIterations, bool useLowerBracketsInCaseOfAmbiguity)
	{
		double  lowerBracket, upperBracket, previousLowerBracketValue, previousUpperBracketValue, lowerBracketValue, upperBracketValue;
		double previousLowerBracket (expectedValue), previousUpperBracket (expectedValue);
		previousLowerBracketValue = previousUpperBracketValue = signum(function(expectedValue));
		for(int i = 1; i <= maximumNumberOfIterations; ++i)
		{
			lowerBracket = previousLowerBracket * 0.5;
			upperBracket = previousUpperBracket * 2.0;
			lowerBracketValue = signum(function(lowerBracket));
			upperBracketValue = signum(function(upperBracket));
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

	template <typename UnaryFunction> static std::pair<double,double> findPositiveBracketsStartingFromBoundary(const UnaryFunction& function, double boundary, bool isLowerBoundary, int maximumNumberOfIterations)
	{
		double factor,currentBracket,currentBracketValue;
		if(isLowerBoundary) { factor = 2.0; } else { factor = 0.5; }
		double previousBracket (boundary);
		double previousBracketValue (signum(function(boundary)));
		for(int i = 1; i <= maximumNumberOfIterations; ++i)
		{
			currentBracket = previousBracket * factor;
			currentBracketValue = signum(function(currentBracket));
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


		const char *const maxIterationErrorMessage = "BasicCalculations::findPositiveBracketsStartingFromBoundary: Maximum number of iterations exceeded.";
		throw(maxIterationErrorMessage);
		return std::pair<double,double>(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN());
	}

};


class BasicCalculations_BoundaryConditionsForEquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero
{
public:
	double froudeNumber;
	double discharge;
	double gravityAcceleration;
	const ChannelGeometry* const channelGeometry;
	BasicCalculations_BoundaryConditionsForEquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero(double froudeNumber, double discharge, double gravityAcceleration, const ChannelGeometry* channelGeometry):
		froudeNumber(froudeNumber),
		discharge(discharge),
		gravityAcceleration(gravityAcceleration),
		channelGeometry(channelGeometry)
	{}
};

class BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero: public std::binary_function<double,BasicCalculations_BoundaryConditionsForEquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero,double>
{
public:
	double operator() (double flowDepth, const BasicCalculations_BoundaryConditionsForEquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero& boundaryConditions) const
	{
		double result;
		if(flowDepth > 0.0)
			{ result = boundaryConditions.froudeNumber - ( boundaryConditions.discharge / ( boundaryConditions.channelGeometry->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth) * sqrt( (boundaryConditions.gravityAcceleration * boundaryConditions.channelGeometry->convertMaximumFlowDepthIntoMeanFlowDepth(flowDepth)) ) ) ); }
		else
			{ result = std::numeric_limits<double>::signaling_NaN(); }
		return result;
	}
};


}

#endif /* BASICCALCULATIONS_H_ */
