/*
 * OverallMethods.h
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

#ifndef OVERALLMETHODS_H_
#define OVERALLMETHODS_H_

#include "CalcGradient.h"
#include "FlowResistance.h"
#include "CalcBedloadVelocity.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class OverallMethods {
public:
	OverallMethods();
	OverallMethods(const CalcGradient* bedslopeCalculationMethod,
			const CalcGradient* waterEnergyslopeCalculationMethod,
			const CalcGradient* sedimentEnergyslopeCalculationMethod,
			const FlowResistance* flowResistance,
			const CalcBedloadVelocity* bedloadVelocityCalculationMethod);
	virtual ~OverallMethods();

	ConstructionVariables createConstructionVariables()const;

	CalcGradient* bedSlopeCalculationMethod;
	CalcGradient* waterEnergySlopeCalculationMethod;
	CalcGradient* sedimentEnergySlopeCalculationMethod;
	FlowResistance* flowResistance;
	CalcBedloadVelocity* bedloadVelocityCalculationMethod;
};

}

#endif /* OVERALLMETHODS_H_ */
