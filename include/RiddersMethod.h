/*
 * RiddersMethod.h
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

#ifndef RIDDERSMETHOD_H_
#define RIDDERSMETHOD_H_

#include "NumericRootFinder.h"

#include <iostream>
#include <math.h>
#include <limits>

namespace SedFlow {

template <typename UnaryFunction> class RiddersMethod: public NumericRootFinder<UnaryFunction> {
private:
	int signum (int value) const { return (0 < value) - (value < 0 ); }
	double signum (double value) const { return static_cast<double>((0.0 < value) - (value < 0.0)); }

public:
	RiddersMethod(){}
	virtual ~RiddersMethod(){}

	NumericRootFinder<UnaryFunction>* createNumericRootFinderPointerCopy() const //This method HAS TO BE implemented.
	{
		NumericRootFinder<UnaryFunction>* result = new RiddersMethod<UnaryFunction>();
		return result;
	}

	ConstructionVariables createConstructionVariables()const
	{
		ConstructionVariables result = ConstructionVariables();
		result.interfaceOrCombinerType = CombinerVariables::NumericRootFinder;
		result.realisationType = CombinerVariables::typeOfNumericRootFinderToString(CombinerVariables::RiddersMethod);
		return result;
	}

	double findRoot (const UnaryFunction& function, double firstBracket, double secondBracket, double errorTolerance, int maximumNumberOfIterations) const
	{
		double fFirst = function(firstBracket);
		double fSecond = function(secondBracket);

		if( (fFirst * fSecond) >= 0.0 )
		{
			if( (fFirst * fSecond) > 0.0 )
			{
				const char *const bracketingErrorMessage = "RiddersMethod: Root must be bracketed by the input brackets.";
				throw(bracketingErrorMessage);
			}
			else
			{
				if( fFirst == 0.0 ){return firstBracket;}
				if( fSecond == 0.0 ){return secondBracket;}
			}
		}

		double mid, fMid, newGuess, fGuess, rootExpression;
		double lastGuess = std::numeric_limits<double>::quiet_NaN();

		for(int i = 1; i <= maximumNumberOfIterations; ++i)
		{
			mid = 0.5 * (firstBracket + secondBracket);
			fMid = function(mid);
			rootExpression = sqrt( ((fMid * fMid) - (fFirst * fSecond)) );

			//if( rootExpression == 0.0 ){ return lastGuess; } //This if is only true if the first AND the second product equals zero.
			if( rootExpression == 0.0 ){ return mid; }         //Thus both the lastGuess and mid are potential results.

			newGuess = mid + ( (mid-firstBracket) * signum((fFirst-fSecond)) * fMid / rootExpression );

			//if( fabs( (newGuess-lastGuess) ) <= errorTolerance ){ return lastGuess; } //If the if-statement is true, both the newGuess as well as the lastGuess close enough to the real solution.
			if( fabs( (newGuess-lastGuess) ) <= errorTolerance ){ return newGuess; }    //Thus both the lastGuess and newGuess are potential results.

			lastGuess = newGuess;
			fGuess = function(lastGuess);
			if( fGuess == 0.0 ){ return lastGuess; }

			if( (fMid * fGuess) < 0.0)
			{
				firstBracket = mid;
				fFirst = fMid;
				secondBracket = lastGuess;
				fSecond = fGuess;
			}
			else
			{
				if( (fFirst * fGuess) > 0.0 )
				{
					firstBracket = lastGuess;
					fFirst = fGuess;
				}
				else
				{
					if( (fSecond * fGuess) > 0.0 )
					{
						secondBracket = lastGuess;
						fSecond = fGuess;
					}
					else
					{
						const char *const unreachableErrorMessage = "RiddersMethod: This point should never be reached.";
						throw(unreachableErrorMessage);
						return std::numeric_limits<double>::signaling_NaN();
					}
				}
			}

			if( fabs( (firstBracket-secondBracket) ) <= errorTolerance ){ return lastGuess; }
		}

		const char *const maxIterationErrorMessage = "RiddersMethod: Maximum number of iterations exceeded.";
		throw(maxIterationErrorMessage);
		return std::numeric_limits<double>::signaling_NaN();
	}

};

}

#endif /* RIDDERSMETHOD_H_ */
