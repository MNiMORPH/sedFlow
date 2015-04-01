/*
 * ChangeRateModifiers.h
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

#ifndef CHANGERATEMODIFIERS_H_
#define CHANGERATEMODIFIERS_H_

#include <vector>

#include "ChangeRateModifiersForSingleFlowMethod.h"
#include "CombinerVariables.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class ChangeRateModifiers {

private:
	std::vector<ChangeRateModifiersForSingleFlowMethod> constitutingChangeRateModifiersForSingleFlowMethods;
	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> treatedTypesOfGeneralFlowMethods;

	static std::vector<CombinerVariables::TypesOfGeneralFlowMethods> forConstructorsCheckAndCreateTreatedTypesOfGeneralFlowMethodsFromSingleChangeRateModifiersForSingleFlowMethod (const std::vector<ChangeRateModifiersForSingleFlowMethod>& singleChangeRateModifiersForSingleFlowMethod);

public:
	ChangeRateModifiers(){}
	ChangeRateModifiers(const std::vector<ChangeRateModifiersForSingleFlowMethod>& singleChangeRateModifiersForSingleFlowMethod);
	ChangeRateModifiers(const ChangeRateModifiers& toCopy);
	virtual ~ChangeRateModifiers(){}

	ConstructionVariables createConstructionVariables()const;

	bool checkForGeneralFlowMethodTreatment(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)const;

	ChangeRateModifiersForSingleFlowMethod& getChangeRateModifiersCorrespondingToGeneralFlowMethod(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods);
	ChangeRateModifiersForSingleFlowMethod* getChangeRateModifiersCorrespondingToGeneralFlowMethodPointer(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods);
	std::vector<ChangeRateModifiersForSingleFlowMethod>::iterator getChangeRateModifiersCorrespondingToGeneralFlowMethodIterator(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods);

};

}

#endif /* CHANGERATEMODIFIERS_H_ */
