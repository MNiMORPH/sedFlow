/*
 * ReckingBedloadCapacityNonFractional.h
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

#ifndef RECKINGBEDLOADCAPACITYNONFRACTIONAL_H_
#define RECKINGBEDLOADCAPACITYNONFRACTIONAL_H_

#include "CalcBedloadCapacity.h"
#include "ConstructionVariables.h"

//This class is based on:
//			Recking(2010): A comparison between flume and field bed load transport data and consequences for surface-based bed load transport prediction. WATER RESOURCES RESEARCH, VOL. 46, W03518, doi:10.1029/2009WR008007
//For references to the single formulas, please see the .cpp

namespace SedFlow {

class ReckingBedloadCapacityNonFractional: public CalcBedloadCapacity {
public:
	ReckingBedloadCapacityNonFractional(){}
	virtual ~ReckingBedloadCapacityNonFractional(){}

	CalcBedloadCapacity* createCalcBedloadCapacityMethodPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	Grains calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const;
};

}

#endif /* RECKINGBEDLOADCAPACITYNONFRACTIONAL_H_ */
