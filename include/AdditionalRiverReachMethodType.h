/*
 * AdditionalRiverReachMethodType.h
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

#ifndef ADDITIONALRIVERREACHMETHODTYPE_H_
#define ADDITIONALRIVERREACHMETHODTYPE_H_

#include "RegularRiverReachProperties.h"
#include "GeometricalChannelBehaviour.h"
#include "RiverReachProperties.h"
#include "RegularRiverReachMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class AdditionalRiverReachMethodType {
protected:
	AdditionalRiverReachPropertyType* correspondingAdditionalRiverReachProperty;
	CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalRiverPropertyAndMethod;

public:
	AdditionalRiverReachMethodType();
	AdditionalRiverReachMethodType(AdditionalRiverReachPropertyType* correspondingAdditionalRiverReachProperty);
	virtual ~AdditionalRiverReachMethodType(){}

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual AdditionalRiverReachMethodType* createAdditionalRiverReachMethodTypeCopy() const = 0; //This method HAS TO BE implemented.

	virtual void updateAdditionalRiverReachProperty (const RegularRiverReachProperties& regularRiverReachProperties, const GeometricalChannelBehaviour* geometricalChannelBehaviour, const RegularRiverReachMethods& regularRiverReachMethods) = 0;
	virtual void typeSpecificAction (RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods) = 0;

	inline CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod getTypeOfAdditionalRiverReachPropertyAndMethod() const { return typeOfAdditionalRiverPropertyAndMethod; }

};

}

#endif /* ADDITIONALRIVERREACHMETHODTYPE_H_ */
