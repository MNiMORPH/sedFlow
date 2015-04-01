/*
 * ChangeRateModifiersType.h
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

#ifndef CHANGERATEMODIFIERSTYPE_H_
#define CHANGERATEMODIFIERSTYPE_H_

#include "RegularRiverReachProperties.h"
#include "RegularRiverSystemProperties.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class ChangeRateModifiersType {
protected:
	CombinerVariables::TypesOfChangeRateModifiers typeOfChangeRateModifiers;
	CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod;

public:
	ChangeRateModifiersType(CombinerVariables::TypesOfChangeRateModifiers typeOfChangeRateModifiers, CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod);
	virtual ~ChangeRateModifiersType(){}

	virtual ChangeRateModifiersType* createChangeRateModifiersTypePointerCopy() const = 0;//This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual void inputModification(RiverReachProperties& riverReachProperties) = 0;
	virtual void modificationBeforeUpdates(RiverReachProperties& riverReachProperties) = 0;
	virtual bool furtherModificationIterationNecessary(const RiverReachProperties& riverReachProperties) const = 0;
	virtual void finalModification(RiverReachProperties& riverReachProperties) = 0;

	inline std::string getTypeOfChangeRateModifiersAsString() const { return CombinerVariables::typeOfChangeRateModifiersToString(this->typeOfChangeRateModifiers); }
	inline CombinerVariables::TypesOfChangeRateModifiers getTypeOfChangeRateModifiers() const { return this->typeOfChangeRateModifiers; }

	inline std::string getCorrespondingGeneralFlowMethodAsString() const { return CombinerVariables::typeOfGeneralFlowMethodsToString(this->correspondingGeneralFlowMethod); }
	inline CombinerVariables::TypesOfGeneralFlowMethods getCorrespondingGeneralFlowMethod() const { return correspondingGeneralFlowMethod; }
};

}

#endif /* CHANGERATEMODIFIERSTYPE_H_ */
