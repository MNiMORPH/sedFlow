/*
 * StringTools.h
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

#ifndef STRINGTOOLS_H_
#define STRINGTOOLS_H_

#include <string>
#include <map>
#include <vector>

namespace SedFlow {

class StringTools {
public:
	StringTools(){}
	virtual ~StringTools(){}

	static std::string getFullPath(std::string path);

	static void removeCharacter (std::string& string, char characterToBeRemoved);
	static void trimString (std::string& string);
	static void trimStringKeepingInternalBlanks (std::string& string);
	static void trimStringKeepingAllBlanks (std::string& string);

	static std::string trimStringCopy (std::string string) { trimString(string); return string;  }
	static std::string trimStringCopyKeepingInternalBlanks (std::string string) { trimStringCopyKeepingInternalBlanks(string); return string; }
	static std::string trimStringCopyKeepingAllBlanks (std::string string) { trimStringCopyKeepingAllBlanks(string); return string; }

	static void removeAdjacentDuplicatesOfCertainCharacter (std::string& string, char characterForDuplicatesToBeRemoved);
	static void removeSingleCertainCharacterFromBeginning (std::string& string, char characterToBeRemoved);
	static void removeSingleCertainCharacterFromEnd (std::string& string, char characterToBeRemoved);

	static void ensureForwardSlashes (std::string& string);

	static std::string extractPathToFolder (const std::string& string);
	static std::string extractPathToFolderAndEnsureForwardSlashes (const std::string& string);

	static void makeStringEndWithOneSlash (std::string& string);
	static void ensureForwardSlashesAndMakeStringEndWithOneSlash (std::string& string) { ensureForwardSlashes(string); makeStringEndWithOneSlash(string); }

	static int countCharacter(const std::string& string, char characterToBeCounted);

	static bool fileExists(const char* const fileName);
	static bool fileExists(const std::string& fileName);
	static std::map<std::string,std::vector<std::string> > tabDelimitedSpreadsheetFileToStringMap(const std::string& tabDelimitedSpreadsheetFileName);

	static std::vector<std::string> splitString(const std::string& inputString, char delim);

	static std::string monthIntToString(int month);
	static int monthStringToInt(const std::string& month);

	static bool stringToBool(std::string inputString);
	static std::string boolTo_string(bool inputBool);
	static std::string boolTo_String(bool inputBool);
	static std::string boolTo_STRING(bool inputBool);

	static std::vector<double> stringVectorToDoubleVector(std::vector<std::string> inputStringVector);
	static std::vector<int> stringVectorToIntVector(std::vector<std::string> inputStringVector);
	static std::vector<bool> stringVectorToBoolVector(std::vector<std::string> inputStringVector);

private:
	static std::map< std::string, int> createMapFromMonthStringToInt();
	static std::map< std::string, int> mapFromMonthStringToInt;
};

}

#endif /* STRINGTOOLS_H_ */
