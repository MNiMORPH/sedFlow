/*
 * DarcyWeisbachFlowResistance.h
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

#ifndef DARCYWEISBACHFLOWRESISTANCE_H_
#define DARCYWEISBACHFLOWRESISTANCE_H_

#include "FlowResistance.h"


// This class is based on the following function:
//       (8/f)^0.5 = v / (( g * d * S )^0.5);
// v = flow velocity
// g = gravity acceleration
// S = energy slope
// d = waterdepth

namespace SedFlow {

class DarcyWeisbachFlowResistance: public FlowResistance {
private:
	double darcyWeisbachFrictionFactorForBedrock;
	double darcyWeisbachFrictionFactorForGravelbed;
	double getInterpolatedFrictionFactor(const RiverReachProperties& riverReachProperties) const;

public:
	DarcyWeisbachFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder,  double darcyWeisbachFrictionFactorForBedrock, double darcyWeisbachFrictionFactorForGravelbed);
	DarcyWeisbachFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder,  double darcyWeisbachFrictionFactorForBedrock, double darcyWeisbachFrictionFactorForGravelbed, const std::set<int>& everTreatedCellIDs);
	virtual ~DarcyWeisbachFlowResistance();

	FlowResistance* createFlowResistancePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const;
	PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReachForGivenSquareRootEightOverDarcyWeisbachFrictionFactor(const RiverReachProperties& riverReachProperties, double sqrtEightOverF) const;

	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	PowerLawRelation darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const;
	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const;

	std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > getInternalParameters() const;
	void setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters);
};

}

#endif /* DARCYWEISBACHFLOWRESISTANCE_H_ */
