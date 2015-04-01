/*
 * RiverReachMethods.h
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

#ifndef RIVERREACHMETHODS_H_
#define RIVERREACHMETHODS_H_

#include "RegularRiverReachMethods.h"
#include "AdditionalRiverReachMethods.h"
#include "RiverReachProperties.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class RiverReachMethods {
private:
	const OverallMethods* overallMethods;
	RegularRiverReachMethods regularRiverReachMethods;
	AdditionalRiverReachMethods additionalRiverReachMethods;
	RiverReachProperties* riverReachProperties;

public:
	RiverReachMethods(){}
	RiverReachMethods(const OverallMethods* overallMethods, const RegularRiverReachMethods& regularRiverReachMethods, const AdditionalRiverReachMethods& additionalRiverReachMethods, RiverReachProperties& riverReachProperties);
	RiverReachMethods(const RiverReachMethods& toCopy);
	virtual ~RiverReachMethods(){}

	ConstructionVariables createConstructionVariables()const;

	void calculateFlowVelocityAndWaterdepth()const;
	double getCurrentDarcyWeisbachFrictionFactorF()const;
	void calculateTau()const;
	void calculateActiveWidth(const SedimentFlowMethods& sedimentFlowMethods)const;

	RegularRiverReachMethods getRegularRiverReachMethods()const {return this->regularRiverReachMethods;}

	void updateAdditionalRiverReachProperties();
	void performAdditionalRiverReachActions();

	inline RiverReachProperties* getPointerToCorrespondingRiverReachProperties()const { return riverReachProperties; }

	friend bool operator < (const RiverReachMethods& riverReachMethods1, const RiverReachMethods& riverReachMethods2)
	{
		return (riverReachMethods1.riverReachProperties->getCellID() < riverReachMethods2.riverReachProperties->getCellID());
	}

};

}

#endif /* RIVERREACHMETHODS_H_ */
