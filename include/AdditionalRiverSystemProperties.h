/*
 * AdditionalRiverSystemProperties.h
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

#ifndef ADDITIONALRIVERSYSTEMPROPERTIES_H_
#define ADDITIONALRIVERSYSTEMPROPERTIES_H_

#include <vector>

#include "AdditionalRiverSystemPropertyType.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class AdditionalRiverSystemProperties {
private:
	std::vector<AdditionalRiverSystemPropertyType*> constitutingAdditionalRiverSystemPropertyTypes;

	static std::vector<AdditionalRiverSystemPropertyType*> forConstructorsCheckAndCopySingleAdditionalRiverSystemPropertyTypes (const std::vector<AdditionalRiverSystemPropertyType*>& singleAdditionalRiverSystemPropertyTypes);

public:
	AdditionalRiverSystemProperties(){}
	AdditionalRiverSystemProperties(const std::vector<AdditionalRiverSystemPropertyType*>& singleAdditionalRiverSystemPropertyTypes);
	AdditionalRiverSystemProperties(const AdditionalRiverSystemProperties& toCopy);
	virtual ~AdditionalRiverSystemProperties();

	ConstructionVariables createConstructionVariables()const;

	std::vector< std::pair< CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod , std::vector<double> > > typeSpecificGetter() const;

	AdditionalRiverSystemProperties& operator = (const AdditionalRiverSystemProperties& toBeAssigned)
	{
		if (this != &toBeAssigned) {
			while(!(this->constitutingAdditionalRiverSystemPropertyTypes.empty()))
			{
				delete this->constitutingAdditionalRiverSystemPropertyTypes.back();
				this->constitutingAdditionalRiverSystemPropertyTypes.pop_back();
			}

			this->constitutingAdditionalRiverSystemPropertyTypes = AdditionalRiverSystemProperties::forConstructorsCheckAndCopySingleAdditionalRiverSystemPropertyTypes( toBeAssigned.constitutingAdditionalRiverSystemPropertyTypes );
		}
		return *this;
	}
};

}

#endif /* ADDITIONALRIVERSYSTEMPROPERTIES_H_ */
