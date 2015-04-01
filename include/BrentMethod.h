/*
 * BrentMethod.h
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

#ifndef BRENTMETHOD_H_
#define BRENTMETHOD_H_

#include "NumericRootFinder.h"

#include <iostream>
#include <math.h>
#include <limits>

namespace SedFlow {

template <typename UnaryFunction> class BrentMethod: public NumericRootFinder<UnaryFunction> {
public:
	BrentMethod(){}
	virtual ~BrentMethod(){}

	NumericRootFinder<UnaryFunction>* createNumericRootFinderPointerCopy() const //This method HAS TO BE implemented.
	{
		NumericRootFinder<UnaryFunction>* result = new BrentMethod<UnaryFunction>();
		return result;
	}

	ConstructionVariables createConstructionVariables()const
	{
		ConstructionVariables result = ConstructionVariables();
		result.interfaceOrCombinerType = CombinerVariables::NumericRootFinder;
		result.realisationType = CombinerVariables::typeOfNumericRootFinderToString(CombinerVariables::BrentMethod);
		return result;
	}

	double findRoot (const UnaryFunction& function, double firstBracket, double secondBracket, double errorTolerance, int maximumNumberOfIterations) const
	{
		double a = firstBracket;
		double b = secondBracket;
		double fa = function(a);
		double fb = function(b);
		double c = b;
		double fc = fb;
		double d,e;

		if( (fa * fb) > 0.0 )
		{
			const char *const bracketingErrorMessage = "BrentMethod: Root must be bracketed by the input brackets.";
			throw(bracketingErrorMessage);
		}

		bool bisectionMethodUsedInLastIteration = true;

		for(int i = 1; i <= maximumNumberOfIterations; ++i)
		{
			//TODO Implement method
			if( fb == 0.0 || fabs((a-b)) <= errorTolerance ){return b;}
		}

		const char *const maxIterationErrorMessage = "BrentMethod: Maximum number of iterations exceeded.";
		throw(maxIterationErrorMessage);
		return std::numeric_limits<double>::signaling_NaN();
	}


};

}

#endif /* BRENTMETHOD_H_ */
