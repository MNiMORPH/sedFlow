/*
 * StandardInput.cpp
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

#include "StandardInput.h"

#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <limits>
#include <math.h>
#include <set>

#include "StringTools.h"

#include "SedFlowBuilders.h"
#include "StratigraphyWithThresholdBasedUpdate.h"

namespace SedFlow {

ConstructionVariables StandardInput::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::UserInputReader;
	result.realisationType = CombinerVariables::typeOfUserInputReadersToString(CombinerVariables::StandardInput);
	return result;
}


ConstructionVariables StandardInput::convertUserInputXMLIntoConstructionVariables (pugi::xml_document& userInputXMLDocument, const std::string& folderContainingXML) const
{
	pugi::xml_node rootNode = *(userInputXMLDocument.begin());

	CombinerVariables::TypesOfUserInputReaders typeOfUserInputReader = CombinerVariables::stringToTypeOfUserInputReaders(CombinerVariables::getInternalStringCorrespondingToExternalString(rootNode.name()));

	//Check by the root name whether the xml Document can be read by this userInputReader class.
	if ( typeOfUserInputReader != CombinerVariables::StandardInput)
	{
		const char *const methodCompatibilityErrorMessage = "Not matching type of user input reader requested.";
		throw(methodCompatibilityErrorMessage);
	}

	pugi::xml_node pathNode = rootNode.child("path");
	std::string path = folderContainingXML;
	//if ( pathNode != 0 ) { path = StringTools::trimStringCopyKeepingInternalBlanks(pathNode.child_value()); }
	if ( pathNode != 0 ) { path = StringTools::trimStringCopy(pathNode.child_value()); }
	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);

	ConstructionVariables overallParameters = createOverallParameters(rootNode,path);
	ConstructionVariables overallMethods = createOverallMethods(rootNode);
	std::pair<ConstructionVariables,RiverSystemInformation> riverSystemPropertiesAndRiverSystemInformation = createRiverSystemPropertiesAndRiverSystemInformation(rootNode, path, overallParameters, overallMethods);
	ConstructionVariables riverSystemProperties = riverSystemPropertiesAndRiverSystemInformation.first;
	RiverSystemInformation riverSystemInformation = riverSystemPropertiesAndRiverSystemInformation.second;
	ConstructionVariables riverSystemMethods = createRiverSystemMethods(rootNode, path, riverSystemInformation);
	ConstructionVariables outputMethods = createOutputMethods(rootNode, path, riverSystemInformation);

	ConstructionVariables sedFlowConstructionVariables;

	sedFlowConstructionVariables.interfaceOrCombinerType = CombinerVariables::SedFlow;

	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( overallParameters );
	sedFlowConstructionVariables.labelledObjects["overallParameters"] = constructionVariablesVector;

	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( overallMethods );
	sedFlowConstructionVariables.labelledObjects["overallMethods"] = constructionVariablesVector;

	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( riverSystemProperties );
	sedFlowConstructionVariables.labelledObjects["riverSystemProperties"] = constructionVariablesVector;

	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( riverSystemMethods );
	sedFlowConstructionVariables.labelledObjects["riverSystemMethods"] = constructionVariablesVector;

	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( outputMethods );
	sedFlowConstructionVariables.labelledObjects["outputMethods"] = constructionVariablesVector;

	return sedFlowConstructionVariables;
}

ConstructionVariables StandardInput::createOverallParameters(pugi::xml_node rootNode, std::string path)const
{
	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);
	std::string longitudinalProfilePath = path;
	longitudinalProfilePath.append("LongitudinalProfile/");
	std::string grainSizeDistributionsPath = path;
	grainSizeDistributionsPath.append("GrainSizeDistributions/");
	std::string tmpFileName;
	std::string tmpString;

	std::map<std::string,std::vector<std::string> > currentSpreadsheetReadOut;
	std::map<std::string,std::vector<std::string> >::const_iterator iteratorForSpreadsheetReadOut;
	std::istringstream iStringStream;

	double tmpDouble;
	std::vector<double> doubleVectorForLabelledDoubles;

	tmpFileName.clear();
	tmpFileName = longitudinalProfilePath;
	tmpFileName.append("BranchTopology.txt");
	currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(tmpFileName);
	iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("BranchIDs");
	if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
	{
		const char *const branchIDsErrorMessage = "The column BranchIDs is needed in the BranchTopology spreadsheet for StandardInput input reader.";
		throw(branchIDsErrorMessage);
	}
	std::string firstBranchID = *( std::min_element(iteratorForSpreadsheetReadOut->second.begin(),iteratorForSpreadsheetReadOut->second.end()) );
	tmpFileName.clear();
	tmpFileName = longitudinalProfilePath;
	tmpFileName.append("Branch");
	tmpFileName.append(firstBranchID);
	tmpFileName.append("Profile.txt");
	currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(tmpFileName);
	iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("StrataGrainSizeDistribution");
	if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
	{
		const char *const strataGrainSizeDistributionErrorMessage = "The column StrataGrainSizeDistribution is needed for the LongitudinalProfile spreadsheets within StandardInput input reader.";
		throw(strataGrainSizeDistributionErrorMessage);
	}
	std::string firstGrainSizeDistribution = *( iteratorForSpreadsheetReadOut->second.begin() );
	tmpFileName.clear();
	tmpFileName = grainSizeDistributionsPath;
	tmpFileName.append(firstGrainSizeDistribution);
	tmpFileName.append(".txt");
	currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(tmpFileName);
	iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("GrainDiameterInCM");
	if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
	{
		const char *const grainDiameterInCMErrorMessage = "The column GrainDiameterInCM is needed for the grain size distribution spreadsheets within StandardInput input reader.";
		throw(grainDiameterInCMErrorMessage);
	}
	std::vector<double> fractionalGrainDiameters = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
	//Convert fractionalGrainDiameters from cm to m
	std::transform(fractionalGrainDiameters.begin(),fractionalGrainDiameters.end(),fractionalGrainDiameters.begin(),std::bind1st(std::multiplies<double>(),0.01));

	ConstructionVariables result;

	result.interfaceOrCombinerType = CombinerVariables::OverallParameters;

	pugi::xml_node overallParametersNode = rootNode.child("overallParameters");
	if ( overallParametersNode == 0 )
	{
		const char *const overallParametersErrorMessage = "The node overallParameters is needed for the StandardInput input reader.";
		throw(overallParametersErrorMessage);
	}

	pugi::xml_node inputUpperBoundaryInsteadOfMeanGrainDiameterNode = overallParametersNode.child("inputUpperBoundaryInsteadOfMeanGrainDiameter");
	bool inputUpperBoundaryInsteadOfMeanGrainDiameter = true;
	if ( inputUpperBoundaryInsteadOfMeanGrainDiameterNode != 0 )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(inputUpperBoundaryInsteadOfMeanGrainDiameterNode.child_value());

		inputUpperBoundaryInsteadOfMeanGrainDiameter = StringTools::stringToBool(tmpString);
	}

	if(inputUpperBoundaryInsteadOfMeanGrainDiameter)
	{

		pugi::xml_node useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode = overallParametersNode.child("useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters");
		bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters = false;
		if ( useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode != 0 )
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode.child_value());

			useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters = StringTools::stringToBool(tmpString);
		}

		pugi::xml_node lowerDiameterBoundaryForFinestFractionInCMNode = overallParametersNode.child("lowerDiameterBoundaryForFinestFractionInCM");
		double lowerDiameterBoundaryForFinestFractionInCM = 4.0 * std::numeric_limits<double>::epsilon();
		if ( lowerDiameterBoundaryForFinestFractionInCMNode == 0 )
		{
			const char *const lowerDiameterBoundaryForFinestFractionInCMErrorMessage = "The node lowerDiameterBoundaryForFinestFractionInCM is needed for the OverallParameters in StandardInput.";
			throw(lowerDiameterBoundaryForFinestFractionInCMErrorMessage);
		}
		else
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(lowerDiameterBoundaryForFinestFractionInCMNode.child_value()) );
			iStringStream >> lowerDiameterBoundaryForFinestFractionInCM;
			//Convert lowerDiameterBoundaryForFinestFractionInCM from cm to m.
			lowerDiameterBoundaryForFinestFractionInCM *= 0.01;
		}

		if(useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters)
		{
			for(std::vector<double>::reverse_iterator currentValue = fractionalGrainDiameters.rbegin(), nextValue = (currentValue+1); nextValue < fractionalGrainDiameters.rend(); ++currentValue,++nextValue)
				{ *currentValue = (*currentValue + *nextValue) * 0.5; }
			fractionalGrainDiameters.at(0) = (lowerDiameterBoundaryForFinestFractionInCM + fractionalGrainDiameters.at(0)) * 0.5;
		}
		else
		{
			for(std::vector<double>::reverse_iterator currentValue = fractionalGrainDiameters.rbegin(), nextValue = (currentValue+1); nextValue < fractionalGrainDiameters.rend(); ++currentValue,++nextValue)
				{ *currentValue = sqrt((*currentValue * *nextValue)); }
			fractionalGrainDiameters.at(0) = sqrt((lowerDiameterBoundaryForFinestFractionInCM * fractionalGrainDiameters.at(0)));
		}

	}

	addDoubleToConstructionVariables(result,overallParametersNode,"densityWater",1000.0);
	addDoubleToConstructionVariables(result,overallParametersNode,"densitySediment",2650.0);
	addDoubleToConstructionVariables(result,overallParametersNode,"poreVolumeFraction",0.3);
	addDoubleToConstructionVariables(result,overallParametersNode,"gravityAcceleration",9.80665);
	addDoubleToConstructionVariables(result,overallParametersNode,"angleOfReposeInDegree",36.0);
	result.labelledDoubles["fractionalGrainDiameters"] = fractionalGrainDiameters;
	addDoubleToConstructionVariables(result,overallParametersNode,"currentTimeStepLengthInSeconds",0.4);

	pugi::xml_node elapsedSecondsNode = overallParametersNode.child("elapsedSeconds");
	pugi::xml_node finishSecondsNode = overallParametersNode.child("finishSeconds");

	std::vector<double> elapsedSeconds;
	if( ( (elapsedSecondsNode == 0) || (finishSecondsNode == 0) ) )
	{
		tmpFileName.clear();
		tmpFileName = path;
		tmpFileName.append("DischargeAndOtherInputs/Branch");
		tmpFileName.append(firstBranchID);
		tmpFileName.append("Discharge.txt");
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(tmpFileName);
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ElapsedSeconds");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const elapsedSecondsErrorMessage = "The column ElapsedSeconds is needed for the Discharge spreadsheets within StandardInput input reader.";
			throw(elapsedSecondsErrorMessage);
		}
		elapsedSeconds = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
	}

	if(elapsedSecondsNode == 0)
	{
		tmpDouble = *(std::min_element(elapsedSeconds.begin(),elapsedSeconds.end()));
	}
	else
	{
		iStringStream.str("");
		iStringStream.clear();
		iStringStream.str( StringTools::trimStringCopy(elapsedSecondsNode.child_value()) );
		iStringStream >> tmpDouble;
	}
	doubleVectorForLabelledDoubles.clear();
	doubleVectorForLabelledDoubles.push_back( tmpDouble );
	result.labelledDoubles["elapsedSeconds"] = doubleVectorForLabelledDoubles;

	if(finishSecondsNode == 0)
	{
		tmpDouble = *(std::max_element(elapsedSeconds.begin(),elapsedSeconds.end()));
	}
	else
	{
		iStringStream.str("");
		iStringStream.clear();
		iStringStream.str( StringTools::trimStringCopy(finishSecondsNode.child_value()) );
		iStringStream >> tmpDouble;
	}
	doubleVectorForLabelledDoubles.clear();
	doubleVectorForLabelledDoubles.push_back( tmpDouble );
	result.labelledDoubles["finishSeconds"] = doubleVectorForLabelledDoubles;

	addDoubleToConstructionVariables(result,overallParametersNode,"courantFriedrichsLewyNumber",0.9);
	addDoubleToConstructionVariables(result,overallParametersNode,"timeStepThresholdForTerminatingSimulation",0.0000000001);
	addDoubleToConstructionVariables(result,overallParametersNode,"timeStepFactor",1.0);

	addBoolToConstructionVariables(result,overallParametersNode,"updateRegularPropertiesAfterInitialisation",true);
	addBoolToConstructionVariables(result,overallParametersNode,"updateAdditionalRiverReachPropertiesAfterInitialisation",true);
	addBoolToConstructionVariables(result,overallParametersNode,"updateOutputMethodsAfterInitialisation",true);

	return result;
}

ConstructionVariables StandardInput::createOverallMethods(pugi::xml_node rootNode)const
{
	ConstructionVariables result;

	result.interfaceOrCombinerType = CombinerVariables::OverallMethods;

	pugi::xml_node riverSystemMethodsNode = rootNode.child("riverSystemMethods");
	if ( !riverSystemMethodsNode )
	{
		const char *const riverSystemMethodsErrorMessage = "The node riverSystemMethods is needed for the StandardInput input reader.";
		throw(riverSystemMethodsErrorMessage);
	}

	CombinerVariables::TypesOfFlowMethods typeOfWaterFlowMethods = CombinerVariables::UniformDischarge;
	pugi::xml_node waterFlowRoutingNode = riverSystemMethodsNode.child("waterFlowRouting");
	pugi::xml_node waterFlowRealisationTypeNode = waterFlowRoutingNode.child("realisationType");
	if ( waterFlowRealisationTypeNode )
	{
		typeOfWaterFlowMethods = CombinerVariables::stringToTypeOfFlowMethods( StringTools::trimStringCopy(waterFlowRealisationTypeNode.child_value()) );
	}

	addFlowResistanceToConstructionVariables(result,riverSystemMethodsNode,"flowResistance",typeOfWaterFlowMethods);
	const ConstructionVariables& flowResistance = result.labelledObjects.find("flowResistance")->second.at(0);

	addCalcGradientToConstructionVariables(result,riverSystemMethodsNode,"bedSlopeCalculationMethod",false,flowResistance,typeOfWaterFlowMethods,createSimpleDownstreamTwoCellGradient(CombinerVariables::elevation));
	addCalcGradientToConstructionVariables(result,riverSystemMethodsNode,"waterEnergySlopeCalculationMethod",false,flowResistance,typeOfWaterFlowMethods,createReturnBedslope());
	if( !(addCalcGradientToConstructionVariables(result,riverSystemMethodsNode,"sedimentEnergySlopeCalculationMethod",true,flowResistance,typeOfWaterFlowMethods,createReturnBedslope())) )
	{
		const char *const sedimentEnergySlopeCalculationMethodErrorMessage = "The node sedimentEnergySlopeCalculationMethod is needed for the RiverSystemMethods in StandardInput input reader.";
		throw(sedimentEnergySlopeCalculationMethodErrorMessage);
	}

	addCalcBedloadVelocityToConstructionVariables(result,riverSystemMethodsNode,"bedloadVelocityCalculationMethod");

	return result;
}

std::pair<ConstructionVariables, RiverSystemInformation> StandardInput::createRiverSystemPropertiesAndRiverSystemInformation(pugi::xml_node rootNode, std::string path, const ConstructionVariables& overallParameters, const ConstructionVariables& overallMethods)const
{
	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);
	std::string longitudinalProfilePath = path;
	longitudinalProfilePath.append("LongitudinalProfile/");
	std::string dischargePath = path;
	dischargePath.append("DischargeAndOtherInputs/");
	std::string grainSizeDistributionsPath = path;
	grainSizeDistributionsPath.append("GrainSizeDistributions/");
	std::string currentFileName;
	char* currentFileNameAsCharPointer;
	std::string tmpString;

	std::vector<ConstructionVariables> emptyConstructionVariablesVector;
	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	std::vector<double> tmpDoubleVector;

	std::istringstream iStringStream;
	std::ostringstream oStringStream;

	pugi::xml_node overallParametersNode = rootNode.child("overallParameters");
	if ( !overallParametersNode )
	{
		const char *const overallParametersErrorMessage = "The node overallParameters is needed for the StandardInput input reader.";
		throw(overallParametersErrorMessage);
	}

	pugi::xml_node riverSystemMethodsNode = rootNode.child("riverSystemMethods");
	if ( !riverSystemMethodsNode )
	{
		const char *const riverSystemMethodsErrorMessage = "The node riverSystemMethods is needed for the StandardInput input reader.";
		throw(riverSystemMethodsErrorMessage);
	}

	bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate = false;
	pugi::xml_node upstreamOfSillsWedgeShapedInsteadOfParallelUpdateNode = riverSystemMethodsNode.child("upstreamOfSillsWedgeShapedInsteadOfParallelUpdate");
	if ( upstreamOfSillsWedgeShapedInsteadOfParallelUpdateNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(upstreamOfSillsWedgeShapedInsteadOfParallelUpdateNode.child_value());

		upstreamOfSillsWedgeShapedInsteadOfParallelUpdate = StringTools::stringToBool(tmpString);
	}

	pugi::xml_node inputUpperBoundaryInsteadOfMeanGrainDiameterNode = overallParametersNode.child("inputUpperBoundaryInsteadOfMeanGrainDiameter");
	bool inputUpperBoundaryInsteadOfMeanGrainDiameter = true;
	if ( inputUpperBoundaryInsteadOfMeanGrainDiameterNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(inputUpperBoundaryInsteadOfMeanGrainDiameterNode.child_value());

		inputUpperBoundaryInsteadOfMeanGrainDiameter = StringTools::stringToBool(tmpString);
	}

	bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters = false;
	double lowerDiameterBoundaryForFinestFractionInCM = 4.0 * std::numeric_limits<double>::epsilon();

	if(inputUpperBoundaryInsteadOfMeanGrainDiameter)
	{

		pugi::xml_node useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode = overallParametersNode.child("useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters");
		if ( useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode )
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode.child_value());

			useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters = StringTools::stringToBool(tmpString);
		}

		pugi::xml_node lowerDiameterBoundaryForFinestFractionInCMNode = overallParametersNode.child("lowerDiameterBoundaryForFinestFractionInCM");
		if ( !lowerDiameterBoundaryForFinestFractionInCMNode )
		{
			const char *const lowerDiameterBoundaryForFinestFractionInCMErrorMessage = "The node lowerDiameterBoundaryForFinestFractionInCM is needed for the OverallParameters in StandardInput.";
			throw(lowerDiameterBoundaryForFinestFractionInCMErrorMessage);
		}
		else
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(lowerDiameterBoundaryForFinestFractionInCMNode.child_value()) );
			iStringStream >> lowerDiameterBoundaryForFinestFractionInCM;
			//Convert lowerDiameterBoundaryForFinestFractionInCM from cm to m.
			lowerDiameterBoundaryForFinestFractionInCM *= 0.01;
		}

	}

	pugi::xml_node kilometrageOfSimulationOutletNode = overallParametersNode.child("kilometrageOfSimulationOutlet");
	double kilometrageOfSimulationOutlet = 0.0;
	if ( kilometrageOfSimulationOutletNode )
	{
		iStringStream.str("");
		iStringStream.clear();
		iStringStream.str( StringTools::trimStringCopy(kilometrageOfSimulationOutletNode.child_value()) );
		iStringStream >> kilometrageOfSimulationOutlet;
	}

	pugi::xml_node thicknessInputsIncludingPoreVolumeNode = overallParametersNode.child("thicknessInputsIncludingPoreVolume");
	bool thicknessInputsIncludingPoreVolume = true;
	if ( thicknessInputsIncludingPoreVolumeNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(thicknessInputsIncludingPoreVolumeNode.child_value());

		thicknessInputsIncludingPoreVolume = StringTools::stringToBool(tmpString);
	}

	pugi::xml_node poreVolumeFractionNode = overallParametersNode.child("poreVolumeFraction");
	double poreVolumeFraction = 0.3;
	if ( poreVolumeFractionNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(poreVolumeFractionNode.child_value());

		iStringStream.clear();
		iStringStream.str( tmpString );
		iStringStream >> poreVolumeFraction;
	}

	std::pair<ConstructionVariables,std::pair<bool,int> > strataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers = createStrataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers(riverSystemMethodsNode,thicknessInputsIncludingPoreVolume,poreVolumeFraction);
	ConstructionVariables strataSorting = strataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers.first;
	std::pair<bool,int> boolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers = strataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers.second;
	bool useInitialGrainSizesForConstantLayerThickness = boolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers.first;
	int numberOfLayers = boolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers.second;

	std::map<std::string,std::vector<std::string> > currentSpreadsheetReadOut;
	std::map<std::string,std::vector<std::string> >::const_iterator iteratorForSpreadsheetReadOut;
	std::vector<double>::iterator iteratorForDoubleVectors;

	currentFileName.clear();
	currentFileName = longitudinalProfilePath;
	currentFileName.append("BranchTopology.txt");
	currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
	iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("BranchIDs");
	if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
	{
		const char *const branchIDsErrorMessage = "The column BranchIDs is needed in the BranchTopology spreadsheet for StandardInput input reader.";
		throw(branchIDsErrorMessage);
	}
	std::vector<int> branchIDs = StringTools::stringVectorToIntVector( iteratorForSpreadsheetReadOut->second );
	iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("DownstreamBranchIDs");
	if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
	{
		const char *const downstreamBranchIDsErrorMessage = "The column DownstreamBranchIDs is needed in the BranchTopology spreadsheet for StandardInput input reader.";
		throw(downstreamBranchIDsErrorMessage);
	}
	std::vector<int> downstreamBranchIDs = StringTools::stringVectorToIntVector( iteratorForSpreadsheetReadOut->second );

	std::vector<RiverBranchForStandardInput> riverBranches;
	for(std::vector<int>::const_iterator currentBranchID = branchIDs.begin(), currentDownstreamBranchID = downstreamBranchIDs.begin(); currentBranchID < branchIDs.end(); ++currentBranchID, ++currentDownstreamBranchID)
		{ riverBranches.push_back( RiverBranchForStandardInput(*currentBranchID,*currentDownstreamBranchID) ); }
	std::sort(riverBranches.begin(),riverBranches.end());
	riverBranches.push_back( RiverBranchForStandardInput( *(max_element(downstreamBranchIDs.begin(),downstreamBranchIDs.end())) , std::numeric_limits<int>::max() ) );
	(riverBranches.back()).downstreamMargin = true;
	std::map<int,int> mapFromRiverBranchIDToUpmostCellID;
	//TODO Adjust this value to a reasonable 1, whenever the Problem in the RegularRiverSystemProperties is solved.
	int currentTopmostCellID = 1001;
	std::map<int,double> mapFromBranchIdToInitialDischarge;
	std::map<int,double>::iterator iteratorForMapFromBranchIdToInitialDischarge;
	double currentBranchInitialDischarge;
	for(std::vector<RiverBranchForStandardInput>::iterator currentRiverBranch = riverBranches.begin(); currentRiverBranch < riverBranches.end(); ++currentRiverBranch)
	{
		if( !(checkIfIntVectorContainsValue(downstreamBranchIDs,currentRiverBranch->branchID)) ) { currentRiverBranch->upstreamMargin = true; }
		currentRiverBranch->topmostCellID = currentTopmostCellID;
		mapFromRiverBranchIDToUpmostCellID[ currentRiverBranch->branchID ] = currentRiverBranch->topmostCellID;
		//Adjust numberOfCells
		currentFileName.clear();
		currentFileName = longitudinalProfilePath;
		currentFileName.append("Branch");
		oStringStream.str("");
		oStringStream.clear();
		oStringStream << currentRiverBranch->branchID << std::flush;
		currentFileName.append(oStringStream.str());
		currentFileName.append("Profile.txt");
		currentSpreadsheetReadOut.clear();
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
		currentRiverBranch->numberOfCells = (currentSpreadsheetReadOut.begin())->second.size();
		currentTopmostCellID += currentRiverBranch->numberOfCells;
		//Adjust topmostKilometrage
		currentFileName.clear();
		currentFileName = longitudinalProfilePath;
		currentFileName.append("Branch");
		oStringStream.str("");
		oStringStream.clear();
		oStringStream << currentRiverBranch->branchID << std::flush;
		currentFileName.append(oStringStream.str());
		currentFileName.append("Profile.txt");
		currentSpreadsheetReadOut.clear();
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("KilometrageUpstreamDirected");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const kilometrageUpstreamDirectedErrorMessage = "The column KilometrageUpstreamDirected is needed for the LongitudinalProfile spreadsheets within StandardInput input reader.";
			throw(kilometrageUpstreamDirectedErrorMessage);
		}
		currentRiverBranch->topmostKilometrage = *( (StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second )).begin() );
		//Update mapFromBranchIdToInitialDischarge
		if( currentRiverBranch->upstreamMargin )
		{
			currentFileName.clear();
			currentFileName = dischargePath;
			currentFileName.append("Branch");
			oStringStream.str("");
			oStringStream.clear();
			oStringStream << currentRiverBranch->branchID << std::flush;
			currentFileName.append(oStringStream.str());
			currentFileName.append("Discharge.txt");
			currentSpreadsheetReadOut.clear();
			currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("DischargeInM3PerS");
			if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
			{
				const char *const dischargeErrorMessage = "The column DischargeInM3PerS is needed within discharge spreadsheets for StandardInput input reader.";
				throw(dischargeErrorMessage);
			}
			mapFromBranchIdToInitialDischarge[currentRiverBranch->branchID] = *((StringTools::stringVectorToDoubleVector(iteratorForSpreadsheetReadOut->second)).begin());
		}
		if( !(currentRiverBranch->downstreamMargin) )
		{
			iteratorForMapFromBranchIdToInitialDischarge = mapFromBranchIdToInitialDischarge.find( currentRiverBranch->branchID );
			currentBranchInitialDischarge = iteratorForMapFromBranchIdToInitialDischarge->second;
			iteratorForMapFromBranchIdToInitialDischarge = mapFromBranchIdToInitialDischarge.find( currentRiverBranch->downstreamBranchID );
			if( iteratorForMapFromBranchIdToInitialDischarge == mapFromBranchIdToInitialDischarge.end() )
			{
				mapFromBranchIdToInitialDischarge[currentRiverBranch->downstreamBranchID] = 0.0;
				iteratorForMapFromBranchIdToInitialDischarge = mapFromBranchIdToInitialDischarge.find( currentRiverBranch->downstreamBranchID );
			}
			iteratorForMapFromBranchIdToInitialDischarge->second += currentBranchInitialDischarge;
		}
	}

	//Adjust initialDischarge
	for(std::vector<RiverBranchForStandardInput>::iterator currentRiverBranch = riverBranches.begin(); currentRiverBranch < riverBranches.end(); ++currentRiverBranch)
	{
		iteratorForMapFromBranchIdToInitialDischarge = mapFromBranchIdToInitialDischarge.find( currentRiverBranch->branchID );
		currentRiverBranch->initialDischarge = iteratorForMapFromBranchIdToInitialDischarge->second;
	}

	//Get sills
	std::map< std::pair<int,double>, std::pair<double,double> > mapFromSillLocationsToTopEdgeElevationsANDPoleniFactors;
	bool dropHeightInsteadOfTopEdgeElevation = false;
	currentFileName.clear();
	currentFileName = longitudinalProfilePath;
	currentFileName.append("Sills.txt");
	if( StringTools::fileExists(currentFileName) )
	{
		currentSpreadsheetReadOut.clear();
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("BranchIDs");
		if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
		{
			const char *const branchIDsErrorMessage = "The column BranchIDs is needed in the Sills spreadsheet for StandardInput input reader.";
			throw(branchIDsErrorMessage);
		}
		std::vector<int> sillBranchIDs = StringTools::stringVectorToIntVector( iteratorForSpreadsheetReadOut->second );

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("KilometrageUpstreamDirected");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const kilometrageUpstreamDirectedErrorMessage = "The column KilometrageUpstreamDirected is needed for the Sills spreadsheets within StandardInput input reader.";
			throw(kilometrageUpstreamDirectedErrorMessage);
		}
		std::vector<double> sillKilometrages = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("SillTopEdgeElevationInM");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("SillDropHeightInM");
			if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
			{
				const char *const sillTopEdgeElevationOrDropHeightInMErrorMessage = "The column SillTopEdgeElevationInM or SillDropHeightInM is needed for the Sills spreadsheets within StandardInput input reader.";
				throw(sillTopEdgeElevationOrDropHeightInMErrorMessage);
			}
			dropHeightInsteadOfTopEdgeElevation = true;
		}
		std::vector<double> sillTopEdges = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

		std::vector<double> poleniFactors ( sillTopEdges.size(), (1.0/sqrt(3.0)) );
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("PoleniFactor");
		if(iteratorForSpreadsheetReadOut != currentSpreadsheetReadOut.end())
		{
			poleniFactors = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
		}

		std::pair<int,double> currentKey;
		std::pair<double,double> currentValue;
		std::vector<int>::const_iterator currentSillBranchId = sillBranchIDs.begin();
		for(std::vector<double>::const_iterator currentSillKilometrage = sillKilometrages.begin(), currentSillTopEdge = sillTopEdges.begin(), currentPoleniFactor = poleniFactors.begin(); currentSillKilometrage < sillKilometrages.end(); ++currentSillBranchId, ++currentSillKilometrage, ++currentSillTopEdge, ++currentPoleniFactor)
		{
			currentKey = std::make_pair(*currentSillBranchId,*currentSillKilometrage);
			currentValue = std::make_pair(*currentSillTopEdge,*currentPoleniFactor);
			mapFromSillLocationsToTopEdgeElevationsANDPoleniFactors[currentKey] = currentValue;
		}
	}
	//End of get sills

	ConstructionVariables rawRiverSystemProperties;
	rawRiverSystemProperties.interfaceOrCombinerType = CombinerVariables::RiverSystemProperties;

	ConstructionVariables additionalRiverSystemProperties;
	additionalRiverSystemProperties.interfaceOrCombinerType = CombinerVariables::AdditionalRiverSystemProperties;
	additionalRiverSystemProperties.labelledObjects["constitutingAdditionalRiverSystemPropertyTypes"] = emptyConstructionVariablesVector;

	ConstructionVariables regularRiverSystemProperties;
	regularRiverSystemProperties.interfaceOrCombinerType = CombinerVariables::RegularRiverSystemProperties;

	std::vector<ConstructionVariables> cellProperties;

	std::vector<double> fractionalGrainDiametersOfFirstReach;
	std::vector<CombinerVariables::TypesOfGrains> grainTypesOfFirstReach;
	std::string currentGrainTypeAsString;
	bool firstReach = true;
	//TODO Adjust this value to a reasonable 1, whenever the Problem in the RegularRiverSystemProperties is solved.
	int currentCellID = 1001;
	int currentDownstreamCellID;
	int currentBranchID;
	double currentLength = 4.0;
	bool currentSillOccurence;
	double currentSillTopEdgeElevation;
	double currentPoleniFactor;
	std::pair<int,double> tmpSillMapKey;
	std::map< std::pair<int,double>, std::pair<double,double> >::const_iterator sillMapIterator;
	std::vector<std::string> currentKilometersAsString;
	std::vector<std::string>::const_iterator currentStringKilometersEntry;
	std::vector<double> currentKilometers;
	std::vector<double> currentElevations;
	std::vector<double> currentChannelWidths;
	std::vector<double> currentAlluviumThicknesses;
	std::vector<double> currentBedrockRoughnessEquivalentRepresentativeGrainDiameters;
	std::vector<std::string> currentStrataGrainSizeDistributions;
	std::vector<std::string>::const_iterator currentStrataGrainSizeDistributionsEntry;
	std::vector<std::string> currentSurfaceLayerGrainSizeDistributions;
	std::vector<std::string>::const_iterator currentSurfaceLayerGrainSizeDistributionsEntry;
	std::map<std::string,ConstructionVariables> mapFromGrainSizeDistributionNameToConstructionVariables;
	std::map<std::string,ConstructionVariables>::const_iterator grainSizeDistributionMapIterator;
	bool bedrockRoughnessGiven;
	std::map<std::string,double> mapFromGrainSizeDistributionNameToD84;
	std::map<std::string,double>::const_iterator D84MapIterator;
	std::map<std::pair<int,std::string>,int> mapFromBranchIDAndKilometrageToUserCellID;
	std::pair<int,std::string> currentKeyForMapFromBranchIDAndKilometrageToUserCellID;
	currentFileName.clear();
	currentFileName = path;
	currentFileName.append("Output/ReachIDExplanations.txt");
	std::ofstream outputFileForCellIDsBranchIDsAndKilometrage;
	currentFileNameAsCharPointer = new char [currentFileName.size()+1];
	std::strcpy(currentFileNameAsCharPointer, currentFileName.c_str());
	outputFileForCellIDsBranchIDsAndKilometrage.open(currentFileNameAsCharPointer, std::ios::out | std::ios::trunc);
	delete[] currentFileNameAsCharPointer;
	outputFileForCellIDsBranchIDsAndKilometrage << "ReachID\tBranchID\tKilometrage" << std::endl;

	for(std::vector<RiverBranchForStandardInput>::const_iterator currentRiverBranch = riverBranches.begin(); currentRiverBranch < riverBranches.end(); ++currentRiverBranch)
	{
		currentBranchID = currentRiverBranch->branchID;
		currentFileName.clear();
		currentFileName = longitudinalProfilePath;
		currentFileName.append("Branch");
		oStringStream.str("");
		oStringStream.clear();
		oStringStream << currentBranchID << std::flush;
		currentFileName.append(oStringStream.str());
		currentFileName.append("Profile.txt");
		currentSpreadsheetReadOut.clear();
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("KilometrageUpstreamDirected");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const kilometrageUpstreamDirectedErrorMessage = "The column KilometrageUpstreamDirected is needed for the LongitudinalProfile spreadsheets within StandardInput input reader.";
			throw(kilometrageUpstreamDirectedErrorMessage);
		}
		currentKilometersAsString = iteratorForSpreadsheetReadOut->second;
		currentKilometers = StringTools::stringVectorToDoubleVector(currentKilometersAsString);
		for(std::vector<double>::const_iterator currentKilometersEntry = currentKilometers.begin(), nextKilometersEntry = (currentKilometersEntry + 1); nextKilometersEntry < currentKilometers.end(); ++currentKilometersEntry,++nextKilometersEntry)
		{
			if ( *currentKilometersEntry <= *nextKilometersEntry )
			{
				const char *const longitudinalProfileErrorMessage = "River reaches in LongitudinalProfile spreadsheets need to be sorted from upstream to downstream, i.e. kilometrage going from large to small values.";
				throw(longitudinalProfileErrorMessage);
			}
		}
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ElevationInM");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const elevationErrorMessage = "The column ElevationInM is needed for the LongitudinalProfile spreadsheets within StandardInput input reader.";
			throw(elevationErrorMessage);
		}
		currentElevations = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ChannelWidthInM");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const channelWidthErrorMessage = "The column ChannelWidthInM is needed for the LongitudinalProfile spreadsheets within StandardInput input reader.";
			throw(channelWidthErrorMessage);
		}
		currentChannelWidths = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("AlluviumThicknessInM");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			currentAlluviumThicknesses = std::vector<double>(currentElevations.size(),4000.0);
		}
		else
		{
			currentAlluviumThicknesses = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
		}
		if(thicknessInputsIncludingPoreVolume)
			{ std::transform(currentAlluviumThicknesses.begin(),currentAlluviumThicknesses.end(),currentAlluviumThicknesses.begin(),std::bind1st(std::multiplies<double>(),(1-poreVolumeFraction))); }
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("BedrockRoughnessEquivalentRepresentativeGrainDiameterInCM");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			bedrockRoughnessGiven = false;
			currentBedrockRoughnessEquivalentRepresentativeGrainDiameters = std::vector<double>(currentElevations.size(),0.04);
		}
		else
		{
			bedrockRoughnessGiven = true;
			currentBedrockRoughnessEquivalentRepresentativeGrainDiameters = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
			//Convert cm into m.
			std::transform(currentBedrockRoughnessEquivalentRepresentativeGrainDiameters.begin(),currentBedrockRoughnessEquivalentRepresentativeGrainDiameters.end(),currentBedrockRoughnessEquivalentRepresentativeGrainDiameters.begin(),std::bind1st(std::multiplies<double>(),0.01));
		}
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("StrataGrainSizeDistribution");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const strataGrainSizeDistributionErrorMessage = "The column StrataGrainSizeDistribution is needed for the LongitudinalProfile spreadsheets within StandardInput input reader.";
			throw(strataGrainSizeDistributionErrorMessage);
		}
		currentStrataGrainSizeDistributions = iteratorForSpreadsheetReadOut->second;
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("SurfaceLayerGrainSizeDistribution");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			currentSurfaceLayerGrainSizeDistributions = currentStrataGrainSizeDistributions;
		}
		else
		{
			currentSurfaceLayerGrainSizeDistributions = iteratorForSpreadsheetReadOut->second;
		}

		currentStringKilometersEntry = currentKilometersAsString.begin();
		currentStrataGrainSizeDistributionsEntry = currentStrataGrainSizeDistributions.begin();
		currentSurfaceLayerGrainSizeDistributionsEntry = currentSurfaceLayerGrainSizeDistributions.begin();
		for(std::vector<double>::const_iterator currentKilometersEntry = currentKilometers.begin(), currentElevationEntry = currentElevations.begin(), currentChannelWidthEntry = currentChannelWidths.begin(), currentAlluviumThicknessEntry = currentAlluviumThicknesses.begin(), currentBedrockRoughnessEquivalentRepresentativeGrainDiameterEntry = currentBedrockRoughnessEquivalentRepresentativeGrainDiameters.begin(); currentKilometersEntry < currentKilometers.end(); ++currentKilometersEntry, ++currentStrataGrainSizeDistributionsEntry, ++currentSurfaceLayerGrainSizeDistributionsEntry, ++currentStringKilometersEntry, ++currentElevationEntry, ++currentChannelWidthEntry, ++currentAlluviumThicknessEntry, ++currentBedrockRoughnessEquivalentRepresentativeGrainDiameterEntry)
		{
			currentKeyForMapFromBranchIDAndKilometrageToUserCellID = std::make_pair(currentBranchID,*currentStringKilometersEntry);
			mapFromBranchIDAndKilometrageToUserCellID[currentKeyForMapFromBranchIDAndKilometrageToUserCellID] = currentCellID;

			//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
			outputFileForCellIDsBranchIDsAndKilometrage << (currentCellID-1000) << "\t" << currentBranchID << "\t" << *currentStringKilometersEntry << std::endl;

			if( currentElevationEntry != (currentElevations.end()-1) || currentRiverBranch == (riverBranches.end()-1) ) { currentDownstreamCellID = currentCellID + 1; }
			else { currentDownstreamCellID = RiverBranchForStandardInput::getTopmostCellIDofCertainBranch(riverBranches,currentRiverBranch->downstreamBranchID); }

			if( currentKilometersEntry != (currentKilometers.end()-1) ) { currentLength = *currentKilometersEntry - *(currentKilometersEntry+1); }
			else
			{
				if( currentRiverBranch != (riverBranches.end()-1) ) { currentLength = *currentKilometersEntry - RiverBranchForStandardInput::getTopmostKilometrageofCertainBranch(riverBranches,currentRiverBranch->downstreamBranchID); }
				else { currentLength = *currentKilometersEntry - kilometrageOfSimulationOutlet; }
			}
			//Translate currentLength from km to m
			currentLength *= 1000.0;

			tmpSillMapKey = std::make_pair(currentBranchID,*currentKilometersEntry);
			sillMapIterator = mapFromSillLocationsToTopEdgeElevationsANDPoleniFactors.find(tmpSillMapKey);
			if( sillMapIterator == mapFromSillLocationsToTopEdgeElevationsANDPoleniFactors.end() )
			{
				currentSillOccurence = false;
				currentSillTopEdgeElevation = -9999.0;
				currentPoleniFactor = -9999.0;
			}
			else
			{
				currentSillOccurence = true;
				currentSillTopEdgeElevation = (sillMapIterator->second).first;
				if(dropHeightInsteadOfTopEdgeElevation) { currentSillTopEdgeElevation += *currentElevationEntry; }
				currentPoleniFactor = (sillMapIterator->second).second;
			}

			grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToConstructionVariables.find(*currentStrataGrainSizeDistributionsEntry);
			if( grainSizeDistributionMapIterator == mapFromGrainSizeDistributionNameToConstructionVariables.end() )
			{
				currentFileName.clear();
				currentFileName = grainSizeDistributionsPath;
				currentFileName.append(*currentStrataGrainSizeDistributionsEntry);
				currentFileName.append(".txt");
				if(firstReach)
				{
					currentSpreadsheetReadOut.clear();
					currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
					for(iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.begin(); iteratorForSpreadsheetReadOut != currentSpreadsheetReadOut.end(); ++iteratorForSpreadsheetReadOut)
					{
						currentGrainTypeAsString.clear();
						currentGrainTypeAsString = iteratorForSpreadsheetReadOut->first;
						if( currentGrainTypeAsString != "GrainDiameterInCM" )
						{
							grainTypesOfFirstReach.push_back( CombinerVariables::stringToTypeOfGrains( currentGrainTypeAsString ) );
						}
					}
				}
				std::pair<std::vector<double>,ConstructionVariables> strataGrainsFractionalGrainDiametersAndGrainsConstructionVariables = createFractionalGrainDiametersAndGrainsConstructionVariablesFromSpreadsheet(currentFileName,inputUpperBoundaryInsteadOfMeanGrainDiameter,useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters,lowerDiameterBoundaryForFinestFractionInCM,grainTypesOfFirstReach);
				std::vector<double> strataGrainsFractionalGrainDiameters = strataGrainsFractionalGrainDiametersAndGrainsConstructionVariables.first;
				if(firstReach)
				{
					fractionalGrainDiametersOfFirstReach = strataGrainsFractionalGrainDiameters;
					for(std::vector<double>::const_iterator currentGrainSize = fractionalGrainDiametersOfFirstReach.begin(), nextGrainSize = (currentGrainSize + 1); nextGrainSize < fractionalGrainDiametersOfFirstReach.end(); ++currentGrainSize,++nextGrainSize)
					{
						if ( *currentGrainSize >= *nextGrainSize )
						{
							const char *const sortedGrainSizesErrorMessage = "Grain sizes in corresponding spreadsheets need to be sorted from small to large.";
							throw(sortedGrainSizesErrorMessage);
						}
					}
					firstReach = false;
				}
				else
				{
					if( fractionalGrainDiametersOfFirstReach.size() != strataGrainsFractionalGrainDiameters.size() )
					{
						const char *const grainSizesNumberErrorMessage = "The number of treated grain sizes needs to be the same in all corresponding spreadsheets.";
						throw(grainSizesNumberErrorMessage);
					}
					for(std::vector<double>::const_iterator currentFirstReachGrainSize = fractionalGrainDiametersOfFirstReach.begin(), currentStrataGrainSize = strataGrainsFractionalGrainDiameters.begin(); currentFirstReachGrainSize < fractionalGrainDiametersOfFirstReach.end(); ++currentFirstReachGrainSize, ++currentStrataGrainSize)
					{
						if ( *currentFirstReachGrainSize != *currentStrataGrainSize )
						{
							const char *const sameGrainSizesFractionsErrorMessage = "The treated grain sizes need to be exactly the same in all corresponding spreadsheets.";
							throw(sameGrainSizesFractionsErrorMessage);
						}
					}
				}
				mapFromGrainSizeDistributionNameToConstructionVariables[*currentStrataGrainSizeDistributionsEntry] = strataGrainsFractionalGrainDiametersAndGrainsConstructionVariables.second;
				grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToConstructionVariables.find(*currentStrataGrainSizeDistributionsEntry);
			}
			ConstructionVariables strataGrains = grainSizeDistributionMapIterator->second;

			grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToConstructionVariables.find(*currentSurfaceLayerGrainSizeDistributionsEntry);
			if( grainSizeDistributionMapIterator == mapFromGrainSizeDistributionNameToConstructionVariables.end() )
			{
				currentFileName.clear();
				currentFileName = grainSizeDistributionsPath;
				currentFileName.append(*currentSurfaceLayerGrainSizeDistributionsEntry);
				currentFileName.append(".txt");
				std::pair<std::vector<double>,ConstructionVariables> armourGrainsFractionalGrainDiametersAndGrainsConstructionVariables = createFractionalGrainDiametersAndGrainsConstructionVariablesFromSpreadsheet(currentFileName,inputUpperBoundaryInsteadOfMeanGrainDiameter,useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters,lowerDiameterBoundaryForFinestFractionInCM,grainTypesOfFirstReach);
				std::vector<double> armourGrainsFractionalGrainDiameters = armourGrainsFractionalGrainDiametersAndGrainsConstructionVariables.first;
				if( fractionalGrainDiametersOfFirstReach.size() != armourGrainsFractionalGrainDiameters.size() )
				{
					const char *const grainSizesNumberErrorMessage = "The number of treated grain sizes needs to be the same in all corresponding spreadsheets.";
					throw(grainSizesNumberErrorMessage);
				}
				for(std::vector<double>::const_iterator currentFirstReachGrainSize = fractionalGrainDiametersOfFirstReach.begin(), currentArmourGrainSize = armourGrainsFractionalGrainDiameters.begin(); currentFirstReachGrainSize < fractionalGrainDiametersOfFirstReach.end(); ++currentFirstReachGrainSize, ++currentArmourGrainSize)
				{
					if ( *currentFirstReachGrainSize != *currentArmourGrainSize )
					{
						const char *const sameGrainSizesFractionsErrorMessage = "The treated grain sizes need to be exactly the same in all corresponding spreadsheets.";
						throw(sameGrainSizesFractionsErrorMessage);
					}
				}
				mapFromGrainSizeDistributionNameToConstructionVariables[*currentSurfaceLayerGrainSizeDistributionsEntry] = armourGrainsFractionalGrainDiametersAndGrainsConstructionVariables.second;
				grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToConstructionVariables.find(*currentSurfaceLayerGrainSizeDistributionsEntry);
			}
			ConstructionVariables armourGrains = grainSizeDistributionMapIterator->second;

			double currentBedrockRoughnessEquivalentRepresentativeGrainDiameter;
			if(bedrockRoughnessGiven)
			{
				currentBedrockRoughnessEquivalentRepresentativeGrainDiameter = *currentBedrockRoughnessEquivalentRepresentativeGrainDiameterEntry;
			}
			else
			{
				D84MapIterator = mapFromGrainSizeDistributionNameToD84.find(*currentSurfaceLayerGrainSizeDistributionsEntry);
				if( D84MapIterator == mapFromGrainSizeDistributionNameToD84.end() )
				{
					HighestOrderStructuresPointers tmpHighestOrderStructuresPointers;
					Grains* tmpArmourGrainsPointer = static_cast<Grains*>( SedFlowBuilders::generalBuilder(armourGrains,tmpHighestOrderStructuresPointers) );
					Grains armourLayerGrains = Grains(*tmpArmourGrainsPointer);
					delete tmpArmourGrainsPointer;
					mapFromGrainSizeDistributionNameToD84[*currentSurfaceLayerGrainSizeDistributionsEntry] = armourLayerGrains.getPercentileGrainDiameter(fractionalGrainDiametersOfFirstReach,84.0);
					D84MapIterator = mapFromGrainSizeDistributionNameToD84.find(*currentSurfaceLayerGrainSizeDistributionsEntry);
				}
				currentBedrockRoughnessEquivalentRepresentativeGrainDiameter = D84MapIterator->second;
			}

			ConstructionVariables currentSillProperties;
			currentSillProperties.interfaceOrCombinerType = CombinerVariables::SillProperties;
			currentSillProperties.realisationType = CombinerVariables::typeOfSillPropertiesToString( CombinerVariables::PoleniSill );
			tmpDoubleVector.clear();
			tmpDoubleVector.push_back(*currentChannelWidthEntry);
			currentSillProperties.labelledDoubles["overfallWidth"] = tmpDoubleVector;
			tmpDoubleVector.clear();
			tmpDoubleVector.push_back(currentPoleniFactor);
			currentSillProperties.labelledDoubles["poleniFactor"] = tmpDoubleVector;

			ConstructionVariables currentRiverReachProperties = createInfinitelyDeepRectangularRiverReachProperties(currentCellID,currentDownstreamCellID,*currentElevationEntry,*currentChannelWidthEntry,currentLength,currentBedrockRoughnessEquivalentRepresentativeGrainDiameter,currentSillOccurence,currentSillTopEdgeElevation,currentRiverBranch->initialDischarge,*currentAlluviumThicknessEntry,upstreamOfSillsWedgeShapedInsteadOfParallelUpdate,armourGrains,strataGrains,strataSorting,currentSillProperties,useInitialGrainSizesForConstantLayerThickness,fractionalGrainDiametersOfFirstReach,numberOfLayers);
			cellProperties.push_back( currentRiverReachProperties );
			++currentCellID;
		}
	}

	outputFileForCellIDsBranchIDsAndKilometrage.close();

	regularRiverSystemProperties.labelledObjects["cellProperties"] = cellProperties;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(additionalRiverSystemProperties);
	rawRiverSystemProperties.labelledObjects["additionalRiverSystemProperties"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(regularRiverSystemProperties);
	rawRiverSystemProperties.labelledObjects["regularRiverSystemProperties"] = tmpConstructionVariablesVector;

	HighestOrderStructuresPointers tmpHighestOrderStructurePointers;
	OverallParameters* tmpOverallParameters = SedFlowBuilders::overallParametersBuilder(overallParameters,tmpHighestOrderStructurePointers);
	tmpHighestOrderStructurePointers.overallParameters = tmpOverallParameters;
	OverallMethods* tmpOverallMethods = SedFlowBuilders::overallMethodsBuilder(overallMethods,tmpHighestOrderStructurePointers);
	tmpHighestOrderStructurePointers.overallMethods = tmpOverallMethods;
	RiverSystemProperties* tmpRiverSystemProperties = SedFlowBuilders::riverSystemPropertiesBuilder(rawRiverSystemProperties,tmpHighestOrderStructurePointers);
	for(std::vector<RiverReachProperties>::reverse_iterator currentRiverReachPropertiesBackward = tmpRiverSystemProperties->regularRiverSystemProperties.cellProperties.rbegin(); currentRiverReachPropertiesBackward < tmpRiverSystemProperties->regularRiverSystemProperties.cellProperties.rend(); ++currentRiverReachPropertiesBackward)
	{
		currentRiverReachPropertiesBackward->regularRiverReachProperties.bedslope = tmpOverallMethods->bedSlopeCalculationMethod->calculate( *currentRiverReachPropertiesBackward );
		if( tmpOverallMethods->waterEnergySlopeCalculationMethod->dependsOnBedslope() ) { currentRiverReachPropertiesBackward->regularRiverReachProperties.waterEnergyslope = tmpOverallMethods->waterEnergySlopeCalculationMethod->calculate( *currentRiverReachPropertiesBackward ); }
		tmpOverallMethods->flowResistance->calculateAndUpdateFlowDepthAndFlowVelocityUsingDischargeAsInput( *currentRiverReachPropertiesBackward );
		currentRiverReachPropertiesBackward->regularRiverReachProperties.waterEnergyslope = tmpOverallMethods->waterEnergySlopeCalculationMethod->calculate( *currentRiverReachPropertiesBackward );
		if( tmpOverallMethods->sedimentEnergySlopeCalculationMethod->dependsOnWaterEnergyslope() )
		{
			std::pair<double,double> reducedAndUnreducedSlope = tmpOverallMethods->sedimentEnergySlopeCalculationMethod->calculateReducedAndUnreducedSedimentEnergyslope( *currentRiverReachPropertiesBackward );
			currentRiverReachPropertiesBackward->regularRiverReachProperties.sedimentEnergyslope = reducedAndUnreducedSlope.first;
			currentRiverReachPropertiesBackward->regularRiverReachProperties.unreducedSedimentEnergyslope = reducedAndUnreducedSlope.second;
		}
	}
	ConstructionVariables riverSystemProperties = tmpRiverSystemProperties->createConstructionVariables();

	delete tmpOverallParameters;
	delete tmpOverallMethods;
	delete tmpRiverSystemProperties;

	RiverSystemInformation riverSystemInformation;

	riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID = mapFromBranchIDAndKilometrageToUserCellID;
	riverSystemInformation.riverBranches = riverBranches;
	riverSystemInformation.usedTypesOfGrains = grainTypesOfFirstReach;
	riverSystemInformation.usedFractionalGrainDiameters = fractionalGrainDiametersOfFirstReach;

	std::pair<ConstructionVariables, RiverSystemInformation> result = std::make_pair(riverSystemProperties,riverSystemInformation);

	return result;
}

ConstructionVariables StandardInput::createRiverSystemMethods(pugi::xml_node rootNode, std::string path, const RiverSystemInformation& riverSystemInformation)const
{
	ConstructionVariables result;

	std::vector<ConstructionVariables> emptyConstructionVariablesVector;
	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	std::vector<bool> tmpBoolVector;
	std::vector<double> tmpDoubleVector;
	std::string tmpString;
	std::vector<std::string> tmpStringVector;
	std::istringstream iStringStream;

	result.interfaceOrCombinerType = CombinerVariables::RiverSystemMethods;

	pugi::xml_node riverSystemMethodsNode = rootNode.child("riverSystemMethods");
	if ( !riverSystemMethodsNode )
	{
		const char *const riverSystemMethodsErrorMessage = "The node riverSystemMethods is needed for the StandardInput input reader.";
		throw(riverSystemMethodsErrorMessage);
	}

	pugi::xml_node overallParametersNode = rootNode.child("overallParameters");
	if ( !overallParametersNode )
	{
		const char *const overallParametersErrorMessage = "The node overallParameters is needed for the StandardInput input reader.";
		throw(overallParametersErrorMessage);
	}

	pugi::xml_node inputUpperBoundaryInsteadOfMeanGrainDiameterNode = overallParametersNode.child("inputUpperBoundaryInsteadOfMeanGrainDiameter");
	bool inputUpperBoundaryInsteadOfMeanGrainDiameter = true;
	if ( inputUpperBoundaryInsteadOfMeanGrainDiameterNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(inputUpperBoundaryInsteadOfMeanGrainDiameterNode.child_value());

		inputUpperBoundaryInsteadOfMeanGrainDiameter = StringTools::stringToBool(tmpString);
	}

	bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters = false;
	double lowerDiameterBoundaryForFinestFractionInCM = 4.0 * std::numeric_limits<double>::epsilon();

	if(inputUpperBoundaryInsteadOfMeanGrainDiameter)
	{

		pugi::xml_node useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode = overallParametersNode.child("useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters");
		if ( useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode )
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiametersNode.child_value());

			useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters = StringTools::stringToBool(tmpString);
		}

		pugi::xml_node lowerDiameterBoundaryForFinestFractionInCMNode = overallParametersNode.child("lowerDiameterBoundaryForFinestFractionInCM");
		if ( !lowerDiameterBoundaryForFinestFractionInCMNode )
		{
			const char *const lowerDiameterBoundaryForFinestFractionInCMErrorMessage = "The node lowerDiameterBoundaryForFinestFractionInCM is needed for the OverallParameters in StandardInput.";
			throw(lowerDiameterBoundaryForFinestFractionInCMErrorMessage);
		}
		else
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(lowerDiameterBoundaryForFinestFractionInCMNode.child_value()) );
			iStringStream >> lowerDiameterBoundaryForFinestFractionInCM;
			//Convert lowerDiameterBoundaryForFinestFractionInCM from cm to m.
			lowerDiameterBoundaryForFinestFractionInCM *= 0.01;
		}

	}

	pugi::xml_node poreVolumeFractionNode = overallParametersNode.child("poreVolumeFraction");
	double poreVolumeFraction = 0.3;
	if ( poreVolumeFractionNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(poreVolumeFractionNode.child_value());

		iStringStream.clear();
		iStringStream.str( tmpString );
		iStringStream >> poreVolumeFraction;
	}

	////////////////////////////////////// Create additionalRiverSystemMethods ///////////////////////////////////////////////////
	ConstructionVariables additionalRiverSystemMethods;
	additionalRiverSystemMethods.interfaceOrCombinerType = CombinerVariables::AdditionalRiverSystemMethods;
	additionalRiverSystemMethods.labelledObjects["constitutingAdditionalRiverSystemMethodTypes"] = emptyConstructionVariablesVector;

	////////////////////////////////////// Create regularRiverSystemMethods ///////////////////////////////////////////////////
	ConstructionVariables regularRiverSystemMethods;
	regularRiverSystemMethods.interfaceOrCombinerType = CombinerVariables::RegularRiverSystemMethods;

			////////////////////////////////////// Create changeRateModifiers ///////////////////////////////////////////////////
			ConstructionVariables changeRateModifiers;
			changeRateModifiers.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiers;
			std::string dischargeAndOtherInputsPath = path;
			dischargeAndOtherInputsPath.append("DischargeAndOtherInputs/");

			ConstructionVariables changeRateModifiersForWaterFlowMethodsInGeneral;
			changeRateModifiersForWaterFlowMethodsInGeneral.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersForSingleFlowMethod;

			std::vector<ConstructionVariables> changeRateModifiersForWaterFlowMethods;

			pugi::xml_node adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMarginsNode = riverSystemMethodsNode.child("adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins");
			if ( adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMarginsNode )
			{
				ConstructionVariables adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins;
				adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
				adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins);

				if( !(addDoubleToConstructionVariables(adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins,adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMarginsNode,"minimumBedSlope",0.4)) )
				{
					const char *const minimumBedSlopeErrorMessage = "The node minimumBedSlope is needed for the AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins within StandardInput input reader.";
					throw(minimumBedSlopeErrorMessage);
				}
				if( !(addDoubleToConstructionVariables(adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins,adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMarginsNode,"maximumBedSlope",400)) )
				{
					const char *const maximumBedSlopeErrorMessage = "The node maximumBedSlope is needed for the AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins within StandardInput input reader.";
					throw(maximumBedSlopeErrorMessage);
				}
				if( !(addDoubleToConstructionVariables(adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins,adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMarginsNode,"minimumWaterSurfaceSlope",0.4)) )
				{
					const char *const minimumWaterSurfaceSlopeErrorMessage = "The node minimumWaterSurfaceSlope is needed for the AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins within StandardInput input reader.";
					throw(minimumWaterSurfaceSlopeErrorMessage);
				}
				if( !(addDoubleToConstructionVariables(adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins,adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMarginsNode,"maximumWaterSurfaceSlope",400)) )
				{
					const char *const maximumWaterSurfaceSlopeErrorMessage = "The node maximumWaterSurfaceSlope is needed for the AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins within StandardInput input reader.";
					throw(maximumWaterSurfaceSlopeErrorMessage);
				}

				tmpBoolVector.clear();
				tmpBoolVector.push_back(false);
				adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.labelledBools["ensureModificationAgainstOtherChangeRateModifiers"] = tmpBoolVector;

				tmpStringVector.clear();
				tmpStringVector.push_back( CombinerVariables::typeOfGeneralFlowMethodsToString(CombinerVariables::WaterFlowMethodsInGeneral) );
				adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.labelledStrings["correspondingGeneralFlowMethod"] = tmpStringVector;

				changeRateModifiersForWaterFlowMethods.push_back( adjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins );
			}


			ConstructionVariables inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods;
			inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
			inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated);

			std::vector<ConstructionVariables> dischargeTimeSeries = createDischargeTimeSeriesFromSpreadsheets(dischargeAndOtherInputsPath,riverSystemInformation);
			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector = dischargeTimeSeries;
			inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods.labelledObjects["timeSeriesInputs"] = tmpConstructionVariablesVector;

			tmpBoolVector.clear();
			tmpBoolVector.push_back(true);
			inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods.labelledBools["ensureModificationAgainstOtherChangeRateModifiers"] = tmpBoolVector;

			tmpStringVector.clear();
			tmpStringVector.push_back( CombinerVariables::typeOfGeneralFlowMethodsToString(CombinerVariables::WaterFlowMethodsInGeneral) );
			inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods.labelledStrings["correspondingGeneralFlowMethod"] = tmpStringVector;

			changeRateModifiersForWaterFlowMethods.push_back( inputPropertyTimeSeriesLinearlyInterpolatedForWaterFlowMethods );
			changeRateModifiersForWaterFlowMethodsInGeneral.labelledObjects["constitutingChangeRateModifiersTypes"] = changeRateModifiersForWaterFlowMethods;

			ConstructionVariables changeRateModifiersForSedimentFlowMethodsInGeneral;
			changeRateModifiersForSedimentFlowMethodsInGeneral.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersForSingleFlowMethod;

			ConstructionVariables inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods;
			inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods.interfaceOrCombinerType = CombinerVariables::ChangeRateModifiersType;
			inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods.realisationType = CombinerVariables::typeOfChangeRateModifiersToString(CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated);

			std::vector<ConstructionVariables> sedimentInputTimeSeries = createSedimentInputTimeSeriesFromSpreadsheets(path,riverSystemInformation,inputUpperBoundaryInsteadOfMeanGrainDiameter,useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters,lowerDiameterBoundaryForFinestFractionInCM,poreVolumeFraction);
			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector = sedimentInputTimeSeries;
			inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods.labelledObjects["timeSeriesInputs"] = tmpConstructionVariablesVector;

			tmpBoolVector.clear();
			tmpBoolVector.push_back(true);
			inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods.labelledBools["ensureModificationAgainstOtherChangeRateModifiers"] = tmpBoolVector;

			tmpStringVector.clear();
			tmpStringVector.push_back( CombinerVariables::typeOfGeneralFlowMethodsToString(CombinerVariables::SedimentFlowMethodsInGeneral) );
			inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods.labelledStrings["correspondingGeneralFlowMethod"] = tmpStringVector;

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back( inputPropertyTimeSeriesLinearlyInterpolatedForSedimentFlowMethods );
			changeRateModifiersForSedimentFlowMethodsInGeneral.labelledObjects["constitutingChangeRateModifiersTypes"] = tmpConstructionVariablesVector;

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back( changeRateModifiersForWaterFlowMethodsInGeneral );
			tmpConstructionVariablesVector.push_back( changeRateModifiersForSedimentFlowMethodsInGeneral );
			changeRateModifiers.labelledObjects["constitutingChangeRateModifiersForSingleFlowMethods"] = tmpConstructionVariablesVector;

			////////////////////////////////////// Create flowMethods ///////////////////////////////////////////////////
			ConstructionVariables flowMethods;
			flowMethods.interfaceOrCombinerType = CombinerVariables::FlowMethods;

			ConstructionVariables waterFlowMethods;
			pugi::xml_node waterFlowRoutingNode = riverSystemMethodsNode.child("waterFlowRouting");
			if ( waterFlowRoutingNode )
			{
				waterFlowMethods = createWaterFlowMethods(waterFlowRoutingNode);
			}
			else
			{
				waterFlowMethods = ConstructionVariables();
				waterFlowMethods.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;
				waterFlowMethods.realisationType = CombinerVariables::typeOfFlowMethodsToString(CombinerVariables::UniformDischarge);
				tmpDoubleVector.clear();
				tmpDoubleVector.push_back(900);
				waterFlowMethods.labelledDoubles["maximumTimeStep"] = tmpDoubleVector;
			}

			CombinerVariables::TypesOfFlowMethods typeOfWaterFlowMethods = CombinerVariables::stringToTypeOfFlowMethods(waterFlowMethods.realisationType);

			pugi::xml_node bedLoadFlowMethodsNode = riverSystemMethodsNode.child("bedloadTransportEquations");
			if ( !bedLoadFlowMethodsNode )
			{
				const char *const bedLoadFlowMethodsErrorMessage = "The node bedloadTransportEquations is needed for the riverSystemMethods within StandardInput input reader.";
				throw(bedLoadFlowMethodsErrorMessage);
			}

			ConstructionVariables sedimentFlowMethods;
			sedimentFlowMethods.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;
			sedimentFlowMethods.realisationType = CombinerVariables::typeOfFlowMethodsToString(CombinerVariables::sedimentFlowMethods);
			addBoolToConstructionVariables(sedimentFlowMethods,bedLoadFlowMethodsNode,"updateErosionRatesInMarginCells",true);

			ConstructionVariables bedLoadFlowMethods;
			bedLoadFlowMethods.interfaceOrCombinerType = CombinerVariables::SedimentFlowTypeMethods;
			bedLoadFlowMethods.realisationType = CombinerVariables::typeOfSedimentFlowMethodsToString(CombinerVariables::BedloadFlowMethods);

			addDoubleToConstructionVariables(bedLoadFlowMethods,bedLoadFlowMethodsNode,"maximumFractionOfActiveLayerToBeEroded",0.9);
			addBoolToConstructionVariables(bedLoadFlowMethods,bedLoadFlowMethodsNode,"preventZeroOrNegativeBedSlopes",( (typeOfWaterFlowMethods == CombinerVariables::ImplicitKinematicWave) || (typeOfWaterFlowMethods == CombinerVariables::ExplicitKinematicWave) ));
			addDoubleToConstructionVariables(bedLoadFlowMethods,bedLoadFlowMethodsNode,"maximumRelativeTwoCellBedSlopeChange",0.9);

			std::pair<ConstructionVariables,bool> bedLoadCapacityCalculationMethodWithBoolNonFractional = createBedLoadCapacityCalculationMethodWithBoolNonFractional(bedLoadFlowMethodsNode);
			ConstructionVariables bedLoadCapacityCalculationMethod = bedLoadCapacityCalculationMethodWithBoolNonFractional.first;
			bool nonFractional = bedLoadCapacityCalculationMethodWithBoolNonFractional.second;

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back(bedLoadCapacityCalculationMethod);
			bedLoadFlowMethods.labelledObjects["bedLoadCapacityCalculationMethod"] = tmpConstructionVariablesVector;

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back(bedLoadFlowMethods);
			sedimentFlowMethods.labelledObjects["constitutingSedimentFlowMethodsTypes"] = tmpConstructionVariablesVector;


			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back(waterFlowMethods);
			tmpConstructionVariablesVector.push_back(sedimentFlowMethods);
			flowMethods.labelledObjects["constitutingFlowMethodsTypes"] = tmpConstructionVariablesVector;

			////////////////////////////////////// Create riverReachMethods ///////////////////////////////////////////////////

			std::map<int,ConstructionVariables> mapFromCellIDToInstantaneousSedimentInputs = createMapFromCellIDToInstantaneousSedimentInputsFromSpreadsheet(path,riverSystemInformation,inputUpperBoundaryInsteadOfMeanGrainDiameter,useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters,lowerDiameterBoundaryForFinestFractionInCM,poreVolumeFraction);
			tmpConstructionVariablesVector.clear();
			//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
			for(std::map<std::pair<int,std::string>,int>::const_iterator currentCellID = riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID.begin(); currentCellID != riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID.end(); ++currentCellID)
					{ tmpConstructionVariablesVector.push_back( createRiverReachMethodsWithCertainCellID(riverSystemMethodsNode,(currentCellID->second - 1000),nonFractional,mapFromCellIDToInstantaneousSedimentInputs) ); }
			regularRiverSystemMethods.labelledObjects["riverReachMethods"] = tmpConstructionVariablesVector;

			//Compose regularRiverSystemMethods

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back(changeRateModifiers);
			regularRiverSystemMethods.labelledObjects["changeRateModifiers"] = tmpConstructionVariablesVector;

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back(flowMethods);
			regularRiverSystemMethods.labelledObjects["flowMethods"] = tmpConstructionVariablesVector;



	//Compose result

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(additionalRiverSystemMethods);
	result.labelledObjects["additionalRiverSystemMethods"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(regularRiverSystemMethods);
	result.labelledObjects["regularRiverSystemMethods"] = tmpConstructionVariablesVector;

	return result;
}

ConstructionVariables StandardInput::createOutputMethods(pugi::xml_node rootNode, std::string path, const RiverSystemInformation& riverSystemInformation)const
{
	ConstructionVariables result;

	result.interfaceOrCombinerType = CombinerVariables::OutputMethods;
	std::vector<ConstructionVariables> constitutingOutputMethodTypes;

	path.append("Output/");

	std::vector<std::string> stringVectorForLabelledStrings;
	std::vector<int> intVectorForLabelledInts;
	std::vector<double> doubleVectorForLabelledDoubles;
	std::vector<bool> boolVectorForLabelledBools;
	std::istringstream iStringStream;
	std::string	tmpString;
	char* tmpCharPointer;
	int tmpInt;
	double tmpDouble;

	bool emptyABTNameAlreadyUsed = false;
	bool createStandardOutputs = true;
	StandardOutputCharacteristics standardOutputCharacteristics;
	standardOutputCharacteristics.forVisualInterpretation = false;
	standardOutputCharacteristics.outputInterval = 3600.0;
	standardOutputCharacteristics.precisionForOutput = 4;
	standardOutputCharacteristics.writeLineEachTimeStep = false;
	standardOutputCharacteristics.outputTimeStepLength = false;
	standardOutputCharacteristics.outputInitialValues = true;
	standardOutputCharacteristics.printUpstreamMargins = false;
	standardOutputCharacteristics.printDownstreamMargin = false;
	standardOutputCharacteristics.useSecondaryOutputInterval = false;
	std::vector<std::string> standardOutputProperties;
	standardOutputProperties.reserve(6);
	standardOutputProperties.push_back(CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::elevation));
	standardOutputProperties.push_back(CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::activeLayerPerUnitBedSurfaceD50));
	standardOutputProperties.push_back(CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::activeLayerPerUnitBedSurfaceD84));
	standardOutputProperties.push_back(CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::bedslope));
	standardOutputProperties.push_back(CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::maximumWaterdepth));
	standardOutputProperties.push_back("accumulatedBedloadTransport");

	pugi::xml_node outputMethodsNode = rootNode.child("outputMethods");
	if ( outputMethodsNode )
	{

		////////////////////// Update standardOutputCharacteristics //////////////////////

		pugi::xml_node currentStandardNode = outputMethodsNode.child("createStandardOutputs");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			createStandardOutputs = StringTools::stringToBool(tmpString);
		}

		currentStandardNode = outputMethodsNode.child("forVisualInterpretationStandard");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.forVisualInterpretation = StringTools::stringToBool(tmpString);
		}

		currentStandardNode = outputMethodsNode.child("explicitTimesForOutputStandard");
		if(currentStandardNode)
		{
			currentStandardNode = currentStandardNode.first_child();
			while(currentStandardNode)
			{
				iStringStream.str("");
				iStringStream.clear();
				tmpString.clear();
				tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
				iStringStream.str( tmpString );
				iStringStream >> tmpDouble;
				standardOutputCharacteristics.explicitTimesForOutput.push_back( tmpDouble );
				currentStandardNode = currentStandardNode.next_sibling();
			}
		}

		currentStandardNode = outputMethodsNode.child("outputIntervalStandard");
		if(currentStandardNode)
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentStandardNode.child_value()) );
			iStringStream >> standardOutputCharacteristics.outputInterval;
		}

		currentStandardNode = outputMethodsNode.child("precisionForOutputStandard");
		if(currentStandardNode)
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentStandardNode.child_value()) );
			iStringStream >> standardOutputCharacteristics.precisionForOutput;
		}

		currentStandardNode = outputMethodsNode.child("reachIDsForOutputStandard");
		if(currentStandardNode)
		{
			std::string reachID ("reachID");
			std::string branchID ("branchID");
			bool isReachID, isBranchID;

			currentStandardNode = currentStandardNode.first_child();
			while(currentStandardNode)
			{
				isReachID = (currentStandardNode.name() == reachID );
				isBranchID = (currentStandardNode.name() == branchID );
				if( isReachID || isBranchID )
				{
					iStringStream.str("");
					iStringStream.clear();
					tmpString.clear();
					tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
					iStringStream.str( tmpString );
					iStringStream >> tmpInt;
					if(isReachID)
					{
						standardOutputCharacteristics.reachIDsForOutput.push_back( tmpInt );
					}
					else //isBranchID
					{
						for(std::vector<RiverBranchForStandardInput>::const_iterator currentBranch = riverSystemInformation.riverBranches.begin(); currentBranch < riverSystemInformation.riverBranches.end(); ++currentBranch)
						{
							if(currentBranch->branchID == tmpInt)
							{
								for(int currentReachID = currentBranch->topmostCellID; currentReachID < (currentBranch->topmostCellID + currentBranch->numberOfCells); ++currentReachID)
								{
									//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
									standardOutputCharacteristics.reachIDsForOutput.push_back( (currentReachID - 1000) );
								}
							}
						}
					}
				}
				currentStandardNode = currentStandardNode.next_sibling();
			}
		}

		currentStandardNode = outputMethodsNode.child("writeLineEachTimeStepStandard");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.writeLineEachTimeStep = StringTools::stringToBool(tmpString);
		}

		currentStandardNode = outputMethodsNode.child("outputTimeStepLengthStandard");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.outputTimeStepLength = StringTools::stringToBool(tmpString);
		}

		currentStandardNode = outputMethodsNode.child("outputInitialValuesStandard");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.outputInitialValues = StringTools::stringToBool(tmpString);
		}

		currentStandardNode = outputMethodsNode.child("printUpstreamMarginsStandard");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.printUpstreamMargins = StringTools::stringToBool(tmpString);
		}

		currentStandardNode = outputMethodsNode.child("printDownstreamMargin");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.printDownstreamMargin = StringTools::stringToBool(tmpString);
		}

		pugi::xml_node currentStandardReferenceCellIDNode = outputMethodsNode.child("referenceCellIDStandard");
		pugi::xml_node currentStandardReferencePropertyNode = outputMethodsNode.child("referencePropertyStandard");
		pugi::xml_node currentStandardThresholdToBeExceededNode = outputMethodsNode.child("thresholdToBeExceededStandard");
		pugi::xml_node currentStandardSecondaryOutputIntervalNode = outputMethodsNode.child("secondaryOutputIntervalStandard");

		standardOutputCharacteristics.useSecondaryOutputInterval = ( (currentStandardReferenceCellIDNode!=0) || (currentStandardReferencePropertyNode!=0) || (currentStandardThresholdToBeExceededNode!=0) ||(currentStandardSecondaryOutputIntervalNode!=0) );

		currentStandardNode = outputMethodsNode.child("useSecondaryOutputIntervalStandard");
		if(currentStandardNode)
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentStandardNode.child_value());
			standardOutputCharacteristics.useSecondaryOutputInterval = StringTools::stringToBool(tmpString);
		}

		if( standardOutputCharacteristics.useSecondaryOutputInterval)
		{
			if( (currentStandardReferenceCellIDNode==0) || (currentStandardReferencePropertyNode==0) || (currentStandardThresholdToBeExceededNode==0) ||(currentStandardSecondaryOutputIntervalNode==0) )
			{
				const char *const useSecondaryOutputIntervalErrorMessage = "For useSecondaryOutputInterval either all or none of the following nodes must be given: referenceCellIDStandard, referencePropertyStandard, thresholdToBeExceededStandard, secondaryOutputIntervalStandard.";
				throw(useSecondaryOutputIntervalErrorMessage);
			}

			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentStandardReferenceCellIDNode.child_value()) );
			iStringStream >> standardOutputCharacteristics.referenceCellUserCellID;

			standardOutputCharacteristics.referenceProperty = StringTools::trimStringCopy(currentStandardReferencePropertyNode.child_value());

			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentStandardThresholdToBeExceededNode.child_value()) );
			iStringStream >> standardOutputCharacteristics.thresholdToBeExceeded;

			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentStandardSecondaryOutputIntervalNode.child_value()) );
			iStringStream >> standardOutputCharacteristics.secondaryOutputInterval;

		}

		currentStandardNode = outputMethodsNode.child("regularRiverReachPropertiesForOutputStandard");
		if(currentStandardNode != 0)
		{
			bool hasNotBeenClearedYet = true;
			currentStandardNode = currentStandardNode.first_child();
			while(currentStandardNode != 0)
			{
				if(hasNotBeenClearedYet) { standardOutputProperties.clear(); hasNotBeenClearedYet = false; }
				standardOutputProperties.push_back( currentStandardNode.name() );
				currentStandardNode = currentStandardNode.next_sibling();
			}
		}

		////////////////////// Create regular output methods //////////////////////

		tmpString = "regularOutput";
		std::ostringstream oStringStream;
		int currentOutputID = 1;
		oStringStream << currentOutputID << std::flush;
		tmpString.append( oStringStream.str() );

		pugi::xml_node currentRegularOutputNode = outputMethodsNode.child(tmpString.c_str());

		while( currentRegularOutputNode )
		{
			tmpString.append(".txt");
			constitutingOutputMethodTypes.push_back( createSingleRegularOutputMethod(currentRegularOutputNode,path,tmpString,riverSystemInformation,standardOutputCharacteristics,false,"default") );

			tmpString = "regularOutput";
			oStringStream.str("");
			oStringStream.clear();
			oStringStream << ++currentOutputID << std::flush;
			tmpString.append( oStringStream.str() );

			currentRegularOutputNode = outputMethodsNode.child(tmpString.c_str());
		}

		tmpString = "regularOutput";
		currentRegularOutputNode = outputMethodsNode.child(tmpString.c_str());

		if( currentRegularOutputNode != 0 )
		{
			tmpString.append(".txt");
			constitutingOutputMethodTypes.push_back( createSingleRegularOutputMethod(currentRegularOutputNode,path,tmpString,riverSystemInformation,standardOutputCharacteristics,false,"default") );
		}

		tmpString.clear();
		oStringStream.str("");
		oStringStream.clear();


		////////////////////// Create OutputAccumulatedBedloadTransport //////////////////////

		tmpString = "outputAccumulatedBedloadTransport";
		currentOutputID = 1;
		oStringStream << currentOutputID << std::flush;
		tmpString.append( oStringStream.str() );

		pugi::xml_node currentOutputAccumulatedBedloadTransportNode = outputMethodsNode.child(tmpString.c_str());

		while( currentOutputAccumulatedBedloadTransportNode != 0 )
		{
			constitutingOutputMethodTypes.push_back( createSingleOutputAccumulatedBedloadTransport(currentOutputAccumulatedBedloadTransportNode,path,tmpString,riverSystemInformation,standardOutputCharacteristics) );

			tmpString = "outputAccumulatedBedloadTransport";
			oStringStream.str("");
			oStringStream.clear();
			oStringStream << ++currentOutputID << std::flush;
			tmpString.append( oStringStream.str() );

			currentOutputAccumulatedBedloadTransportNode = outputMethodsNode.child(tmpString.c_str());
		}

		tmpString = "outputAccumulatedBedloadTransport";
		currentOutputAccumulatedBedloadTransportNode = outputMethodsNode.child(tmpString.c_str());

		if( currentOutputAccumulatedBedloadTransportNode != 0 )
		{
			emptyABTNameAlreadyUsed = true;
			constitutingOutputMethodTypes.push_back( createSingleOutputAccumulatedBedloadTransport(currentOutputAccumulatedBedloadTransportNode,path,"",riverSystemInformation,standardOutputCharacteristics) );
		}

		tmpString.clear();
		oStringStream.str("");
		oStringStream.clear();


		////////////////////// Create OutputVerbatimTranslationOfConstructionVariablesToXML //////////////////////

		pugi::xml_node XMLOutputNode = outputMethodsNode.child("backupXML");
		if ( XMLOutputNode )
		{
		ConstructionVariables outputVerbatimTranslationOfConstructionVariablesToXML;

		outputVerbatimTranslationOfConstructionVariablesToXML.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
		outputVerbatimTranslationOfConstructionVariablesToXML.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML);

		stringVectorForLabelledStrings.push_back(path);
		outputVerbatimTranslationOfConstructionVariablesToXML.labelledStrings["path"] = stringVectorForLabelledStrings;
		stringVectorForLabelledStrings.clear();

		pugi::xml_node alternativePathForXMLBackupOutputsNode = XMLOutputNode.child("alternativePathForXMLBackupOutputs");
		if ( alternativePathForXMLBackupOutputsNode )
		{
			//std::string alternativePathForXMLBackupOutputs = StringTools::trimStringCopyKeepingInternalBlanks(alternativePathForXMLBackupOutputsNode.child_value());
			std::string alternativePathForXMLBackupOutputs = StringTools::trimStringCopy(alternativePathForXMLBackupOutputsNode.child_value());
			StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(alternativePathForXMLBackupOutputs);
			stringVectorForLabelledStrings.push_back(alternativePathForXMLBackupOutputs);
			outputVerbatimTranslationOfConstructionVariablesToXML.labelledStrings["path"] = stringVectorForLabelledStrings;
			stringVectorForLabelledStrings.clear();
		}

		stringVectorForLabelledStrings.push_back("BackupOutput.xml");
		outputVerbatimTranslationOfConstructionVariablesToXML.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
		stringVectorForLabelledStrings.clear();

		addDoubleVectorToConstructionVariables(outputVerbatimTranslationOfConstructionVariablesToXML,XMLOutputNode,"explicitTimesForOutput",standardOutputCharacteristics.explicitTimesForOutput);
		addDoubleToConstructionVariables(outputVerbatimTranslationOfConstructionVariablesToXML,XMLOutputNode,"outputInterval",standardOutputCharacteristics.outputInterval);
		addIntToConstructionVariables(outputVerbatimTranslationOfConstructionVariablesToXML,XMLOutputNode,"precisionForOutput",standardOutputCharacteristics.precisionForOutput);
		addIntToConstructionVariables(outputVerbatimTranslationOfConstructionVariablesToXML,XMLOutputNode,"numberOfFileIDDigits",4);

		boolVectorForLabelledBools.push_back(false);
		outputVerbatimTranslationOfConstructionVariablesToXML.labelledBools["writeLineEachTimeStep"] = boolVectorForLabelledBools;
		boolVectorForLabelledBools.clear();
		addBoolToConstructionVariables(outputVerbatimTranslationOfConstructionVariablesToXML,XMLOutputNode,"overwriteFiles",true);

		constitutingOutputMethodTypes.push_back(outputVerbatimTranslationOfConstructionVariablesToXML);
		}
	}

	////////////////////// Create standard outputs //////////////////////
	if(createStandardOutputs)
	{
		for(std::vector<std::string>::const_iterator currentProperty = standardOutputProperties.begin(); currentProperty < standardOutputProperties.end(); ++currentProperty)
		{
			if( (*currentProperty) == "accumulatedBedloadTransport" )
			{
				tmpString.clear();
				if(emptyABTNameAlreadyUsed) { tmpString = "standardOutput"; } else { tmpString = ""; }
				constitutingOutputMethodTypes.push_back( createSingleOutputAccumulatedBedloadTransport(pugi::xml_node(),path,tmpString,riverSystemInformation,standardOutputCharacteristics) );
			}
			else
			{
				constitutingOutputMethodTypes.push_back( createSingleRegularOutputMethod(pugi::xml_node(),path,"",riverSystemInformation,standardOutputCharacteristics,true,(*currentProperty)) );
			}
		}
	}

	////////////////////// Create OutputSimulationSetup //////////////////////

	pugi::xml_node outputSimulationSetupNode = outputMethodsNode.child("outputSimulationSetup");
	bool notOutputSimulationSetup = false;
	pugi::xml_node notOutputSimulationSetupNode = outputMethodsNode.child("notOutputSimulationSetup");
	if(notOutputSimulationSetupNode)
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(notOutputSimulationSetupNode.child_value());
		notOutputSimulationSetup = StringTools::stringToBool(tmpString);
	}

	if ( !notOutputSimulationSetup )
	{
		ConstructionVariables outputSimulationSetup;

		outputSimulationSetup.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
		outputSimulationSetup.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputSimulationSetup);

		stringVectorForLabelledStrings.push_back(path);
		outputSimulationSetup.labelledStrings["path"] = stringVectorForLabelledStrings;
		stringVectorForLabelledStrings.clear();

		stringVectorForLabelledStrings.push_back("SimulationSetup.txt");
		outputSimulationSetup.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
		stringVectorForLabelledStrings.clear();

		pugi::xml_node outputSimulationSetupNodeAlternativeFileNameNode = outputSimulationSetupNode.child("name");
		if ( outputSimulationSetupNodeAlternativeFileNameNode )
		{
			std::string alternativeFileName = StringTools::trimStringCopy(outputSimulationSetupNodeAlternativeFileNameNode.child_value());
			stringVectorForLabelledStrings.push_back(alternativeFileName);
			outputSimulationSetup.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
			stringVectorForLabelledStrings.clear();
		}

		boolVectorForLabelledBools.push_back(false);
		outputSimulationSetup.labelledBools["writeLineEachTimeStep"] = boolVectorForLabelledBools;
		boolVectorForLabelledBools.clear();

		doubleVectorForLabelledDoubles.push_back( 900 );
		outputSimulationSetup.labelledDoubles["outputInterval"] = doubleVectorForLabelledDoubles;
		doubleVectorForLabelledDoubles.clear();
		outputSimulationSetup.labelledDoubles["explicitTimesForOutput"] = doubleVectorForLabelledDoubles;
		doubleVectorForLabelledDoubles.clear();

		addIntToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"precisionForOutput",standardOutputCharacteristics.precisionForOutput);

		if( !(addStringVectorToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"setupPropertiesForOutput")) )
		{
			stringVectorForLabelledStrings.push_back(CombinerVariables::typeOfCombinersAndInterfacesToString(CombinerVariables::CalcBedloadCapacity));
			stringVectorForLabelledStrings.push_back(CombinerVariables::typeOfCombinersAndInterfacesToString(CombinerVariables::FlowResistance));
			stringVectorForLabelledStrings.push_back(CombinerVariables::typeOfCombinersAndInterfacesToString(CombinerVariables::CalcGradient));
			stringVectorForLabelledStrings.push_back(CombinerVariables::typeOfCombinersAndInterfacesToString(CombinerVariables::StrataSorting));
			outputSimulationSetup.labelledStrings["setupPropertiesForOutput"] = stringVectorForLabelledStrings;
			stringVectorForLabelledStrings.clear();
		}

		//boolVectorForLabelledBools.push_back(addStringWithInternalBlanksToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"simulationID",""));
		boolVectorForLabelledBools.push_back(addStringToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"simulationID",""));
		outputSimulationSetup.labelledBools["printSimulationID"] = boolVectorForLabelledBools;
		boolVectorForLabelledBools.clear();

		//boolVectorForLabelledBools.push_back(addStringWithInternalBlanksToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"simulationName",""));
		boolVectorForLabelledBools.push_back(addStringToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"simulationName",""));
		outputSimulationSetup.labelledBools["printSimulationName"] = boolVectorForLabelledBools;
		boolVectorForLabelledBools.clear();

		addBoolToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"printStartingTime",true);
		addBoolToConstructionVariables(outputSimulationSetup,outputSimulationSetupNode,"printModelVersion",true);

		constitutingOutputMethodTypes.push_back(outputSimulationSetup);
	}

	//////////////////////// Compose the outputMethods ConstructionVariables ////////////////////////////////
	result.labelledObjects["constitutingOutputMethodTypes"] = constitutingOutputMethodTypes;

	return result;
}

ConstructionVariables StandardInput::createReturnBedslope()
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReturnBedslope);
	std::vector<double> doubleVector;
	doubleVector.push_back((4.0 - std::numeric_limits<double>::max()));
	result.labelledDoubles["minimumSlope"] = doubleVector;
	return result;
}

ConstructionVariables StandardInput::createSimpleDownstreamTwoCellGradient(CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest)
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::CalcGradient;
	result.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::SimpleDownstreamTwoCellGradient);
	std::vector<std::string> stringVector;
	stringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(propertyOfInterest) );
	result.labelledStrings["propertyOfInterest"] = stringVector;
	return result;
}

bool StandardInput::addCalcGradientToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, bool isSedimentEnergySlope, const ConstructionVariables& flowResistance, CombinerVariables::TypesOfFlowMethods typeOfWaterFlowMethods, const ConstructionVariables& defaultValue)
{
	bool result = true;

	pugi::xml_node calcGradientNode = rootNode.child(name.c_str());

	ConstructionVariables calcGradient;

	if ( !calcGradientNode )
	{
		result = false;
		calcGradient = defaultValue;
	}
	else
	{
		calcGradient.interfaceOrCombinerType = CombinerVariables::CalcGradient;

		pugi::xml_node realisationTypeNode = calcGradientNode.child("realisationType");
		if ( !realisationTypeNode )
		{
			const char *const realisationTypeErrorMessage = "The node realisationType is needed for the CalcGradient in StandardInput.";
			throw(realisationTypeErrorMessage);
		}
		calcGradient.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
		bool useHydraulicHeadAsDefaultPropertyOfInterestForSimpleDownstreamTwoCellGradient = false;
		if(isSedimentEnergySlope)
		{
			if( (calcGradient.realisationType == "Energyslope_NoFlowResistancePartitioning") || (calcGradient.realisationType == "Energyslope") || (calcGradient.realisationType == "NoFlowResistancePartitioning") )
			{
				switch (typeOfWaterFlowMethods)
				{
				case CombinerVariables::ImplicitKinematicWave:
					// The missing break is on purpose as Implicit and ExplicitKinematicWave have the same effect
				case CombinerVariables::ExplicitKinematicWave:
					calcGradient.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReturnBedslope);
					break;

				case CombinerVariables::UniformDischarge:
					calcGradient.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::SimpleDownstreamTwoCellGradient);
					useHydraulicHeadAsDefaultPropertyOfInterestForSimpleDownstreamTwoCellGradient = true;
					break;

				default:
					const char *const firstInvalidTypeErrorMessage = "Sorry, this is an internal error. You may avoid this by not using the shorthand EnergySlope_NoFlowResistancePartitioning for sedimentEnergySlopeCalculationMethod.";
					throw (firstInvalidTypeErrorMessage);
				}
			}

			if( (calcGradient.realisationType == "ReducedEnergyslope_WithFlowResistancePartitioning") || (calcGradient.realisationType == "ReducedEnergyslope") || (calcGradient.realisationType == "WithFlowResistancePartitioning") )
			{
				switch (typeOfWaterFlowMethods)
				{
				case CombinerVariables::ImplicitKinematicWave:
					// The missing break is on purpose as Implicit and ExplicitKinematicWave have the same effect
				case CombinerVariables::ExplicitKinematicWave:
					calcGradient.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReducedWaterEnergyslope);
					break;

				case CombinerVariables::UniformDischarge:
					calcGradient.realisationType = CombinerVariables::typeOfGradientCalculationMethodToString(CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable);
					break;

				default:
					const char *const secondInvalidTypeErrorMessage = "Sorry, this is an internal error. You may avoid this by not using the shorthand ReducedEnergySlope_WithFlowResistancePartitioning for sedimentEnergySlopeCalculationMethod.";
					throw (secondInvalidTypeErrorMessage);
				}
			}
		}

		CombinerVariables::TypesOfGradientCalculationMethod realisationType = CombinerVariables::stringToTypeOfGradientCalculationMethod( calcGradient.realisationType );
		bool givenMinimumInputSlope = false;

		switch (realisationType)
		{
		case CombinerVariables::SimpleDownstreamTwoCellGradient:
			if( !( (addStringToConstructionVariables(calcGradient,calcGradientNode,"propertyOfInterest",CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::hydraulicHead))) || useHydraulicHeadAsDefaultPropertyOfInterestForSimpleDownstreamTwoCellGradient ) )
			{
				const char *const propertyOfInterestErrorMessage = "The node propertyOfInterest is needed for the SimpleDownstreamTwoCellGradient in StandardInput.";
				throw(propertyOfInterestErrorMessage);
			}
			break;

		case CombinerVariables::SimpleThreeCellGradient:
			if( !(addStringToConstructionVariables(calcGradient,calcGradientNode,"propertyOfInterest","defaultValue")) )
			{
				const char *const propertyOfInterestErrorMessage = "The node propertyOfInterest is needed for the SimpleThreeCellGradient in StandardInput.";
				throw(propertyOfInterestErrorMessage);
			}
			if( !(addStringToConstructionVariables(calcGradient,calcGradientNode,"weightingProperty","defaultValue")) )
			{
				const char *const weightingPropertyErrorMessage = "The node weightingProperty is needed for the SimpleThreeCellGradient in StandardInput.";
				throw(weightingPropertyErrorMessage);
			}
			break;

		case CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable:
			addCalcGradientToConstructionVariables(calcGradient,calcGradientNode,"simpleWaterEnergyslopeCalculationMethod",false,flowResistance,typeOfWaterFlowMethods,createSimpleDownstreamTwoCellGradient(CombinerVariables::hydraulicHead));

		case CombinerVariables::ReducedWaterEnergyslope:
			addDoubleToConstructionVariables(calcGradient,calcGradientNode,"stressPartitioningExponent",1.5);
			addBoolToConstructionVariables(calcGradient,calcGradientNode,"calculationBasedOnqInsteadOfh",false);
			addDoubleToConstructionVariables(calcGradient,calcGradientNode,"maximumFroudeNumber",((flowResistance.labelledDoubles.find("maximumFroudeNumber"))->second.at(0)));
			givenMinimumInputSlope = addDoubleToConstructionVariables(calcGradient,calcGradientNode,"minimumInputSlope",0.0004);
			addBoolToConstructionVariables(calcGradient,calcGradientNode,"ensureMinimumInputSlope",givenMinimumInputSlope);

			if( (!(givenMinimumInputSlope)) && ((calcGradient.labelledBools.find("ensureMinimumInputSlope"))->second.at(0)) )
			{
				const char *const minimumInputSlopeErrorMessage = "If ensureMinimumInputSlope, then the node minimumInputSlope is needed for the ReducedWaterEnergyslope in StandardInput.";
				throw(minimumInputSlopeErrorMessage);
			}
			break;

		case CombinerVariables::ReturnBedslope: //For both Return-methods the same thing is to do.
		case CombinerVariables::ReturnWaterEnergyslope:
			addDoubleToConstructionVariables(calcGradient,calcGradientNode,"minimumSlope",(4.0 - std::numeric_limits<double>::max()));
			break;

		default:
			const char *const invalidTypeErrorMessage = "Invalid Type of CalcGradient for StandardInput.";
			throw (invalidTypeErrorMessage);
		}
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(calcGradient);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

bool StandardInput::addFlowResistanceToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, CombinerVariables::TypesOfFlowMethods typeOfWaterFlowMethods)
{
	bool result = true;

	ConstructionVariables flowResistance;
	pugi::xml_node flowResistanceNode = rootNode.child(name.c_str());
	if ( !flowResistanceNode )
	{
		result = false;

		flowResistance.interfaceOrCombinerType = CombinerVariables::FlowResistance;
		std::vector<double> doubleVector;
		doubleVector.push_back(0.4);
		flowResistance.labelledDoubles["startingValueForIteration"] = doubleVector;
		doubleVector.clear();
		doubleVector.push_back(0.001);
		flowResistance.labelledDoubles["accuracyForTerminatingIteration"] = doubleVector;
		std::vector<int> intVector;
		intVector.push_back(400);
		flowResistance.labelledInts["maximumNumberOfIterations"] = intVector;
		std::vector<std::string> stringVector;
		stringVector.push_back(CombinerVariables::typeOfNumericRootFinderToString(CombinerVariables::RiddersMethod));
		flowResistance.labelledStrings["typeOfNumericRootFinder"] = stringVector;
		std::vector<bool> boolVector;
		boolVector.push_back((typeOfWaterFlowMethods == CombinerVariables::ImplicitKinematicWave));
		flowResistance.labelledBools["useApproximationsForHydraulicRadius"] = boolVector;
		doubleVector.clear();
		doubleVector.push_back(4.0);
		flowResistance.labelledDoubles["maximumFroudeNumber"] = doubleVector;
		doubleVector.clear();
		doubleVector.push_back(0.0004);
		flowResistance.labelledDoubles["minimumHydraulicSlope"] = doubleVector;

		if(typeOfWaterFlowMethods == CombinerVariables::ImplicitKinematicWave)
		{
			flowResistance.realisationType = CombinerVariables::typeOfFlowResistanceToString(CombinerVariables::FixedPowerLawFlowResistance);
			doubleVector.clear();
			doubleVector.push_back(6.5);
			flowResistance.labelledDoubles["factor"] = doubleVector;
			doubleVector.clear();
			doubleVector.push_back(1.0);
			flowResistance.labelledDoubles["grainsFactor"] = doubleVector;
			doubleVector.clear();
			doubleVector.push_back(84.0);
			flowResistance.labelledDoubles["grainsPercentile"] = doubleVector;
			doubleVector.clear();
			doubleVector.push_back(0.166666667);
			flowResistance.labelledDoubles["exponent"] = doubleVector;
		}
		else
		{
			flowResistance.realisationType = CombinerVariables::typeOfFlowResistanceToString(CombinerVariables::VariablePowerLawFlowResistance);
		}


	}
	else
	{
		flowResistance.interfaceOrCombinerType = CombinerVariables::FlowResistance;

		pugi::xml_node realisationTypeNode = flowResistanceNode.child("realisationType");
		if ( !realisationTypeNode )
		{
			const char *const realisationTypeErrorMessage = "The node realisationType is needed for the FlowResistance in StandardInput.";
			throw(realisationTypeErrorMessage);
		}
		flowResistance.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
		CombinerVariables::TypesOfFlowResistance realisationType = CombinerVariables::stringToTypeOfFlowResistance( flowResistance.realisationType );

		addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"startingValueForIteration",0.4);

		addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"accuracyForTerminatingIteration",0.001);

		addIntToConstructionVariables(flowResistance,flowResistanceNode,"maximumNumberOfIterations",400);

		addStringToConstructionVariables(flowResistance,flowResistanceNode,"typeOfNumericRootFinder",CombinerVariables::typeOfNumericRootFinderToString(CombinerVariables::RiddersMethod));

		addBoolToConstructionVariables(flowResistance,flowResistanceNode,"useApproximationsForHydraulicRadius",(typeOfWaterFlowMethods == CombinerVariables::ImplicitKinematicWave));

		addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"maximumFroudeNumber",4.0);
		addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"minimumHydraulicSlope",0.0004);

		switch (realisationType)
		{
		case CombinerVariables::VariablePowerLawFlowResistance:
			// In this case there is nothing to do.
			break;

		case CombinerVariables::FixedPowerLawFlowResistance:
			addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"factor",6.5);
			addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"grainsFactor",1.0);
			addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"grainsPercentile",84.0);
			addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"exponent",0.166666667);
			break;

		case CombinerVariables::DarcyWeisbachFlowResistance:
			if( !(addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"darcyWeisbachFrictionFactorForBedrock",4.0)) )
			{
				const char *const darcyWeisbachFrictionFactorForBedrockErrorMessage = "The node darcyWeisbachFrictionFactorForBedrock is needed for the DarcyWeisbachFlowResistance in StandardInput.";
				throw(darcyWeisbachFrictionFactorForBedrockErrorMessage);
			}
			if( !(addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"darcyWeisbachFrictionFactorForGravelbed",4.0)) )
			{
				const char *const darcyWeisbachFrictionFactorForGravelbedErrorMessage = "The node darcyWeisbachFrictionFactorForGravelbed is needed for the DarcyWeisbachFlowResistance in StandardInput.";
				throw(darcyWeisbachFrictionFactorForGravelbedErrorMessage);
			}
			break;

		case CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead:
			addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"turbulenceLossFactor",0.0);
			addDoubleToConstructionVariables(flowResistance,flowResistanceNode,"maximumFroudeNumber",0.999);
			addFlowResistanceToConstructionVariables(flowResistance,flowResistanceNode,"usedFlowResistanceRelation",typeOfWaterFlowMethods);
			break;

		default:
			const char *const invalidTypeErrorMessage = "Invalid Type of FlowResistance for StandardInput.";
			throw (invalidTypeErrorMessage);
		}
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(flowResistance);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

ConstructionVariables StandardInput::createWaterFlowMethods (pugi::xml_node rootNode)
{
	ConstructionVariables result;
	result.interfaceOrCombinerType = CombinerVariables::FlowTypeMethods;

	pugi::xml_node realisationTypeNode = rootNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		const char *const realisationTypeErrorMessage = "The node realisationType is needed for the WaterFlowMethods in StandardInput.";
		throw(realisationTypeErrorMessage);
	}
	result.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
	CombinerVariables::TypesOfFlowMethods realisationType = CombinerVariables::stringToTypeOfFlowMethods( result.realisationType );

	switch (realisationType)
	{
	case CombinerVariables::ImplicitKinematicWave:
		addDoubleToConstructionVariables(result,rootNode,"maximumTimeStep",900.0);
		addBoolToConstructionVariables(result,rootNode,"checkForCourantFriedrichsLewy",false);
		break;

	case CombinerVariables::ExplicitKinematicWave:
		// In this case there is nothing to do.
		break;

	case CombinerVariables::UniformDischarge:
		addDoubleToConstructionVariables(result,rootNode,"maximumTimeStep",900.0);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of WaterFlowMethods for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

std::pair<ConstructionVariables,bool> StandardInput::createBedLoadCapacityCalculationMethodWithBoolNonFractional (pugi::xml_node rootNode)
{
	bool nonFractional = false;
	ConstructionVariables result;
	result.interfaceOrCombinerType = CombinerVariables::CalcBedloadCapacity;

	pugi::xml_node realisationTypeNode = rootNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		const char *const realisationTypeErrorMessage = "The node realisationType is needed for the bedloadTransportEquations in StandardInput.";
		throw(realisationTypeErrorMessage);
	}
	result.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
	CombinerVariables::TypesOfBedloadCapacityCalculationMethod realisationType = CombinerVariables::stringToTypeOfBedloadCapacityCalculationMethod( result.realisationType );

	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;

	switch (realisationType)
	{
	case CombinerVariables::ChengBedloadCapacity:
		addBoolToConstructionVariables(result,rootNode,"thetaCriticalBasedOnConstantSred",false);
		addDoubleToConstructionVariables(result,rootNode,"factor",13.0);
		addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(result,rootNode,"thresholdCalculationMethod");
		addCalcHidingFactorsToConstructionVariables(result,rootNode,"hidingFactorsCalculationMethod");
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnTheta:
		addBoolToConstructionVariables(result,rootNode,"thetaCriticalBasedOnConstantSred",false);
		addBoolToConstructionVariables(result,rootNode,"useOnePointOneAsExponentForFroudeNumber",false);
		addBoolToConstructionVariables(result,rootNode,"simplifiedEquation",true);
		addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(result,rootNode,"thresholdCalculationMethod");
		addCalcHidingFactorsToConstructionVariables(result,rootNode,"hidingFactorsCalculationMethod");
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnThetaNonFractional:
		addBoolToConstructionVariables(result,rootNode,"thetaCriticalBasedOnConstantSred",false);
		if( !(addBoolToConstructionVariables(result,rootNode,"takeArmourLayerIntoAccount",false)) )
		{
			const char *const thresholdCalculationMethodErrorMessage = "The node takeArmourLayerIntoAccount is needed for the BedLoadCapacityCalculationMethod RickenmannBedloadCapacityBasedOnThetaNonFractional in StandardInput.";
			throw(thresholdCalculationMethodErrorMessage);
		}
		addBoolToConstructionVariables(result,rootNode,"useOnePointOneAsExponentForFroudeNumber",false);
		addBoolToConstructionVariables(result,rootNode,"simplifiedEquation",true);
		addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(result,rootNode,"thresholdCalculationMethod");
		nonFractional = true;
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnq:
		addBoolToConstructionVariables(result,rootNode,"correctionForBedloadWeightAtSteepSlopes",true);
		addCalcHidingFactorsToConstructionVariables(result,rootNode,"hidingFactorsCalculationMethod");
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnqNonFractional:
		addBoolToConstructionVariables(result,rootNode,"correctionForBedloadWeightAtSteepSlopes",true);
		if( !(addBoolToConstructionVariables(result,rootNode,"takeArmourLayerIntoAccount",false)) )
		{
			const char *const thresholdCalculationMethodErrorMessage = "The node takeArmourLayerIntoAccount is needed for the BedLoadCapacityCalculationMethod RickenmannBedloadCapacityBasedOnqNonFractional in StandardInput.";
			throw(thresholdCalculationMethodErrorMessage);
		}
		nonFractional = true;
		break;

	case CombinerVariables::WilcockCroweBedloadCapacity:
		{
			pugi::xml_node constantSandFractionNode = rootNode.child("constantSandFraction");
			addBoolToConstructionVariables(result,rootNode,"useConstantSandFraction",(constantSandFractionNode != 0));
			boolMapIterator = result.labelledBools.find("useConstantSandFraction");
			if( (boolMapIterator->second.at(0)) )
			{
				if( !(addDoubleToConstructionVariables(result,rootNode,"constantSandFraction",0.4)) )
				{
					const char *const constantSandFractionErrorMessage = "The node constantSandFraction is needed for the BedLoadCapacityCalculationMethod WilcockCroweBedloadCapacity in StandardInput.";
					throw(constantSandFractionErrorMessage);
				}
			}
			else
			{
				std::cout << std::endl << "Warning: It is not recommended to use WilcockCroweBedloadCapacity without constantSandFraction." << std::endl;
			}
		}
		break;

	case CombinerVariables::SchneiderEtAlBedloadCapacity:
		addDoubleToConstructionVariables(result,rootNode,"minimumDimensionlessReferenceShearStressForMeanDiameter",0.021);
		addBoolToConstructionVariables(result,rootNode,"referenceShearStressBasedOnSlopeANDGrainsizeDistribution",false);
		break;

	case CombinerVariables::ReckingBedloadCapacityNonFractional:
		nonFractional = true;
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of bedloadTransportEquations for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::pair<ConstructionVariables,bool> completeResult = std::make_pair(result,nonFractional);

	return completeResult;
}

bool StandardInput::addAdditionalRiverReachMethodsTypes(std::vector<ConstructionVariables>& targetConstitutingAdditionalMethodTypes, pugi::xml_node riverSystemMethodsNode)
{
	bool result = false;
	pugi::xml_node additionalMethodsNode = riverSystemMethodsNode.child("additionalMethods");
	if ( additionalMethodsNode != 0 )
	{
		pugi::xml_node sternbergAbrasionIncludingFiningNode = additionalMethodsNode.child(CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::SternbergAbrasionIncludingFining).c_str());
		if( sternbergAbrasionIncludingFiningNode != 0 )
		{
			ConstructionVariables sternbergAbrasionIncludingFining;
			sternbergAbrasionIncludingFining.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
			sternbergAbrasionIncludingFining.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::SternbergAbrasionIncludingFining);
			if( !(addDoubleToConstructionVariables(sternbergAbrasionIncludingFining,sternbergAbrasionIncludingFiningNode,"sternbergAbrasionCoefficient",0.4)) )
			{
				const char *const sternbergAbrasionCoefficientFirstErrorMessage = "The node sternbergAbrasionCoefficient is needed for the AdditionalMethod SternbergAbrasionIncludingFining in StandardInput.";
				throw(sternbergAbrasionCoefficientFirstErrorMessage);
			}
			targetConstitutingAdditionalMethodTypes.push_back(sternbergAbrasionIncludingFining);
			result = true;
		}
		else //Check only for the SternbergAbrasionWithoutFining if there is no SternbergAbrasionIncludingFining.
		{
			pugi::xml_node sternbergAbrasionWithoutFiningNode = additionalMethodsNode.child(CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::SternbergAbrasionWithoutFining).c_str());
			if( sternbergAbrasionWithoutFiningNode )
			{
				ConstructionVariables sternbergAbrasionWithoutFining;
				sternbergAbrasionWithoutFining.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
				sternbergAbrasionWithoutFining.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::SternbergAbrasionWithoutFining);
				if( !(addDoubleToConstructionVariables(sternbergAbrasionWithoutFining,sternbergAbrasionWithoutFiningNode,"sternbergAbrasionCoefficient",0.4)) )
				{
					const char *const sternbergAbrasionCoefficientSecondErrorMessage = "The node sternbergAbrasionCoefficient is needed for the AdditionalMethod SternbergAbrasionWithoutFining in StandardInput.";
					throw(sternbergAbrasionCoefficientSecondErrorMessage);
				}
				targetConstitutingAdditionalMethodTypes.push_back(sternbergAbrasionWithoutFining);
				result = true;
			}
		}
	}
	return result;
}

bool StandardInput::addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name)
{

	pugi::xml_node calcThresholdForInitiationOfBedloadMotionNode = rootNode.child(name.c_str());
	bool result = ( calcThresholdForInitiationOfBedloadMotionNode );

	ConstructionVariables calcThresholdForInitiationOfBedloadMotion;
	calcThresholdForInitiationOfBedloadMotion.interfaceOrCombinerType = CombinerVariables::CalcThresholdForInitiationOfBedloadMotion;

	pugi::xml_node realisationTypeNode = calcThresholdForInitiationOfBedloadMotionNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		calcThresholdForInitiationOfBedloadMotion.realisationType = CombinerVariables::typeOfCalculationMethodForBedloadMotionInitiationThresholdToString(CombinerVariables::LambEtAlCriticalTheta);
	}
	else
	{
		calcThresholdForInitiationOfBedloadMotion.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
	}
	CombinerVariables::TypesOfCalculationMethodForBedloadMotionInitiationThreshold realisationType = CombinerVariables::stringToTypeOfCalculationMethodForBedloadMotionInitiationThreshold( calcThresholdForInitiationOfBedloadMotion.realisationType );

	switch (realisationType)
	{
	case CombinerVariables::LambEtAlCriticalTheta:
		addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"minimumCriticalDimensionlessShearStress",0.03);
		addBoolToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"correctionForBedloadWeightAtSteepCounterSlopes",true);
		break;

	case CombinerVariables::ConstantThresholdForInitiationOfBedloadMotion:
		if( !(addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"constantThreshold",0.04)) )
		{
			const char *const constantThresholdErrorMessage = "The node constantThreshold is needed for the ConstantThresholdForInitiationOfBedloadMotion in StandardInput.";
			throw(constantThresholdErrorMessage);
		}
		addBoolToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"correctionForBedloadWeightAtSteepCounterSlopes",true);
		break;

	case CombinerVariables::StochasticThresholdForInitiationOfBedloadMotion:
		addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"minimumThresholdValue",0.0004);
		if( !(addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"miu",0.04)) )
		{
			const char *const miuErrorMessage = "The node miu is needed for the StochasticThresholdForInitiationOfBedloadMotion in StandardInput.";
			throw(miuErrorMessage);
		}
		if( !(addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"beta",0.04)) )
		{
			const char *const betaErrorMessage = "The node beta is needed for the StochasticThresholdForInitiationOfBedloadMotion in StandardInput.";
			throw(betaErrorMessage);
		}
		addIntToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"seed",4);
		if( !(addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"weightForCurrent",0.04)) )
		{
			const char *const weightForCurrentErrorMessage = "The node weightForCurrent is needed for the StochasticThresholdForInitiationOfBedloadMotion in StandardInput.";
			throw(weightForCurrentErrorMessage);
		}
		if( !(addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"weightForPrevious",0.04)) )
		{
			const char *const weightForPreviousErrorMessage = "The node weightForPrevious is needed for the StochasticThresholdForInitiationOfBedloadMotion in StandardInput.";
			throw(weightForPreviousErrorMessage);
		}
		if( !(addDoubleToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"weightForPrePrevious",0.04)) )
		{
			const char *const weightForPrePreviousErrorMessage = "The node weightForPrePrevious is needed for the StochasticThresholdForInitiationOfBedloadMotion in StandardInput.";
			throw(weightForPrePreviousErrorMessage);
		}
		addBoolToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"correctionForBedloadWeightAtSteepCounterSlopes",true);
		{
			std::vector<double> emptyDoubleVector;
			addDoubleVectorToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"widthsForSpecialValues",emptyDoubleVector);
			addDoubleVectorToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"miuSpecialValues",emptyDoubleVector);
			addDoubleVectorToConstructionVariables(calcThresholdForInitiationOfBedloadMotion,calcThresholdForInitiationOfBedloadMotionNode,"betaSpecialValues",emptyDoubleVector);
		}
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of CalcThresholdForInitiationOfBedloadMotion for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(calcThresholdForInitiationOfBedloadMotion);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

bool StandardInput::addCalcHidingFactorsToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name)
{
	pugi::xml_node calcHidingFactorsNode = rootNode.child(name.c_str());
	bool calcHidingFactorsGiven = calcHidingFactorsNode;

	ConstructionVariables calcHidingFactors;
	calcHidingFactors.interfaceOrCombinerType = CombinerVariables::CalcHidingFactors;

	pugi::xml_node realisationTypeNode = calcHidingFactorsNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		calcHidingFactors.realisationType = CombinerVariables::typeOfHidingFactorsCalculationMethodToString(CombinerVariables::PowerLawHidingFunction);
	}
	else
	{
		calcHidingFactors.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
	}
	CombinerVariables::TypesOfHidingFactorsCalculationMethod realisationType = CombinerVariables::stringToTypeOfHidingFactorsCalculationMethod( calcHidingFactors.realisationType );

	switch (realisationType)
	{
	case CombinerVariables::PowerLawHidingFunction:
		addDoubleToConstructionVariables(calcHidingFactors,calcHidingFactorsNode,"referenceDiameterPercentile",50.0);

		addDoubleToConstructionVariables(calcHidingFactors,calcHidingFactorsNode,"exponent",-0.8);
		break;

	case CombinerVariables::WilcockCroweHidingFunction:
		// In this case there is nothing to do.
		break;

	case CombinerVariables::NoHiding:
		// In this case there is nothing to do.
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of CalcHidingFactors for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(calcHidingFactors);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return calcHidingFactorsGiven;
}

ConstructionVariables StandardInput::createRiverReachMethodsWithCertainCellID (pugi::xml_node riverSystemMethodsNode, int cellID, bool nonFractional, const std::map<int,ConstructionVariables>& mapFromCellIDToInstantaneousSedimentInputs)
{
	ConstructionVariables riverReachMethods;
	riverReachMethods.interfaceOrCombinerType = CombinerVariables::RiverReachMethods;

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;

	std::vector<int> intVector;
	intVector.push_back(cellID);
	riverReachMethods.labelledInts["cellID"] = intVector;

	std::vector<ConstructionVariables> constitutingAdditionalRiverReachMethodTypes;
	addAdditionalRiverReachMethodsTypes(constitutingAdditionalRiverReachMethodTypes,riverSystemMethodsNode);
	std::map<int,ConstructionVariables>::const_iterator instantaneousSedimentInputs = mapFromCellIDToInstantaneousSedimentInputs.find(cellID);
	if( instantaneousSedimentInputs != mapFromCellIDToInstantaneousSedimentInputs.end() )
		{ constitutingAdditionalRiverReachMethodTypes.push_back(instantaneousSedimentInputs->second); }
	if(nonFractional)
	{
		ConstructionVariables useLocalGrainSizeDistributionForDepositedMaterial;
		useLocalGrainSizeDistributionForDepositedMaterial.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
		useLocalGrainSizeDistributionForDepositedMaterial.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::PreventLocalGrainSizeDistributionChanges);
		constitutingAdditionalRiverReachMethodTypes.push_back(useLocalGrainSizeDistributionForDepositedMaterial);
	}
	ConstructionVariables additionalRiverReachMethods;
	additionalRiverReachMethods.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethods;
	additionalRiverReachMethods.labelledObjects["constitutingAdditionalRiverReachMethodTypes"] = constitutingAdditionalRiverReachMethodTypes;

	ConstructionVariables regularRiverReachMethods;
	regularRiverReachMethods.interfaceOrCombinerType = CombinerVariables::RegularRiverReachMethods;

	addCalcTauToConstructionVariables(regularRiverReachMethods,riverSystemMethodsNode,"tauCalculationMethod",CombinerVariables::EnergyslopeTau);
	addCalcActiveWidthToConstructionVariables(regularRiverReachMethods,riverSystemMethodsNode,"activeWidthCalculationMethod",CombinerVariables::SetActiveWidthEqualFlowWidth);

	// Compose riverReachMethods;
	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(additionalRiverReachMethods);
	riverReachMethods.labelledObjects["additionalRiverReachMethods"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(regularRiverReachMethods);
	riverReachMethods.labelledObjects["regularRiverReachMethods"] = tmpConstructionVariablesVector;

	return riverReachMethods;
}

std::vector<ConstructionVariables> StandardInput::createDischargeTimeSeriesFromSpreadsheets (std::string path, const RiverSystemInformation& riverSystemInformation)
{
	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);
	std::vector<ConstructionVariables> result;
	int currentRiverBranchID;
	int currentUpmostCellID;
	std::string currentFileName;
	std::ostringstream oStringStream;
	std::map<std::string,std::vector<std::string> > currentSpreadsheetReadOut;
	std::map<std::string,std::vector<std::string> >::const_iterator iteratorForSpreadsheetReadOut;
	std::vector<double> currentElapsedSeconds;
	std::vector<double>	currentDischargeValues;
	std::vector<int> tmpIntVector;
	std::vector<bool> marginBoolVector;
	marginBoolVector.push_back(true);
	std::vector<bool> surplusInsteadOfAbsoluteValuesBoolVector;
	surplusInsteadOfAbsoluteValuesBoolVector.push_back(false);
	std::vector<std::string> propertyStringVector;
	propertyStringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::discharge) );
	std::vector<ConstructionVariables> timeSeriesEntriesVector;
	std::vector<double> tmpDoubleVector;

	for(std::vector<RiverBranchForStandardInput>::const_iterator currentRiverBranch = riverSystemInformation.riverBranches.begin(); currentRiverBranch < riverSystemInformation.riverBranches.end(); ++currentRiverBranch)
	{
		if(currentRiverBranch->upstreamMargin)
		{
			currentRiverBranchID = currentRiverBranch->branchID;
			currentUpmostCellID = currentRiverBranch->topmostCellID;
			currentFileName.clear();
			currentFileName = path;
			currentFileName.append("Branch");
			oStringStream.str("");
			oStringStream.clear();
			oStringStream << currentRiverBranchID << std::flush;
			currentFileName.append(oStringStream.str());
			currentFileName.append("Discharge.txt");
			currentSpreadsheetReadOut.clear();
			currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ElapsedSeconds");
			if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
			{
				const char *const elapsedSecondsErrorMessage = "The column ElapsedSeconds is needed within discharge spreadsheets for StandardInput input reader.";
				throw(elapsedSecondsErrorMessage);
			}
			currentElapsedSeconds.clear();
			currentElapsedSeconds = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("DischargeInM3PerS");
			if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
			{
				const char *const dischargeErrorMessage = "The column DischargeInM3PerS is needed within discharge spreadsheets for StandardInput input reader.";
				throw(dischargeErrorMessage);
			}
			currentDischargeValues.clear();
			currentDischargeValues = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

			ConstructionVariables currentTimeSeries;
			currentTimeSeries.interfaceOrCombinerType = CombinerVariables::TimeSeries;
			tmpIntVector.clear();
			//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
			tmpIntVector.push_back((currentUpmostCellID-1000));
			currentTimeSeries.labelledInts["userCellID"] = tmpIntVector;
			currentTimeSeries.labelledBools["margin"] = marginBoolVector;
			currentTimeSeries.labelledBools["surplusInsteadOfAbsoluteValues"] = surplusInsteadOfAbsoluteValuesBoolVector;
			currentTimeSeries.labelledStrings["property"] = propertyStringVector;

			timeSeriesEntriesVector.clear();
			for(std::vector<double>::const_iterator currentElapsedSecondsEntry = currentElapsedSeconds.begin(), currentDischargeValuesEntry = currentDischargeValues.begin(); currentElapsedSecondsEntry < currentElapsedSeconds.end(); ++currentElapsedSecondsEntry, ++currentDischargeValuesEntry)
			{
				ConstructionVariables currentTimeSeriesEntry;
				currentTimeSeriesEntry.interfaceOrCombinerType = CombinerVariables::TimeSeriesEntry;
				tmpDoubleVector.clear();
				tmpDoubleVector.push_back( *currentElapsedSecondsEntry );
				currentTimeSeriesEntry.labelledDoubles["elapsedSeconds"] = tmpDoubleVector;
				tmpDoubleVector.clear();
				tmpDoubleVector.push_back( *currentDischargeValuesEntry );
				currentTimeSeriesEntry.labelledDoubles["doubleValue"] = tmpDoubleVector;

				timeSeriesEntriesVector.push_back( currentTimeSeriesEntry );
			}
			currentTimeSeries.labelledObjects["timeSeries"] = timeSeriesEntriesVector;

			result.push_back(currentTimeSeries);
		}
	}

	return result;
}

std::vector<ConstructionVariables> StandardInput::createSedimentInputTimeSeriesFromSpreadsheets (std::string path, const RiverSystemInformation& riverSystemInformation, bool inputUpperBoundaryInsteadOfMeanGrainDiameter, bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters, double lowerDiameterBoundaryForFinestFractionInCM, double poreVolumeFraction)
{
	std::vector<ConstructionVariables> result;

	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);
	std::string dischargeAndOtherInputsPath = path;
	dischargeAndOtherInputsPath.append("DischargeAndOtherInputs/");

	std::string currentFileName = dischargeAndOtherInputsPath;
	currentFileName.append("SedimentInputs.txt");

	if( StringTools::fileExists(currentFileName) )
	{
		std::string grainSizeDistributionsPath = path;
		grainSizeDistributionsPath.append("GrainSizeDistributions/");

		double conversionFactorFromPorousToSolidVolume =  1.0 - poreVolumeFraction ;

		int currentCellID;
		std::map<std::string,std::vector<std::string> > currentSpreadsheetReadOut;
		std::map<std::string,std::vector<std::string> >::const_iterator iteratorForSpreadsheetReadOut;
		std::vector<double> currentElapsedSeconds;
		std::vector<double>	currentSedimentInputs;
		std::vector<bool> marginBoolVector;
		marginBoolVector.push_back(false);
		std::vector<bool> tmpBoolVector;
		std::vector<std::string> propertyStringVector;
		propertyStringVector.push_back( CombinerVariables::typeOfRegularRiverReachPropertiesToString(CombinerVariables::depositionRate) );
		std::vector<ConstructionVariables> timeSeriesEntriesVector;
		std::map<std::string,Grains> mapFromGrainSizeDistributionNameToGrainsObject;
		std::map<std::string,Grains>::const_iterator grainSizeDistributionMapIterator;
		std::map<std::pair<int,std::string>,int>::const_iterator cellIDMapIterator;
		std::pair<int,std::string> currentBranchIDAndKilometrage;
		std::vector<int> tmpIntVector;
		std::vector<double> tmpDoubleVector;
		std::vector<ConstructionVariables> tmpConstructionVariablesVector;
		HighestOrderStructuresPointers tmpHighestOrderStructuresPointers;
		Grains* tmpGrainsPointer;

		currentSpreadsheetReadOut.clear();
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("BranchIDs");
		if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
		{
			const char *const branchIDsErrorMessage = "The column BranchIDs is needed in the SedimentInputs spreadsheet for StandardInput input reader.";
			throw(branchIDsErrorMessage);
		}
		std::vector<int> branchIDs = StringTools::stringVectorToIntVector( iteratorForSpreadsheetReadOut->second );

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("KilometrageUpstreamDirected");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const kilometrageUpstreamDirectedErrorMessage = "The column KilometrageUpstreamDirected is needed for the SedimentInputs spreadsheets within StandardInput input reader.";
			throw(kilometrageUpstreamDirectedErrorMessage);
		}
		std::vector<std::string> kilometrages = iteratorForSpreadsheetReadOut->second;

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("GrainSizeDistribution");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const grainSizeDistributionErrorMessage = "The column GrainSizeDistribution is needed for the SedimentInputs spreadsheets within StandardInput input reader.";
			throw(grainSizeDistributionErrorMessage);
		}
		std::vector<std::string> grainSizeDistributions = iteratorForSpreadsheetReadOut->second;

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("SedimentInputTimeSeries");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const sedimentInputTimeSeriesErrorMessage = "The column SedimentInputTimeSeries is needed for the SedimentInputs spreadsheets within StandardInput input reader.";
			throw(sedimentInputTimeSeriesErrorMessage);
		}
		std::vector<std::string> sedimentInputTimeSeriesFileNames = iteratorForSpreadsheetReadOut->second;

		std::vector<bool> replacingRegularDepositionRateInsteadOfAddingToIt;
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ReplacingRegularDepositionRateInsteadOfAddingToIt");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			replacingRegularDepositionRateInsteadOfAddingToIt = std::vector<bool>(kilometrages.size(),false);
		}
		else
		{
			replacingRegularDepositionRateInsteadOfAddingToIt = StringTools::stringVectorToBoolVector( iteratorForSpreadsheetReadOut->second );
		}

		std::vector<bool> inputIncludingPoreVolume;
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("InputIncludingPoreVolume");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			inputIncludingPoreVolume = std::vector<bool>(kilometrages.size(),true);
		}
		else
		{
			inputIncludingPoreVolume = StringTools::stringVectorToBoolVector( iteratorForSpreadsheetReadOut->second );
		}

		std::vector<int>::const_iterator currentBranchID = branchIDs.begin();
		std::vector<bool>::const_iterator currentBoolReplacingRegularDepositionRateInsteadOfAddingToIt = replacingRegularDepositionRateInsteadOfAddingToIt.begin();
		std::vector<bool>::const_iterator currentBoolInputIncludingPoreVolume = inputIncludingPoreVolume.begin();
		for(std::vector<std::string>::const_iterator currentKilometrage = kilometrages.begin(), currentGrainSizeDistribution = grainSizeDistributions.begin(), currentSedimentInputTimeSeries = sedimentInputTimeSeriesFileNames.begin(); currentKilometrage < kilometrages.end(); ++currentBranchID, ++currentKilometrage, ++currentGrainSizeDistribution, ++currentSedimentInputTimeSeries, ++currentBoolReplacingRegularDepositionRateInsteadOfAddingToIt, ++currentBoolInputIncludingPoreVolume)
		{
			currentBranchIDAndKilometrage = std::make_pair(*currentBranchID,*currentKilometrage);
			cellIDMapIterator = riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID.find(currentBranchIDAndKilometrage);
			if(cellIDMapIterator == riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID.end())
			{
				const char *const unknownLocationErrorMessage = "A location (branchID and kilometrage) used in SedimentInputs has not been defined in the longitudinal profile yet.";
				throw(unknownLocationErrorMessage);
			}
			currentCellID = cellIDMapIterator->second;
			grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToGrainsObject.find(*currentGrainSizeDistribution);
			if( grainSizeDistributionMapIterator == mapFromGrainSizeDistributionNameToGrainsObject.end() )
			{
				currentFileName.clear();
				currentFileName = grainSizeDistributionsPath;
				currentFileName.append(*currentGrainSizeDistribution);
				currentFileName.append(".txt");
				std::pair<std::vector<double>,ConstructionVariables> currentFractionalGrainDiametersAndGrainsConstructionVariables = createFractionalGrainDiametersAndGrainsConstructionVariablesFromSpreadsheet(currentFileName,inputUpperBoundaryInsteadOfMeanGrainDiameter,useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters,lowerDiameterBoundaryForFinestFractionInCM,riverSystemInformation.usedTypesOfGrains);
				std::vector<double> currentFractionalGrainDiameters = currentFractionalGrainDiametersAndGrainsConstructionVariables.first;

				if( riverSystemInformation.usedFractionalGrainDiameters.size() != currentFractionalGrainDiameters.size() )
				{
					const char *const grainSizesNumberErrorMessage = "The number of treated grain sizes needs to be the same in all corresponding spreadsheets.";
					throw(grainSizesNumberErrorMessage);
				}
				for(std::vector<double>::const_iterator currentRiverSystemGrainSize = riverSystemInformation.usedFractionalGrainDiameters.begin(), currentGrainSizeOfCurrentGrains = currentFractionalGrainDiameters.begin(); currentGrainSizeOfCurrentGrains < currentFractionalGrainDiameters.end(); ++currentRiverSystemGrainSize, ++currentGrainSizeOfCurrentGrains)
				{
					if ( *currentRiverSystemGrainSize != *currentGrainSizeOfCurrentGrains )
					{
						const char *const sameGrainSizesFractionsErrorMessage = "The treated grain sizes need to be exactly the same in all corresponding spreadsheets.";
						throw(sameGrainSizesFractionsErrorMessage);
					}
				}

				ConstructionVariables grainsConstructionVariables = currentFractionalGrainDiametersAndGrainsConstructionVariables.second;
				tmpGrainsPointer = static_cast<Grains*>(SedFlowBuilders::generalBuilder(grainsConstructionVariables,tmpHighestOrderStructuresPointers));
				//Normalise Grains so that the fractions sum up to 1.
				*tmpGrainsPointer /= tmpGrainsPointer->getOverallVolume();
				mapFromGrainSizeDistributionNameToGrainsObject[*currentGrainSizeDistribution] = Grains(*tmpGrainsPointer);
				delete tmpGrainsPointer;
				grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToGrainsObject.find(*currentGrainSizeDistribution);
			}
			Grains currentGrainsBase = grainSizeDistributionMapIterator->second;
			currentFileName.clear();
			currentFileName = dischargeAndOtherInputsPath;
			currentFileName.append(*currentSedimentInputTimeSeries);
			currentFileName.append(".txt");
			currentSpreadsheetReadOut.clear();
			currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ElapsedSeconds");
			if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
			{
				const char *const elapsedSecondsErrorMessage = "The column ElapsedSeconds is needed within sediment input weighting factor time series spreadsheets for StandardInput input reader.";
				throw(elapsedSecondsErrorMessage);
			}
			currentElapsedSeconds.clear();
			currentElapsedSeconds = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("SedimentInputInM3PerS");
			if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
			{
				const char *const SedimentInputInMCubicPerSErrorMessage = "The column SedimentInputInM3PerS is needed within sediment input weighting factor time series spreadsheets for StandardInput input reader.";
				throw(SedimentInputInMCubicPerSErrorMessage);
			}
			currentSedimentInputs.clear();
			currentSedimentInputs = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );
			if( *currentBoolInputIncludingPoreVolume )
			{
				std::transform(currentSedimentInputs.begin(),currentSedimentInputs.end(),currentSedimentInputs.begin(),std::bind1st(std::multiplies<double>(),conversionFactorFromPorousToSolidVolume));
			}

			ConstructionVariables currentTimeSeries;
			currentTimeSeries.interfaceOrCombinerType = CombinerVariables::TimeSeries;
			tmpIntVector.clear();
			//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
			tmpIntVector.push_back((currentCellID-1000));
			currentTimeSeries.labelledInts["userCellID"] = tmpIntVector;
			currentTimeSeries.labelledBools["margin"] = marginBoolVector;
			tmpBoolVector.clear();
			tmpBoolVector.push_back( !(*currentBoolReplacingRegularDepositionRateInsteadOfAddingToIt) );
			currentTimeSeries.labelledBools["surplusInsteadOfAbsoluteValues"] = tmpBoolVector;
			tmpBoolVector.clear();
			tmpBoolVector.push_back( true );
			currentTimeSeries.labelledBools["finalInsteadOfInitialAdding"] = tmpBoolVector;
			currentTimeSeries.labelledStrings["property"] = propertyStringVector;

			timeSeriesEntriesVector.clear();
			for(std::vector<double>::const_iterator currentElapsedSecondsEntry = currentElapsedSeconds.begin(), currentSedimentInputEntry = currentSedimentInputs.begin(); currentElapsedSecondsEntry < currentElapsedSeconds.end(); ++currentElapsedSecondsEntry, ++currentSedimentInputEntry)
			{
				ConstructionVariables currentTimeSeriesEntry;
				currentTimeSeriesEntry.interfaceOrCombinerType = CombinerVariables::TimeSeriesEntry;
				tmpDoubleVector.clear();
				tmpDoubleVector.push_back( *currentElapsedSecondsEntry );
				currentTimeSeriesEntry.labelledDoubles["elapsedSeconds"] = tmpDoubleVector;

				Grains grainsObjectForCurrentGrainsEntry = currentGrainsBase * (*currentSedimentInputEntry);
				ConstructionVariables currentGrainsEntry = grainsObjectForCurrentGrainsEntry.createConstructionVariables();

				tmpConstructionVariablesVector.clear();
				tmpConstructionVariablesVector.push_back( currentGrainsEntry );
				currentTimeSeriesEntry.labelledObjects["grainsValue"] = tmpConstructionVariablesVector;

				timeSeriesEntriesVector.push_back( currentTimeSeriesEntry );
			}
			currentTimeSeries.labelledObjects["timeSeries"] = timeSeriesEntriesVector;

			result.push_back(currentTimeSeries);
		}
	}

	return result;
}

bool StandardInput::addCalcTauToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, CombinerVariables::TypesOfTauCalculationMethod defaultValue)
{
	bool result = true;
	std::string tmpString;

	CombinerVariables::TypesOfTauCalculationMethod realisationType;

	pugi::xml_node calcTauNode = rootNode.child(name.c_str());
	if ( !calcTauNode ) { realisationType = defaultValue; result = false; }
	else
	{
		pugi::xml_node realisationTypeNode = calcTauNode.child("realisationType");
		if ( !realisationTypeNode )
		{
			const char *const realisationTypeErrorMessage = "The node realisationType is needed for the TauCalculationMethod in StandardInput.";
			throw(realisationTypeErrorMessage);
		}
		tmpString = StringTools::trimStringCopy(realisationTypeNode.child_value());
		realisationType = CombinerVariables::stringToTypeOfTauCalculationMethod( tmpString );
		tmpString.clear();
	}


	ConstructionVariables calcTau;
	calcTau.interfaceOrCombinerType = CombinerVariables::CalcTau;
	calcTau.realisationType = CombinerVariables::typeOfTauCalculationMethodToString(realisationType);

	switch (realisationType)
	{
	case CombinerVariables::EnergyslopeTauBasedOnFlowDepth:
		addBoolToConstructionVariables(calcTau,calcTauNode,"correctionForBedloadWeightAtSteepSlopes",true);
		break;

	case CombinerVariables::EnergyslopeTau:
		addBoolToConstructionVariables(calcTau,calcTauNode,"correctionForBedloadWeightAtSteepSlopes",true);
		break;

	case CombinerVariables::FlowVelocityTau:
		// In this case there is nothing to do.
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of TauCalculationMethod for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(calcTau);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

bool StandardInput::addCalcBedloadVelocityToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name)
{
	bool result = true;

	CombinerVariables::TypesOfBedloadVelocityCalculationMethod realisationType;
	ConstructionVariables calcBedloadVelocity;
	calcBedloadVelocity.interfaceOrCombinerType = CombinerVariables::CalcBedloadVelocity;

	pugi::xml_node calcBedloadVelocityNode = rootNode.child(name.c_str());
	pugi::xml_node realisationTypeNode = calcBedloadVelocityNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		result = false;
		realisationType = CombinerVariables::VelocityAsTransportRatePerUnitCrossSectionalArea;
		calcBedloadVelocity.realisationType = CombinerVariables::typeOfBedloadVelocityCalculationMethodToString(realisationType);
	}
	else
	{
		calcBedloadVelocity.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
		realisationType = CombinerVariables::stringToTypeOfBedloadVelocityCalculationMethod( calcBedloadVelocity.realisationType );
	}

	switch (realisationType)
	{
	case CombinerVariables::JulienBounvilayRollingParticlesVelocity:
		addDoubleToConstructionVariables(calcBedloadVelocity,calcBedloadVelocityNode,"roughnessDiameterPercentile",84.0);
		addDoubleToConstructionVariables(calcBedloadVelocity,calcBedloadVelocityNode,"roughnessDiameterFactor",3.5);
		break;

	case CombinerVariables::VelocityAsTransportRatePerUnitCrossSectionalArea:
		addEstimateThicknessOfMovingSedimentLayerToConstructionVariables(calcBedloadVelocity,calcBedloadVelocityNode,"estimateThicknessOfMovingSedimentLayer");
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of BedloadVelocityCalculationMethod for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(calcBedloadVelocity);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

bool StandardInput::addEstimateThicknessOfMovingSedimentLayerToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name)
{
	bool result = true;
	std::string tmpString;

	CombinerVariables::TypesOfEstimationMethodForThicknessOfMovingSedimentLayer realisationType;
	ConstructionVariables estimateThicknessOfMovingSedimentLayer;
	estimateThicknessOfMovingSedimentLayer.interfaceOrCombinerType = CombinerVariables::EstimateThicknessOfMovingSedimentLayer;

	pugi::xml_node estimateThicknessOfMovingSedimentLayerNode = rootNode.child(name.c_str());
	pugi::xml_node realisationTypeNode = estimateThicknessOfMovingSedimentLayerNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		result = false;
		realisationType = CombinerVariables::ConstantThicknessOfMovingSedimentLayer;
		estimateThicknessOfMovingSedimentLayer.realisationType = CombinerVariables::typeOfEstimationMethodForThicknessOfMovingSedimentLayerToString(realisationType);
	}
	else
	{
		estimateThicknessOfMovingSedimentLayer.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
		realisationType = CombinerVariables::stringToTypeOfEstimationMethodForThicknessOfMovingSedimentLayer( estimateThicknessOfMovingSedimentLayer.realisationType );
	}

	switch (realisationType)
	{
	case CombinerVariables::MultipleDiameterOfCoarsestGrainMoved:
		addDoubleToConstructionVariables(estimateThicknessOfMovingSedimentLayer,estimateThicknessOfMovingSedimentLayerNode,"minimumTransportRateForFractionToBeConsideredMoving",0.0001);
		addDoubleToConstructionVariables(estimateThicknessOfMovingSedimentLayer,estimateThicknessOfMovingSedimentLayerNode,"factor",1.75);
		break;

	case CombinerVariables::MultipleReferenceGrainDiameter:
		addDoubleToConstructionVariables(estimateThicknessOfMovingSedimentLayer,estimateThicknessOfMovingSedimentLayerNode,"factor",1.25);
		addDoubleToConstructionVariables(estimateThicknessOfMovingSedimentLayer,estimateThicknessOfMovingSedimentLayerNode,"referenceDiameterPercentile",84.0);
		addDoubleToConstructionVariables(estimateThicknessOfMovingSedimentLayer,estimateThicknessOfMovingSedimentLayerNode,"defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream",0.7);
		break;

	case CombinerVariables::ConstantThicknessOfMovingSedimentLayer:
		addDoubleToConstructionVariables(estimateThicknessOfMovingSedimentLayer,estimateThicknessOfMovingSedimentLayerNode,"constantThickness",0.7);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of EstimationMethodForThicknessOfMovingSedimentLayer for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(estimateThicknessOfMovingSedimentLayer);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

bool StandardInput::addCalcActiveWidthToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, CombinerVariables::TypesOfActiveWidthCalculationMethod defaultValue)
{
	bool result = true;
	std::string tmpString;

	CombinerVariables::TypesOfActiveWidthCalculationMethod realisationType;

	pugi::xml_node calcActiveWidthNode = rootNode.child(name.c_str());
	if ( !calcActiveWidthNode ) { realisationType = defaultValue; result = false; }
	else
	{
		pugi::xml_node realisationTypeNode = calcActiveWidthNode.child("realisationType");
		if ( !realisationTypeNode )
		{
			const char *const realisationTypeErrorMessage = "The node realisationType is needed for the ActiveWidthCalculationMethod in StandardInput.";
			throw(realisationTypeErrorMessage);
		}
		tmpString = StringTools::trimStringCopy(realisationTypeNode.child_value());
		realisationType = CombinerVariables::stringToTypeOfActiveWidthCalculationMethod( tmpString );
		tmpString.clear();
	}


	ConstructionVariables calcActiveWidth;
	calcActiveWidth.interfaceOrCombinerType = CombinerVariables::CalcActiveWidth;
	calcActiveWidth.realisationType = CombinerVariables::typeOfActiveWidthCalculationMethodToString(realisationType);

	switch (realisationType)
	{
	case CombinerVariables::SetActiveWidthEqualFlowWidth:
		// In this case there is nothing to do.
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of ActiveWidthCalculationMethod for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	tmpConstructionVariablesVector.push_back(calcActiveWidth);
	target.labelledObjects[name] = tmpConstructionVariablesVector;

	return result;
}

ConstructionVariables StandardInput::createInfinitelyDeepRectangularRiverReachProperties(int cellID, int downstreamCellID, double elevation, double channelWidth, double length, double bedrockRoughnessEquivalentRepresentativeGrainDiameter, bool sillOccurence, double sillTopEdgeElevation, double initialDischarge, double alluviumThickness, bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate, ConstructionVariables armourGrains, ConstructionVariables strataGrains, ConstructionVariables strataSorting, ConstructionVariables sillProperties, bool useInitialGrainSizesForConstantLayerThickness, const std::vector<double>& fractionalGrainDiameters, int numberOfLayers)
{
	HighestOrderStructuresPointers tmpHighestOrderStructuresPointers;
	std::vector<ConstructionVariables> emptyConstructionVariablesVector;
	std::vector<ConstructionVariables> tmpConstructionVariablesVector;
	std::vector<double> tmpDoubleVector;
	std::vector<int> tmpIntVector;
	std::vector<bool> tmpBoolVector;
	std::map< std::string, std::vector<double> >::const_iterator tempDoubleIterator;
	std::map< std::string, std::vector<bool> >::const_iterator tempBoolIterator;

	ConstructionVariables riverReachProperties;
	riverReachProperties.interfaceOrCombinerType = CombinerVariables::RiverReachProperties;

	tmpIntVector.clear();
	tmpIntVector.push_back( cellID );
	riverReachProperties.labelledInts["cellID"] = tmpIntVector;

	tmpIntVector.clear();
	tmpIntVector.push_back( downstreamCellID );
	riverReachProperties.labelledInts["downstreamCellID"] = tmpIntVector;

	tmpBoolVector.clear();
	tmpBoolVector.push_back(false);
	riverReachProperties.labelledBools["upstreamMarginCell"] = tmpBoolVector;
	riverReachProperties.labelledBools["downstreamMarginCell"] = tmpBoolVector;

	////////////////////////////////////// Create additionalRiverReachProperties ///////////////////////////////////////////////////
	ConstructionVariables additionalRiverReachProperties;
	additionalRiverReachProperties.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachProperties;

	ConstructionVariables bedrockRoughnessEquivalentRepresentativeGrainDiameterConstructionVariables;
	bedrockRoughnessEquivalentRepresentativeGrainDiameterConstructionVariables.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachPropertyType;
	bedrockRoughnessEquivalentRepresentativeGrainDiameterConstructionVariables.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter);
	tmpDoubleVector.clear();
	tmpDoubleVector.push_back(bedrockRoughnessEquivalentRepresentativeGrainDiameter);
	bedrockRoughnessEquivalentRepresentativeGrainDiameterConstructionVariables.labelledDoubles["equivalentGrainDiameter"] = tmpDoubleVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back(bedrockRoughnessEquivalentRepresentativeGrainDiameterConstructionVariables);
	additionalRiverReachProperties.labelledObjects["constitutingAdditionalRiverReachPropertyTypes"] = tmpConstructionVariablesVector;

	////////////////////////////////////// Create regularRiverReachProperties ///////////////////////////////////////////////////
	ConstructionVariables regularRiverReachProperties;
	regularRiverReachProperties.interfaceOrCombinerType = CombinerVariables::RegularRiverReachProperties;

			////////////////////////////////////// Adjust regularRiverReachProperties Doubles ///////////////////////////////////////////////////

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( elevation );
			regularRiverReachProperties.labelledDoubles["elevation"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( length );
			regularRiverReachProperties.labelledDoubles["length"] = tmpDoubleVector;

			tmpBoolVector.clear();
			tmpBoolVector.push_back( sillOccurence );
			regularRiverReachProperties.labelledBools["sillOccurence"] = tmpBoolVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( sillTopEdgeElevation );
			regularRiverReachProperties.labelledDoubles["sillTopEdgeElevation"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( initialDischarge );
			regularRiverReachProperties.labelledDoubles["discharge"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 4.0 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["flowVelocity"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 4.0 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["maximumWaterdepth"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 4.0 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["bedShearStress"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( channelWidth );
			regularRiverReachProperties.labelledDoubles["activeWidth"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 0.4 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["bedslope"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 0.4 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["waterVolumeChangeRate"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 0.4 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["waterVolumeChange"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 0.4 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["waterEnergyslope"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 0.4 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["sedimentEnergyslope"] = tmpDoubleVector;

			tmpDoubleVector.clear();
			tmpDoubleVector.push_back( 0.4 ); //Is recalculated afterwards
			regularRiverReachProperties.labelledDoubles["unreducedSedimentEnergyslope"] = tmpDoubleVector;

			////////////////////////////////////// Adjust regularRiverReachProperties Grains ///////////////////////////////////////////////////

			Grains* tmpArmourGrainsPointer = static_cast<Grains*>( SedFlowBuilders::generalBuilder(armourGrains,tmpHighestOrderStructuresPointers) );
			Grains armourLayerGrains = Grains(*tmpArmourGrainsPointer);
			delete tmpArmourGrainsPointer;

			Grains* tmpStrataGrainsPointer = static_cast<Grains*>( SedFlowBuilders::generalBuilder(strataGrains,tmpHighestOrderStructuresPointers) );
			Grains strataLayerGrains = Grains(*tmpStrataGrainsPointer);
			Grains baseLayerGrains = Grains(*tmpStrataGrainsPointer);
			delete tmpStrataGrainsPointer;

			double activeLayerThickness;
			double baseLayerThickness;
			int numberOfFilledLayers;

			double layerThickness;
			double layerThicknessFactor;
			double referenceGrainSizePercentile;
			double referenceGrainSize;

			double minimumLayerThicknessPerUnitLayerThickness;
			double maximumLayerThicknessPerUnitLayerThickness;

			bool dynamicLayerThickness;

			switch (CombinerVariables::stringToTypeOfStrataSorting(strataSorting.realisationType))
			{
			case CombinerVariables::StratigraphyWithThresholdBasedUpdate:
				{//This block bracketing is necessary to allow for declarations within the switch case.

				HighestOrderStructuresPointers emptyHighestOrderStructurePointers;
				std::map<  std::string, std::vector<void*> > emptyLabelledObjectsMap;
				StratigraphyWithThresholdBasedUpdate* stratigraphyWithThresholdBasedUpdate = dynamic_cast<StratigraphyWithThresholdBasedUpdate*>(SedFlowInterfaceRealisationBuilders::strataSortingBuilder(strataSorting,emptyHighestOrderStructurePointers,emptyLabelledObjectsMap));


				tempBoolIterator = strataSorting.labelledBools.find("dynamicThresholds");
				if(tempBoolIterator == strataSorting.labelledBools.end() )
				{
					const char *const dynamicThresholdsErrorMessage = "The variable dynamicThresholds is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate within StandardInput.";
					throw(dynamicThresholdsErrorMessage);
				}
				else { dynamicLayerThickness = ((*tempBoolIterator).second).at(0); }

				std::pair<double,double> thresholdsForActiveLayerThickness = stratigraphyWithThresholdBasedUpdate->getThresholdsForActiveLayerThickness(( useInitialGrainSizesForConstantLayerThickness || dynamicLayerThickness ), armourLayerGrains, fractionalGrainDiameters);
				delete stratigraphyWithThresholdBasedUpdate;

				double minActiveThickness = thresholdsForActiveLayerThickness.first;
				double maxActiveThickness = thresholdsForActiveLayerThickness.second;
				double meanActiveThickness = 0.5 * (minActiveThickness + maxActiveThickness);

				double incrementLayerThickness;
				tempDoubleIterator = strataSorting.labelledDoubles.find("incrementLayerThickness");
				if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
				{
					const char *const incrementLayerThicknessErrorMessage = "The variable incrementLayerThickness is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate within StandardInput.";
					throw(incrementLayerThicknessErrorMessage);
				}
				else { incrementLayerThickness = ((*tempDoubleIterator).second).at(0); }

				layerThickness = incrementLayerThickness;

				if( alluviumThickness >= ( meanActiveThickness + ( (numberOfLayers - 2.0) * incrementLayerThickness ) ) )
				{
					activeLayerThickness = meanActiveThickness;
					baseLayerThickness = alluviumThickness - ( meanActiveThickness + ( (numberOfLayers - 2.0) * incrementLayerThickness ) );
					numberOfFilledLayers = numberOfLayers;
				}
				else
				{
					baseLayerThickness = 0.0;
					if( alluviumThickness >= (meanActiveThickness + incrementLayerThickness) )
					{
						double tmpStrataThickness = alluviumThickness - meanActiveThickness;
						int numberOfCompletelyFilledLayers = tmpStrataThickness / incrementLayerThickness;
						double surplus = tmpStrataThickness - (numberOfCompletelyFilledLayers * incrementLayerThickness);
						if( surplus < (0.5 * incrementLayerThickness) )
						{
							activeLayerThickness = meanActiveThickness + surplus;
							numberOfFilledLayers = numberOfCompletelyFilledLayers + 1;
						}
						else
						{
							activeLayerThickness = meanActiveThickness + surplus - incrementLayerThickness;
							numberOfFilledLayers = numberOfCompletelyFilledLayers + 2;
						}
					}
					else
					{
						if( alluviumThickness < (minActiveThickness + incrementLayerThickness) )
						{
							activeLayerThickness = alluviumThickness;
							numberOfFilledLayers = 1;
						}
						else
						{
							activeLayerThickness = alluviumThickness - incrementLayerThickness;
							numberOfFilledLayers = 2;
						}
					}
				}

				}
				break;
			case CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate:

				if( useInitialGrainSizesForConstantLayerThickness )
				{

					tempDoubleIterator = strataSorting.labelledDoubles.find("layerThicknessFactor");
					if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
					{
						const char *const layerThicknessFactorErrorMessage = "The variable layerThicknessFactor is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate within StandardInput.";
						throw(layerThicknessFactorErrorMessage);
					}
					else { layerThicknessFactor = ((*tempDoubleIterator).second).at(0); }

					tempDoubleIterator = strataSorting.labelledDoubles.find("referenceGrainSizePercentile");
					if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
					{
						const char *const referenceGrainSizePercentileErrorMessage = "The variable referenceGrainSizePercentile is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate within StandardInput.";
						throw(referenceGrainSizePercentileErrorMessage);
					}
					else { referenceGrainSizePercentile = ((*tempDoubleIterator).second).at(0); }

					referenceGrainSize = armourLayerGrains.getPercentileGrainDiameter(fractionalGrainDiameters,referenceGrainSizePercentile);
					layerThickness = layerThicknessFactor * referenceGrainSize;
					tmpDoubleVector.clear();
					tmpDoubleVector.push_back(layerThickness);
					strataSorting.labelledDoubles["layerThickness"] = tmpDoubleVector;
				}
				else
				{
					tempDoubleIterator = strataSorting.labelledDoubles.find("layerThickness");
					if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
					{
						const char *const layerThicknessErrorMessage = "The variable layerThickness is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate within StandardInput.";
						throw(layerThicknessErrorMessage);
					}
					else { layerThickness = ((*tempDoubleIterator).second).at(0); }
				}

				tempDoubleIterator = strataSorting.labelledDoubles.find("minimumLayerThicknessPerUnitLayerThickness");
				if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
				{
					const char *const minimumLayerThicknessPerUnitLayerThicknessErrorMessage = "The variable minimumLayerThicknessPerUnitLayerThickness is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate within StandardInput.";
					throw(minimumLayerThicknessPerUnitLayerThicknessErrorMessage);
				}
				else { minimumLayerThicknessPerUnitLayerThickness = ((*tempDoubleIterator).second).at(0); }

				tempDoubleIterator = strataSorting.labelledDoubles.find("maximumLayerThicknessPerUnitLayerThickness");
				if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
				{
					const char *const maximumLayerThicknessPerUnitLayerThicknessErrorMessage = "The variable maximumLayerThicknessPerUnitLayerThickness is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate within StandardInput.";
					throw(maximumLayerThicknessPerUnitLayerThicknessErrorMessage);
				}
				else { maximumLayerThicknessPerUnitLayerThickness = ((*tempDoubleIterator).second).at(0); }

				if ( alluviumThickness >= ( (numberOfLayers-1) * layerThickness ) )
				{
					activeLayerThickness = layerThickness;
					baseLayerThickness = alluviumThickness - ( (numberOfLayers-1) * layerThickness );
					numberOfFilledLayers = numberOfLayers;
				}
				else
				{
					baseLayerThickness = 0.0;
					int numberOfCompletelyFilledLayers = alluviumThickness / layerThickness;
					double relativeSurplusPerUnitLayerThickness = (alluviumThickness - (numberOfCompletelyFilledLayers*layerThickness)) / layerThickness;
					if( relativeSurplusPerUnitLayerThickness < minimumLayerThicknessPerUnitLayerThickness )
					{
						numberOfFilledLayers = numberOfCompletelyFilledLayers;
						activeLayerThickness = ( 1.0 + relativeSurplusPerUnitLayerThickness ) * layerThickness;
					}
					else
					{
						if( relativeSurplusPerUnitLayerThickness > (maximumLayerThicknessPerUnitLayerThickness - 1.0) )
						{
							numberOfFilledLayers = numberOfCompletelyFilledLayers + 1;
							activeLayerThickness = relativeSurplusPerUnitLayerThickness * layerThickness;
						}
						else
						{
							//The in between case
							if( relativeSurplusPerUnitLayerThickness <= ( (minimumLayerThicknessPerUnitLayerThickness + maximumLayerThicknessPerUnitLayerThickness - 1.0) / 2.0 ) )
							{
								numberOfFilledLayers = numberOfCompletelyFilledLayers;
								activeLayerThickness = ( 1.0 + relativeSurplusPerUnitLayerThickness ) * layerThickness;
							}
							else
							{
								numberOfFilledLayers = numberOfCompletelyFilledLayers + 1;
								activeLayerThickness = relativeSurplusPerUnitLayerThickness * layerThickness;
							}
						}
					}
				}
				break;

			case CombinerVariables::TwoLayerWithShearStressBasedUpdate:
				//The missing break is on purpose.

			case CombinerVariables::TwoLayerWithContinuousUpdate:

				tempBoolIterator = strataSorting.labelledBools.find("dynamicLayerThickness");
				if(tempBoolIterator == strataSorting.labelledBools.end() )
				{
					const char *const dynamicLayerThicknessErrorMessage = "The variable dynamicLayerThickness is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate within StandardInput.";
					throw(dynamicLayerThicknessErrorMessage);
				}
				else { dynamicLayerThickness = ((*tempBoolIterator).second).at(0); }

				if( useInitialGrainSizesForConstantLayerThickness || dynamicLayerThickness )
				{

					tempDoubleIterator = strataSorting.labelledDoubles.find("layerThicknessFactor");
					if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
					{
						const char *const layerThicknessFactorErrorMessage = "The variable layerThicknessFactor is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate within StandardInput.";
						throw(layerThicknessFactorErrorMessage);
					}
					else { layerThicknessFactor = ((*tempDoubleIterator).second).at(0); }

					tempDoubleIterator = strataSorting.labelledDoubles.find("referenceGrainSizePercentile");
					if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
					{
						const char *const referenceGrainSizePercentileErrorMessage = "The variable referenceGrainSizePercentile is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate within StandardInput.";
						throw(referenceGrainSizePercentileErrorMessage);
					}
					else { referenceGrainSizePercentile = ((*tempDoubleIterator).second).at(0); }

					referenceGrainSize = armourLayerGrains.getPercentileGrainDiameter(fractionalGrainDiameters,referenceGrainSizePercentile);
					layerThickness = layerThicknessFactor * referenceGrainSize;
					tmpDoubleVector.clear();
					tmpDoubleVector.push_back(layerThickness);
					strataSorting.labelledDoubles["layerThickness"] = tmpDoubleVector;
				}
				else
				{
					tempDoubleIterator = strataSorting.labelledDoubles.find("layerThickness");
					if(tempDoubleIterator == strataSorting.labelledDoubles.end() )
					{
						const char *const layerThicknessErrorMessage = "The variable layerThickness is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate within StandardInput.";
						throw(layerThicknessErrorMessage);
					}
					else { layerThickness = ((*tempDoubleIterator).second).at(0); }
				}

				numberOfLayers = 2;
				if( alluviumThickness > layerThickness)
				{
					activeLayerThickness = layerThickness;
					baseLayerThickness = alluviumThickness - layerThickness;
					numberOfFilledLayers = 2;
				}
				else
				{
					activeLayerThickness = alluviumThickness;
					baseLayerThickness = 0.0;
					numberOfFilledLayers = 1;
				}
				break;

			case CombinerVariables::SingleLayerNoSorting:
				numberOfLayers = 1;
				armourLayerGrains = baseLayerGrains;
				layerThickness = alluviumThickness;
				activeLayerThickness = alluviumThickness;
				baseLayerThickness = alluviumThickness;
				numberOfFilledLayers = 1;
				break;

			default:
				const char *const invalidTypeErrorMessage = "Invalid Type of Strata Sorting for StandardInput.";
				throw (invalidTypeErrorMessage);
			}

			armourLayerGrains *= (activeLayerThickness / armourLayerGrains.getOverallVolume());
			strataLayerGrains *= (layerThickness / strataLayerGrains.getOverallVolume());
			baseLayerGrains *= (baseLayerThickness / baseLayerGrains.getOverallVolume());

			Grains updaterGrains = Grains(armourLayerGrains);
			updaterGrains *= 0.4;

			ConstructionVariables finalArmourGrains = armourLayerGrains.createConstructionVariables();
			ConstructionVariables finalStrataGrains = strataLayerGrains.createConstructionVariables();
			ConstructionVariables finalBaseGrains = baseLayerGrains.createConstructionVariables();
			ConstructionVariables finalUpdaterGrains = updaterGrains.createConstructionVariables();
			std::vector<ConstructionVariables> finalUpdaterGrainsAsVector;
			finalUpdaterGrainsAsVector.push_back( finalUpdaterGrains );

			tmpConstructionVariablesVector.clear();
			for(int i = 1; i <= numberOfLayers; ++i)
			{
				if( i <= numberOfFilledLayers )
				{
					if( i == 1 )
					{
						tmpConstructionVariablesVector.push_back( finalArmourGrains );
					}
					else
					{
						if( i != numberOfLayers )
						{
							tmpConstructionVariablesVector.push_back( finalStrataGrains );
						}
						else
						{
							tmpConstructionVariablesVector.push_back( finalBaseGrains );
						}
					}
				}
				else
				{
					tmpConstructionVariablesVector.push_back( finalBaseGrains );
				}
			}
			regularRiverReachProperties.labelledObjects["strataPerUnitBedSurface"] = tmpConstructionVariablesVector;
			regularRiverReachProperties.labelledObjects["erosionRate"] = finalUpdaterGrainsAsVector;
			regularRiverReachProperties.labelledObjects["depositionRate"] = finalUpdaterGrainsAsVector;
			regularRiverReachProperties.labelledObjects["erosion"] = finalUpdaterGrainsAsVector;
			regularRiverReachProperties.labelledObjects["deposition"] = finalUpdaterGrainsAsVector;
			regularRiverReachProperties.labelledObjects["erosionPerUnitBedSurface"] = finalUpdaterGrainsAsVector;
			regularRiverReachProperties.labelledObjects["depositionPerUnitBedSurface"] = finalUpdaterGrainsAsVector;

			////////////////////////////////////// Create geometricalChannelBehaviour ///////////////////////////////////////////////////
			ConstructionVariables geometricalChannelBehaviour;
			geometricalChannelBehaviour.interfaceOrCombinerType = CombinerVariables::GeometricalChannelBehaviour;
			geometricalChannelBehaviour.realisationType = CombinerVariables::typeOfGeometricalChannelBehaviourToString( CombinerVariables::ParallelShiftOfBasicGeometry );

			tmpBoolVector.clear();
			tmpBoolVector.push_back(upstreamOfSillsWedgeShapedInsteadOfParallelUpdate);
			geometricalChannelBehaviour.labelledBools["upstreamOfSillsWedgeShapedInsteadOfParallelUpdate"] = tmpBoolVector;

			ConstructionVariables alluviumChannel;
			alluviumChannel.interfaceOrCombinerType = CombinerVariables::ChannelGeometry;
			alluviumChannel.realisationType = CombinerVariables::typeOfChannelGeometryToString( CombinerVariables::InfinitelyDeepRectangularChannel );
			tmpDoubleVector.clear();
			tmpDoubleVector.push_back(channelWidth);
			alluviumChannel.labelledDoubles["channelWidth"] = tmpDoubleVector;

			tmpConstructionVariablesVector.clear();
			tmpConstructionVariablesVector.push_back( alluviumChannel );
			geometricalChannelBehaviour.labelledObjects["alluviumChannel"] = tmpConstructionVariablesVector;

	////////////////////////////////////// Compose riverReachProperties ///////////////////////////////////////////////////

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back( additionalRiverReachProperties );
	riverReachProperties.labelledObjects["additionalRiverReachProperties"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back( regularRiverReachProperties );
	riverReachProperties.labelledObjects["regularRiverReachProperties"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back( strataSorting );
	riverReachProperties.labelledObjects["strataSorting"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back( geometricalChannelBehaviour );
	riverReachProperties.labelledObjects["geometricalChannelBehaviour"] = tmpConstructionVariablesVector;

	tmpConstructionVariablesVector.clear();
	tmpConstructionVariablesVector.push_back( sillProperties );
	riverReachProperties.labelledObjects["sillProperties"] = tmpConstructionVariablesVector;

	return riverReachProperties;
}

std::pair<std::vector<double>,ConstructionVariables> StandardInput::createFractionalGrainDiametersAndGrainsConstructionVariablesFromSpreadsheet(const std::string& fileName, bool inputUpperBoundaryInsteadOfMeanGrainDiameter, bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters, double lowerDiameterBoundaryForFinestFractionInCM, const std::vector<CombinerVariables::TypesOfGrains>& expectedTypesOfGrains)
{
	std::map<std::string,std::vector<std::string> > currentSpreadsheetReadOut;
	std::map<std::string,std::vector<std::string> >::const_iterator iteratorForSpreadsheetReadOut;

	currentSpreadsheetReadOut.clear();
	currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(fileName);
	iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("GrainDiameterInCM");
	if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
	{
		const char *const grainDiameterInCMErrorMessage = "The column GrainDiameterInCM is needed for the grain size distribution spreadsheets within StandardInput input reader.";
		throw(grainDiameterInCMErrorMessage);
	}
	std::vector<double> fractionalGrainDiameters = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

	//Convert fractionalGrainDiameters from cm to m
	std::transform(fractionalGrainDiameters.begin(),fractionalGrainDiameters.end(),fractionalGrainDiameters.begin(),std::bind1st(std::multiplies<double>(),0.01));

	if(inputUpperBoundaryInsteadOfMeanGrainDiameter)
	{
		if(useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters)
		{
			for(std::vector<double>::reverse_iterator currentValue = fractionalGrainDiameters.rbegin(), nextValue = (currentValue+1); nextValue < fractionalGrainDiameters.rend(); ++currentValue,++nextValue)
				{ *currentValue = (*currentValue + *nextValue) * 0.5; }
			fractionalGrainDiameters.at(0) = (lowerDiameterBoundaryForFinestFractionInCM + fractionalGrainDiameters.at(0)) * 0.5;
		}
		else
		{
			for(std::vector<double>::reverse_iterator currentValue = fractionalGrainDiameters.rbegin(), nextValue = (currentValue+1); nextValue < fractionalGrainDiameters.rend(); ++currentValue,++nextValue)
				{ *currentValue = sqrt((*currentValue * *nextValue)); }
			fractionalGrainDiameters.at(0) = sqrt((lowerDiameterBoundaryForFinestFractionInCM * fractionalGrainDiameters.at(0)));
		}
	}


	ConstructionVariables grains;
	grains.interfaceOrCombinerType = CombinerVariables::Grains;
	std::vector<ConstructionVariables> constitutingGrainTypes;
	std::string currentTypeOfGrainsAsString;
	for(std::vector<CombinerVariables::TypesOfGrains>::const_iterator currentTypeOfGrains = expectedTypesOfGrains.begin(); currentTypeOfGrains < expectedTypesOfGrains.end(); ++currentTypeOfGrains)
	{
		const char *const fishEggsErrorMessage = "FishEggs cannot be used in StandardInput as the burial depth is not provided.";
		switch (*currentTypeOfGrains)
		{
		case CombinerVariables::NormalGrains:
			// In this case there is nothing to do.
			break;

		case CombinerVariables::TracerGrains:
			// In this case there is nothing to do.
			break;

		case CombinerVariables::FishEggs:
			throw (fishEggsErrorMessage);
			break;

		default:
			const char *const invalidTypeErrorMessage = "Invalid Type of Grains for StandardInput.";
			throw (invalidTypeErrorMessage);
		}

		currentTypeOfGrainsAsString.clear();
		currentTypeOfGrainsAsString = CombinerVariables::typeOfGrainsToString(*currentTypeOfGrains);
		ConstructionVariables currentGrainType;
		currentGrainType.interfaceOrCombinerType = CombinerVariables::GrainType;
		currentGrainType.realisationType = currentTypeOfGrainsAsString;
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find(currentTypeOfGrainsAsString);

		if( ( *currentTypeOfGrains == CombinerVariables::NormalGrains ) && ( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() ) )
		{
			iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("RelativeAbundance");
		}

		if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
		{
			const char *const sameGrainTypesErrorMessage = "Treated types of Grains need to be the same in all corresponding spreadsheets.";
			throw(sameGrainTypesErrorMessage);
		}
		currentGrainType.labelledDoubles["fractionalAbundance"] = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

		constitutingGrainTypes.push_back( currentGrainType );
	}
	grains.labelledObjects["constitutingGrainTypes"] = constitutingGrainTypes;

	std::pair<std::vector<double>,ConstructionVariables> result = std::make_pair(fractionalGrainDiameters,grains);
	return result;


}

ConstructionVariables StandardInput::createSingleRegularOutputMethod(pugi::xml_node rootNode, const std::string& outputPath, const std::string& defaultFileName, const RiverSystemInformation& riverSystemInformation, const StandardOutputCharacteristics& standardOutputCharacteristics, bool useDefaultPropertyForOutput, const std::string& defaultPropertyForOutput)
{
	ConstructionVariables result;
	result.interfaceOrCombinerType = CombinerVariables::OutputMethodType;

	std::istringstream iStringStream;
	std::vector<std::string> stringVectorForLabelledStrings;
	std::vector<int> intVectorForLabelledInts;
	std::vector<double> doubleVectorForLabelledDoubles;
	std::vector<bool> boolVectorForLabelledBools;
	std::string	tmpString;

	bool forVisualInterpretation = standardOutputCharacteristics.forVisualInterpretation;

	pugi::xml_node forVisualInterpretationNode = rootNode.child("forVisualInterpretation");
	if ( forVisualInterpretationNode )
	{
		tmpString.clear();
		tmpString = StringTools::trimStringCopy(forVisualInterpretationNode.child_value());
		forVisualInterpretation = StringTools::stringToBool(tmpString);
	}

	if(forVisualInterpretation)
	{ result.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputRegularRiverReachPropertiesForVisualInterpretation); }
	else
	{ result.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputRegularRiverReachProperties); }

	stringVectorForLabelledStrings.push_back(outputPath);
	result.labelledStrings["path"] = stringVectorForLabelledStrings;
	stringVectorForLabelledStrings.clear();

	if( !(addStringVectorToConstructionVariables(result,rootNode,"regularRiverReachPropertiesForOutput")) )
	{
		if(useDefaultPropertyForOutput)
		{
			stringVectorForLabelledStrings.push_back(defaultPropertyForOutput);
			result.labelledStrings["regularRiverReachPropertiesForOutput"] = stringVectorForLabelledStrings;
			stringVectorForLabelledStrings.clear();
		}
		else
		{
			const char *const regularRiverReachPropertiesForOutputErrorMessage = "The node regularRiverReachPropertiesForOutput is needed for the outputMethods in StandardInput.";
			throw(regularRiverReachPropertiesForOutputErrorMessage);
		}
	}

	stringVectorForLabelledStrings.push_back(defaultFileName);
	result.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
	stringVectorForLabelledStrings.clear();

	std::vector<std::string> regularRiverReachPropertiesForOutput = (result.labelledStrings.find("regularRiverReachPropertiesForOutput"))->second;
	if(regularRiverReachPropertiesForOutput.size() == 1)
	{
		tmpString.clear();
		tmpString = regularRiverReachPropertiesForOutput.at(0);
		tmpString.append(".txt");
		stringVectorForLabelledStrings.push_back(tmpString);
		result.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
		stringVectorForLabelledStrings.clear();
	}

	pugi::xml_node alternativeFileNameNode = rootNode.child("name");
	if ( alternativeFileNameNode )
	{
		std::string alternativeFileName = StringTools::trimStringCopy(alternativeFileNameNode.child_value());
		alternativeFileName.append(".txt");
		stringVectorForLabelledStrings.push_back(alternativeFileName);
		result.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
		stringVectorForLabelledStrings.clear();
	}

	addDoubleVectorToConstructionVariables(result,rootNode,"explicitTimesForOutput",standardOutputCharacteristics.explicitTimesForOutput);
	addDoubleToConstructionVariables(result,rootNode,"outputInterval",standardOutputCharacteristics.outputInterval);
	addIntToConstructionVariables(result,rootNode,"precisionForOutput",standardOutputCharacteristics.precisionForOutput);

	intVectorForLabelledInts.push_back(4);
	result.labelledInts["numberOfFileIDDigits"] = intVectorForLabelledInts;
	intVectorForLabelledInts.clear();

	result.labelledInts["reachIDsForOutput"] = standardOutputCharacteristics.reachIDsForOutput;
	addReachIDVectorToConstructionVariables(result,rootNode,"reachIDsForOutput",riverSystemInformation);

	intVectorForLabelledInts.push_back(400);
	result.labelledInts["horizontalBarLength"] = intVectorForLabelledInts;
	intVectorForLabelledInts.clear();

	addBoolToConstructionVariables(result,rootNode,"writeLineEachTimeStep",standardOutputCharacteristics.writeLineEachTimeStep);
	addBoolToConstructionVariables(result,rootNode,"outputTimeStepLength",standardOutputCharacteristics.outputTimeStepLength);
	addBoolToConstructionVariables(result,rootNode,"outputInitialValues",standardOutputCharacteristics.outputInitialValues);
	addBoolToConstructionVariables(result,rootNode,"printUpstreamMargins",standardOutputCharacteristics.printUpstreamMargins);
	addBoolToConstructionVariables(result,rootNode,"printDownstreamMargin",standardOutputCharacteristics.printDownstreamMargin);

	pugi::xml_node secondaryOutputIntervalNode = rootNode.child("SecondaryOutputInterval");
	bool useSecondaryOutputInterval = ( standardOutputCharacteristics.useSecondaryOutputInterval || (secondaryOutputIntervalNode != 0) );
	if ( useSecondaryOutputInterval )
	{
		pugi::xml_node referenceCellIDNode = secondaryOutputIntervalNode.child("referenceCellID");
		if ( referenceCellIDNode == 0 )
		{
			if(standardOutputCharacteristics.useSecondaryOutputInterval)
			{
				intVectorForLabelledInts.push_back(standardOutputCharacteristics.referenceCellUserCellID);
				result.labelledInts["referenceCellUserCellID"] = intVectorForLabelledInts;
				intVectorForLabelledInts.clear();
			}
			else
			{
				const char *const referenceCellIDErrorMessage = "The node referenceCellID is needed for the SecondaryOutputInterval of outputMethods in StandardInput.";
				throw(referenceCellIDErrorMessage);
			}
		}
		else
		{
			std::string referenceCellUserCellIDAsString = StringTools::trimStringCopy(referenceCellIDNode.child_value());
			int referenceCellUserCellID;
			iStringStream.clear();
			iStringStream.str( referenceCellUserCellIDAsString );
			iStringStream >> referenceCellUserCellID;
			intVectorForLabelledInts.push_back(referenceCellUserCellID);
			result.labelledInts["referenceCellUserCellID"] = intVectorForLabelledInts;
			intVectorForLabelledInts.clear();
		}

		if( !(addStringToConstructionVariables(result,secondaryOutputIntervalNode,"referenceProperty",standardOutputCharacteristics.referenceProperty) || standardOutputCharacteristics.useSecondaryOutputInterval) )
		{
			const char *const referencePropertyErrorMessage = "The node referenceProperty is needed for the SecondaryOutputInterval of outputMethods in StandardInput.";
			throw(referencePropertyErrorMessage);
		}

		if( !(addDoubleToConstructionVariables(result,secondaryOutputIntervalNode,"thresholdToBeExceeded",standardOutputCharacteristics.thresholdToBeExceeded) || standardOutputCharacteristics.useSecondaryOutputInterval) )
		{
			const char *const thresholdToBeExceededErrorMessage = "The node thresholdToBeExceeded is needed for the SecondaryOutputInterval of outputMethods in StandardInput.";
			throw(thresholdToBeExceededErrorMessage);
		}

		if( !(addDoubleToConstructionVariables(result,secondaryOutputIntervalNode,"secondaryOutputInterval",standardOutputCharacteristics.secondaryOutputInterval) || standardOutputCharacteristics.useSecondaryOutputInterval) )
		{
			const char *const secondaryOutputIntervalErrorMessage = "The node secondaryOutputInterval is needed for the SecondaryOutputInterval of outputMethods in StandardInput.";
			throw(secondaryOutputIntervalErrorMessage);
		}
	}
	boolVectorForLabelledBools.push_back(useSecondaryOutputInterval);
	result.labelledBools["useSecondaryOutputInterval"] = boolVectorForLabelledBools;
	boolVectorForLabelledBools.clear();

	return result;
}

ConstructionVariables StandardInput::createSingleOutputAccumulatedBedloadTransport(pugi::xml_node rootNode, const std::string& outputPath, const std::string& defaultFileName, const RiverSystemInformation& riverSystemInformation, const StandardOutputCharacteristics& standardOutputCharacteristics)
{
	ConstructionVariables outputAccumulatedBedloadTransport;

	std::vector<std::string> stringVectorForLabelledStrings;

	outputAccumulatedBedloadTransport.interfaceOrCombinerType = CombinerVariables::OutputMethodType;
	outputAccumulatedBedloadTransport.realisationType = CombinerVariables::typeOfOutputMethodToString(CombinerVariables::OutputAccumulatedBedloadTransport);

	stringVectorForLabelledStrings.push_back(outputPath);
	outputAccumulatedBedloadTransport.labelledStrings["path"] = stringVectorForLabelledStrings;
	stringVectorForLabelledStrings.clear();

	if(defaultFileName != "")
	{
		stringVectorForLabelledStrings.push_back(defaultFileName);
	}
	pugi::xml_node alternativeFileNameNode = rootNode.child("name");
	if ( alternativeFileNameNode )
	{
		stringVectorForLabelledStrings.clear();
		std::string alternativeFileName = StringTools::trimStringCopy(alternativeFileNameNode.child_value());
		stringVectorForLabelledStrings.push_back(alternativeFileName);
	}

	outputAccumulatedBedloadTransport.labelledStrings["outputFiles"] = stringVectorForLabelledStrings;
	stringVectorForLabelledStrings.clear();

	addBoolToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"writeLineEachTimeStep",standardOutputCharacteristics.writeLineEachTimeStep);

	addDoubleVectorToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"explicitTimesForOutput",standardOutputCharacteristics.explicitTimesForOutput);

	addDoubleToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"outputInterval",standardOutputCharacteristics.outputInterval);
	addIntToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"precisionForOutput",standardOutputCharacteristics.precisionForOutput);

	outputAccumulatedBedloadTransport.labelledInts["reachIDsForOutput"] = standardOutputCharacteristics.reachIDsForOutput;
	addReachIDVectorToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"reachIDsForOutput",riverSystemInformation);

	addBoolToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"outputIncludingPoreVolume",true);
	addBoolToConstructionVariables(outputAccumulatedBedloadTransport,rootNode,"outputDetailedFractional",false);

	return outputAccumulatedBedloadTransport;
}

std::pair<ConstructionVariables,std::pair<bool,int> > StandardInput::createStrataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers (pugi::xml_node riverSystemMethodsNode, bool thicknessInputsIncludingPoreVolume, double poreVolumeFraction)
{
	pugi::xml_node strataSortingNode = riverSystemMethodsNode.child("strataSorting");

	std::map< std::string, std::vector<int> >::const_iterator tempIntIterator;
	std::map< std::string, std::vector<bool> >::const_iterator tempBoolIterator;
	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::string tmpString;
	double tmpDouble;
	std::vector<double> tmpDoubleVector;

	CombinerVariables::TypesOfStrataSorting realisationType;
	ConstructionVariables strataSorting;
	strataSorting.interfaceOrCombinerType = CombinerVariables::StrataSorting;

	pugi::xml_node realisationTypeNode = strataSortingNode.child("realisationType");
	if ( !realisationTypeNode )
	{
		realisationType = CombinerVariables::TwoLayerWithShearStressBasedUpdate;
		strataSorting.realisationType = CombinerVariables::typeOfStrataSortingToString(realisationType);
	}
	else
	{
		strataSorting.realisationType = StringTools::trimStringCopy(realisationTypeNode.child_value());
		realisationType = CombinerVariables::stringToTypeOfStrataSorting( strataSorting.realisationType );
	}

	strataSorting.realisationType = CombinerVariables::typeOfStrataSortingToString(realisationType);

	bool useInitialGrainSizesForConstantLayerThickness = true;
	int numberOfLayers = 1;

	bool dynamicThresholdsOrLayerThickness;
	double incrementLayerThickness, valueOrFactorForLowerThresholdForActiveLayerThickness, valueOrFactorForUpperThresholdForActiveLayerThickness, minimumDifferenceBetweenThresholdsForActiveLayerThickness;
	pugi::xml_node useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithOLDConstantThresholdBasedUpdate;

	switch (realisationType)
	{
	case CombinerVariables::StratigraphyWithThresholdBasedUpdate:
		if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"incrementLayerThickness",0.04)) )
		{
			const char *const incrementLayerThicknessErrorMessage = "The node incrementLayerThickness is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
			throw(incrementLayerThicknessErrorMessage);
		}
		doubleMapIterator = strataSorting.labelledDoubles.find("incrementLayerThickness");
		incrementLayerThickness = ((*doubleMapIterator).second).at(0);

		if( !(addBoolToConstructionVariables(strataSorting,strataSortingNode,"dynamicThresholds",true)) )
		{
			const char *const dynamicThresholdsErrorMessage = "The node dynamicThresholds is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
			throw(dynamicThresholdsErrorMessage);
		}
		tempBoolIterator = strataSorting.labelledBools.find("dynamicThresholds");
		dynamicThresholdsOrLayerThickness = ((*tempBoolIterator).second).at(0);

		if(!dynamicThresholdsOrLayerThickness)
		{
			pugi::xml_node useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithThresholdBasedUpdate = strataSortingNode.child("useInitialGrainSizesForConstantLayerThickness");
			if ( !useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithThresholdBasedUpdate )
			{
				const char *const useInitialGrainSizesForConstantLayerThicknessForStratigraphyWithThresholdBasedUpdateErrorMessage = "The node useInitialGrainSizesForConstantLayerThickness is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
				throw(useInitialGrainSizesForConstantLayerThicknessForStratigraphyWithThresholdBasedUpdateErrorMessage);
			}
			else
			{
				tmpString.clear();
				tmpString = StringTools::trimStringCopy(useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithThresholdBasedUpdate.child_value());
				useInitialGrainSizesForConstantLayerThickness = StringTools::stringToBool(tmpString);
			}
		}

		if(dynamicThresholdsOrLayerThickness || useInitialGrainSizesForConstantLayerThickness)
		{
			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"referenceGrainSizePercentile",40.0)) )
			{
				const char *const referenceGrainSizePercentileForTwoLayerWithContinuousUpdateErrorMessage = "The node referenceGrainSizePercentile is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
				throw(referenceGrainSizePercentileForTwoLayerWithContinuousUpdateErrorMessage);
			}
		}

		if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"valueOrFactorForLowerThresholdForActiveLayerThickness",0.4)) )
		{
			const char *const valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage = "The node valueOrFactorForLowerThresholdForActiveLayerThickness is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
			throw(valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage);
		}
		doubleMapIterator = strataSorting.labelledDoubles.find("valueOrFactorForLowerThresholdForActiveLayerThickness");
		valueOrFactorForLowerThresholdForActiveLayerThickness = ((*doubleMapIterator).second).at(0);

		if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"valueOrFactorForUpperThresholdForActiveLayerThickness",0.4)) )
		{
			const char *const valueOrFactorForUpperThresholdForActiveLayerThicknessErrorMessage = "The node valueOrFactorForUpperThresholdForActiveLayerThickness is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
			throw(valueOrFactorForUpperThresholdForActiveLayerThicknessErrorMessage);
		}
		doubleMapIterator = strataSorting.labelledDoubles.find("valueOrFactorForUpperThresholdForActiveLayerThickness");
		valueOrFactorForUpperThresholdForActiveLayerThickness = ((*doubleMapIterator).second).at(0);

		addDoubleToConstructionVariables(strataSorting,strataSortingNode,"minimumDifferenceBetweenThresholdsForActiveLayerThickness",(1.1 * incrementLayerThickness));
		doubleMapIterator = strataSorting.labelledDoubles.find("minimumDifferenceBetweenThresholdsForActiveLayerThickness");
		minimumDifferenceBetweenThresholdsForActiveLayerThickness = ((*doubleMapIterator).second).at(0);

		addDoubleToConstructionVariables(strataSorting,strataSortingNode,"minimumLowerThresholdForActiveLayerThickness",(minimumDifferenceBetweenThresholdsForActiveLayerThickness * valueOrFactorForLowerThresholdForActiveLayerThickness / (valueOrFactorForUpperThresholdForActiveLayerThickness - valueOrFactorForLowerThresholdForActiveLayerThickness)));

		if( !(addIntToConstructionVariables(strataSorting,strataSortingNode,"numberOfLayers",4)) )
		{
			const char *const numberOfLayersErrorMessage = "The node numberOfLayers is needed for the strataSorting StratigraphyWithThresholdBasedUpdate in StandardInput.";
			throw(numberOfLayersErrorMessage);
		}
		tempIntIterator = strataSorting.labelledInts.find("numberOfLayers");
		numberOfLayers = ((*tempIntIterator).second).at(0);
		break;

	case CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate:
		useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithOLDConstantThresholdBasedUpdate = strataSortingNode.child("useInitialGrainSizesForConstantLayerThickness");
		if ( !useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithOLDConstantThresholdBasedUpdate )
		{
			const char *const useInitialGrainSizesForConstantLayerThicknessForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage = "The node useInitialGrainSizesForConstantLayerThickness is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
			throw(useInitialGrainSizesForConstantLayerThicknessForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage);
		}
		else
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(useInitialGrainSizesForConstantLayerThicknessNodeForStratigraphyWithOLDConstantThresholdBasedUpdate.child_value());
			useInitialGrainSizesForConstantLayerThickness = StringTools::stringToBool(tmpString);
		}

		if(useInitialGrainSizesForConstantLayerThickness)
		{
			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"layerThicknessFactor",0.4)) )
			{
				const char *const layerThicknessFactorForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage = "The node layerThicknessFactor is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
				throw(layerThicknessFactorForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage);
			}

			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"referenceGrainSizePercentile",40.0)) )
			{
				const char *const referenceGrainSizePercentileForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage = "The node referenceGrainSizePercentile is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
				throw(referenceGrainSizePercentileForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage);
			}
		}
		else
		{
			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"layerThickness",0.4)) )
			{
				const char *const layerThicknessForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage = "The node layerThickness is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
				throw(layerThicknessForStratigraphyWithOLDConstantThresholdBasedUpdateErrorMessage);
			}
		}

		if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"minimumLayerThicknessPerUnitLayerThickness",0.4)) )
		{
			const char *const minimumLayerThicknessPerUnitLayerThicknessErrorMessage = "The node minimumLayerThicknessPerUnitLayerThickness is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
			throw(minimumLayerThicknessPerUnitLayerThicknessErrorMessage);
		}

		if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"maximumLayerThicknessPerUnitLayerThickness",1.4)) )
		{
			const char *const maximumLayerThicknessPerUnitLayerThicknessErrorMessage = "The node maximumLayerThicknessPerUnitLayerThickness is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
			throw(maximumLayerThicknessPerUnitLayerThicknessErrorMessage);
		}

		if( !(addIntToConstructionVariables(strataSorting,strataSortingNode,"numberOfLayers",4)) )
		{
			const char *const numberOfLayersErrorMessage = "The node numberOfLayers is needed for the strataSorting StratigraphyWithOLDConstantThresholdBasedUpdate in StandardInput.";
			throw(numberOfLayersErrorMessage);
		}
		tempIntIterator = strataSorting.labelledInts.find("numberOfLayers");
		numberOfLayers = ((*tempIntIterator).second).at(0);
		break;

	case CombinerVariables::TwoLayerWithShearStressBasedUpdate:

		addBoolToConstructionVariables(strataSorting,strataSortingNode,"dynamicBreakUpConditions",false);
		addBoolToConstructionVariables(strataSorting,strataSortingNode,"usePredefinedBreakUpConditions",false);
		{
		std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator = strataSorting.labelledBools.find("usePredefinedBreakUpConditions");
		bool usePredefinedBreakUpConditions = boolMapIterator->second.at(0);
		if(usePredefinedBreakUpConditions)
		{
			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"referenceMedianDiameter",0.04)) )
			{
				const char *const referenceMedianDiameterErrorMessage = "If usePredefinedBreakUpConditions, the node referenceMedianDiameter is needed for the strataSorting TwoLayerWithShearStressBasedUpdate in StandardInput.";
				throw(referenceMedianDiameterErrorMessage);
			}

			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"thetaCriticalForActiveLayer",0.04)) )
			{
				const char *const thetaCriticalForActiveLayerErrorMessage = "If usePredefinedBreakUpConditions, the node thetaCriticalForActiveLayer is needed for the strataSorting TwoLayerWithShearStressBasedUpdate in StandardInput.";
				throw(thetaCriticalForActiveLayerErrorMessage);
			}

			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"thetaCriticalForSublayer",0.044)) )
			{
				const char *const thetaCriticalForSublayerErrorMessage = "If usePredefinedBreakUpConditions, the node thetaCriticalForSublayer is needed for the strataSorting TwoLayerWithShearStressBasedUpdate in StandardInput.";
				throw(thetaCriticalForSublayerErrorMessage);
			}
		}

		}

		if(!(addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(strataSorting,strataSortingNode,"thresholdCalculationMethod")))
		{
			pugi::xml_node bedloadTransportEquationsNode = riverSystemMethodsNode.child("bedloadTransportEquations");
			addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(strataSorting,bedloadTransportEquationsNode,"thresholdCalculationMethod");
		}

		//The missing break is on purpose.

	case CombinerVariables::TwoLayerWithContinuousUpdate:

		addBoolToConstructionVariables(strataSorting,strataSortingNode,"dynamicLayerThickness",false);
		tempBoolIterator = strataSorting.labelledBools.find("dynamicLayerThickness");
		dynamicThresholdsOrLayerThickness = ((*tempBoolIterator).second).at(0);

		if(!dynamicThresholdsOrLayerThickness)
		{
			pugi::xml_node useInitialGrainSizesForConstantLayerThicknessNodeForTwoLayerWithContinuousUpdate = strataSortingNode.child("useInitialGrainSizesForConstantLayerThickness");
			if ( useInitialGrainSizesForConstantLayerThicknessNodeForTwoLayerWithContinuousUpdate )
			{
				tmpString.clear();
				tmpString = StringTools::trimStringCopy(useInitialGrainSizesForConstantLayerThicknessNodeForTwoLayerWithContinuousUpdate.child_value());
				useInitialGrainSizesForConstantLayerThickness = StringTools::stringToBool(tmpString);
			}
		}

		if(dynamicThresholdsOrLayerThickness || useInitialGrainSizesForConstantLayerThickness)
		{
			addDoubleToConstructionVariables(strataSorting,strataSortingNode,"layerThicknessFactor",1.75);
			addDoubleToConstructionVariables(strataSorting,strataSortingNode,"referenceGrainSizePercentile",84.0);
		}
		else
		{
			if( !(addDoubleToConstructionVariables(strataSorting,strataSortingNode,"layerThickness",0.4)) )
			{
				const char *const layerThicknessForTwoLayerWithContinuousUpdateErrorMessage = "The node layerThickness is needed for the strataSorting TwoLayerWithContinuousUpdate or TwoLayerWithShearStressBasedUpdate in StandardInput.";
				throw(layerThicknessForTwoLayerWithContinuousUpdateErrorMessage);
			}
		}
		numberOfLayers = 2;
		break;

	case CombinerVariables::SingleLayerNoSorting:
		addDoubleToConstructionVariables(strataSorting,strataSortingNode,"layerThickness",0.4);
		numberOfLayers = 1;
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Type of StrataSorting for StandardInput.";
		throw (invalidTypeErrorMessage);
	}

	if(thicknessInputsIncludingPoreVolume)
	{
		doubleMapIterator = strataSorting.labelledDoubles.find("layerThickness");
		if(doubleMapIterator != strataSorting.labelledDoubles.end() )
		{
			tmpDouble = doubleMapIterator->second.at(0);
			tmpDouble *= (1 - poreVolumeFraction);
			tmpDoubleVector.clear();
			tmpDoubleVector.push_back(tmpDouble);
			strataSorting.labelledDoubles["layerThickness"] = tmpDoubleVector;
		}

		doubleMapIterator = strataSorting.labelledDoubles.find("layerThicknessFactor");
		if(doubleMapIterator != strataSorting.labelledDoubles.end() )
		{
			tmpDouble = doubleMapIterator->second.at(0);
			tmpDouble *= (1 - poreVolumeFraction);
			tmpDoubleVector.clear();
			tmpDoubleVector.push_back(tmpDouble);
			strataSorting.labelledDoubles["layerThicknessFactor"] = tmpDoubleVector;
		}
	}

	std::pair<bool,int> useInitialGrainSizesForConstantLayerThicknessANDnumberOfLayers = std::make_pair(useInitialGrainSizesForConstantLayerThickness,numberOfLayers);
	std::pair<ConstructionVariables,std::pair<bool,int> > strataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers = std::make_pair(strataSorting,useInitialGrainSizesForConstantLayerThicknessANDnumberOfLayers);

	return strataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers;

}

std::map<int,ConstructionVariables> StandardInput::createMapFromCellIDToInstantaneousSedimentInputsFromSpreadsheet (std::string path, const RiverSystemInformation& riverSystemInformation, bool inputUpperBoundaryInsteadOfMeanGrainDiameter, bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters, double lowerDiameterBoundaryForFinestFractionInCM, double poreVolumeFraction)
{
	std::map<int,ConstructionVariables> result;

	StringTools::ensureForwardSlashesAndMakeStringEndWithOneSlash(path);
	std::string dischargeAndOtherInputsPath = path;
	dischargeAndOtherInputsPath.append("DischargeAndOtherInputs/");

	std::string currentFileName = dischargeAndOtherInputsPath;
	currentFileName.append("InstantaneousSedimentInputs.txt");

	if( StringTools::fileExists(currentFileName) )
	{
		std::string grainSizeDistributionsPath = path;
		grainSizeDistributionsPath.append("GrainSizeDistributions/");

		double conversionFactorFromPorousToSolidVolume =  1.0 - poreVolumeFraction ;

		int currentCellID;
		std::map<std::string,std::vector<std::string> > currentSpreadsheetReadOut;
		std::map<std::string,std::vector<std::string> >::const_iterator iteratorForSpreadsheetReadOut;
		std::vector<bool> tmpBoolVector;
		std::map<std::string,Grains> mapFromGrainSizeDistributionNameToGrainsObject;
		std::map<std::string,Grains>::const_iterator grainSizeDistributionMapIterator;
		std::map<std::pair<int,std::string>,int>::const_iterator cellIDMapIterator;
		std::pair<int,std::string> currentBranchIDAndKilometrage;
		std::vector<int> tmpIntVector;
		std::vector<double> tmpDoubleVector;
		std::vector<ConstructionVariables> tmpConstructionVariablesVector;
		HighestOrderStructuresPointers tmpHighestOrderStructuresPointers;
		Grains* tmpGrainsPointer;

		currentSpreadsheetReadOut.clear();
		currentSpreadsheetReadOut = StringTools::tabDelimitedSpreadsheetFileToStringMap(currentFileName);

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("BranchIDs");
		if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
		{
			const char *const branchIDsErrorMessage = "The column BranchIDs is needed in the InstantaneousSedimentInputs spreadsheet for StandardInput input reader.";
			throw(branchIDsErrorMessage);
		}
		std::vector<int> branchIDs = StringTools::stringVectorToIntVector( iteratorForSpreadsheetReadOut->second );

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("KilometrageUpstreamDirected");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const kilometrageUpstreamDirectedErrorMessage = "The column KilometrageUpstreamDirected is needed for the InstantaneousSedimentInputs spreadsheet within StandardInput input reader.";
			throw(kilometrageUpstreamDirectedErrorMessage);
		}
		std::vector<std::string> kilometrages = iteratorForSpreadsheetReadOut->second;

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("GrainSizeDistribution");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			const char *const grainSizeDistributionErrorMessage = "The column GrainSizeDistribution is needed for the InstantaneousSedimentInputs spreadsheet within StandardInput input reader.";
			throw(grainSizeDistributionErrorMessage);
		}
		std::vector<std::string> grainSizeDistributions = iteratorForSpreadsheetReadOut->second;

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("InputVolumesInM3");
		if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
		{
			const char *const inputVolumesErrorMessage = "The column InputVolumesInM3 is needed in the InstantaneousSedimentInputs spreadsheet for StandardInput input reader.";
			throw(inputVolumesErrorMessage);
		}
		std::vector<double> inputVolumes = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("ElapsedSeconds");
		if( iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end() )
		{
			const char *const elapsedSecondsErrorMessage = "The column ElapsedSeconds is needed in the InstantaneousSedimentInputs spreadsheet for StandardInput input reader.";
			throw(elapsedSecondsErrorMessage);
		}
		std::vector<double> elapsedSeconds = StringTools::stringVectorToDoubleVector( iteratorForSpreadsheetReadOut->second );

		std::vector<bool> inputIncludingPoreVolume;
		iteratorForSpreadsheetReadOut = currentSpreadsheetReadOut.find("InputIncludingPoreVolume");
		if(iteratorForSpreadsheetReadOut == currentSpreadsheetReadOut.end())
		{
			inputIncludingPoreVolume = std::vector<bool>(kilometrages.size(),true);
		}
		else
		{
			inputIncludingPoreVolume = StringTools::stringVectorToBoolVector( iteratorForSpreadsheetReadOut->second );
		}

		std::vector<int>::const_iterator currentBranchID = branchIDs.begin();
		std::vector<bool>::const_iterator currentBoolInputIncludingPoreVolume = inputIncludingPoreVolume.begin();
		std::vector<double>::const_iterator currentInputVolume = inputVolumes.begin();
		std::vector<double>::const_iterator currentElapsedSeconds = elapsedSeconds.begin();

		std::set<int> treatedCellIDs;
		std::multimap<int,std::pair<double,ConstructionVariables> > mapFromCellIDToRawInput;

		for(std::vector<std::string>::const_iterator currentKilometrage = kilometrages.begin(), currentGrainSizeDistribution = grainSizeDistributions.begin(); currentKilometrage < kilometrages.end(); ++currentBranchID, ++currentKilometrage, ++currentGrainSizeDistribution, ++currentInputVolume, ++currentElapsedSeconds, ++currentBoolInputIncludingPoreVolume)
		{
			currentBranchIDAndKilometrage = std::make_pair(*currentBranchID,*currentKilometrage);
			cellIDMapIterator = riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID.find(currentBranchIDAndKilometrage);
			if(cellIDMapIterator == riverSystemInformation.mapFromBranchIDAndKilometrageToUserCellID.end())
			{
				const char *const unknownLocationErrorMessage = "A location (branchID and kilometrage) used in SedimentInputs has not been defined in the longitudinal profile yet.";
				throw(unknownLocationErrorMessage);
			}
			currentCellID = cellIDMapIterator->second;
			grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToGrainsObject.find(*currentGrainSizeDistribution);
			if( grainSizeDistributionMapIterator == mapFromGrainSizeDistributionNameToGrainsObject.end() )
			{
				currentFileName.clear();
				currentFileName = grainSizeDistributionsPath;
				currentFileName.append(*currentGrainSizeDistribution);
				currentFileName.append(".txt");
				std::pair<std::vector<double>,ConstructionVariables> currentFractionalGrainDiametersAndGrainsConstructionVariables = createFractionalGrainDiametersAndGrainsConstructionVariablesFromSpreadsheet(currentFileName,inputUpperBoundaryInsteadOfMeanGrainDiameter,useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters,lowerDiameterBoundaryForFinestFractionInCM,riverSystemInformation.usedTypesOfGrains);
				std::vector<double> currentFractionalGrainDiameters = currentFractionalGrainDiametersAndGrainsConstructionVariables.first;

				if( riverSystemInformation.usedFractionalGrainDiameters.size() != currentFractionalGrainDiameters.size() )
				{
					const char *const grainSizesNumberErrorMessage = "The number of treated grain sizes needs to be the same in all corresponding spreadsheets.";
					throw(grainSizesNumberErrorMessage);
				}
				for(std::vector<double>::const_iterator currentRiverSystemGrainSize = riverSystemInformation.usedFractionalGrainDiameters.begin(), currentGrainSizeOfCurrentGrains = currentFractionalGrainDiameters.begin(); currentGrainSizeOfCurrentGrains < currentFractionalGrainDiameters.end(); ++currentRiverSystemGrainSize, ++currentGrainSizeOfCurrentGrains)
				{
					if ( *currentRiverSystemGrainSize != *currentGrainSizeOfCurrentGrains )
					{
						const char *const sameGrainSizesFractionsErrorMessage = "The treated grain sizes need to be exactly the same in all corresponding spreadsheets.";
						throw(sameGrainSizesFractionsErrorMessage);
					}
				}

				ConstructionVariables grainsConstructionVariables = currentFractionalGrainDiametersAndGrainsConstructionVariables.second;
				tmpGrainsPointer = static_cast<Grains*>(SedFlowBuilders::generalBuilder(grainsConstructionVariables,tmpHighestOrderStructuresPointers));
				//Normalise Grains so that the fractions sum up to 1.
				*tmpGrainsPointer /= tmpGrainsPointer->getOverallVolume();
				mapFromGrainSizeDistributionNameToGrainsObject[*currentGrainSizeDistribution] = Grains(*tmpGrainsPointer);
				delete tmpGrainsPointer;
				grainSizeDistributionMapIterator = mapFromGrainSizeDistributionNameToGrainsObject.find(*currentGrainSizeDistribution);
			}
			Grains currentGrainsBase = grainSizeDistributionMapIterator->second;

			double currentInputVolumeCorrected = *currentInputVolume;
			if( *currentBoolInputIncludingPoreVolume ) { currentInputVolumeCorrected *= conversionFactorFromPorousToSolidVolume; }

			Grains grainsObjectForCurrentGrainsEntry = currentGrainsBase * currentInputVolumeCorrected;
			ConstructionVariables currentGrainsEntry = grainsObjectForCurrentGrainsEntry.createConstructionVariables();

			std::pair<double,ConstructionVariables> currentInputEvent (*currentElapsedSeconds,currentGrainsEntry);
			mapFromCellIDToRawInput.insert(std::pair<int,std::pair<double,ConstructionVariables> >(currentCellID,currentInputEvent));
			treatedCellIDs.insert(currentCellID);
		}

		for(std::set<int>::iterator currentCellIDIterator = treatedCellIDs.begin(); currentCellIDIterator != treatedCellIDs.end(); ++currentCellIDIterator)
		{
			currentCellID = *currentCellIDIterator;

			ConstructionVariables currentResultEntry;
			currentResultEntry.interfaceOrCombinerType = CombinerVariables::AdditionalRiverReachMethodType;
			currentResultEntry.realisationType = CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString(CombinerVariables::InstantaneousSedimentInputs);

			std::vector<double> elapsedSecondsVector;
			std::vector<ConstructionVariables> sedimentInputsVector;

		    std::pair<std::multimap<int,std::pair<double,ConstructionVariables> >::const_iterator, std::multimap<int,std::pair<double,ConstructionVariables> >::const_iterator> entryRangeForCurrentCellID;
		    entryRangeForCurrentCellID = mapFromCellIDToRawInput.equal_range(currentCellID);
		    for (std::multimap<int,std::pair<double,ConstructionVariables> >::const_iterator currentEventEntry = entryRangeForCurrentCellID.first; currentEventEntry != entryRangeForCurrentCellID.second; ++currentEventEntry)
		    {
		    	std::pair<double,ConstructionVariables> currentEvent (currentEventEntry->second);
		    	elapsedSecondsVector.push_back(currentEvent.first);
		    	sedimentInputsVector.push_back(currentEvent.second);
		    }

		    currentResultEntry.labelledDoubles["elapsedSeconds"] = elapsedSecondsVector;
		    currentResultEntry.labelledObjects["sedimentInputs"] = sedimentInputsVector;

			//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
		    result[(currentCellID - 1000)] = currentResultEntry;
		 }
	}
	return result;
}

bool StandardInput::addDoubleToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, double defaultValue)
{
	bool result = false;

	std::istringstream iStringStream;

	double tmpDouble;
	std::vector<double> doubleVectorForLabelledDoubles;

	if ( rootNode == 0 ) { doubleVectorForLabelledDoubles.push_back(defaultValue); }
	else
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if ( !currentNode ) { doubleVectorForLabelledDoubles.push_back(defaultValue); }
		else
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentNode.child_value()) );
			iStringStream >> tmpDouble;
			doubleVectorForLabelledDoubles.push_back( tmpDouble );
			result = true;
		}
	}
	target.labelledDoubles[name] = doubleVectorForLabelledDoubles;

	return result;
}

bool StandardInput::addIntToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, int defaultValue)
{
	bool result = false;

	std::istringstream iStringStream;

	int tmpInt;
	std::vector<int> intVectorForLabelledInts;

	if ( rootNode == 0 ) { intVectorForLabelledInts.push_back(defaultValue); }
	else
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if ( !currentNode ) { intVectorForLabelledInts.push_back(defaultValue); }
		else
		{
			iStringStream.str("");
			iStringStream.clear();
			iStringStream.str( StringTools::trimStringCopy(currentNode.child_value()) );
			iStringStream >> tmpInt;
			intVectorForLabelledInts.push_back( tmpInt );
			result = true;
		}
	}
	target.labelledInts[name] = intVectorForLabelledInts;

	return result;
}

bool StandardInput::addBoolToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, bool defaultValue)
{
	bool result = false;

	std::string tmpString;

	bool tmpBool = false;
	std::vector<bool> boolVectorForLabelledBools;

	if ( rootNode == 0 ) { boolVectorForLabelledBools.push_back(defaultValue); }
	else
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if ( !currentNode ) { boolVectorForLabelledBools.push_back(defaultValue); }
		else
		{
			tmpString.clear();
			tmpString = StringTools::trimStringCopy(currentNode.child_value());
			tmpBool = StringTools::stringToBool(tmpString);
			boolVectorForLabelledBools.push_back( tmpBool );
			result = true;
		}
	}
	target.labelledBools[name] = boolVectorForLabelledBools;

	return result;
}

bool StandardInput::addStringToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, const std::string& defaultValue)
{
	bool result = false;

	std::vector<std::string> stringVectorForLabelledStrings;

	if ( !rootNode ) { stringVectorForLabelledStrings.push_back(defaultValue); }
	else
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if ( !currentNode ) { stringVectorForLabelledStrings.push_back(defaultValue); }
		else
		{
			stringVectorForLabelledStrings.push_back( StringTools::trimStringCopy(currentNode.child_value()) );
			result = true;
		}
	}
	target.labelledStrings[name] = stringVectorForLabelledStrings;

	return result;
}

bool StandardInput::addStringWithInternalBlanksToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, const std::string& defaultValue)
{
	bool result = false;

	std::vector<std::string> stringVectorForLabelledStrings;

	if ( !rootNode ) { stringVectorForLabelledStrings.push_back(defaultValue); }
	else
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if ( !currentNode ) { stringVectorForLabelledStrings.push_back(defaultValue); }
		else
		{
			stringVectorForLabelledStrings.push_back(StringTools::trimStringCopyKeepingInternalBlanks((currentNode.text()).get()));
			result = true;
		}
	}
	target.labelledStrings[name] = stringVectorForLabelledStrings;

	return result;
}

bool StandardInput::addDoubleVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, std::vector<double> defaultValue)
{
	bool result = false;

	std::istringstream iStringStream;

	std::string tmpString;
	double tmpDouble;

	if(rootNode)
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if(currentNode)
		{
			defaultValue.clear();
			currentNode = currentNode.first_child();
			while(currentNode)
			{
				iStringStream.str("");
				iStringStream.clear();
				tmpString.clear();
				tmpString = StringTools::trimStringCopy(currentNode.child_value());
				iStringStream.str( tmpString );
				iStringStream >> tmpDouble;
				defaultValue.push_back( tmpDouble );
				currentNode = currentNode.next_sibling();
			}

			result = true;
		}
	}

	target.labelledDoubles[name] = defaultValue;

	return result;
}

bool StandardInput::addIntVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, std::vector<int> defaultValue)
{
	bool result = false;

	std::istringstream iStringStream;

	std::string tmpString;
	int tmpInt;

	if(rootNode != 0)
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if(currentNode != 0)
		{
			defaultValue.clear();
			currentNode = currentNode.first_child();
			while(currentNode != 0)
			{
				iStringStream.str("");
				iStringStream.clear();
				tmpString.clear();
				tmpString = StringTools::trimStringCopy(currentNode.child_value());
				iStringStream.str( tmpString );
				iStringStream >> tmpInt;
				defaultValue.push_back( tmpInt );
				currentNode = currentNode.next_sibling();
			}

			result = true;
		}
	}

	target.labelledInts[name] = defaultValue;

	return result;
}

bool StandardInput::addReachIDVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, const RiverSystemInformation& riverSystemInformation)
{
	bool result = false;

	std::istringstream iStringStream;

	std::string tmpString;
	int tmpInt;
	std::vector<int> intVectorForLabelledInts;

	std::map< std::string, std::vector<int> >::const_iterator tmpIterator = target.labelledInts.find(name);
	if( tmpIterator == target.labelledInts.end() )
		{ target.labelledInts[name] = intVectorForLabelledInts; }

	std::string reachID ("reachID");
	std::string branchID ("branchID");
	bool isReachID, isBranchID;

	if(rootNode != 0)
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if(currentNode != 0)
		{
			currentNode = currentNode.first_child();
			while(currentNode != 0)
			{
				isReachID = (currentNode.name() == reachID );
				isBranchID = (currentNode.name() == branchID );
				if( isReachID || isBranchID )
				{
					iStringStream.str("");
					iStringStream.clear();
					tmpString.clear();
					tmpString = StringTools::trimStringCopy(currentNode.child_value());
					iStringStream.str( tmpString );
					iStringStream >> tmpInt;
					if(isReachID)
					{
						intVectorForLabelledInts.push_back( tmpInt );
					}
					else //isBranchID
					{
						for(std::vector<RiverBranchForStandardInput>::const_iterator currentBranch = riverSystemInformation.riverBranches.begin(); currentBranch < riverSystemInformation.riverBranches.end(); ++currentBranch)
						{
							if(currentBranch->branchID == tmpInt)
							{
								for(int currentReachID = currentBranch->topmostCellID; currentReachID < (currentBranch->topmostCellID + currentBranch->numberOfCells); ++currentReachID)
								{
									//TODO Remove this bugfix subtraction by 1000, whenever the Problem in the RegularRiverSystemProperties is solved.
									intVectorForLabelledInts.push_back( (currentReachID - 1000) );
								}
							}
						}
					}
				}
				currentNode = currentNode.next_sibling();
			}

			target.labelledInts[name] = intVectorForLabelledInts;
			result = true;
		}
	}

	return result;
}

bool StandardInput::addStringVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name)
{
	bool result = false;

	char* tmpCharPointer;
	std::string tmpString;
	std::vector<std::string> stringVectorForLabelledStrings;

	if(rootNode != 0)
	{
		pugi::xml_node currentNode = rootNode.child(name.c_str());
		if(currentNode != 0)
		{
			currentNode = currentNode.first_child();
			while(currentNode != 0)
			{
				tmpString.clear();
				stringVectorForLabelledStrings.push_back( currentNode.name() );
				currentNode = currentNode.next_sibling();
			}

			result = true;
		}
	}

	target.labelledStrings[name] = stringVectorForLabelledStrings;

	return result;
}

bool StandardInput::checkIfIntVectorContainsValue(const std::vector<int>& vector, int value)
{
	bool result = false;
	for(std::vector<int>::const_iterator currentVectorEntry = vector.begin(); currentVectorEntry < vector.end();++currentVectorEntry)
	{
		if( *currentVectorEntry == value )
		{
			result = true;
			break;
		}
	}
	return result;
}

std::vector<int> StandardInput::extractUserCellIDsFromRiverSystemProperties(const ConstructionVariables& riverSystemProperties)
{
	std::vector<int> result;

	ConstructionVariables regularRiverSystemProperties = riverSystemProperties.labelledObjects.find("regularRiverSystemProperties")->second.at(0);
	std::vector<ConstructionVariables> cellProperties = regularRiverSystemProperties.labelledObjects.find("cellProperties")->second;

	for(std::vector<ConstructionVariables>::iterator currentCell = cellProperties.begin(); currentCell < cellProperties.end(); ++currentCell)
	{
		result.push_back( currentCell->labelledInts.find("cellID")->second.at(0) );
	}

	return result;
}

}
