/*
 * PowerLawRelation.cpp
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

#include "PowerLawRelation.h"

#include <iostream>
#include <limits>

namespace SedFlow {

PowerLawRelation::PowerLawRelation(bool isPowerLaw):
	isPowerLaw(false),
	a(std::numeric_limits<double>::quiet_NaN()),
	b(std::numeric_limits<double>::quiet_NaN()),
	c(std::numeric_limits<double>::quiet_NaN())
{
	if(isPowerLaw)
	{
		const char *const errorMessage = "For a valid PowerLaw the doubles addition, factor and exponent need to be given instead of the bool isPowerLaw.";
		throw(errorMessage);
	}
}

PowerLawRelation::PowerLawRelation(double addition, double factor, double exponent):
	isPowerLaw(true),
	a(addition),
	b(factor),
	c(exponent)
{}

PowerLawRelation::~PowerLawRelation() {}

}
