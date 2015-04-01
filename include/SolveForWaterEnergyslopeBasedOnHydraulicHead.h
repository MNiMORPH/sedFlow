/*
 * SolveForWaterEnergyslopeBasedOnHydraulicHead.h
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

#ifndef SOLVEFORWATERENERGYSLOPEBASEDONHYDRAULICHEAD_H_
#define SOLVEFORWATERENERGYSLOPEBASEDONHYDRAULICHEAD_H_

#include "FlowResistance.h"

#include <functional>

#include "BasicCalculations.h"

namespace SedFlow {

class SolveForWaterEnergyslopeBasedOnHydraulicHead: public FlowResistance {
private:
	double turbulenceLossFactor;
	double maximumFroudeNumber;
	double elapsedSeconds;
	std::set<int> alreadyTreatedCellIDs;

public:
	const FlowResistance* usedFlowResistanceRelation;

	SolveForWaterEnergyslopeBasedOnHydraulicHead(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation);
	SolveForWaterEnergyslopeBasedOnHydraulicHead(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation, double turbulenceLossFactor);
	SolveForWaterEnergyslopeBasedOnHydraulicHead(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation, double turbulenceLossFactor, double elapsedSeconds, const std::set<int>& alreadyTreatedCellIDs, const std::set<int>& everTreatedCellIDs);
	virtual ~SolveForWaterEnergyslopeBasedOnHydraulicHead();

	FlowResistance* createFlowResistancePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED

	std::pair<double,double> calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(double discharge, const RiverReachProperties& riverReachProperties) const;
	std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED
	std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED

	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED
	PowerLawRelation darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const;
	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED

	std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > getInternalParameters() const;
	void setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters);
};

}

#endif /* SOLVEFORWATERENERGYSLOPEBASEDONHYDRAULICHEAD_H_ */
