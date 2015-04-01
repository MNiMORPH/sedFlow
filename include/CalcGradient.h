/*
 * CalcGradient.h
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

#ifndef CALCGRADIENT_H_
#define CALCGRADIENT_H_

#include <vector>

#include "RiverSystemProperties.h"
#include "CombinerVariables.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class CalcGradient {
protected:
	CombinerVariables::TypesOfGradientCalculationMethod typeOfGradientCalculationMethod;
	bool isDependentOnBedslope;
	bool isDependentOnWaterEnergyslope;

public:
	CalcGradient();
	virtual ~CalcGradient();

	virtual CalcGradient* createCalcGradientMethodPointerCopy() const = 0;//This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual double calculate (const RiverReachProperties& riverReachProperties) const = 0;
	virtual double calculate (const RiverSystemProperties& river, int cellID) const = 0;
	virtual std::vector<double> calculate (const RiverSystemProperties& river) const = 0;
	virtual double calculate (const std::vector<double>& levels, const std::vector<double>& lengths, int cellID) const = 0;
	virtual std::vector<double> calculate (const std::vector<double>& levels, const std::vector<double>& lengths) const = 0;

	virtual std::pair<double,double> calculateReducedAndUnreducedSedimentEnergyslope (const RiverReachProperties& riverReachProperties) const;
	virtual std::pair<double,double> calculateReducedAndUnreducedSedimentEnergyslope (const RiverSystemProperties& river, int cellID) const;
	virtual std::pair< std::vector<double>, std::vector<double> > calculateReducedAndUnreducedSedimentEnergyslope (const RiverSystemProperties& river) const;

	inline bool dependsOnBedslope() const { return isDependentOnBedslope; }
	inline bool dependsOnWaterEnergyslope() const { return isDependentOnWaterEnergyslope; }

	CombinerVariables::TypesOfGradientCalculationMethod getTypeOfGradientCalculationMethod() const;
	std::string getTypeOfGradientCalculationMethodAsString() const;

};

}

#endif /* CALCGRADIENT_H_ */
