/*
 * OutputSimulationSetup.h
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

#ifndef OUTPUTSIMULATIONSETUP_H_
#define OUTPUTSIMULATIONSETUP_H_

#include "OutputMethodType.h"

namespace SedFlow {

class OutputSimulationSetup: public OutputMethodType {
private:
	std::vector<CombinerVariables::TypesOfCombinersAndInterfaces> setupPropertiesForOutput;
	bool printSimulationID;
	std::string simulationID;
	bool printSimulationName;
	std::string simulationName;
	bool printStartingTime;
	bool printModelVersion;

	int compilationYear;
	std::string compilationMonth;
	int compilationDay;

	std::string boolToString(bool inputBool) const;

	enum TypesOfUseForCalcGradient {bedSlopeCalculationMethod, waterEnergySlopeCalculationMethod, sedimentEnergySlopeCalculationMethod};
	void addCalcGradientToFileStream(TypesOfUseForCalcGradient typeOfUseForCalcGradient, std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addCalcGradientToFileStream(const ConstructionVariables& calcGradient, std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addFlowResistanceToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addFlowResistanceToFileStream(const ConstructionVariables& flowResistance, std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addCalcBedloadCapacityToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addCalcTauToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addCalcActiveWidthToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addStrataSortingToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addCalcHidingFactorsToFileStream(const ConstructionVariables& calcHidingFactors, std::ofstream& oFileStream, int numberOfPrecedingTabs) const;
	void addCalcThresholdForInitiationOfBedloadMotionToFileStream(const ConstructionVariables& calcThresholdForInitiationOfBedloadMotion, std::ofstream& oFileStream, int numberOfPrecedingTabs) const;

public:
	OutputSimulationSetup(const std::vector<CombinerVariables::TypesOfCombinersAndInterfaces>& setupPropertiesForOutput, bool printSimulationID, const std::string& simulationID, bool printSimulationName, const std::string& simulationName, bool printStartingTime, bool printModelVersion, std::string path, std::vector<std::string> outputFiles, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods);
	OutputSimulationSetup(const std::vector<CombinerVariables::TypesOfCombinersAndInterfaces>& setupPropertiesForOutput, bool printSimulationID, const std::string& simulationID, bool printSimulationName, const std::string& simulationName, bool printStartingTime, bool printModelVersion, int compilationYear, const std::string& compilationMonth, int compilationDay, std::string path, std::vector<std::string> outputFiles, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods);
	virtual ~OutputSimulationSetup();

	OutputMethodType* createOutputMethodTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes){}
	void forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes){}
	void finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes){}
};

}

#endif /* OUTPUTSIMULATIONSETUP_H_ */
