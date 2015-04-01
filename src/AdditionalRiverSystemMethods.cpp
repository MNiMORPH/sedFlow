/*
 * AdditionalRiverSystemMethods.cpp
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

#include "AdditionalRiverSystemMethods.h"

#include <algorithm>

namespace SedFlow {

std::vector<AdditionalRiverSystemMethodType*> AdditionalRiverSystemMethods::forConstructorsCheckAndCopySingleAdditionalRiverSystemMethodTypes (const std::vector<AdditionalRiverSystemMethodType*>& singleAdditionalRiverSystemMethodTypes)
{
	std::vector<CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod> typesOfAdditionalRiverSystemPropertyAndMethod;
	for(std::vector<AdditionalRiverSystemMethodType*>::const_iterator i = singleAdditionalRiverSystemMethodTypes.begin(); i < singleAdditionalRiverSystemMethodTypes.end(); ++i)
		{ typesOfAdditionalRiverSystemPropertyAndMethod.push_back((*(*i)).getTypeOfAdditionalRiverSystemPropertyAndMethod()); }
	std::vector<CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod> sortedTypesOfAdditionalRiverSystemPropertyAndMethod = std::vector<CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod>(typesOfAdditionalRiverSystemPropertyAndMethod);
	std::sort(sortedTypesOfAdditionalRiverSystemPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverSystemPropertyAndMethod.end());
	if (std::adjacent_find(sortedTypesOfAdditionalRiverSystemPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverSystemPropertyAndMethod.end()) != sortedTypesOfAdditionalRiverSystemPropertyAndMethod.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	std::vector<AdditionalRiverSystemMethodType*> result;

	for(std::vector<AdditionalRiverSystemMethodType*>::const_iterator i = singleAdditionalRiverSystemMethodTypes.begin(); i < singleAdditionalRiverSystemMethodTypes.end(); ++i)
	{
		result.push_back((*(*i)).createAdditionalRiverSystemMethodTypePointerCopy());
	}

	return result;
}

AdditionalRiverSystemMethods::AdditionalRiverSystemMethods(const std::vector<AdditionalRiverSystemMethodType*>& singleAdditionalRiverSystemMethodTypes):
	constitutingAdditionalRiverSystemMethodTypes( AdditionalRiverSystemMethods::forConstructorsCheckAndCopySingleAdditionalRiverSystemMethodTypes(singleAdditionalRiverSystemMethodTypes) )
{}

ConstructionVariables AdditionalRiverSystemMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverSystemMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingAdditionalRiverSystemMethodTypes.size() );
	for(std::vector<AdditionalRiverSystemMethodType*>::const_iterator iterator = constitutingAdditionalRiverSystemMethodTypes.begin(); iterator < constitutingAdditionalRiverSystemMethodTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingAdditionalRiverSystemMethodTypes"] = constructionVariablesVector;
	return result;
}

AdditionalRiverSystemMethods::AdditionalRiverSystemMethods(const AdditionalRiverSystemMethods& toCopy):
	constitutingAdditionalRiverSystemMethodTypes( AdditionalRiverSystemMethods::forConstructorsCheckAndCopySingleAdditionalRiverSystemMethodTypes(toCopy.constitutingAdditionalRiverSystemMethodTypes) )
{}

AdditionalRiverSystemMethods::~AdditionalRiverSystemMethods()
{
	while( ! (this->constitutingAdditionalRiverSystemMethodTypes.empty()) )
	{
		delete this->constitutingAdditionalRiverSystemMethodTypes.back();
		this->constitutingAdditionalRiverSystemMethodTypes.pop_back();
	}
}

void AdditionalRiverSystemMethods::updateAdditionalRiverSystemProperties(const RegularRiverSystemProperties& RegularRiverSystemProperties, const OverallParameters& overallParameters, const RegularRiverSystemMethods& regularRiverSystemMethods)
{
	for(std::vector<AdditionalRiverSystemMethodType*>::iterator currentAdditionalRiverSystemMethodTypePointer = constitutingAdditionalRiverSystemMethodTypes.begin(); currentAdditionalRiverSystemMethodTypePointer < constitutingAdditionalRiverSystemMethodTypes.end(); ++currentAdditionalRiverSystemMethodTypePointer)
			{ (*(*currentAdditionalRiverSystemMethodTypePointer)).updateAdditionalRiverSystemProperties(RegularRiverSystemProperties, overallParameters, regularRiverSystemMethods); }
}

void AdditionalRiverSystemMethods::performAdditionalRiverSystemActions(RiverSystemProperties& riverSystemProperties, const RegularRiverSystemMethods& regularRiverSystemMethods)
{
	for(std::vector<AdditionalRiverSystemMethodType*>::iterator currentAdditionalRiverSystemMethodTypePointer = constitutingAdditionalRiverSystemMethodTypes.begin(); currentAdditionalRiverSystemMethodTypePointer < constitutingAdditionalRiverSystemMethodTypes.end(); ++currentAdditionalRiverSystemMethodTypePointer)
			{ (*(*currentAdditionalRiverSystemMethodTypePointer)).performAdditionalRiverSystemActions(riverSystemProperties, regularRiverSystemMethods); }
}

}
