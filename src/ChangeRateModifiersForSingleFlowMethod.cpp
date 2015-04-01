/*
 * ChangeRateModifiersForSingleFlowMethod.cpp
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

#include "ChangeRateModifiersForSingleFlowMethod.h"

#include <algorithm>

namespace SedFlow {

std::vector<ChangeRateModifiersType*> ChangeRateModifiersForSingleFlowMethod::forConstructorsCheckAndCopySingleChangeRateModifiersTypes (const std::vector<ChangeRateModifiersType*>& singleChangeRateModifiersTypes)
{
	std::vector<CombinerVariables::TypesOfChangeRateModifiers> typesOfChangeRateModifiers;
	CombinerVariables::TypesOfGeneralFlowMethods firstCorrespondingGeneralFlowMethod = (*(singleChangeRateModifiersTypes.at(0))).getCorrespondingGeneralFlowMethod();
	for(std::vector<ChangeRateModifiersType*>::const_iterator i = singleChangeRateModifiersTypes.begin(); i < singleChangeRateModifiersTypes.end(); ++i)
	{
		if ( firstCorrespondingGeneralFlowMethod != (*(*i)).getCorrespondingGeneralFlowMethod() )
		{
			const char *const errorMessage = "Construction of modifiers set with non-corresponding General Flow Methods";
			throw(errorMessage);
		}
		typesOfChangeRateModifiers.push_back((*(*i)).getTypeOfChangeRateModifiers());
	}
	std::vector<CombinerVariables::TypesOfChangeRateModifiers> sortedTypesOfChangeRateModifiers = std::vector<CombinerVariables::TypesOfChangeRateModifiers>(typesOfChangeRateModifiers);
	std::sort(sortedTypesOfChangeRateModifiers.begin(),sortedTypesOfChangeRateModifiers.end());
	if (std::adjacent_find(sortedTypesOfChangeRateModifiers.begin(),sortedTypesOfChangeRateModifiers.end()) != sortedTypesOfChangeRateModifiers.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	std::vector<ChangeRateModifiersType*> result;

	for(std::vector<ChangeRateModifiersType*>::const_iterator i = singleChangeRateModifiersTypes.begin(); i < singleChangeRateModifiersTypes.end(); ++i)
	{
		result.push_back((*(*i)).createChangeRateModifiersTypePointerCopy());
	}

	return result;
}

ChangeRateModifiersForSingleFlowMethod::ChangeRateModifiersForSingleFlowMethod(const std::vector<ChangeRateModifiersType*>& singleChangeRateModifiersTypes):
	constitutingChangeRateModifiersTypes( ChangeRateModifiersForSingleFlowMethod::forConstructorsCheckAndCopySingleChangeRateModifiersTypes(singleChangeRateModifiersTypes) ),
	correspondingGeneralFlowMethod( (*(singleChangeRateModifiersTypes.at(0))).getCorrespondingGeneralFlowMethod() )
{}

ChangeRateModifiersForSingleFlowMethod::ChangeRateModifiersForSingleFlowMethod(const ChangeRateModifiersForSingleFlowMethod& toCopy):
	constitutingChangeRateModifiersTypes( ChangeRateModifiersForSingleFlowMethod::forConstructorsCheckAndCopySingleChangeRateModifiersTypes(toCopy.constitutingChangeRateModifiersTypes) ),
	correspondingGeneralFlowMethod( (*(toCopy.constitutingChangeRateModifiersTypes.at(0))).getCorrespondingGeneralFlowMethod() )
{}

ChangeRateModifiersForSingleFlowMethod::~ChangeRateModifiersForSingleFlowMethod()
{
	while(!(this->constitutingChangeRateModifiersTypes.empty()))
	{
		delete this->constitutingChangeRateModifiersTypes.back();
		this->constitutingChangeRateModifiersTypes.pop_back();
	}
}

ConstructionVariables ChangeRateModifiersForSingleFlowMethod::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersForSingleFlowMethod;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingChangeRateModifiersTypes.size() );
	for(std::vector<ChangeRateModifiersType*>::const_iterator iterator = constitutingChangeRateModifiersTypes.begin(); iterator < constitutingChangeRateModifiersTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingChangeRateModifiersTypes"] = constructionVariablesVector;
	return result;
}

void ChangeRateModifiersForSingleFlowMethod::inputModification(RiverReachProperties& riverReachProperties)
{
	for(std::vector<ChangeRateModifiersType*>::iterator currentChangeRateModifiers = constitutingChangeRateModifiersTypes.begin(); currentChangeRateModifiers < constitutingChangeRateModifiersTypes.end(); ++currentChangeRateModifiers)
		{ (*(*currentChangeRateModifiers)).inputModification(riverReachProperties); }
}

void ChangeRateModifiersForSingleFlowMethod::modificationBeforeUpdates(RiverReachProperties& riverReachProperties)
{
	for(std::vector<ChangeRateModifiersType*>::iterator currentChangeRateModifiers = constitutingChangeRateModifiersTypes.begin(); currentChangeRateModifiers < constitutingChangeRateModifiersTypes.end(); ++currentChangeRateModifiers)
			{ (*(*currentChangeRateModifiers)).modificationBeforeUpdates(riverReachProperties); }
}

bool ChangeRateModifiersForSingleFlowMethod::furtherModificationIterationNecessary(const RiverReachProperties& riverReachProperties) const
{
	bool result = true;
	for(std::vector<ChangeRateModifiersType*>::const_iterator currentChangeRateModifiers = constitutingChangeRateModifiersTypes.begin(); currentChangeRateModifiers < constitutingChangeRateModifiersTypes.end(); ++currentChangeRateModifiers)
			{ result = result && ( (*(*currentChangeRateModifiers)).furtherModificationIterationNecessary(riverReachProperties) ); }
	return result;
}

bool ChangeRateModifiersForSingleFlowMethod::furtherModificationIterationNecessary(const RiverSystemProperties& riverSystemProperties) const
{
	bool result = true;

	bool tmp;
	const RiverReachProperties* currentRiverReachProperties;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP ChangeRateModifiersForSingleFlowMethod::furtherModificationIterationNecessary" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentRiverReachProperties,tmp) default(shared)
	for(int i = 0; i < riverSystemProperties.regularRiverSystemProperties.cellProperties.size(); ++i)
	{
		currentRiverReachProperties = &(riverSystemProperties.regularRiverSystemProperties.cellProperties[i]);
		tmp = this->furtherModificationIterationNecessary((*currentRiverReachProperties));
		#pragma omp critical
		{ result = result && tmp; }
	}
	return result;
}

void ChangeRateModifiersForSingleFlowMethod::finalModification(RiverReachProperties& riverReachProperties)
{
	for(std::vector<ChangeRateModifiersType*>::iterator currentChangeRateModifiers = constitutingChangeRateModifiersTypes.begin(); currentChangeRateModifiers < constitutingChangeRateModifiersTypes.end(); ++currentChangeRateModifiers)
		{ (*(*currentChangeRateModifiers)).finalModification(riverReachProperties); }
}

}
