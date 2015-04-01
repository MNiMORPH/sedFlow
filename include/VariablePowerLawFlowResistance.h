/*
 * VariablePowerLawFlowResistance.h
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

#ifndef VARIABLEPOWERLAWFLOWRESISTANCE_H_
#define VARIABLEPOWERLAWFLOWRESISTANCE_H_

#include "FlowResistance.h"

#include <utility>

namespace SedFlow {

class VariablePowerLawFlowResistance: public FlowResistance {
private:
	double calculateFlowVelocityUsingDischargeAsInputForGivenDEightyfourAndFlowWidth(double discharge, const RiverReachProperties& riverReachProperties, double dEightyfour, double flowWidth) const;
	double getInterpolatedDEightyfour(const RiverReachProperties& riverReachProperties) const;

public:
	VariablePowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder);
	VariablePowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const std::set<int>& everTreatedCellIDs);
	virtual ~VariablePowerLawFlowResistance(){}

	FlowResistance* createFlowResistancePointerCopy() const; //This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const;

	std::pair<double,double> calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(double discharge, const RiverReachProperties& riverReachProperties) const;
	std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const;

	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	PowerLawRelation darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const;
	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const;

	std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > getInternalParameters() const;
	void setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters);

};

}

#endif /* VARIABLEPOWERLAWFLOWRESISTANCE_H_ */
