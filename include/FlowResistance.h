/*
 * FlowResistance.h
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

#ifndef FLOWRESISTANCE_H_
#define FLOWRESISTANCE_H_

#include <utility>
#include <vector>
#include <set>

#include "CombinerVariables.h"
#include "RiverReachProperties.h"
#include "ConstructionVariables.h"
#include "PowerLawRelation.h"
#include "BasicCalculations.h"
#include "SedFlowNumericSolverRealisationBuilders.h"

namespace SedFlow {

class FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero;
class FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead;

class FlowResistance {
protected:
	CombinerVariables::TypesOfFlowResistance typeOfFlowResistance;

	double startingValueForIteration;
	double accuracyForTerminatingIteration;
	int maximumNumberOfIterations;
	bool useApproximationsForHydraulicRadius;
	std::set<int> everTreatedCellIDs;

	CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder;

	SedFlowNumericSolverRealisationBuilders< std::binder2nd< FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero > > flowResistanceSolverBuilder;
	NumericRootFinder< std::binder2nd< FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero > >* flowResistanceSolver;

	SedFlowNumericSolverRealisationBuilders< std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero > > maximumFroudeSolverBuilder;
	NumericRootFinder< std::binder2nd< BasicCalculations_EquilibriumFlowDepthForGivenFroudeAndDischargeReturnsZero > >* maximumFroudeSolver;

	SedFlowNumericSolverRealisationBuilders< std::binder2nd< FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead > > minimumHydraulicSlopeSolverBuilder;
	NumericRootFinder< std::binder2nd< FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead > >* minimumHydraulicSlopeSolver;

	double maximumFroudeNumber;
	double minimumHydraulicSlope;

	PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfCrossSectionalArea(const RiverReachProperties& riverReachProperties) const;
	PowerLawRelation hydraulicRadiusAsPowerLawFunctionOfMaximumFlowDepth(const RiverReachProperties& riverReachProperties) const;
	double convertMaximumFlowDepthIntoHydraulicRadius(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	double convertHydraulicRadiusIntoMaximumFlowDepth(double hydraulicRadius, const RiverReachProperties& riverReachProperties) const;
	double getHydraulicRadiusForActiveWidth(double maximumFlowDepth, double activeWidth, const RiverReachProperties& riverReachProperties) const;

	double getStartingFlowDepthForIteration(const RiverReachProperties& riverReachProperties) const;
	std::pair<double,double> getFlowDepthAndVelocityEnsuringMinimumHydraulicSlope(std::pair<double,double> previouslyCalculatedFlowDepthAndVelocity, double discharge, double minimumFlowDepth, double gravityAcceleration, const RiverReachProperties& riverReachProperties)const;

public:
	// In each implementation, the following constructor should be implemented:
	//SpecificFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, double darcyWeisbachFrictionFactorForBedrock, const std::vector< std::vector<double> >& parameters);
	FlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder);
	FlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const std::set<int>& everTreatedCellIDs);
	virtual ~FlowResistance();

	virtual FlowResistance* createFlowResistancePointerCopy() const = 0; //This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const = 0;

	std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInputBasedOnDarcyWeisbachFrictionFactor(double flowDepth, const RiverReachProperties& riverReachProperties, double darcyWeisbachFrictionFactor) const;
	std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInputBasedOnDarcyWeisbachFrictionFactor(double flowVelocity, const RiverReachProperties& riverReachProperties, double darcyWeisbachFrictionFactor) const;

	virtual std::pair<double,double> calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(double discharge, const RiverReachProperties& riverReachProperties) const;
	virtual std::pair<double,double> calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(double discharge, const RiverReachProperties& riverReachProperties) const;
	virtual std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	virtual std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const;

	inline std::pair<double,double> calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(const RiverReachProperties& riverReachProperties) const { return calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(riverReachProperties.regularRiverReachProperties.discharge, riverReachProperties); }
	inline std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(const RiverReachProperties& riverReachProperties) const { return calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(riverReachProperties.regularRiverReachProperties.maximumWaterdepth, riverReachProperties); }
	inline std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(const RiverReachProperties& riverReachProperties) const { return calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(riverReachProperties.regularRiverReachProperties.flowVelocity, riverReachProperties); }

	inline void calculateAndUpdateFlowDepthAndFlowVelocityUsingDischargeAsInput(RiverReachProperties& riverReachProperties) const { std::pair<double,double> result = calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(riverReachProperties); riverReachProperties.regularRiverReachProperties.maximumWaterdepth = result.first; riverReachProperties.regularRiverReachProperties.flowVelocity = result.second; }
	inline void calculateAndUpdateDischargeAndFlowVelocityUsingFlowDepthAsInput(RiverReachProperties& riverReachProperties) const { std::pair<double,double> result = calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(riverReachProperties); riverReachProperties.regularRiverReachProperties.discharge = result.first; riverReachProperties.regularRiverReachProperties.flowVelocity = result.second; }
	inline void calculateAndUpdateDischargeAndFlowDepthUsingFlowVelocityAsInput(RiverReachProperties& riverReachProperties) const { std::pair<double,double> result = calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(riverReachProperties); riverReachProperties.regularRiverReachProperties.discharge = result.first; riverReachProperties.regularRiverReachProperties.maximumWaterdepth = result.second; }

	virtual double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const = 0;
	virtual PowerLawRelation darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const = 0;
	virtual double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const = 0;

	double returnlocalFrictionSlopeNotUsingLocalFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	double returnlocalFrictionSlopeNotUsingLocalFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const;
	double returnlocalFrictionSlope(const RiverReachProperties& riverReachProperties) const;

	virtual std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > getInternalParameters() const = 0;
	virtual void setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters) = 0;

	inline CombinerVariables::TypesOfFlowResistance getTypeOfFlowResistance() const { return typeOfFlowResistance; }
	inline std::string getTypeOfFlowResistanceAsString() const { return CombinerVariables::typeOfFlowResistanceToString(typeOfFlowResistance); }

	inline CombinerVariables::TypesOfNumericRootFinder getTypeOfNumericRootFinder() const { return typeOfNumericRootFinder; }
	inline std::string getTypeOfNumericRootFinderAsString() const { return CombinerVariables::typeOfNumericRootFinderToString(typeOfNumericRootFinder); }

	inline double getAccuracy() const { return accuracyForTerminatingIteration; }

};


class FlowResistance_BoundaryConditionsForEquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero
{
public:
	double discharge;
	const RiverReachProperties& riverReachProperties;
	const FlowResistance* const flowResistance;
	FlowResistance_BoundaryConditionsForEquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero(double discharge, const RiverReachProperties& riverReachProperties, const FlowResistance* const flowResistance):
		discharge(discharge),
		riverReachProperties(riverReachProperties),
		flowResistance(flowResistance)
	{}
};

class FlowResistance_EquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero: public std::binary_function<double,FlowResistance_BoundaryConditionsForEquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero,double>
{
public:
	double operator() (double flowDepth, const FlowResistance_BoundaryConditionsForEquilibriumFlowDepthForGivenDischargeAndUsedFlowResistanceReturnsZero& boundaryConditions) const
	{
		return ( boundaryConditions.discharge - (boundaryConditions.flowResistance->calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(flowDepth,boundaryConditions.riverReachProperties)).first );
	}
};


class FlowResistance_BoundaryConditionsForCurrentHydraulicHeadMinusMinimumHydraulicHead
{
public:
	double minimumHydraulicHead;
	double elevation;
	double discharge;
	double gravityAcceleration;
	const ChannelGeometry* const channelGeometry;
	FlowResistance_BoundaryConditionsForCurrentHydraulicHeadMinusMinimumHydraulicHead(double minimumHydraulicHead, double elevation, double discharge, double gravityAcceleration, const ChannelGeometry* channelGeometry):
		minimumHydraulicHead(minimumHydraulicHead),
		elevation(elevation),
		discharge(discharge),
		gravityAcceleration(gravityAcceleration),
		channelGeometry(channelGeometry)
	{}
};

class FlowResistance_CurrentHydraulicHeadMinusMinimumHydraulicHead: public std::binary_function<double,FlowResistance_BoundaryConditionsForCurrentHydraulicHeadMinusMinimumHydraulicHead,double>
{
public:
	double operator() (double flowDepth, const FlowResistance_BoundaryConditionsForCurrentHydraulicHeadMinusMinimumHydraulicHead& boundaryConditions) const
	{
		double flowVelocity = boundaryConditions.discharge / boundaryConditions.channelGeometry->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
		return ( BasicCalculations::hydraulicHeadBernoulliEquation(boundaryConditions.elevation,flowDepth,flowVelocity,boundaryConditions.gravityAcceleration) - boundaryConditions.minimumHydraulicHead );
	}
};


}

#endif /* FLOWRESISTANCE_H_ */
