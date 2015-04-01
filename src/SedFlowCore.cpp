/*
 * SedFlowCore.cpp
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

#include "SedFlowCore.h"

#include <limits>
#include <math.h>
#include <sstream>
#include <limits>
#include <cstring>

#include "StringTools.h"

namespace SedFlow {

SedFlowCore::SedFlowCore(const char* inputXMLfile)
{

	std::string fullPath = StringTools::getFullPath(inputXMLfile);

	if( !(StringTools::fileExists(fullPath)) )
	{
		std::ostringstream oStringStream;
		oStringStream << "The specified input file" << std::endl << "\"" << fullPath << "\"" << std::endl << "cannot be found." << std::flush;

		char* tmpChar = new char [(oStringStream.str()).size()+1];
		std::strcpy(tmpChar, (oStringStream.str()).c_str());
		const char *const inputFileNotFoundErrorMessage = tmpChar;
		throw(inputFileNotFoundErrorMessage);
	}

	fullPath = StringTools::extractPathToFolderAndEnsureForwardSlashes(fullPath);

	pugi::xml_document doc;
	pugi::xml_parse_result parseResult = doc.load_file(inputXMLfile);

    if (!parseResult)
    {
		std::ostringstream oStringStream;
		oStringStream << "XML parse error: " << parseResult.description() << std::flush;

		char* tmpChar = new char [(oStringStream.str()).size()+1];
		std::strcpy(tmpChar, (oStringStream.str()).c_str());
		const char *const xmlParseErrorMessage = tmpChar;
		throw(xmlParseErrorMessage);
    }

	UserInputReader* userInputReader = SedFlowInterfaceRealisationBuilders::userInputReaderBuilder( doc.first_child().name() );

	ConstructionVariables sedFlowConstructionVariables = userInputReader->convertUserInputXMLIntoConstructionVariables(doc,fullPath);

	delete userInputReader;


	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator tempObjectsIterator;
	HighestOrderStructuresPointers highestOrderStructuresPointers = HighestOrderStructuresPointers();

	tempObjectsIterator = sedFlowConstructionVariables.labelledObjects.find("overallParameters");
	if(tempObjectsIterator == sedFlowConstructionVariables.labelledObjects.end() )
	{
		const char *const overallParametersErrorMessage = "The variable overallParameters is needed for the construction of a SedFlow simulation set-up.";
		throw(overallParametersErrorMessage);
	}
	else
	{
		this->overallParameters = SedFlowBuilders::overallParametersBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers);
		highestOrderStructuresPointers.overallParameters = this->overallParameters;
	}

	tempObjectsIterator = sedFlowConstructionVariables.labelledObjects.find("riverSystemProperties");
	if(tempObjectsIterator == sedFlowConstructionVariables.labelledObjects.end() )
	{
		const char *const riverSystemPropertiesErrorMessage = "The variable riverSystemProperties is needed for the construction of a SedFlow simulation set-up.";
		throw(riverSystemPropertiesErrorMessage);
	}
	else
	{
		this->riverSystemProperties = SedFlowBuilders::riverSystemPropertiesBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers);
		highestOrderStructuresPointers.riverSystemProperties = this->riverSystemProperties;
	}

	tempObjectsIterator = sedFlowConstructionVariables.labelledObjects.find("overallMethods");
	if(tempObjectsIterator == sedFlowConstructionVariables.labelledObjects.end() )
	{
		const char *const overallMethodsErrorMessage = "The variable overallMethods is needed for the construction of a SedFlow simulation set-up.";
		throw(overallMethodsErrorMessage);
	}
	else
	{
		this->overallMethods = SedFlowBuilders::overallMethodsBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers);
		highestOrderStructuresPointers.overallMethods = this->overallMethods;
	}

	tempObjectsIterator = sedFlowConstructionVariables.labelledObjects.find("riverSystemMethods");
	if(tempObjectsIterator == sedFlowConstructionVariables.labelledObjects.end() )
	{
		const char *const riverSystemMethodsErrorMessage = "The variable riverSystemMethods is needed for the construction of a SedFlow simulation set-up.";
		throw(riverSystemMethodsErrorMessage);
	}
	else
	{
		this->riverSystemMethods = SedFlowBuilders::riverSystemMethodsBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers);
		highestOrderStructuresPointers.riverSystemMethods = this->riverSystemMethods;
	}

	tempObjectsIterator = sedFlowConstructionVariables.labelledObjects.find("outputMethods");
	if(tempObjectsIterator == sedFlowConstructionVariables.labelledObjects.end() )
	{
		const char *const outputMethodsErrorMessage = "The variable outputMethods is needed for the construction of a SedFlow simulation set-up.";
		throw(outputMethodsErrorMessage);
	}
	else
	{
		this->outputMethods = SedFlowBuilders::outputMethodsBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers);
		highestOrderStructuresPointers.outputMethods = this->outputMethods;
	}
}

ConstructionVariables SedFlowCore::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::SedFlow;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( overallParameters->createConstructionVariables() );
	result.labelledObjects["overallParameters"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( overallMethods->createConstructionVariables() );
	result.labelledObjects["overallMethods"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( riverSystemProperties->createConstructionVariables() );
	result.labelledObjects["riverSystemProperties"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( riverSystemMethods->createConstructionVariables() );
	result.labelledObjects["riverSystemMethods"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( outputMethods->createConstructionVariables() );
	result.labelledObjects["outputMethods"] = constructionVariablesVector;
	return result;
}

void SedFlowCore::checkForInfiniteOrNaNTimeSteps() const
{
	double elapsedSeconds = this->overallParameters->getElapsedSeconds();

	if( elapsedSeconds != elapsedSeconds || fabs(elapsedSeconds) == std::numeric_limits<double>::infinity() )
	{
		const char *const errorMessage = "Infinite or not defined time step.";
		throw(errorMessage);
	}
}

void SedFlowCore::checkForTooSmallTimeSteps() const
{
	if( this->overallParameters->getCurrentTimeStepLengthInSeconds() < this->overallParameters->getTimeStepThresholdForTerminatingSimulation() )
	{
		std::string errorMessageAsString = "Time step of ";
		std::ostringstream oStringStream;
		oStringStream << this->overallParameters->getCurrentTimeStepLengthInSeconds() << std::flush;
		errorMessageAsString.append( oStringStream.str() );
		errorMessageAsString.append(" seconds smaller than threshold of ");
		oStringStream.str("");
		oStringStream.clear();
		oStringStream << this->overallParameters->getTimeStepThresholdForTerminatingSimulation() << std::flush;
		errorMessageAsString.append( oStringStream.str() );
		errorMessageAsString.append(" seconds. Potentially zero bed slope in ");
		oStringStream.str("");
		oStringStream.clear();
		int zeroBedslopeCellID = 0;
		bool atMargin = false;
		double minimumSlope = std::numeric_limits<double>::max();
		for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = this->riverSystemProperties->regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < this->riverSystemProperties->regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachProperties)
		{
			if(currentRiverReachProperties->regularRiverReachProperties.bedslope < minimumSlope)
			{
				minimumSlope = currentRiverReachProperties->regularRiverReachProperties.bedslope;
				if(currentRiverReachProperties->isMargin())
				{
					atMargin = true;
					if(currentRiverReachProperties->isUpstreamMargin())
					{
						zeroBedslopeCellID = this->riverSystemProperties->regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( currentRiverReachProperties->getDownstreamCellID() );
					}
					else
					{
						zeroBedslopeCellID = this->riverSystemProperties->regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( (currentRiverReachProperties->getUpstreamCellIDs()).at(0) );
					}
				}
				else
				{
					atMargin = false;
					zeroBedslopeCellID = this->riverSystemProperties->regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( currentRiverReachProperties->getCellID() );
				}
			}
		}

		if(atMargin)
		{
			errorMessageAsString.append("margin at ");
		}
		errorMessageAsString.append("cell ");
		oStringStream << zeroBedslopeCellID << std::flush;
		errorMessageAsString.append( oStringStream.str() );
		errorMessageAsString.append(".");

		char* tmpChar = new char [errorMessageAsString.size()+1];
		std::strcpy(tmpChar, errorMessageAsString.c_str());
		const char *const errorMessage = tmpChar;
		throw(errorMessage);
	}
}

}
