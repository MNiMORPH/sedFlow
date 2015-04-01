/*
 * StringTools.cpp
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

#include "StringTools.h"

#include <algorithm>
#include <ctype.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

#if defined CURRENTLYWINDOWS
//This is the Windows version
#include <stdlib.h>
#endif

#if defined CURRENTLYUNIX
//This is the Linux version
#include <stdlib.h>
#include <limits.h> /* PATH_MAX */
#endif


namespace SedFlow {

std::string StringTools::getFullPath(std::string path)
{

#if defined CURRENTLYWINDOWS
	//This is the Windows version
	char full[_MAX_PATH];
	if( _fullpath( full, path.c_str(), _MAX_PATH ) == NULL )
	{
		std::string invalidPathErrorMessageString = "The following partial path is invalid:\n";
		invalidPathErrorMessageString.append(path);
		invalidPathErrorMessageString.append("\n");
		char* invalidPathErrorMessageNotConst = new char [invalidPathErrorMessageString.size()+1];
		std::strcpy (invalidPathErrorMessageNotConst,invalidPathErrorMessageString.c_str());
		const char *const invalidPathErrorMessage = invalidPathErrorMessageNotConst;
		throw(invalidPathErrorMessage);
	}
	path = full;
	return path;
#endif


#if defined CURRENTLYUNIX
	//This is the Linux version
	char full[PATH_MAX];
	if (realpath(path.c_str(), full) == NULL)
	{
		std::string invalidPathErrorMessageString = "The following partial path is invalid:\n";
		invalidPathErrorMessageString.append(path);
		invalidPathErrorMessageString.append("\n");
		char* invalidPathErrorMessageNotConst = new char [invalidPathErrorMessageString.size()+1];
		std::strcpy (invalidPathErrorMessageNotConst,invalidPathErrorMessageString.c_str());
		const char *const invalidPathErrorMessage = invalidPathErrorMessageNotConst;
		throw(invalidPathErrorMessage);
	}
 	path = full;
	return path;
#endif

	const char *const unknownPlatformErrorMessage = "Sorry, this copy of sedFlow has been compiled on an unknown system. (Neither Windows nor Linux)";
	throw(unknownPlatformErrorMessage);
	return path;
}

void StringTools::removeCharacter (std::string& string, char characterToBeRemoved)
{
	string.erase(std::remove(string.begin(), string.end(), characterToBeRemoved), string.end());
}

void StringTools::trimStringKeepingAllBlanks (std::string& string)
{
	removeCharacter(string,'\r');
	removeCharacter(string,'\n');
	removeCharacter(string,'\t');
}

void StringTools::trimString (std::string& string)
{
	trimStringKeepingAllBlanks(string);
	string.erase(remove_if(string.begin(), string.end(), isspace), string.end());
}

void StringTools::trimStringKeepingInternalBlanks (std::string& string)
{
	trimStringKeepingAllBlanks(string);
	const std::size_t firstRealCharacter = string.find_first_not_of(' ');
	if (firstRealCharacter == std::string::npos)
	{
		string.clear();
	}
	else
	{
		const std::size_t lastRealCharacter = string.find_last_not_of(' ');
		const std::size_t numberOfRealCharacters = lastRealCharacter - firstRealCharacter + 1;
		string = string.substr(firstRealCharacter, numberOfRealCharacters);
	}
}

void StringTools::removeAdjacentDuplicatesOfCertainCharacter (std::string& string, char characterForDuplicatesToBeRemoved)
{
	std::string::iterator startingIterator = string.begin();
	std::string::iterator adjacentFindIterator = std::adjacent_find(startingIterator,string.end());
	while(adjacentFindIterator != string.end())
	{
		if( *adjacentFindIterator == characterForDuplicatesToBeRemoved )
		{
			string.erase(adjacentFindIterator);
			startingIterator = string.begin();
			if(startingIterator == string.end()) { adjacentFindIterator = startingIterator; }
			else
			{
				adjacentFindIterator = std::adjacent_find(startingIterator,string.end());
			}
		}
		else
		{
			startingIterator = ++adjacentFindIterator;
			adjacentFindIterator = std::adjacent_find(startingIterator,string.end());
		}
	}
}

void StringTools::removeSingleCertainCharacterFromBeginning (std::string& string, char characterToBeRemoved)
{
	if(string.size() > 0)
	{
		std::string::iterator stringIterator = string.begin();
		if( *stringIterator == characterToBeRemoved )
			{ string.erase(stringIterator); }
	}
}

void StringTools::removeSingleCertainCharacterFromEnd (std::string& string, char characterToBeRemoved)
{
	if(string.size() > 0)
	{
		std::string::iterator stringIterator = string.end() - 1;
		if( *stringIterator == characterToBeRemoved )
			{ string.erase(stringIterator); }
	}
}

void StringTools::ensureForwardSlashes (std::string& string)
{
	std::replace(string.begin(),string.end(),'\\','/');
}

std::string StringTools::extractPathToFolder (const std::string& string)
{
	return string.substr(0, string.find_last_of("/\\"));
}

std::string StringTools::extractPathToFolderAndEnsureForwardSlashes (const std::string& string)
{
	std::string result (extractPathToFolder(string));
	ensureForwardSlashes(result);
	return result;
}

void StringTools::makeStringEndWithOneSlash (std::string& string)
{
	std::string::iterator lastChar = string.end() - 1;
	if ( *lastChar == '/')
	{
		std::string::iterator oneCharBeforeLast;
		while( lastChar != string.begin())
		{
			oneCharBeforeLast = string.end() - 2;
			if( *oneCharBeforeLast == '/')
			{
				string.erase(lastChar);
				lastChar = string.end() - 1;
			}
			else
			{
				break;
			}

		}
	} else
	{
		string.append("/");
	}
}

int StringTools::countCharacter(const std::string& string, char characterToBeCounted)
{
	int result = 0;
	for(std::string::const_iterator currentCharacter = string.begin(); currentCharacter < string.end(); ++currentCharacter)
	{
		if ( *currentCharacter == characterToBeCounted )
		{
			++result;
		}
	}
	return result;
}

bool StringTools::fileExists(const char* const fileName)
{
	std::ifstream file(fileName);
	bool result = file;
	return result;
}

bool StringTools::fileExists(const std::string& fileName)
{
	char* charFileName  = new char [fileName.size()+1];
	std::strcpy (charFileName, fileName.c_str());
	bool result = fileExists(charFileName);
	delete[] charFileName;
	return result;
}

std::map<std::string,std::vector<std::string> > StringTools::tabDelimitedSpreadsheetFileToStringMap(const std::string& tabDelimitedSpreadsheetFileName)
{
	const char rowDelim = '\n';
	const char fieldDelim = '\t';
	const char surplusRowDelim = '\r';

	std::map<std::string,std::vector<std::string> > result;
	std::vector<std::string> headerEntries;
	std::map<std::string,std::vector<std::string> >::iterator resultMapIterator;
	std::vector<std::string> emptyStringVector;
	std::string tmpString;
	int numberOfFieldDelims;
	std::istringstream iStringStream;

	char* inputFileName  = new char [tabDelimitedSpreadsheetFileName.size()+1];
	std::strcpy (inputFileName, tabDelimitedSpreadsheetFileName.c_str());
	std::ifstream spreadsheetFile(inputFileName);
	delete[] inputFileName;


	if( !(spreadsheetFile) )
	{
		std::string fileExistErrorMessageString = "Following file cannot be found:\n";
		fileExistErrorMessageString.append(tabDelimitedSpreadsheetFileName);
		fileExistErrorMessageString.append("\n");
		char* fileExistErrorMessageNotConst = new char [fileExistErrorMessageString.size()+1];
		std::strcpy (fileExistErrorMessageNotConst,fileExistErrorMessageString.c_str());
		const char *const fileExistErrorMessage = fileExistErrorMessageNotConst;
		throw(fileExistErrorMessage);
	}

	std::copy(std::istreambuf_iterator<char>(spreadsheetFile), std::istreambuf_iterator<char>(), std::back_inserter(tmpString));
	spreadsheetFile.close();
	std::replace(tmpString.begin(),tmpString.end(),surplusRowDelim,rowDelim);
	removeAdjacentDuplicatesOfCertainCharacter(tmpString,rowDelim);
	std::istringstream spreadsheet;
	spreadsheet.str(tmpString);
	tmpString.clear();

	for(bool firstLine = true; std::getline(spreadsheet,tmpString,rowDelim); )
	{
		removeAdjacentDuplicatesOfCertainCharacter(tmpString,fieldDelim);
		removeSingleCertainCharacterFromBeginning(tmpString,fieldDelim);
		removeSingleCertainCharacterFromEnd(tmpString,fieldDelim);
		iStringStream.clear();
		iStringStream.str( tmpString );
		numberOfFieldDelims = countCharacter(tmpString,fieldDelim);
		tmpString.clear();

		if(firstLine)
		{
			for(;std::getline(iStringStream,tmpString,fieldDelim);tmpString.clear())
			{
				headerEntries.push_back(tmpString);
				result[tmpString] = emptyStringVector;
			}
			firstLine = false;
		}
		else
		{
			if( numberOfFieldDelims >= (headerEntries.size()-1) )
			{
				for(std::vector<std::string>::const_iterator currentHeaderEntry = headerEntries.begin(); currentHeaderEntry < headerEntries.end(); ++currentHeaderEntry)
				{
					resultMapIterator = result.find(*currentHeaderEntry);
					std::getline(iStringStream,tmpString,fieldDelim);
					resultMapIterator->second.push_back(tmpString);
					tmpString.clear();
				}
			}
		}
	}

	if( (result.begin())->second.size() == 0 )
	{
		std::string noSufficientRowErrorMessageString = "The following file does not contain any row with the minimum number of entries defined by the header line:\n";
		noSufficientRowErrorMessageString.append(tabDelimitedSpreadsheetFileName);
		noSufficientRowErrorMessageString.append("\n");
		char* noSufficientRowErrorMessageNotConst = new char [noSufficientRowErrorMessageString.size()+1];
		std::strcpy (noSufficientRowErrorMessageNotConst,noSufficientRowErrorMessageString.c_str());
		const char *const noSufficientRowErrorMessage = noSufficientRowErrorMessageNotConst;
		throw(noSufficientRowErrorMessage);
	}

	return result;
}


std::vector<std::string> StringTools::splitString(const std::string& inputString, char delim)
{
    std::vector<std::string> result;
	std::istringstream iStringStream;
	iStringStream.str(inputString);
    std::string tmpString;
    while(std::getline(iStringStream, tmpString, delim))
    {
    	result.push_back(tmpString);
    }
    return result;
}

std::string StringTools::monthIntToString(int month)
{
	std::string result;
	switch (month)
	{
	case 1:
		result = "Jan";
		break;

	case 2:
		result = "Feb";
		break;

	case 3:
		result = "Mar";
		break;

	case 4:
		result = "Apr";
		break;

	case 5:
		result = "May";
		break;

	case 6:
		result = "Jun";
		break;

	case 7:
		result = "Jul";
		break;

	case 8:
		result = "Aug";
		break;

	case 9:
		result = "Sep";
		break;

	case 10:
		result = "Oct";
		break;

	case 11:
		result = "Nov";
		break;

	case 12:
		result = "Dec";
		break;

	default:
		const char *const errorMessage = "Invalid month integer. Possibilities are 1 to 12.";
		throw (errorMessage);
	}
	return result;
}

std::map< std::string, int> StringTools::createMapFromMonthStringToInt()
{
	std::map< std::string, int> result;
	result["Jan"] = 1;
	result["Feb"] = 2;
	result["Mar"] = 3;
	result["Apr"] = 4;
	result["May"] = 5;
	result["Jun"] = 6;
	result["Jul"] = 7;
	result["Aug"] = 8;
	result["Sep"] = 9;
	result["Oct"] = 10;
	result["Nov"] = 11;
	result["Dec"] = 12;
	return result;
}
std::map< std::string, int> StringTools::mapFromMonthStringToInt(StringTools::createMapFromMonthStringToInt());

int StringTools::monthStringToInt(const std::string& month)
{
	std::map< std::string, int>::const_iterator resultIterator = mapFromMonthStringToInt.find(month);
	if (resultIterator == mapFromMonthStringToInt.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(month);
		errorMessageAsString.append("\" not mapped to month. Possiblities are Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

bool StringTools::stringToBool(std::string inputString)
{
	bool result;

	if(inputString == "true" || inputString == "True" || inputString == "TRUE")
	{
		result = true;
	}
	else
	{
		if(inputString == "false" || inputString == "False" || inputString == "FALSE")
		{
			result = false;
		}
		else
		{
			std::string errorMessageAsString = "String \"";
			errorMessageAsString.append(inputString);
			errorMessageAsString.append("\" not recognisable as bool.");
			const char *const errorMessage = errorMessageAsString.c_str();
			throw(errorMessage);
		}
	}
	return result;
}

std::string StringTools::boolTo_string(bool inputBool)
{
	std::string result;
	if(inputBool)
	{
		result = "true";
	}
	else
	{
		result = "false";
	}
	return result;
}

std::string StringTools::boolTo_String(bool inputBool)
{
	std::string result;
	if(inputBool)
	{
		result = "True";
	}
	else
	{
		result = "False";
	}
	return result;
}

std::string StringTools::boolTo_STRING(bool inputBool)
{
	std::string result;
	if(inputBool)
	{
		result = "TRUE";
	}
	else
	{
		result = "FALSE";
	}
	return result;
}

std::vector<double> StringTools::stringVectorToDoubleVector(std::vector<std::string> inputStringVector)
{
	std::vector<double> result;
	double tmpValue;
	std::istringstream iStringStream;

	for(std::vector<std::string>::iterator currentStringVectorEntry = inputStringVector.begin(); currentStringVectorEntry < inputStringVector.end(); ++currentStringVectorEntry)
	{
		trimString( *currentStringVectorEntry );
		iStringStream.clear();
		iStringStream.str( *currentStringVectorEntry );
		iStringStream >> tmpValue;
		result.push_back(tmpValue);
	}

	return result;
}

std::vector<int> StringTools::stringVectorToIntVector(std::vector<std::string> inputStringVector)
{
	std::vector<int> result;
	int tmpValue;
	std::istringstream iStringStream;

	for(std::vector<std::string>::iterator currentStringVectorEntry = inputStringVector.begin(); currentStringVectorEntry < inputStringVector.end(); ++currentStringVectorEntry)
	{
		trimString( *currentStringVectorEntry );
		iStringStream.clear();
		iStringStream.str( *currentStringVectorEntry );
		iStringStream >> tmpValue;
		result.push_back(tmpValue);
	}

	return result;
}

std::vector<bool> StringTools::stringVectorToBoolVector(std::vector<std::string> inputStringVector)
{
	std::vector<bool> result;
	std::string tmpString;

	for(std::vector<std::string>::iterator currentStringVectorEntry = inputStringVector.begin(); currentStringVectorEntry < inputStringVector.end(); ++currentStringVectorEntry)
	{
		trimString( *currentStringVectorEntry );
		tmpString.clear();
		tmpString = *currentStringVectorEntry;
		trimString(tmpString);

		result.push_back( stringToBool(tmpString) );
	}

	return result;
}


}
