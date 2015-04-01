/*
 * ConstructionVariables.cpp
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

#include "ConstructionVariables.h"

#include "iostream"

namespace SedFlow {

std::map<std::string, ConstructionVariables::ConstructionVariableTypes> ConstructionVariables::createMapForConstructionVariableTypes()
{
	std::map<std::string, ConstructionVariableTypes> result;
	result["interfaceOrCombinerType"] = ConstructionVariables::InterfaceOrCombinerType;
	result["realisationType"] = ConstructionVariables::RealisationType;
	result["labelledDoubles"] = ConstructionVariables::LabelledDoubles;
	result["labelledInts"] = ConstructionVariables::LabelledInts;
	result["labelledBools"] = ConstructionVariables::LabelledBools;
	result["labelledObjects"] = ConstructionVariables::LabelledObjects;
	result["labelledStrings"] = ConstructionVariables::LabelledStrings;
	return result;
}

std::map<std::string, ConstructionVariables::ConstructionVariableTypes> ConstructionVariables::mapForConstructionVariableTypes(ConstructionVariables::createMapForConstructionVariableTypes());

void ConstructionVariables::printToCout() const
{
	std::cout << "--------------------------" << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "interfaceOrCombinerType: " << CombinerVariables::typeOfCombinersAndInterfacesToString(this->interfaceOrCombinerType) << std::endl;
	std::cout << "realisationType: " << this->realisationType << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << "labelledDoubles:" << std::endl;
	for(std::map<std::string,std::vector<double> >::const_iterator mapIterator = this->labelledDoubles.begin(); mapIterator != this->labelledDoubles.end(); ++mapIterator)
	{
		std::cout << "----" << std::endl;
		std::cout << mapIterator->first << ":";
		for(std::vector<double>::const_iterator vectorIterator = mapIterator->second.begin(); vectorIterator < mapIterator->second.end(); ++vectorIterator)
		{ std::cout << " " << *vectorIterator; }
		std::cout << ";" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << "labelledInts:" << std::endl;
	for(std::map<std::string,std::vector<int> >::const_iterator mapIterator = this->labelledInts.begin(); mapIterator != this->labelledInts.end(); ++mapIterator)
	{
		std::cout << "----" << std::endl;
		std::cout << mapIterator->first << ":";
		for(std::vector<int>::const_iterator vectorIterator = mapIterator->second.begin(); vectorIterator < mapIterator->second.end(); ++vectorIterator)
		{ std::cout << " " << *vectorIterator; }
		std::cout << ";" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << "labelledBools:" << std::endl;
	for(std::map<std::string,std::vector<bool> >::const_iterator mapIterator = this->labelledBools.begin(); mapIterator != this->labelledBools.end(); ++mapIterator)
	{
		std::cout << "----" << std::endl;
		std::cout << mapIterator->first << ":";
		for(std::vector<bool>::const_iterator vectorIterator = mapIterator->second.begin(); vectorIterator < mapIterator->second.end(); ++vectorIterator)
		{ std::cout << " " << *vectorIterator; }
		std::cout << ";" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << "labelledObjects:" << std::endl;
	for(std::map<std::string,std::vector<ConstructionVariables> >::const_iterator mapIterator = this->labelledObjects.begin(); mapIterator != this->labelledObjects.end(); ++mapIterator)
	{
		std::cout << "----" << std::endl;
		std::cout << mapIterator->first << ";" << std::endl;
		//mapIterator->second.at(0).printToCout();
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << "labelledStrings:" << std::endl;
	for(std::map<std::string,std::vector<std::string> >::const_iterator mapIterator = this->labelledStrings.begin(); mapIterator != this->labelledStrings.end(); ++mapIterator)
	{
		std::cout << "----" << std::endl;
		std::cout << mapIterator->first << ":";
		for(std::vector<std::string>::const_iterator vectorIterator = mapIterator->second.begin(); vectorIterator < mapIterator->second.end(); ++vectorIterator)
		{ std::cout << " " << *vectorIterator; }
		std::cout << ";" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << "--------------------------" << std::endl;
}

ConstructionVariables::ConstructionVariableTypes ConstructionVariables::stringToConstructionVariableType(std::string string)
{
	std::map< std::string, ConstructionVariableTypes>::const_iterator resultIterator = mapForConstructionVariableTypes.find(string);
	if (resultIterator == mapForConstructionVariableTypes.end())
	{
		const char *const errorMessage = "String not mapped to construction variable type.";
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string ConstructionVariables::constructionVariableTypeToString(ConstructionVariables::ConstructionVariableTypes constructionVariableType)
{
	std::string result;
	switch (constructionVariableType)
	{
	case ConstructionVariables::InterfaceOrCombinerType:
		result = "interfaceOrCombinerType";
		break;

	case ConstructionVariables::RealisationType:
		result = "realisationType";
		break;

	case ConstructionVariables::LabelledDoubles:
		result = "labelledDoubles";
		break;

	case ConstructionVariables::LabelledInts:
		result = "labelledInts";
		break;

	case ConstructionVariables::LabelledBools:
		result = "labelledBools";
		break;

	case ConstructionVariables::LabelledObjects:
		result = "labelledObjects";
		break;

	case ConstructionVariables::LabelledStrings:
		result = "labelledStrings";
		break;

	default:
		const char *const errorMessage = "Invalid construction variable type";
		throw (errorMessage);
	}
	return result;
}

}
