/*
 * Grains.cpp
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
#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <limits>

#include "Grains.h"

namespace SedFlow {

void Grains::naCheck() const
{
	if( ((this->constitutingGrainTypes.size()) != 0) && ((this->getNumberOfFractions())!= 0) )
	{
		double tmp = this->getOverallVolume();
		if( tmp != tmp )
		{
			const char *const errorMessage = "OccurenceOfNaN";
			throw(errorMessage);
		}
	}
}

std::vector<GrainType*> Grains::forConstructorsCheckAndCopySingleGrainTypes (const std::vector<GrainType*>& singleGrainTypes)
{
	std::vector<CombinerVariables::TypesOfGrains> typesOfGrains;
	typesOfGrains.reserve( singleGrainTypes.size() );
	for(std::vector<GrainType*>::const_iterator i = singleGrainTypes.begin(); i < singleGrainTypes.end(); ++i)
		{ typesOfGrains.push_back((*(*i)).getTypeOfGrains()); }
	std::vector<CombinerVariables::TypesOfGrains> sortedTypesOfGrains = std::vector<CombinerVariables::TypesOfGrains>(typesOfGrains);
	std::sort(sortedTypesOfGrains.begin(),sortedTypesOfGrains.end());
	if (std::adjacent_find(sortedTypesOfGrains.begin(),sortedTypesOfGrains.end()) != sortedTypesOfGrains.end())
	{
		const char *const errorMessage = "Construction with duplicate combiner variables";
		throw(errorMessage);
	}

	return GrainType::copyGrainTypePointerVector(singleGrainTypes);
}

Grains::Grains(const std::vector<GrainType*>& singleGrainTypes):
	constitutingGrainTypes( Grains::forConstructorsCheckAndCopySingleGrainTypes(singleGrainTypes) )
{}

Grains::Grains(const Grains& toCopy):
#if defined _DEBUG || defined DEBUG
	constitutingGrainTypes( Grains::forConstructorsCheckAndCopySingleGrainTypes(toCopy.constitutingGrainTypes) )
#else
	constitutingGrainTypes( GrainType::copyGrainTypePointerVector(toCopy.constitutingGrainTypes) )
#endif
{
	#if defined _DEBUG || defined DEBUG
		this->naCheck();
	#endif
}

Grains::~Grains()
{
	while( ! (this->constitutingGrainTypes.empty()) )
	{
		delete this->constitutingGrainTypes.back();
		this->constitutingGrainTypes.pop_back();
	}
}

ConstructionVariables Grains::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::Grains;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( constitutingGrainTypes.size() );
	for(std::vector<GrainType*>::const_iterator iterator = constitutingGrainTypes.begin(); iterator < constitutingGrainTypes.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator)->createConstructionVariables() ); }
	result.labelledObjects["constitutingGrainTypes"] = constructionVariablesVector;
	return result;
}

void Grains::insert(std::vector<Grains>& strata) const
{
	if ( this->matchingTypesOfGrains(strata.at(0)) == (false) )
	{
		const char *const errorMessage = "Action on not matching types of grains";
		throw(errorMessage);
	}

	std::vector< std::vector<GrainType*> > grainTypePointerFrontEndForStrata = Grains::displayStrataByGrainTypePointerFrontEnd(strata);
	for(std::vector< std::vector<GrainType*> >::iterator currentGrains = grainTypePointerFrontEndForStrata.begin(); currentGrains < grainTypePointerFrontEndForStrata.end(); ++currentGrains)
	{
		(*(this->getSingleGrainTypeConstIterator((*(*currentGrains).at(0)).getTypeOfGrains())))->insert(*currentGrains);
	}
}

Grains Grains::erode(std::vector<Grains>& strata) const
{
	if ( this->matchingTypesOfGrains(strata.at(0)) == (false) )
	{
		const char *const errorMessage = "Action on not matching types of grains";
		throw(errorMessage);
	}

	std::vector< std::vector<GrainType*> > grainTypePointerFrontEndForStrata = Grains::displayStrataByGrainTypePointerFrontEnd(strata);
	std::vector<GrainType*> resultVector;
	for(std::vector< std::vector<GrainType*> >::iterator currentGrains = grainTypePointerFrontEndForStrata.begin(); currentGrains < grainTypePointerFrontEndForStrata.end(); ++currentGrains)
	{
		resultVector.push_back( (*(this->getSingleGrainTypeConstIterator((*(*currentGrains).at(0)).getTypeOfGrains())))->erode(*currentGrains) );
	}
	Grains result = Grains(resultVector);
	while( ! (resultVector.empty()) )
	{
		delete resultVector.back();
		resultVector.pop_back();
	}
	return result;
}

Grains Grains::getPotentialErosion(const std::vector<Grains>& strata) const
{
	if ( this->matchingTypesOfGrains(strata.at(0)) == (false) )
	{
		const char *const errorMessage = "Action on not matching types of grains";
		throw(errorMessage);
	}

	std::vector< std::vector<const GrainType*> > constGrainTypePointerFrontEndForStrata = Grains::displayStrataByConstGrainTypePointerFrontEnd(strata);
	std::vector<GrainType*> resultVector;
	for(std::vector< std::vector<const GrainType*> >::iterator currentGrains = constGrainTypePointerFrontEndForStrata.begin(); currentGrains < constGrainTypePointerFrontEndForStrata.end(); ++currentGrains)
	{
		resultVector.push_back( (*(this->getSingleGrainTypeConstIterator((*(*currentGrains).at(0)).getTypeOfGrains())))->getPotentialErosion(*currentGrains) );
	}
	Grains result = Grains(resultVector);
	while( ! (resultVector.empty()) )
	{
		delete resultVector.back();
		resultVector.pop_back();
	}
	return result;
}

void Grains::deposit(std::vector<Grains>& strata) const
{
	if ( this->matchingTypesOfGrains(strata.at(0)) == (false) )
	{
		const char *const errorMessage = "Action on not matching types of grains";
		throw(errorMessage);
	}

	std::vector< std::vector<GrainType*> > grainTypePointerFrontEndForStrata = Grains::displayStrataByGrainTypePointerFrontEnd(strata);
	for(std::vector< std::vector<GrainType*> >::iterator currentGrains = grainTypePointerFrontEndForStrata.begin(); currentGrains < grainTypePointerFrontEndForStrata.end(); ++currentGrains)
	{
		(*(this->getSingleGrainTypeConstIterator((*(*currentGrains).at(0)).getTypeOfGrains())))->deposit(*currentGrains);
	}
}

void Grains::add(const Grains& toAdd)
{
	if ( this->matchingTypesOfGrains(toAdd) == (false) )
	{
		const char *const errorMessage = "Action on not matching number of grain types";
		throw(errorMessage);
	}

	for(std::vector<GrainType*>::iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		(*(*i)).add(*(toAdd.getSingleGrainTypeConstIterator((*(*i)).getTypeOfGrains())));
	}
}


Grains Grains::subtract(const Grains& toSubtract)
{
	if ( this->matchingTypesOfGrains(toSubtract) == (false) )
	{
		const char *const errorMessage = "Action on not matching number of grain types";
		throw(errorMessage);
	}

	std::vector<GrainType*> resultVector;
	GrainType* currentGrainTypePointer;
	for(std::vector<GrainType*>::iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		currentGrainTypePointer = toSubtract.createPointerCopyOfSingleGrainType((*(*i)).getTypeOfGrains());
		resultVector.push_back( (*(*i)).subtract(currentGrainTypePointer) );
		delete currentGrainTypePointer;
	}
	Grains result = Grains(resultVector);

	while( ! (resultVector.empty()) )
	{
		delete resultVector.back();
		resultVector.pop_back();
	}

	return result;
}

Grains Grains::getPotentialSubtraction(const Grains& toSubtract) const
{
	if ( this->matchingTypesOfGrains(toSubtract) == (false) )
	{
		const char *const errorMessage = "Action on not matching number of grain types";
		throw(errorMessage);
	}

	std::vector<GrainType*> resultVector;
	GrainType* currentGrainTypePointer;
	for(std::vector<GrainType*>::const_iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		currentGrainTypePointer = toSubtract.createPointerCopyOfSingleGrainType((*(*i)).getTypeOfGrains());
		resultVector.push_back( (*(*i)).getPotentialSubtraction(currentGrainTypePointer));
		delete currentGrainTypePointer;
	}
	Grains result = Grains(resultVector);

	while( ! (resultVector.empty()) )
	{
		delete resultVector.back();
		resultVector.pop_back();
	}

	return result;
}

Grains Grains::distributeFractionalAbundance(const std::vector<double>& fractionalAbundance, Grains distributionBase)
{
	if (fractionalAbundance.size() != distributionBase.getNumberOfFractions())
	{
		const char *const errorMessage = "Action on not matching number of fractions";
		throw(errorMessage);
	}

	double oneOverNumberOfGrainTypes = 1.0 / static_cast<double>( distributionBase.getNumberOfGrainTypes() );

	double tmp;
	std::vector<double> overallAbundances = distributionBase.getOverallFractionalAbundance();
	int fraction = 0;

	for(std::vector<double>::const_iterator inputFraction = fractionalAbundance.begin(); inputFraction < fractionalAbundance.end(); ++inputFraction, ++fraction)
	{
		for(std::vector<GrainType*>::iterator grainType = distributionBase.getBeginGrainTypeIterator(); grainType < distributionBase.getEndGrainTypeIterator(); ++grainType)
		{
			if( overallAbundances.at(fraction) == 0.0 )
				{ tmp = (*inputFraction) * oneOverNumberOfGrainTypes; }
			else
				{ tmp = (*inputFraction) * ( (*(*grainType)).getFraction(fraction) / overallAbundances.at(fraction) ); }
			(*(*grainType)).setFraction(fraction, tmp);
		}
	}

	return distributionBase;
}


double Grains::getPercentileGrainDiameter(const std::vector<double>& fractionalGrainDiameters, double percentileRank) const
{
	std::vector<double> overallFractionalAbundances = this->getOverallFractionalAbundance();
	return getPercentileGrainDiameter(fractionalGrainDiameters,overallFractionalAbundances,percentileRank);
}

double Grains::getPercentileGrainDiameter(const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, double percentileRank)
{
#if defined _DEBUG || defined DEBUG
		if (fractionalGrainDiameters.size() != fractionalAbundances.size())
		{
			const char *const errorMessage = "Action on not matching number of fractions";
			throw(errorMessage);
		}
#endif

	double result = std::numeric_limits<double>::quiet_NaN();
	double overallVolume = std::accumulate(fractionalAbundances.begin(),fractionalAbundances.end(),0.0);
	if ( overallVolume > 0.0 )
	{
		percentileRank *= 0.01;
		std::vector<double> normalisedCumulativeAbundance = getOverallCumulativeFractionalAbundance(fractionalAbundances);
		int currentFractionID = 0;
		int upperFractionID = 0;
		bool notYetFound = true;
		for(std::vector<double>::iterator currentNormalisedCumulativeAbundance = normalisedCumulativeAbundance.begin(); currentNormalisedCumulativeAbundance < normalisedCumulativeAbundance.end(); ++currentNormalisedCumulativeAbundance, ++currentFractionID)
		{
			*currentNormalisedCumulativeAbundance /= overallVolume;
			if(notYetFound)
			{
				if( *currentNormalisedCumulativeAbundance >= percentileRank)
				{
					upperFractionID = currentFractionID;
					notYetFound = false;
				}
			}
		}

		int lowerFractionID;
		if (upperFractionID == 0) { result = fractionalGrainDiameters.at(upperFractionID); }
		else
		{
			lowerFractionID = upperFractionID-1;
			double upperDiameter = fractionalGrainDiameters.at(upperFractionID);
			double lowerDiameter = fractionalGrainDiameters.at(lowerFractionID);
			double upperNormCumulativeAbundance = normalisedCumulativeAbundance.at(upperFractionID);
			double lowerNormCumulativeAbundance = normalisedCumulativeAbundance.at(lowerFractionID);


			result = exp( ( log(lowerDiameter) + ( (log(upperDiameter)-log(lowerDiameter)) * (percentileRank - lowerNormCumulativeAbundance) / (upperNormCumulativeAbundance - lowerNormCumulativeAbundance) ) ) );
		}
	}
	return result;
}

double Grains::getArithmeticMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters) const
{
	std::vector<double> overallFractionalAbundances = this->getOverallFractionalAbundance();
	return getArithmenticMeanGrainDiameter(fractionalGrainDiameters,overallFractionalAbundances);
}

double Grains::getArithmenticMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters, std::vector<double> fractionalAbundances)
{
#if defined _DEBUG || defined DEBUG
	if (fractionalGrainDiameters.size() != fractionalAbundances.size())
	{
		const char *const errorMessage = "Action on not matching number of fractions";
		throw(errorMessage);
	}
#endif

	double result = std::numeric_limits<double>::quiet_NaN();

	std::vector<double>::iterator fractionalAbundancesBegin = fractionalAbundances.begin();
	std::vector<double>::iterator fractionalAbundancesEnd = fractionalAbundances.end();

	double overallVolume = std::accumulate(fractionalAbundancesBegin,fractionalAbundancesEnd,0.0);
	if ( overallVolume > 0.0 )
	{
		//Make fractionalAbundances relative.
		std::transform(fractionalAbundancesBegin,fractionalAbundancesEnd,fractionalAbundancesBegin,std::bind2nd(std::divides<double>(),overallVolume));
		//Make fractionalAbundances weightedGrainDiameters.
		std::transform(fractionalAbundancesBegin,fractionalAbundancesEnd,fractionalGrainDiameters.begin(),fractionalAbundancesBegin,std::multiplies<double>());
		//Summation series.
		result = std::accumulate(fractionalAbundancesBegin,fractionalAbundancesEnd,0.0);
	}
	return result;

}

double Grains::getGeometricMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters) const
{
	std::vector<double> overallFractionalAbundances = this->getOverallFractionalAbundance();
	return getGeometricMeanGrainDiameter(fractionalGrainDiameters,overallFractionalAbundances);
}

double Grains::getGeometricMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters, std::vector<double> fractionalAbundances)
{
#if defined _DEBUG || defined DEBUG
	if (fractionalGrainDiameters.size() != fractionalAbundances.size())
	{
		const char *const errorMessage = "Action on not matching number of fractions";
		throw(errorMessage);
	}
#endif

	double result = std::numeric_limits<double>::quiet_NaN();

	std::vector<double>::iterator fractionalAbundancesBegin = fractionalAbundances.begin();
	std::vector<double>::iterator fractionalAbundancesEnd = fractionalAbundances.end();

	double overallVolume = std::accumulate(fractionalAbundancesBegin,fractionalAbundancesEnd,0.0);
	if ( overallVolume > 0.0 )
	{
		//Make fractionalAbundances relative.
		std::transform(fractionalAbundancesBegin,fractionalAbundancesEnd,fractionalAbundancesBegin,std::bind2nd(std::divides<double>(),overallVolume));
		//Make fractionalAbundances weightedGrainDiameters.
		int i = 0;
		for(std::vector<double>::iterator currentFractionalAbundance = fractionalAbundancesBegin; currentFractionalAbundance < fractionalAbundancesEnd; ++i, ++currentFractionalAbundance)
		{
			*currentFractionalAbundance = pow(fractionalGrainDiameters.at(i),(*currentFractionalAbundance));
		}
		//Multiplication series.
		result = std::accumulate(fractionalAbundancesBegin,fractionalAbundancesEnd,1.0,std::multiplies<double>());
	}
	return result;

}

int Grains::getNumberOfGrainTypes() const
{
	return this->constitutingGrainTypes.size();
}

int Grains::getNumberOfFractions() const
{
#if defined _DEBUG || defined DEBUG
	if((this->constitutingGrainTypes.size()) == 0)
	{
		const char *const errorMessage = "Action on empty grains container (Grains::getNumberOfFractions No GrainTypes)";
		throw(errorMessage);
	}
#endif

	int result = (this->constitutingGrainTypes[0])->getNumberOfFractions();

#if defined _DEBUG || defined DEBUG
	if (result == 0)
	{
		const char *const errorMessage = "Action on empty grains container (Grains::getNumberOfFractions No Fractions)";
		throw(errorMessage);
	}
	for(std::vector<GrainType*>::const_iterator i = ((this->getBeginGrainTypeConstIterator()) + 1); i < this->getEndGrainTypeConstIterator(); ++i)
	{
		if (result != ((*(*i)).getNumberOfFractions()) )
		{
			const char *const errorMessage = "Not matching number of fractions within grains container";
			throw(errorMessage);
		}
	}
#endif

		return result;
}

std::vector<double> Grains::getOverallFractionalAbundance() const
{
	int numberOfFractions = this->getNumberOfFractions();
	std::vector<double> result = std::vector<double>(numberOfFractions, 0.0);
	std::vector<double>::iterator resultBegin = result.begin();
	std::vector<double>::iterator resultEnd = result.end();
	std::vector<double> singleGrainTypeFractions;

	for(std::vector<GrainType*>::const_iterator grainTypeIt = this->getBeginGrainTypeConstIterator(); grainTypeIt < this->getEndGrainTypeConstIterator(); grainTypeIt++)
	{
		singleGrainTypeFractions = (*(*grainTypeIt)).getFractions();
		std::transform(resultBegin,resultEnd,singleGrainTypeFractions.begin(),resultBegin,std::plus<double>());
	}
	return result;
}

std::vector<double> Grains::getOverallCumulativeFractionalAbundance() const
{
	std::vector<double> overallFractionalAbundance = this->getOverallFractionalAbundance();
	return getOverallCumulativeFractionalAbundance(overallFractionalAbundance);
}

std::vector<double> Grains::getOverallCumulativeFractionalAbundance(const std::vector<double>& fractionalAbundances)
{
	double cumulativeAbundance = 0.0;
	std::vector<double> result;
	result.reserve( fractionalAbundances.size() );
	for(std::vector<double>::const_iterator currentAbundance = fractionalAbundances.begin(); currentAbundance < fractionalAbundances.end(); ++currentAbundance)
	{
		cumulativeAbundance += *currentAbundance;
		result.push_back(cumulativeAbundance);
	}
	return result;
}

double Grains::getOverallVolume() const
{
	std::vector<double> overallFractionalAbundance = this->getOverallFractionalAbundance();
	return std::accumulate(overallFractionalAbundance.begin(),overallFractionalAbundance.end(),0.0);
}

double Grains::getOverallVolumeWithinDiameterRange(const std::vector<double>& fractionalGrainDiameters, double firstDiameterLimit, double secondDiameterLimit) const
{
	if(this->getNumberOfFractions() != fractionalGrainDiameters.size())
	{
		const char *const errorMessage = "Action on not matching number of fractions";
		throw(errorMessage);
	}
	if(firstDiameterLimit > secondDiameterLimit)
	{
		double tempDiameterLimit = firstDiameterLimit;
		firstDiameterLimit = secondDiameterLimit;
		secondDiameterLimit = tempDiameterLimit;
	}
	std::vector<double> overallFractionalAbundances = this->getOverallFractionalAbundance();
	double result = 0.0;
	for(std::vector<double>::const_iterator currentGrainDiameter = fractionalGrainDiameters.begin(), currentFractionalAbundance = overallFractionalAbundances.begin(); currentGrainDiameter < fractionalGrainDiameters.end(); ++currentGrainDiameter, ++currentFractionalAbundance)
	{
		if( (*currentGrainDiameter) <= secondDiameterLimit && (*currentGrainDiameter) >= firstDiameterLimit ) { result += (*currentFractionalAbundance); }
	}
	return result;
}

std::vector<std::string> Grains::getTypesOfGrainsAsString() const
{
	std::vector<std::string> result;
	for(std::vector<GrainType*>::const_iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfGrainsAsString());
	}
	return result;
}

std::vector<CombinerVariables::TypesOfGrains> Grains::getTypesOfGrains() const
{
	std::vector<CombinerVariables::TypesOfGrains> result;
	for (std::vector<GrainType*>::const_iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		result.push_back((*(*i)).getTypeOfGrains());
	}
	return result;
}

bool Grains::matchingTypesOfGrains(const Grains& objectOfComparison) const
{
	std::vector<CombinerVariables::TypesOfGrains> firstSortedTypesOfGrains = std::vector<CombinerVariables::TypesOfGrains>(this->getTypesOfGrains());
	std::sort(firstSortedTypesOfGrains.begin(),firstSortedTypesOfGrains.end());
	std::vector<CombinerVariables::TypesOfGrains> secondSortedTypesOfGrains = std::vector<CombinerVariables::TypesOfGrains>(objectOfComparison.getTypesOfGrains());
	std::sort(secondSortedTypesOfGrains.begin(),secondSortedTypesOfGrains.end());

	if (firstSortedTypesOfGrains == secondSortedTypesOfGrains) {return (true);}
	else {return (false);}
}

void Grains::zeroFractions()
{
	for(std::vector<GrainType*>::iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		(*(*i)).zeroFractions();
	}
}

void Grains::zeroFractions(CombinerVariables::TypesOfGrains typeOfGrains)
{
	std::vector<GrainType*>::iterator target = Grains::getSingleGrainTypeIterator(typeOfGrains);
	(*(*target)).zeroFractions();
}

bool Grains::areFractionsZero() const
{
	bool result = true;
	for(std::vector<GrainType*>::const_iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		result *= (*(*i)).areFractionsZero();
	}
	return result;
}

bool Grains::areFractionsZero(CombinerVariables::TypesOfGrains typeOfGrains) const
{
	bool result;
	std::vector<GrainType*>::const_iterator target = Grains::getSingleGrainTypeConstIterator(typeOfGrains);
	result = (*(*target)).areFractionsZero();
	return result;
}

double Grains::getSingleFraction(CombinerVariables::TypesOfGrains typeOfGrains, int fraction) const
{
	return (*(this->getSingleGrainTypeConstIterator(typeOfGrains)))->getFraction(fraction);
}

void Grains::setSingleFraction(CombinerVariables::TypesOfGrains typeOfGrains, int fraction, double abundance)
{
	(*(*(this->getSingleGrainTypeIterator(typeOfGrains)))).setFraction(fraction, abundance);
}

GrainType* Grains::createPointerCopyOfSingleGrainType(CombinerVariables::TypesOfGrains typeOfGrains) const
{
	return (*(*(this->getSingleGrainTypeConstIterator(typeOfGrains)))).createGrainTypePointerCopy();
}

std::vector<GrainType*>::iterator Grains::getSingleGrainTypeIterator(CombinerVariables::TypesOfGrains typeOfGrains)
{
	std::vector<GrainType*>::iterator result = this->constitutingGrainTypes.end();
	bool alreadyFound = (false);

	for(std::vector<GrainType*>::iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfGrains() == typeOfGrains )
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

	if ( result == this->constitutingGrainTypes.end() )
	{
		const char *const errorMessage = "In this simulation, this type of grains is not used.";
		throw(errorMessage);
	}
	else { return result;}
}

std::vector<GrainType*>::const_iterator Grains::getSingleGrainTypeConstIterator(CombinerVariables::TypesOfGrains typeOfGrains) const
{
	std::vector<GrainType*>::const_iterator result = this->constitutingGrainTypes.end();
	bool alreadyFound = (false);

	for(std::vector<GrainType*>::const_iterator i = this->constitutingGrainTypes.begin(); i < this->constitutingGrainTypes.end(); ++i)
	{
		if( (*(*i)).getTypeOfGrains() == typeOfGrains )
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

	if ( result == this->constitutingGrainTypes.end() )
	{
		const char *const errorMessage = "In this simulation, this type of grains is not used.";
		throw(errorMessage);
	}
	else { return result;}
}

GrainType* Grains::getSingleGrainTypePointer(CombinerVariables::TypesOfGrains typeOfGrains)
{
	return *( getSingleGrainTypeIterator(typeOfGrains) );
}

const GrainType* Grains::getSingleGrainTypeConstPointer(CombinerVariables::TypesOfGrains typeOfGrains) const
{
	const GrainType* result = *( getSingleGrainTypeConstIterator(typeOfGrains) );
	return result;
}

std::vector<GrainType*>::iterator Grains::getBeginGrainTypeIterator()
{
	return this->constitutingGrainTypes.begin();
}

std::vector<GrainType*>::const_iterator Grains::getBeginGrainTypeConstIterator() const
{
	std::vector<GrainType*>::const_iterator result = this->constitutingGrainTypes.begin();
	return result;
}

std::vector<GrainType*>::iterator Grains::getEndGrainTypeIterator()
{
	return this->constitutingGrainTypes.end();
}

std::vector<GrainType*>::const_iterator Grains::getEndGrainTypeConstIterator() const
{
	std::vector<GrainType*>::const_iterator result = this->constitutingGrainTypes.end();
	return result;
}

void Grains::setSingleGrainType(CombinerVariables::TypesOfGrains typeOfGrains, const GrainType& newGrainType)
{
	*(*(this->getSingleGrainTypeIterator(typeOfGrains))) = newGrainType;
}

std::pair< CombinerVariables::TypesOfGrains , std::vector<double> > Grains::useTypeSpecificGetter(CombinerVariables::TypesOfGrains typeOfGrains) const
{
	return (*(this->getSingleGrainTypeConstIterator(typeOfGrains)))->typeSpecificGetter();
}

std::vector< std::vector<GrainType*> > Grains::displayStrataByGrainTypePointerFrontEnd(std::vector<Grains>& strata)
{
	if(strata.size() == 0 || (strata.at(0).getNumberOfGrainTypes()) == 0)
	{
		const char *const errorMessage = "Action on empty grains container (Grains::displayStrataByGrainTypePointerFrontEnd)";
		throw(errorMessage);
	}
	for(std::vector<Grains>::const_iterator i = strata.begin(); i < strata.end(); ++i)
	{
		if( strata.at(0).matchingTypesOfGrains(*i) == (false) )
		{
			const char *const errorMessage = "Action on not matching types of grains";
			throw(errorMessage);
		}
	}

	std::vector<GrainType*> tmp;
	std::vector< std::vector<GrainType*> > result = std::vector< std::vector<GrainType*> >(strata.at(0).getNumberOfGrainTypes(), tmp);
	std::vector< std::vector<GrainType*> >::iterator resultIt = result.begin();
	for(std::vector<GrainType*>::const_iterator i = strata.at(0).getBeginGrainTypeConstIterator(); i < strata.at(0).getEndGrainTypeConstIterator(); ++i, ++resultIt)
	{
		for(std::vector<Grains>::iterator j = strata.begin(); j < strata.end(); ++j)
		{
			(*resultIt).push_back((*j).getSingleGrainTypePointer((*(*i)).getTypeOfGrains()));
		}
	}
	return result;
}

std::vector< std::vector<const GrainType*> > Grains::displayStrataByConstGrainTypePointerFrontEnd(const std::vector<Grains>& strata)
{
	if(strata.size() == 0 || (strata.at(0).getNumberOfGrainTypes()) == 0)
	{
		const char *const errorMessage = "Action on empty grains container (Grains::displayStrataByConstGrainTypePointerFrontEnd)";
		throw(errorMessage);
	}
	for(std::vector<Grains>::const_iterator i = strata.begin(); i < strata.end(); ++i)
	{
		if( strata.at(0).matchingTypesOfGrains(*i) == (false) )
		{
			const char *const errorMessage = "Action on not matching types of grains";
			throw(errorMessage);
		}
	}

	std::vector<const GrainType*> tmp;
	std::vector< std::vector<const GrainType*> > result = std::vector< std::vector<const GrainType*> >(strata.at(0).getNumberOfGrainTypes(), tmp);
	std::vector< std::vector<const GrainType*> >::iterator resultIt = result.begin();
	for(std::vector<GrainType*>::const_iterator i = strata.at(0).getBeginGrainTypeConstIterator(); i < strata.at(0).getEndGrainTypeConstIterator(); ++i, ++resultIt)
	{
		for(std::vector<Grains>::const_iterator j = strata.begin(); j < strata.end(); ++j)
		{
			(*resultIt).push_back((*j).getSingleGrainTypeConstPointer((*(*i)).getTypeOfGrains()));
		}
	}
	return result;
}

/*
std::vector<Grains> Grains::combineToGrains(const std::vector< std::vector<GrainType*> >& strata)
{
	if(strata.size() == 0)
	{
		const char *const errorMessage = "Action on empty container";
		throw(errorMessage);
	}
	int numberOfLayers = strata.at(0).size();
	if(numberOfLayers == 0)
	{
		const char *const errorMessage = "Action on empty container";
		throw(errorMessage);
	}
	for(std::vector< std::vector<GrainType*> >::const_iterator i = strata.begin(); i < strata.end(); ++i)
	{
		if (numberOfLayers != (*i).size())
		{
			const char *const errorMessage = "Action on not matching number of layers";
			throw(errorMessage);
		}
	}

	std::vector<GrainType*> tmpVector;
	for(std::vector< std::vector<GrainType*> >::const_iterator i = strata.begin(); i < strata.end(); ++i){tmpVector.push_back((*i).at(0));}
	Grains tmpGrains = Grains(tmpVector);
	std::vector<Grains> result = std::vector<Grains>(numberOfLayers, tmpGrains);

	std::vector<CombinerVariables::TypesOfGrains> duplicateCheck;
	CombinerVariables::TypesOfGrains currentTypeOfGrains;
	int inputLayer = 1;
	for(std::vector<Grains>::iterator resultLayer = (result.begin() + 1); resultLayer < result.end(); ++resultLayer, ++inputLayer)
	{
		duplicateCheck.clear();
		for(std::vector< std::vector<GrainType*> >::const_iterator inputGrain = strata.begin(); inputGrain < strata.end(); ++inputGrain)
		{
			currentTypeOfGrains = (*(*inputGrain).at(inputLayer)).getTypeOfGrains();
			if (std::find(duplicateCheck.begin(), duplicateCheck.end(), currentTypeOfGrains) != duplicateCheck.end())
			{
				const char *const errorMessage = "Duplicate combiner variable";
				throw(errorMessage);
			}
			(*resultLayer).setSingleGrainType(currentTypeOfGrains, (*(*inputGrain).at(inputLayer)));
			duplicateCheck.push_back(currentTypeOfGrains);
		}
	}

	return result;
}
*/

Grains Grains::interpolateLinearly(const Grains& firstValue, const Grains& secondValue, double interpolationValue)
{
	Grains result;

	if(0.0 < interpolationValue && interpolationValue < 1.0)
	{
		if ( firstValue.matchingTypesOfGrains(secondValue) == (false) )
		{
			const char *const errorMessage = "Action on not matching number of grain types";
			throw(errorMessage);
		}

		std::vector<GrainType*> resultVector;
		const GrainType* firstValueGrainTypePointer;
		const GrainType* secondValueGrainTypePointer;
		for(std::vector<GrainType*>::const_iterator firstValueGrainTypeIterator = firstValue.constitutingGrainTypes.begin(); firstValueGrainTypeIterator < firstValue.constitutingGrainTypes.end(); ++firstValueGrainTypeIterator)
		{
			firstValueGrainTypePointer = *firstValueGrainTypeIterator;
			secondValueGrainTypePointer = secondValue.getSingleGrainTypeConstPointer((*(*firstValueGrainTypeIterator)).getTypeOfGrains());
			resultVector.push_back( GrainType::interpolateLinearly(firstValueGrainTypePointer,secondValueGrainTypePointer,interpolationValue) );
		}
		result = Grains(resultVector);

		while( ! (resultVector.empty()) )
		{
			delete resultVector.back();
			resultVector.pop_back();
		}

	} else
	{
		if(interpolationValue >= 1.0)
			{ result = secondValue; }
		else
			{ result = firstValue; }
	}
	return result;
}

}
