/*
 * FlowTypeMethods.h
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

#ifndef FLOWTYPEMETHODS_H_
#define FLOWTYPEMETHODS_H_

#include "CombinerVariables.h"
#include "CalcGradient.h"
#include "RegularRiverReachProperties.h"
#include "RiverSystemProperties.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class FlowTypeMethods {
protected:
	CombinerVariables::TypesOfFlowMethods typeOfFlowMethods;
	CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods;

public:
	virtual FlowTypeMethods* createFlowTypeMethodsPointerCopy() const = 0;//This method HAS TO BE implemented.

	FlowTypeMethods(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods, CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods);
	virtual ~FlowTypeMethods();

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual void calculateChangeRate (RiverReachProperties& riverReachProperties) const = 0;
	virtual void handDownChangeRate (RiverReachProperties& riverReachProperties) const = 0;
	virtual void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const = 0;
	virtual double calculateTimeStep (const RiverSystemProperties& riverSystem) const = 0;
	virtual void calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const = 0;
	virtual void handDownChange (RiverReachProperties& riverReachProperties) const = 0;
	virtual void applyChange (RiverReachProperties& riverReachProperties) const = 0;
	virtual void updateOtherParameters (RiverReachProperties& riverReachProperties) const = 0;
	virtual void handDownOtherParameters (RiverReachProperties& riverReachProperties) const = 0;

	inline std::string getTypeOfFlowMethodsAsString() const { return CombinerVariables::typeOfFlowMethodsToString(typeOfFlowMethods); }
	inline CombinerVariables::TypesOfFlowMethods getTypeOfFlowMethods() const { return typeOfFlowMethods; }
	inline std::string getTypeOfGeneralFlowMethodsAsString() const { return CombinerVariables::typeOfGeneralFlowMethodsToString(typeOfGeneralFlowMethods); }
	inline CombinerVariables::TypesOfGeneralFlowMethods getTypeOfGeneralFlowMethods() const { return typeOfGeneralFlowMethods; }

	inline bool isWaterFlow() const { return (typeOfGeneralFlowMethods==CombinerVariables::WaterFlowMethodsInGeneral); }

	friend bool operator < (const FlowTypeMethods& flowTypeMethods1, const FlowTypeMethods& flowTypeMethods2)
	{
		if (flowTypeMethods1.typeOfFlowMethods < flowTypeMethods2.typeOfFlowMethods) {return (true);}
		else {return (false);}
	}

	friend bool operator == (const FlowTypeMethods& flowTypeMethods1, const FlowTypeMethods& flowTypeMethods2)
	{
		if (flowTypeMethods1.typeOfFlowMethods == flowTypeMethods2.typeOfFlowMethods) {return (true);}
		else {return (false);}
	}

};

}

#endif /* FLOWTYPEMETHODS_H_ */
