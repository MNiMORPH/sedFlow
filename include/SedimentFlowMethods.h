/*
 * SedimentFlowMethods.h
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

#ifndef SEDIMENTFLOWMETHODS_H_
#define SEDIMENTFLOWMETHODS_H_

#include <vector>
#include <utility>
#include <string>

#include "FlowTypeMethods.h"

#include "SedimentFlowTypeMethods.h"
#include "BedloadFlowMethods.h"
#include "SuspensionLoadFlowMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class SedimentFlowMethods: public FlowTypeMethods {
private:
	std::vector<SedimentFlowTypeMethods*> constitutingSedimentFlowMethodsTypes;

	std::vector<SedimentFlowTypeMethods*>::iterator getSingleSedimentFlowMethodsTypeIterator(CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods);
	std::vector<SedimentFlowTypeMethods*>::const_iterator getSingleSedimentFlowMethodsTypeConstIterator(CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods) const;
	std::vector<SedimentFlowTypeMethods*>::iterator getBeginSedimentFlowMethodsTypeIterator();
	std::vector<SedimentFlowTypeMethods*>::const_iterator getBeginSedimentFlowMethodsTypeConstIterator() const;
	std::vector<SedimentFlowTypeMethods*>::iterator getEndSedimentFlowMethodsTypeIterator();
	std::vector<SedimentFlowTypeMethods*>::const_iterator getEndSedimentFlowMethodsTypeConstIterator() const;

	static std::vector<SedimentFlowTypeMethods*> forConstructorsCheckAndCopySingleSedimentFlowMethodsTypes(const std::vector<SedimentFlowTypeMethods*>& singleSedimentFlowMethodsTypes);

	bool updateErosionRatesInMarginCells;

public:
	//TODO Less Important: Check whether these constructors are really necessary.
	/*
	SedimentFlowMethods(const std::vector<CombinerVariables::TypesOfFlowMethods>& typesOfSedimentFlowMethods, CalcGradient bedSlopeCalculationMethod);
	SedimentFlowMethods(const std::vector<CombinerVariables::TypesOfFlowMethods>& typesOfSedimentFlowMethods, CalcGradient bedSlopeCalculationMethod, const std::vector<double>& additionalParameters);
	*/

	SedimentFlowMethods();
	SedimentFlowMethods(const std::vector<SedimentFlowTypeMethods*>& singleSedimentFlowMethodsTypes, bool updateErosionRatesInMarginCells);
	SedimentFlowMethods(const FlowTypeMethods* toCopy);
	SedimentFlowMethods(const SedimentFlowMethods& toCopy);
	virtual ~SedimentFlowMethods();

	virtual FlowTypeMethods* createFlowTypeMethodsPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void calculateChangeRate (RiverReachProperties& riverReachProperties) const;
	void handDownChangeRate (RiverReachProperties& riverReachProperties) const; //Only the first method is applied.
	void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const; //Only the first method is applied.
	double calculateTimeStep (const RiverSystemProperties& riverSystem) const; //TODO Less Important: Check whether it would be enough to apply only the first method.
	void calculateChange (RiverReachProperties& riverReachProperties, double timeStep) const; //Only the first method is applied.
	void handDownChange (RiverReachProperties& riverReachProperties) const; //Only the first method is applied.
	void applyChange (RiverReachProperties& riverReachProperties) const; //Only the first method is applied.
	void updateOtherParameters (RiverReachProperties& riverReachProperties) const; //Only the first method is applied.
	void handDownOtherParameters (RiverReachProperties& riverReachProperties) const; //Only the first method is applied.

	std::string getTypeOfFlowMethodsAsString() const; //Pre-Implemented in FlowTypeMethod
	CombinerVariables::TypesOfFlowMethods getTypeOfFlowMethods() const; //Pre-Implemented in FlowTypeMethod

	int getNumberOfSedimentFlowMethodsTypes() const;
	std::vector<std::string> getTypesOfSedimentFlowMethodsAsString() const;
	std::vector<CombinerVariables::TypesOfSedimentFlowMethods> getTypesOfSedimentFlowMethods() const;
	bool matchingTypesOfSedimentFlowMethods (const SedimentFlowMethods& objectOfComparison) const;

	SedimentFlowTypeMethods* createPointerCopyOfSingleSedimentFlowTypeMethods(CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods) const;

	SedimentFlowMethods& operator = (const SedimentFlowMethods& newSedimentFlowMethods)
		{
			if (this != &newSedimentFlowMethods) {
				while(!(this->constitutingSedimentFlowMethodsTypes.empty()))
				{
					delete this->constitutingSedimentFlowMethodsTypes.back();
					this->constitutingSedimentFlowMethodsTypes.pop_back();
				}

				this->constitutingSedimentFlowMethodsTypes = SedimentFlowMethods::forConstructorsCheckAndCopySingleSedimentFlowMethodsTypes( newSedimentFlowMethods.constitutingSedimentFlowMethodsTypes );
			    }
			    return *this;
		}

};

}

#endif /* SEDIMENTFLOWMETHODS_H_ */
