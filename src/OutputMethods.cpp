/*
 * OutputMethods.cpp
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

#include "OutputMethods.h"

#include <algorithm>

namespace SedFlow {

std::vector<OutputMethodType*> OutputMethods::forConstructorsCheckAndCopySingleOutputMethodTypes (const std::vector<OutputMethodType*>& singleOutputMethodTypes)
{
		//As it is desirable to have multiple outputs of the same type, the following check for type-duplicates is not desired and therefore omitted.
		/*
		std::vector<CombinerVariables::TypesOfOutputMethod> typesOfOutputMethod;
		for(std::vector<OutputMethodType*>::const_iterator i = singleOutputMethodTypes.begin(); i < singleOutputMethodTypes.end(); ++i)
			{ typesOfOutputMethod.push_back((*(*i)).getTypeOfOutputMethod()); }
		std::vector<CombinerVariables::TypesOfOutputMethod> sortedTypesOfOutputMethod = std::vector<CombinerVariables::TypesOfOutputMethod>(typesOfOutputMethod);
		std::sort(sortedTypesOfOutputMethod.begin(),sortedTypesOfOutputMethod.end());
		if (std::adjacent_find(sortedTypesOfOutputMethod.begin(),sortedTypesOfOutputMethod.end()) != sortedTypesOfOutputMethod.end())
		{
			const char *const errorMessage = "Construction with duplicate combiner variables";
			throw(errorMessage);
		}
		*/

		std::vector<OutputMethodType*> result;
		result.reserve( singleOutputMethodTypes.size() );

		for(std::vector<OutputMethodType*>::const_iterator i = singleOutputMethodTypes.begin(); i < singleOutputMethodTypes.end(); ++i)
		{
			result.push_back((*(*i)).createOutputMethodTypePointerCopy());
		}

		return result;
}

OutputMethods::OutputMethods(const std::vector<OutputMethodType*>& singleOutputMethodTypes):
	constitutingOutputMethodTypes( OutputMethods::forConstructorsCheckAndCopySingleOutputMethodTypes(singleOutputMethodTypes) )
{}

ConstructionVariables OutputMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OutputMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingOutputMethodTypes.size() );
	for(std::vector<OutputMethodType*>::const_iterator iterator = constitutingOutputMethodTypes.begin(); iterator < constitutingOutputMethodTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingOutputMethodTypes"] = constructionVariablesVector;
	return result;
}

OutputMethods::OutputMethods(const OutputMethods& toCopy):
	constitutingOutputMethodTypes( OutputMethods::forConstructorsCheckAndCopySingleOutputMethodTypes(toCopy.constitutingOutputMethodTypes) )
{}

OutputMethods::~OutputMethods()
{
	while(!(this->constitutingOutputMethodTypes.empty()))
	{
		delete this->constitutingOutputMethodTypes.back();
		this->constitutingOutputMethodTypes.pop_back();
	}
}

void OutputMethods::initialiseOutput()const
{
	OutputMethodType* currentOutputMethod;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP OutputMethods::initialiseOutput" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentOutputMethod) default(shared)
	for(int i = 0; i < constitutingOutputMethodTypes.size(); ++i)
	{
		currentOutputMethod = constitutingOutputMethodTypes[i];
		currentOutputMethod->initialiseOutput(constitutingOutputMethodTypes);
	}
}

void OutputMethods::update()
{
	OutputMethodType* currentOutputMethod;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP OutputMethods::update" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentOutputMethod) default(shared)
	for(int i = 0; i < constitutingOutputMethodTypes.size(); ++i)
	{
		currentOutputMethod = constitutingOutputMethodTypes[i];
		currentOutputMethod->update(constitutingOutputMethodTypes);
	}
}

void OutputMethods::writeOutputLineIfScheduled()
{
	OutputMethodType* currentOutputMethod;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP OutputMethods::writeOutputLineIfScheduled" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentOutputMethod) default(shared)
	for(int i = 0; i < constitutingOutputMethodTypes.size(); ++i)
	{
		currentOutputMethod = constitutingOutputMethodTypes[i];
		currentOutputMethod->writeOutputLineIfScheduled(constitutingOutputMethodTypes);
	}
}

void OutputMethods::forcedWriteOutputLine()const
{
	OutputMethodType* currentOutputMethod;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP OutputMethods::forcedWriteOutputLine" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentOutputMethod) default(shared)
	for(int i = 0; i < constitutingOutputMethodTypes.size(); ++i)
	{
		currentOutputMethod = constitutingOutputMethodTypes[i];
		currentOutputMethod->forcedWriteOutputLine(constitutingOutputMethodTypes);
	}
}

void OutputMethods::finaliseOutput()const
{
	OutputMethodType* currentOutputMethod;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP OutputMethods::finaliseOutput" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentOutputMethod) default(shared)
	for(int i = 0; i < constitutingOutputMethodTypes.size(); ++i)
	{
		currentOutputMethod = constitutingOutputMethodTypes[i];
		currentOutputMethod->finaliseOutput(constitutingOutputMethodTypes);
	}
}


}
