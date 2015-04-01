/*
 * BedrockRoughnessEquivalentRepresentativeGrainDiameter.cpp
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

#include "BedrockRoughnessEquivalentRepresentativeGrainDiameter.h"

namespace SedFlow {

BedrockRoughnessEquivalentRepresentativeGrainDiameter::BedrockRoughnessEquivalentRepresentativeGrainDiameter(double equivalentGrainDiameter):
	equivalentGrainDiameter(equivalentGrainDiameter)
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter;
}

BedrockRoughnessEquivalentRepresentativeGrainDiameter::~BedrockRoughnessEquivalentRepresentativeGrainDiameter(){}

AdditionalRiverReachPropertyType* BedrockRoughnessEquivalentRepresentativeGrainDiameter::createAdditionalRiverPropertyAndMethodTypePointerCopy() const
{
	AdditionalRiverReachPropertyType* result = new BedrockRoughnessEquivalentRepresentativeGrainDiameter(this->equivalentGrainDiameter);
	return result;
}

ConstructionVariables BedrockRoughnessEquivalentRepresentativeGrainDiameter::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachPropertyType;
	result.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter);
	std::vector<double> doubleVector (1,this->equivalentGrainDiameter);
	result.labelledDoubles["equivalentGrainDiameter"] = doubleVector;
	return result;
}


std::pair< CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod , std::vector<double> > BedrockRoughnessEquivalentRepresentativeGrainDiameter::typeSpecificGetter() const
{
	std::vector<double> resultDoubleVector (1,this->equivalentGrainDiameter);
	std::pair< CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod , std::vector<double> > result (this->typeOfAdditionalRiverPropertyAndMethod,resultDoubleVector);
	return result;
}

}
