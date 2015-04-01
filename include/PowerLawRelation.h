/*
 * PowerLawRelation.h
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

#ifndef POWERLAWRELATION_H_
#define POWERLAWRELATION_H_

#include <utility>

namespace SedFlow {

class PowerLawRelation {
private:
	bool isPowerLaw;
	// f(x) = a + b * (x^c);
	double a;
	double b;
	double c;

public:
	PowerLawRelation(bool isPowerLaw = false);
	PowerLawRelation(double addition, double factor, double exponent);
	virtual ~PowerLawRelation();

	inline bool powerLawCheck() const { return isPowerLaw; }
	inline std::pair<bool,double> additionWithPowerLawCheck() const { return std::make_pair(isPowerLaw,a); }
	inline std::pair<bool,double> factorWithPowerLawCheck() const { return std::make_pair(isPowerLaw,b); }
	inline std::pair<bool,double> exponentWithPowerLawCheck() const { return std::make_pair(isPowerLaw,c); }
	inline double addition() const { return a; }
	inline double factor() const { return b; }
	inline double exponent() const { return c; }
};

}

#endif /* POWERLAWRELATION_H_ */
