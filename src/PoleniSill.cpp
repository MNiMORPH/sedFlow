/*
 * PoleniSill.cpp
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

#include "PoleniSill.h"

#include <iostream>
#include <math.h>

namespace SedFlow {

PoleniSill::PoleniSill(double overfallWidth):
	overfallWidth(overfallWidth),
	poleniFactor( (1.0/sqrt(3.0)) )
{}

PoleniSill::PoleniSill(double overfallWidth, double poleniFactor):
	overfallWidth(overfallWidth),
	poleniFactor(poleniFactor)
{}

PoleniSill::~PoleniSill(){}

SillProperties* PoleniSill::createSillPropertiesPointerCopy() const
{
	SillProperties* result = new PoleniSill(this->overfallWidth,this->poleniFactor);
	return result;
}

ConstructionVariables PoleniSill::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::SillProperties;
	result.realisationType = CombinerVariables::typeOfSillPropertiesToString(CombinerVariables::PoleniSill);
	std::vector<double> doubleVector;
	doubleVector.push_back(overfallWidth);
	result.labelledDoubles["overfallWidth"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(poleniFactor);
	result.labelledDoubles["poleniFactor"] = doubleVector;
	return result;
}

double PoleniSill::calculateFreeOverfallHydraulicHeadAtSill (double discharge, const RegularRiverReachProperties& regularRiverReachProperties, const AdditionalRiverReachProperties& additionalRiverReachProperties) const
{
	if( !( regularRiverReachProperties.sillOccurence ) )
	{
		const char *const missingSillErrorMessage = "SillProperties cannot be applied in reaches where no sills are present. (PoleniSill)";
		throw(missingSillErrorMessage);
	}

	return ( regularRiverReachProperties.sillTopEdgeElevation + pow(  ( 1.5 * discharge / ( overfallWidth * poleniFactor * sqrt( (2.0 * (regularRiverReachProperties.getOverallParameters())->getGravityAcceleration()) ) ) ) , (2.0/3.0)) );
}


}
