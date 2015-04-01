/*
 * BasicCalculations.cpp
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

#include "BasicCalculations.h"

#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cstring>

namespace SedFlow {

//TODO Remove these debugging lines.
/////////////////////////////////////////////////////////////////////////////////////////
/*
std::pair<double,double> Debugging_BasicCalculations_findPositiveBracketsStartingFromExpectedValue(const std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero >& function, double expectedValue, bool abortAtAmbiguity, int maximumNumberOfIterations, bool useLowerBracketsInCaseOfAmbiguity)
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
/////////////////////////////////////////////////////////////////////////////////////////

double BasicCalculations::calculateFroude(double meanFlowDepth, double flowVelocity, double gravityAcceleration)
{
	return ( flowVelocity /  sqrt( (gravityAcceleration * meanFlowDepth) ) );
}

double BasicCalculations::calculateFroude(const RiverReachProperties& riverReachProperties)
{
	return BasicCalculations::calculateFroude(riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoMeanFlowDepth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth),riverReachProperties.regularRiverReachProperties.flowVelocity,(riverReachProperties.getOverallParameters())->getGravityAcceleration());
}

double BasicCalculations::calculateMaximumFlowDepthForGivenFroudeAndDischarge(double froudeNumber, double discharge, double gravityAcceleration, const ChannelGeometry* channelGeometry, double expectedFlowDepth, double accuracyForTerminatingIteration, int maximumNumberOfIterations, const NumericRootFinder< std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero > >* numericRootFinder)
{
	double result = 0.0;
	if(froudeNumber > 0.0)
	{
		PowerLawRelation crossSectionalAreaAsFunctionOfMaximumFlowDepth = channelGeometry->crossSectionalAreaAsPowerLawFunctionOfMaximumFlowDepth();
		PowerLawRelation meanFlowDepthAsFunctionOfMaximumFlowDepth = channelGeometry->meanFlowDepthAsPowerLawFunctionOfMaximumFlowDepth();

		if( crossSectionalAreaAsFunctionOfMaximumFlowDepth.powerLawCheck() && meanFlowDepthAsFunctionOfMaximumFlowDepth.powerLawCheck() && (crossSectionalAreaAsFunctionOfMaximumFlowDepth.addition() == 0.0) && (meanFlowDepthAsFunctionOfMaximumFlowDepth.addition() == 0.0) && (crossSectionalAreaAsFunctionOfMaximumFlowDepth.factor() != 0.0) && (meanFlowDepthAsFunctionOfMaximumFlowDepth.factor() > 0.0) && (crossSectionalAreaAsFunctionOfMaximumFlowDepth.exponent() != (-0.5 * meanFlowDepthAsFunctionOfMaximumFlowDepth.exponent())) )
		{
			result = pow( ( discharge / (froudeNumber * crossSectionalAreaAsFunctionOfMaximumFlowDepth.factor() * sqrt((gravityAcceleration * meanFlowDepthAsFunctionOfMaximumFlowDepth.factor())) ) ) , (1.0 / ((0.5 * meanFlowDepthAsFunctionOfMaximumFlowDepth.exponent()) + crossSectionalAreaAsFunctionOfMaximumFlowDepth.exponent())) );
		}
		else
		{
			BasicCalculations_BoundaryConditionsForEquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero boundaryConditions (froudeNumber, discharge, gravityAcceleration, channelGeometry);
			std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero > unaryFunctionObject (BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero(),boundaryConditions);

			std::pair<double,double> brackets = findPositiveBracketsStartingFromExpectedValue< std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero > >(unaryFunctionObject,expectedFlowDepth,true,maximumNumberOfIterations,true);
			result = numericRootFinder->findRoot(unaryFunctionObject,brackets.first,brackets.second,accuracyForTerminatingIteration,maximumNumberOfIterations);
		}
	}
	else
	{
		const char *const errorMessage = "BasicCalculations::calculateMaximumFlowDepthForGivenFroudeAndDischarge is not defined for non-positive Froude numbers.";
		throw(errorMessage);
	}
	return result;
}

double BasicCalculations::calculateFlowVelocityForGivenFroudeAndMeanFlowDepth(double froudeNumber, double meanFlowDepth, double gravityAcceleration)
{
	return ( froudeNumber * sqrt( (meanFlowDepth * gravityAcceleration) ) );
}

double BasicCalculations::calculateHydraulicRadiusFromDimensionlessShearStress(double dimensionlessShearStress, double diameter, double slope, const RiverReachProperties& riverReachProperties)
{
	return ( (dimensionlessShearStress * (((riverReachProperties.getOverallParameters())->getSedimentDensity() / (riverReachProperties.getOverallParameters())->getWaterDensity()) - 1.0) * diameter) / slope );
}

double BasicCalculations::calculateDimensionlessShearStress(double diameter, const RegularRiverReachProperties& regularRiverReachProperties)
{
	return ( regularRiverReachProperties.bedShearStress / (((regularRiverReachProperties.getOverallParameters())->getSedimentDensity() - (regularRiverReachProperties.getOverallParameters())->getWaterDensity()) *  (regularRiverReachProperties.getOverallParameters())->getGravityAcceleration() * diameter) );
}

std::vector<double> BasicCalculations::calculateDimensionlessShearStress(const RegularRiverReachProperties& regularRiverReachProperties)
{
	std::vector<double> grainDiameters = (regularRiverReachProperties.getOverallParameters())->getFractionalGrainDiameters();
	double constantFactors = ( regularRiverReachProperties.bedShearStress / (((regularRiverReachProperties.getOverallParameters())->getSedimentDensity() - (regularRiverReachProperties.getOverallParameters())->getWaterDensity()) *  (regularRiverReachProperties.getOverallParameters())->getGravityAcceleration()) );
	std::transform(grainDiameters.begin(),grainDiameters.end(),grainDiameters.begin(),std::bind1st(std::divides<double>(),constantFactors));
	//The previous transform uses grainDiameters as container for the results. Therefore it is returned
	return grainDiameters;
}

double BasicCalculations::calculateDimensionlessDischargePerUnitFlowWidth (const RiverReachProperties& riverReachProperties, double waterEnergyslope, double flowWidth, double grainDiameterPercentileRank)
{
	if(waterEnergyslope < 0.0)
	{
		std::ostringstream oStringStream;
		oStringStream << "At ";
		if(riverReachProperties.isMargin())
		{
			if(riverReachProperties.isDownstreamMargin())
			{
				oStringStream << "downstream margin";
			}
			else
			{
				oStringStream << "upstream margin at reach " << riverReachProperties.getDownstreamUserCellID();
			}
		}
		else
		{
			oStringStream << "reach " << riverReachProperties.getUserCellID();
		}
		oStringStream << " occurence of negative water energyslope. (BasicCalculations)" << std::flush;

		char* tmpChar = new char [(oStringStream.str()).size()+1];
		std::strcpy(tmpChar, (oStringStream.str()).c_str());
		const char *const negativeWaterEnergyslopeErrorMessage = tmpChar;
		throw(negativeWaterEnergyslopeErrorMessage);
	}

	return ( riverReachProperties.regularRiverReachProperties.discharge / ( flowWidth * sqrt( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * waterEnergyslope * pow( riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(), grainDiameterPercentileRank) , 3) ) ) );
}

double BasicCalculations::convertDarcyWeisbachFrictionFactorFIntoManningsN (double darcyWeisbachFrictionFactorF, const RiverReachProperties& riverReachProperties)
{
	return ( pow( riverReachProperties.regularRiverReachProperties.maximumWaterdepth , (1/6) ) * sqrt( ( darcyWeisbachFrictionFactorF / (8*(riverReachProperties.getOverallParameters())->getGravityAcceleration()) ) ) );
}

double BasicCalculations::hydraulicHeadBernoulliEquation (double elevation, double maximumWaterdepth, double flowVelocity, double gravityAcceleration)
{
	return ( elevation + maximumWaterdepth + hydraulicHeadBernoulliEquationKineticComponent(flowVelocity,gravityAcceleration));
}

double BasicCalculations::hydraulicHeadBernoulliEquation (const RiverReachProperties& riverReachProperties)
{
	return hydraulicHeadBernoulliEquation(riverReachProperties.regularRiverReachProperties.elevation,riverReachProperties.regularRiverReachProperties.maximumWaterdepth,riverReachProperties.regularRiverReachProperties.flowVelocity,((riverReachProperties.getOverallParameters())->getGravityAcceleration()));
}

double BasicCalculations::hydraulicHeadBernoulliEquationKineticComponent (double flowVelocity, double gravityAcceleration)
{
	return (0.5 * flowVelocity * flowVelocity / gravityAcceleration);
}

double BasicCalculations::hydraulicHeadBernoulliEquationKineticComponent (const RiverReachProperties& riverReachProperties)
{
	return hydraulicHeadBernoulliEquationKineticComponent(riverReachProperties.regularRiverReachProperties.flowVelocity,((riverReachProperties.getOverallParameters())->getGravityAcceleration()));
}

double BasicCalculations::calculateFlowVelocityForGivenHydraulicHeadAndWaterLevel(double hydraulicHead, double maximumWaterdepth, double elevation, double gravityAcceleration)
{
	double kineticHead = hydraulicHead - elevation - maximumWaterdepth;
	if(kineticHead > 0.0)
	{
		return sqrt( (2.0 * gravityAcceleration * kineticHead) );
	}
	else
	{
		const char *const errorMessage = "Occurence of negative kinetic hydraulic head. (BasicCalculations::calculateFlowVelocityForGivenHydraulicHeadAndWaterLevel)";
		throw(errorMessage);
	}

}

}
