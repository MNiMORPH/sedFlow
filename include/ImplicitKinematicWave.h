/*
 * ImplicitKinematicWave.h
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

#ifndef IMPLICITKINEMATICWAVE_H_
#define IMPLICITKINEMATICWAVE_H_

#include <utility>

#include "FlowTypeMethods.h"
#include "OverallMethods.h"

namespace SedFlow {

class ImplicitKinematicWave: public FlowTypeMethods {
private:
	double maximumTimeStep;
	bool checkForCourantFriedrichsLewy;
	const OverallMethods& overallMethods;

public:
	ImplicitKinematicWave(double maximumTimeStep, bool checkForCourantFriedrichsLewy, const OverallMethods& overallMethods);
	virtual ~ImplicitKinematicWave(){}

	virtual FlowTypeMethods* createFlowTypeMethodsPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void calculateChangeRate (RiverReachProperties& riverReachProperties) const;
	void handDownChangeRate (RiverReachProperties& riverReachProperties) const;
	void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const;
	double calculateTimeStep (const RiverSystemProperties& riverSystem) const;
	void calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const;
	void handDownChange (RiverReachProperties& riverReachProperties) const;
	void applyChange (RiverReachProperties& riverReachProperties) const;
	void updateOtherParameters (RiverReachProperties& riverReachProperties) const;
	void handDownOtherParameters (RiverReachProperties& riverReachProperties) const;

};

}

#endif /* IMPLICITKINEMATICWAVE_H_ */
