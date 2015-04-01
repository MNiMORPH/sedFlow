/*
 * MultipleReferenceGrainDiameter.cpp
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

#include "MultipleReferenceGrainDiameter.h"

namespace SedFlow {

MultipleReferenceGrainDiameter::MultipleReferenceGrainDiameter(double factor, double referenceDiameterPercentile, double defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream):
	factor(factor),
	referenceDiameterPercentile(referenceDiameterPercentile),
	defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream(defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream)
{}


MultipleReferenceGrainDiameter::~MultipleReferenceGrainDiameter(){}

EstimateThicknessOfMovingSedimentLayer* MultipleReferenceGrainDiameter::createEstimateThicknessOfMovingSedimentLayerPointerCopy() const
{
	EstimateThicknessOfMovingSedimentLayer* result = new MultipleReferenceGrainDiameter(this->factor, this->referenceDiameterPercentile, this->defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream);
	return result;
}

ConstructionVariables MultipleReferenceGrainDiameter::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::EstimateThicknessOfMovingSedimentLayer;
	result.realisationType = CombinerVariables::typeOfEstimationMethodForThicknessOfMovingSedimentLayerToString(CombinerVariables::MultipleReferenceGrainDiameter);
	std::vector<double> doubleVector;
	doubleVector.push_back(this->factor);
	result.labelledDoubles["factor"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(this->referenceDiameterPercentile);
	result.labelledDoubles["referenceDiameterPercentile"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(this->defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream);
	result.labelledDoubles["defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream"] = doubleVector;
	return result;
}

double MultipleReferenceGrainDiameter::estimate (const RiverReachProperties& riverReachProperties) const
{
	const Grains* estimationBase = &(riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0));
	if( !(estimationBase->getOverallVolume() > 0.0) )
	{
		estimationBase = &(riverReachProperties.regularRiverReachProperties.depositionRate);
	}

	double result = factor * estimationBase->getPercentileGrainDiameter( ((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters()) ,referenceDiameterPercentile);

	if( result != result )
	{
		result = defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream;
	}

	return result;
}

}
