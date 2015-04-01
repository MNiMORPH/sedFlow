/*
 * BedrockRoughnessContribution.cpp
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

#include "BedrockRoughnessContribution.h"

namespace SedFlow {

double BedrockRoughnessContribution::getBedrockRoughnessContribution(const RiverReachProperties& riverReachProperties)
{
	double contribution = 0.0;
	if( riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.size() == 1 || (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(1)).getOverallVolume() < 0.0000001 ) //i.e. There is definitely not more than the active layer left.
	{
		double activeLayerRelativeThickness = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getOverallVolume() / riverReachProperties.strataSorting->getLayerThickness(riverReachProperties.regularRiverReachProperties);
		if( activeLayerRelativeThickness > 1.0) {activeLayerRelativeThickness = 1.0;}
		contribution = 1.0 - activeLayerRelativeThickness;
	}
	return contribution;
}

double BedrockRoughnessContribution::getBedrockRoughnessEquivalentRepresentativeGrainDiameterValue(const RiverReachProperties& riverReachProperties)
{
	double bedrockRoughnessEquivalentRepresentativeGrainDiameterValue;

	const AdditionalRiverReachPropertyType* bedrockRoughnessEquivalentRepresentativeGrainDiameter = riverReachProperties.additionalRiverReachProperties.getAdditionalPropertyConstPointer(CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter);
	if(bedrockRoughnessEquivalentRepresentativeGrainDiameter)
	{
		#if defined _DEBUG || defined DEBUG
		if((bedrockRoughnessEquivalentRepresentativeGrainDiameter->typeSpecificGetter()).first != CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter)
		{
			const char *const typeMismatchErrorMessage = "Other than requested type of AdditionalRiverReachPropertyType has been returned.";
			throw(typeMismatchErrorMessage);
		}
		#endif
		bedrockRoughnessEquivalentRepresentativeGrainDiameterValue = ((bedrockRoughnessEquivalentRepresentativeGrainDiameter->typeSpecificGetter()).second).at(0);
	}
	else
	{
		const char *const bedrockRoughnessEquivalentRepresentativeGrainDiameterErrorMessage = "The AdditionalRiverReachPropertyType BedrockRoughnessEquivalentRepresentativeGrainDiameter is needed for the current simulation setup.";
		throw(bedrockRoughnessEquivalentRepresentativeGrainDiameterErrorMessage);
	}
	return bedrockRoughnessEquivalentRepresentativeGrainDiameterValue;
}

}
