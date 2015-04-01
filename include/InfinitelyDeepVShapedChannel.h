/*
 * InfinitelyDeepVShapedChannel.h
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

#ifndef INFINITELYDEEPVSHAPEDCHANNEL_H_
#define INFINITELYDEEPVSHAPEDCHANNEL_H_

#include "ChannelGeometry.h"

namespace SedFlow {

class InfinitelyDeepVShapedChannel: public ChannelGeometry {
private:
	double bankSlope;

public:
	InfinitelyDeepVShapedChannel(double bankSlope);
	virtual ~InfinitelyDeepVShapedChannel();

	ChannelGeometry* createChannelGeometryPointerCopy() const; //This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	PowerLawRelation maximumFlowDepthAsPowerLawFunctionOfCrossSectionalArea() const;
	PowerLawRelation crossSectionalAreaAsPowerLawFunctionOfMaximumFlowDepth() const;
	PowerLawRelation meanFlowDepthAsPowerLawFunctionOfMaximumFlowDepth() const;
	PowerLawRelation channelWidthAsPowerLawFunctionOfLevel() const;
	PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea() const;
	PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth() const;

	double convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(double crossSectionalArea, double startingLevel) const; //The result is flow depth ABOVE the starting level. Thus it can be smaller than starting level.
	double convertMaximumFlowDepthIntoCrossSectionalArea(double flowDepth) const;
	double convertMaximumFlowDepthIntoMeanFlowDepth(double flowDepth) const;
	double convertMaximumFlowDepthIntoHydraulicRadius(double flowDepth) const;
	double convertHydraulicRadiusIntoMaximumFlowDepth(double hydraulicRadius) const;
	double getChannelWidthAtCertainLevel(double level) const;
	double convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithoutFloor(double crossSectionalArea, double startingLevel) const;
	double convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithPlanarFloor(double crossSectionalArea, double startingLevel) const;
	double getMeanFlowDepthForActiveWidth(double maximumFlowDepth, double activeWidth) const;
	double getHydraulicRadiusForActiveWidth(double maximumFlowDepth, double activeWidth) const;
	double convertActiveWidthIntoActivePerimeter(double activeWidth) const;

	std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > getInternalParameters() const;
	void setInternalParameters( std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > newParameters );

};

}

#endif /* INFINITELYDEEPVSHAPEDCHANNEL_H_ */
