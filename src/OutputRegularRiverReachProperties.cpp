/*
 * OutputRegularRiverReachProperties.cpp
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

#include "OutputRegularRiverReachProperties.h"

#include <iostream>
#include <cstring>

namespace SedFlow {


OutputRegularRiverReachProperties::OutputRegularRiverReachProperties(std::vector<CombinerVariables::TypesOfRegularRiverReachProperties> regularRiverReachPropertiesForOutput, std::vector<int> userCellIDsForOutput, bool outputTimeStepLength, bool outputInitialValues, bool printUpstreamMargins, bool printDownstreamMargin, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, bool useSecondaryOutputInterval, int referenceCellUserCellID, CombinerVariables::TypesOfRegularRiverReachProperties referenceProperty, double thresholdToBeExceeded, double secondaryOutputInterval, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods):
		OutputMethodType(path, outputFiles, writeLineEachTimeStep, outputInterval, explicitTimesForOutput, precisionForOutput, overallParameters, overallMethods, riverSystemProperties, riverSystemMethods),
		regularRiverReachPropertiesForOutput(regularRiverReachPropertiesForOutput),
		userCellIDsForOutput(userCellIDsForOutput),
		outputTimeStepLength(outputTimeStepLength),
		outputInitialValues(outputInitialValues),
		printUpstreamMargins(printUpstreamMargins),
		printDownstreamMargin(printDownstreamMargin),
		useSecondaryOutputInterval(useSecondaryOutputInterval),
		primaryOutputInterval(outputInterval),
		referenceCellUserCellID(referenceCellUserCellID),
		referenceCell(NULL),
		referenceProperty(referenceProperty),
		thresholdToBeExceeded(thresholdToBeExceeded),
		secondaryOutputInterval(secondaryOutputInterval),
		fractionalGrainDiameters(overallParameters->getFractionalGrainDiameters()),
		fractionalGrainDiametersBegin(fractionalGrainDiameters.begin()),
		fractionalGrainDiametersEnd(fractionalGrainDiameters.end())
{
	this->typeOfOutputMethod = CombinerVariables::OutputRegularRiverReachProperties;
	if (outputFiles.size() != 1)
	{
		const char *const errorMessage = "For OutputRegularRiverReachProperties exactly one (no more, no less) output file name is needed.";
		throw(errorMessage);
	}
	makePathEndWithOneSlash();
	outputFileAsString = this->path;
	outputFileAsString.append( outputFiles.at(0) );
	outputFile = new char [outputFileAsString.size()+1];
	std::strcpy (outputFile, outputFileAsString.c_str());

	oFileStream.precision(precisionForOutput);
	oFileStream << std::scientific;
	oStringStream.precision(precisionForOutput);
	oStringStream << std::scientific;

	if(this->printUpstreamMargins)
	{
		for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = riverSystemProperties->regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < riverSystemProperties->regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachProperties)
		{
			if ( currentRiverReachProperties->isUpstreamMargin() )
			{
				cellPointersForOutput.push_back( &( currentRiverReachProperties->regularRiverReachProperties ) );
				oStringStream.str("");
				oStringStream.clear();
				oStringStream << "UpstreamMarginAtReach" << currentRiverReachProperties->getDownstreamUserCellID() << std::flush;
				cellIDLabels.push_back(oStringStream.str());
			}
		}
	}

	if(userCellIDsForOutput.empty())
	{
		for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = riverSystemProperties->regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < riverSystemProperties->regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachProperties)
		{
			if ( !( currentRiverReachProperties->isMargin() ) )
			{
				cellPointersForOutput.push_back( &( currentRiverReachProperties->regularRiverReachProperties ) );
				this->userCellIDsForOutput.push_back( riverSystemProperties->regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( currentRiverReachProperties->getCellID() ) );
				oStringStream.str("");
				oStringStream.clear();
				oStringStream << "Reach" << this->userCellIDsForOutput.back() << std::flush;
				cellIDLabels.push_back(oStringStream.str());
			}
		}
	}
	else
	{
		for(std::vector<int>::const_iterator currentUserCellID = userCellIDsForOutput.begin(); currentUserCellID < userCellIDsForOutput.end(); ++currentUserCellID)
		{
			cellPointersForOutput.push_back( &( (riverSystemProperties->regularRiverSystemProperties.cellProperties.at( riverSystemProperties->regularRiverSystemProperties.getRealCellIDcorrespondingToUserCellID( *currentUserCellID ) )).regularRiverReachProperties ) );
			oStringStream.str("");
			oStringStream.clear();
			oStringStream << "Reach" << *currentUserCellID << std::flush;
			cellIDLabels.push_back(oStringStream.str());
		}
	}

	if(this->printDownstreamMargin)
	{
		cellPointersForOutput.push_back( &( (riverSystemProperties->regularRiverSystemProperties.cellProperties.back()).regularRiverReachProperties ) );
		cellIDLabels.push_back("DownstreamMargin");
	}

	typesOfGrainsOrderForOutput = ((cellPointersForOutput.at(0))->strataPerUnitBedSurface.at(0)).getTypesOfGrains();

	if(this->useSecondaryOutputInterval)
	{
		referenceCell = &( (riverSystemProperties->regularRiverSystemProperties.cellProperties.at( riverSystemProperties->regularRiverSystemProperties.getRealCellIDcorrespondingToUserCellID( referenceCellUserCellID ) )).regularRiverReachProperties );
	}

}

OutputRegularRiverReachProperties::~OutputRegularRiverReachProperties()
{
	delete[] outputFile;
}

OutputMethodType* OutputRegularRiverReachProperties::createOutputMethodTypePointerCopy() const
{
	OutputMethodType* result = new OutputRegularRiverReachProperties(this->regularRiverReachPropertiesForOutput, this->userCellIDsForOutput, this->outputTimeStepLength, this->outputInitialValues, this->printUpstreamMargins, this->printDownstreamMargin, this->path, this->outputFiles, this->writeLineEachTimeStep, this->outputInterval, this->explicitTimesForOutput, this->precisionForOutput, this->useSecondaryOutputInterval, this->referenceCellUserCellID, this->referenceProperty, this->thresholdToBeExceeded, this->secondaryOutputInterval, this->overallParameters, this->overallMethods, this->riverSystemProperties, this->riverSystemMethods);
	return result;
}


ConstructionVariables OutputRegularRiverReachProperties::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
	result.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputRegularRiverReachProperties);
	std::vector<double> doubleVector;
	doubleVector.push_back(primaryOutputInterval);
	result.labelledDoubles["outputInterval"] = doubleVector;
	doubleVector.clear();
	for(std::vector<double>::const_iterator iterator = explicitTimesForOutput.begin(); iterator < explicitTimesForOutput.end(); ++iterator)
			{ doubleVector.push_back(*iterator); }
	result.labelledDoubles["explicitTimesForOutput"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(precisionForOutput);
	result.labelledInts["precisionForOutput"] = intVector;
	intVector.clear();
	for(std::vector<int>::const_iterator iterator = userCellIDsForOutput.begin(); iterator < userCellIDsForOutput.end(); ++iterator)
			{ intVector.push_back(*iterator); }
	result.labelledInts["reachIDsForOutput"] = intVector;
	std::vector<bool> boolVector;
	boolVector.push_back(writeLineEachTimeStep);
	result.labelledBools["writeLineEachTimeStep"] = boolVector;
	boolVector.clear();
	boolVector.push_back(outputTimeStepLength);
	result.labelledBools["outputTimeStepLength"] = boolVector;
	boolVector.clear();
	boolVector.push_back(outputInitialValues);
	result.labelledBools["outputInitialValues"] = boolVector;
	boolVector.clear();
	boolVector.push_back(printUpstreamMargins);
	result.labelledBools["printUpstreamMargins"] = boolVector;
	boolVector.clear();
	boolVector.push_back(printDownstreamMargin);
	result.labelledBools["printDownstreamMargin"] = boolVector;
	boolVector.clear();
	boolVector.push_back(useSecondaryOutputInterval);
	result.labelledBools["useSecondaryOutputInterval"] = boolVector;
	intVector.clear();
	intVector.push_back(referenceCellUserCellID);
	result.labelledInts["referenceCellUserCellID"] = intVector;
	std::vector<std::string> stringVector;
	stringVector.push_back(CombinerVariables::typeOfRegularRiverReachPropertiesToString(referenceProperty));
	result.labelledStrings["referenceProperty"] = stringVector;
	doubleVector.clear();
	doubleVector.push_back(thresholdToBeExceeded);
	result.labelledDoubles["thresholdToBeExceeded"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(secondaryOutputInterval);
	result.labelledDoubles["secondaryOutputInterval"] = doubleVector;
	stringVector.clear();
	stringVector.push_back(path);
	result.labelledStrings["path"] = stringVector;
	stringVector.clear();
	for(std::vector<std::string>::const_iterator iterator = outputFiles.begin(); iterator < outputFiles.end(); ++iterator)
			{ stringVector.push_back(*iterator); }
	result.labelledStrings["outputFiles"] = stringVector;
	stringVector.clear();
	for(std::vector<CombinerVariables::TypesOfRegularRiverReachProperties>::const_iterator iterator = regularRiverReachPropertiesForOutput.begin(); iterator < regularRiverReachPropertiesForOutput.end(); ++iterator)
			{ stringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(*iterator) ); }
	result.labelledStrings["regularRiverReachPropertiesForOutput"] = stringVector;
	return result;
}

void OutputRegularRiverReachProperties::initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	int numberOfStrataLayers = (cellPointersForOutput.at(0))->strataPerUnitBedSurface.size();
	std::ostringstream diameterStringStream;

	oFileStream.open(outputFile, std::ios::out | std::ios::trunc);
	//Write header line
	oFileStream << "ElapsedSeconds";
	if(outputTimeStepLength) { oFileStream << "\t" << "CurrentTimeStepLength[sec]"; }
	for(std::vector<CombinerVariables::TypesOfRegularRiverReachProperties>::const_iterator currentPropertyType = regularRiverReachPropertiesForOutput.begin(); currentPropertyType < regularRiverReachPropertiesForOutput.end(); ++currentPropertyType)
	{
		if (CombinerVariables::regularRiverReachPropertyIsGrains(*currentPropertyType))
		{
			if( *currentPropertyType == CombinerVariables::strataPerUnitBedSurface )
			{
				for(std::vector<std::string>::const_iterator currentCellIDLabel = cellIDLabels.begin(); currentCellIDLabel < cellIDLabels.end(); ++currentCellIDLabel)
				{
					for(int currentStrataLayer = 0; currentStrataLayer < numberOfStrataLayers; ++currentStrataLayer)
					{
						for(std::vector<CombinerVariables::TypesOfGrains>::const_iterator currentTypeOfGrains = typesOfGrainsOrderForOutput.begin(); currentTypeOfGrains < typesOfGrainsOrderForOutput.end() ; ++currentTypeOfGrains)
						{
							for(std::vector<double>::const_iterator currentDiameter = fractionalGrainDiametersBegin; currentDiameter < fractionalGrainDiametersEnd; ++currentDiameter)
							{
								diameterStringStream.str("");
								diameterStringStream.clear();
								diameterStringStream << *currentDiameter;
								diameterStringStream.flush();
								oFileStream << "\t" << CombinerVariables::typeOfRegularRiverReachPropertiesToString(*currentPropertyType) << "_" << *currentCellIDLabel << "_Layer" << currentStrataLayer << "_" << CombinerVariables::typeOfGrainsToString(*currentTypeOfGrains) << "_Fraction" << diameterStringStream.str() << "m";
							}
						}
					}
				}
			}
			else
			{
				for(std::vector<std::string>::const_iterator currentCellIDLabel = cellIDLabels.begin(); currentCellIDLabel < cellIDLabels.end(); ++currentCellIDLabel)
				{
					for(std::vector<CombinerVariables::TypesOfGrains>::const_iterator currentTypeOfGrains = typesOfGrainsOrderForOutput.begin(); currentTypeOfGrains < typesOfGrainsOrderForOutput.end() ; ++currentTypeOfGrains)
					{
						for(std::vector<double>::const_iterator currentDiameter = fractionalGrainDiametersBegin; currentDiameter < fractionalGrainDiametersEnd; ++currentDiameter)
						{
							diameterStringStream.str("");
							diameterStringStream.clear();
							diameterStringStream << *currentDiameter;
							diameterStringStream.flush();
							oFileStream << "\t" << CombinerVariables::typeOfRegularRiverReachPropertiesToString(*currentPropertyType) << "_" << *currentCellIDLabel << "_" << CombinerVariables::typeOfGrainsToString(*currentTypeOfGrains) << "_Fraction" << diameterStringStream.str() << "m";
						}
					}
				}
			}
		}
		else
		{
			for(std::vector<std::string>::const_iterator currentCellIDLabel = cellIDLabels.begin(); currentCellIDLabel < cellIDLabels.end(); ++currentCellIDLabel)
			{ oFileStream << "\t" << CombinerVariables::typeOfRegularRiverReachPropertiesToString(*currentPropertyType) << "_" << *currentCellIDLabel; }
		}
	}
	oFileStream << "\n";
	oFileStream.close();

	if(outputInitialValues) { forcedWriteOutputLine(allConstitutingOutputMethodTypes); }
}

void OutputRegularRiverReachProperties::update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if(useSecondaryOutputInterval)
	{
		if( referenceCell->getDoubleProperty(referenceProperty) >= thresholdToBeExceeded )
		{
			this->outputInterval = secondaryOutputInterval;
		}
		else
		{
			this->outputInterval = primaryOutputInterval;
		}
	}
}

void OutputRegularRiverReachProperties::forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	oFileStream.open(outputFile, std::ios::out | std::ios::app);
	oFileStream << overallParameters->getElapsedSeconds();
	if(outputTimeStepLength) { oFileStream << "\t" << overallParameters->getCurrentTimeStepLengthInSeconds(); }
	for(std::vector<CombinerVariables::TypesOfRegularRiverReachProperties>::const_iterator currentPropertyType = regularRiverReachPropertiesForOutput.begin(); currentPropertyType < regularRiverReachPropertiesForOutput.end(); ++currentPropertyType)
	{
		oFileStream << "\t";
		if ( CombinerVariables::regularRiverReachPropertyIsGrains(*currentPropertyType) )
		{
			for(currentCellPointerIterator = cellPointersForOutput.begin(); currentCellPointerIterator < (cellPointersForOutput.end()-1); ++currentCellPointerIterator)
			{
				oFileStream << printGrainsProperty(*currentCellPointerIterator, *currentPropertyType) << "\t";
			}
			oFileStream << printGrainsProperty(cellPointersForOutput.back(), *currentPropertyType);
		}
		else
		{
			for(currentCellPointerIterator = cellPointersForOutput.begin(); currentCellPointerIterator < (cellPointersForOutput.end()-1); ++currentCellPointerIterator)
			{
				oFileStream << (*currentCellPointerIterator)->getDoubleProperty(*currentPropertyType) << "\t";
			}
			oFileStream << (cellPointersForOutput.back())->getDoubleProperty(*currentPropertyType);
		}
	}
	oFileStream << "\n";
	oFileStream.close();
}

void OutputRegularRiverReachProperties::finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if ( !(writeLineEachTimeStep) ) { forcedWriteOutputLine(allConstitutingOutputMethodTypes); }
}

void OutputRegularRiverReachProperties::appendGrainstoOStringStream(const Grains& toPrint)
{
	for(currentTypeOfGrainsIterator = typesOfGrainsOrderForOutput.begin(); currentTypeOfGrainsIterator < (typesOfGrainsOrderForOutput.end()-1); ++currentTypeOfGrainsIterator)
	{
		currentGrainTypePointer = toPrint.getSingleGrainTypeConstPointer(*currentTypeOfGrainsIterator);
		currentFractionalAbundances = currentGrainTypePointer->getFractions();
		for(currentFractionIterator = currentFractionalAbundances.begin(); currentFractionIterator < currentFractionalAbundances.end(); ++currentFractionIterator)
				{ oStringStream << *currentFractionIterator << "\t"; }
	}
	currentGrainTypePointer = toPrint.getSingleGrainTypeConstPointer(typesOfGrainsOrderForOutput.back());
	currentFractionalAbundances = currentGrainTypePointer->getFractions();
	for(currentFractionIterator = currentFractionalAbundances.begin(); currentFractionIterator < (currentFractionalAbundances.end()-1); ++currentFractionIterator)
			{ oStringStream << *currentFractionIterator << "\t"; }
	oStringStream << *currentFractionIterator;
	oStringStream.flush();
}

std::string OutputRegularRiverReachProperties::printGrains(const Grains& toPrint)
{
	oStringStream.str("");
	oStringStream.clear();
	appendGrainstoOStringStream(toPrint);
	oStringStream.flush();
	return oStringStream.str();
}

std::string OutputRegularRiverReachProperties::printStrata(const std::vector<Grains>& toPrint)
{
	oStringStream.str("");
	oStringStream.clear();
	for(currentGrainsIterator = toPrint.begin(); currentGrainsIterator < (toPrint.end()-1); ++currentGrainsIterator)
	{
		appendGrainstoOStringStream(*currentGrainsIterator);
		oStringStream << "\t";
	}
	appendGrainstoOStringStream(toPrint.back());
	oStringStream.flush();
	return oStringStream.str();
}

std::string OutputRegularRiverReachProperties::printGrainsProperty(const RegularRiverReachProperties* cellPointer, CombinerVariables::TypesOfRegularRiverReachProperties typeOfProperty)
{
	std::string result;
	double multiplicationFactor;
	std::vector<Grains> strataForPrinting;
	switch(typeOfProperty)
	{
	case CombinerVariables::strataPerUnitBedSurface:
		result = printStrata(cellPointer->strataPerUnitBedSurface);
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		multiplicationFactor = 1.0 / (1.0 - overallParameters->getPoreVolumeFraction());
		strataForPrinting = cellPointer->strataPerUnitBedSurface;
		for(std::vector<Grains>::iterator currentStrataLayer = strataForPrinting.begin(); currentStrataLayer < strataForPrinting.end(); ++currentStrataLayer)
		{
			*currentStrataLayer *= multiplicationFactor;
		}
		result = printStrata(strataForPrinting);
		break;

	default:
		result = printGrains(cellPointer->getGrainsProperty(typeOfProperty));
		break;
	}
	return result;
}

}
