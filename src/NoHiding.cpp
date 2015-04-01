/*
 * NoHiding.cpp
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

#include "NoHiding.h"

namespace SedFlow {

NoHiding::NoHiding() {}

NoHiding::~NoHiding() {}


CalcHidingFactors* NoHiding::createCalcHidingFactorsMethodPointerCopy() const
{
	CalcHidingFactors* result = new NoHiding();
	return result;
}


ConstructionVariables NoHiding::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcHidingFactors;
	result.realisationType = CombinerVariables::typeOfHidingFactorsCalculationMethodToString(CombinerVariables::NoHiding);
	return result;
}

std::vector<double> NoHiding::calculateFractionalHidingFactors (const std::vector<double>& fractionalGrainDiameters, const std::vector<double>& fractionalAbundances, const RiverReachProperties& riverReachProperties) const
{
	return (std::vector<double>(fractionalGrainDiameters.size(),1.0));
}

}
