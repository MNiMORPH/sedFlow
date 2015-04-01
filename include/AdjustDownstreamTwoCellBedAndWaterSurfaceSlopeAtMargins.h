/*
 * AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.h
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

#ifndef ADJUSTDOWNSTREAMTWOCELLBEDANDWATERSURFACESLOPEATMARGINS_H_
#define ADJUSTDOWNSTREAMTWOCELLBEDANDWATERSURFACESLOPEATMARGINS_H_

#include "ChangeRateModifiersType.h"

namespace SedFlow {

class AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins: public ChangeRateModifiersType {
private:
	double minimumBedSlope;
	double maximumBedSlope;
	double minimumWaterSurfaceSlope;
	double maximumWaterSurfaceSlope;
	bool ensureModificationAgainstOtherChangeRateModifiers;
	double currentlyElapsedSeconds;
	bool newTimeStep;

public:
	AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(double minimumBedSlope, double maximumBedSlope, double minimumWaterSurfaceSlope, double maximumWaterSurfaceSlope, bool ensureModificationAgainstOtherChangeRateModifiers, CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod);
	AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(double minimumBedSlope, double maximumBedSlope, double minimumWaterSurfaceSlope, double maximumWaterSurfaceSlope, bool ensureMinimumSlopesAgainstOtherChangeRateModifiers, CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod, double currentlyElapsedSeconds, bool newTimeStep);
	virtual ~AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(){}

	ChangeRateModifiersType* createChangeRateModifiersTypePointerCopy() const;//This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	void adjustSlopes(RiverReachProperties& riverReachProperties);

	inline void inputModification(RiverReachProperties& riverReachProperties) { if(ensureModificationAgainstOtherChangeRateModifiers) { this->adjustSlopes(riverReachProperties); } }
	void modificationBeforeUpdates(RiverReachProperties& riverReachProperties);
	inline bool furtherModificationIterationNecessary(const RiverReachProperties& riverReachProperties) const { return newTimeStep; }
	inline void finalModification(RiverReachProperties& riverReachProperties)  { if(ensureModificationAgainstOtherChangeRateModifiers) { this->adjustSlopes(riverReachProperties); } }

};

}

#endif /* ADJUSTDOWNSTREAMTWOCELLBEDANDWATERSURFACESLOPEATMARGINS_H_ */
