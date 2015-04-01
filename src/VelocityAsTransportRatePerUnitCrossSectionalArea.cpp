/*
 * VelocityAsTransportRatePerUnitCrossSectionalArea.cpp
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

#include "VelocityAsTransportRatePerUnitCrossSectionalArea.h"

namespace SedFlow {

VelocityAsTransportRatePerUnitCrossSectionalArea::VelocityAsTransportRatePerUnitCrossSectionalArea(const EstimateThicknessOfMovingSedimentLayer* estimateThicknessOfMovingSedimentLayer):
	estimateThicknessOfMovingSedimentLayer( estimateThicknessOfMovingSedimentLayer->createEstimateThicknessOfMovingSedimentLayerPointerCopy() )
{}

VelocityAsTransportRatePerUnitCrossSectionalArea::VelocityAsTransportRatePerUnitCrossSectionalArea(const EstimateThicknessOfMovingSedimentLayer* estimateThicknessOfMovingSedimentLayer, const std::map<int,double>& mapFromCellIDToPreviousErosionRate):
	estimateThicknessOfMovingSedimentLayer( estimateThicknessOfMovingSedimentLayer->createEstimateThicknessOfMovingSedimentLayerPointerCopy() ),
	mapFromCellIDToPreviousErosionRate(mapFromCellIDToPreviousErosionRate)
{}

VelocityAsTransportRatePerUnitCrossSectionalArea::~VelocityAsTransportRatePerUnitCrossSectionalArea()
{
	delete estimateThicknessOfMovingSedimentLayer;
}


CalcBedloadVelocity* VelocityAsTransportRatePerUnitCrossSectionalArea::createCalcBedloadVelocityPointerCopy() const
{
	CalcBedloadVelocity* result = new VelocityAsTransportRatePerUnitCrossSectionalArea(this->estimateThicknessOfMovingSedimentLayer,this->mapFromCellIDToPreviousErosionRate);
	return result;
}

ConstructionVariables VelocityAsTransportRatePerUnitCrossSectionalArea::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadVelocity;
	result.realisationType = CombinerVariables::typeOfBedloadVelocityCalculationMethodToString(CombinerVariables::VelocityAsTransportRatePerUnitCrossSectionalArea);
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( estimateThicknessOfMovingSedimentLayer->createConstructionVariables() );
	result.labelledObjects["estimateThicknessOfMovingSedimentLayer"] = constructionVariablesVector;
	return result;
}

double VelocityAsTransportRatePerUnitCrossSectionalArea::calculate (const RiverReachProperties& riverReachProperties) const
{
	double currentErosionRateOverallVolume = riverReachProperties.regularRiverReachProperties.erosionRate.getOverallVolume();
	std::map<int,double>::const_iterator previousErosionRateIterator = mapFromCellIDToPreviousErosionRate.find(riverReachProperties.getCellID());
	if( previousErosionRateIterator != mapFromCellIDToPreviousErosionRate.end() )
	{
		double previousErosionRate = previousErosionRateIterator->second;
		if(previousErosionRate > 0.0)
		{
			double ratioOfSupplyToCapacity = riverReachProperties.regularRiverReachProperties.erosion.getOverallVolume() / (previousErosionRate * ((riverReachProperties.getOverallParameters())->getCurrentTimeStepLengthInSeconds()) );
			currentErosionRateOverallVolume *= ratioOfSupplyToCapacity;
		}
	}
	const_cast<VelocityAsTransportRatePerUnitCrossSectionalArea*>(this)->mapFromCellIDToPreviousErosionRate[riverReachProperties.getCellID()] = riverReachProperties.regularRiverReachProperties.erosionRate.getOverallVolume();
	return ( currentErosionRateOverallVolume / ( riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertActiveWidthIntoActivePerimeter(riverReachProperties.regularRiverReachProperties.activeWidth) * estimateThicknessOfMovingSedimentLayer->estimate(riverReachProperties)) );
}

}
