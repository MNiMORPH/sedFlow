/*
 * RecirculateSediment.h
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

#ifndef RECIRCULATESEDIMENT_H_
#define RECIRCULATESEDIMENT_H_

#include "ChangeRateModifiersType.h"

namespace SedFlow {

class RecirculateSediment: public ChangeRateModifiersType {
private:
	double inputDivisor;
	bool ensureModificationAgainstOtherChangeRateModifiers;
	bool firstTime;
	RiverReachProperties* downstreamMarginCellPointer;
	std::vector<RiverReachProperties*> inputCellPointersForDownstreamMargin;
	double currentlyElapsedSeconds;
	Grains inputErosionRate;
	Grains inputErosionRateForNextTimeStep;
	void updateDownstreamMarginCellPointer(RiverReachProperties& riverReachProperties);

public:
	RecirculateSediment(double inputDivisor, bool ensureModificationAgainstOtherChangeRateModifiers);
	RecirculateSediment(double inputDivisor, bool ensureModificationAgainstOtherChangeRateModifiers, RiverReachProperties* downstreamMarginCellPointer, double currentlyElapsedSeconds, const Grains& inputErosionRate, const Grains& inputErosionRateForNextTimeStep);
	virtual ~RecirculateSediment(){}

	ChangeRateModifiersType* createChangeRateModifiersTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	inline void inputModification(RiverReachProperties& riverReachProperties){}
	void modificationBeforeUpdates(RiverReachProperties& riverReachProperties);
	inline bool furtherModificationIterationNecessary(const RiverReachProperties& riverReachProperties) const { return false; }
	inline void finalModification(RiverReachProperties& riverReachProperties) { if(ensureModificationAgainstOtherChangeRateModifiers) { this->modificationBeforeUpdates(riverReachProperties); } }

};

}

#endif /* RECIRCULATESEDIMENT_H_ */
