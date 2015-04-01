/*
 * OutputVerbatimTranslationOfConstructionVariablesToXML.h
 *
 *   Copyright (C) 2014 Swiss Federal Research Institute WSL (http://www.wsl.ch)
 *   Developed by F.U.M. Heimann
 *   Published by the Swiss Federal Research Institute WSL
 *   
 *   This software is based on pugixml library (http://pugixml.org).
 *   pugixml is Copyright (C) 2006-2012 Arseny Kapoulkine.
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

#ifndef OUTPUTVERBATIMTRANSLATIONOFCONSTRUCTIONVARIABLESTOXML_H_
#define OUTPUTVERBATIMTRANSLATIONOFCONSTRUCTIONVARIABLESTOXML_H_

#include "OutputMethodType.h"

#include <string>
#include <sstream>

#include "pugixml.h"

#include "ConstructionVariables.h"

namespace SedFlow {

class OutputVerbatimTranslationOfConstructionVariablesToXML: public OutputMethodType {
private:
	std::string outputFile;
	bool overwriteFiles;
	std::ostringstream oStringStreamForFileID;
	int numberOfFileIDDigits;
	std::string outputFileWithoutExtension;
	int fileID;
	std::string fileExtension;
public:
	OutputVerbatimTranslationOfConstructionVariablesToXML(bool overwriteFiles, int numberOfFileIDDigits, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods);
	OutputVerbatimTranslationOfConstructionVariablesToXML(bool overwriteFiles, int fileID, int numberOfFileIDDigits, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods);
	virtual ~OutputVerbatimTranslationOfConstructionVariablesToXML(){}

	OutputMethodType* createOutputMethodTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);
	void update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes){}
	void writeOutputLineIfScheduled(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes); //Overwrites the pre-implemented function of base class.
	void forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes); //EMPTY except when compiled for Debugging.
	void finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes){}

	//Replacing the forcedWriteOutputLine by writeOutputXML omits the overwriting of the last stable set-up in case of an exception.
	void writeOutputXML(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);

	static void appendChildNodeCorrespondingToConstructionVariables (pugi::xml_node& targetParentNode, const ConstructionVariables& inputConstructionVariables, std::string nameForNewNode, int precisionForOutput);
	static ConstructionVariables createConstructionVariablesForCompleteModel(const OverallParameters* const overallParameters, const OverallMethods* const overallMethods, const RiverSystemProperties* const riverSystemProperties, const RiverSystemMethods* const riverSystemMethods, const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes);

};

}

#endif /* OUTPUTVERBATIMTRANSLATIONOFCONSTRUCTIONVARIABLESTOXML_H_ */
