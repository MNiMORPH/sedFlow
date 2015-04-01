/*
 * ChangeRateModifiers.cpp
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

#include "ChangeRateModifiers.h"

#include <algorithm>

namespace SedFlow {

std::vector<CombinerVariables::TypesOfGeneralFlowMethods> ChangeRateModifiers::forConstructorsCheckAndCreateTreatedTypesOfGeneralFlowMethodsFromSingleChangeRateModifiersForSingleFlowMethod (const std::vector<ChangeRateModifiersForSingleFlowMethod>& singleChangeRateModifiersForSingleFlowMethod)
{
	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> result;

	for(std::vector<ChangeRateModifiersForSingleFlowMethod>::const_iterator i = singleChangeRateModifiersForSingleFlowMethod.begin(); i < singleChangeRateModifiersForSingleFlowMethod.end(); ++i)
		{ result.push_back( (*i).getCorrespondingGeneralFlowMethod()); }

	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> sortedTypesOfCorrespondingFlowMethods = std::vector<CombinerVariables::TypesOfGeneralFlowMethods>(result);
	std::sort(sortedTypesOfCorrespondingFlowMethods.begin(),sortedTypesOfCorrespondingFlowMethods.end());
	if (std::adjacent_find(sortedTypesOfCorrespondingFlowMethods.begin(),sortedTypesOfCorrespondingFlowMethods.end()) != sortedTypesOfCorrespondingFlowMethods.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	return result;
}

ChangeRateModifiers::ChangeRateModifiers(const std::vector<ChangeRateModifiersForSingleFlowMethod>& singleChangeRateModifiersForSingleFlowMethod):
	treatedTypesOfGeneralFlowMethods( ChangeRateModifiers::forConstructorsCheckAndCreateTreatedTypesOfGeneralFlowMethodsFromSingleChangeRateModifiersForSingleFlowMethod(singleChangeRateModifiersForSingleFlowMethod) ),
	constitutingChangeRateModifiersForSingleFlowMethods(singleChangeRateModifiersForSingleFlowMethod)
{}

ConstructionVariables ChangeRateModifiers::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiers;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingChangeRateModifiersForSingleFlowMethods.size() );
	for(std::vector<ChangeRateModifiersForSingleFlowMethod>::const_iterator iterator = constitutingChangeRateModifiersForSingleFlowMethods.begin(); iterator < constitutingChangeRateModifiersForSingleFlowMethods.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator).createConstructionVariables() ); }
	result.labelledObjects["constitutingChangeRateModifiersForSingleFlowMethods"] = constructionVariablesVector;
	return result;
}


ChangeRateModifiers::ChangeRateModifiers(const ChangeRateModifiers& toCopy):
	treatedTypesOfGeneralFlowMethods( ChangeRateModifiers::forConstructorsCheckAndCreateTreatedTypesOfGeneralFlowMethodsFromSingleChangeRateModifiersForSingleFlowMethod(toCopy.constitutingChangeRateModifiersForSingleFlowMethods) ),
	constitutingChangeRateModifiersForSingleFlowMethods(toCopy.constitutingChangeRateModifiersForSingleFlowMethods)
{}

bool ChangeRateModifiers::checkForGeneralFlowMethodTreatment(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)const
{
	return (std::find(treatedTypesOfGeneralFlowMethods.begin(), treatedTypesOfGeneralFlowMethods.end(), typeOfGeneralFlowMethods) != treatedTypesOfGeneralFlowMethods.end());
}

ChangeRateModifiersForSingleFlowMethod& ChangeRateModifiers::getChangeRateModifiersCorrespondingToGeneralFlowMethod(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)
{
	ChangeRateModifiersForSingleFlowMethod& result = *(this->getChangeRateModifiersCorrespondingToGeneralFlowMethodIterator(typeOfGeneralFlowMethods));
	return result;
}

ChangeRateModifiersForSingleFlowMethod* ChangeRateModifiers::getChangeRateModifiersCorrespondingToGeneralFlowMethodPointer(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)
{
	ChangeRateModifiersForSingleFlowMethod* result = &(*(this->getChangeRateModifiersCorrespondingToGeneralFlowMethodIterator(typeOfGeneralFlowMethods)));
	return result;
}

std::vector<ChangeRateModifiersForSingleFlowMethod>::iterator ChangeRateModifiers::getChangeRateModifiersCorrespondingToGeneralFlowMethodIterator(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)
{
	std::vector<ChangeRateModifiersForSingleFlowMethod>::iterator result = this->constitutingChangeRateModifiersForSingleFlowMethods.end();
	bool alreadyFound = (false);

	for(std::vector<ChangeRateModifiersForSingleFlowMethod>::iterator i = this->constitutingChangeRateModifiersForSingleFlowMethods.begin(); i < this->constitutingChangeRateModifiersForSingleFlowMethods.end(); ++i)
	{
		if( (*i).getCorrespondingGeneralFlowMethod() == typeOfGeneralFlowMethods )
		{
			if (alreadyFound == (false))
			{
				result = i;
				alreadyFound = (true);

			} else
			{
				const char *const firstErrorMessage = "Duplicate combiner variable";
				throw(firstErrorMessage);
			}
		}
	}

	if ( result == this->constitutingChangeRateModifiersForSingleFlowMethods.end() )
	{
		const char *const secondErrorMessage = "In this simulation, there are no change rate modifiers for this type of general flow method.";
		throw(secondErrorMessage);
	}
	else { return result;}
}

}
