/*
 * ConstructionVariables.h
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

#ifndef CONSTRUCTIONVARIABLES_H_
#define CONSTRUCTIONVARIABLES_H_

#include <string>
#include <map>
#include <vector>
#include <utility>

#include "CombinerVariables.h"

namespace SedFlow {

class ConstructionVariables {
public:
	ConstructionVariables(){}
	virtual ~ConstructionVariables(){}

	CombinerVariables::TypesOfCombinersAndInterfaces interfaceOrCombinerType;
	std::string realisationType;

	std::map< std::string, std::vector<double> > labelledDoubles;
	std::map< std::string, std::vector<int> > labelledInts;
	std::map< std::string, std::vector<bool> > labelledBools;
	std::map< std::string, std::vector<ConstructionVariables> > labelledObjects;
	std::map< std::string, std::vector<std::string> > labelledStrings;

	enum ConstructionVariableTypes {InterfaceOrCombinerType, RealisationType, LabelledDoubles, LabelledInts, LabelledBools, LabelledObjects, LabelledStrings};
	static ConstructionVariableTypes stringToConstructionVariableType(std::string string);
	static std::string constructionVariableTypeToString(ConstructionVariableTypes constructionVariableType);
	void printToCout() const; //This method is intended for debugging.

private:
	static std::map<std::string, ConstructionVariableTypes> mapForConstructionVariableTypes;
	static std::map<std::string, ConstructionVariableTypes> createMapForConstructionVariableTypes();
};

}

#endif /* CONSTRUCTIONVARIABLES_H_ */
