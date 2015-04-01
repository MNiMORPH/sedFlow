/*
 * StratigraphyWithThresholdBasedUpdate.h
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

#ifndef STRATIGRAPHYWITHTHRESHOLDBASEDUPDATE_H_
#define STRATIGRAPHYWITHTHRESHOLDBASEDUPDATE_H_

#include "StrataSorting.h"

namespace SedFlow {

class StratigraphyWithThresholdBasedUpdate: public StrataSorting {
protected:
	void updateLayerThickness (const RegularRiverReachProperties& regularRiverReachProperties);

	double incrementLayerThickness;
	bool dynamicThresholds;
	double referenceGrainSizePercentile;
	double valueOrFactorForLowerThresholdForActiveLayerThickness, valueOrFactorForUpperThresholdForActiveLayerThickness;
	double minimumLowerThresholdForActiveLayerThickness, minimumDifferenceBetweenThresholdsForActiveLayerThickness;

	double lowerThresholdForActiveLayerThickness, upperThresholdForActiveLayerThickness;

	virtual void checkAndUpdateValuesForConstructor();

private:
	void updateThresholds(const Grains& activeLayer, const std::vector<double>& fractionalGrainDiameters);

public:
	StratigraphyWithThresholdBasedUpdate(double incrementLayerThickness, bool dynamicThresholds, double valueOrFactorForLowerThresholdForActiveLayerThickness, double valueOrFactorForUpperThresholdForActiveLayerThickness);
	StratigraphyWithThresholdBasedUpdate(double incrementLayerThickness, bool dynamicThresholds, double referenceGrainSizePercentile, double valueOrFactorForLowerThresholdForActiveLayerThickness, double valueOrFactorForUpperThresholdForActiveLayerThickness);
	StratigraphyWithThresholdBasedUpdate(double incrementLayerThickness, bool dynamicThresholds, double referenceGrainSizePercentile, double valueOrFactorForLowerThresholdForActiveLayerThickness, double valueOrFactorForUpperThresholdForActiveLayerThickness, double minimumLowerThresholdForActiveLayerThickness, double minimumDifferenceBetweenThresholdsForActiveLayerThickness);
	virtual ~StratigraphyWithThresholdBasedUpdate();

	StrataSorting* createStrataSortingPointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	bool sortStrata (RegularRiverReachProperties& regularRiverReachProperties, int cellID);

	std::pair<double,double> getThresholdsForActiveLayerThickness (bool dynamic, const Grains& activeLayer, const std::vector<double>& fractionalGrainDiameters);
};

}

#endif /* STRATIGRAPHYWITHTHRESHOLDBASEDUPDATE_H_ */
