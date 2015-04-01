/*
 * ScourChainProperties.h
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

#ifndef SCOURCHAINPROPERTIES_H_
#define SCOURCHAINPROPERTIES_H_

#include <utility>
#include <vector>

#include "AdditionalRiverReachPropertyType.h"
#include "CombinerVariables.h"
#include "RegularRiverReachProperties.h"

namespace SedFlow {

class ScourChainProperties: public AdditionalRiverReachPropertyType {

protected:
	double insertionTime;
	double insertionElevation;
	double currentChainElevation;

public:
	ScourChainProperties();
	virtual ~ScourChainProperties();

	ConstructionVariables createConstructionVariables()const;

	std::pair< CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod , std::vector<double> > typeSpecificGetter() const;
	void typeSpecificAction(RegularRiverReachProperties& regularRiverReachProperties, GeometricalChannelBehaviour* geometricalChannelBehaviour);
};

}

#endif /* SCOURCHAINPROPERTIES_H_ */
