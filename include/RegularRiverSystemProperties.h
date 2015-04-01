/*
 * RegularRiverSystemProperties.h
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

#ifndef REGULARRIVERSYSTEMPROPERTIES_H_
#define REGULARRIVERSYSTEMPROPERTIES_H_

#include <vector>
#include <map>

#include "Grains.h"
#include "RiverReachProperties.h"
#include "OverallParameters.h"
#include "CombinerVariables.h"
#include "ConstructionVariables.h"
#include "CellIDConversions.h"

namespace SedFlow {

class RegularRiverSystemProperties {
private:
	std::map<int,int> mapFromRealCellIDtoUserCellID;
public:
	RegularRiverSystemProperties(){}
	RegularRiverSystemProperties(const std::vector<RiverReachProperties>& cellProperties);
	RegularRiverSystemProperties(const RegularRiverSystemProperties& toCopy);
	virtual ~RegularRiverSystemProperties(){}

	ConstructionVariables createConstructionVariables()const;

	int numberOfCells;
	std::vector<RiverReachProperties> cellProperties;

	void setRegularCellParameters (const RegularRiverReachProperties& newParameters, int cellID);
	std::vector<double> extractSingleRegularParameter (CombinerVariables::TypesOfRegularRiverReachProperties parameter) const;
	void setSingleRegularParameter (CombinerVariables::TypesOfRegularRiverReachProperties parameter, const std::vector<double>& newValues);

	RiverReachProperties* getReachPropertiesPointerCorrespondingToUserCellID (int userCellID);
	const RiverReachProperties* getReachPropertiesConstPointerCorrespondingToUserCellID (int userCellID) const;
	int getRealCellIDcorrespondingToUserCellID (int userCellID) const { return CellIDConversions::getRealCellIDcorrespondingToUserCellID(userCellID,mapFromRealCellIDtoUserCellID); }
	int getUserCellIDcorrespondingToRealCellID (int realCellID) const { return CellIDConversions::getUserCellIDcorrespondingToRealCellID(realCellID,mapFromRealCellIDtoUserCellID); }
};

}

#endif /* REGULARRIVERSYSTEMPROPERTIES_H_ */
