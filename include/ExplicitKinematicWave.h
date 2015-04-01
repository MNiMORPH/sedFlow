/*
 * ExplicitKinematicWave.h
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

#ifndef EXPLICITKINEMATICWAVE_H_
#define EXPLICITKINEMATICWAVE_H_

#include "FlowTypeMethods.h"

#include "OverallMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class ExplicitKinematicWave: public FlowTypeMethods {
private:
	// TODO If the lines of the To Do in the *.cpp are switched on again, this variable needs to be switched on and included in the constructor and builder.
	//double maximumRelativeTwoCellDischargeGradientChange;
	const OverallMethods& overallMethods;

public:
	ExplicitKinematicWave(const OverallMethods& overallMethods);
	virtual ~ExplicitKinematicWave(){}

	virtual FlowTypeMethods* createFlowTypeMethodsPointerCopy() const;

	virtual ConstructionVariables createConstructionVariables() const;

	virtual void calculateChangeRate (RiverReachProperties& riverReachProperties) const;
	virtual void handDownChangeRate (RiverReachProperties& riverReachProperties) const;
	virtual void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const;
	virtual double calculateTimeStep (const RiverSystemProperties& riverSystem) const;
	virtual void calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const;
	virtual void handDownChange (RiverReachProperties& riverReachProperties) const;
	virtual void applyChange (RiverReachProperties& riverReachProperties) const;
	virtual void updateOtherParameters (RiverReachProperties& riverReachProperties) const;
	virtual void handDownOtherParameters (RiverReachProperties& riverReachProperties) const;
};

}

#endif /* EXPLICITKINEMATICWAVE_H_ */
