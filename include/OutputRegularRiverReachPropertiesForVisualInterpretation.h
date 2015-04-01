/*
 * OutputRegularRiverReachPropertiesForVisualInterpretation.h
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

#ifndef OUTPUTREGULARRIVERREACHPROPERTIESFORVISUALINTERPRETATION_H_
#define OUTPUTREGULARRIVERREACHPROPERTIESFORVISUALINTERPRETATION_H_

#include "OutputMethodType.h"

#include <fstream>
#include <sstream>

namespace SedFlow {

class OutputRegularRiverReachPropertiesForVisualInterpretation: public OutputMethodType {
private:
	std::ofstream oFileStream;
	std::ostringstream oStringStream;
	std::string outputFileAsString;
	char* outputFile;
	int horizontalBarLength;

	std::vector<CombinerVariables::TypesOfRegularRiverReachProperties> regularRiverReachPropertiesForOutput;
	std::vector<int> userCellIDsForOutput;
	bool printUpstreamMargins;
	bool printDownstreamMargin;
	bool outputTimeStepLength;
	bool outputInitialValues;

	bool useSecondaryOutputInterval;
	double primaryOutputInterval;
	int referenceCellUserCellID;
	const RegularRiverReachProperties* referenceCell;
	CombinerVariables::TypesOfRegularRiverReachProperties referenceProperty;
	double thresholdToBeExceeded;
	double secondaryOutputInterval;

	std::vector<const RegularRiverReachProperties*> cellPointersForOutput;
	std::vector<std::string> cellIDLabels;
	std::vector<CombinerVariables::TypesOfGrains> typesOfGrainsOrderForOutput;

	void appendGrainstoOStringStream(const Grains& toPrint);
	std::string printGrains(const Grains& toPrint);
	std::string printStrata(const std::vector<Grains>& toPrint);
	std::string printGrainsProperty(const RegularRiverReachProperties* cellPointer, CombinerVariables::TypesOfRegularRiverReachProperties typeOfProperty);

	std::vector<const RegularRiverReachProperties*>::const_iterator currentCellPointerIterator;
	std::vector<Grains>::const_iterator currentGrainsIterator;
	std::vector<CombinerVariables::TypesOfGrains>::const_iterator currentTypeOfGrainsIterator;
	const GrainType* currentGrainTypePointer;
	std::vector<double> currentFractionalAbundances;
	std::vector<double>::const_iterator currentFractionIterator;

	std::vector<double> fractionalGrainDiameters;
	std::vector<double>::const_iterator fractionalGrainDiametersBegin;
	std::vector<double>::const_iterator fractionalGrainDiametersEnd;

public:
	OutputRegularRiverReachPropertiesForVisualInterpretation(std::vector<CombinerVariables::TypesOfRegularRiverReachProperties> regularRiverReachPropertiesForOutput, std::vector<int> userCellIDsForOutput, bool outputTimeStepLength, int horizontalBarLength, bool outputInitialValues, bool printUpstreamMargins, bool printDownstreamMargin, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, bool useSecondaryOutputInterval, int referenceCellUserCellID, CombinerVariables::TypesOfRegularRiverReachProperties referenceProperty, double thresholdToBeExceeded, double secondaryOutputInterval, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods);
	virtual ~OutputRegularRiverReachPropertiesForVisualInterpretation();

	OutputMethodType* createOutputMethodTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
};

}

#endif /* OUTPUTREGULARRIVERREACHPROPERTIESFORVISUALINTERPRETATION_H_ */
