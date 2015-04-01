/*
 * AdditionalRiverReachMethods.cpp
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

#include "AdditionalRiverReachMethods.h"

#include <algorithm>

namespace SedFlow {

std::vector<AdditionalRiverReachMethodType*> AdditionalRiverReachMethods::forConstructorsCheckAndCopySingleAdditionalRiverReachMethodTypes (const std::vector<AdditionalRiverReachMethodType*>& singleAdditionalRiverReachMethodTypes)
{
	std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod> typesOfAdditionalRiverReachPropertyAndMethod;
	for(std::vector<AdditionalRiverReachMethodType*>::const_iterator i = singleAdditionalRiverReachMethodTypes.begin(); i < singleAdditionalRiverReachMethodTypes.end(); ++i)
		{ typesOfAdditionalRiverReachPropertyAndMethod.push_back((*(*i)).getTypeOfAdditionalRiverReachPropertyAndMethod()); }
	std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod> sortedTypesOfAdditionalRiverReachPropertyAndMethod = std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod>(typesOfAdditionalRiverReachPropertyAndMethod);
	std::sort(sortedTypesOfAdditionalRiverReachPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverReachPropertyAndMethod.end());
	if (std::adjacent_find(sortedTypesOfAdditionalRiverReachPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverReachPropertyAndMethod.end()) != sortedTypesOfAdditionalRiverReachPropertyAndMethod.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	std::vector<AdditionalRiverReachMethodType*> result;

	for(std::vector<AdditionalRiverReachMethodType*>::const_iterator i = singleAdditionalRiverReachMethodTypes.begin(); i < singleAdditionalRiverReachMethodTypes.end(); ++i)
	{
		result.push_back((*(*i)).createAdditionalRiverReachMethodTypeCopy());
	}

	return result;
}

AdditionalRiverReachMethods::AdditionalRiverReachMethods(const std::vector<AdditionalRiverReachMethodType*>& singleAdditionalRiverReachMethodTypes):
	constitutingAdditionalRiverReachMethodTypes( AdditionalRiverReachMethods::forConstructorsCheckAndCopySingleAdditionalRiverReachMethodTypes(singleAdditionalRiverReachMethodTypes) )
{}

ConstructionVariables AdditionalRiverReachMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingAdditionalRiverReachMethodTypes.size() );
	for(std::vector<AdditionalRiverReachMethodType*>::const_iterator iterator = constitutingAdditionalRiverReachMethodTypes.begin(); iterator < constitutingAdditionalRiverReachMethodTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingAdditionalRiverReachMethodTypes"] = constructionVariablesVector;
	return result;
}

AdditionalRiverReachMethods::AdditionalRiverReachMethods(const AdditionalRiverReachMethods& toCopy):
		constitutingAdditionalRiverReachMethodTypes( AdditionalRiverReachMethods::forConstructorsCheckAndCopySingleAdditionalRiverReachMethodTypes(toCopy.constitutingAdditionalRiverReachMethodTypes) )
{}

AdditionalRiverReachMethods::~AdditionalRiverReachMethods()
{
	while( ! (this->constitutingAdditionalRiverReachMethodTypes.empty()) )
	{
		delete this->constitutingAdditionalRiverReachMethodTypes.back();
		this->constitutingAdditionalRiverReachMethodTypes.pop_back();
	}
}

void AdditionalRiverReachMethods::updateAdditionalRiverReachProperties(const RegularRiverReachProperties& regularRiverReachProperties, const GeometricalChannelBehaviour* geometricalChannelBehaviour, const RegularRiverReachMethods& regularRiverReachMethods)
{
	for(std::vector<AdditionalRiverReachMethodType*>::iterator currentAdditionalRiverReachMethodTypePointer = constitutingAdditionalRiverReachMethodTypes.begin(); currentAdditionalRiverReachMethodTypePointer < constitutingAdditionalRiverReachMethodTypes.end(); ++currentAdditionalRiverReachMethodTypePointer)
			{ (*(*currentAdditionalRiverReachMethodTypePointer)).updateAdditionalRiverReachProperty(regularRiverReachProperties, geometricalChannelBehaviour, regularRiverReachMethods); }
}

void AdditionalRiverReachMethods::performTypeSpecificActions(RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods)
{
	for(std::vector<AdditionalRiverReachMethodType*>::iterator currentAdditionalRiverReachMethodTypePointer = constitutingAdditionalRiverReachMethodTypes.begin(); currentAdditionalRiverReachMethodTypePointer < constitutingAdditionalRiverReachMethodTypes.end(); ++currentAdditionalRiverReachMethodTypePointer)
			{ (*(*currentAdditionalRiverReachMethodTypePointer)).typeSpecificAction(riverReachProperties, regularRiverReachMethods); }
}

}
