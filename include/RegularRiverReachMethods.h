/*
 * RegularRiverReachMethods.h
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

#ifndef REGULARRIVERREACHMETHODS_H_
#define REGULARRIVERREACHMETHODS_H_

#include "RiverReachProperties.h"
#include "Grains.h"
#include "CalcTau.h"
#include "CalcActiveWidth.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class RegularRiverReachMethods {
private:
	const CalcTau* tauCalculationMethod;
	const CalcActiveWidth* activeWidthCalculationMethod;

public:
	RegularRiverReachMethods();
	RegularRiverReachMethods(const CalcTau *tauCalculationMethod, const CalcActiveWidth* activeWidthCalculationMethod);
	RegularRiverReachMethods(const RegularRiverReachMethods& toCopy);
	virtual ~RegularRiverReachMethods();

	ConstructionVariables createConstructionVariables()const;

	void calculateFlowVelocityAndWaterdepth(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const;
	double getCurrentDarcyWeisbachFrictionFactorF(const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const;
	void calculateTau(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const;
	void calculateActiveWidth(RiverReachProperties& riverReachProperties, const SedimentFlowMethods& sedimentFlowMethods, const OverallMethods& overallMethods) const;

	const CalcTau* getTauCalculationMethod() const { return this->tauCalculationMethod; }
	const CalcActiveWidth* getActiveWidthCalculationMethod() const { return this->activeWidthCalculationMethod; }

	RegularRiverReachMethods& operator = (const RegularRiverReachMethods& toBeAssigned)
	{
		if (this != &toBeAssigned) {
			delete this->tauCalculationMethod;
			delete this->activeWidthCalculationMethod;

			this->tauCalculationMethod = toBeAssigned.tauCalculationMethod->createCalcTauPointerCopy();
			this->activeWidthCalculationMethod = toBeAssigned.activeWidthCalculationMethod->createCalcActiveWidthPointerCopy();
		}

		return *this;
	}
};

}

#endif /* REGULARRIVERREACHMETHODS_H_ */
