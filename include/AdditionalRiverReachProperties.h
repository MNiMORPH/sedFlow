/*
 * AdditionalRiverReachProperties.h
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

#ifndef ADDITIONALRIVERREACHPROPERTIES_H_
#define ADDITIONALRIVERREACHPROPERTIES_H_

#include <utility>
#include <map>
#include <vector>

#include "RegularRiverReachProperties.h"
#include "GeometricalChannelBehaviour.h"
#include "AdditionalRiverReachPropertyType.h"
#include "CombinerVariables.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class AdditionalRiverReachProperties {

private:
	std::map<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod,AdditionalRiverReachPropertyType*> mapFromTypeToPointerOfConstitutingProperties;
	std::vector<AdditionalRiverReachPropertyType*> constitutingAdditionalRiverReachPropertyTypes;

	static std::vector<AdditionalRiverReachPropertyType*> forConstructorsCheckAndCopySingleAdditionalRiverReachPropertyTypes (const std::vector<AdditionalRiverReachPropertyType*>& singleAdditionalRiverReachPropertyTypes);

public:
	/*
	AdditionalRiverReachProperties(RegularRiverReachProperties& regularRiverReachProperties, const std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod>& typesOfAdditionalRiverParameters);
	AdditionalRiverReachProperties(RegularRiverReachProperties& regularRiverReachProperties, const std::vector<CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod>& typesOfAdditionalRiverParameters, const std::vector< std::vector<double> >& additionalParameters);
	*/
	AdditionalRiverReachProperties(const std::vector<AdditionalRiverReachPropertyType*>& singleAdditionalRiverReachPropertyTypes);
	AdditionalRiverReachProperties(const AdditionalRiverReachProperties& toCopy);
	virtual ~AdditionalRiverReachProperties();

	ConstructionVariables createConstructionVariables()const;

	AdditionalRiverReachPropertyType* getAdditionalPropertyPointer(CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalProperty);
	const AdditionalRiverReachPropertyType* getAdditionalPropertyConstPointer(CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalProperty) const;

	std::vector< std::pair< CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod , std::vector<double> > > typeSpecificGetter() const;

	AdditionalRiverReachProperties& operator = (const AdditionalRiverReachProperties& toBeAssigned)
	{
		if (this != &toBeAssigned) {
			while(!(this->constitutingAdditionalRiverReachPropertyTypes.empty()))
			{
				delete this->constitutingAdditionalRiverReachPropertyTypes.back();
				this->constitutingAdditionalRiverReachPropertyTypes.pop_back();
			}

			this->constitutingAdditionalRiverReachPropertyTypes = AdditionalRiverReachProperties::forConstructorsCheckAndCopySingleAdditionalRiverReachPropertyTypes( toBeAssigned.constitutingAdditionalRiverReachPropertyTypes );

			this->mapFromTypeToPointerOfConstitutingProperties.clear();
			for(std::vector<AdditionalRiverReachPropertyType*>::const_iterator currentProperty = this->constitutingAdditionalRiverReachPropertyTypes.begin(); currentProperty < this->constitutingAdditionalRiverReachPropertyTypes.end(); ++currentProperty)
			{
				this->mapFromTypeToPointerOfConstitutingProperties[ (*currentProperty)->getTypeOfAdditionalRiverReachPropertyAndMethod() ] = *currentProperty;
			}
		}
		return *this;
	}

};

}

#endif /* ADDITIONALRIVERREACHPROPERTIES_H_ */
