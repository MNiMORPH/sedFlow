/*
 * TimeSeriesEntry.cpp
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

#include "TimeSeriesEntry.h"
#include <limits>

namespace SedFlow {

TimeSeriesEntry::~TimeSeriesEntry(){}

TimeSeriesEntry::TimeSeriesEntry(double elapsedSeconds, double doubleValue):
	elapsedSeconds(elapsedSeconds),
	doubleValue(doubleValue)
{}

TimeSeriesEntry::TimeSeriesEntry(double elapsedSeconds, Grains grainsValue):
	elapsedSeconds(elapsedSeconds),
	grainsValue(grainsValue),
	doubleValue(std::numeric_limits<double>::quiet_NaN())
{}

ConstructionVariables TimeSeriesEntry::createConstructionVariables() const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::TimeSeriesEntry;
	std::vector<double> doubleVector;
	doubleVector.push_back(this->elapsedSeconds);
	result.labelledDoubles["elapsedSeconds"] = doubleVector;
	if( this->doubleValue == this->doubleValue ) //i.e. if it is not a NaN.
	{
		doubleVector.clear();
		doubleVector.push_back(this->doubleValue);
		result.labelledDoubles["doubleValue"] = doubleVector;
	}
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( this->grainsValue.createConstructionVariables() );
	result.labelledObjects["grainsValue"] = constructionVariablesVector;
	return result;
}

}
