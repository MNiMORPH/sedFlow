/*
 * FlowMethods.cpp
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

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "FlowMethods.h"

namespace SedFlow {

FlowMethods::FlowMethods() {}

std::vector<FlowTypeMethods*> FlowMethods::forConstructorsCheckAndCopySingleFlowMethodsTypes (const std::vector<FlowTypeMethods*>& singleFlowMethodsTypes)
{
	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> typesOfGeneralFlowMethods;
	for(std::vector<FlowTypeMethods*>::const_iterator i = singleFlowMethodsTypes.begin(); i < singleFlowMethodsTypes.end(); ++i)
	{
		typesOfGeneralFlowMethods.push_back((*(*i)).getTypeOfGeneralFlowMethods());
	}
	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> sortedTypesOfGeneralFlowMethods = std::vector<CombinerVariables::TypesOfGeneralFlowMethods>(typesOfGeneralFlowMethods);
	std::sort(sortedTypesOfGeneralFlowMethods.begin(),sortedTypesOfGeneralFlowMethods.end());
	if (std::adjacent_find(sortedTypesOfGeneralFlowMethods.begin(),sortedTypesOfGeneralFlowMethods.end()) != sortedTypesOfGeneralFlowMethods.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	std::vector<FlowTypeMethods*> result;

	for(std::vector<FlowTypeMethods*>::const_iterator i = singleFlowMethodsTypes.begin(); i < singleFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).createFlowTypeMethodsPointerCopy());
	}

	return result;
}

FlowMethods::FlowMethods(const std::vector<FlowTypeMethods*>& singleFlowMethodsTypes):
	constitutingFlowMethodsTypes( FlowMethods::forConstructorsCheckAndCopySingleFlowMethodsTypes(singleFlowMethodsTypes) )
{}

FlowMethods::FlowMethods(const FlowMethods& toCopy):
	constitutingFlowMethodsTypes( FlowMethods::forConstructorsCheckAndCopySingleFlowMethodsTypes(toCopy.constitutingFlowMethodsTypes) )
{}

FlowMethods::~FlowMethods()
{
	while(!(this->constitutingFlowMethodsTypes.empty()))
	{
		delete this->constitutingFlowMethodsTypes.back();
		this->constitutingFlowMethodsTypes.pop_back();
	}
}

ConstructionVariables FlowMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingFlowMethodsTypes.size() );
	for(std::vector<FlowTypeMethods*>::const_iterator iterator = constitutingFlowMethodsTypes.begin(); iterator < constitutingFlowMethodsTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingFlowMethodsTypes"] = constructionVariablesVector;
	return result;
}


std::vector<FlowTypeMethods*>::iterator FlowMethods::getSingleFlowMethodsTypeIterator(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods)
{
	std::vector<FlowTypeMethods*>::iterator result = this->constitutingFlowMethodsTypes.end();
	bool alreadyFound = (false);

	for(std::vector<FlowTypeMethods*>::iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfFlowMethods() == typeOfFlowMethods )
		{
			if (alreadyFound == (false))
			{
				result = i;
				alreadyFound = (true);

			} else
			{
				const char *const errorMessage = "Duplicate combiner variable";
				throw(errorMessage);
			}
		}
	}

	if ( result == this->constitutingFlowMethodsTypes.end() )
	{
		const char *const errorMessage = "In this simulation, this type of flow method is not used.";
		throw(errorMessage);
	}
	else { return result;}
}

std::vector<FlowTypeMethods*>::const_iterator FlowMethods::getSingleFlowMethodsTypeConstIterator(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods) const
{
	std::vector<FlowTypeMethods*>::const_iterator result = this->constitutingFlowMethodsTypes.end();
	bool alreadyFound = (false);

	for(std::vector<FlowTypeMethods*>::const_iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfFlowMethods() == typeOfFlowMethods )
		{
			if (alreadyFound == (false))
			{
				result = i;
				alreadyFound = (true);

			} else
			{
				const char *const errorMessage = "Duplicate combiner variable";
				throw(errorMessage);
			}
		}
	}

	if ( result == this->constitutingFlowMethodsTypes.end() )
	{
		const char *const errorMessage = "In this simulation, this type of flow method is not used.";
		throw(errorMessage);
	}
	else { return result;}
}

std::vector<FlowTypeMethods*>::iterator FlowMethods::getSingleFlowMethodsTypeIterator(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)
{
	std::vector<FlowTypeMethods*>::iterator result = this->constitutingFlowMethodsTypes.end();
	bool alreadyFound = (false);

	for(std::vector<FlowTypeMethods*>::iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfGeneralFlowMethods() == typeOfGeneralFlowMethods )
		{
			if (alreadyFound == (false))
			{
				result = i;
				alreadyFound = (true);

			} else
			{
				const char *const errorMessage = "Duplicate combiner variable";
				throw(errorMessage);
			}
		}
	}

	if ( result == this->constitutingFlowMethodsTypes.end() )
	{
		const char *const errorMessage = "In this simulation, this type of general flow method is not used.";
		throw(errorMessage);
	}
	else { return result;}
}

std::vector<FlowTypeMethods*>::const_iterator FlowMethods::getSingleFlowMethodsTypeConstIterator(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods) const
{
	std::vector<FlowTypeMethods*>::const_iterator result = this->constitutingFlowMethodsTypes.end();
	bool alreadyFound = (false);

	for(std::vector<FlowTypeMethods*>::const_iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfGeneralFlowMethods() == typeOfGeneralFlowMethods )
		{
			if (alreadyFound == (false))
			{
				result = i;
				alreadyFound = (true);

			} else
			{
				const char *const errorMessage = "Duplicate combiner variable";
				throw(errorMessage);
			}
		}
	}

	if ( result == this->constitutingFlowMethodsTypes.end() )
	{
		const char *const errorMessage = "In this simulation, this type of general flow method is not used.";
		throw(errorMessage);
	}
	else { return result;}
}


std::vector<FlowTypeMethods*>::iterator FlowMethods::getBeginFlowMethodsTypeIterator()
{
	return this->constitutingFlowMethodsTypes.begin();
}

std::vector<FlowTypeMethods*>::const_iterator FlowMethods::getBeginFlowMethodsTypeConstIterator() const
{
	std::vector<FlowTypeMethods*>::const_iterator result = this->constitutingFlowMethodsTypes.begin();
	return result;
}

std::vector<FlowTypeMethods*>::iterator FlowMethods::getEndFlowMethodsTypeIterator()
{
	return this->constitutingFlowMethodsTypes.end();
}
std::vector<FlowTypeMethods*>::const_iterator FlowMethods::getEndFlowMethodsTypeConstIterator() const
{
	std::vector<FlowTypeMethods*>::const_iterator result = this->constitutingFlowMethodsTypes.end();
	return result;
}

void FlowMethods::calculateChangeRates (RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
	{
		(*(*currentTypeOfFlowMethods)).calculateChangeRate(riverReachProperties);
	}
}

void FlowMethods::handDownChangeRates (RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
	{
		(*(*currentTypeOfFlowMethods)).handDownChangeRate(riverReachProperties);
	}
}

void FlowMethods::updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
	{
		(*(*currentTypeOfFlowMethods)).updateChangeRateDependingParameters(riverReachProperties);
	}
}

double FlowMethods::calculateTimeStep (const RiverSystemProperties& riverSystem) const
{
	std::vector<double> singleTimeSteps;
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
	{
		singleTimeSteps.push_back( (*(*currentTypeOfFlowMethods)).calculateTimeStep(riverSystem) );
	}
	return *( std::min_element( singleTimeSteps.begin(), singleTimeSteps.end() ) );
}

void FlowMethods::calculateChanges (RiverReachProperties& riverReachProperties, double timeStep) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
		{
			(*(*currentTypeOfFlowMethods)).calculateChange(riverReachProperties, timeStep);
		}
}

void FlowMethods::handDownChanges (RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
		{
			(*(*currentTypeOfFlowMethods)).handDownChange(riverReachProperties);
		}
}

void FlowMethods::applyChanges (RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
		{
			(*(*currentTypeOfFlowMethods)).applyChange(riverReachProperties);
		}
}
void FlowMethods::updateOtherParameters (RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
		{
			(*(*currentTypeOfFlowMethods)).updateOtherParameters(riverReachProperties);
		}
}

void FlowMethods::handDownOtherParameters (RiverReachProperties& riverReachProperties) const
{
	for(std::vector<FlowTypeMethods*>::const_iterator currentTypeOfFlowMethods = this->constitutingFlowMethodsTypes.begin(); currentTypeOfFlowMethods < this->constitutingFlowMethodsTypes.end(); ++currentTypeOfFlowMethods)
		{
			(*(*currentTypeOfFlowMethods)).handDownOtherParameters(riverReachProperties);
		}
}

int FlowMethods::getNumberOfFlowMethodsTypes() const
{
	return this->constitutingFlowMethodsTypes.size();
}
std::vector<std::string> FlowMethods::getTypesOfFlowMethodsAsString() const
{
	std::vector<std::string> result;
	for(std::vector<FlowTypeMethods*>::const_iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfFlowMethodsAsString());
	}
	return result;
}

std::vector<CombinerVariables::TypesOfFlowMethods> FlowMethods::getTypesOfFlowMethods() const
{
	std::vector<CombinerVariables::TypesOfFlowMethods> result;
	for(std::vector<FlowTypeMethods*>::const_iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfFlowMethods());
	}
	return result;
}

std::vector<std::string> FlowMethods::getTypesOfGeneralFlowMethodsAsString() const
{
	std::vector<std::string> result;
	for(std::vector<FlowTypeMethods*>::const_iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfGeneralFlowMethodsAsString());
	}
	return result;
}

std::vector<CombinerVariables::TypesOfGeneralFlowMethods> FlowMethods::getTypesOfGeneralFlowMethods() const
{
	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> result;
	for(std::vector<FlowTypeMethods*>::const_iterator i = this->constitutingFlowMethodsTypes.begin(); i < this->constitutingFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfGeneralFlowMethods());
	}
	return result;
}

bool FlowMethods::isFirstWaterFlow () const
{
	return (*constitutingFlowMethodsTypes.at(0)).isWaterFlow();
}

bool FlowMethods::matchingTypesOfFlowMethods (const FlowMethods& objectOfComparison) const
{
	std::vector<CombinerVariables::TypesOfFlowMethods> firstSortedTypesOfFlowMethods = std::vector<CombinerVariables::TypesOfFlowMethods>(this->getTypesOfFlowMethods());
	std::sort(firstSortedTypesOfFlowMethods.begin(),firstSortedTypesOfFlowMethods.end());
	std::vector<CombinerVariables::TypesOfFlowMethods> secondSortedTypesOfFlowMethods = std::vector<CombinerVariables::TypesOfFlowMethods>(objectOfComparison.getTypesOfFlowMethods());
	std::sort(secondSortedTypesOfFlowMethods.begin(),secondSortedTypesOfFlowMethods.end());

	if (firstSortedTypesOfFlowMethods == secondSortedTypesOfFlowMethods) {return (true);}
	else {return (false);}
}

FlowTypeMethods* FlowMethods::createPointerCopyOfSingleFlowTypeMethods(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods) const
{
	return (*(*(this->getSingleFlowMethodsTypeConstIterator(typeOfFlowMethods)))).createFlowTypeMethodsPointerCopy();
}

}
