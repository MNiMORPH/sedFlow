/*
 * UserInputReader.h
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

#ifndef USERINPUTREADER_H_
#define USERINPUTREADER_H_

#include "pugixml.h"

#include "ConstructionVariables.h"

namespace SedFlow {

class UserInputReader {
public:
	UserInputReader(){}
	virtual ~UserInputReader(){}

	virtual ConstructionVariables createConstructionVariables()const = 0;

	virtual ConstructionVariables convertUserInputXMLIntoConstructionVariables (pugi::xml_document& userInputXMLDocument, const std::string& folderContainingXML) const {return ConstructionVariables();}
};

}

#endif /* USERINPUTREADER_H_ */
