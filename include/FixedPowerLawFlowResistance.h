/*
 * FixedPowerLawFlowResistance.h
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

#ifndef FIXEDPOWERLAWFLOWRESISTANCE_H_
#define FIXEDPOWERLAWFLOWRESISTANCE_H_

#include "FlowResistance.h"

// This class is based on the following function:
//       (8/f)^0.5 = a * ( d / b*Dn ) ^ c ; with:
//       (8/f)^0.5 = v / (( g * d * S )^0.5);
// v = flow velocity
// g = gravity acceleration
// S = energy slope
// d = hydraulicRadius
// D = grain diameter
// The further variables are parameters of this method:
// a = factor
// b = grainsFactor
// n = grainsPercentile
// c = exponent

namespace SedFlow {

class FixedPowerLawFlowResistance: public FlowResistance {
private:
	double factor;
	double grainsFactor;
	double grainsPercentile;
	double exponent;

	double getInterpolatedRepresentativeGrainDiameter(const RiverReachProperties& riverReachProperties) const;

public:
	FixedPowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, double factor, double grainsFactor, double grainsPercentile, double exponent);
	FixedPowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, double factor, double grainsFactor, double grainsPercentile, double exponent, const std::set<int>& everTreatedCellIDs);
	virtual ~FixedPowerLawFlowResistance();

	FlowResistance* createFlowResistancePointerCopy() const; //This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	PowerLawRelation dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const;

	std::pair<double,double> calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	std::pair<double,double> calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const;

	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const;
	PowerLawRelation darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const;
	double returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const;

	std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > getInternalParameters() const;
	void setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters);

};

}

#endif /* FIXEDPOWERLAWFLOWRESISTANCE_H_ */
