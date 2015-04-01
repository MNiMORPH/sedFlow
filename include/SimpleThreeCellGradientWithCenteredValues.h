/*
 * SimpleThreeCellGradientWithCenteredValues.h
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

#ifndef SIMPLETHREECELLGRADIENTWITHCENTEREDVALUES_H_
#define SIMPLETHREECELLGRADIENTWITHCENTEREDVALUES_H_

#include <vector>

#include "CalcGradient.h"

#include "RegularRiverSystemMethods.h"

namespace SedFlow {

class SimpleThreeCellGradientWithCenteredValues: public CalcGradient {
private:
	CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest;
	CombinerVariables::TypesOfRegularRiverReachProperties weightingProperty;

public:
	SimpleThreeCellGradientWithCenteredValues(CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest, CombinerVariables::TypesOfRegularRiverReachProperties weightingProperty);
	virtual ~SimpleThreeCellGradientWithCenteredValues(){}

	CalcGradient* createCalcGradientMethodPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	double calculate (double upstreamValue, double downstreamValue, double upstreamLength, double localLength, double downstreamLength) const;

	double calculate (const RiverReachProperties& riverReachProperties) const;
	inline double calculate (const RiverSystemProperties& riverSystem, int cellID) const { return this->calculate(riverSystem.regularRiverSystemProperties.cellProperties.at(cellID)); }
	std::vector<double> calculate (const RiverSystemProperties& riverSystem) const;
	double calculate (const std::vector<double>& values, const std::vector<double>& lengths, int cellID) const;
	std::vector<double> calculate (const std::vector<double>& values, const std::vector<double>& lengths) const;

};

}

#endif /* SIMPLETHREECELLGRADIENTWITHCENTEREDVALUES_H_ */
