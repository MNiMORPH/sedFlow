/*
 * ReducedWaterEnergyslope.cpp
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

#include "ReducedWaterEnergyslope.h"

#include <math.h>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <cstring>

namespace SedFlow {

ReducedWaterEnergyslope::ReducedWaterEnergyslope(double stressPartitioningExponent, bool calculationBasedOnqInsteadOfh, double maximumFroudeNumber, bool ensureMinimumInputSlope, double minimumInputSlope):
	stressPartitioningExponent(stressPartitioningExponent),
	calculationBasedOnqInsteadOfh(calculationBasedOnqInsteadOfh),
	maximumFroudeNumber(maximumFroudeNumber),
	ensureMinimumInputSlope(ensureMinimumInputSlope),
	minimumInputSlope(minimumInputSlope)
{
	this->typeOfGradientCalculationMethod = CombinerVariables::ReducedWaterEnergyslope;
	this->isDependentOnBedslope = false;
	this->isDependentOnWaterEnergyslope = true;
	if( this->ensureMinimumInputSlope && ( this->minimumInputSlope <= 0.0 ) )
	{
			const char *const negativeMinimumInputSlopeErrorMessage = "ReducedWaterEnergyslope requires a positive minimumInputSlope.";
			throw(negativeMinimumInputSlopeErrorMessage);
	}

	if( this->maximumFroudeNumber < 0.5 )
	{
				const char *const maximumFroudeNumberErrorMessage = "The maximum Froude number cannot be smaller than 0.5. (ReducedWaterEnergyslope)";
				throw(maximumFroudeNumberErrorMessage);
	}
}

ReducedWaterEnergyslope::~ReducedWaterEnergyslope() {}

CalcGradient* ReducedWaterEnergyslope::createCalcGradientMethodPointerCopy() const
{
	CalcGradient* result = new ReducedWaterEnergyslope(this->stressPartitioningExponent,this->calculationBasedOnqInsteadOfh, this->maximumFroudeNumber,this->ensureMinimumInputSlope,this->minimumInputSlope);
	return result;
}

ConstructionVariables ReducedWaterEnergyslope::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReducedWaterEnergyslope);
	std::vector<double> doubleVector;
	doubleVector.push_back(stressPartitioningExponent);
	result.labelledDoubles["stressPartitioningExponent"] = doubleVector;
	std::vector<bool> boolVector;
	boolVector.push_back(calculationBasedOnqInsteadOfh);
	result.labelledBools["calculationBasedOnqInsteadOfh"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(maximumFroudeNumber);
	result.labelledDoubles["maximumFroudeNumber"] = doubleVector;
	boolVector.clear();
	boolVector.push_back(ensureMinimumInputSlope);
	result.labelledBools["ensureMinimumInputSlope"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(minimumInputSlope);
	result.labelledDoubles["minimumInputSlope"] = doubleVector;
	return result;
}

double ReducedWaterEnergyslope::convertSimpleToReducedSlope (double simpleSlope, const RiverReachProperties& riverReachProperties, double stressPartitioningExponent, double maximumFroudeNumber, bool calculationBasedOnqInsteadOfh) const
{
	// The following line calculates flow depth of a virtual rectangular channel corresponding to the mean flow depth within the active width.
	double flowDepth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->getMeanFlowDepthForActiveWidth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth, riverReachProperties.regularRiverReachProperties.activeWidth);
	return ( convertSimpleToReducedSlope(simpleSlope,flowDepth,riverReachProperties,stressPartitioningExponent,maximumFroudeNumber,calculationBasedOnqInsteadOfh) );
}

double ReducedWaterEnergyslope::convertSimpleToReducedSlope (double simpleSlope, double flowDepth, const RiverReachProperties& riverReachProperties, double stressPartitioningExponent, double maximumFroudeNumber, bool calculationBasedOnqInsteadOfh) const
{
	double result = simpleSlope;
	if( !(riverReachProperties.isDownstreamMargin()) )
	{
		if(ensureMinimumInputSlope)
		{
			simpleSlope = std::max(simpleSlope,this->minimumInputSlope);
		}
		else
		{
			if(simpleSlope < 0.0)
			{
				std::ostringstream oStringStream;
				oStringStream << "At ";
				if(riverReachProperties.isMargin())
				{
					if(riverReachProperties.isDownstreamMargin())
					{
						oStringStream << "downstream margin";
					}
					else
					{
						oStringStream << "upstream margin at reach " << riverReachProperties.getDownstreamUserCellID();
					}
				}
				else
				{
					oStringStream << "reach " << riverReachProperties.getUserCellID();
				}
				oStringStream << " occurence of negative water energyslope. (ReducedWaterEnergyslope)" << std::flush;

				char* tmpChar = new char [(oStringStream.str()).size()+1];
				std::strcpy(tmpChar, (oStringStream.str()).c_str());
				const char *const negativeWaterEnergyslopeErrorMessage = tmpChar;
				throw(negativeWaterEnergyslopeErrorMessage);
			}
		}

		double dEightyfour = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(),84.0);

		if( riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.size() == 1 || (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(1)).getOverallVolume() < 0.0000001 ) //i.e. There is definitely not more than the active layer left.
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
				const char *const bedrockRoughnessEquivalentRepresentativeGrainDiameterErrorMessage = "The AdditionalRiverReachPropertyType BedrockRoughnessEquivalentRepresentativeGrainDiameter is needed for ReducedWaterEnergyslope.";
				throw(bedrockRoughnessEquivalentRepresentativeGrainDiameterErrorMessage);
			}

			if( dEightyfour != dEightyfour ) //i.e. The river is running over bedrock and there are no grains left to define the dEightyfour.
			{
				dEightyfour = bedrockRoughnessEquivalentRepresentativeGrainDiameterValue;
			}
			else //i.e. Transition with beginning bedrock exposure.
			{
				double activeLayerRelativeThickness = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getOverallVolume() / riverReachProperties.strataSorting->getLayerThickness(riverReachProperties.regularRiverReachProperties);
				if( activeLayerRelativeThickness > 1.0) {activeLayerRelativeThickness = 1.0;}
				dEightyfour = bedrockRoughnessEquivalentRepresentativeGrainDiameterValue + ( activeLayerRelativeThickness * (dEightyfour - bedrockRoughnessEquivalentRepresentativeGrainDiameterValue) );
			}

		}

		// The following line calculates flow width of a virtual rectangular channel corresponding to the mean flow depth within the active width.
		double flowWidth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(riverReachProperties.regularRiverReachProperties.maximumWaterdepth) / flowDepth;

		// All cited equations correspond to the findings of Rickenmann & Recking (2011).Evaluation of flow resistance in gravel-bed rivers through a large field data set. Water Ressources Research. vol. 47.
		// Analytic combinations of single equations are done for performance reasons.
		double flowResistance; //i.e. (f_0/f_tot)^0.5
		double Utot, Uo;
		double gravityAcceleration = (riverReachProperties.getOverallParameters())->getGravityAcceleration();
		double dischargePerUnitFlowWidth = riverReachProperties.regularRiverReachProperties.discharge / flowWidth;
		double UtotFroude = pow((maximumFroudeNumber * maximumFroudeNumber * gravityAcceleration * dischargePerUnitFlowWidth),(1.0/3.0));
		if(calculationBasedOnqInsteadOfh)
		{
			// The following line represents equation (11) in Rickenmann Recking (2011) (The triple multiplication was chosen to avoid the performance problems of math::pow)
			double dimensionlessDischarge = dischargePerUnitFlowWidth / sqrt( gravityAcceleration * simpleSlope * dEightyfour * dEightyfour * dEightyfour );
			double rootExpression = sqrt( (gravityAcceleration * simpleSlope * dEightyfour) );
			// If one combines equation (12) + (22) in Rickenmann Recking (2011), one would end up with the following line:
			Utot = 1.443 * rootExpression * pow(dimensionlessDischarge,0.60) * pow(( 1.0 + pow((dimensionlessDischarge / 43.78),0.8214) ),(-0.2435));
			Utot = std::min(Utot,UtotFroude);
			// If one combines equation (12) + (20a) in Rickenmann Recking (2011), one would end up with the following line:
			Uo = 3.074 * rootExpression * pow(dimensionlessDischarge,0.40);
			// The following line represents equation (27a) in Rickenmann Recking (2011).
			flowResistance = pow((Utot / Uo),1.5);
			// If Froude condition would be neglected and if one combines equation (12) + (22) + (20a) + (27a) in Rickenmann Recking (2011), one would end up with the following line:
			//flowResistance = 0.3216205 * pow(dimensionlessDischarge,0.3) * pow( ( 1 + pow( (dimensionlessDischarge/43.78) ,0.8214) ) , -0.36525 );
		}
		else //i.e. calculation based on h instead of q
		{
			double relativeFlowDepth = flowDepth / dEightyfour;
			double rootExpression = sqrt( (gravityAcceleration * flowDepth * simpleSlope) );
			// If one combines equation (2) + (u*=(gdS)^0.5) + (10a) + (a1=6.5) + (a2=2.5) in Rickenmann Recking (2011), one would end up with the following line:
			Utot = rootExpression * 16.25 * relativeFlowDepth / sqrt( (42.25 + (6.25*pow(relativeFlowDepth,(5.0/3.0)))) );
			Utot = std::min(Utot,UtotFroude);
			// If one combines equation (2) + (v*=u*=(gdS)^0.5) + (20b) in Rickenmann Recking (2011), one would end up with the following line:
			Uo = 6.5 * rootExpression * pow(relativeFlowDepth,(1.0/6.0));
			// The following line represents equation (27b) in Rickenmann Recking (2011).
			flowResistance = Utot / Uo;
			// If Froude condition would be neglected and if one combines equation (2) + (u*=v*) + (10a) + (20b) + (27b) in Rickenmann Recking (2011), one would end up with the following line:
			//flowResistance = 2.5 * pow(relativeFlowDepth,(5.0/6.0)) / sqrt( (42.25 + (6.25*pow(relativeFlowDepth,(5.0/3.0)))) );
		}

		result = simpleSlope * pow(flowResistance, stressPartitioningExponent);
	}
	return result;
}

std::vector<double> ReducedWaterEnergyslope::convertSimpleToReducedSlope (const std::vector<double>& simpleSlopes, const RiverSystemProperties& river, double stressPartitioningExponent, double maximumFroudeNumber, bool calculationBasedOnqInsteadOfh) const
{
	if(simpleSlopes.size() != river.regularRiverSystemProperties.numberOfCells)
	{
		const char *const errorMessage = "Action on not matching number of river cells";
		throw(errorMessage);
	}
	std::vector<double> result;
	result.reserve( simpleSlopes.size() );
	int currentCellID = 0;
	for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = river.regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < river.regularRiverSystemProperties.cellProperties.end(); ++currentCellID, ++currentRiverReachProperties)
	{
		result.push_back( this->convertSimpleToReducedSlope(simpleSlopes.at(currentCellID), (*currentRiverReachProperties),stressPartitioningExponent,maximumFroudeNumber,calculationBasedOnqInsteadOfh) );
	}
	return result;
}

double ReducedWaterEnergyslope::convertSimpleToReducedSlope (double simpleSlope, const RiverReachProperties& riverReachProperties) const
{
	return this->convertSimpleToReducedSlope(simpleSlope, riverReachProperties, stressPartitioningExponent,maximumFroudeNumber,calculationBasedOnqInsteadOfh);
}

double ReducedWaterEnergyslope::convertSimpleToReducedSlope (double simpleSlope, double alternativeFlowDepth, const RiverReachProperties& riverReachProperties, bool calculationBasedOnqInsteadOfh) const
{
	return this->convertSimpleToReducedSlope(simpleSlope, alternativeFlowDepth, riverReachProperties, stressPartitioningExponent,maximumFroudeNumber,calculationBasedOnqInsteadOfh);
}

std::vector<double> ReducedWaterEnergyslope::convertSimpleToReducedSlope (const std::vector<double>& simpleSlopes, const RiverSystemProperties& river) const
{
	return this->convertSimpleToReducedSlope(simpleSlopes, river, stressPartitioningExponent,maximumFroudeNumber,calculationBasedOnqInsteadOfh);
}


double ReducedWaterEnergyslope::calculate (const RiverReachProperties& riverReachProperties) const
{
	return this->convertSimpleToReducedSlope(riverReachProperties.regularRiverReachProperties.waterEnergyslope,riverReachProperties);
}

std::pair<double,double> ReducedWaterEnergyslope::calculateReducedAndUnreducedSedimentEnergyslope (const RiverReachProperties& riverReachProperties) const
{
	return std::pair<double,double> (this->convertSimpleToReducedSlope(riverReachProperties.regularRiverReachProperties.waterEnergyslope,riverReachProperties),riverReachProperties.regularRiverReachProperties.waterEnergyslope);
}


double ReducedWaterEnergyslope::calculate (const RiverSystemProperties& river, int cellID) const
{
	return this->convertSimpleToReducedSlope( (river.regularRiverSystemProperties.cellProperties.at(cellID)).regularRiverReachProperties.waterEnergyslope, (river.regularRiverSystemProperties.cellProperties.at(cellID)) );
}

std::pair<double,double> ReducedWaterEnergyslope::calculateReducedAndUnreducedSedimentEnergyslope (const RiverSystemProperties& river, int cellID) const
{
	return std::pair<double,double> (this->convertSimpleToReducedSlope( (river.regularRiverSystemProperties.cellProperties.at(cellID)).regularRiverReachProperties.waterEnergyslope, (river.regularRiverSystemProperties.cellProperties.at(cellID)) ), (river.regularRiverSystemProperties.cellProperties.at(cellID)).regularRiverReachProperties.waterEnergyslope);
}

std::vector<double> ReducedWaterEnergyslope::calculate (const RiverSystemProperties& river) const
{
	std::vector<double> waterEnergyslopes = river.regularRiverSystemProperties.extractSingleRegularParameter(CombinerVariables::waterEnergyslope);
	return this->convertSimpleToReducedSlope(waterEnergyslopes, river);
}

std::pair< std::vector<double>, std::vector<double> > ReducedWaterEnergyslope::calculateReducedAndUnreducedSedimentEnergyslope (const RiverSystemProperties& river) const
{
	std::vector<double> waterEnergyslopes = river.regularRiverSystemProperties.extractSingleRegularParameter(CombinerVariables::waterEnergyslope);
	return std::pair< std::vector<double>, std::vector<double> >(this->convertSimpleToReducedSlope(waterEnergyslopes, river),waterEnergyslopes);
}

double ReducedWaterEnergyslope::calculate (const std::vector<double>& heights, const std::vector<double>& lengths, int cellID) const
{
	const char *const errorMessage = "The method ReducedWaterEnergyslope::calculate is not defined for input without river parameters";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

std::vector<double> ReducedWaterEnergyslope::calculate (const std::vector<double>& heights, const std::vector<double>& lengths) const
{
	const char *const errorMessage = "The method ReducedWaterEnergyslope::calculate is not defined for input without river parameters";
	throw(errorMessage);
	std::vector<double> fakeResult;
	return fakeResult;
}

}
