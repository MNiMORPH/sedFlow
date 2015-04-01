/*
 * GeometricalChannelBehaviour.h
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

#ifndef GEOMETRICALCHANNELBEHAVIOUR_H_
#define GEOMETRICALCHANNELBEHAVIOUR_H_

#include "CombinerVariables.h"
#include "ChannelGeometry.h"
#include "Grains.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class GeometricalChannelBehaviour {
protected:
	CombinerVariables::TypesOfGeometricalChannelBehaviour typeOfGeometricalChannelBehaviour;
	bool doesNOTchangeTypeOfChannelGeometry;

public:
	GeometricalChannelBehaviour();
	//Each realisation should implement the following constructor
	//GeometricalChannelBehaviourRealisation(const ChannelGeometry* alluviumChannel, std::vector< std::vector<double> > furtherParameters);
	GeometricalChannelBehaviour(const ChannelGeometry* alluviumChannel);
	virtual ~GeometricalChannelBehaviour();

	virtual GeometricalChannelBehaviour* createGeometricalChannelBehaviourPointerCopy() const = 0; //This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	ChannelGeometry* alluviumChannel;

	virtual double convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementIncludingAlluviumChannelUpdate (double activeWidth, double reachLength, double sedimentVolumeIncrement) = 0;
	virtual double convertActiveWidthAndOverallSedimentVolumeIncrementIntoElevationIncrementWithoutUpdatingAlluviumChannel (double activeWidth, double reachLength, double sedimentVolumeIncrement) const = 0;
	virtual std::pair<bool,double> getLinearConversionFactorFromVolumeToHeight(double activeWidth, double reachLength) const = 0;

	//The following two methods are pre-implemented
	virtual Grains convertActiveWidthAndOverallSedimentVolumeIncrementIntoSedimentVolumeIncrementPerUnitBedSurface (double activeWidth, Grains sedimentVolumeIncrement) const;
	virtual Grains convertActiveWidthAndSedimentVolumePerUnitBedSurfaceIntoOverallActiveSedimentVolume (double activeWidth, Grains sedimentVolumePerUnitBedSurface) const;

	inline bool keepsTYPEofChannelGeometryConstant() const { return doesNOTchangeTypeOfChannelGeometry; }

	virtual std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > getInternalParameters() const = 0;
	virtual void setInternalParameters( std::pair< CombinerVariables::TypesOfGeometricalChannelBehaviour, std::vector< std::vector<double> > > newParameters ) = 0;

	inline CombinerVariables::TypesOfGeometricalChannelBehaviour getTypeOfGeometricalChannelBehaviour() const { return typeOfGeometricalChannelBehaviour; }
	inline std::string getTypeOfGeometricalChannelBehaviourAsString() const { return CombinerVariables::typeOfGeometricalChannelBehaviourToString(typeOfGeometricalChannelBehaviour); }

};

}

#endif /* GEOMETRICALCHANNELBEHAVIOUR_H_ */
