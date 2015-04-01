/*
 * PreventLocalGrainSizeDistributionChanges.cpp
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

#include "PreventLocalGrainSizeDistributionChanges.h"

namespace SedFlow {

PreventLocalGrainSizeDistributionChanges::PreventLocalGrainSizeDistributionChanges()
{
	this->typeOfAdditionalRiverPropertyAndMethod = CombinerVariables::PreventLocalGrainSizeDistributionChanges;
}

PreventLocalGrainSizeDistributionChanges::~PreventLocalGrainSizeDistributionChanges(){}

ConstructionVariables PreventLocalGrainSizeDistributionChanges::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
	result.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::PreventLocalGrainSizeDistributionChanges);
	return result;
}

AdditionalRiverReachMethodType* PreventLocalGrainSizeDistributionChanges::createAdditionalRiverReachMethodTypeCopy() const
{
	AdditionalRiverReachMethodType* result = new PreventLocalGrainSizeDistributionChanges();
	return result;
}

void PreventLocalGrainSizeDistributionChanges::typeSpecificAction (RiverReachProperties& riverReachProperties, const RegularRiverReachMethods& regularRiverReachMethods)
{
	double activeLayerOverallVolume = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getOverallVolume();
	if( (!(riverReachProperties.isUpstreamMargin())) && (activeLayerOverallVolume < 0.0) )
	{
		riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0) * riverReachProperties.regularRiverReachProperties.depositionPerUnitBedSurface.getOverallVolume() / activeLayerOverallVolume;
		// The following line is not necessary as all ...NonFractional capacity calculation methods already use the local grain size distribution for calculating erosion.
		//riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0) * riverReachProperties.regularRiverReachProperties.erosionPerUnitBedSurface.getOverallVolume() / activeLayerOverallVolume;
	}
}

}
