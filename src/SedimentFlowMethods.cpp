/*
 * SedimentFlowMethods.cpp
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

#include "SedimentFlowMethods.h"

namespace SedFlow {

SedimentFlowMethods::SedimentFlowMethods():
	FlowTypeMethods(CombinerVariables::sedimentFlowMethods,CombinerVariables::SedimentFlowMethodsInGeneral),
	updateErosionRatesInMarginCells(false)
{}

std::vector<SedimentFlowTypeMethods*> SedimentFlowMethods::forConstructorsCheckAndCopySingleSedimentFlowMethodsTypes(const std::vector<SedimentFlowTypeMethods*>& singleSedimentFlowMethodsTypes)
{
	std::vector<CombinerVariables::TypesOfSedimentFlowMethods> typesOfSedimentFlowMethods;
	for(std::vector<SedimentFlowTypeMethods*>::const_iterator i = singleSedimentFlowMethodsTypes.begin(); i < singleSedimentFlowMethodsTypes.end(); ++i)
		{ typesOfSedimentFlowMethods.push_back((*(*i)).getTypeOfSedimentFlowMethods()); }
	std::vector<CombinerVariables::TypesOfSedimentFlowMethods> sortedTypesOfSedimentFlowMethods = std::vector<CombinerVariables::TypesOfSedimentFlowMethods>(typesOfSedimentFlowMethods);
	std::sort(sortedTypesOfSedimentFlowMethods.begin(),sortedTypesOfSedimentFlowMethods.end());
	if (std::adjacent_find(sortedTypesOfSedimentFlowMethods.begin(),sortedTypesOfSedimentFlowMethods.end()) != sortedTypesOfSedimentFlowMethods.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	std::vector<SedimentFlowTypeMethods*> result;

	for(std::vector<SedimentFlowTypeMethods*>::const_iterator i = singleSedimentFlowMethodsTypes.begin(); i < singleSedimentFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).createSedimentFlowTypeMethodsPointerCopy());
	}

	return result;
}

SedimentFlowMethods::SedimentFlowMethods(const std::vector<SedimentFlowTypeMethods*>& singleSedimentFlowMethodsTypes, bool updateErosionRatesInMarginCells):
	FlowTypeMethods(CombinerVariables::sedimentFlowMethods,CombinerVariables::SedimentFlowMethodsInGeneral),
	updateErosionRatesInMarginCells(updateErosionRatesInMarginCells),
	constitutingSedimentFlowMethodsTypes( SedimentFlowMethods::forConstructorsCheckAndCopySingleSedimentFlowMethodsTypes(singleSedimentFlowMethodsTypes) )
{}

SedimentFlowMethods::SedimentFlowMethods(const FlowTypeMethods* toCopy):
	FlowTypeMethods(CombinerVariables::sedimentFlowMethods,CombinerVariables::SedimentFlowMethodsInGeneral)
{
	if( (*toCopy).getTypeOfFlowMethods() != CombinerVariables::sedimentFlowMethods)
	{
		const char *const errorMessage = "Action on not matching types of flow methods";
		throw(errorMessage);
	} else {
		this->updateErosionRatesInMarginCells = (static_cast<const SedimentFlowMethods*>(toCopy))->updateErosionRatesInMarginCells;
		this->constitutingSedimentFlowMethodsTypes = SedimentFlowMethods::forConstructorsCheckAndCopySingleSedimentFlowMethodsTypes( (static_cast<const SedimentFlowMethods*>(toCopy))->constitutingSedimentFlowMethodsTypes );
	}
}

SedimentFlowMethods::SedimentFlowMethods(const SedimentFlowMethods& toCopy):
	FlowTypeMethods(CombinerVariables::sedimentFlowMethods,CombinerVariables::SedimentFlowMethodsInGeneral),
	updateErosionRatesInMarginCells(toCopy.updateErosionRatesInMarginCells),
	constitutingSedimentFlowMethodsTypes( SedimentFlowMethods::forConstructorsCheckAndCopySingleSedimentFlowMethodsTypes(toCopy.constitutingSedimentFlowMethodsTypes) )
{}

SedimentFlowMethods::~SedimentFlowMethods()
{
	// The line { delete gradientCalculationMethod; } has already been called in FlowTypeMethods::~FlowTypeMethods()
	while(!(this->constitutingSedimentFlowMethodsTypes.empty()))
	{
		delete this->constitutingSedimentFlowMethodsTypes.back();
		this->constitutingSedimentFlowMethodsTypes.pop_back();
	}
}

FlowTypeMethods* SedimentFlowMethods::createFlowTypeMethodsPointerCopy() const
{
	FlowTypeMethods* result = new SedimentFlowMethods(this->constitutingSedimentFlowMethodsTypes, this->updateErosionRatesInMarginCells);
	return result;
}

ConstructionVariables SedimentFlowMethods::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;
	result.realisationType = CombinerVariables::typeOfFlowMethodsToString(CombinerVariables::sedimentFlowMethods);
	std::vector<bool> boolVector;
	boolVector.push_back(updateErosionRatesInMarginCells);
	result.labelledBools["updateErosionRatesInMarginCells"] = boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingSedimentFlowMethodsTypes.size() );
	for(std::vector<SedimentFlowTypeMethods*>::const_iterator iterator = constitutingSedimentFlowMethodsTypes.begin(); iterator < constitutingSedimentFlowMethodsTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingSedimentFlowMethodsTypes"] = constructionVariablesVector;
	return result;
}

std::vector<SedimentFlowTypeMethods*>::iterator SedimentFlowMethods::getSingleSedimentFlowMethodsTypeIterator(CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods)
{
	std::vector<SedimentFlowTypeMethods*>::iterator result = this->constitutingSedimentFlowMethodsTypes.end();
	bool alreadyFound = (false);

	for(std::vector<SedimentFlowTypeMethods*>::iterator i = this->constitutingSedimentFlowMethodsTypes.begin(); i < this->constitutingSedimentFlowMethodsTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfSedimentFlowMethods() == typeOfSedimentFlowMethods )
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

	if ( result == this->constitutingSedimentFlowMethodsTypes.end() )
	{
		const char *const secondErrorMessage = "In this simulation, this type of sediment flow methods is not used.";
		throw(secondErrorMessage);
	}
	else { return result;}
}

std::vector<SedimentFlowTypeMethods*>::const_iterator SedimentFlowMethods::getSingleSedimentFlowMethodsTypeConstIterator(CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods) const
{
	std::vector<SedimentFlowTypeMethods*>::const_iterator result = this->constitutingSedimentFlowMethodsTypes.end();
	bool alreadyFound = (false);

	for(std::vector<SedimentFlowTypeMethods*>::const_iterator i = this->constitutingSedimentFlowMethodsTypes.begin(); i < this->constitutingSedimentFlowMethodsTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfSedimentFlowMethods() == typeOfSedimentFlowMethods )
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

	if ( result == this->constitutingSedimentFlowMethodsTypes.end() )
	{
		const char *const secondErrorMessage = "In this simulation, this type of sediment flow methods is not used.";
		throw(secondErrorMessage);
	}
	else { return result;}
}

std::vector<SedimentFlowTypeMethods*>::iterator SedimentFlowMethods::getBeginSedimentFlowMethodsTypeIterator()
{
	return this->constitutingSedimentFlowMethodsTypes.begin();
}

std::vector<SedimentFlowTypeMethods*>::const_iterator SedimentFlowMethods::getBeginSedimentFlowMethodsTypeConstIterator() const
{
	std::vector<SedimentFlowTypeMethods*>::const_iterator result = this->constitutingSedimentFlowMethodsTypes.begin();
	return result;
}

std::vector<SedimentFlowTypeMethods*>::iterator SedimentFlowMethods::getEndSedimentFlowMethodsTypeIterator()
{
	return this->constitutingSedimentFlowMethodsTypes.end();
}

std::vector<SedimentFlowTypeMethods*>::const_iterator SedimentFlowMethods::getEndSedimentFlowMethodsTypeConstIterator() const
{
	std::vector<SedimentFlowTypeMethods*>::const_iterator result = this->constitutingSedimentFlowMethodsTypes.end();
	return result;
}

void SedimentFlowMethods::calculateChangeRate (RiverReachProperties& riverReachProperties) const
{
	if( updateErosionRatesInMarginCells || !(riverReachProperties.isMargin()) )
	{
		Grains tmpErosionRate = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0);
		tmpErosionRate.zeroFractions();
		for(std::vector<SedimentFlowTypeMethods*>::const_iterator currentTypeOfSedimentFlowMethods = this->constitutingSedimentFlowMethodsTypes.begin(); currentTypeOfSedimentFlowMethods < this->constitutingSedimentFlowMethodsTypes.end(); ++currentTypeOfSedimentFlowMethods)
		{
			(*(*currentTypeOfSedimentFlowMethods)).calculateChangeRate(riverReachProperties);
			tmpErosionRate += riverReachProperties.regularRiverReachProperties.erosionRate;
		}
		riverReachProperties.regularRiverReachProperties.erosionRate = tmpErosionRate;
	}
}

void SedimentFlowMethods::handDownChangeRate (RiverReachProperties& riverReachProperties) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).handDownChangeRate(riverReachProperties);
}

void SedimentFlowMethods::updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).updateChangeRateDependingParameters(riverReachProperties);
}

double SedimentFlowMethods::calculateTimeStep (const RiverSystemProperties& riverSystem) const
{
	std::vector<double> singleTimeSteps;
	for(std::vector<SedimentFlowTypeMethods*>::const_iterator currentTypeOfSedimentFlowMethods = this->constitutingSedimentFlowMethodsTypes.begin(); currentTypeOfSedimentFlowMethods < this->constitutingSedimentFlowMethodsTypes.end(); ++currentTypeOfSedimentFlowMethods)
	{
		singleTimeSteps.push_back( (*(*currentTypeOfSedimentFlowMethods)).calculateTimeStep(riverSystem) );
	}
	return *( std::min_element( singleTimeSteps.begin(), singleTimeSteps.end() ) );
}

void SedimentFlowMethods::calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).calculateChange(riverReachProperties, timeStep);
}

void SedimentFlowMethods::handDownChange (RiverReachProperties& riverReachProperties) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).handDownChange(riverReachProperties);
}

void SedimentFlowMethods::applyChange (RiverReachProperties& riverReachProperties) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).applyChange(riverReachProperties);
}

void SedimentFlowMethods::updateOtherParameters (RiverReachProperties& riverReachProperties) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).updateOtherParameters(riverReachProperties);
}
void SedimentFlowMethods::handDownOtherParameters (RiverReachProperties& riverReachProperties) const
{
	(*(this->constitutingSedimentFlowMethodsTypes.at(0))).handDownOtherParameters(riverReachProperties);
}

std::string SedimentFlowMethods::getTypeOfFlowMethodsAsString() const
{
	return CombinerVariables::typeOfFlowMethodsToString(this->typeOfFlowMethods);
}
CombinerVariables::TypesOfFlowMethods SedimentFlowMethods::getTypeOfFlowMethods() const
{
	return this->typeOfFlowMethods;
}

int SedimentFlowMethods::getNumberOfSedimentFlowMethodsTypes() const
{
	return this->constitutingSedimentFlowMethodsTypes.size();
}

std::vector<std::string> SedimentFlowMethods::getTypesOfSedimentFlowMethodsAsString() const
{
	std::vector<std::string> result;
	for(std::vector<SedimentFlowTypeMethods*>::const_iterator i = this->constitutingSedimentFlowMethodsTypes.begin(); i < this->constitutingSedimentFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfSedimentFlowMethodsAsString());
	}
	return result;
}

std::vector<CombinerVariables::TypesOfSedimentFlowMethods> SedimentFlowMethods::getTypesOfSedimentFlowMethods() const
{
	std::vector<CombinerVariables::TypesOfSedimentFlowMethods> result;
	for(std::vector<SedimentFlowTypeMethods*>::const_iterator i = this->constitutingSedimentFlowMethodsTypes.begin(); i < this->constitutingSedimentFlowMethodsTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfSedimentFlowMethods());
	}
	return result;
}

bool SedimentFlowMethods::matchingTypesOfSedimentFlowMethods (const SedimentFlowMethods& objectOfComparison) const
{
	std::vector<CombinerVariables::TypesOfSedimentFlowMethods> firstSortedTypesOfSedimentFlowMethods = std::vector<CombinerVariables::TypesOfSedimentFlowMethods>(this->getTypesOfSedimentFlowMethods());
	std::sort(firstSortedTypesOfSedimentFlowMethods.begin(),firstSortedTypesOfSedimentFlowMethods.end());
	std::vector<CombinerVariables::TypesOfSedimentFlowMethods> secondSortedTypesOfSedimentFlowMethods = std::vector<CombinerVariables::TypesOfSedimentFlowMethods>(objectOfComparison.getTypesOfSedimentFlowMethods());
	std::sort(secondSortedTypesOfSedimentFlowMethods.begin(),secondSortedTypesOfSedimentFlowMethods.end());

	if (firstSortedTypesOfSedimentFlowMethods == secondSortedTypesOfSedimentFlowMethods) {return (true);}
	else {return (false);}
}

SedimentFlowTypeMethods* SedimentFlowMethods::createPointerCopyOfSingleSedimentFlowTypeMethods(CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods) const
{
	return (*(*(this->getSingleSedimentFlowMethodsTypeConstIterator(typeOfSedimentFlowMethods)))).createSedimentFlowTypeMethodsPointerCopy();
}

}
