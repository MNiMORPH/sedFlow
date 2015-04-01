/*
 * ReturnBedslope.h
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

#ifndef RETURNBEDSLOPE_H_
#define RETURNBEDSLOPE_H_

#include "CalcGradient.h"

#include <vector>

#include "RiverSystemProperties.h"

namespace SedFlow {

class ReturnBedslope: public CalcGradient {
private:
	double minimumSlope;

public:
	ReturnBedslope();
	ReturnBedslope(double minimumSlope);
	virtual ~ReturnBedslope(){}

	CalcGradient* createCalcGradientMethodPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	double calculate (const RiverReachProperties& riverReachProperties) const;
	double calculate (const RiverSystemProperties& river, int cellID) const;
	std::vector<double> calculate (const RiverSystemProperties& river) const;
	double calculate (const std::vector<double>& levels, const std::vector<double>& lengths, int cellID) const;
	std::vector<double> calculate (const std::vector<double>& levels, const std::vector<double>& lengths) const;

};

}

#endif /* RETURNBEDSLOPE_H_ */
