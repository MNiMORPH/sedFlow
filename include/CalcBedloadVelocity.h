/*
 * CalcBedloadVelocity.h
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

#ifndef CALCBEDLOADVELOCITY_H_
#define CALCBEDLOADVELOCITY_H_

#include "RiverReachProperties.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class CalcBedloadVelocity {
public:
	CalcBedloadVelocity();
	virtual ~CalcBedloadVelocity();

	virtual CalcBedloadVelocity* createCalcBedloadVelocityPointerCopy() const = 0; //This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual double calculate (const RiverReachProperties& riverReachProperties) const = 0;
};

}

#endif /* CALCBEDLOADVELOCITY_H_ */
