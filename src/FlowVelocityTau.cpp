/*
 * FlowVelocityTau.cpp
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

#include "FlowVelocityTau.h"
#include "BasicCalculations.h"

#include <math.h>

namespace SedFlow {

FlowVelocityTau::FlowVelocityTau() {}

FlowVelocityTau::~FlowVelocityTau() {}

CalcTau* FlowVelocityTau::createCalcTauPointerCopy() const
{
	CalcTau* result = new FlowVelocityTau();
	return result;
}

ConstructionVariables FlowVelocityTau::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcTau;
	result.realisationType = CombinerVariables::typeOfTauCalculationMethodToString(CombinerVariables::FlowVelocityTau);
	return result;
}

double FlowVelocityTau::calculate (const RiverReachProperties& riverReachProperties, const FlowResistance* flowResistance) const
{
	return ( pow( (BasicCalculations::convertDarcyWeisbachFrictionFactorFIntoManningsN( (*flowResistance).returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth,riverReachProperties), riverReachProperties ) * riverReachProperties.regularRiverReachProperties.flowVelocity) ,2.0) * (riverReachProperties.getOverallParameters())->getGravityAcceleration() * pow(riverReachProperties.regularRiverReachProperties.maximumWaterdepth, (-0.33)) * (riverReachProperties.getOverallParameters())->getWaterDensity() );
}

}
