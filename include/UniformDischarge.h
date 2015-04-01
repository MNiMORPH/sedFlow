/*
 * UniformDischarge.h
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

#ifndef UNIFORMDISCHARGE_H_
#define UNIFORMDISCHARGE_H_

#include "FlowTypeMethods.h"

#include "OverallMethods.h"

namespace SedFlow {

class UniformDischarge: public FlowTypeMethods {
private:
	double maximumTimeStep;
	const OverallMethods& overallMethods;

public:
	UniformDischarge(double maximumTimeStep, const OverallMethods& overallMethods);
	virtual ~UniformDischarge();

	FlowTypeMethods* createFlowTypeMethodsPointerCopy() const;

	ConstructionVariables createConstructionVariables() const;

	inline void calculateChangeRate (RiverReachProperties& riverReachProperties) const { riverReachProperties.regularRiverReachProperties.waterVolumeChange = riverReachProperties.regularRiverReachProperties.waterVolumeChangeRate = 0.0; }
	inline void handDownChangeRate (RiverReachProperties& riverReachProperties) const {}
	inline void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const {}
	inline double calculateTimeStep (const RiverSystemProperties& riverSystem) const { return maximumTimeStep; }
	inline void calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const {}
	inline void handDownChange (RiverReachProperties& riverReachProperties) const {}
	void applyChange (RiverReachProperties& riverReachProperties) const;
	void updateOtherParameters (RiverReachProperties& riverReachProperties) const;
	inline void handDownOtherParameters (RiverReachProperties& riverReachProperties) const {}
};

}

#endif /* UNIFORMDISCHARGE_H_ */
