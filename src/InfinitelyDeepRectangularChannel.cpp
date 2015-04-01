/*
 * InfinitelyDeepRectangularChannel.cpp
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

#include "InfinitelyDeepRectangularChannel.h"

namespace SedFlow {

InfinitelyDeepRectangularChannel::InfinitelyDeepRectangularChannel(double channelWidth):
	channelWidth(channelWidth)
{
	this->typeOfChannelGeometry = CombinerVariables::InfinitelyDeepRectangularChannel;
	this->channelWidthIncreasesMonotonouslyWithWaterdepthOrCrossSectionalArea = true;
}

InfinitelyDeepRectangularChannel::~InfinitelyDeepRectangularChannel(){}

ChannelGeometry* InfinitelyDeepRectangularChannel::createChannelGeometryPointerCopy() const
{
	ChannelGeometry* result = new InfinitelyDeepRectangularChannel(this->channelWidth);
	return result;
}

ConstructionVariables InfinitelyDeepRectangularChannel::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChannelGeometry;
	result.realisationType = CombinerVariables::typeOfChannelGeometryToString( CombinerVariables::InfinitelyDeepRectangularChannel );
	std::vector<double> doubleVector;
	doubleVector.push_back(channelWidth);
	result.labelledDoubles["channelWidth"] = doubleVector;
	return result;
}

PowerLawRelation InfinitelyDeepRectangularChannel::maximumFlowDepthAsPowerLawFunctionOfCrossSectionalArea() const
{
	return PowerLawRelation(0.0,(1.0/channelWidth),1.0);
}

PowerLawRelation InfinitelyDeepRectangularChannel::crossSectionalAreaAsPowerLawFunctionOfMaximumFlowDepth() const
{
	return PowerLawRelation(0.0,channelWidth,1.0);
}

PowerLawRelation InfinitelyDeepRectangularChannel::meanFlowDepthAsPowerLawFunctionOfMaximumFlowDepth() const
{
	return PowerLawRelation(0.0,1.0,1.0);
}

PowerLawRelation InfinitelyDeepRectangularChannel::channelWidthAsPowerLawFunctionOfLevel() const
{
	return PowerLawRelation(0.0,channelWidth,0.0);
}

PowerLawRelation InfinitelyDeepRectangularChannel::hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea() const
{
	PowerLawRelation result (false);
	return result;
}

PowerLawRelation InfinitelyDeepRectangularChannel::hydraulicRadiusAsPowerLawFunctionOfCrossSectionalAreaUsingApproximations() const
{
	PowerLawRelation result = this->maximumFlowDepthAsPowerLawFunctionOfCrossSectionalArea();
	return result;
}

PowerLawRelation InfinitelyDeepRectangularChannel::hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth() const
{
	PowerLawRelation result (false);
	return result;
}

PowerLawRelation InfinitelyDeepRectangularChannel::hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepthUsingApproximations() const
{
	PowerLawRelation result = PowerLawRelation(0.0,1.0,1.0);
	return result;
}

double  InfinitelyDeepRectangularChannel::convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(double crossSectionalArea, double startingLevel) const
{
	return (crossSectionalArea / channelWidth);
}


double  InfinitelyDeepRectangularChannel::convertMaximumFlowDepthIntoCrossSectionalArea(double flowDepth) const
{
	return (flowDepth * channelWidth);
}

double  InfinitelyDeepRectangularChannel::convertMaximumFlowDepthIntoMeanFlowDepth(double flowDepth) const
{
	return flowDepth;
}

double InfinitelyDeepRectangularChannel::convertMaximumFlowDepthIntoHydraulicRadius(double flowDepth) const
{
	return ((channelWidth * flowDepth) / (channelWidth + (2.0 * flowDepth)));
}

double InfinitelyDeepRectangularChannel::convertMaximumFlowDepthIntoHydraulicRadiusUsingApproximations(double flowDepth) const
{
	return flowDepth;
}

double InfinitelyDeepRectangularChannel::convertHydraulicRadiusIntoMaximumFlowDepth(double hydraulicRadius) const
{
	return ((channelWidth * hydraulicRadius) / (channelWidth - (2.0 * hydraulicRadius)));
}

double InfinitelyDeepRectangularChannel::convertHydraulicRadiusIntoMaximumFlowDepthUsingApproximations(double hydraulicRadius) const
{
	return hydraulicRadius;
}

double  InfinitelyDeepRectangularChannel::getChannelWidthAtCertainLevel(double level) const
{
	return channelWidth;
}

double InfinitelyDeepRectangularChannel::convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithoutFloor(double crossSectionalArea, double startingLevel) const
{
	double flowDepth = convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(crossSectionalArea, startingLevel);
	return (2 * flowDepth);
}

double  InfinitelyDeepRectangularChannel::convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithPlanarFloor(double crossSectionalArea, double startingLevel) const
{
	double flowDepth = convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(crossSectionalArea, startingLevel);
	return (channelWidth + (2 * flowDepth));
}

double InfinitelyDeepRectangularChannel::getMeanFlowDepthForActiveWidth(double maximumFlowDepth, double activeWidth) const
{
	return maximumFlowDepth;
}

double InfinitelyDeepRectangularChannel::getHydraulicRadiusForActiveWidth(double maximumFlowDepth, double activeWidth) const
{
	double result = 0.0;

	if( activeWidth > 0.0 )
	{
		if(activeWidth < 0.99 * channelWidth)
			{ result = maximumFlowDepth; }
		else
			{ result = (channelWidth * maximumFlowDepth) / ( channelWidth + 2 * maximumFlowDepth ); }
	}

	return result;
}

double InfinitelyDeepRectangularChannel::getHydraulicRadiusForActiveWidthUsingApproximations(double maximumFlowDepth, double activeWidth) const
{
	double result = 0.0;

	if( activeWidth > 0.0 )
	{
		result = maximumFlowDepth;
	}

	return result;
}

double InfinitelyDeepRectangularChannel::convertActiveWidthIntoActivePerimeter(double activeWidth) const
{
	return std::min(activeWidth, channelWidth);
}

std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > InfinitelyDeepRectangularChannel::getInternalParameters() const
{
	std::vector<double> doubleVector (1,channelWidth);
	std::vector< std::vector<double> > resultVector (1,doubleVector);
	return std::pair<CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > >(this->typeOfChannelGeometry, resultVector);
}


void InfinitelyDeepRectangularChannel::setInternalParameters( std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > newParameters )
{
	if (newParameters.first == this->typeOfChannelGeometry)
		{ channelWidth = ((newParameters.second).at(0)).at(0); }
	else
	{
		const char *const errorMessage = "Action on not matching type of channel geometry.";
		throw(errorMessage);
	}
}

}
