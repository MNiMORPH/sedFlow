/*
 * OutputSimulationSetup.cpp
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

#include "OutputSimulationSetup.h"

#include <fstream>
#include <sstream>
#include <cstring>
#include <ctime>

#define myStringify(s) myStringifySubsidiary(s)
#define myStringifySubsidiary(s) #s

#include "StringTools.h"

namespace SedFlow {

std::string OutputSimulationSetup::boolToString(bool inputBool) const
{
	return StringTools::boolTo_string(inputBool);
}

OutputSimulationSetup::OutputSimulationSetup(const std::vector<CombinerVariables::TypesOfCombinersAndInterfaces>& setupPropertiesForOutput, bool printSimulationID, const std::string& simulationID, bool printSimulationName, const std::string& simulationName, bool printStartingTime, bool printModelVersion, std::string path, std::vector<std::string> outputFiles, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods):
	OutputMethodType(path, outputFiles, false, 444.4, std::vector<double>(), precisionForOutput, overallParameters, overallMethods, riverSystemProperties, riverSystemMethods),
	setupPropertiesForOutput(setupPropertiesForOutput),
	printSimulationID(printSimulationID),
	simulationID(simulationID),
	printSimulationName(printSimulationName),
	simulationName(simulationName),
	printStartingTime(printStartingTime),
	printModelVersion(printModelVersion)
{
	this->typeOfOutputMethod = CombinerVariables::OutputSimulationSetup;
	std::string compilationDate = __DATE__;
	StringTools::removeAdjacentDuplicatesOfCertainCharacter(compilationDate,' ');
	std::vector<std::string> splittedCompilationDate = StringTools::splitString(compilationDate,' ');
	this->compilationMonth = splittedCompilationDate.at(0);
	std::istringstream iStringStream;
	iStringStream.str( splittedCompilationDate.at(1) );
	iStringStream >> this->compilationDay;
	iStringStream.str("");
	iStringStream.clear();
	iStringStream.str( splittedCompilationDate.at(2) );
	iStringStream >> this->compilationYear;
}

OutputSimulationSetup::OutputSimulationSetup(const std::vector<CombinerVariables::TypesOfCombinersAndInterfaces>& setupPropertiesForOutput, bool printSimulationID, const std::string& simulationID, bool printSimulationName, const std::string& simulationName, bool printStartingTime, bool printModelVersion, int compilationYear, const std::string& compilationMonth, int compilationDay, std::string path, std::vector<std::string> outputFiles, int precisionForOutput, const OverallParameters* overallParameters, const OverallMethods* overallMethods, const RiverSystemProperties* riverSystemProperties, const RiverSystemMethods* riverSystemMethods):
	OutputMethodType(path, outputFiles, false, 444.4, std::vector<double>(), precisionForOutput, overallParameters, overallMethods, riverSystemProperties, riverSystemMethods),
	setupPropertiesForOutput(setupPropertiesForOutput),
	printSimulationID(printSimulationID),
	simulationID(simulationID),
	printSimulationName(printSimulationName),
	simulationName(simulationName),
	printStartingTime(printStartingTime),
	printModelVersion(printModelVersion),
	compilationYear(compilationYear),
	compilationMonth(compilationMonth),
	compilationDay(compilationDay)
{
	this->typeOfOutputMethod = CombinerVariables::OutputSimulationSetup;
}

OutputSimulationSetup::~OutputSimulationSetup(){}

OutputMethodType* OutputSimulationSetup::createOutputMethodTypePointerCopy() const
{
	OutputMethodType* result = new OutputSimulationSetup(this->setupPropertiesForOutput, this->printSimulationID, this->simulationID, this->printSimulationName, this->simulationName, this->printStartingTime, this->printModelVersion, this->compilationYear, this->compilationMonth, this->compilationDay, this->path, this->outputFiles, this->precisionForOutput, this->overallParameters, this->overallMethods, this->riverSystemProperties, this->riverSystemMethods);
	return result;
}


ConstructionVariables OutputSimulationSetup::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
	result.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputSimulationSetup);
	std::vector<std::string> stringVector;
	stringVector.push_back(path);
	result.labelledStrings["path"] = stringVector;
	stringVector.clear();
	for(std::vector<std::string>::const_iterator iterator = outputFiles.begin(); iterator < outputFiles.end(); ++iterator)
			{ stringVector.push_back(*iterator); }
	result.labelledStrings["outputFiles"] = stringVector;
	std::vector<bool> boolVector;
	boolVector.push_back(writeLineEachTimeStep);
	result.labelledBools["writeLineEachTimeStep"] = boolVector;
	std::vector<double> doubleVector;
	doubleVector.push_back(outputInterval);
	result.labelledDoubles["outputInterval"] = doubleVector;
	doubleVector.clear();
	for(std::vector<double>::const_iterator iterator = explicitTimesForOutput.begin(); iterator < explicitTimesForOutput.end(); ++iterator)
			{ doubleVector.push_back(*iterator); }
	result.labelledDoubles["explicitTimesForOutput"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(precisionForOutput);
	result.labelledInts["precisionForOutput"] = intVector;
	stringVector.clear();
	for(std::vector<CombinerVariables::TypesOfCombinersAndInterfaces>::const_iterator iterator = setupPropertiesForOutput.begin(); iterator < setupPropertiesForOutput.end(); ++iterator)
			{ stringVector.push_back( CombinerVariables::typeOfCombinersAndInterfacesToString(*iterator) ); }
	result.labelledStrings["setupPropertiesForOutput"] = stringVector;
	boolVector.clear();
	boolVector.push_back(printSimulationID);
	result.labelledBools["printSimulationID"] = boolVector;
	stringVector.clear();
	stringVector.push_back(simulationID);
	result.labelledStrings["simulationID"] = stringVector;
	boolVector.clear();
	boolVector.push_back(printSimulationName);
	result.labelledBools["printSimulationName"] = boolVector;
	stringVector.clear();
	stringVector.push_back(simulationName);
	result.labelledStrings["simulationName"] = stringVector;
	boolVector.clear();
	boolVector.push_back(printStartingTime);
	result.labelledBools["printStartingTime"] = boolVector;
	boolVector.clear();
	boolVector.push_back(printModelVersion);
	result.labelledBools["printModelVersion"] = boolVector;
	intVector.clear();
	intVector.push_back(compilationYear);
	result.labelledInts["compilationYear"] = intVector;
	stringVector.clear();
	stringVector.push_back(compilationMonth);
	result.labelledStrings["compilationMonth"] = stringVector;
	intVector.clear();
	intVector.push_back(compilationDay);
	result.labelledInts["compilationDay"] = intVector;
	return result;
}

void OutputSimulationSetup::initialiseOutput(const std::vector<OutputMethodType*>& allConstitutingOutputMethodTypes)
{
	if (outputFiles.size() != 1)
	{
		const char *const numberOfFilesErrorMessage = "For OutputSimulationSetup exactly one (no more, no less) output file name is needed.";
		throw(numberOfFilesErrorMessage);
	}
	makePathEndWithOneSlash();
	std::string outputFileAsString (this->path);
	outputFileAsString.append( outputFiles.at(0) );
	char* outputFile = new char [outputFileAsString.size()+1];
	std::strcpy (outputFile, outputFileAsString.c_str());

	std::ofstream oFileStream;
	oFileStream.precision(precisionForOutput);
	oFileStream << std::scientific;

	oFileStream.open(outputFile, std::ios::out | std::ios::trunc);
	delete[] outputFile;

	if( this->printSimulationID )
		{ oFileStream << "Simulation-ID:\t" << this->simulationID << std::endl; }

	if( this->printSimulationName )
		{ oFileStream << "Simulation name:\t" << this->simulationName << std::endl; }

	if( this->printStartingTime )
	{
	    time_t startingTimeAsTimeT = time(0);
	    struct tm * startingTime = localtime( & startingTimeAsTimeT );
		oFileStream << "Simulation started at:\t" << (startingTime->tm_year + 1900) << "-" << StringTools::monthIntToString((startingTime->tm_mon + 1)) << "-" <<  startingTime->tm_mday << " " << startingTime->tm_hour << ":" << startingTime->tm_min << std::endl;
	}

	if( this->printModelVersion )
	{
		#if defined SEDFLOWVERSION
		oFileStream << "Model version:\t" << myStringify(SEDFLOWVERSION) << std::endl;
		#endif
	    oFileStream << "Compilation date:\t" << this->compilationYear << "-" << this->compilationMonth << "-" <<  this->compilationDay << std::endl;
	}

	for(std::vector<CombinerVariables::TypesOfCombinersAndInterfaces>::const_iterator currentSetupPropertyForOutput = setupPropertiesForOutput.begin(); currentSetupPropertyForOutput < setupPropertiesForOutput.end(); ++currentSetupPropertyForOutput)
	{
		oFileStream << std::endl;

		switch ( (*currentSetupPropertyForOutput) )
		{
		case CombinerVariables::CalcGradient:
			oFileStream << "Bed slope calculation method:" << std::endl;
			addCalcGradientToFileStream(bedSlopeCalculationMethod, oFileStream,1);
			oFileStream << std::endl << "Water energy slope calculation method:" << std::endl;
			addCalcGradientToFileStream(waterEnergySlopeCalculationMethod, oFileStream,1);
			oFileStream << std::endl << "Sediment energy slope calculation method:" << std::endl;
			addCalcGradientToFileStream(sedimentEnergySlopeCalculationMethod, oFileStream,1);
			break;

		case CombinerVariables::FlowResistance:
			oFileStream << "Flow resistance:" << std::endl;
			addFlowResistanceToFileStream(oFileStream,1);
			break;

		case CombinerVariables::CalcBedloadCapacity:
			oFileStream << "Bedload capacity calculation method:" << std::endl;
			addCalcBedloadCapacityToFileStream(oFileStream,1);
			break;

		case CombinerVariables::CalcTau:
			oFileStream << "Bed shear stress calculation method:" << std::endl;
			addCalcTauToFileStream(oFileStream,1);
			break;

		case CombinerVariables::CalcActiveWidth:
			oFileStream << "Active width calculation method:" << std::endl;
			addCalcActiveWidthToFileStream(oFileStream,1);
			break;

		case CombinerVariables::StrataSorting:
			oFileStream << "Strata sorting method (at first reach):" << std::endl;
			addStrataSortingToFileStream(oFileStream,1);
			break;

		default:
			std::string unknownTypeErrorMessageAsString = "The selected interface ";
			unknownTypeErrorMessageAsString.append(CombinerVariables::typeOfCombinersAndInterfacesToString( (*currentSetupPropertyForOutput) ));
			unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
			const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
			throw (unknownTypeErrorMessage);
			break;
		}
	}

	oFileStream.close();
}

void OutputSimulationSetup::addCalcGradientToFileStream(TypesOfUseForCalcGradient typeOfUseForCalcGradient, std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	ConstructionVariables calcGradient;

	switch ( typeOfUseForCalcGradient )
	{
	case bedSlopeCalculationMethod:
		calcGradient = overallMethods->bedSlopeCalculationMethod->createConstructionVariables();
		break;

	case waterEnergySlopeCalculationMethod:
		calcGradient = overallMethods->waterEnergySlopeCalculationMethod->createConstructionVariables();
		break;

	case sedimentEnergySlopeCalculationMethod:
		calcGradient = overallMethods->sedimentEnergySlopeCalculationMethod->createConstructionVariables();
		break;

	default:
		const char *const unknownTypeErrorMessage = "Unknown type of use for CalcGradient.";
		throw (unknownTypeErrorMessage);
		break;
	}

	addCalcGradientToFileStream(calcGradient,oFileStream,numberOfPrecedingTabs);
}

void OutputSimulationSetup::addCalcGradientToFileStream(const ConstructionVariables& calcGradient, std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	if( calcGradient.interfaceOrCombinerType != CombinerVariables::CalcGradient )
	{
		const char *const notMatchingInterfaceErrorMessage = "Not matching type of interface. (OutputSimulationSetup::addCalcGradientToFileStream)";
		throw (notMatchingInterfaceErrorMessage);
	}

	oFileStream << precedingTabs << "Realisation type:\t" << calcGradient.realisationType << std::endl;

	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;
	std::string tmpString;
	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	double tmpDouble;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	bool tmpBool,ensureMinimumInputSlope;
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator objectMapIterator;
	ConstructionVariables tmpObject;

	CombinerVariables::TypesOfGradientCalculationMethod realisationType = CombinerVariables::stringToTypeOfGradientCalculationMethod(calcGradient.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::ReturnBedslope: //For both Return-methods the same thing is to do.
	case CombinerVariables::ReturnWaterEnergyslope:
		doubleMapIterator = calcGradient.labelledDoubles.find("minimumSlope");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum slope:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::SimpleDownstreamTwoCellGradient: //For both SimpleDownstreamTwoCellGradient the same thing is to do.
	case CombinerVariables::SimpleDownstreamTwoCellGradientWithCenteredValues:
		stringMapIterator = calcGradient.labelledStrings.find("propertyOfInterest");
		tmpString = stringMapIterator->second.at(0);
		oFileStream << precedingTabs << "Property of interest:\t" << tmpString << std::endl;
		break;

	case CombinerVariables::SimpleThreeCellGradient: //For both SimpleThreeCellGradient the same thing is to do.
	case CombinerVariables::SimpleThreeCellGradientWithCenteredValues:
		stringMapIterator = calcGradient.labelledStrings.find("propertyOfInterest");
		tmpString = stringMapIterator->second.at(0);
		oFileStream << precedingTabs << "Property of interest:\t" << tmpString << std::endl;
		stringMapIterator = calcGradient.labelledStrings.find("weightingProperty");
		tmpString = stringMapIterator->second.at(0);
		oFileStream << precedingTabs << "Weighting property:\t" << tmpString << std::endl;
		break;

	case CombinerVariables::ReducedWaterEnergyslope:
		doubleMapIterator = calcGradient.labelledDoubles.find("stressPartitioningExponent");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Stress partitioning exponent:\t" << tmpDouble << std::endl;
		boolMapIterator = calcGradient.labelledBools.find("calculationBasedOnqInsteadOfh");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Calculation based on q instead of h:\t" << boolToString(tmpBool) << std::endl;
		doubleMapIterator = calcGradient.labelledDoubles.find("maximumFroudeNumber");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Maximum Froude number:\t" << tmpDouble << std::endl;
		boolMapIterator = calcGradient.labelledBools.find("ensureMinimumInputSlope");
		ensureMinimumInputSlope = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Ensure minimum input slope:\t" << boolToString(ensureMinimumInputSlope) << std::endl;
		if(ensureMinimumInputSlope)
		{
			doubleMapIterator = calcGradient.labelledDoubles.find("minimumInputSlope");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Minimum input slopet:\t" << tmpDouble << std::endl;
		}
		break;

	case CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable:
		doubleMapIterator = calcGradient.labelledDoubles.find("stressPartitioningExponent");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Stress partitioning exponent:\t" << tmpDouble << std::endl;
		boolMapIterator = calcGradient.labelledBools.find("calculationBasedOnqInsteadOfh");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Calculation based on q instead of h:\t" << boolToString(tmpBool) << std::endl;
		doubleMapIterator = calcGradient.labelledDoubles.find("maximumFroudeNumber");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Maximum Froude number:\t" << tmpDouble << std::endl;
		boolMapIterator = calcGradient.labelledBools.find("ensureMinimumInputSlope");
		ensureMinimumInputSlope = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Ensure minimum input slope:\t" << boolToString(ensureMinimumInputSlope) << std::endl;
		if(ensureMinimumInputSlope)
		{
			doubleMapIterator = calcGradient.labelledDoubles.find("minimumInputSlope");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Minimum input slopet:\t" << tmpDouble << std::endl;
		}
		objectMapIterator = calcGradient.labelledObjects.find("simpleWaterEnergyslopeCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Simple water energy slope calculation method:" << std::endl;
		addCalcGradientToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		break;

	default:
		std::string unknownTypeErrorMessageAsString(calcGradient.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addFlowResistanceToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	addFlowResistanceToFileStream( (overallMethods->flowResistance->createConstructionVariables()) ,oFileStream,numberOfPrecedingTabs);
}

void OutputSimulationSetup::addFlowResistanceToFileStream(const ConstructionVariables& flowResistance, std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	if( flowResistance.interfaceOrCombinerType != CombinerVariables::FlowResistance )
	{
		const char *const notMatchingInterfaceErrorMessage = "Not matching type of interface. (OutputSimulationSetup::addFlowResistanceToFileStream)";
		throw (notMatchingInterfaceErrorMessage);
	}

	oFileStream << precedingTabs << "Realisation type:\t" << flowResistance.realisationType << std::endl;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	double tmpDouble;
	std::map<std::string, std::vector<int> >::const_iterator intMapIterator;
	int tmpInt;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	bool tmpBool;
	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;
	std::string tmpString;
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator objectMapIterator;
	ConstructionVariables tmpObject;

	doubleMapIterator = flowResistance.labelledDoubles.find("startingValueForIteration");
	tmpDouble = doubleMapIterator->second.at(0);
	oFileStream << precedingTabs << "Starting value for iteration:\t" << tmpDouble << std::endl;
	doubleMapIterator = flowResistance.labelledDoubles.find("accuracyForTerminatingIteration");
	tmpDouble = doubleMapIterator->second.at(0);
	oFileStream << precedingTabs << "Accuracy for terminating iteration:\t" << tmpDouble << std::endl;
	intMapIterator = flowResistance.labelledInts.find("maximumNumberOfIterations");
	tmpInt = intMapIterator->second.at(0);
	oFileStream << precedingTabs << "Maximum number of iterations:\t" << tmpInt << std::endl;
	stringMapIterator = flowResistance.labelledStrings.find("typeOfNumericRootFinder");
	tmpString = ""; tmpString.clear(); tmpString = stringMapIterator->second.at(0);
	oFileStream << precedingTabs << "Type of numeric root finder:\t" << tmpString << std::endl;
	boolMapIterator = flowResistance.labelledBools.find("useApproximationsForHydraulicRadius");
	tmpBool = boolMapIterator->second.at(0);
	oFileStream << precedingTabs << "Use approximations for hydraulic radius:\t" << boolToString(tmpBool) << std::endl;
	doubleMapIterator = flowResistance.labelledDoubles.find("maximumFroudeNumber");
	tmpDouble = doubleMapIterator->second.at(0);
	oFileStream << precedingTabs << "Maximum Froude number:\t" << tmpDouble << std::endl;
	doubleMapIterator = flowResistance.labelledDoubles.find("minimumHydraulicSlope");
	tmpDouble = doubleMapIterator->second.at(0);
	oFileStream << precedingTabs << "Minimum hydraulic slope:\t" << tmpDouble << std::endl;

	CombinerVariables::TypesOfFlowResistance realisationType = CombinerVariables::stringToTypeOfFlowResistance(flowResistance.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::FixedPowerLawFlowResistance:
		doubleMapIterator = flowResistance.labelledDoubles.find("factor");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Factor:\t" << tmpDouble << std::endl;
		doubleMapIterator = flowResistance.labelledDoubles.find("grainsFactor");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Grains factor:\t" << tmpDouble << std::endl;
		doubleMapIterator = flowResistance.labelledDoubles.find("grainsPercentile");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Grains percentile:\t" << tmpDouble << std::endl;
		doubleMapIterator = flowResistance.labelledDoubles.find("exponent");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Exponent:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::VariablePowerLawFlowResistance:
		//In this case there is nothing to do.
		break;

	case CombinerVariables::DarcyWeisbachFlowResistance:
		doubleMapIterator = flowResistance.labelledDoubles.find("darcyWeisbachFrictionFactorForBedrock");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Darcy-Weisbach friction factor for bedrock:\t" << tmpDouble << std::endl;
		doubleMapIterator = flowResistance.labelledDoubles.find("darcyWeisbachFrictionFactorForGravelbed");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Darcy-Weisbach friction factor for gravelbed:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead:
		doubleMapIterator = flowResistance.labelledDoubles.find("turbulenceLossFactor");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Turbulence loss factor:\t" << tmpDouble << std::endl;
		objectMapIterator = flowResistance.labelledObjects.find("usedFlowResistanceRelation");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Used flow resistance relation:" << std::endl;
		addFlowResistanceToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		break;

	default:
		std::string unknownTypeErrorMessageAsString(flowResistance.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addCalcBedloadCapacityToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	FlowTypeMethods* sedimentFlowMethods = ((riverSystemMethods->getRegularRiverSystemMethods()).getFlowMethods()).createPointerCopyOfSingleFlowTypeMethods(CombinerVariables::sedimentFlowMethods);
	SedimentFlowTypeMethods* bedloadFlowMethods = dynamic_cast<SedimentFlowMethods*>(sedimentFlowMethods)->createPointerCopyOfSingleSedimentFlowTypeMethods(CombinerVariables::BedloadFlowMethods);
	delete sedimentFlowMethods;
	CalcBedloadCapacity* pointerToCalcBedloadCapacity = dynamic_cast<BedloadFlowMethods*>(bedloadFlowMethods)->createCalcBedloadCapacityPointerCopy();
	delete bedloadFlowMethods;
	ConstructionVariables calcBedloadCapacity = pointerToCalcBedloadCapacity->createConstructionVariables();
	delete pointerToCalcBedloadCapacity;

	oFileStream << precedingTabs << "Realisation type:\t" << calcBedloadCapacity.realisationType << std::endl;

	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;
	std::string tmpString;
	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	double tmpDouble;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	bool tmpBool;
	bool useConstantSandFraction;
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator objectMapIterator;
	ConstructionVariables tmpObject;

	CombinerVariables::TypesOfBedloadCapacityCalculationMethod realisationType = CombinerVariables::stringToTypeOfBedloadCapacityCalculationMethod(calcBedloadCapacity.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::WilcockCroweBedloadCapacity:
		boolMapIterator = calcBedloadCapacity.labelledBools.find("useConstantSandFraction");
		useConstantSandFraction = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Use constant sand fraction:\t" << boolToString(useConstantSandFraction) << std::endl;
		if(useConstantSandFraction)
		{
			doubleMapIterator = calcBedloadCapacity.labelledDoubles.find("constantSandFraction");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Constant sand fraction:\t" << tmpDouble << std::endl;
		}
		break;

	case CombinerVariables::SchneiderEtAlBedloadCapacity:
		doubleMapIterator = calcBedloadCapacity.labelledDoubles.find("minimumDimensionlessReferenceShearStressForMeanDiameter");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum dimensionless reference shear stress for mean diameter:\t" << tmpDouble << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("referenceShearStressBasedOnSlopeANDGrainsizeDistribution");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Reference shear stress based on slope AND grainsize distribution:\t" << boolToString(tmpBool) << std::endl;
		break;

	case CombinerVariables::ReckingBedloadCapacityNonFractional:
		//In this case there is nothing to do.
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnqNonFractional:
		boolMapIterator = calcBedloadCapacity.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Correction for bedload weight at steep slopes:\t" << boolToString(tmpBool) << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("takeArmourLayerIntoAccount");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Take armour layer into account:\t" << boolToString(tmpBool) << std::endl;
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnq:
		boolMapIterator = calcBedloadCapacity.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Correction for bedload weight at steep slopes:\t" << boolToString(tmpBool) << std::endl;
		objectMapIterator = calcBedloadCapacity.labelledObjects.find("hidingFactorsCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Hiding factors calculation method:" << std::endl;
		addCalcHidingFactorsToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnThetaNonFractional:
		boolMapIterator = calcBedloadCapacity.labelledBools.find("thetaCriticalBasedOnConstantSred");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "thetaCritical based on constant Sred:\t" << boolToString(tmpBool) << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("useOnePointOneAsExponentForFroudeNumber");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Use 1.1 as exponent for Froude-number:\t" << boolToString(tmpBool) << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("simplifiedEquation");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Simplified equation:\t" << boolToString(tmpBool) << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("takeArmourLayerIntoAccount");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Take armour layer into account:\t" << boolToString(tmpBool) << std::endl;
		objectMapIterator = calcBedloadCapacity.labelledObjects.find("thresholdCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Threshold calculation method for initiation of bedload motion:" << std::endl;
		addCalcThresholdForInitiationOfBedloadMotionToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnTheta:
		boolMapIterator = calcBedloadCapacity.labelledBools.find("thetaCriticalBasedOnConstantSred");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "thetaCritical based on constant Sred:\t" << boolToString(tmpBool) << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("useOnePointOneAsExponentForFroudeNumber");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Use 1.1 as exponent for Froude-number:\t" << boolToString(tmpBool) << std::endl;
		boolMapIterator = calcBedloadCapacity.labelledBools.find("simplifiedEquation");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Simplified equation:\t" << boolToString(tmpBool) << std::endl;
		objectMapIterator = calcBedloadCapacity.labelledObjects.find("thresholdCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Threshold calculation method for initiation of bedload motion:" << std::endl;
		addCalcThresholdForInitiationOfBedloadMotionToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		objectMapIterator = calcBedloadCapacity.labelledObjects.find("hidingFactorsCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Hiding factors calculation method:" << std::endl;
		addCalcHidingFactorsToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		break;

	case CombinerVariables::ChengBedloadCapacity:
		boolMapIterator = calcBedloadCapacity.labelledBools.find("thetaCriticalBasedOnConstantSred");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "thetaCritical based on constant Sred:\t" << boolToString(tmpBool) << std::endl;
		doubleMapIterator = calcBedloadCapacity.labelledDoubles.find("factor");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Factor:\t" << tmpDouble << std::endl;
		objectMapIterator = calcBedloadCapacity.labelledObjects.find("thresholdCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Threshold calculation method for initiation of bedload motion:" << std::endl;
		addCalcThresholdForInitiationOfBedloadMotionToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		objectMapIterator = calcBedloadCapacity.labelledObjects.find("hidingFactorsCalculationMethod");
		tmpObject = objectMapIterator->second.at(0);
		oFileStream << precedingTabs << "Hiding factors calculation method:" << std::endl;
		addCalcHidingFactorsToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		break;

	default:
		std::string unknownTypeErrorMessageAsString(calcBedloadCapacity.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addCalcTauToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	ConstructionVariables calcTau = ((((riverSystemMethods->getRegularRiverSystemMethods()).getFirstRiverReachMethods()).getRegularRiverReachMethods()).getTauCalculationMethod())->createConstructionVariables();

	oFileStream << precedingTabs << "Realisation type:\t" << calcTau.realisationType << std::endl;

	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	bool tmpBool;

	CombinerVariables::TypesOfTauCalculationMethod realisationType = CombinerVariables::stringToTypeOfTauCalculationMethod(calcTau.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::EnergyslopeTauBasedOnFlowDepth: //For both EnergyslopeTau the same thing is to do.
	case CombinerVariables::EnergyslopeTau:
		boolMapIterator = calcTau.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Correction for bedload weight at steep slopes:\t" << boolToString(tmpBool) << std::endl;
		break;

	case CombinerVariables::FlowVelocityTau:
		//In this case there is nothing to do.
		break;

	default:
		std::string unknownTypeErrorMessageAsString(calcTau.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addCalcActiveWidthToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	ConstructionVariables calcActiveWidth = ((((riverSystemMethods->getRegularRiverSystemMethods()).getFirstRiverReachMethods()).getRegularRiverReachMethods()).getActiveWidthCalculationMethod())->createConstructionVariables();

	oFileStream << precedingTabs << "Realisation type:\t" << calcActiveWidth.realisationType << std::endl;

	CombinerVariables::TypesOfActiveWidthCalculationMethod realisationType = CombinerVariables::stringToTypeOfActiveWidthCalculationMethod(calcActiveWidth.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::SetActiveWidthEqualFlowWidth:
		//In this case there is nothing to do.
		break;

	default:
		std::string unknownTypeErrorMessageAsString(calcActiveWidth.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addStrataSortingToFileStream(std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	ConstructionVariables strataSorting = (riverSystemProperties->regularRiverSystemProperties.cellProperties.front()).strataSorting->createConstructionVariables();

	oFileStream << precedingTabs << "Realisation type:\t" << strataSorting.realisationType << std::endl;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	double tmpDouble;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	bool dynamic;
	bool usePredefinedBreakUpConditions;
	bool tmpBool;
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator objectMapIterator;
	ConstructionVariables tmpObject;

	CombinerVariables::TypesOfStrataSorting realisationType = CombinerVariables::stringToTypeOfStrataSorting(strataSorting.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::SingleLayerNoSorting:
		doubleMapIterator = strataSorting.labelledDoubles.find("layerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Layer thickness:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::StratigraphyWithThresholdBasedUpdate:
		doubleMapIterator = strataSorting.labelledDoubles.find("incrementLayerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Increment layer thickness:\t" << tmpDouble << std::endl;
		boolMapIterator = strataSorting.labelledBools.find("dynamicThresholds");
		dynamic = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Dynamic thresholds:\t" << boolToString(dynamic) << std::endl;
		if(dynamic)
		{
			doubleMapIterator = strataSorting.labelledDoubles.find("referenceGrainSizePercentile");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Reference grain size percentile:\t" << tmpDouble << std::endl;
			doubleMapIterator = strataSorting.labelledDoubles.find("valueOrFactorForLowerThresholdForActiveLayerThickness");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Factor for lower threshold for active layer thickness:\t" << tmpDouble << std::endl;
			doubleMapIterator = strataSorting.labelledDoubles.find("valueOrFactorForUpperThresholdForActiveLayerThickness");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Factor for upper threshold for active layer thickness:\t" << tmpDouble << std::endl;
		}
		else
		{
			doubleMapIterator = strataSorting.labelledDoubles.find("valueOrFactorForLowerThresholdForActiveLayerThickness");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Lower threshold for active layer thickness:\t" << tmpDouble << std::endl;
			doubleMapIterator = strataSorting.labelledDoubles.find("valueOrFactorForUpperThresholdForActiveLayerThickness");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Upper threshold for active layer thickness:\t" << tmpDouble << std::endl;
		}
		doubleMapIterator = strataSorting.labelledDoubles.find("minimumLowerThresholdForActiveLayerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum lower threshold for active layer thickness:\t" << tmpDouble << std::endl;
		doubleMapIterator = strataSorting.labelledDoubles.find("minimumDifferenceBetweenThresholdsForActiveLayerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum difference between thresholds for active layer thickness:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate:
		doubleMapIterator = strataSorting.labelledDoubles.find("layerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Layer thickness:\t" << tmpDouble << std::endl;
		doubleMapIterator = strataSorting.labelledDoubles.find("minimumLayerThicknessPerUnitLayerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum layer thickness per unit layer thickness:\t" << tmpDouble << std::endl;
		doubleMapIterator = strataSorting.labelledDoubles.find("maximumLayerThicknessPerUnitLayerThickness");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Maximum layer thickness per unit layer thickness:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::TwoLayerWithShearStressBasedUpdate:
	case CombinerVariables::TwoLayerWithContinuousUpdate:
		boolMapIterator = strataSorting.labelledBools.find("dynamicLayerThickness");
		dynamic = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Dynamic layer thickness:\t" << boolToString(dynamic) << std::endl;
		if(dynamic)
		{
			doubleMapIterator = strataSorting.labelledDoubles.find("layerThicknessFactor");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Layer thickness factor:\t" << tmpDouble << std::endl;
			doubleMapIterator = strataSorting.labelledDoubles.find("referenceGrainSizePercentile");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Reference grain size percentile:\t" << tmpDouble << std::endl;
		}
		else
		{
			doubleMapIterator = strataSorting.labelledDoubles.find("layerThickness");
			tmpDouble = doubleMapIterator->second.at(0);
			oFileStream << precedingTabs << "Layer thickness:\t" << tmpDouble << std::endl;
		}
		if(realisationType == CombinerVariables::TwoLayerWithShearStressBasedUpdate)
		{
			boolMapIterator = strataSorting.labelledBools.find("usePredefinedBreakUpConditions");
			usePredefinedBreakUpConditions = boolMapIterator->second.at(0);
			oFileStream << precedingTabs << "Use predefined break up conditions:\t" << boolToString(usePredefinedBreakUpConditions) << std::endl;
			if(usePredefinedBreakUpConditions)
			{
				doubleMapIterator = strataSorting.labelledDoubles.find("referenceMedianDiameter");
				tmpDouble = doubleMapIterator->second.at(0);
				oFileStream << precedingTabs << "Reference median diameter:\t" << tmpDouble << std::endl;
				doubleMapIterator = strataSorting.labelledDoubles.find("thetaCriticalForActiveLayer");
				tmpDouble = doubleMapIterator->second.at(0);
				oFileStream << precedingTabs << "Theta critical for active layer:\t" << tmpDouble << std::endl;
				doubleMapIterator = strataSorting.labelledDoubles.find("thetaCriticalForSublayer");
				tmpDouble = doubleMapIterator->second.at(0);
				oFileStream << precedingTabs << "Theta critical for sublayer:\t" << tmpDouble << std::endl;
			}
			else
			{
				boolMapIterator = strataSorting.labelledBools.find("dynamicBreakUpConditions");
				tmpBool = boolMapIterator->second.at(0);
				oFileStream << precedingTabs << "Dynamic break up conditions:\t" << boolToString(tmpBool) << std::endl;
			}
			objectMapIterator = strataSorting.labelledObjects.find("thresholdCalculationMethod");
			tmpObject = objectMapIterator->second.at(0);
			oFileStream << precedingTabs << "Threshold calculation method for initiation of bedload motion:" << std::endl;
			addCalcThresholdForInitiationOfBedloadMotionToFileStream(tmpObject,oFileStream,(numberOfPrecedingTabs+1));
		}
		break;

	default:
		std::string unknownTypeErrorMessageAsString(strataSorting.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addCalcHidingFactorsToFileStream(const ConstructionVariables& calcHidingFactors, std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	if( calcHidingFactors.interfaceOrCombinerType != CombinerVariables::CalcHidingFactors )
	{
		const char *const notMatchingInterfaceErrorMessage = "Not matching type of interface. (OutputSimulationSetup::addCalcHidingFactorsToFileStream)";
		throw (notMatchingInterfaceErrorMessage);
	}

	oFileStream << precedingTabs << "Realisation type:\t" << calcHidingFactors.realisationType << std::endl;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	double tmpDouble;

	CombinerVariables::TypesOfHidingFactorsCalculationMethod realisationType = CombinerVariables::stringToTypeOfHidingFactorsCalculationMethod(calcHidingFactors.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::PowerLawHidingFunction:
		doubleMapIterator = calcHidingFactors.labelledDoubles.find("referenceDiameterPercentile");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Reference diameter percentile:\t" << tmpDouble << std::endl;
		doubleMapIterator = calcHidingFactors.labelledDoubles.find("exponent");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Exponent:\t" << tmpDouble << std::endl;
		break;

	case CombinerVariables::WilcockCroweHidingFunction:
		//In this case there is nothing to do.
		break;

	case CombinerVariables::NoHiding:
		//In this case there is nothing to do.
		break;

	default:
		std::string unknownTypeErrorMessageAsString(calcHidingFactors.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

void OutputSimulationSetup::addCalcThresholdForInitiationOfBedloadMotionToFileStream(const ConstructionVariables& calcThresholdForInitiationOfBedloadMotion, std::ofstream& oFileStream, int numberOfPrecedingTabs) const
{
	std::string precedingTabs (numberOfPrecedingTabs,'\t');
	if( calcThresholdForInitiationOfBedloadMotion.interfaceOrCombinerType != CombinerVariables::CalcThresholdForInitiationOfBedloadMotion )
	{
		const char *const notMatchingInterfaceErrorMessage = "Not matching type of interface. (OutputSimulationSetup::addCalcThresholdForInitiationOfBedloadMotionToFileStream)";
		throw (notMatchingInterfaceErrorMessage);
	}

	oFileStream << precedingTabs << "Realisation type:\t" << calcThresholdForInitiationOfBedloadMotion.realisationType << std::endl;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<int> >::const_iterator intMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	double tmpDouble;
	int tmpInt;
	bool tmpBool;

	CombinerVariables::TypesOfCalculationMethodForBedloadMotionInitiationThreshold realisationType = CombinerVariables::stringToTypeOfCalculationMethodForBedloadMotionInitiationThreshold(calcThresholdForInitiationOfBedloadMotion.realisationType);
	switch ( realisationType )
	{
	case CombinerVariables::ConstantThresholdForInitiationOfBedloadMotion:
		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("constantThreshold");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Constant threshold:\t" << tmpDouble << std::endl;

		boolMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledBools.find("correctionForBedloadWeightAtSteepCounterSlopes");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Correction for bedload weight at steep counter slopes:\t" << boolToString(tmpBool) << std::endl;
		break;

	case CombinerVariables::StochasticThresholdForInitiationOfBedloadMotion:
		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("minimumThresholdValue");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum threshold value:\t" << tmpDouble << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("miu");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Miu:\t" << tmpDouble << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("beta");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Beta:\t" << tmpDouble << std::endl;

		intMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledInts.find("seed");
		tmpInt = intMapIterator->second.at(0);
		oFileStream << precedingTabs << "Seed:\t" << tmpInt << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("weightForCurrent");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Weight for current:\t" << tmpDouble << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("weightForPrevious");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Weight for previous:\t" << tmpDouble << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("weightForPrePrevious");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Weight for prePrevious:\t" << tmpDouble << std::endl;

		boolMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledBools.find("correctionForBedloadWeightAtSteepCounterSlopes");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Correction for bedload weight at steep counter slopes:\t" << boolToString(tmpBool) << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("widthsForSpecialValues");
		oFileStream << precedingTabs << "Widths for special values:";
		for(std::vector<double>::const_iterator currentEntry = doubleMapIterator->second.begin(); currentEntry < doubleMapIterator->second.end(); ++currentEntry)
			{ oFileStream << " " << *currentEntry; }
		oFileStream << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("miuSpecialValues");
		oFileStream << precedingTabs << "Miu special values:";
		for(std::vector<double>::const_iterator currentEntry = doubleMapIterator->second.begin(); currentEntry < doubleMapIterator->second.end(); ++currentEntry)
			{ oFileStream << " " << *currentEntry; }
		oFileStream << std::endl;

		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("betaSpecialValues");
		oFileStream << precedingTabs << "Beta special values:";
		for(std::vector<double>::const_iterator currentEntry = doubleMapIterator->second.begin(); currentEntry < doubleMapIterator->second.end(); ++currentEntry)
			{ oFileStream << " " << *currentEntry; }
		oFileStream << std::endl;
		break;

	case CombinerVariables::LambEtAlCriticalTheta:
		doubleMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledDoubles.find("minimumCriticalDimensionlessShearStress");
		tmpDouble = doubleMapIterator->second.at(0);
		oFileStream << precedingTabs << "Minimum critical dimensionless shear stress:\t" << tmpDouble << std::endl;

		boolMapIterator = calcThresholdForInitiationOfBedloadMotion.labelledBools.find("correctionForBedloadWeightAtSteepCounterSlopes");
		tmpBool = boolMapIterator->second.at(0);
		oFileStream << precedingTabs << "Correction for bedload weight at steep counter slopes:\t" << boolToString(tmpBool) << std::endl;
		break;

	default:
		std::string unknownTypeErrorMessageAsString(calcThresholdForInitiationOfBedloadMotion.realisationType);
		unknownTypeErrorMessageAsString.append(" is not treated in OutputSimulationSetup.");
		const char *const unknownTypeErrorMessage = unknownTypeErrorMessageAsString.c_str();
		throw (unknownTypeErrorMessage);
		break;
	}
}

}
