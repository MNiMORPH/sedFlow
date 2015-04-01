/*
 * ChengBedloadCapacity.h
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

#ifndef CHENGBEDLOADCAPACITY_H_
#define CHENGBEDLOADCAPACITY_H_

#include "CalcBedloadCapacity.h"

#include "CalcThresholdForInitiationOfBedloadMotion.h"
#include "CalcHidingFactors.h"

//This method is based on equation 7 of:
// Nian-Sheng Cheng (2002): Exponential Formula for Bedload Transport. Journal of Hydraulic Engineering, 128(10), 942-946.

namespace SedFlow {

class ChengBedloadCapacity: public CalcBedloadCapacity {
private:
	const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod;
	const CalcHidingFactors* hidingFactorsCalculationMethod;
	double factor; // Default value from the article is 13.
	bool thetaCriticalBasedOnConstantSred;

public:
	ChengBedloadCapacity(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, const CalcHidingFactors* hidingFactorsCalculationMethod, bool thetaCriticalBasedOnConstantSred, double factor);
	ChengBedloadCapacity(const CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod, const CalcHidingFactors* hidingFactorsCalculationMethod, bool thetaCriticalBasedOnConstantSred);
	virtual ~ChengBedloadCapacity();

	CalcBedloadCapacity* createCalcBedloadCapacityMethodPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	Grains calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const;
};

}

#endif /* CHENGBEDLOADCAPACITY_H_ */
