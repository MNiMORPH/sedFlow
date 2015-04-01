/*
 * OutputAccumulatedBedloadTransport.cpp
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

#include "OutputAccumulatedBedloadTransport.h"

#include <iostream>
#include <cstring>

namespace SedFlow {

OutputAccumulatedBedloadTransport::OutputAccumulatedBedloadTransport(std::vector<int> userCellIDsForOutput, std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, bool outputIncludingPoreVolume, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods, bool outputDetailedFractional):
		OutputMethodType(path, outputFiles, writeLineEachTimeStep, outputInterval, explicitTimesForOutput, precisionForOutput, overallParameters, overallMethods, riverSystemProperties, riverSystemMethods),
		userCellIDsForOutput(userCellIDsForOutput),
		fractionalGrainDiameters(overallParameters->getFractionalGrainDiameters()),
		fractionalGrainDiametersBegin(fractionalGrainDiameters.begin()),
		fractionalGrainDiametersEnd(fractionalGrainDiameters.end()),
		outputIncludingPoreVolume(outputIncludingPoreVolume),
		outputDetailedFractional(outputDetailedFractional)
{
	this->typeOfOutputMethod = CombinerVariables::OutputAccumulatedBedloadTransport;
	makePathEndWithOneSlash();
	overallVolumeOutputFileAsString = this->path;
	detailedFractionalOutputFileAsString = this->path;
	switch (outputFiles.size())
	{
	case 1:
		overallVolumeOutputFileAsString.append(outputFiles[0]);
		detailedFractionalOutputFileAsString.append(outputFiles[0]);
		overallVolumeOutputFileAsString.append("_");
		detailedFractionalOutputFileAsString.append("_");
		//The missing break is on purpose here.

	case 0:
		overallVolumeOutputFileAsString.append("ABT-OverallVolume");
		detailedFractionalOutputFileAsString.append("ABT-DetailedFractional");
		break;

	case 2:
		overallVolumeOutputFileAsString.append(outputFiles[0]);
		detailedFractionalOutputFileAsString.append(outputFiles[1]);
		break;

	default:
		const char *const errorMessage = "For OutputRegularRiverReachProperties exactly one (no more, no less) output file name is needed.";
		throw(errorMessage);
	}

	overallVolumeOutputFileAsString.append(".txt");
	detailedFractionalOutputFileAsString.append(".txt");

	overallVolumeOutputFile = new char [overallVolumeOutputFileAsString.size()+1];
	std::strcpy (overallVolumeOutputFile, overallVolumeOutputFileAsString.c_str());
	detailedFractionalOutputFile = new char [detailedFractionalOutputFileAsString.size()+1];
	std::strcpy (detailedFractionalOutputFile, detailedFractionalOutputFileAsString.c_str());

	overallVolumeOFileStream.precision(precisionForOutput);
	overallVolumeOFileStream << std::scientific;
	detailedFractionalOFileStream.precision(precisionForOutput);
	detailedFractionalOFileStream << std::scientific;

	if(userCellIDsForOutput.empty())
	{
		for(std::vector<RiverReachProperties>::const_iterator currentRiverReachProperties = riverSystemProperties->regularRiverSystemProperties.cellProperties.begin(); currentRiverReachProperties < riverSystemProperties->regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachProperties)
		{
			if ( !( currentRiverReachProperties->isMargin() ) )
			{
				cellPointersForOutput.push_back( &( currentRiverReachProperties->regularRiverReachProperties ) );
				this->userCellIDsForOutput.push_back( riverSystemProperties->regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( currentRiverReachProperties->getCellID() ) );
			}
		}
	}
	else
	{
		for(std::vector<int>::const_iterator currentUserCellID = userCellIDsForOutput.begin(); currentUserCellID < userCellIDsForOutput.end(); ++currentUserCellID)
		{
			cellPointersForOutput.push_back( &( (riverSystemProperties->regularRiverSystemProperties.cellProperties.at( riverSystemProperties->regularRiverSystemProperties.getRealCellIDcorrespondingToUserCellID( *currentUserCellID ) )).regularRiverReachProperties ) );
		}
	}

	Grains grainsSample = (cellPointersForOutput.back())->strataPerUnitBedSurface.at(0);

	typesOfGrainsOrderForOutput = grainsSample.getTypesOfGrains();

	grainsSample *= 0.0;
	this->accumulatedBedloadTransport = std::vector<Grains>(cellPointersForOutput.size(),grainsSample);

	if( this->outputIncludingPoreVolume )
	{
		ABTIncrementProperty = CombinerVariables::erosionIncludingPoreVolume;
	}
	else
	{
		ABTIncrementProperty = CombinerVariables::erosion;
	}
}

OutputAccumulatedBedloadTransport::~OutputAccumulatedBedloadTransport()
{
	delete[] overallVolumeOutputFile;
	delete[] detailedFractionalOutputFile;
}

OutputMethodType* OutputAccumulatedBedloadTransport::createOutputMethodTypePointerCopy() const
{
	OutputMethodType* result = new OutputAccumulatedBedloadTransport(this->userCellIDsForOutput, this->path, this->outputFiles, this->writeLineEachTimeStep, this->outputInterval, this->explicitTimesForOutput, this->precisionForOutput, this->outputIncludingPoreVolume, this->overallParameters, this->overallMethods, this->riverSystemProperties, this->riverSystemMethods, this->outputDetailedFractional);
	return result;
}

ConstructionVariables OutputAccumulatedBedloadTransport::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
	result.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputAccumulatedBedloadTransport);
	std::vector<int> intVector;
	for(std::vector<int>::const_iterator iterator = userCellIDsForOutput.begin(); iterator < userCellIDsForOutput.end(); ++iterator)
			{ intVector.push_back(*iterator); }
	result.labelledInts["reachIDsForOutput"] = intVector;
	std::vector<double> doubleVector;
	doubleVector.push_back(outputInterval);
	result.labelledDoubles["outputInterval"] = doubleVector;
	doubleVector.clear();
	for(std::vector<double>::const_iterator iterator = explicitTimesForOutput.begin(); iterator < explicitTimesForOutput.end(); ++iterator)
			{ doubleVector.push_back(*iterator); }
	result.labelledDoubles["explicitTimesForOutput"] = doubleVector;
	intVector.clear();
	intVector.push_back(precisionForOutput);
	result.labelledInts["precisionForOutput"] = intVector;
	std::vector<bool> boolVector;
	boolVector.push_back(writeLineEachTimeStep);
	result.labelledBools["writeLineEachTimeStep"] = boolVector;
	boolVector.clear();
	boolVector.push_back(outputDetailedFractional);
	result.labelledBools["outputDetailedFractional"] = boolVector;
	boolVector.clear();
	boolVector.push_back(outputIncludingPoreVolume);
	result.labelledBools["outputIncludingPoreVolume"] = boolVector;
	std::vector<std::string> stringVector;
	stringVector.push_back(path);
	result.labelledStrings["path"] = stringVector;
	stringVector.clear();
	for(std::vector<std::string>::const_iterator iterator = outputFiles.begin(); iterator < outputFiles.end(); ++iterator)
			{ stringVector.push_back(*iterator); }
	result.labelledStrings["outputFiles"] = stringVector;
	return result;
}

void OutputAccumulatedBedloadTransport::initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	std::ostringstream diameterStringStream;

	overallVolumeOFileStream.open(overallVolumeOutputFile, std::ios::out | std::ios::trunc);
	//Write header line
	overallVolumeOFileStream << "ElapsedSeconds";

	if(this->outputDetailedFractional)
	{
		detailedFractionalOFileStream.open(detailedFractionalOutputFile, std::ios::out | std::ios::trunc);
		//Write header line
		detailedFractionalOFileStream << "ElapsedSeconds";
	}

	for(std::vector<int>::const_iterator currentUserCellID = this->userCellIDsForOutput.begin(); currentUserCellID < this->userCellIDsForOutput.end(); ++currentUserCellID)
	{
		overallVolumeOFileStream << "\tAccumulatedBedloadTransportOverallVolume_Reach" << *currentUserCellID;

		if(this->outputDetailedFractional)
		{
			for(std::vector<CombinerVariables::TypesOfGrains>::const_iterator currentTypeOfGrains = typesOfGrainsOrderForOutput.begin(); currentTypeOfGrains < typesOfGrainsOrderForOutput.end() ; ++currentTypeOfGrains)
			{
				for(std::vector<double>::const_iterator currentDiameter = fractionalGrainDiametersBegin; currentDiameter < fractionalGrainDiametersEnd; ++currentDiameter)
				{
					diameterStringStream.str("");
					diameterStringStream.clear();
					diameterStringStream << *currentDiameter;
					diameterStringStream.flush();
					detailedFractionalOFileStream << "\tAccumulatedBedloadTransport_Reach" << *currentUserCellID << "_" << CombinerVariables::typeOfGrainsToString(*currentTypeOfGrains) << "_Fraction" << diameterStringStream.str() << "m";
				}
			}
		}
	}
	overallVolumeOFileStream << "\n";
	overallVolumeOFileStream.close();

	if(this->outputDetailedFractional)
	{
		detailedFractionalOFileStream << "\n";
		detailedFractionalOFileStream.close();
	}
}

void OutputAccumulatedBedloadTransport::update(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	currentABT = this->accumulatedBedloadTransport.begin();
	for(std::vector<const RegularRiverReachProperties*>::const_iterator currentRiverReachProperties = this->cellPointersForOutput.begin(); ( (currentABT < this->accumulatedBedloadTransport.end()) && (currentRiverReachProperties < this->cellPointersForOutput.end()) ) ; ++currentRiverReachProperties,++currentABT)
	{
		*currentABT += (*currentRiverReachProperties)->getGrainsProperty(ABTIncrementProperty);
	}
}

void OutputAccumulatedBedloadTransport::forcedWriteOutputLine(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	overallVolumeOFileStream.open(overallVolumeOutputFile, std::ios::out | std::ios::app);
	overallVolumeOFileStream << overallParameters->getElapsedSeconds();

	if(this->outputDetailedFractional)
	{
		detailedFractionalOFileStream.open(detailedFractionalOutputFile, std::ios::out | std::ios::app);
		detailedFractionalOFileStream << overallParameters->getElapsedSeconds();
	}

	for(currentConstABT = this->accumulatedBedloadTransport.begin(); currentConstABT < this->accumulatedBedloadTransport.end(); ++currentConstABT)
	{
		overallVolumeOFileStream << "\t" << currentConstABT->getOverallVolume();

		if(this->outputDetailedFractional)
		{
			for(currentTypeOfGrainsIterator = typesOfGrainsOrderForOutput.begin(); currentTypeOfGrainsIterator < typesOfGrainsOrderForOutput.end(); ++currentTypeOfGrainsIterator)
			{
				currentGrainTypePointer = currentConstABT->getSingleGrainTypeConstPointer(*currentTypeOfGrainsIterator);
				currentFractionalAbundances = currentGrainTypePointer->getFractions();
				for(currentFractionIterator = currentFractionalAbundances.begin(); currentFractionIterator < currentFractionalAbundances.end(); ++currentFractionIterator)
						{ detailedFractionalOFileStream << "\t" << *currentFractionIterator; }
			}
		}
	}
	overallVolumeOFileStream << "\n";
	overallVolumeOFileStream.close();

	if(this->outputDetailedFractional)
	{
		detailedFractionalOFileStream << "\n";
		detailedFractionalOFileStream.close();
	}
}

void OutputAccumulatedBedloadTransport::finaliseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if ( !(writeLineEachTimeStep) ) { forcedWriteOutputLine(allConstitutingOutputMethodTypes); }
}

}
