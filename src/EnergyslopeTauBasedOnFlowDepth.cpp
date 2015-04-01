/*
 * EnergyslopeTauBasedOnFlowDepth.cpp
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

#include "EnergyslopeTauBasedOnFlowDepth.h"

#include <sstream>
#include <cstring>

#include "RegularRiverReachProperties.h"
#include "CorrectionForBedloadWeightAtSteepSlopes.h"

namespace SedFlow {

EnergyslopeTauBasedOnFlowDepth::EnergyslopeTauBasedOnFlowDepth(bool correctionForBedloadWeightAtSteepSlopes):
	correctionForBedloadWeightAtSteepSlopes(correctionForBedloadWeightAtSteepSlopes)
{}

EnergyslopeTauBasedOnFlowDepth::~EnergyslopeTauBasedOnFlowDepth() {}

CalcTau* EnergyslopeTauBasedOnFlowDepth::createCalcTauPointerCopy() const
{
	CalcTau* result = new EnergyslopeTauBasedOnFlowDepth(this->correctionForBedloadWeightAtSteepSlopes);
	return result;
}

ConstructionVariables EnergyslopeTauBasedOnFlowDepth::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcTau;
	result.realisationType = CombinerVariables::typeOfTauCalculationMethodToString(CombinerVariables::EnergyslopeTauBasedOnFlowDepth);
	std::vector<bool> boolVector;
	boolVector.push_back(correctionForBedloadWeightAtSteepSlopes);
	result.labelledBools["correctionForBedloadWeightAtSteepSlopes"] = boolVector;
	return result;
}

double EnergyslopeTauBasedOnFlowDepth::calculate (const RiverReachProperties& riverReachProperties, const FlowResistance* flowResistance) const
{
	if(riverReachProperties.regularRiverReachProperties.sedimentEnergyslope < 0.0)
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
		oStringStream << " occurence of negative energyslope for sediment considerations." << std::flush;

		char* tmpChar = new char [(oStringStream.str()).size()+1];
		std::strcpy(tmpChar, (oStringStream.str()).c_str());
		const char *const negativeEnergyslopeErrorMessage = tmpChar;
		throw(negativeEnergyslopeErrorMessage);
	}

	double correctedSedimentEnergySlope = riverReachProperties.regularRiverReachProperties.sedimentEnergyslope;
	if ( correctionForBedloadWeightAtSteepSlopes && (riverReachProperties.regularRiverReachProperties.bedslope > 0.0) )
	{
		correctedSedimentEnergySlope = CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedSedimentEnergySlope(correctedSedimentEnergySlope,(riverReachProperties.getOverallParameters())->getAngleOfReposeInRadians(), riverReachProperties.regularRiverReachProperties.bedslope);
	}

	double meanFlowDepthWithinActiveWidth = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->getMeanFlowDepthForActiveWidth(riverReachProperties.regularRiverReachProperties.maximumWaterdepth, riverReachProperties.regularRiverReachProperties.activeWidth);

	return (meanFlowDepthWithinActiveWidth * (riverReachProperties.getOverallParameters())->getWaterDensity() * (riverReachProperties.getOverallParameters())->getGravityAcceleration() * correctedSedimentEnergySlope );
}

}
