/*
 * SchneiderEtAlBedloadCapacity.cpp
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

#include "SchneiderEtAlBedloadCapacity.h"

#include <math.h>
#include <algorithm>

namespace SedFlow {

double SchneiderEtAlBedloadCapacity_calculateDimlessRefShearStressBasedOnSlopeOnly (const RiverReachProperties& riverReachProperties, const Grains* const erosionBase, const std::vector<double>& fractionalGrainDiameters)
{
	double slope = riverReachProperties.regularRiverReachProperties.bedslope;
	if(slope <= 0) { slope = riverReachProperties.regularRiverReachProperties.sedimentEnergyslope; }
	return ( 0.47 * pow(slope,0.42) );
}

double SchneiderEtAlBedloadCapacity_calculateDimlessRefShearStressBasedOnSlopeAndGSD (const RiverReachProperties& riverReachProperties, const Grains* const erosionBase, const std::vector<double>& fractionalGrainDiameters)
{
	double dEightyfour = erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,84.0);
	double dThirty = erosionBase->getPercentileGrainDiameter(fractionalGrainDiameters,30.0);
	double slope = riverReachProperties.regularRiverReachProperties.bedslope;
	if(slope <= 0) { slope = riverReachProperties.regularRiverReachProperties.sedimentEnergyslope; }
	return ( 0.25 * pow(slope,0.38) * pow((dEightyfour/dThirty),0.31) );
}

double SchneiderEtAlBedloadCapacity::calculateDimensionlessReferenceShearStressForMeanDiameter (const RiverReachProperties& riverReachProperties, const Grains* const erosionBase, const std::vector<double>& fractionalGrainDiameters) const
{
	return (std::max(this->minimumDimensionlessReferenceShearStressForMeanDiameter,(this->calculateDimlessRefShearStress(riverReachProperties,erosionBase,fractionalGrainDiameters))));
}


SchneiderEtAlBedloadCapacity::SchneiderEtAlBedloadCapacity(double minimumDimensionlessReferenceShearStressForMeanDiameter):
	minimumDimensionlessReferenceShearStressForMeanDiameter(minimumDimensionlessReferenceShearStressForMeanDiameter),
	referenceShearStressBasedOnSlopeANDGrainsizeDistribution(false)
{
	this->calculateDimlessRefShearStress = &SchneiderEtAlBedloadCapacity_calculateDimlessRefShearStressBasedOnSlopeOnly;

	this->breakPoint = 1.3;
	this->firstExponentForRelativeShearStress = 13.3;
	this->secondExponentForRelativeShearStress = 0.9;
}

SchneiderEtAlBedloadCapacity::SchneiderEtAlBedloadCapacity(double minimumDimensionlessReferenceShearStressForMeanDiameter, bool referenceShearStressBasedOnSlopeANDGrainsizeDistribution):
	minimumDimensionlessReferenceShearStressForMeanDiameter(minimumDimensionlessReferenceShearStressForMeanDiameter),
	referenceShearStressBasedOnSlopeANDGrainsizeDistribution(referenceShearStressBasedOnSlopeANDGrainsizeDistribution)
{
	if(this->referenceShearStressBasedOnSlopeANDGrainsizeDistribution)
		{this->calculateDimlessRefShearStress = &SchneiderEtAlBedloadCapacity_calculateDimlessRefShearStressBasedOnSlopeAndGSD;}
	else
		{this->calculateDimlessRefShearStress = &SchneiderEtAlBedloadCapacity_calculateDimlessRefShearStressBasedOnSlopeOnly;}

	this->breakPoint = 1.3;
	this->firstExponentForRelativeShearStress = 13.3;
	this->secondExponentForRelativeShearStress = 0.9;
}

SchneiderEtAlBedloadCapacity::~SchneiderEtAlBedloadCapacity() {}

CalcBedloadCapacity* SchneiderEtAlBedloadCapacity::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new SchneiderEtAlBedloadCapacity(this->minimumDimensionlessReferenceShearStressForMeanDiameter,this->referenceShearStressBasedOnSlopeANDGrainsizeDistribution);
	return result;
}


ConstructionVariables SchneiderEtAlBedloadCapacity::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::SchneiderEtAlBedloadCapacity);
	std::vector<bool> boolVector;
	boolVector.push_back(this->referenceShearStressBasedOnSlopeANDGrainsizeDistribution);
	result.labelledBools["referenceShearStressBasedOnSlopeANDGrainsizeDistribution"] = boolVector;
	std::vector<double> doubleVector;
	doubleVector.push_back(this->minimumDimensionlessReferenceShearStressForMeanDiameter);
	result.labelledDoubles["minimumDimensionlessReferenceShearStressForMeanDiameter"] = doubleVector;
	return result;
}


}
