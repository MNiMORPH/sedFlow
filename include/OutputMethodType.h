/*
 * OutputMethodType.h
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

#ifndef OUTPUTMETHODTYPE_H_
#define OUTPUTMETHODTYPE_H_

#include <vector>
#include <string>

#include "CombinerVariables.h"
#include "ConstructionVariables.h"
#include "OverallParameters.h"
#include "OverallMethods.h"
#include "RiverSystemProperties.h"
#include "RiverSystemMethods.h"

namespace SedFlow {

class OutputMethodType {
protected:
	CombinerVariables::TypesOfOutputMethod typeOfOutputMethod;

	const OverallParameters* const overallParameters;
	const OverallMethods* const overallMethods;
	const RiverSystemProperties* const riverSystemProperties;
	const RiverSystemMethods* const riverSystemMethods;

	std::string path;
	std::vector<std::string> outputFiles;

	int precisionForOutput;

	void makePathEndWithOneSlash();

	bool writeLineEachTimeStep;

	double outputInterval;
	std::vector<double> explicitTimesForOutput;

	double timeOfLastOutput;
	std::vector<double> remainingTimesForOutputInReverseOrder;

public:
	OutputMethodType(std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods);
	virtual ~OutputMethodType(){}

	virtual OutputMethodType* createOutputMethodTypePointerCopy() const = 0;//This method HAS TO BE implemented.

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual void initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes) = 0;
	virtual void update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes) = 0;
	virtual void writeOutputLineIfScheduled(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes); //Pre-Implemented
	virtual void forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes) = 0;
	virtual void finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes) = 0;

	inline std::string getTypeOfOutputMethodAsString() const { return CombinerVariables::typeOfOutputMethodToString(typeOfOutputMethod); }
	inline CombinerVariables::TypesOfOutputMethod getTypeOfOutputMethod() const { return typeOfOutputMethod; }

	static bool isDueToWriteLine(double elapsedSeconds, double& timeOfLastOutput, double outputInterval, std::vector<double>& remainingTimesForOutputInReverseOrder);
	static bool isDueToWriteLineWithoutUpdatingParameters(double elapsedSeconds, double timeOfLastOutput, double outputInterval, const std::vector<double>& remainingTimesForOutputInReverseOrder);
};

}

#endif /* OUTPUTMETHODTYPE_H_ */
