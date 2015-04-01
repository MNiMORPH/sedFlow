/*
 * InfinitelyDeepVShapedChannel.cpp
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

#include "InfinitelyDeepVShapedChannel.h"

#include <math.h>

namespace SedFlow {

InfinitelyDeepVShapedChannel::InfinitelyDeepVShapedChannel(double bankSlope):
	bankSlope(bankSlope)
{	this->typeOfChannelGeometry = CombinerVariables::InfinitelyDeepVShapedChannel;
	this->channelWidthIncreasesMonotonouslyWithWaterdepthOrCrossSectionalArea = true;
}

ChannelGeometry* InfinitelyDeepVShapedChannel::createChannelGeometryPointerCopy() const
{
	ChannelGeometry* result = new InfinitelyDeepVShapedChannel(this->bankSlope);
	return result;
}


InfinitelyDeepVShapedChannel::~InfinitelyDeepVShapedChannel(){}

ConstructionVariables InfinitelyDeepVShapedChannel::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChannelGeometry;
	result.realisationType = CombinerVariables::typeOfChannelGeometryToString( CombinerVariables::InfinitelyDeepVShapedChannel );
	std::vector<double> doubleVector;
	doubleVector.push_back(bankSlope);
	result.labelledDoubles["bankSlope"] = doubleVector;
	return result;
}

PowerLawRelation InfinitelyDeepVShapedChannel::maximumFlowDepthAsPowerLawFunctionOfCrossSectionalArea() const
{
	return PowerLawRelation(0.0,sqrt(bankSlope),0.5);
}

PowerLawRelation InfinitelyDeepVShapedChannel::crossSectionalAreaAsPowerLawFunctionOfMaximumFlowDepth() const
{
	return PowerLawRelation(0.0,(1.0 / bankSlope),2.0);
}

PowerLawRelation InfinitelyDeepVShapedChannel::meanFlowDepthAsPowerLawFunctionOfMaximumFlowDepth() const
{
	return PowerLawRelation(0.0,0.5,1.0);
}

PowerLawRelation InfinitelyDeepVShapedChannel::channelWidthAsPowerLawFunctionOfLevel() const
{
	return PowerLawRelation(0.0,(2.0 / bankSlope),1.0);
}

PowerLawRelation InfinitelyDeepVShapedChannel::hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea() const
{
	return PowerLawRelation(0.0,sqrt( (bankSlope / (1.0 + (bankSlope * bankSlope))) ),0.5);
}

PowerLawRelation InfinitelyDeepVShapedChannel::hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth() const
{
	return PowerLawRelation(0.0,( 1.0 / sqrt( (1.0 + (bankSlope * bankSlope)) ) ),1.0);
}

double InfinitelyDeepVShapedChannel::convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(double crossSectionalArea, double startingLevel) const
{
	return ( sqrt( ( bankSlope * (crossSectionalArea + (startingLevel * startingLevel / bankSlope)) ) ) );
}

double InfinitelyDeepVShapedChannel::convertMaximumFlowDepthIntoCrossSectionalArea(double flowDepth) const
{
	return ( flowDepth * flowDepth / bankSlope );
}

double InfinitelyDeepVShapedChannel::convertMaximumFlowDepthIntoMeanFlowDepth(double flowDepth) const
{
	return ( 0.5 * flowDepth );
}

double InfinitelyDeepVShapedChannel::convertMaximumFlowDepthIntoHydraulicRadius(double flowDepth) const
{
	return ( flowDepth / sqrt( (1.0 + (bankSlope * bankSlope)) ) );
}

double InfinitelyDeepVShapedChannel::convertHydraulicRadiusIntoMaximumFlowDepth(double hydraulicRadius) const
{
	return ( hydraulicRadius * sqrt( (1.0 + (bankSlope * bankSlope)) ) );
}

double InfinitelyDeepVShapedChannel::getChannelWidthAtCertainLevel(double level) const
{
	return ( 2 * level / bankSlope );
}

double InfinitelyDeepVShapedChannel::convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithoutFloor(double crossSectionalArea, double startingLevel) const
{
	double flowDepth = convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(crossSectionalArea, startingLevel);
	return ( flowDepth * sqrt( 1.0 + pow((1.0/bankSlope),2.0) ) );
}

double InfinitelyDeepVShapedChannel::convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithPlanarFloor(double crossSectionalArea, double startingLevel) const
{
	return ( convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithoutFloor(crossSectionalArea, startingLevel) + getChannelWidthAtCertainLevel(startingLevel) );
}

double InfinitelyDeepVShapedChannel::getMeanFlowDepthForActiveWidth(double maximumFlowDepth, double activeWidth) const
{
	return ( ( (2*maximumFlowDepth) - (0.5 * activeWidth * bankSlope) ) * 0.5 );
}

double InfinitelyDeepVShapedChannel::getHydraulicRadiusForActiveWidth(double maximumFlowDepth, double activeWidth) const
{
	return ( ( (maximumFlowDepth*activeWidth) - (0.25*bankSlope*activeWidth*activeWidth) ) / convertActiveWidthIntoActivePerimeter(activeWidth) );
}

double InfinitelyDeepVShapedChannel::convertActiveWidthIntoActivePerimeter(double activeWidth) const
{
	return ( activeWidth * sqrt( (1 + bankSlope * bankSlope ) ) );
}

std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > InfinitelyDeepVShapedChannel::getInternalParameters() const
{
	std::vector<double> doubleVector (1,bankSlope);
	std::vector< std::vector<double> > resultVector (1,doubleVector);
	return std::pair<CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > >(this->typeOfChannelGeometry, resultVector);
}


void InfinitelyDeepVShapedChannel::setInternalParameters( std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > newParameters )
{
	if (newParameters.first == this->typeOfChannelGeometry)
		{ bankSlope = ((newParameters.second).at(0)).at(0); }
	else
	{
		const char *const errorMessage = "Action on not matching type of channel geometry.";
		throw(errorMessage);
	}
}

}
