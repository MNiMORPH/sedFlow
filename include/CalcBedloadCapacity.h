/*
 * CalcBedload.h
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

#ifndef CALCBEDLOADCAPACITY_H_
#define CALCBEDLOADCAPACITY_H_

#include "RiverReachProperties.h"
#include "Grains.h"
#include "ConstructionVariables.h"
#include "OverallMethods.h"

namespace SedFlow {

class CalcBedloadCapacity {
public:
	virtual ~CalcBedloadCapacity();

	virtual CalcBedloadCapacity* createCalcBedloadCapacityMethodPointerCopy() const = 0;//This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual Grains calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const = 0;
};

}

#endif /* CALCBEDLOAD_H_ */
