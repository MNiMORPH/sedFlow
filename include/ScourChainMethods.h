/*
 * ScourChainMethods.h
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

#ifndef SCOURCHAINMETHODS_H_
#define SCOURCHAINMETHODS_H_

#include "AdditionalRiverReachMethodType.h"

namespace SedFlow {

class ScourChainMethods: public AdditionalRiverReachMethodType {
//TO DO Implement this class
public:
	ScourChainMethods();
	virtual ~ScourChainMethods(){}

	ConstructionVariables createConstructionVariables()const;

	void update(const RegularRiverReachProperties& regularRiverReachProperties, const GeometricalChannelBehaviour* geometricalChannelBehaviour);
	void typeSpecificAction(RegularRiverReachProperties& regularRiverReachProperties, GeometricalChannelBehaviour* geometricalChannelBehaviour);

};

}

#endif /* SCOURCHAINMETHODS_H_ */
