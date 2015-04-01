/*
 * GrainType.cpp
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
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#include "GrainType.h"
#include "CombinerVariables.h"



namespace SedFlow {

// For the method static GrainType* GrainType::createGrainTypePointerCopy(const GrainType* toCopy) please see GrainTypePointerCopier.h

GrainType::GrainType(std::vector<double> fractionalAbundance):
	fractionalAbundance(fractionalAbundance),
	numberOfFractions(fractionalAbundance.size())
{}

GrainType::~GrainType() {}

void GrainType::insert(std::vector<GrainType*> &strata) const
{
	if (*this == *(strata.at(0)))
	{
		GrainType* copyOfThisPointer = this->createGrainTypePointerCopy();
		(*(strata.at(0))).add(copyOfThisPointer);
		delete copyOfThisPointer;
	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}



GrainType* GrainType::erode(std::vector<GrainType*>& strata) const
{
	if (*this == *(strata.at(0)))
	{
		GrainType* result;
		GrainType* copyOfThisPointer = this->createGrainTypePointerCopy();
		result = (*(strata.at(0))).subtract(copyOfThisPointer);
		delete copyOfThisPointer;
		return result;
	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}



GrainType* GrainType::getPotentialErosion(const std::vector<const GrainType*>& strata) const
{
	if (*this == *(strata.at(0)))
	{
		GrainType* result;
		GrainType* copyOfThisPointer = this->createGrainTypePointerCopy();
		result = (*(strata.at(0))).getPotentialSubtraction(copyOfThisPointer);
		delete copyOfThisPointer;
		return result;
	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}



void GrainType::deposit(std::vector<GrainType*> &strata) const
{
	if (*this == *(strata.at(0)))
	{
		(*(strata.at(0))).add(*this);
	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}

void GrainType::add(const GrainType& toAdd)
{
	this->add(&toAdd);
}

void GrainType::add(const GrainType* toAdd)
{
	if (*this == *toAdd)
	{
		if ( this->getNumberOfFractions() == (*toAdd).getNumberOfFractions() )
		{
			std::vector<double> toAddFractions = toAdd->getFractions();
			std::transform(this->fractionalAbundance.begin(),this->fractionalAbundance.end(),toAddFractions.begin(),this->fractionalAbundance.begin(), std::plus<double>() );

		} else
		{
			const char *const errorMessage = "Operation on not matching number of fractions";
			throw(errorMessage);
		}

	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}



GrainType* GrainType::subtract(const GrainType* toSubtract)
{
	GrainType* actuallyToSubtract = this->getPotentialSubtraction(toSubtract);
	std::vector<double> actuallyToSubtractFractions = actuallyToSubtract->getFractions();
	std::transform(this->fractionalAbundance.begin(),this->fractionalAbundance.end(),actuallyToSubtractFractions.begin(),this->fractionalAbundance.begin(), std::minus<double>() );
	return actuallyToSubtract;
}

GrainType* GrainType::getPotentialSubtraction(const GrainType* toSubtract) const
{
	if (*this == *toSubtract)
	{
		if ( this->getNumberOfFractions() == (*toSubtract).getNumberOfFractions() )
		{
			GrainType* copyOfToSubtract = toSubtract->createGrainTypePointerCopy();

			for(int i=0; i<(*copyOfToSubtract).getNumberOfFractions(); ++i)
			{
				if ((*copyOfToSubtract).getFraction(i) > this->getFraction(i) ) { (*copyOfToSubtract).setFraction(i, this->getFraction(i)); }
			}
			return copyOfToSubtract;

		} else
		{
			const char *const errorMessage = "Operation on not matching number of fractions";
			throw(errorMessage);
		}

	} else
	{
		const char *const errorMessage = "Operation on not matching grain types";
		throw(errorMessage);
	}
}

int GrainType::getNumberOfFractions() const
{
#if defined _DEBUG || defined DEBUG
	if(numberOfFractions != fractionalAbundance.size())
	{
		const char *const errorMessage = "Altered number of Fractions.";
		throw(errorMessage);
	}
#endif
	return numberOfFractions;
}

void GrainType::setFraction (int fraction, double value)
{
	if ((fraction >= 0) && (fraction < this->getNumberOfFractions()))
		{
		if (value<0.0)
		{
			const char *const errorMessage = "Trying to set negative grain fraction";
			throw(errorMessage);
		}
		else {this->fractionalAbundance.at(fraction) = value;}
		}
	else
	{
		const char *const errorMessage = "Trying to set non existing fraction";
		throw(errorMessage);
	}
}

double GrainType::getFraction (int fraction) const
{
#if defined _DEBUG || defined DEBUG
	if ((fraction >= 0) && (fraction < this->getNumberOfFractions() ) )
	{
		if (fractionalAbundance.at(fraction) < 0.0)
		{
			const char *const errorMessage = "Negative fractional abundance";
			throw(errorMessage);
		}
	}
	else
	{
		const char *const errorMessage = "Trying to get non existing fraction";
		throw(errorMessage);
	}
#endif
	return fractionalAbundance[fraction];
}


std::vector<double> GrainType::getFractions() const
{
	return fractionalAbundance;
}

/*//This method is for debugging only. Vectors may be difficult to observe while debugging.
 // Sometimes the only way is by using pointers to the individual elements. For this reason the following method was introduced.
double* GrainType::getFractionAdress (int fraction)
{
	if ((fraction >= 0) && (fraction < this->getNumberOfFractions() ) )
		{
		return &fractionalAbundance.at(fraction);
		}
	else
	{
		const char *const errorMessage = "Trying to get non existing fraction";
		throw(errorMessage);
	}
}
*/

void GrainType::zeroFractions()
{
	for(std::vector<double>::iterator it = this->fractionalAbundance.begin() ; it < this->fractionalAbundance.end(); ++it)
	{
		*it = 0.0;
	}
}

bool GrainType::areFractionsZero() const
{
	for(std::vector<double>::const_iterator it = this->fractionalAbundance.begin() ; it < this->fractionalAbundance.end(); ++it)
	{
		if (*it != 0.0) {return (false);}
	}
	return (true);
}

bool GrainType::hasTypeSpecificGetter() const
{
	return this->typeSpecificGetterAvailable;
}

std::pair<CombinerVariables::TypesOfGrains , std::vector<double> > GrainType::typeSpecificGetter() const
{
	const char *const errorMessage = "Trying to use undefined type-specific Function";
	throw(errorMessage);
	std::vector<double> emptyDoubleVector;
	std::pair<CombinerVariables::TypesOfGrains , std::vector<double> > fakeResult = std::make_pair(this->typeOfGrains,emptyDoubleVector);
	return fakeResult;
}

GrainType* GrainType::interpolateLinearly(const GrainType* firstValue, const GrainType* secondValue, double interpolationValue)
{
	GrainType* result;
	if(0.0 < interpolationValue && interpolationValue < 1.0)
	{
		result = firstValue->createGrainTypePointerCopy();
		if (*firstValue == *secondValue)
		{
			if ( firstValue->getNumberOfFractions() == secondValue->getNumberOfFractions() )
			{
				for(int i=0; i < firstValue->getNumberOfFractions(); ++i)
				{
					result->fractionalAbundance.at(i) = firstValue->getFraction(i) + ( interpolationValue * ( secondValue->getFraction(i) - firstValue->getFraction(i) ) );
				}

			} else
			{
				const char *const errorMessage = "Operation on not matching number of fractions";
				throw(errorMessage);
			}

		} else
		{
			const char *const errorMessage = "Operation on not matching grain types";
			throw(errorMessage);
		}
	}
	else
	{
		if(interpolationValue >= 1.0)
			{ result = secondValue->createGrainTypePointerCopy(); }
		else
			{ result = firstValue->createGrainTypePointerCopy(); }
	}

	return result;
}

std::vector<GrainType*> GrainType::copyGrainTypePointerVector(const std::vector<GrainType*>& singleGrainTypes)
{
	std::vector<GrainType*> result;
	result.reserve(singleGrainTypes.size());

	for(std::vector<GrainType*>::const_iterator i = singleGrainTypes.begin(); i < singleGrainTypes.end(); ++i)
	{
		result.push_back((*(*i)).createGrainTypePointerCopy());
	}

	return result;
}

}
