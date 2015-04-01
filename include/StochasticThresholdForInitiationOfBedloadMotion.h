/*
 * StochasticThresholdForInitiationOfBedloadMotion.h
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

#ifndef STOCHASTICTHRESHOLDFORINITIATIONOFBEDLOADMOTION_H_
#define STOCHASTICTHRESHOLDFORINITIATIONOFBEDLOADMOTION_H_

#include "CalcThresholdForInitiationOfBedloadMotion.h"

namespace SedFlow {

class StochasticThresholdForInitiationOfBedloadMotion: public CalcThresholdForInitiationOfBedloadMotion {
private:
	const double minimumThresholdValue;
	const double miu;
	const double beta;
	const int seed;

	double weightForCurrent;
	double weightForPrevious;
	double weightForPrePrevious;

	bool correctionForBedloadWeightAtSteepCounterSlopes;

	std::map<double,std::vector<double> > mapFromWidthToValues;
	/*
	 * Content of values:
	 * 0: Current discharge
	 * 1: miu
	 * 2: beta
	 * 3: current value
	 * 4: previous value
	 * 5: prePrevioust value
	 * */

	std::vector<double> widthsForSpecialValues;
	std::vector<double> miuSpecialValues;
	std::vector<double> betaSpecialValues;
	std::map<double,std::pair<double,double> > mapFromWidthToSpecialValues;

	double calculateNotConst (const RegularRiverReachProperties& regularRiverReachProperties);


public:
	StochasticThresholdForInitiationOfBedloadMotion(double minimumThresholdValue, double miu, double beta, int seed, double weightForCurrent, double weightForPrevious, double weightForPrePrevious, bool correctionForBedloadWeightAtSteepCounterSlopes, std::vector<double> widthsForSpecialValues, std::vector<double> miuSpecialValues, std::vector<double> betaSpecialValues);
	StochasticThresholdForInitiationOfBedloadMotion(double minimumThresholdValue, double miu, double beta, int seed, double weightForCurrent, double weightForPrevious, double weightForPrePrevious, bool correctionForBedloadWeightAtSteepCounterSlopes, std::map<double,std::vector<double> > mapFromWidthToValues, std::vector<double> widthsForSpecialValues, std::vector<double> miuSpecialValues, std::vector<double> betaSpecialValues, std::map<double,std::pair<double,double> > mapFromWidthToSpecialValues);
	virtual ~StochasticThresholdForInitiationOfBedloadMotion();

	CalcThresholdForInitiationOfBedloadMotion* createCalcThresholdForInitiationOfBedloadMotionPointerCopy() const; //This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	double calculate (const RegularRiverReachProperties& regularRiverReachProperties) const;

};

}

#endif /* STOCHASTICTHRESHOLDFORINITIATIONOFBEDLOADMOTION_H_ */
