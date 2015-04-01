/*
 * UniformDischarge.cpp
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

#include "UniformDischarge.h"

namespace SedFlow {

UniformDischarge::UniformDischarge(double maximumTimeStep, const OverallMethods& overallMethods):
	FlowTypeMethods(CombinerVariables::UniformDischarge,CombinerVariables::WaterFlowMethodsInGeneral),
	maximumTimeStep(maximumTimeStep),
	overallMethods(overallMethods)
{}

UniformDischarge::~UniformDischarge(){}

FlowTypeMethods* UniformDischarge::createFlowTypeMethodsPointerCopy() const
{
	FlowTypeMethods* result = new UniformDischarge(this->maximumTimeStep,this->overallMethods);
	return result;
}

ConstructionVariables UniformDischarge::createConstructionVariables() const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;
	result.realisationType = CombinerVariables::typeOfFlowMethodsToString(CombinerVariables::UniformDischarge);
	std::vector<double> doubleVector;
	doubleVector.push_back(maximumTimeStep);
	result.labelledDoubles["maximumTimeStep"] = doubleVector;
	return result;
}

void UniformDischarge::applyChange (RiverReachProperties& riverReachProperties) const
{
	if (!(riverReachProperties.isUpstreamMargin()))
	{
		double newDischarge = 0.0;
		std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
		for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamPointerIterator = upstreamCellPointers.begin(); currentUpstreamPointerIterator < upstreamCellPointers.end(); ++currentUpstreamPointerIterator)
		{
			newDischarge += (*currentUpstreamPointerIterator)->regularRiverReachProperties.discharge;
		}
		riverReachProperties.regularRiverReachProperties.waterVolumeChange = newDischarge - riverReachProperties.regularRiverReachProperties.discharge;
		riverReachProperties.regularRiverReachProperties.discharge = newDischarge;
	}
}

void UniformDischarge::updateOtherParameters (RiverReachProperties& riverReachProperties) const
{
	overallMethods.flowResistance->calculateAndUpdateFlowDepthAndFlowVelocityUsingDischargeAsInput(riverReachProperties);
}

}
