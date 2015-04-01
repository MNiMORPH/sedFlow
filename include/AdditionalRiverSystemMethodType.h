/*
 * AdditionalRiverSystemMethodType.h
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

#ifndef ADDITIONALRIVERSYSTEMMETHODTYPE_H_
#define ADDITIONALRIVERSYSTEMMETHODTYPE_H_

#include "AdditionalRiverSystemPropertyType.h"
#include "RiverSystemProperties.h"
#include "RegularRiverSystemMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class AdditionalRiverSystemMethodType {
protected:
	AdditionalRiverSystemPropertyType& correspondingAdditionalRiverSystemPropertyType;

public:
	AdditionalRiverSystemMethodType(AdditionalRiverSystemPropertyType& correspondingAdditionalRiverSystemPropertyType);
	virtual ~AdditionalRiverSystemMethodType();

	virtual AdditionalRiverSystemMethodType* createAdditionalRiverSystemMethodTypePointerCopy() const = 0; //This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual void updateAdditionalRiverSystemProperties(const RegularRiverSystemProperties& regularRiverSystemProperties, const OverallParameters& overallParameters, const RegularRiverSystemMethods& regularRiverSystemMethods) = 0;
	virtual void performAdditionalRiverSystemActions(RiverSystemProperties& riverSystemProperties, const RegularRiverSystemMethods& regularRiverSystemMethods) = 0;

	inline CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod getTypeOfAdditionalRiverSystemPropertyAndMethod() const { return correspondingAdditionalRiverSystemPropertyType.getTypeOfAdditionalRiverSystemPropertyAndMethod(); }
};

}

#endif /* ADDITIONALRIVERSYSTEMMETHODTYPE_H_ */
