/*
 * OutputVerbatimTranslationOfConstructionVariablesToXML.cpp
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

#include "OutputVerbatimTranslationOfConstructionVariablesToXML.h"

#include <cstring>
#include <iomanip>

namespace SedFlow {

OutputVerbatimTranslationOfConstructionVariablesToXML::OutputVerbatimTranslationOfConstructionVariablesToXML(bool overwriteFiles, int numberOfFileIDDigits, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods):
	OutputMethodType(path, outputFiles, writeLineEachTimeStep, outputInterval, explicitTimesForOutput, precisionForOutput, overallParameters, overallMethods, riverSystemProperties, riverSystemMethods),
	overwriteFiles(overwriteFiles),
	numberOfFileIDDigits(numberOfFileIDDigits),
	fileID(0),
	fileExtension(".xml")
{
	this->typeOfOutputMethod = CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML;
	try
		{
			if (outputFiles.size() != 1)
			{
				const char *const errorMessage = "For OutputVerbatimTranslationOfConstructionVariablesToXML exactly one (no more, no less) output file name is needed.";
				throw(errorMessage);
			}
		} catch (const char *const msg) {
			std::cerr << msg << std::endl;
			throw;
		}
	makePathEndWithOneSlash();
	outputFile = this->path;
	outputFile.append( outputFiles.at(0) );

	if(!overwriteFiles)
	{
		outputFileWithoutExtension = outputFile;
		outputFileWithoutExtension.erase((outputFileWithoutExtension.end()-4),outputFileWithoutExtension.end());

		oStringStreamForFileID.str("");
		oStringStreamForFileID.clear();
		oStringStreamForFileID << std::setfill('0') << std::setw(numberOfFileIDDigits) << fileID;
		oStringStreamForFileID.flush();

		outputFile = outputFileWithoutExtension;
		outputFile.append(oStringStreamForFileID.str());
		outputFile.append(fileExtension);
	}
}

OutputVerbatimTranslationOfConstructionVariablesToXML::OutputVerbatimTranslationOfConstructionVariablesToXML(bool overwriteFiles, int fileID, int numberOfFileIDDigits, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods):
	OutputMethodType(path, outputFiles, writeLineEachTimeStep, outputInterval, explicitTimesForOutput, precisionForOutput, overallParameters, overallMethods, riverSystemProperties, riverSystemMethods),
	overwriteFiles(overwriteFiles),
	numberOfFileIDDigits(numberOfFileIDDigits),
	fileID(fileID),
	fileExtension(".xml")
{
	this->typeOfOutputMethod = CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML;
	try
		{
			if (outputFiles.size() != 1)
			{
				const char *const errorMessage = "For OutputVerbatimTranslationOfConstructionVariablesToXML exactly one (no more, no less) output file name is needed.";
				throw(errorMessage);
			}
		} catch (const char *const msg) {
			std::cerr << msg << std::endl;
			throw;
		}
	makePathEndWithOneSlash();
	outputFile = path;
	outputFile.append( outputFiles.at(0) );

	if(!overwriteFiles)
	{
		outputFileWithoutExtension = outputFile;
		outputFileWithoutExtension.erase((outputFileWithoutExtension.end()-4),outputFileWithoutExtension.end());

		oStringStreamForFileID.str("");
		oStringStreamForFileID.clear();
		oStringStreamForFileID << std::setfill('0') << std::setw(numberOfFileIDDigits) << fileID;
		oStringStreamForFileID.flush();

		outputFile = outputFileWithoutExtension;
		outputFile.append(oStringStreamForFileID.str());
		outputFile.append(fileExtension);
	}
}

OutputMethodType* OutputVerbatimTranslationOfConstructionVariablesToXML::createOutputMethodTypePointerCopy() const
{
	OutputMethodType* result = new OutputVerbatimTranslationOfConstructionVariablesToXML(this->overwriteFiles, this->fileID, this->numberOfFileIDDigits, this->path, this->outputFiles, this->writeLineEachTimeStep, this->outputInterval, this->explicitTimesForOutput, this->precisionForOutput, this->overallParameters, this->overallMethods, this->riverSystemProperties, this->riverSystemMethods);
	return result;
}


ConstructionVariables OutputVerbatimTranslationOfConstructionVariablesToXML::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
	result.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML);
	std::vector<double> doubleVector;
	doubleVector.push_back(outputInterval);
	result.labelledDoubles["outputInterval"] = doubleVector;
	doubleVector.clear();
	for(std::vector<double>::const_iterator iterator = explicitTimesForOutput.begin(); iterator < explicitTimesForOutput.end(); ++iterator)
			{ doubleVector.push_back(*iterator); }
	result.labelledDoubles["explicitTimesForOutput"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(precisionForOutput);
	result.labelledInts["precisionForOutput"] = intVector;
	intVector.clear();
	intVector.push_back(fileID);
	result.labelledInts["fileID"] = intVector;
	intVector.clear();
	intVector.push_back(numberOfFileIDDigits);
	result.labelledInts["numberOfFileIDDigits"] = intVector;
	intVector.clear();
	std::vector<bool> boolVector;
	boolVector.push_back(writeLineEachTimeStep);
	result.labelledBools["writeLineEachTimeStep"] = boolVector;
	boolVector.clear();
	boolVector.push_back(overwriteFiles);
	result.labelledBools["overwriteFiles"] = boolVector;
	boolVector.clear();
	std::vector<std::string> stringVector;
	stringVector.push_back(path);
	result.labelledStrings["path"] = stringVector;
	stringVector.clear();
	for(std::vector<std::string>::const_iterator iterator = outputFiles.begin(); iterator < outputFiles.end(); ++iterator)
			{ stringVector.push_back(*iterator); }
	result.labelledStrings["outputFiles"] = stringVector;
	return result;
}

void OutputVerbatimTranslationOfConstructionVariablesToXML::initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if (!overwriteFiles) { writeOutputXML(allConstitutingOutputMethodTypes); }
}

void OutputVerbatimTranslationOfConstructionVariablesToXML::writeOutputLineIfScheduled(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if (writeLineEachTimeStep)
	{
		this->writeOutputXML(allConstitutingOutputMethodTypes);
	}
	else
	{
		if (isDueToWriteLine(overallParameters->getElapsedSeconds(), timeOfLastOutput, outputInterval, remainingTimesForOutputInReverseOrder))
		{
			this->writeOutputXML(allConstitutingOutputMethodTypes);
		}
	}
}

void OutputVerbatimTranslationOfConstructionVariablesToXML::forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	#if defined _DEBUG || defined DEBUG
		this->writeOutputXML(allConstitutingOutputMethodTypes);
	#endif
}

void OutputVerbatimTranslationOfConstructionVariablesToXML::writeOutputXML(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if (!overwriteFiles)
	{
		oStringStreamForFileID.str("");
		oStringStreamForFileID.clear();
		oStringStreamForFileID << std::setfill('0') << std::setw(numberOfFileIDDigits) << fileID;
		oStringStreamForFileID.flush();

		outputFile = outputFileWithoutExtension;
		outputFile.append(oStringStreamForFileID.str());
		outputFile.append(fileExtension);
	}

	ConstructionVariables sedFlow = createConstructionVariablesForCompleteModel(overallParameters, overallMethods, riverSystemProperties, riverSystemMethods, allConstitutingOutputMethodTypes);

	std::string rootNodeName = CombinerVariables::getExternalStringCorrespondingToInternalString( CombinerVariables::typeOfUserInputReadersToString(CombinerVariables::VerbatimTranslationFromXMLToConstructionVariables) );
	pugi::xml_document containerDocument;
    pugi::xml_node rootNode = containerDocument.append_child(rootNodeName.c_str());


	appendChildNodeCorrespondingToConstructionVariables(rootNode,sedFlow, "sedFlow", this->precisionForOutput);

	containerDocument.save_file(outputFile.c_str());

	if (!overwriteFiles) { ++fileID; }
}

void OutputVerbatimTranslationOfConstructionVariablesToXML::appendChildNodeCorrespondingToConstructionVariables (pugi::xml_node& targetParentNode, const ConstructionVariables& inputConstructionVariables, std::string nameForNewNode, int precisionForOutput)
{
	std::ostringstream oStringStream;
	oStringStream.precision(precisionForOutput);
	oStringStream << std::scientific;

	pugi::xml_node currentNode = targetParentNode.append_child(nameForNewNode.c_str());

	//Append interfaceOrCombinerType
	currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::InterfaceOrCombinerType).c_str()).text().set(CombinerVariables::typeOfCombinersAndInterfacesToString(inputConstructionVariables.interfaceOrCombinerType).c_str());

	//Append realisationType
	currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::RealisationType).c_str()).text().set(inputConstructionVariables.realisationType.c_str());

	//Append labelledDoubles
	//Create Node
	currentNode = currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::LabelledDoubles).c_str());
	//Fill Node
	for(std::map< std::string, std::vector<double> >::const_iterator currentMapEntry = inputConstructionVariables.labelledDoubles.begin() ; currentMapEntry != inputConstructionVariables.labelledDoubles.end(); ++currentMapEntry)
	{
		currentNode = currentNode.append_child((*currentMapEntry).first.c_str());
		for(std::vector<double>::const_iterator currentVectorItem = (*currentMapEntry).second.begin(); currentVectorItem < (*currentMapEntry).second.end(); ++currentVectorItem)
		{
			oStringStream.str("");
			oStringStream.clear();
			oStringStream << *currentVectorItem << std::flush;
			currentNode.append_child("DoubleVectorItem").text().set(((*currentVectorItem == *currentVectorItem) ? oStringStream.str().c_str() : "NaN"));
		}
		currentNode = currentNode.parent();
	}
	currentNode = currentNode.parent();

	//Append labelledInts
	//Create Node
	currentNode = currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::LabelledInts).c_str());
	//Fill Node
	for(std::map< std::string, std::vector<int> >::const_iterator currentMapEntry = inputConstructionVariables.labelledInts.begin() ; currentMapEntry != inputConstructionVariables.labelledInts.end(); ++currentMapEntry)
	{
		currentNode = currentNode.append_child((*currentMapEntry).first.c_str());
		for(std::vector<int>::const_iterator currentVectorItem = (*currentMapEntry).second.begin(); currentVectorItem < (*currentMapEntry).second.end(); ++currentVectorItem)
		{
			currentNode.append_child("IntVectorItem").text().set(*currentVectorItem);
		}
		currentNode = currentNode.parent();
	}
	currentNode = currentNode.parent();

	//Append labelledBools
	//Create Node
	currentNode = currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::LabelledBools).c_str());
	//Fill Node
	for(std::map< std::string, std::vector<bool> >::const_iterator currentMapEntry = inputConstructionVariables.labelledBools.begin() ; currentMapEntry != inputConstructionVariables.labelledBools.end(); ++currentMapEntry)
	{
		currentNode = currentNode.append_child((*currentMapEntry).first.c_str());
		for(std::vector<bool>::const_iterator currentVectorItem = (*currentMapEntry).second.begin(); currentVectorItem < (*currentMapEntry).second.end(); ++currentVectorItem)
		{
			currentNode.append_child("BoolVectorItem").text().set((*currentVectorItem ? "true" : "false"));
		}
		currentNode = currentNode.parent();
	}
	currentNode = currentNode.parent();

	//Append labelledObjects
	//Create Node
	currentNode = currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::LabelledObjects).c_str());
	//Fill Node
	for(std::map< std::string, std::vector<ConstructionVariables> >::const_iterator currentMapEntry = inputConstructionVariables.labelledObjects.begin() ; currentMapEntry != inputConstructionVariables.labelledObjects.end(); ++currentMapEntry)
	{
		currentNode = currentNode.append_child((*currentMapEntry).first.c_str());
		for(std::vector<ConstructionVariables>::const_iterator currentVectorItem = (*currentMapEntry).second.begin(); currentVectorItem < (*currentMapEntry).second.end(); ++currentVectorItem)
		{
			appendChildNodeCorrespondingToConstructionVariables(currentNode,*currentVectorItem,"ObjectVectorItem", precisionForOutput);
		}
		currentNode = currentNode.parent();
	}
	currentNode = currentNode.parent();

	//Append labelledStrings
	//Create Node
	currentNode = currentNode.append_child(ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::LabelledStrings).c_str());
	//Fill Node
	for(std::map< std::string, std::vector<std::string> >::const_iterator currentMapEntry = inputConstructionVariables.labelledStrings.begin() ; currentMapEntry != inputConstructionVariables.labelledStrings.end(); ++currentMapEntry)
	{
		currentNode = currentNode.append_child((*currentMapEntry).first.c_str());
		for(std::vector<std::string>::const_iterator currentVectorItem = (*currentMapEntry).second.begin(); currentVectorItem < (*currentMapEntry).second.end(); ++currentVectorItem)
		{
			currentNode.append_child("StringVectorItem").text().set(currentVectorItem->c_str());
		}
		currentNode = currentNode.parent();
	}
	currentNode = currentNode.parent();

}

ConstructionVariables OutputVerbatimTranslationOfConstructionVariablesToXML::createConstructionVariablesForCompleteModel (const OverallParameters* const overallParameters, const OverallMethods* const overallMethods, const RiverSystemProperties* const riverSystemProperties, const RiverSystemMethods* const riverSystemMethods, const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	ConstructionVariables sedFlow;

	sedFlow.interfaceOrCombinerType = CombinerVariables::SedFlow;

	tmpConstructionVariablesVector.push_back( overallParameters->createConstructionVariables() );
	sedFlow.labelledObjects["overallParameters"] = tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.clear();

	tmpConstructionVariablesVector.push_back( overallMethods->createConstructionVariables() );
	sedFlow.labelledObjects["overallMethods"] = tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.clear();

	tmpConstructionVariablesVector.push_back( riverSystemProperties->createConstructionVariables() );
	sedFlow.labelledObjects["riverSystemProperties"] = tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.clear();

	tmpConstructionVariablesVector.push_back( riverSystemMethods->createConstructionVariables() );
	sedFlow.labelledObjects["riverSystemMethods"] = tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.clear();

	ConstructionVariables outputMethods;
	outputMethods.interfaceOrCombinerType = CombinerVariables::OutputMethods;
	for(std::vector<OutputMethodType*>::const_iterator currentOutputMethodType = allConstitutingOutputMethodTypes.begin(); currentOutputMethodType < allConstitutingOutputMethodTypes.end(); ++currentOutputMethodType)
			{ tmpConstructionVariablesVector.push_back( (*currentOutputMethodType)->createConstructionVariables() ); }
	outputMethods.labelledObjects["constitutingOutputMethodTypes"] = tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.clear();

	tmpConstructionVariablesVector.push_back( outputMethods );
	sedFlow.labelledObjects["outputMethods"] = tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.clear();

	return sedFlow;
}

}
