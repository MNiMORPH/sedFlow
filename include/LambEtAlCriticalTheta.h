/*
 * LambEtAlCriticalTheta.h
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

#ifndef LAMBETALCRITICALTHETA_H_
#define LAMBETALCRITICALTHETA_H_

#include "CalcThresholdForInitiationOfBedloadMotion.h"

namespace SedFlow {

class LambEtAlCriticalTheta: public CalcThresholdForInitiationOfBedloadMotion {
private:
	double minimumCriticalDimensionlessShearStress;
	bool correctionForBedloadWeightAtSteepCounterSlopes;

public:
	LambEtAlCriticalTheta(double minimumCriticalDimensionlessShearStress, bool correctionForBedloadWeightAtSteepCounterSlopes);
	virtual ~LambEtAlCriticalTheta();

	CalcThresholdForInitiationOfBedloadMotion* createCalcThresholdForInitiationOfBedloadMotionPointerCopy() const; //This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	double calculate (const RegularRiverReachProperties& regularRiverReachProperties) const;

};

}

#endif /* LAMBETALCRITICALTHETA_H_ */
