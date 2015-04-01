/*
 * OutputMethodType.cpp
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

#include "OutputMethodType.h"

#include <iostream>
#include <algorithm>

#include "StringTools.h"

namespace SedFlow {

OutputMethodType::OutputMethodType(std::string path, std::vector<std::string> outputFiles, bool writeLineEachTimeStep, double outputInterval, const std::vector<double>& explicitTimesForOutput, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods):
		path(path),
		outputFiles(outputFiles),
		writeLineEachTimeStep(writeLineEachTimeStep),
		outputInterval(outputInterval),
		explicitTimesForOutput(explicitTimesForOutput),
		precisionForOutput(precisionForOutput),
		overallParameters(overallParameters),
		overallMethods(overallMethods),
		riverSystemProperties(riverSystemProperties),
		riverSystemMethods(riverSystemMethods)
{
	if( 0 >= this->outputInterval )
	{
		const char *const errorMessage = "Zero or negative output interval";
		throw(errorMessage);
	}

	makePathEndWithOneSlash();
	std::sort(this->explicitTimesForOutput.begin(),this->explicitTimesForOutput.end());
	this->remainingTimesForOutputInReverseOrder.reserve(this->explicitTimesForOutput.size());
	for(std::vector<double>::reverse_iterator reverseIterator = this->explicitTimesForOutput.rbegin(); reverseIterator < this->explicitTimesForOutput.rend(); ++reverseIterator)
			{ this->remainingTimesForOutputInReverseOrder.push_back( *reverseIterator ); }
	this->timeOfLastOutput = this->overallParameters->getElapsedSeconds();
	while( !(this->remainingTimesForOutputInReverseOrder.empty()) && this->timeOfLastOutput >= this->remainingTimesForOutputInReverseOrder.back() )
		{ this->remainingTimesForOutputInReverseOrder.pop_back(); }
}

void OutputMethodType::makePathEndWithOneSlash()
{
	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);
}

bool OutputMethodType::isDueToWriteLineWithoutUpdatingParameters(double elapsedSeconds, double timeOfLastOutput, double outputInterval, const std::vector<double>& remainingTimesForOutputInReverseOrder)
{
	return ( elapsedSeconds >= (timeOfLastOutput + outputInterval) || (!(remainingTimesForOutputInReverseOrder.empty()) && elapsedSeconds >= remainingTimesForOutputInReverseOrder.back()) );
}

bool OutputMethodType::isDueToWriteLine(double elapsedSeconds, double& timeOfLastOutput, double outputInterval, std::vector<double>& remainingTimesForOutputInReverseOrder)
{
	 bool result = isDueToWriteLineWithoutUpdatingParameters(elapsedSeconds,timeOfLastOutput, outputInterval, remainingTimesForOutputInReverseOrder);
	 if(result)
	 {
		timeOfLastOutput = elapsedSeconds;
		while( !(remainingTimesForOutputInReverseOrder.empty()) && timeOfLastOutput >= remainingTimesForOutputInReverseOrder.back() )
			{ remainingTimesForOutputInReverseOrder.pop_back(); }
	 }
	 return result;
}

void OutputMethodType::writeOutputLineIfScheduled(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if (writeLineEachTimeStep)
	{
		this->forcedWriteOutputLine(allConstitutingOutputMethodTypes);
	}
	else
	{
		if (isDueToWriteLine(overallParameters->getElapsedSeconds(), timeOfLastOutput, outputInterval, remainingTimesForOutputInReverseOrder))
		{
			this->forcedWriteOutputLine(allConstitutingOutputMethodTypes);
		}
	}
}

}
