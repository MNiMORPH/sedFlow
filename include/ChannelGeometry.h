/*
 * ChannelGeometry.h
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

#ifndef CHANNELGEOMETRY_H_
#define CHANNELGEOMETRY_H_

#include <utility>
#include <vector>

#include "RegularRiverReachProperties.h"
#include "CombinerVariables.h"
#include "ConstructionVariables.h"
#include "PowerLawRelation.h"

namespace SedFlow {

class ChannelGeometry {
protected:
	CombinerVariables::TypesOfChannelGeometry typeOfChannelGeometry;
	bool channelWidthIncreasesMonotonouslyWithWaterdepthOrCrossSectionalArea;
	// !! Each implementation MUST fulfill the pre-requisite that Waterdepth as well as WettedPerimeter increase monotonously with CrossSectionalArea. Anything else would not be a realistic channel. !!

public:
	ChannelGeometry(){}
	virtual ~ChannelGeometry();

	virtual ChannelGeometry* createChannelGeometryPointerCopy() const = 0; //This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual PowerLawRelation maximumFlowDepthAsPowerLawFunctionOfCrossSectionalArea() const = 0;
	virtual PowerLawRelation crossSectionalAreaAsPowerLawFunctionOfMaximumFlowDepth() const = 0;
	virtual PowerLawRelation meanFlowDepthAsPowerLawFunctionOfMaximumFlowDepth() const = 0;
	virtual PowerLawRelation channelWidthAsPowerLawFunctionOfLevel() const;
	virtual PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea() const = 0;
	virtual PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfCrossSectionalAreaUsingApproximations() const {return hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea();};
	virtual PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth() const = 0;
	virtual PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepthUsingApproximations() const {return hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth();};

	virtual double convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(double crossSectionalArea, double startingLevel) const = 0; //The result is flow depth ABOVE the starting level. Thus it can be smaller than starting level.
	virtual double convertMaximumFlowDepthIntoCrossSectionalArea(double flowDepth) const = 0;
	virtual double convertMaximumFlowDepthIntoMeanFlowDepth(double flowDepth) const;
	virtual double convertMaximumFlowDepthIntoHydraulicRadius(double flowDepth) const;
	virtual double convertMaximumFlowDepthIntoHydraulicRadiusUsingApproximations(double flowDepth) const {return convertMaximumFlowDepthIntoHydraulicRadius(flowDepth);};
	virtual double convertHydraulicRadiusIntoMaximumFlowDepth(double hydraulicRadius) const = 0;
	virtual double convertHydraulicRadiusIntoMaximumFlowDepthUsingApproximations(double hydraulicRadius) const {return convertHydraulicRadiusIntoMaximumFlowDepth(hydraulicRadius);};
	virtual double getChannelWidthAtCertainLevel(double level) const = 0;
	virtual double convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithoutFloor(double crossSectionalArea, double startingLevel) const = 0;
	virtual double convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevelWithPlanarFloor(double crossSectionalArea, double startingLevel) const = 0;
	virtual double getMeanFlowDepthForActiveWidth(double maximumFlowDepth, double activeWidth) const = 0;
	virtual double getHydraulicRadiusForActiveWidth(double maximumFlowDepth, double activeWidth) const = 0;
	virtual double getHydraulicRadiusForActiveWidthUsingApproximations(double maximumFlowDepth, double activeWidth) const {return getHydraulicRadiusForActiveWidth(maximumFlowDepth,activeWidth);};
	virtual double convertActiveWidthIntoActivePerimeter(double activeWidth) const = 0;

	virtual std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > getInternalParameters() const = 0;
	virtual void setInternalParameters( std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > newParameters ) = 0;

	double convertCrossSectionalAreaIntoMaximumFlowDepth(double crossSectionalArea) const; //i.e.{ return convertCrossSectionalAreaIntoMaximumFlowDepthStartingAtCertainLevel(crossSectionalArea, 0.0); }
	double convertCrossSectionalAreaIntoWettedPerimeter(double crossSectionalArea) const; //i.e. { return convertCrossSectionalAreaIntoWettedPerimeterStartingAtCertainLevel(crossSectionalArea, 0.0); }

	inline CombinerVariables::TypesOfChannelGeometry getTypeOfChannelGeometry() const { return typeOfChannelGeometry; }
	inline std::string getTypeOfChannelGeometryAsString() const { return CombinerVariables::typeOfChannelGeometryToString(typeOfChannelGeometry); }

	inline bool doesChannelWidthIncreaseMonotonouslyWithWaterdepthOrCrossSectionalArea() const { return channelWidthIncreasesMonotonouslyWithWaterdepthOrCrossSectionalArea; }
};

}

#endif /* CHANNELGEOMETRY_H_ */
