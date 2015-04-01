/*
 * SedimentFlowTypeMethods.h
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

#ifndef SEDIMENTFLOWTYPEMETHODS_H_
#define SEDIMENTFLOWTYPEMETHODS_H_

#include <utility>
#include <string>

#include "CalcGradient.h"
#include "RegularRiverReachProperties.h"
#include "RiverSystemProperties.h"
#include "CombinerVariables.h"
#include "OverallMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class SedimentFlowTypeMethods {
protected:
	CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods;
	double maximumFractionOfActiveLayerToBeEroded;
	bool preventZeroOrNegativeBedSlopes;
	double maximumRelativeTwoCellBedSlopeChange;
	const OverallMethods& overallMethods;

public:
	virtual SedimentFlowTypeMethods* createSedimentFlowTypeMethodsPointerCopy() const = 0;//This method HAS TO BE implemented.

	// In each implementation at least one of the following constructors should be implemented:
	//SpecificFlowTypeMethod(const CalcGradient& bedSlopeCalculationMethod);
	//SpecificFlowTypeMethod(const CalcGradient& bedSlopeCalculationMethod, const std::vector<double>& additionalParameters);
	SedimentFlowTypeMethods(double maximumFractionOfActiveLayerToBeEroded, bool preventZeroOrNegativeBedSlopes, double maximumRelativeTwoCellBedSlopeChange, const OverallMethods& overallMethods);
	virtual ~SedimentFlowTypeMethods(){}

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual void calculateChangeRate (RiverReachProperties& riverReachProperties) const = 0;
	virtual void handDownChangeRate (RiverReachProperties& riverReachProperties) const; //Pre-Implemented
	virtual void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const; //Pre-Implemented
	virtual double calculateTimeStep (const RiverSystemProperties& riverSystem) const; //Pre-Implemented
	virtual void calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const; //Pre-Implemented
	virtual void handDownChange (RiverReachProperties& riverReachProperties) const; //Pre-Implemented
	virtual void applyChange (RiverReachProperties& riverReachProperties) const; //Pre-Implemented
	virtual void updateOtherParameters (RiverReachProperties& riverReachProperties) const; //Pre-Implemented
	virtual void handDownOtherParameters (RiverReachProperties& riverReachProperties) const; //Pre-Implemented

	virtual std::string getTypeOfSedimentFlowMethodsAsString() const; //Pre-Implemented
	virtual CombinerVariables::TypesOfSedimentFlowMethods getTypeOfSedimentFlowMethods() const; //Pre-Implemented

	friend bool operator < (const SedimentFlowTypeMethods& sedimentFlowTypeMethods1, const SedimentFlowTypeMethods& sedimentFlowTypeMethods2)
	{
		if (sedimentFlowTypeMethods1.typeOfSedimentFlowMethods < sedimentFlowTypeMethods2.typeOfSedimentFlowMethods) {return (true);}
		else {return (false);}
	}

	friend bool operator == (const SedimentFlowTypeMethods& sedimentFlowTypeMethods1, const SedimentFlowTypeMethods& sedimentFlowTypeMethods2)
	{
		if (sedimentFlowTypeMethods1.typeOfSedimentFlowMethods == sedimentFlowTypeMethods2.typeOfSedimentFlowMethods) {return (true);}
		else {return (false);}
	}

};

}

#endif /* SEDIMENTFLOWTYPEMETHODS_H_ */
