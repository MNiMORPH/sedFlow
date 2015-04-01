/*
 * GrainType.h
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

#ifndef GRAINTYPE_H_
#define GRAINTYPE_H_

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdlib>

#include "CombinerVariables.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class GrainType {

protected:
	CombinerVariables::TypesOfGrains typeOfGrains;
	bool typeSpecificGetterAvailable;
	std::vector<double> fractionalAbundance; //Represents partial layer thickness. So the sum of this array is the thickness of corresponding layer.
	int numberOfFractions;

public:
	// In each implementation at least one of the following constructors should be implemented:
	//SpecificGrainType(const std::vector<double>& fractionalAbundance);
	//SpecificGrainType(const std::vector<double>& fractionalAbundance, const std::vector<double>& additionalParameters);
	GrainType(){}
	GrainType(std::vector<double> fractionalAbundance);
	virtual ~GrainType();

	virtual GrainType* createGrainTypePointerCopy() const = 0;//This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual void insert (std::vector<GrainType*>& strata) const;
	virtual GrainType* erode (std::vector<GrainType*>& strata) const; //The method erode tries to erode a certain amount of Grains the returned value is the amount that has actually been eroded.
	virtual GrainType* getPotentialErosion (const std::vector<const GrainType*>& strata) const; //The method "getPotentialErosion" returns the same value as "erode", but without actually eroding.
	virtual void deposit (std::vector<GrainType*>& strata) const;
	virtual void add (const GrainType& toAdd); //The argument is the amount to be added.
	virtual void add (const GrainType* toAdd); //The argument is the amount to be added.
	virtual GrainType* subtract (const GrainType* toSubtract); //The argument is the amount that is tried to be subtracted. The actually subtracted amount is returned.
	virtual GrainType* getPotentialSubtraction (const GrainType* toSubtract) const; //The method "getPotentialSubtraction" returns the same value as "subtract", but without actually subtracting.
	virtual int getNumberOfFractions() const;
	virtual void setFraction (int fraction, double value);
	virtual double getFraction(int fraction) const;
	virtual std::vector<double> getFractions() const;
	//virtual double* getFractionAdress(int fraction); //This method is for debugging only. Vectors may be difficult to observe while debugging. Sometimes the only way is by using pointers to the individual elements. For that purpose this method was introduced.
	virtual void zeroFractions();
	virtual bool areFractionsZero() const;
	inline std::string getTypeOfGrainsAsString() const { return CombinerVariables::typeOfGrainsToString(this->typeOfGrains); }
	inline CombinerVariables::TypesOfGrains getTypeOfGrains() const { return this->typeOfGrains; }
	virtual bool hasTypeSpecificGetter() const;

	virtual std::pair<CombinerVariables::TypesOfGrains , std::vector<double> > typeSpecificGetter() const; //Place-holder for getters of type specific variables. Default just throws an error.

	static GrainType* interpolateLinearly(const GrainType* firstValue, const GrainType* secondValue, double interpolationValue);

	static std::vector<GrainType*> copyGrainTypePointerVector(const std::vector<GrainType*>& singleGrainTypes);

	friend bool operator < (const GrainType& grainType1, const GrainType& grainType2)
	{
		return (grainType1.typeOfGrains < grainType2.typeOfGrains);
	}

	friend bool operator == (const GrainType& grainType1, const GrainType& grainType2)
	{
		return (grainType1.typeOfGrains == grainType2.typeOfGrains);
	}

	friend GrainType& operator *= (GrainType& grainType, double multiplicator)
	{
		if(multiplicator < 0)
		{
			const char *const errorMessage = "Creating negative fractional abundances";
			throw(errorMessage);
		}
		for(std::vector<double>::iterator currentFraction = grainType.fractionalAbundance.begin(); currentFraction < grainType.fractionalAbundance.end(); ++currentFraction)
					{ (*currentFraction) *= multiplicator; }
		return grainType;
	}
/*
	friend GrainType operator * (GrainType grainType, double multiplicator)
	{
		grainType *= multiplicator;
		return grainType;
	}

	friend GrainType operator * (double multiplicator, const GrainType& grainType)
	{
		return (grainType * multiplicator);
	}
*/
	friend GrainType& operator *= (GrainType& grainType, std::vector<double> multiplicators)
	{
		if(grainType.getNumberOfFractions() != multiplicators.size())
		{
			const char *const firstErrorMessage = "Action on not matching number of fractions";
			throw(firstErrorMessage);
		}

		for(std::vector<double>::iterator currentFraction = grainType.fractionalAbundance.begin(), currentMultiplicator = multiplicators.begin(); currentFraction < grainType.fractionalAbundance.end(); ++currentFraction, ++currentMultiplicator)
		{
			if((*currentMultiplicator) < 0)
			{
				const char *const secondErrorMessage = "Creating negative fractional abundances";
				throw(secondErrorMessage);
			}
			(*currentFraction) *= (*currentMultiplicator);
		}
		return grainType;
	}
/*
	friend GrainType operator * (GrainType grainType, std::vector<double> multiplicators)
	{
		grainType *= multiplicators;
		return grainType;
	}

	friend GrainType operator * (std::vector<double> multiplicators, const GrainType& grainType)
	{
		return (grainType * multiplicators);
	}
*/
	friend GrainType& operator /= (GrainType& grainType, std::vector<double> divisors)
	{
		for(std::vector<double>::iterator i = divisors.begin(); i < divisors.end(); ++i)
				{ (*i) = ( 1.0 / (*i) ); }
		grainType *= divisors;
		return grainType;
	}
/*
	friend GrainType operator / (GrainType grainType, std::vector<double> divisors)
	{
		grainType /= divisors;
		return grainType;
	}
*/
	friend GrainType& operator += (GrainType& grainType1, const GrainType& grainType2)
	{
		grainType1.add(grainType2);
		return grainType1;
	}
/*
	friend GrainType operator + (GrainType grainType1, const GrainType& grainType2)
	{
		grainType1 += grainType2;
		return grainType1;
	}
*/
};

}

#endif /* GRAINTYPE_H_ */
