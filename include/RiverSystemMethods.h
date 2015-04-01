/*
 * RiverSystemMethods.h
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

#ifndef RIVERSYSTEMMETHODS_H_
#define RIVERSYSTEMMETHODS_H_

#include "RegularRiverSystemMethods.h"
#include "AdditionalRiverSystemMethods.h"
#include "RiverSystemProperties.h"
#include "OverallMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class RiverSystemMethods {
private:
	RegularRiverSystemMethods regularRiverSystemMethods;
	AdditionalRiverSystemMethods additionalRiverSystemMethods;

	RiverSystemProperties& riverSystemProperties;
	const OverallMethods& overallMethods;

public:
	RiverSystemMethods(RegularRiverSystemMethods regularRiverSystemMethods,
			AdditionalRiverSystemMethods additionalRiverSystemMethods,
			RiverSystemProperties& riverSystemProperties,
			const OverallMethods& overallMethods);
	virtual ~RiverSystemMethods(){}

	ConstructionVariables createConstructionVariables()const;

	void throwExceptionIfWaterFlowIsNotHighestOrderFlowMethod();
	void calculateAndModifyChangeRates();
	double calculateTimeStep();
	void calculateAndHandDownChanges();
	void performAdditionalReachActions();
	void applyChanges();
	void updateRegularProperties();
	void updateAdditionalRiverReachProperties();

	int getNumberOfCells() const;

	void updateBedSlopes();
	void updateEnergySlopes();

	void updateAdditionalRiverSystemProperties();
	void performAdditionalRiverSystemActions();

	const RegularRiverSystemMethods& getRegularRiverSystemMethods() const {const RegularRiverSystemMethods& result (this->regularRiverSystemMethods); return result; }
	const AdditionalRiverSystemMethods& getAdditionalRiverSystemMethods() const {const AdditionalRiverSystemMethods& result (this->additionalRiverSystemMethods); return result; }

};

}

#endif /* RIVERSYSTEMMETHODS_H_ */
