/*
 * WilcockCroweBedloadCapacity.h
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

#ifndef WILCOCKCROWEBEDLOADCAPACITY_H_
#define WILCOCKCROWEBEDLOADCAPACITY_H_

#include "CalcBedloadCapacity.h"

namespace SedFlow {

class WilcockCroweBedloadCapacity: public CalcBedloadCapacity {
private:
	bool useVersionOfSchneiderEtAl;
	bool useConstantSandFraction;
	double constantSandFraction;
	double getInterpolatedMeanDiameterForHiding (double meanDiameter, const std::vector<double>& fractionalGrainDiameters, const RiverReachProperties& riverReachProperties) const;

protected:
	double breakPoint;
	double firstExponentForRelativeShearStress;
	double secondExponentForRelativeShearStress;
	virtual double calculateDimensionlessReferenceShearStressForMeanDiameter (const RiverReachProperties& riverReachProperties, const Grains* const erosionBase, const std::vector<double>& fractionalGrainDiameters) const;

public:
	WilcockCroweBedloadCapacity();
	WilcockCroweBedloadCapacity(bool useConstantSandFraction, double constantSandFraction);
	virtual ~WilcockCroweBedloadCapacity(){}

	virtual CalcBedloadCapacity* createCalcBedloadCapacityMethodPointerCopy() const;

	virtual ConstructionVariables createConstructionVariables()const;

	Grains calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const;
};

}

#endif /* WILCOCKCROWEBEDLOADCAPACITY_H_ */
