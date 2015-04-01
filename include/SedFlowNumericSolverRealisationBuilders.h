/*
 * SedFlowNumericSolverRealisationBuilders.h
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

#ifndef SEDFLOWNUMERICSOLVERREALISATIONBUILDERS_H_
#define SEDFLOWNUMERICSOLVERREALISATIONBUILDERS_H_

#include "NumericRootFinder.h"

#include <iostream>

//Types of Numeric Root Finder
#include "BisectionMethod.h"
#include "SecantMethod.h"
#include "FalsePositionMethod.h"
#include "RiddersMethod.h"
#include "BrentMethod.h"

namespace SedFlow {

template <typename UnaryFunction> class SedFlowNumericSolverRealisationBuilders {
public:
	SedFlowNumericSolverRealisationBuilders(){}
	virtual ~SedFlowNumericSolverRealisationBuilders(){}

	NumericRootFinder<UnaryFunction>* numericRootFinderBuilder(CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder)
	{
		NumericRootFinder<UnaryFunction>* result;

		switch (typeOfNumericRootFinder)
		{
		case CombinerVariables::BisectionMethod:
			result = new BisectionMethod<UnaryFunction>();
			break;

		case CombinerVariables::SecantMethod:
			result = new SecantMethod<UnaryFunction>();
			break;

		case CombinerVariables::FalsePositionMethod:
			result = new FalsePositionMethod<UnaryFunction>();
			break;

		case CombinerVariables::RiddersMethod:
			result = new RiddersMethod<UnaryFunction>();
			break;

		case CombinerVariables::BrentMethod:
			result = new BrentMethod<UnaryFunction>();
			break;

		default:
			const char *const invalidTypeErrorMessage = "Invalid Numeric Root Finder Type";
			throw (invalidTypeErrorMessage);
		}

		return result;
	}


};

}

#endif /* SEDFLOWNUMERICSOLVERREALISATIONBUILDERS_H_ */
