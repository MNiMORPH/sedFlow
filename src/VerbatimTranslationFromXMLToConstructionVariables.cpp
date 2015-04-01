/*
 * VerbatimTranslationFromXMLToConstructionVariables.cpp
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

#include "VerbatimTranslationFromXMLToConstructionVariables.h"

#include <string>
#include <sstream>
#include <iostream>

#include "StringTools.h"

namespace SedFlow {

ConstructionVariables VerbatimTranslationFromXMLToConstructionVariables::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::UserInputReader;
	result.realisationType = CombinerVariables::typeOfUserInputReadersToString(CombinerVariables::VerbatimTranslationFromXMLToConstructionVariables);
	return result;
}

ConstructionVariables VerbatimTranslationFromXMLToConstructionVariables::convertUserInputXMLIntoConstructionVariables (pugi::xml_document& userInputXMLDocument, const std::string& folderContainingXML) const
{
	pugi::xml_node rootNode = *(userInputXMLDocument.begin());

	CombinerVariables::TypesOfUserInputReaders typeOfUserInputReader = CombinerVariables::stringToTypeOfUserInputReaders(CombinerVariables::getInternalStringCorrespondingToExternalString(rootNode.name()));

	//Check by the root name whether the xml Document can be read by this userInputReader class.
	if ( typeOfUserInputReader != CombinerVariables::VerbatimTranslationFromXMLToConstructionVariables)
	{
		const char *const firstErrorMessage = "Not matching type of user input reader requested.";
		throw(firstErrorMessage);
	}

	pugi::xml_node firstChildNode = rootNode.first_child();
	return this->convertXMLNodeToConstructionVariables(firstChildNode);
}

ConstructionVariables VerbatimTranslationFromXMLToConstructionVariables::convertXMLNodeToConstructionVariables (pugi::xml_node& rootNode) const
{
	ConstructionVariables result = ConstructionVariables();
	std::istringstream iStringStream;
	double tmpDouble;
	int tmpInt;
	std::string tmpString;

	std::string currentNodeName;
	ConstructionVariables::ConstructionVariableTypes currentConstructionVariableTypes;

	std::vector<double> doubleVectorForLabelledDoubles;
	std::vector<int> intVectorForLabelledInts;
	std::vector<bool> boolVectorForLabelledBools;
	std::vector<ConstructionVariables> constructionVariablesVectorForLabelledObjects;
	std::vector<std::string> stringVectorForLabelledStrings;

	pugi::xml_node currentNodeConstructionVariablesLevel = rootNode.first_child();
	pugi::xml_node currentNodeMapLevel;
	pugi::xml_node currentNodeVectorLevel;

	while(currentNodeConstructionVariablesLevel)
	{
		currentNodeName = currentNodeConstructionVariablesLevel.name();
		currentConstructionVariableTypes = ConstructionVariables::stringToConstructionVariableType( currentNodeName );

		switch(currentConstructionVariableTypes)
		{
		case ConstructionVariables::InterfaceOrCombinerType:
			result.interfaceOrCombinerType = CombinerVariables::stringToTypeOfCombinersAndInterfaces( StringTools::trimStringCopy(currentNodeConstructionVariablesLevel.child_value()) );
			break;

		case ConstructionVariables::RealisationType:
			result.realisationType = StringTools::trimStringCopy(currentNodeConstructionVariablesLevel.child_value());
			break;

		case ConstructionVariables::LabelledDoubles:
			currentNodeMapLevel = currentNodeConstructionVariablesLevel.first_child();
			while(currentNodeMapLevel)
			{
				//Now we are in the labelledDoubles
				currentNodeName = currentNodeMapLevel.name();
				currentNodeVectorLevel = currentNodeMapLevel.first_child();
				while(currentNodeVectorLevel)
				{
					//Now we are in one of the double vectors within labelledDoubles
					iStringStream.clear();
					iStringStream.str( StringTools::trimStringCopy(currentNodeVectorLevel.child_value()) );
					iStringStream >> tmpDouble;
					doubleVectorForLabelledDoubles.push_back( tmpDouble );
					currentNodeVectorLevel = currentNodeVectorLevel.next_sibling();
				}
				//Now we get back out of one of the double vectors within labelledDoubles
				result.labelledDoubles[currentNodeName] = doubleVectorForLabelledDoubles;
				doubleVectorForLabelledDoubles.clear();
				currentNodeMapLevel = currentNodeMapLevel.next_sibling();
			}
			//Now we get back out of the labelledDoubles
			break;

		case ConstructionVariables::LabelledInts:
			currentNodeMapLevel = currentNodeConstructionVariablesLevel.first_child();
			while(currentNodeMapLevel)
			{
				//Now we are in the LabelledInts
				currentNodeName = currentNodeMapLevel.name();
				currentNodeVectorLevel = currentNodeMapLevel.first_child();
				while(currentNodeVectorLevel)
				{
					//Now we are in one of the double vectors within LabelledInts
					iStringStream.clear();
					iStringStream.str( StringTools::trimStringCopy(currentNodeVectorLevel.child_value()) );
					iStringStream >> tmpInt;
					intVectorForLabelledInts.push_back( tmpInt );
					currentNodeVectorLevel = currentNodeVectorLevel.next_sibling();
				}
				//Now we get back out of one of the double vectors within LabelledInts
				result.labelledInts[currentNodeName] = intVectorForLabelledInts;
				intVectorForLabelledInts.clear();
				currentNodeMapLevel = currentNodeMapLevel.next_sibling();
			}
			//Now we get back out of the LabelledInts
			break;

		case ConstructionVariables::LabelledBools:
			currentNodeMapLevel = currentNodeConstructionVariablesLevel.first_child();
			while(currentNodeMapLevel)
			{
				//Now we are in the LabelledBools
				currentNodeName = currentNodeMapLevel.name();
				currentNodeVectorLevel = currentNodeMapLevel.first_child();
				while(currentNodeVectorLevel)
				{
					//Now we are in one of the double vectors within LabelledBools
					tmpString.clear();
					tmpString = StringTools::trimStringCopy(currentNodeVectorLevel.child_value());
					boolVectorForLabelledBools.push_back( StringTools::stringToBool(tmpString) );
					currentNodeVectorLevel = currentNodeVectorLevel.next_sibling();
				}
				//Now we get back out of one of the double vectors within LabelledBools
				result.labelledBools[currentNodeName] = boolVectorForLabelledBools;
				boolVectorForLabelledBools.clear();
				currentNodeMapLevel = currentNodeMapLevel.next_sibling();
			}
			//Now we get back out of the LabelledBools
			break;

		case ConstructionVariables::LabelledObjects:
			currentNodeMapLevel = currentNodeConstructionVariablesLevel.first_child();
			while(currentNodeMapLevel)
			{
				//Now we are in the LabelledObjects
				currentNodeName = currentNodeMapLevel.name();
				currentNodeVectorLevel = currentNodeMapLevel.first_child();
				while(currentNodeVectorLevel)
				{
					//Now we are in one of the double vectors within LabelledObjects
					//Recursive function call
					constructionVariablesVectorForLabelledObjects.push_back( convertXMLNodeToConstructionVariables(currentNodeVectorLevel) );
					currentNodeVectorLevel = currentNodeVectorLevel.next_sibling();
				}
				//Now we get back out of one of the double vectors within LabelledObjects
				result.labelledObjects[currentNodeName] = constructionVariablesVectorForLabelledObjects;
				constructionVariablesVectorForLabelledObjects.clear();
				currentNodeMapLevel = currentNodeMapLevel.next_sibling();
			}
			//Now we get back out of the LabelledObjects
			break;

		case ConstructionVariables::LabelledStrings:
			currentNodeMapLevel = currentNodeConstructionVariablesLevel.first_child();
			while(currentNodeMapLevel)
			{
				//Now we are in the LabelledStrings
				currentNodeName = currentNodeMapLevel.name();
				currentNodeVectorLevel = currentNodeMapLevel.first_child();
				while(currentNodeVectorLevel)
				{
					//Now we are in one of the double vectors within LabelledStrings
					stringVectorForLabelledStrings.push_back( StringTools::trimStringCopy(currentNodeVectorLevel.child_value()) );
					currentNodeVectorLevel = currentNodeVectorLevel.next_sibling();
				}
				//Now we get back out of one of the double vectors within LabelledStrings
				result.labelledStrings[currentNodeName] = stringVectorForLabelledStrings;
				stringVectorForLabelledStrings.clear();
				currentNodeMapLevel = currentNodeMapLevel.next_sibling();
			}
			//Now we get back out of the LabelledStrings
			break;

		default:
			const char *const errorMessage = "Invalid construction variable type";
			throw(errorMessage);
		}
		currentNodeConstructionVariablesLevel = currentNodeConstructionVariablesLevel.next_sibling();
	}

	return result;
}

}
