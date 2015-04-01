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
#include <set>

#include "BasicCalculations.h"

namespace SedFlow {

class SolveForWaterEnergyslopeBasedOnHydraulicHead_EquilibriumFlowDepthReturnsZero;

//TODO Remove the following debugging lines
//////////////////////////////////////////////////////////////
/*
class myDebuggingRiddersMethod
{
private:
	static int signum (int value) const { return (0 < value) - (value < 0 ); }
	static double signum (double value) const { return static_cast<double>((0.0 < value) - (value < 0.0)); }

public:
	static double findRoot (const std::binder2nd< SolveForWaterEnergyslopeBasedOnHydraulicHead_EquilibriumFlowDepthReturnsZero >& function, double firstBracket, double secondBracket, double errorTolerance, int maximumNumberOfIterations);
};
*/
//////////////////////////////////////////////////////////////

class SolveForWaterEnergyslopeBasedOnHydraulicHead: public FlowResistance {
private:
	SedFlowNumericSolverRealisationBuilders< std::binder2nd< SolveForWaterEnergyslopeBasedOnHydraulicHead_EquilibriumFlowDepthReturnsZero > > numericSolverRealisationBuilders_SolveForWaterEnergyslopeBasedOnHydraulicHead;
	NumericRootFinder< std::binder2nd< SolveForWaterEnergyslopeBasedOnHydraulicHead_EquilibriumFlowDepthReturnsZero > >* numericRootFinder_SolveForWaterEnergyslopeBasedOnHydraulicHead;

	double elapsedSeconds;
	std::set<int> alreadyTreatedCellIDs;

public:
	const FlowResistance* usedFlowResistanceRelation;

	SolveForWaterEnergyslopeBasedOnHydraulicHead(double minimumFlowDepth, double maximumFlowDepth, double accuracyForTerminatingIteration, int maximumNumberOfIterations, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation);
	SolveForWaterEnergyslopeBasedOnHydraulicHead(double minimumFlowDepth, double maximumFlowDepth, double accuracyForTerminatingIteration, int maximumNumberOfIterations, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation, double elapsedSeconds, std::set<int> alreadyTreatedCellIDs);
	virtual ~SolveForWaterEnergyslopeBasedOnHydraulicHead();

	FlowResistance* createFlowResistancePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED

	std::pair<double,double> calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(double discharge, const RiverReachProperties& riverReachProperties) const;
	std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED
	std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED

	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED
	PowerLawRelation darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const;
	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const; // NOT IMPLEMENTED

	std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > getInternalParameters() const;
	void setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters);
};

class SolveForWaterEnergyslopeBasedOnHydraulicHead_BoundaryConditionsForEquilibriumFlowDepthReturnsZero
{
public:
	const FlowResistance* const flowResistance;
	double discharge;
	const RiverReachProperties& riverReachProperties;
	double downstreamHydraulicHead;
	double downstreamFrictionSlope;
	bool downstreamSillOccurence;
	double length;
	double oneOverLength;
	bool solveForDenominatorConditionInsteadOfRealEquilibrium;
	SolveForWaterEnergyslopeBasedOnHydraulicHead_BoundaryConditionsForEquilibriumFlowDepthReturnsZero(const FlowResistance* const flowResistance, double discharge, const RiverReachProperties& riverReachProperties, double downstreamHydraulicHead, double downstreamFrictionSlope, bool downstreamSillOccurence, double length):
		flowResistance(flowResistance),
		discharge(discharge),
		riverReachProperties(riverReachProperties),
		downstreamHydraulicHead(downstreamHydraulicHead),
		downstreamFrictionSlope(downstreamFrictionSlope),
		downstreamSillOccurence(downstreamSillOccurence),
		length(length),
		oneOverLength( (1.0/length) ),
		solveForDenominatorConditionInsteadOfRealEquilibrium(false)
	{}
};

class SolveForWaterEnergyslopeBasedOnHydraulicHead_EquilibriumFlowDepthReturnsZero: public std::binary_function<double,SolveForWaterEnergyslopeBasedOnHydraulicHead_BoundaryConditionsForEquilibriumFlowDepthReturnsZero,double>
{
public:
	double operator() (double flowDepth, const SolveForWaterEnergyslopeBasedOnHydraulicHead_BoundaryConditionsForEquilibriumFlowDepthReturnsZero& boundaryConditions) const
	{
		double result;
		if(boundaryConditions.solveForDenominatorConditionInsteadOfRealEquilibrium)
		{
			double sillFactor = 0.5;
			if(boundaryConditions.downstreamSillOccurence)
			{
				sillFactor = 1.0;
			}
			result = (4.0 * boundaryConditions.riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth) / boundaryConditions.flowResistance->returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(flowDepth,boundaryConditions.riverReachProperties)) - (sillFactor * boundaryConditions.length);
		}
		else
		{
			double localFlowVelocity = boundaryConditions.discharge / boundaryConditions.riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
			double localHydraulicHead = BasicCalculations::hydraulicHeadBernoulliEquation(boundaryConditions.riverReachProperties.regularRiverReachProperties.elevation,flowDepth,localFlowVelocity,((boundaryConditions.riverReachProperties.getOverallParameters())->getGravityAcceleration()) );
			double localFrictionSlope = boundaryConditions.flowResistance->returnlocalFrictionSlopeNotUsingLocalFlowDepth(flowDepth,boundaryConditions.riverReachProperties);
			double frictionSlopeProxy;
			if(boundaryConditions.downstreamSillOccurence)
				{ frictionSlopeProxy = localFrictionSlope; }
			else
				{ frictionSlopeProxy = (0.5 * (localFrictionSlope + boundaryConditions.downstreamFrictionSlope)); }
			result = (boundaryConditions.oneOverLength * (localHydraulicHead - boundaryConditions.downstreamHydraulicHead)) - frictionSlopeProxy;
		}
		return result;
	}
};

}

#endif /* SOLVEFORWATERENERGYSLOPEBASEDONHYDRAULICHEAD_H_ */
