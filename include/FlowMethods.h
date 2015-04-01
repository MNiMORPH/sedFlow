/*
 * FlowMethods.h
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

#ifndef FLOWMETHODS_H_
#define FLOWMETHODS_H_

#include <vector>
#include <utility>
#include <string>

#include "FlowTypeMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class FlowMethods {
private:
	std::vector<FlowTypeMethods*> constitutingFlowMethodsTypes;

	std::vector<FlowTypeMethods*>::iterator getSingleFlowMethodsTypeIterator(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods);
	std::vector<FlowTypeMethods*>::const_iterator getSingleFlowMethodsTypeConstIterator(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods) const;
	std::vector<FlowTypeMethods*>::iterator getSingleFlowMethodsTypeIterator(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods);
	std::vector<FlowTypeMethods*>::const_iterator getSingleFlowMethodsTypeConstIterator(CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods) const;
	std::vector<FlowTypeMethods*>::iterator getBeginFlowMethodsTypeIterator();
	std::vector<FlowTypeMethods*>::iterator getEndFlowMethodsTypeIterator();
	static std::vector<FlowTypeMethods*> forConstructorsCheckAndCopySingleFlowMethodsTypes (const std::vector<FlowTypeMethods*>& singleFlowMethodsTypes);

public:
	//TODO Less Important: Check whether these constructors are really necessary.
	/*
	FlowMethods(const std::vector<CombinerVariables::TypesOfFlowMethods>& typesOfFlowMethods, std::vector<CalcGradient*> gradientCalculationMethods);
	FlowMethods(const std::vector<CombinerVariables::TypesOfFlowMethods>& typesOfFlowMethods, std::vector<CalcGradient*> gradientCalculationMethods, const std::vector< std::vector<double> >& additionalParameters);
	*/
	FlowMethods(); //This default constructor just creates an empty Grains object, which cannot be filled afterwards by public methods.
	FlowMethods(const std::vector<FlowTypeMethods*>& singleFlowMethodsTypes);
	FlowMethods(const FlowMethods& toCopy);
	virtual ~FlowMethods();

	ConstructionVariables createConstructionVariables()const;

	std::vector<FlowTypeMethods*>::const_iterator getBeginFlowMethodsTypeConstIterator() const;
	std::vector<FlowTypeMethods*>::const_iterator getEndFlowMethodsTypeConstIterator() const;

	void calculateChangeRates (RiverReachProperties& riverReachProperties) const;
	void handDownChangeRates (RiverReachProperties& riverReachProperties) const;
	void updateChangeRateDependingParameters(RiverReachProperties& riverReachProperties) const;
	double calculateTimeStep (const RiverSystemProperties& riverSystem) const;
	void calculateChanges (RiverReachProperties& riverReachProperties, double timeStep) const;
	void handDownChanges (RiverReachProperties& riverReachProperties) const;
	void applyChanges (RiverReachProperties& riverReachProperties) const;
	void updateOtherParameters (RiverReachProperties& riverReachProperties) const;
	void handDownOtherParameters (RiverReachProperties& riverReachProperties) const;

	int getNumberOfFlowMethodsTypes() const;
	std::vector<std::string> getTypesOfFlowMethodsAsString() const;
	std::vector<CombinerVariables::TypesOfFlowMethods> getTypesOfFlowMethods() const;
	std::vector<std::string> getTypesOfGeneralFlowMethodsAsString() const;
	std::vector<CombinerVariables::TypesOfGeneralFlowMethods> getTypesOfGeneralFlowMethods() const;
	bool isFirstWaterFlow () const;
	bool matchingTypesOfFlowMethods (const FlowMethods& objectOfComparison) const;

	FlowTypeMethods* createPointerCopyOfSingleFlowTypeMethods(CombinerVariables::TypesOfFlowMethods typeOfFlowMethods) const;

	FlowMethods& operator = (const FlowMethods& newFlowMethods)
	{
		if (this != &newFlowMethods) {
			while(!(this->constitutingFlowMethodsTypes.empty()))
			{
				delete this->constitutingFlowMethodsTypes.back();
				this->constitutingFlowMethodsTypes.pop_back();
			}

			this->constitutingFlowMethodsTypes = FlowMethods::forConstructorsCheckAndCopySingleFlowMethodsTypes( newFlowMethods.constitutingFlowMethodsTypes );
			}
			return *this;
	}

};

}

#endif /* FLOWMETHODS_H_ */
