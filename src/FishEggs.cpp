/*
 * FishEggs.cpp
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

#include <vector>
#include <iostream>
#include <cstdlib>
#include <utility>

#include "FishEggs.h"
#include "CombinerVariables.h"

namespace SedFlow {

FishEggs::FishEggs():
	insertionLayer(0)
{
	this->typeOfGrains = CombinerVariables::FishEggs;
	this->typeSpecificGetterAvailable = true;
}

FishEggs::FishEggs(const std::vector<double> &fractionalAbundance):
	GrainType(fractionalAbundance),
	insertionLayer(0)
{
	this->typeOfGrains = CombinerVariables::FishEggs;
	this->typeSpecificGetterAvailable = true;
}

FishEggs::FishEggs(const std::vector<double> &fractionalAbundance, int insertionLayer):
	GrainType(fractionalAbundance)
{
	this->typeOfGrains = CombinerVariables::FishEggs;
	this->typeSpecificGetterAvailable = true;
	if (insertionLayer < 0 )
	{
		const char *const errorMessage = "Application of fish egg constructor with additional parameter not representing an insertion layer";
		throw(errorMessage);
	}

	this->insertionLayer = static_cast<unsigned int>(insertionLayer);
}

FishEggs::FishEggs(const std::vector<double>& fractionalAbundance, const std::vector<double>& additionalParameters):
	GrainType(fractionalAbundance)
{
	if (additionalParameters.size() != 1)
	{
		const char *const errorMessage ="Application of GrainType constructor with not matching number of additional parameters";
		throw(errorMessage);
	}
	if (additionalParameters.empty())
	{
		const char *const errorMessage = "Application of GrainType constructor with empty additional parameters";
		throw(errorMessage);
	}
	if (additionalParameters.at(0) < 0.0 || ( additionalParameters.at(0) - static_cast<int> (additionalParameters.at(0)) ) != 0.0 )
	{
		const char *const errorMessage = "Application of fish egg constructor with additional parameter not representing an insertion layer";
		throw(errorMessage);
	}

	this->typeOfGrains = CombinerVariables::FishEggs;
	this->typeSpecificGetterAvailable = true;
	this->insertionLayer = static_cast<unsigned int> (additionalParameters.at(0));
}

FishEggs::FishEggs(const GrainType* toCopy):
	GrainType(toCopy->getFractions())
{
	if( (*toCopy).getTypeOfGrains() != CombinerVariables::FishEggs)
	{
		const char *const errorMessage = "Action on not matching types of grains";
		throw(errorMessage);
	} else {
		this->typeOfGrains = CombinerVariables::FishEggs;
		this->typeSpecificGetterAvailable = true;
		this->insertionLayer = (*(dynamic_cast<const FishEggs*>(toCopy))).getInsertionLayer();
	}
}

FishEggs::~FishEggs() {}

GrainType* FishEggs::createGrainTypePointerCopy() const
{
	GrainType* result = new FishEggs(static_cast<const GrainType*>(this));
	return result;
}

ConstructionVariables FishEggs::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::GrainType;
	result.realisationType = CombinerVariables::typeOfGrainsToString(CombinerVariables::FishEggs);
	std::vector<double> doubleVector;
	doubleVector.reserve( fractionalAbundance.size() );
	for(std::vector<double>::const_iterator iterator = fractionalAbundance.begin(); iterator < fractionalAbundance.end(); ++iterator)
			{ doubleVector.push_back(*iterator); }
	result.labelledDoubles["fractionalAbundance"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(insertionLayer);
	result.labelledInts["insertionLayer"] = intVector;
	return result;
}

void FishEggs::insert(std::vector<GrainType*>& strata) const
{
	if (this->insertionLayer < strata.size())
	{
		if (*this == *(strata.at(this->insertionLayer)) )
		{
			if (this->insertionLayer == static_cast<unsigned int> (strata.at(this->insertionLayer)->typeSpecificGetter().second.at(0)) )
			{
				strata.at(this->insertionLayer)->add(*this);

			} else
			{
				const char *const errorMessage = "Operation on fish eggs with differing insertion layers";
				throw(errorMessage);
			}

		} else
		{
			const char *const errorMessage = "Operation on not matching grain types";
			throw(errorMessage);
		}

	} else
	{
		const char *const errorMessage = "Fish egg insertion layer beyond available layers";
		throw (errorMessage);
	}
}

GrainType* FishEggs::erode(std::vector<GrainType*>& strata) const
{
	GrainType* result = this->getPotentialErosion(strata);
	if ( this->areFractionsZero() == (false) ) { strata.at(0)->zeroFractions(); }
	return result;
}

GrainType* FishEggs::getPotentialErosion(const std::vector<GrainType*>& strata) const
{
	if (*this == *(strata.at(0)))
	{
		if (this->insertionLayer == static_cast<unsigned int> (strata.at(0)->typeSpecificGetter().second.at(0)) )
		{
			if ( this->areFractionsZero() == (false) )
			{
				return (strata.at(0))->createGrainTypePointerCopy();
			} else {return this->createGrainTypePointerCopy();}

		} else
		{
			const char *const errorMessage = "Operation on fish eggs with differing insertion layers";
			throw(errorMessage);
		}

	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}

void FishEggs::deposit(std::vector<GrainType*> &strata) const
{
	if(this->areFractionsZero()==(false))
	{
		const char *const errorMessage = "Trying to deposit fish eggs";
		throw(errorMessage);
	}
}

unsigned int FishEggs::getInsertionLayer() const {return this->insertionLayer;}

std::pair<CombinerVariables::TypesOfGrains , std::vector<double> > FishEggs::typeSpecificGetter() const
{
	return std::pair<CombinerVariables::TypesOfGrains , std::vector<double> >(this->typeOfGrains, std::vector<double>( 1 , static_cast<double> (this->getInsertionLayer()) )  );
}

}
