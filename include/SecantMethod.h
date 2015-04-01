/*
 * SecantMethod.h
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

#ifndef SECANTMETHOD_H_
#define SECANTMETHOD_H_

#include "NumericRootFinder.h"

#include <iostream>
#include <math.h>
#include <limits>

namespace SedFlow {

template <typename UnaryFunction> class SecantMethod: public NumericRootFinder<UnaryFunction> {
public:
	SecantMethod(){}
	virtual ~SecantMethod(){}

	NumericRootFinder<UnaryFunction>* createNumericRootFinderPointerCopy() const //This method HAS TO BE implemented.
	{
		NumericRootFinder<UnaryFunction>* result = new SecantMethod<UnaryFunction>();
		return result;
	}

	ConstructionVariables createConstructionVariables()const
	{
		ConstructionVariables result = ConstructionVariables();
		result.interfaceOrCombinerType = CombinerVariables::NumericRootFinder;
		result.realisationType = CombinerVariables::typeOfNumericRootFinderToString(CombinerVariables::SecantMethod);
		return result;
	}

	double findRoot (const UnaryFunction& function, double firstBracket, double secondBracket, double errorTolerance, int maximumNumberOfIterations) const
	{
		double fFirst = function(firstBracket);
		double currentGuess = secondBracket;
		double fCurrentGuess = function(secondBracket);

		if( (fFirst * fCurrentGuess) > 0.0 )
		{
			const char *const bracketingErrorMessage = "SecantMethod: Root must be bracketed by the input brackets.";
			throw(bracketingErrorMessage);
		}

		if( fabs(fCurrentGuess) > fabs(fFirst) ) //fCurrentGuess should be closer to 0.0
		{
			double tmp = firstBracket;
			firstBracket = currentGuess;
			currentGuess = tmp;

			tmp = fFirst;
			fFirst = fCurrentGuess;
			fCurrentGuess = tmp;
		}

		double delta;

		for(int i = 1; i <= maximumNumberOfIterations; ++i)
		{
			delta = (firstBracket - currentGuess) * (fCurrentGuess / (fCurrentGuess - fFirst));
			firstBracket = currentGuess;
			fFirst = fCurrentGuess;
			currentGuess += delta;
			fCurrentGuess = function(currentGuess);
			if( fCurrentGuess == 0.0 || fabs(delta) <= errorTolerance ){return currentGuess;}
		}

		const char *const maxIterationErrorMessage = "SecantMethod: Maximum number of iterations exceeded.";
		throw(maxIterationErrorMessage);
		return std::numeric_limits<double>::signaling_NaN();
	}

};

}

#endif /* SECANTMETHOD_H_ */
