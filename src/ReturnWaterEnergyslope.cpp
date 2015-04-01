/*
 * ReturnWaterEnergyslope.cpp
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

#include "ReturnWaterEnergyslope.h"

#include <limits>

namespace SedFlow {

ReturnWaterEnergyslope::ReturnWaterEnergyslope():
	minimumSlope(4.0 - std::numeric_limits<double>::max())
{
	this->typeOfGradientCalculationMethod = CombinerVariables::ReturnWaterEnergyslope;
	this->isDependentOnBedslope = false;
	this->isDependentOnWaterEnergyslope = true;
}

ReturnWaterEnergyslope::ReturnWaterEnergyslope(double minimumSlope):
	minimumSlope(minimumSlope)
{
	this->typeOfGradientCalculationMethod = CombinerVariables::ReturnWaterEnergyslope;
	this->isDependentOnBedslope = false;
	this->isDependentOnWaterEnergyslope = true;
}

ReturnWaterEnergyslope::~ReturnWaterEnergyslope(){}

CalcGradient* ReturnWaterEnergyslope::createCalcGradientMethodPointerCopy() const
{
	CalcGradient* result = new ReturnWaterEnergyslope(this->minimumSlope);
	return result;
}

ConstructionVariables ReturnWaterEnergyslope::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReturnWaterEnergyslope);
	std::vector<double> doubleVector;
	doubleVector.push_back(minimumSlope);
	result.labelledDoubles["minimumSlope"] = doubleVector;
	return result;
}

double ReturnWaterEnergyslope::calculate (const RiverReachProperties& riverReachProperties) const
{
	return std::max(riverReachProperties.regularRiverReachProperties.waterEnergyslope,this->minimumSlope);
}

double ReturnWaterEnergyslope::calculate (const RiverSystemProperties& river, int cellID) const
{
	return std::max((river.regularRiverSystemProperties.cellProperties.at(cellID)).regularRiverReachProperties.waterEnergyslope,this->minimumSlope);
}

std::vector<double> ReturnWaterEnergyslope::calculate (const RiverSystemProperties& river) const
{
	std::vector<double> result (river.regularRiverSystemProperties.extractSingleRegularParameter(CombinerVariables::waterEnergyslope));
	for(std::vector<double>::iterator resultIterator = result.begin(); resultIterator < result.end(); ++resultIterator)
	{
		*resultIterator = std::max(*resultIterator,this->minimumSlope);
	}
	return result;
}

double ReturnWaterEnergyslope::calculate (const std::vector<double>& levels, const std::vector<double>& lengths, int cellID) const
{
	const char *const errorMessage = "Within ReturnWaterEnergyslope the method is not defined for levels and lengths as input parameters";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

std::vector<double> ReturnWaterEnergyslope::calculate (const std::vector<double>& levels, const std::vector<double>& lengths) const
{
	const char *const errorMessage = "Within ReturnWaterEnergyslope the method is not defined for levels and lengths as input parameters";
	throw(errorMessage);
	std::vector<double> fakeResult;
	return fakeResult;
}

}
