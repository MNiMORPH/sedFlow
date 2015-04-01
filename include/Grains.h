/*
 * Grains.h
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

// REMARK: Grains always represent volume. For a conversion to height Grains have to be divided by area.

#ifndef GRAINS_H_
#define GRAINS_H_

#include <vector>
#include <string>
#include <utility>

#include "GrainType.h"
#include "CombinerVariables.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class Grains {

private:
	std::vector<GrainType*> constitutingGrainTypes;

	std::vector<GrainType*>::iterator getSingleGrainTypeIterator(CombinerVariables::TypesOfGrains typeOfGrains);
	std::vector<GrainType*>::const_iterator getSingleGrainTypeConstIterator(CombinerVariables::TypesOfGrains typeOfGrains) const;
	GrainType* getSingleGrainTypePointer(CombinerVariables::TypesOfGrains typeOfGrains);
	std::vector<GrainType*>::iterator getBeginGrainTypeIterator();
	std::vector<GrainType*>::iterator getEndGrainTypeIterator();
	void setSingleGrainType(CombinerVariables::TypesOfGrains typeOfGrains, const GrainType& newGrainType);
	static std::vector< std::vector<GrainType*> > displayStrataByGrainTypePointerFrontEnd(std::vector<Grains>& strata);
	static std::vector< std::vector<const GrainType*> > displayStrataByConstGrainTypePointerFrontEnd(const std::vector<Grains>& strata);
	//static std::vector<Grains> combineToGrains(const std::vector< std::vector<GrainType*> >& strata); //This method is implemented but not used.
	static std::vector<GrainType*> forConstructorsCheckAndCopySingleGrainTypes (const std::vector<GrainType*>& singleGrainTypes);
	void naCheck() const;

public:
	//TODO Less Important: Check whether the following constructors are really necessary
	/*
	Grains(const std::vector<CombinerVariables::TypesOfGrains>& typesOfGrains, const std::vector< std::vector<double> >& fractionalAbundances);
	Grains(const std::vector<CombinerVariables::TypesOfGrains>& typesOfGrains, const std::vector< std::vector<double> >& fractionalAbundances, const std::vector< std::vector<double> >& additionalInputParameters);
	*/
	Grains(){} //This default constructor just creates an empty Grains object, which cannot be filled afterwards by public methods.
	Grains(const std::vector<GrainType*>& singleGrainTypes);
	Grains(const Grains& toCopy);
	virtual ~Grains();

	ConstructionVariables createConstructionVariables()const;

	std::vector<GrainType*>::const_iterator getEndGrainTypeConstIterator() const;
	std::vector<GrainType*>::const_iterator getBeginGrainTypeConstIterator() const;


	void insert (std::vector<Grains>& strata) const;
	Grains erode (std::vector<Grains>& strata) const; //The method "erode" tries to erode a certain amount of Grains the returned value is the amount that has actually been eroded.
	Grains getPotentialErosion (const std::vector<Grains>& strata) const; //The method "getPotentialErosion" returns the same value as "erode", but without actually eroding.
	void deposit (std::vector<Grains>& strata) const;
	void add (const Grains& toAdd); //The argument is the amount to be added.
	Grains subtract (const Grains& toSubtract); //The argument is the amount to be subtracted.
	Grains getPotentialSubtraction (const Grains& toSubtract) const; //The method "getPotentialSubtraction" returns the same value as "subtract", but without actually subtracting.
	static Grains distributeFractionalAbundance(const std::vector<double>& fractionalAbundance, Grains distributionBase);

	double getPercentileGrainDiameter(const std::vector<double>& fractionalGrainDiameters, double percentileRank) const;
	static double getPercentileGrainDiameter(const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, double percentileRank);
	double getArithmeticMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters) const;
	static double getArithmenticMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters, std::vector<double> fractionalAbundances);
	double getGeometricMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters) const;
	static double getGeometricMeanGrainDiameter(const std::vector<double>& fractionalGrainDiameters, std::vector<double> fractionalAbundances);

	int getNumberOfGrainTypes() const;
	int getNumberOfFractions() const;
	std::vector<double> getOverallFractionalAbundance() const;
	std::vector<double> getOverallCumulativeFractionalAbundance() const;
	static std::vector<double> getOverallCumulativeFractionalAbundance(const std::vector<double>& fractionalAbundances);
	double getOverallVolume() const;
	double getOverallVolumeWithinDiameterRange(const std::vector<double>& fractionalGrainDiameters, double firstDiameterLimit, double secondDiameterLimit) const;
	std::vector<std::string> getTypesOfGrainsAsString() const;
	std::vector<CombinerVariables::TypesOfGrains> getTypesOfGrains() const;
	bool matchingTypesOfGrains(const Grains& objectOfComparison) const;
	void zeroFractions();
	void zeroFractions(CombinerVariables::TypesOfGrains typeOfGrains);
	bool areFractionsZero() const;
	bool areFractionsZero(CombinerVariables::TypesOfGrains typeOfGrains) const;
	double getSingleFraction(CombinerVariables::TypesOfGrains typeOfGrains, int fraction) const;
	void setSingleFraction(CombinerVariables::TypesOfGrains typeOfGrains, int fraction, double abundance);
	GrainType* createPointerCopyOfSingleGrainType(CombinerVariables::TypesOfGrains typeOfGrains) const;
	std::pair< CombinerVariables::TypesOfGrains , std::vector<double> > useTypeSpecificGetter(CombinerVariables::TypesOfGrains typeOfGrains) const;

	static Grains interpolateLinearly(const Grains& firstValue, const Grains& secondValue, double interpolationValue);

	const GrainType* getSingleGrainTypeConstPointer(CombinerVariables::TypesOfGrains typeOfGrains) const;

	Grains& operator = (const Grains& newGrains)
	{
		if (this != &newGrains) {
			while(!(this->constitutingGrainTypes.empty()))
			{
				delete this->constitutingGrainTypes.back();
				this->constitutingGrainTypes.pop_back();
			}

			this->constitutingGrainTypes = Grains::forConstructorsCheckAndCopySingleGrainTypes( newGrains.constitutingGrainTypes );
		    }
			#if defined _DEBUG || defined DEBUG
				this->naCheck();
			#endif
		    return *this;
	}

	friend Grains operator *= (Grains& grains, double multiplicator)
	{
		for(std::vector<GrainType*>::iterator currentGrainType = grains.constitutingGrainTypes.begin(); currentGrainType < grains.constitutingGrainTypes.end(); ++currentGrainType)
				{ (*(*currentGrainType)) *= multiplicator; }
		return grains;
	}

	friend Grains operator * (Grains grains, double multiplicator)
	{
		grains *= multiplicator;
		return grains;
	}

	friend Grains operator * (double multiplicator, const Grains& grains)
	{
		return (grains * multiplicator);
	}

	friend Grains operator /= (Grains& grains, double divisor)
	{
		grains *= (1.0/divisor);
		return grains;
	}

	friend Grains operator / (Grains grains, double divisor)
	{
		grains /= divisor;
		return grains;
	}

	friend Grains operator *= (Grains& grains, std::vector<double> multiplicators)
	{
		for(std::vector<GrainType*>::iterator currentGrainType = grains.constitutingGrainTypes.begin(); currentGrainType < grains.constitutingGrainTypes.end(); ++currentGrainType)
				{ (*(*currentGrainType)) *= multiplicators; }
		return grains;
	}

	friend Grains operator * (Grains grains, std::vector<double> multiplicators)
	{
		grains *= multiplicators;
		return grains;
	}

	friend Grains operator * (std::vector<double> multiplicators, const Grains& grains)
	{
		return (grains * multiplicators);
	}

	friend Grains operator /= (Grains& grains, std::vector<double> divisors)
	{
		for(std::vector<double>::iterator i = divisors.begin(); i < divisors.end(); ++i)
				{ (*i) = ( 1.0 / (*i) ); }
		grains *= divisors;
		return grains;
	}

	friend Grains operator / (Grains grains, std::vector<double> divisors)
	{
		grains /= divisors;
		return grains;
	}

	friend Grains operator += (Grains& grains1, const Grains& grains2)
	{
		grains1.add(grains2);
		return grains1;
	}

	friend Grains operator + (Grains grains1, const Grains& grains2)
	{
		grains1 += grains2;
		return grains1;
	}

};

}

#endif /* GRAINS_H_ */
