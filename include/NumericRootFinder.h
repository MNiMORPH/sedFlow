/*
 * NumericRootFinder.h
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

#ifndef NUMERICROOTFINDER_H_
#define NUMERICROOTFINDER_H_

#include "ConstructionVariables.h"

namespace SedFlow {

template <typename UnaryFunction> class NumericRootFinder {
public:
	NumericRootFinder(){}
	virtual ~NumericRootFinder(){}

	virtual NumericRootFinder<UnaryFunction>* createNumericRootFinderPointerCopy() const = 0; //This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables() const = 0;

	virtual double findRoot (const UnaryFunction& function, double firstBracket, double secondBracket, double errorTolerance, int maximumNumberOfIterations) const = 0;

};

}

#endif /* NUMERICROOTFINDER_H_ */
