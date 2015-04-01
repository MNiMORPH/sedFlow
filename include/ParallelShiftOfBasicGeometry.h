/*
 * ParallelShiftOfBasicGeometry.h
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

#ifndef PARALLELSHIFTOFBASICGEOMETRY_H_
#define PARALLELSHIFTOFBASICGEOMETRY_H_

#include "GeometricalChannelBehaviour.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class ParallelShiftOfBasicGeometry: public GeometricalChannelBehaviour {
private:
	bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate;

public:
	ParallelShiftOfBasicGeometry(const ChannelGeometry* alluviumChannel, bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate);
	ParallelShiftOfBasicGeometry(const ChannelGeometry* alluviumChannel, bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate, std::vector<double> additionalParameters);
	virtual ~ParallelShiftOfBasicGeometry(){} //alluviumChannel is already deleted in ~GeometricalChannelBehaviour

	GeometricalChannelBehaviour* createGeometricalChannelBehaviourPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	double convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementIncludingAlluviumChannelUpdate (double activeWidth, double reachLength, double sedimentVolumeIncrement);
	double convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel (double activeWidth, double reachLength, double sedimentVolumeIncrement) const;
	std::pair<bool,double> getLinearConversionFactorFromVolumeToHeight(double activeWidth, double reachLength) const;

	virtual std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > getInternalParameters() const;
	virtual void setInternalParameters( std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > newParameters );

};

}

#endif /* PARALLELSHIFTOFBASICGEOMETRY_H_ */
