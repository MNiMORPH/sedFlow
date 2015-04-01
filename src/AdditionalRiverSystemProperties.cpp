/*
 * AdditionalRiverSystemProperties.cpp
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

#include "AdditionalRiverSystemProperties.h"

#include <algorithm>
#include <iostream>

namespace SedFlow {

std::vector<AdditionalRiverSystemPropertyType*> AdditionalRiverSystemProperties::forConstructorsCheckAndCopySingleAdditionalRiverSystemPropertyTypes (const std::vector<AdditionalRiverSystemPropertyType*>& singleAdditionalRiverSystemPropertyTypes)
{
	std::vector<CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod> typesOfAdditionalRiverSystemPropertyAndMethod;
	for(std::vector<AdditionalRiverSystemPropertyType*>::const_iterator i = singleAdditionalRiverSystemPropertyTypes.begin(); i < singleAdditionalRiverSystemPropertyTypes.end(); ++i)
		{ typesOfAdditionalRiverSystemPropertyAndMethod.push_back((*(*i)).getTypeOfAdditionalRiverSystemPropertyAndMethod()); }
	std::vector<CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod> sortedTypesOfAdditionalRiverSystemPropertyAndMethod = std::vector<CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod>(typesOfAdditionalRiverSystemPropertyAndMethod);
	std::sort(sortedTypesOfAdditionalRiverSystemPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverSystemPropertyAndMethod.end());
	if (std::adjacent_find(sortedTypesOfAdditionalRiverSystemPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverSystemPropertyAndMethod.end()) != sortedTypesOfAdditionalRiverSystemPropertyAndMethod.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	std::vector<AdditionalRiverSystemPropertyType*> result;

	for(std::vector<AdditionalRiverSystemPropertyType*>::const_iterator i = singleAdditionalRiverSystemPropertyTypes.begin(); i < singleAdditionalRiverSystemPropertyTypes.end(); ++i)
	{
		result.push_back((*(*i)).createAdditionalRiverSystemPropertyTypePointerCopy());
	}

	return result;
}

AdditionalRiverSystemProperties::AdditionalRiverSystemProperties(const std::vector<AdditionalRiverSystemPropertyType*>& singleAdditionalRiverSystemPropertyTypes):
	constitutingAdditionalRiverSystemPropertyTypes( AdditionalRiverSystemProperties::forConstructorsCheckAndCopySingleAdditionalRiverSystemPropertyTypes(singleAdditionalRiverSystemPropertyTypes) )
{}

ConstructionVariables AdditionalRiverSystemProperties::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverSystemProperties;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingAdditionalRiverSystemPropertyTypes.size() );
	for(std::vector<AdditionalRiverSystemPropertyType*>::const_iterator iterator = constitutingAdditionalRiverSystemPropertyTypes.begin(); iterator < constitutingAdditionalRiverSystemPropertyTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingAdditionalRiverSystemPropertyTypes"] = constructionVariablesVector;
	return result;
}

AdditionalRiverSystemProperties::AdditionalRiverSystemProperties(const AdditionalRiverSystemProperties& toCopy):
constitutingAdditionalRiverSystemPropertyTypes( AdditionalRiverSystemProperties::forConstructorsCheckAndCopySingleAdditionalRiverSystemPropertyTypes(toCopy.constitutingAdditionalRiverSystemPropertyTypes) )
{}

AdditionalRiverSystemProperties::~AdditionalRiverSystemProperties()
{
	while(!(this->constitutingAdditionalRiverSystemPropertyTypes.empty()))
	{
		delete this->constitutingAdditionalRiverSystemPropertyTypes.back();
		this->constitutingAdditionalRiverSystemPropertyTypes.pop_back();
	}
}

std::vector< std::pair< CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod , std::vector<double> > > AdditionalRiverSystemProperties::typeSpecificGetter() const
{
	std::vector< std::pair< CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod , std::vector<double> > > result;
	for(std::vector<AdditionalRiverSystemPropertyType*>::const_iterator currentPropertyAndMethod = constitutingAdditionalRiverSystemPropertyTypes.begin(); currentPropertyAndMethod < constitutingAdditionalRiverSystemPropertyTypes.end(); ++currentPropertyAndMethod)
			{  result.push_back( (*(*currentPropertyAndMethod)).typeSpecificGetter() ); }
	return result;
}

}
