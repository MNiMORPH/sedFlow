/*
 * CorrectionForBedloadWeightAtSteepSlopes.cpp
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

#include "CorrectionForBedloadWeightAtSteepSlopes.h"

#include <math.h>

namespace SedFlow {

double CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedSedimentEnergySlope (double simpleSedimentEnergySlope, double angleOfReposeInRadians, double bedSlope)
{
	double relativeSlope = (angleOfReposeInRadians - atan(bedSlope));

	if(relativeSlope > 0.0)
	{
		// The following line represents equation (7) in Rickenmann(2005). Geschiebetransport bei steilen Gefällen. Versuchsanstalt für Wasserbau, Hydrologie und Glaziologie - Festkolloquium 7. Oktober 2005
		simpleSedimentEnergySlope *= sin(angleOfReposeInRadians) / sin( relativeSlope );
	}

	return simpleSedimentEnergySlope;

}

double CorrectionForBedloadWeightAtSteepSlopes::calculateCorrectedDimensionlessCriticalShearStressAtSteepCounterSlopes (double simpleDimensionlessCriticalShearStress, double angleOfReposeInRadians, double counterBedSlope)
{
	double relativeSlope = (angleOfReposeInRadians + atan(fabs(counterBedSlope)));
	return ( simpleDimensionlessCriticalShearStress * sin(relativeSlope) / sin(angleOfReposeInRadians) );
}

}
