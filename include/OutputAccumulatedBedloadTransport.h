/*
 * OutputAccumulatedBedloadTransport.h
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

#ifndef OUTPUTACCUMULATEDBEDLOADTRANSPORT_H_
#define OUTPUTACCUMULATEDBEDLOADTRANSPORT_H_

#include "OutputMethodType.h"

#include <string>
#include <fstream>
#include <sstream>

namespace SedFlow {

class OutputAccumulatedBedloadTransport: public OutputMethodType {
private:
	std::vector<Grains> accumulatedBedloadTransport;
	bool outputDetailedFractional;

	std::ofstream overallVolumeOFileStream;
	std::ofstream detailedFractionalOFileStream;
	std::string overallVolumeOutputFileAsString;
	char* overallVolumeOutputFile;
	std::string detailedFractionalOutputFileAsString;
	char* detailedFractionalOutputFile;

	bool outputIncludingPoreVolume;
	CombinerVariables::TypesOfRegularRiverReachProperties ABTIncrementProperty;

	std::vector<int> userCellIDsForOutput;

	std::vector<const RegularRiverReachProperties*> cellPointersForOutput;
	std::vector<CombinerVariables::TypesOfGrains> typesOfGrainsOrderForOutput;

	std::vector<Grains>::iterator currentABT;
	std::vector<Grains>::const_iterator currentConstABT;
	std::vector<CombinerVariables::TypesOfGrains>::const_iterator currentTypeOfGrainsIterator;
	const GrainType* currentGrainTypePointer;
	std::vector<double> currentFractionalAbundances;
	std::vector<double>::const_iterator currentFractionIterator;

	std::vector<double> fractionalGrainDiameters;
	std::vector<double>::const_iterator fractionalGrainDiametersBegin;
	std::vector<double>::const_iterator fractionalGrainDiametersEnd;

public:
	OutputAccumulatedBedloadTransport(std::vector<int> userCellIDsForOutput, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, bool outputIncludingPoreVolume, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods, bool outputDetailedFractional);
	virtual ~OutputAccumulatedBedloadTransport();

	OutputMethodType* createOutputMethodTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
};

}

#endif /* OUTPUTACCUMULATEDBEDLOADTRANSPORT_H_ */
