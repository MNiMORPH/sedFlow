/*
 * ChangeRateModifiersForSingleFlowMethod.h
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

#ifndef CHANGERATEMODIFIERSFORSINGLEFLOWMETHOD_H_
#define CHANGERATEMODIFIERSFORSINGLEFLOWMETHOD_H_

#include <vector>

#include "ChangeRateModifiersType.h"
#include "RiverReachProperties.h"
#include "RiverSystemProperties.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class ChangeRateModifiersForSingleFlowMethod {

private:
	CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod;
	std::vector<ChangeRateModifiersType*> constitutingChangeRateModifiersTypes;

	static std::vector<ChangeRateModifiersType*> forConstructorsCheckAndCopySingleChangeRateModifiersTypes (const std::vector<ChangeRateModifiersType*>& singleChangeRateModifiersTypes);

public:
	ChangeRateModifiersForSingleFlowMethod(){}
	ChangeRateModifiersForSingleFlowMethod(const std::vector<ChangeRateModifiersType*>& singleChangeRateModifiersTypes);
	ChangeRateModifiersForSingleFlowMethod(const ChangeRateModifiersForSingleFlowMethod& toCopy);
	virtual ~ChangeRateModifiersForSingleFlowMethod();

	ConstructionVariables createConstructionVariables()const;

	inline std::string getCorrespondingGeneralFlowMethodAsString() const { return CombinerVariables::typeOfGeneralFlowMethodsToString(this->correspondingGeneralFlowMethod); }
	inline CombinerVariables::TypesOfGeneralFlowMethods getCorrespondingGeneralFlowMethod() const { return correspondingGeneralFlowMethod; }

	void inputModification(RiverReachProperties& riverReachProperties);
	void modificationBeforeUpdates(RiverReachProperties& riverReachProperties);
	bool furtherModificationIterationNecessary(const RiverReachProperties& riverReachProperties) const;
	bool furtherModificationIterationNecessary(const RiverSystemProperties& riverSystemProperties) const;
	void finalModification(RiverReachProperties& riverReachProperties);

	ChangeRateModifiersForSingleFlowMethod& operator = (const ChangeRateModifiersForSingleFlowMethod& newChangeRateModifiersForSingleFlowMethod)
	{
		if (this != &newChangeRateModifiersForSingleFlowMethod) {
			this->correspondingGeneralFlowMethod = newChangeRateModifiersForSingleFlowMethod.correspondingGeneralFlowMethod;

			while(!(this->constitutingChangeRateModifiersTypes.empty()))
			{
				delete this->constitutingChangeRateModifiersTypes.back();
				this->constitutingChangeRateModifiersTypes.pop_back();
			}

			this->constitutingChangeRateModifiersTypes = ChangeRateModifiersForSingleFlowMethod::forConstructorsCheckAndCopySingleChangeRateModifiersTypes( newChangeRateModifiersForSingleFlowMethod.constitutingChangeRateModifiersTypes );
			}
			return *this;
	}
};

}

#endif /* CHANGERATEMODIFIERSFORSINGLEFLOWMETHOD_H_ */
