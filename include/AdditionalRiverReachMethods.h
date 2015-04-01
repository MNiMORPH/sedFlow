/*
 * AdditionalRiverReachMethods.h
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

#ifndef ADDITIONALRIVERREACHMETHODS_H_
#define ADDITIONALRIVERREACHMETHODS_H_

#include <vector>

#include "AdditionalRiverReachMethodType.h"
#include "RegularRiverReachProperties.h"
#include "GeometricalChannelBehaviour.h"
#include "RiverReachProperties.h"
#include "RegularRiverReachMethods.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class AdditionalRiverReachMethods {
private:
	std::vector<AdditionalRiverReachMethodType*> constitutingAdditionalRiverReachMethodTypes;

	static std::vector<AdditionalRiverReachMethodType*> forConstructorsCheckAndCopySingleAdditionalRiverReachMethodTypes (const std::vector<AdditionalRiverReachMethodType*>& singleAdditionalRiverReachMethodTypes);

public:
	AdditionalRiverReachMethods(){}
	AdditionalRiverReachMethods(const std::vector<AdditionalRiverReachMethodType*>& singleAdditionalRiverReachMethodTypes);
	AdditionalRiverReachMethods(const AdditionalRiverReachMethods& toCopy);
	virtual ~AdditionalRiverReachMethods();

	ConstructionVariables createConstructionVariables()const;

	void updateAdditionalRiverReachProperties(const RegularRiverReachProperties& regularRiverReachProperties, const GeometricalChannelBehaviour* geometricalChannelBehaviour, const RegularRiverReachMethods& regularRiverReachMethods);
	void performTypeSpecificActions(RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods);

	AdditionalRiverReachMethods& operator = (const AdditionalRiverReachMethods& newAdditionalRiverReachMethods)
	{
		if (this != &newAdditionalRiverReachMethods) {
			while(!(this->constitutingAdditionalRiverReachMethodTypes.empty()))
			{
				delete this->constitutingAdditionalRiverReachMethodTypes.back();
				this->constitutingAdditionalRiverReachMethodTypes.pop_back();
			}

			this->constitutingAdditionalRiverReachMethodTypes = AdditionalRiverReachMethods::forConstructorsCheckAndCopySingleAdditionalRiverReachMethodTypes( newAdditionalRiverReachMethods.constitutingAdditionalRiverReachMethodTypes );
			}
			return *this;
	}
};

}

#endif /* ADDITIONALRIVERREACHMETHODS_H_ */
