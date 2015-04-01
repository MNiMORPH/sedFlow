/*
 * ReckingBedloadCapacityNonFractional.cpp
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

#include "ReckingBedloadCapacityNonFractional.h"

#include <math.h>
#include <sstream>
#include <cstring>

//This class is based on:
//			Recking(2010): A comparison between flume and field bed load transport data and consequences for surface-based bed load transport prediction. WATER RESOURCES RESEARCH, VOL. 46, W03518, doi:10.1029/2009WR008007

namespace SedFlow {

CalcBedloadCapacity* ReckingBedloadCapacityNonFractional::createCalcBedloadCapacityMethodPointerCopy() const
{
	CalcBedloadCapacity* result = new ReckingBedloadCapacityNonFractional();
	return result;
}

ConstructionVariables ReckingBedloadCapacityNonFractional::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;
	result.realisationType = CombinerVariables::typeOfBedloadCapacityCalculationMethodToString(CombinerVariables::ReckingBedloadCapacityNonFractional);
	return result;
}

Grains ReckingBedloadCapacityNonFractional::calculate (const RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods) const
{
	if(riverReachProperties.regularRiverReachProperties.bedslope < 0.0)
	{
		std::ostringstream oStringStream;
		oStringStream << "The formula of Recking 2010 is not defined for negative bedslopes. At ";
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
		oStringStream << " occurence of negative bedslope." << std::flush;

		char* tmpChar = new char [(oStringStream.str()).size()+1];
		std::strcpy(tmpChar, (oStringStream.str()).c_str());
		const char *const negativeBedslopeErrorMessage = tmpChar;
		throw(negativeBedslopeErrorMessage);
	}

	Grains bedloadCapacity = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0);
	double activeLayerOverallVolume = bedloadCapacity.getOverallVolume();

	if( activeLayerOverallVolume >= 0.0 )
	{
		if( activeLayerOverallVolume <= 1e-9 ) // In this case use the sediment coming from upstream as distribution base. This enables bypassing of sediment.
		{
			int localCellID = riverReachProperties.getCellID();
			std::vector<RiverReachProperties*> upstreamCellPointers = riverReachProperties.getUpstreamCellPointers();
			for(std::vector<RiverReachProperties*>::const_iterator currentUpstreamPointerIterator = upstreamCellPointers.begin(); currentUpstreamPointerIterator < upstreamCellPointers.end(); ++currentUpstreamPointerIterator)
			{
				#if !defined SEDFLOWPARALLEL
				if( (*currentUpstreamPointerIterator)->getCellID() < localCellID )
					{ bedloadCapacity += (*currentUpstreamPointerIterator)->regularRiverReachProperties.erosionRate; }
				else
				#endif
					{ bedloadCapacity += this->calculate( (*(*currentUpstreamPointerIterator)), overallMethods ); }
			}
			activeLayerOverallVolume = bedloadCapacity.getOverallVolume();
		}

		if( activeLayerOverallVolume > 1e-9 )
		{
			double dEightyfour = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(), 84.0);
			double dFifty = riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(), 50.0);
			//In text below equation (2)
			double thetaEightyfour = riverReachProperties.regularRiverReachProperties.bedShearStress / ( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * ( (riverReachProperties.getOverallParameters())->getSedimentDensity() - (riverReachProperties.getOverallParameters())->getWaterDensity() ) * dEightyfour);
			//Equation(4)
			double thetaEightyfourCritical = ( (1.32 * riverReachProperties.regularRiverReachProperties.bedslope) + 0.037) * pow( ( dEightyfour / dFifty ), (-0.93) );
			//Equation(13)
			double breakpointL = 12.53 * pow( (dEightyfour/dFifty), ( 4.445*sqrt(riverReachProperties.regularRiverReachProperties.bedslope) ) ) * pow(thetaEightyfourCritical,1.605);
			double einsteinDimensionlessParameter;
			if ( breakpointL >= thetaEightyfour )
			{
				//Equation(12a)
				einsteinDimensionlessParameter = 0.0005 * pow( ( dEightyfour / dFifty ), ( (-18)*sqrt(riverReachProperties.regularRiverReachProperties.bedslope) ) ) * pow( (thetaEightyfour / thetaEightyfourCritical), 6.5 );
			} else
			{
				//Equation(12b)
				einsteinDimensionlessParameter = 14 * pow( thetaEightyfour, 2.45);
			}
			//In text below equation(2)
			double bedloadCapacityVolumePerUnitBedSurface = einsteinDimensionlessParameter * sqrt( ( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * ( ((riverReachProperties.getOverallParameters())->getSedimentDensity()/(riverReachProperties.getOverallParameters())->getWaterDensity()) - 1.0 ) * dEightyfour * dEightyfour * dEightyfour ) );
			double bedloadCapacityVolume = bedloadCapacityVolumePerUnitBedSurface * riverReachProperties.regularRiverReachProperties.activeWidth;
			bedloadCapacity *= ( bedloadCapacityVolume / activeLayerOverallVolume );
		}
		else // If there is no sediment in the reach and there is nothing coming from upstream. Do nothing and return zeros.
		{
			bedloadCapacity *= 0.0;
		}
	}

	return bedloadCapacity;
}

}
