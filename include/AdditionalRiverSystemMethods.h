/*
 * AdditionalRiverSystemMethods.h
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

#ifndef ADDITIONALRIVERSYSTEMMETHODS_H_
#define ADDITIONALRIVERSYSTEMMETHODS_H_

#include "AdditionalRiverSystemMethodType.h"
#include "RiverSystemProperties.h"
#include "RegularRiverSystemMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class AdditionalRiverSystemMethods {
private:
	std::vector<AdditionalRiverSystemMethodType*> constitutingAdditionalRiverSystemMethodTypes;

	static std::vector<AdditionalRiverSystemMethodType*> forConstructorsCheckAndCopySingleAdditionalRiverSystemMethodTypes (const std::vector<AdditionalRiverSystemMethodType*>& singleAdditionalRiverSystemMethodTypes);

public:
	AdditionalRiverSystemMethods(){}
	AdditionalRiverSystemMethods(const std::vector<AdditionalRiverSystemMethodType*>& constitutingAdditionalRiverSystemMethodTypes);
	AdditionalRiverSystemMethods(const AdditionalRiverSystemMethods& toCopy);
	virtual ~AdditionalRiverSystemMethods();

	ConstructionVariables createConstructionVariables()const;

	void updateAdditionalRiverSystemProperties(const RegularRiverSystemProperties& regularRiverSystemProperties, const OverallParameters& overallParameters, const RegularRiverSystemMethods& regularRiverSystemMethods);
	void performAdditionalRiverSystemActions(RiverSystemProperties& riverSystemProperties, const RegularRiverSystemMethods& regularRiverSystemMethods);

	AdditionalRiverSystemMethods& operator = (const AdditionalRiverSystemMethods& newAdditionalRiverSystemMethods)
	{
		if (this != &newAdditionalRiverSystemMethods) {
			while(!(this->constitutingAdditionalRiverSystemMethodTypes.empty()))
			{
				delete this->constitutingAdditionalRiverSystemMethodTypes.back();
				this->constitutingAdditionalRiverSystemMethodTypes.pop_back();
			}

			this->constitutingAdditionalRiverSystemMethodTypes = AdditionalRiverSystemMethods::forConstructorsCheckAndCopySingleAdditionalRiverSystemMethodTypes( newAdditionalRiverSystemMethods.constitutingAdditionalRiverSystemMethodTypes );
			}
			return *this;
	}

};

}

#endif /* ADDITIONALRIVERSYSTEMMETHODS_H_ */
