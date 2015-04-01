/*
 * RegularRiverSystemMethods.h
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

#ifndef REGULARRIVERSYSTEMMETHODS_H_
#define REGULARRIVERSYSTEMMETHODS_H_

#include <vector>

#include "RiverSystemProperties.h"
#include "OverallMethods.h"
#include "FlowMethods.h"
#include "ChangeRateModifiers.h"
#include "ChangeRateModifiersForSingleFlowMethod.h"
#include "Grains.h"
#include "RiverReachMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class RegularRiverSystemMethods {
private:
	std::vector<RiverReachMethods> riverReachMethods;

	FlowMethods flowMethods;
	ChangeRateModifiers changeRateModifiers;

public:
	RegularRiverSystemMethods(std::vector<RiverReachMethods> riverReachMethods, FlowMethods flowMethods, ChangeRateModifiers changeRateModifiers);
	virtual ~RegularRiverSystemMethods();

	ConstructionVariables createConstructionVariables(const RiverSystemProperties& parameters)const;

	void throwExceptionIfWaterFlowIsNotHighestOrderFlowMethod();
	void calculateAndModifyChangeRates(RiverSystemProperties& parameters, const OverallMethods& overallMethods);
	double calculateTimeStep(const RiverSystemProperties& parameters);
	void calculateAndHandDownChanges(RiverSystemProperties& parameters);
	void performAdditionalReachActions();
	void applyChanges(RiverSystemProperties& parameters);
	void updateRegularProperties(RiverSystemProperties& parameters);
	void updateAdditionalRiverReachProperties();

	void updateBedSlopes(RiverSystemProperties& parameters, const OverallMethods& overallMethods)const;
	void updateBedSlope(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods)const;
	void updateWaterEnergySlopes(RiverSystemProperties& parameters, const OverallMethods& overallMethods)const;
	void updateWaterEnergySlope(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods)const;
	void updateSedimentEnergySlopes(RiverSystemProperties& parameters, const OverallMethods& overallMethods)const;
	void updateSedimentEnergySlope(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods)const;
	void updateTaus()const;
	void updateActiveWidths()const;

	const RiverReachMethods& getFirstRiverReachMethods() const { const RiverReachMethods& result (riverReachMethods.front()); return result; }
	const RiverReachMethods& getLastRiverReachMethods() const { const RiverReachMethods& result (riverReachMethods.back()); return result; }
	const FlowMethods& getFlowMethods() const { const FlowMethods& result (flowMethods); return result; }
	const ChangeRateModifiers& getChangeRateModifiers() const { const ChangeRateModifiers& result (changeRateModifiers); return result; }

};

}

#endif /* REGULARRIVERSYSTEMMETHODS_H_ */
