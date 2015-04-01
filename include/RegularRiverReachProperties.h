/*
 * RegularRiverReachProperties.h
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

#ifndef REGULARRIVERREACHPROPERTIES_H_
#define REGULARRIVERREACHPROPERTIES_H_


#include <vector>

#include "Grains.h"
#include "OverallParameters.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class RegularRiverReachProperties {
friend class RegularRiverReachMethods;
private:
	const OverallParameters* overallParameters;
public:
	RegularRiverReachProperties(const OverallParameters* overallParameters);
	RegularRiverReachProperties(const OverallParameters* overallParameters,
								double elevation,
								double length,
								bool sillOccurence,
								double sillTopEdgeElevation,
								double discharge,
								double flowVelocity,
								double maximumWaterdepth,
								double bedShearStress,
								double activeWidth,
								std::vector<Grains> strataPerUnitBedSurface,
								double bedslope,
								double waterEnergyslope,
								double sedimentEnergyslope,
								double unreducedSedimentEnergyslope,
								double waterVolumeChangeRate,
								double waterVolumeChange,
								Grains erosionRate,
								Grains depositionRate,
								Grains erosion,
								Grains deposition,
								Grains erosionPerUnitBedSurface,
								Grains depositionPerUnitBedSurface);
	virtual ~RegularRiverReachProperties();

	ConstructionVariables createConstructionVariables()const;

	double elevation;
	double length;
	bool sillOccurence;
	double sillTopEdgeElevation;
	double discharge;
	double flowVelocity;
	double maximumWaterdepth;
	double alluviumThicknessIncludingPoreVolume() const;
	inline double bedrockElevation()const{return (elevation - alluviumThicknessIncludingPoreVolume());}
	inline double waterLevel()const{return (elevation + maximumWaterdepth);}
	double hydraulicHead()const;
	double kineticComponentOfHydraulicHead()const;
	double bedShearStress;
	double activeWidth;
	std::vector<Grains> strataPerUnitBedSurface;

	double bedslope;
	double waterEnergyslope;
	double sedimentEnergyslope;
	double unreducedSedimentEnergyslope;

	double waterVolumeChangeRate;
	double waterVolumeChange;

	Grains erosionRate; //As unit volume per unit time
	Grains depositionRate; //As unit volume

	Grains erosion; //As unit volume
	Grains deposition; //As unit volume

	Grains erosionPerUnitBedSurface;
	Grains depositionPerUnitBedSurface;

	inline const OverallParameters* getOverallParameters() const { return overallParameters; }

	double getDoubleProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties) const;
	void setDoubleProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties, double newValue);
	Grains getGrainsProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties) const;
	void setGrainsProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties, Grains newValue);

	std::vector<double> getActiveOverallFractionalAbundance() const;

};

}

#endif /* REGULARRIVERREACHPROPERTIES_H_ */
