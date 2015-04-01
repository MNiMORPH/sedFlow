/*
 * InputPropertyTimeSeriesLinearlyInterpolated.h
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

#ifndef INPUTPROPERTYTIMESERIESLINEARLYINTERPOLATED_H_
#define INPUTPROPERTYTIMESERIESLINEARLYINTERPOLATED_H_

#include "ChangeRateModifiersType.h"

#include "TimeSeries.h"
#include "OverallMethods.h"

namespace SedFlow {

class InputPropertyTimeSeriesLinearlyInterpolated: public ChangeRateModifiersType {
private:
	std::map<int, std::vector<TimeSeries> > timeSeriesInputsByCellID;
	bool ensureModificationAgainstOtherChangeRateModifiers;
	std::vector<TimeSeries> createSingleTimeSeriesVectorCopy()const;
	const OverallMethods& overallMethods;

	enum ModificationPhases {InputModification, ModificationBeforeUpdates, FinalModification};
	ModificationPhases currentModificationPhase;

	void applyModification(RiverReachProperties& riverReachProperties);

public:
	InputPropertyTimeSeriesLinearlyInterpolated(CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod, std::vector<TimeSeries> timeSeriesInputs, bool ensureModificationAgainstOtherChangeRateModifiers, const OverallMethods& overallMethods);
	virtual ~InputPropertyTimeSeriesLinearlyInterpolated();

	ChangeRateModifiersType* createChangeRateModifiersTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void inputModification(RiverReachProperties& riverReachProperties); //Surplus adding is only performed in this method. Even if ensureModificationAgainstOtherChangeRateModifiers is true.
	void modificationBeforeUpdates(RiverReachProperties& riverReachProperties);
	bool furtherModificationIterationNecessary(const RiverReachProperties& riverReachProperties) const { return false; }
	void finalModification(RiverReachProperties& riverReachProperties);

};

}

#endif /* INPUTPROPERTYTIMESERIESLINEARLYINTERPOLATED_H_ */
