/*
 * AdditionalRiverReachProperties.cpp
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

#include "AdditionalRiverReachProperties.h"

#include <algorithm>

namespace SedFlow {

std::vector<AdditionalRiverReachPropertyType*> AdditionalRiverReachProperties::forConstructorsCheckAndCopySingleAdditionalRiverReachPropertyTypes (const std::vector<AdditionalRiverReachPropertyType*>& singleAdditionalRiverReachPropertyTypes)
{
		std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod> typesOfAdditionalRiverPropertyAndMethod;
		for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator i = singleAdditionalRiverReachPropertyTypes.begin(); i < singleAdditionalRiverReachPropertyTypes.end(); ++i)
			{ typesOfAdditionalRiverPropertyAndMethod.push_back((*(*i)).getTypeOfAdditionalRiverReachPropertyAndMethod()); }
		std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod> sortedTypesOfAdditionalRiverPropertyAndMethod = std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod>(typesOfAdditionalRiverPropertyAndMethod);
		std::sort(sortedTypesOfAdditionalRiverPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverPropertyAndMethod.end());
		if (std::adjacent_find(sortedTypesOfAdditionalRiverPropertyAndMethod.begin(),sortedTypesOfAdditionalRiverPropertyAndMethod.end()) != sortedTypesOfAdditionalRiverPropertyAndMethod.end())
		{
			const char *const errorMessage = "Construction with duplicate combiner variables";
			throw(errorMessage);
		}

		std::vector<AdditionalRiverReachPropertyType*> result;

		for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator i = singleAdditionalRiverReachPropertyTypes.begin(); i < singleAdditionalRiverReachPropertyTypes.end(); ++i)
		{
			result.push_back( (*i)->createAdditionalRiverPropertyAndMethodTypePointerCopy() );
		}

		return result;
}

AdditionalRiverReachProperties::AdditionalRiverReachProperties(const std::vector<AdditionalRiverReachPropertyType*>& singleAdditionalRiverReachPropertyTypes):
	constitutingAdditionalRiverReachPropertyTypes( AdditionalRiverReachProperties::forConstructorsCheckAndCopySingleAdditionalRiverReachPropertyTypes(singleAdditionalRiverReachPropertyTypes) )
{
	this->mapFromTypeToPointerOfConstitutingProperties.clear();
	for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator currentProperty = this->constitutingAdditionalRiverReachPropertyTypes.begin(); currentProperty < this->constitutingAdditionalRiverReachPropertyTypes.end(); ++currentProperty)
	{
		this->mapFromTypeToPointerOfConstitutingProperties[ (*currentProperty)->getTypeOfAdditionalRiverReachPropertyAndMethod() ] = *currentProperty;
	}
}

AdditionalRiverReachProperties::AdditionalRiverReachProperties(const AdditionalRiverReachProperties& toCopy):
		constitutingAdditionalRiverReachPropertyTypes( AdditionalRiverReachProperties::forConstructorsCheckAndCopySingleAdditionalRiverReachPropertyTypes(toCopy.constitutingAdditionalRiverReachPropertyTypes) )
{
	this->mapFromTypeToPointerOfConstitutingProperties.clear();
	for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator currentProperty = this->constitutingAdditionalRiverReachPropertyTypes.begin(); currentProperty < this->constitutingAdditionalRiverReachPropertyTypes.end(); ++currentProperty)
	{
		this->mapFromTypeToPointerOfConstitutingProperties[ (*currentProperty)->getTypeOfAdditionalRiverReachPropertyAndMethod() ] = *currentProperty;
	}
}

AdditionalRiverReachProperties::~AdditionalRiverReachProperties()
{
	while(!(this->constitutingAdditionalRiverReachPropertyTypes.empty()))
	{
		delete this->constitutingAdditionalRiverReachPropertyTypes.back();
		this->constitutingAdditionalRiverReachPropertyTypes.pop_back();
	}
}

ConstructionVariables AdditionalRiverReachProperties::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachProperties;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingAdditionalRiverReachPropertyTypes.size() );
	for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator iterator = constitutingAdditionalRiverReachPropertyTypes.begin(); iterator < constitutingAdditionalRiverReachPropertyTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingAdditionalRiverReachPropertyTypes"] = constructionVariablesVector;
	return result;
}

AdditionalRiverReachPropertyType* AdditionalRiverReachProperties::getAdditionalPropertyPointer(CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalProperty)
{
	AdditionalRiverReachPropertyType* result = const_cast<AdditionalRiverReachPropertyType*>(this->getAdditionalPropertyPointer(typeOfAdditionalProperty));
	return result;
}
const AdditionalRiverReachPropertyType* AdditionalRiverReachProperties::getAdditionalPropertyConstPointer(CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalProperty) const
{
	const AdditionalRiverReachPropertyType* result = NULL;
	std::map<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod,AdditionalRiverReachPropertyType*>::const_iterator resultIterator = this->mapFromTypeToPointerOfConstitutingProperties.find(typeOfAdditionalProperty);
	if(resultIterator != mapFromTypeToPointerOfConstitutingProperties.end())
		{ result = resultIterator->second; }
	return result;
}

std::vector< std::pair< CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod , std::vector<double> > >  AdditionalRiverReachProperties::typeSpecificGetter() const
{
	std::vector< std::pair< CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod , std::vector<double> > > result;
	for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator currentPropertyAndMethod = constitutingAdditionalRiverReachPropertyTypes.begin(); currentPropertyAndMethod < constitutingAdditionalRiverReachPropertyTypes.end(); ++currentPropertyAndMethod)
			{  result.push_back( (*(*currentPropertyAndMethod)).typeSpecificGetter() ); }
	return result;
}

}
