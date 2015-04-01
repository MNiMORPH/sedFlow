/*
 * ReturnBedslope.cpp
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

#include <limits>
#include <algorithm>

#include "ReturnBedslope.h"

namespace SedFlow {

ReturnBedslope::ReturnBedslope():
	minimumSlope(4.0 - std::numeric_limits<double>::max())
{
	this->typeOfGradientCalculationMethod = CombinerVariables::ReturnBedslope;
	this->isDependentOnBedslope = true;
	this->isDependentOnWaterEnergyslope = false;
}

ReturnBedslope::ReturnBedslope(double minimumSlope):
	minimumSlope(minimumSlope)
{
	this->typeOfGradientCalculationMethod = CombinerVariables::ReturnBedslope;
	this->isDependentOnBedslope = true;
	this->isDependentOnWaterEnergyslope = false;
}

CalcGradient* ReturnBedslope::createCalcGradientMethodPointerCopy() const
{
	CalcGradient* result = new ReturnBedslope(this->minimumSlope);
	return result;
}

ConstructionVariables ReturnBedslope::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReturnBedslope);
	std::vector<double> doubleVector;
	doubleVector.push_back(minimumSlope);
	result.labelledDoubles["minimumSlope"] = doubleVector;
	return result;
}

double ReturnBedslope::calculate (const RiverReachProperties& riverReachProperties) const
{
	return std::max(riverReachProperties.regularRiverReachProperties.bedslope,this->minimumSlope);
}

double ReturnBedslope::calculate (const RiverSystemProperties& river, int cellID) const
{
	return std::max((river.regularRiverSystemProperties.cellProperties.at(cellID)).regularRiverReachProperties.bedslope,this->minimumSlope);
}

std::vector<double> ReturnBedslope::calculate (const RiverSystemProperties& river) const
{
	std::vector<double> result (river.regularRiverSystemProperties.extractSingleRegularParameter(CombinerVariables::bedslope));
	for(std::vector<double>::iterator resultIterator = result.begin(); resultIterator < result.end(); ++resultIterator)
	{
		*resultIterator = std::max(*resultIterator,this->minimumSlope);
	}
	return result;
}

double ReturnBedslope::calculate (const std::vector<double>& levels, const std::vector<double>& lengths, int cellID) const
{
	const char *const errorMessage = "Within ReturnBedslope the method is not defined for levels and lengths as input parameters";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

std::vector<double> ReturnBedslope::calculate (const std::vector<double>& levels, const std::vector<double>& lengths) const
{
	const char *const errorMessage = "Within ReturnBedslope the method is not defined for levels and lengths as input parameters";
	throw(errorMessage);
	std::vector<double> fakeResult;
	return fakeResult;
}

}
