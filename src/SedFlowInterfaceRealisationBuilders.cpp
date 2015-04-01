/*
 * SedFlowInterfaceRealisationBuilders.cpp
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

#include "SedFlowInterfaceRealisationBuilders.h"

//Types of User Input Reader
#include "VerbatimTranslationFromXMLToConstructionVariables.h"
#include "StandardInput.h"

//Types of Output Method
#include "OutputVerbatimTranslationOfConstructionVariablesToXML.h"
#include "OutputRegularRiverReachProperties.h"
#include "OutputRegularRiverReachPropertiesForVisualInterpretation.h"
#include "OutputAccumulatedBedloadTransport.h"
#include "OutputSimulationSetup.h"

//Types of Additional River System Property
//#include "???.h"

//Types of Additional River Reach Property
#include "BedrockRoughnessEquivalentRepresentativeGrainDiameter.h"
#include "ScourChainProperties.h"

//Types of Grains
#include "NormalGrains.h"
#include "TracerGrains.h"
#include "FishEggs.h"

//Types of Geometrical Channel Behaviour
#include "ParallelShiftOfBasicGeometry.h"

//Types of Channel Geometry
#include "InfinitelyDeepRectangularChannel.h"
#include "InfinitelyDeepVShapedChannel.h"

//Types of Strata Sorting
#include "StratigraphyWithThresholdBasedUpdate.h"
#include "StratigraphyWithOLDConstantThresholdBasedUpdate.h"
#include "TwoLayerWithContinuousUpdate.h"
#include "TwoLayerWithShearStressBasedUpdate.h"
#include "SingleLayerNoSorting.h"

//Types of Sill Properties
#include "PoleniSill.h"

//Types of Gradient Calculation Method
#include "SimpleDownstreamTwoCellGradient.h"
#include "SimpleThreeCellGradient.h"
#include "SimpleDownstreamTwoCellGradientWithCenteredValues.h"
#include "SimpleThreeCellGradientWithCenteredValues.h"
#include "ReducedWaterEnergyslope.h"
#include "ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.h"
#include "ReturnBedslope.h"
#include "ReturnWaterEnergyslope.h"

//Types of flow resistance
#include "VariablePowerLawFlowResistance.h"
#include "FixedPowerLawFlowResistance.h"
#include "DarcyWeisbachFlowResistance.h"
#include "SolveForWaterEnergyslopeBasedOnHydraulicHead.h"

//Types of Additional River System Method Type
//#include "???.h"

//Types of Flow Type Methods
#include "ImplicitKinematicWave.h"
#include "ExplicitKinematicWave.h"
#include "SedimentFlowMethods.h"
#include "UniformDischarge.h"

//Types of Sediment Flow Type Methods
#include "SuspensionLoadFlowMethods.h"
#include "BedloadFlowMethods.h"

//Types of Bedload Capacity Calculation Methods
#include "ChengBedloadCapacity.h"
#include "RickenmannBedloadCapacityBasedOnTheta.h"
#include "RickenmannBedloadCapacityBasedOnThetaNonFractional.h"
#include "RickenmannBedloadCapacityBasedOnq.h"
#include "RickenmannBedloadCapacityBasedOnqNonFractional.h"
#include "WilcockCroweBedloadCapacity.h"
#include "SchneiderEtAlBedloadCapacity.h"
#include "ReckingBedloadCapacityNonFractional.h"

//Types of Change Rate Modifiers
#include "AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.h"
#include "RecirculateWater.h"
#include "RecirculateSediment.h"
#include "InputPropertyTimeSeriesLinearlyInterpolated.h"

//Types of Additional River Reach Method
#include "PreventLocalGrainSizeDistributionChanges.h"
#include "InstantaneousSedimentInputs.h"
#include "SternbergAbrasionWithoutFining.h"
#include "SternbergAbrasionIncludingFining.h"
#include "ScourChainMethods.h"

//Types of Tau Calculation Method
#include "EnergyslopeTauBasedOnFlowDepth.h"
#include "EnergyslopeTau.h"
#include "FlowVelocityTau.h"

//Types of Bedload Velocity Calculation Methods
#include "JulienBounvilayRollingParticlesVelocity.h"
#include "VelocityAsTransportRatePerUnitCrossSectionalArea.h"

//Types of estimation methods thickness of moving sediment layer
#include "MultipleDiameterOfCoarsestGrainMoved.h"
#include "MultipleReferenceGrainDiameter.h"
#include "ConstantThicknessOfMovingSedimentLayer.h"

//Types of Active Width Calculation Methods
#include "SetActiveWidthEqualFlowWidth.h"

//Types of Calculation Method for Bedload Motion Initiation Threshold
#include "LambEtAlCriticalTheta.h"
#include "ConstantThresholdForInitiationOfBedloadMotion.h"
#include "StochasticThresholdForInitiationOfBedloadMotion.h"

//Types of Active Width Calculation Methods
#include "PowerLawHidingFunction.h"
#include "WilcockCroweHidingFunction.h"
#include "NoHiding.h"

namespace SedFlow {

UserInputReader* SedFlowInterfaceRealisationBuilders::userInputReaderBuilder (std::string xmlRootName)
{
	std::string userInputReaderName = CombinerVariables::getInternalStringCorrespondingToExternalString(xmlRootName);

	CombinerVariables::TypesOfUserInputReaders typeOfUserInputReader = CombinerVariables::stringToTypeOfUserInputReaders(userInputReaderName);

	UserInputReader* result;

	switch (typeOfUserInputReader)
	{
	case CombinerVariables::VerbatimTranslationFromXMLToConstructionVariables:
		result = new VerbatimTranslationFromXMLToConstructionVariables();
		break;

	case CombinerVariables::StandardInput:
		result = new StandardInput();
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid User Input Reader Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

OutputMethodType* SedFlowInterfaceRealisationBuilders::outputMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::OutputMethodType)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (outputMethodTypeBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfOutputMethod typeOfOutputMethod = CombinerVariables::stringToTypeOfOutputMethod(constructionVariables.realisationType);

	OutputMethodType* result;

	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;
	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<int> >::const_iterator intMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;

	std::vector<std::string> stringVector;

	std::string path;
	stringMapIterator = constructionVariables.labelledStrings.find("path");
	if(stringMapIterator == constructionVariables.labelledStrings.end() )
	{
		const char *const pathErrorMessage = "The variable path is needed for the construction of an OutputMethodType";
		throw(pathErrorMessage);
	}
	else { path = stringMapIterator->second.at(0); }

	std::vector<std::string> outputFiles;
	stringMapIterator = constructionVariables.labelledStrings.find("outputFiles");
	if(stringMapIterator == constructionVariables.labelledStrings.end() )
	{
		const char *const outputFilesErrorMessage = "The variable outputFiles is needed for the construction of an OutputMethodType";
		throw(outputFilesErrorMessage);
	}
	else { outputFiles = stringMapIterator->second; }

	bool writeLineEachTimeStep;
	boolMapIterator = constructionVariables.labelledBools.find("writeLineEachTimeStep");
	if(boolMapIterator == constructionVariables.labelledBools.end() )
	{
		const char *const writeLineEachTimeStepErrorMessage = "The variable writeLineEachTimeStep is needed for the construction of an OutputMethodType";
		throw(writeLineEachTimeStepErrorMessage);
	}
	else { writeLineEachTimeStep = boolMapIterator->second.at(0); }

	double outputInterval;
	doubleMapIterator = constructionVariables.labelledDoubles.find("outputInterval");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const outputIntervalErrorMessage = "The variable outputInterval is needed for the construction of an OutputMethodType";
		throw(outputIntervalErrorMessage);
	}
	else { outputInterval = doubleMapIterator->second.at(0); }


	std::vector<double> explicitTimesForOutput;
	doubleMapIterator = constructionVariables.labelledDoubles.find("explicitTimesForOutput");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const explicitTimesForOutputErrorMessage = "The variable explicitTimesForOutput is needed for the construction of an OutputMethodType";
		throw(explicitTimesForOutputErrorMessage);
	}
	else { explicitTimesForOutput = doubleMapIterator->second; }

	int precisionForOutput;
	intMapIterator = constructionVariables.labelledInts.find("precisionForOutput");
	if(intMapIterator == constructionVariables.labelledInts.end() )
	{
		const char *const precisionForOutputErrorMessage = "The variable precisionForOutput is needed for the construction of an OutputMethodType";
		throw(precisionForOutputErrorMessage);
	}
	else { precisionForOutput = intMapIterator->second.at(0); }

	bool overwriteFiles;
	int fileID;
	int numberOfFileIDDigits;
	std::vector<CombinerVariables::TypesOfRegularRiverReachProperties> regularRiverReachPropertiesForOutput;
	std::vector<int> userCellIDsForOutput;
	bool outputTimeStepLength, outputInitialValues, printUpstreamMargins, printDownstreamMargin;
	int horizontalBarLength;
	bool outputIncludingPoreVolume,outputDetailedFractional;

	bool useSecondaryOutputInterval = false;
	int referenceCellUserCellID;
	CombinerVariables::TypesOfRegularRiverReachProperties referenceProperty = CombinerVariables::discharge;
	double thresholdToBeExceeded;
	double secondaryOutputInterval;
	std::vector<CombinerVariables::TypesOfCombinersAndInterfaces> setupPropertiesForOutput;
	bool printSimulationID, printSimulationName, printStartingTime, printModelVersion;
	std::string simulationID, simulationName;

	switch (typeOfOutputMethod)
	{
	case CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML:
		boolMapIterator = constructionVariables.labelledBools.find("overwriteFiles");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const OutputVerbatimTranslationOfConstructionVariablesToXMLErrorMessage = "The variable overwriteFiles is needed for the construction of the OutputMethodType OutputVerbatimTranslationOfConstructionVariablesToXML";
			throw(OutputVerbatimTranslationOfConstructionVariablesToXMLErrorMessage);
		}
		else { overwriteFiles = boolMapIterator->second.at(0); }
		intMapIterator = constructionVariables.labelledInts.find("fileID");
		if(intMapIterator == constructionVariables.labelledInts.end() ) { fileID = 0; }
		else { fileID = intMapIterator->second.at(0); }
		intMapIterator = constructionVariables.labelledInts.find("numberOfFileIDDigits");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const numberOfFileIDDigitsErrorMessage = "The variable numberOfFileIDDigits is needed for the construction of the OutputMethodType OutputVerbatimTranslationOfConstructionVariablesToXML";
			throw(numberOfFileIDDigitsErrorMessage);
		}
		else { numberOfFileIDDigits = intMapIterator->second.at(0); }
		result = new OutputVerbatimTranslationOfConstructionVariablesToXML(overwriteFiles,fileID,numberOfFileIDDigits,path,outputFiles,writeLineEachTimeStep,outputInterval,explicitTimesForOutput,precisionForOutput,highestOrderStructuresPointers.overallParameters,highestOrderStructuresPointers.overallMethods,highestOrderStructuresPointers.riverSystemProperties,highestOrderStructuresPointers.riverSystemMethods);
		break;

	case CombinerVariables::OutputRegularRiverReachProperties:
		stringMapIterator = constructionVariables.labelledStrings.find("regularRiverReachPropertiesForOutput");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const regularRiverReachPropertiesForOutputErrorMessage = "The variable regularRiverReachPropertiesForOutput is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
			throw(regularRiverReachPropertiesForOutputErrorMessage);
		}
		else
		{
			stringVector = stringMapIterator->second;
			regularRiverReachPropertiesForOutput.clear();
			for(std::vector<std::string>::const_iterator currentString = stringVector.begin(); currentString < stringVector.end(); ++currentString)
					{ regularRiverReachPropertiesForOutput.push_back( CombinerVariables::stringToTypeOfRegularRiverReachProperties( *currentString ) );	}
		}
		intMapIterator = constructionVariables.labelledInts.find("reachIDsForOutput");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const reachIDsForOutputErrorMessage = "The variable reachIDsForOutput is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
			throw(reachIDsForOutputErrorMessage);
		}
		else { userCellIDsForOutput = intMapIterator->second; }
		boolMapIterator = constructionVariables.labelledBools.find("outputTimeStepLength");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const outputTimeStepLengthErrorMessage = "The variable outputTimeStepLength is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
			throw(outputTimeStepLengthErrorMessage);
		}
		else { outputTimeStepLength = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("outputInitialValues");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const outputInitialValues = "The variable outputInitialValues is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
			throw(outputInitialValues);
		}
		else { outputInitialValues = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("printUpstreamMargins");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printUpstreamMarginsErrorMessage = "The variable printUpstreamMargins is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
			throw(printUpstreamMarginsErrorMessage);
		}
		else { printUpstreamMargins = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("printDownstreamMargin");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printDownstreamMarginErrorMessage = "The variable printDownstreamMargin is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
			throw(printDownstreamMarginErrorMessage);
		}
		else { printDownstreamMargin = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("useSecondaryOutputInterval");
		if(boolMapIterator != constructionVariables.labelledBools.end() )
		{ useSecondaryOutputInterval = boolMapIterator->second.at(0); }
		if(useSecondaryOutputInterval)
		{
			intMapIterator = constructionVariables.labelledInts.find("referenceCellUserCellID");
			if(intMapIterator == constructionVariables.labelledInts.end() )
			{
				const char *const referenceCellUserCellIDErrorMessage = "The variable referenceCellUserCellID is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
				throw(referenceCellUserCellIDErrorMessage);
			}
			else { referenceCellUserCellID = intMapIterator->second.at(0); }
			stringMapIterator = constructionVariables.labelledStrings.find("referenceProperty");
			if(stringMapIterator == constructionVariables.labelledStrings.end() )
			{
				const char *const referencePropertyErrorMessage = "The variable referenceProperty is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
				throw(referencePropertyErrorMessage);
			}
			else { referenceProperty = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }
			doubleMapIterator = constructionVariables.labelledDoubles.find("thresholdToBeExceeded");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const thresholdToBeExceededErrorMessage = "The variable thresholdToBeExceeded is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
				throw(thresholdToBeExceededErrorMessage);
			}
			else { thresholdToBeExceeded = doubleMapIterator->second.at(0); }
			doubleMapIterator = constructionVariables.labelledDoubles.find("secondaryOutputInterval");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const secondaryOutputIntervalErrorMessage = "The variable secondaryOutputInterval is needed for the construction of the OutputMethodType OutputRegularRiverReachProperties";
				throw(secondaryOutputIntervalErrorMessage);
			}
			else { secondaryOutputInterval = doubleMapIterator->second.at(0); }
		}
		result = new OutputRegularRiverReachProperties(regularRiverReachPropertiesForOutput,userCellIDsForOutput,outputTimeStepLength,outputInitialValues,printUpstreamMargins,printDownstreamMargin,path,outputFiles,writeLineEachTimeStep,outputInterval,explicitTimesForOutput,precisionForOutput,useSecondaryOutputInterval,referenceCellUserCellID,referenceProperty,thresholdToBeExceeded,secondaryOutputInterval,highestOrderStructuresPointers.overallParameters,highestOrderStructuresPointers.overallMethods,highestOrderStructuresPointers.riverSystemProperties,highestOrderStructuresPointers.riverSystemMethods);
		break;

	case CombinerVariables::OutputRegularRiverReachPropertiesForVisualInterpretation:
		stringMapIterator = constructionVariables.labelledStrings.find("regularRiverReachPropertiesForOutput");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const regularRiverReachPropertiesForOutputErrorMessage = "The variable regularRiverReachPropertiesForOutput is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(regularRiverReachPropertiesForOutputErrorMessage);
		}
		else
		{
			stringVector = stringMapIterator->second;
			regularRiverReachPropertiesForOutput.clear();
			for(std::vector<std::string>::const_iterator currentString = stringVector.begin(); currentString < stringVector.end(); ++currentString)
					{ regularRiverReachPropertiesForOutput.push_back( CombinerVariables::stringToTypeOfRegularRiverReachProperties( *currentString ) );	}
		}
		intMapIterator = constructionVariables.labelledInts.find("reachIDsForOutput");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const reachIDsForOutputErrorMessage = "The variable reachIDsForOutput is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(reachIDsForOutputErrorMessage);
		}
		else { userCellIDsForOutput = intMapIterator->second; }
		boolMapIterator = constructionVariables.labelledBools.find("outputTimeStepLength");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const outputTimeStepLengthErrorMessage = "The variable outputTimeStepLength is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(outputTimeStepLengthErrorMessage);
		}
		else { outputTimeStepLength = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("outputInitialValues");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const outputInitialValues = "The variable outputInitialValues is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(outputInitialValues);
		}
		else { outputInitialValues = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("printUpstreamMargins");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printUpstreamMarginsErrorMessage = "The variable printUpstreamMargins is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(printUpstreamMarginsErrorMessage);
		}
		else { printUpstreamMargins = boolMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("printDownstreamMargin");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printDownstreamMarginErrorMessage = "The variable printDownstreamMargin is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(printDownstreamMarginErrorMessage);
		}
		else { printDownstreamMargin = boolMapIterator->second.at(0); }
		intMapIterator = constructionVariables.labelledInts.find("horizontalBarLength");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const horizontalBarLengthErrorMessage = "The variable horizontalBarLength is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
			throw(horizontalBarLengthErrorMessage);
		}
		else { horizontalBarLength = intMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("useSecondaryOutputInterval");
		if(boolMapIterator != constructionVariables.labelledBools.end() )
		{ useSecondaryOutputInterval = boolMapIterator->second.at(0); }
		if(useSecondaryOutputInterval)
		{
			intMapIterator = constructionVariables.labelledInts.find("referenceCellUserCellID");
			if(intMapIterator == constructionVariables.labelledInts.end() )
			{
				const char *const referenceCellUserCellIDErrorMessage = "The variable referenceCellUserCellID is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
				throw(referenceCellUserCellIDErrorMessage);
			}
			else { referenceCellUserCellID = intMapIterator->second.at(0); }
			stringMapIterator = constructionVariables.labelledStrings.find("referenceProperty");
			if(stringMapIterator == constructionVariables.labelledStrings.end() )
			{
				const char *const referencePropertyErrorMessage = "The variable referenceProperty is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
				throw(referencePropertyErrorMessage);
			}
			else { referenceProperty = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }
			doubleMapIterator = constructionVariables.labelledDoubles.find("thresholdToBeExceeded");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const thresholdToBeExceededErrorMessage = "The variable thresholdToBeExceeded is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
				throw(thresholdToBeExceededErrorMessage);
			}
			else { thresholdToBeExceeded = doubleMapIterator->second.at(0); }
			doubleMapIterator = constructionVariables.labelledDoubles.find("secondaryOutputInterval");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const secondaryOutputIntervalErrorMessage = "The variable secondaryOutputInterval is needed for the construction of the OutputMethodType OutputRegularRiverReachPropertiesForVisualInterpretation";
				throw(secondaryOutputIntervalErrorMessage);
			}
			else { secondaryOutputInterval = doubleMapIterator->second.at(0); }
		}
		result = new OutputRegularRiverReachPropertiesForVisualInterpretation(regularRiverReachPropertiesForOutput,userCellIDsForOutput,outputTimeStepLength,horizontalBarLength,outputInitialValues,printUpstreamMargins,printDownstreamMargin,path,outputFiles,writeLineEachTimeStep,outputInterval,explicitTimesForOutput,precisionForOutput,useSecondaryOutputInterval,referenceCellUserCellID,referenceProperty,thresholdToBeExceeded,secondaryOutputInterval,highestOrderStructuresPointers.overallParameters,highestOrderStructuresPointers.overallMethods,highestOrderStructuresPointers.riverSystemProperties,highestOrderStructuresPointers.riverSystemMethods);
		break;

	case CombinerVariables::OutputAccumulatedBedloadTransport:
		intMapIterator = constructionVariables.labelledInts.find("reachIDsForOutput");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const reachIDsForOutputErrorMessage = "The variable reachIDsForOutput is needed for the construction of the OutputMethodType OutputAccumulatedBedloadTransport";
			throw(reachIDsForOutputErrorMessage);
		}
		else { userCellIDsForOutput = intMapIterator->second; }

		boolMapIterator = constructionVariables.labelledBools.find("outputIncludingPoreVolume");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const outputIncludingPoreVolumeErrorMessage = "The variable outputIncludingPoreVolume is needed for the construction of the OutputMethodType OutputAccumulatedBedloadTransport";
			throw(outputIncludingPoreVolumeErrorMessage);
		}
		else { outputIncludingPoreVolume = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("outputDetailedFractional");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const outputDetailedFractionalErrorMessage = "The variable outputDetailedFractional is needed for the construction of the OutputMethodType OutputAccumulatedBedloadTransport";
			throw(outputDetailedFractionalErrorMessage);
		}
		else { outputDetailedFractional = boolMapIterator->second.at(0); }

		result = new OutputAccumulatedBedloadTransport(userCellIDsForOutput,path,outputFiles,writeLineEachTimeStep,outputInterval,explicitTimesForOutput,precisionForOutput,outputIncludingPoreVolume,highestOrderStructuresPointers.overallParameters,highestOrderStructuresPointers.overallMethods,highestOrderStructuresPointers.riverSystemProperties,highestOrderStructuresPointers.riverSystemMethods,outputDetailedFractional);
		break;

	case CombinerVariables::OutputSimulationSetup:
		stringMapIterator = constructionVariables.labelledStrings.find("setupPropertiesForOutput");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const setupPropertiesForOutputErrorMessage = "The variable setupPropertiesForOutput is needed for the construction of the OutputMethodType OutputSimulationSetup";
			throw(setupPropertiesForOutputErrorMessage);
		}
		else
		{
			stringVector = stringMapIterator->second;
			setupPropertiesForOutput.clear();
			for(std::vector<std::string>::const_iterator currentString = stringVector.begin(); currentString < stringVector.end(); ++currentString)
					{ setupPropertiesForOutput.push_back( CombinerVariables::stringToTypeOfCombinersAndInterfaces( *currentString ) );	}
		}

		boolMapIterator = constructionVariables.labelledBools.find("printSimulationID");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printSimulationIDErrorMessage = "The variable printSimulationID is needed for the construction of the OutputMethodType OutputSimulationSetup";
			throw(printSimulationIDErrorMessage);
		}
		else { printSimulationID = boolMapIterator->second.at(0); }
		if(printSimulationID)
		{
			stringMapIterator = constructionVariables.labelledStrings.find("simulationID");
			if(stringMapIterator == constructionVariables.labelledStrings.end() )
			{
				const char *const simulationIDErrorMessage = "The variable simulationID is needed for the construction of the OutputMethodType OutputSimulationSetup";
				throw(simulationIDErrorMessage);
			}
			else { simulationID = stringMapIterator->second.at(0); }
		}

		boolMapIterator = constructionVariables.labelledBools.find("printSimulationName");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printSimulationNameErrorMessage = "The variable printSimulationName is needed for the construction of the OutputMethodType OutputSimulationSetup";
			throw(printSimulationNameErrorMessage);
		}
		else { printSimulationName = boolMapIterator->second.at(0); }
		if(printSimulationName)
		{
			stringMapIterator = constructionVariables.labelledStrings.find("simulationName");
			if(stringMapIterator == constructionVariables.labelledStrings.end() )
			{
				const char *const simulationNameErrorMessage = "The variable simulationName is needed for the construction of the OutputMethodType OutputSimulationSetup";
				throw(simulationNameErrorMessage);
			}
			else { simulationName = stringMapIterator->second.at(0); }
		}

		boolMapIterator = constructionVariables.labelledBools.find("printStartingTime");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printStartingTimeErrorMessage = "The variable printStartingTime is needed for the construction of the OutputMethodType OutputSimulationSetup";
			throw(printStartingTimeErrorMessage);
		}
		else { printStartingTime = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("printModelVersion");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const printModelVersionErrorMessage = "The variable printModelVersion is needed for the construction of the OutputMethodType OutputSimulationSetup";
			throw(printModelVersionErrorMessage);
		}
		else { printModelVersion = boolMapIterator->second.at(0); }

		result = new OutputSimulationSetup(setupPropertiesForOutput,printSimulationID,simulationID,printSimulationName,simulationName,printStartingTime,printModelVersion,path,outputFiles,precisionForOutput,highestOrderStructuresPointers.overallParameters,highestOrderStructuresPointers.overallMethods,highestOrderStructuresPointers.riverSystemProperties,highestOrderStructuresPointers.riverSystemMethods);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Output Method Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

AdditionalRiverSystemPropertyType* SedFlowInterfaceRealisationBuilders::additionalRiverSystemPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::AdditionalRiverSystemPropertyType)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (additionalRiverSystemPropertyTypeBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod typeOfAdditionalRiverSystemPropertyAndMethod = CombinerVariables::stringToTypeOfAdditionalRiverSystemPropertyAndMethod(constructionVariables.realisationType);

	AdditionalRiverSystemPropertyType* result;

	// Switch this switch-case back on again, whenever there are real cases besides the default.
	/*
	switch (typeOfAdditionalRiverSystemPropertyAndMethod)
	{
	case CombinerVariables::???:
		result = new ???(???, constructionVariables.unlabelledDoubles);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Additional River System Property and Method Type";
		throw (invalidTypeErrorMessage);
	}
	*/

	return result;
}

AdditionalRiverReachPropertyType* SedFlowInterfaceRealisationBuilders::additionalRiverReachPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	try
	{
		if (constructionVariables.interfaceOrCombinerType != CombinerVariables::AdditionalRiverReachPropertyType)
		{
			const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (additionalRiverReachPropertyTypeBuilder)";
			throw(methodCompatibilityErrorMessage);
		}

		CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalRiverReachPropertyAndMethod = CombinerVariables::stringToTypeOfAdditionalRiverReachPropertyAndMethod(constructionVariables.realisationType);

		AdditionalRiverReachPropertyType* result;

		std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;

		double equivalentGrainDiameter;

		char* errorMessage;
		switch (typeOfAdditionalRiverReachPropertyAndMethod)
		{
		case CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter:

			doubleMapIterator = constructionVariables.labelledDoubles.find("equivalentGrainDiameter");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const equivalentGrainDiameterErrorMessage = "The variable equivalentGrainDiameter is needed for the construction of the AdditionalRiverReachPropertyType BedrockRoughnessEquivalentRepresentativeGrainDiameter.";
				throw(equivalentGrainDiameterErrorMessage);
			}
			else { equivalentGrainDiameter = doubleMapIterator->second.at(0); }

			result = new BedrockRoughnessEquivalentRepresentativeGrainDiameter(equivalentGrainDiameter);
			break;

		case CombinerVariables::ScourChain:
			//TODO Less Important: Implement method
			errorMessage = "The build method for ScourChainProperties has not been implemented yet.";
			throw(errorMessage);
			break;

		default:
			errorMessage = "Invalid Additional River Reach Property and Method Type";
			throw (errorMessage);
		}

		return result;

	} catch (char* msg) {
		const char *const constMsg = msg;
		throw(constMsg);
	}
}

GrainType* SedFlowInterfaceRealisationBuilders::grainTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::GrainType)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (grainTypeBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfGrains typeOfGrains = CombinerVariables::stringToTypeOfGrains(constructionVariables.realisationType);

	GrainType* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<int> >::const_iterator intMapIterator;

	std::vector<double> fractionalAbundance;
	doubleMapIterator = constructionVariables.labelledDoubles.find("fractionalAbundance");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const fractionalAbundanceErrorMessage = "The variable fractionalAbundance is needed for the construction of a GrainType";
		throw(fractionalAbundanceErrorMessage);
	}
	else { fractionalAbundance = doubleMapIterator->second; }

	int insertionLayer;

	switch (typeOfGrains)
	{
	case CombinerVariables::NormalGrains:
		result = new NormalGrains(fractionalAbundance);
		break;

	case CombinerVariables::TracerGrains:
		result = new TracerGrains(fractionalAbundance);
		break;

	case CombinerVariables::FishEggs:
		intMapIterator = constructionVariables.labelledInts.find("insertionLayer");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const insertionLayerErrorMessage = "The variable insertionLayer is needed for the construction of the GrainType FishEggs";
			throw(insertionLayerErrorMessage);
		}
		else { insertionLayer = intMapIterator->second.at(0); }
		result = new FishEggs(fractionalAbundance, insertionLayer);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Grain Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

GeometricalChannelBehaviour* SedFlowInterfaceRealisationBuilders::geometricalChannelBehaviourBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::GeometricalChannelBehaviour)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (geometricalChannelBehaviourBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfGeometricalChannelBehaviour typeOfGeometricalChannelBehaviour = CombinerVariables::stringToTypeOfGeometricalChannelBehaviour(constructionVariables.realisationType);

	GeometricalChannelBehaviour* result;

	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	ChannelGeometry* alluviumChannel;
	objectMapIterator = labelledObjects.find("alluviumChannel");
	if(objectMapIterator == labelledObjects.end() )
	{
		const char *const alluviumChannelErrorMessage = "The variable alluviumChannel is needed for the construction of a GeometricalChannelBehaviour";
		throw(alluviumChannelErrorMessage);
	}
	else { alluviumChannel = static_cast<ChannelGeometry*>( objectMapIterator->second.at(0) ); }

	bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate;

	switch (typeOfGeometricalChannelBehaviour)
	{
	case CombinerVariables::ParallelShiftOfBasicGeometry:
		boolMapIterator = constructionVariables.labelledBools.find("upstreamOfSillsWedgeShapedInsteadOfParallelUpdate");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const upstreamOfSillsWedgeShapedInsteadOfParallelUpdateErrorMessage = "The variable upstreamOfSillsWedgeShapedInsteadOfParallelUpdate is needed for the construction of the GeometricalChannelBehaviour ParallelShiftOfBasicGeometry";
			throw(upstreamOfSillsWedgeShapedInsteadOfParallelUpdateErrorMessage);
		}
		else { upstreamOfSillsWedgeShapedInsteadOfParallelUpdate = boolMapIterator->second.at(0); }
		result = new ParallelShiftOfBasicGeometry(alluviumChannel,upstreamOfSillsWedgeShapedInsteadOfParallelUpdate);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Geometrical Channel Behaviour Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

ChannelGeometry* SedFlowInterfaceRealisationBuilders::channelGeometryBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::ChannelGeometry)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (channelGeometryBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfChannelGeometry typeOfChannelGeometry = CombinerVariables::stringToTypeOfChannelGeometry(constructionVariables.realisationType);

	ChannelGeometry* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;

	double channelWidth, bankSlope;

	switch (typeOfChannelGeometry)
	{
	case CombinerVariables::InfinitelyDeepRectangularChannel:

		doubleMapIterator = constructionVariables.labelledDoubles.find("channelWidth");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const channelWidthErrorMessage = "The variable channelWidth is needed for the construction of the ChannelGeometry InfinitelyDeepRectangularChannel.";
			throw(channelWidthErrorMessage);
		}
		else { channelWidth = doubleMapIterator->second.at(0); }

		result = new InfinitelyDeepRectangularChannel(channelWidth);
		break;

	case CombinerVariables::InfinitelyDeepVShapedChannel:

		doubleMapIterator = constructionVariables.labelledDoubles.find("bankSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const bankSlopeErrorMessage = "The variable bankSlope is needed for the construction of the ChannelGeometry InfinitelyDeepVShapedChannel.";
			throw(bankSlopeErrorMessage);
		}
		else { bankSlope = doubleMapIterator->second.at(0); }

		result = new InfinitelyDeepVShapedChannel(bankSlope);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Channel Geometry Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

StrataSorting* SedFlowInterfaceRealisationBuilders::strataSortingBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::StrataSorting)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (strataSortingBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfStrataSorting typeOfStrataSorting = CombinerVariables::stringToTypeOfStrataSorting(constructionVariables.realisationType);

	StrataSorting* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	double layerThickness;
	if(typeOfStrataSorting != CombinerVariables::StratigraphyWithThresholdBasedUpdate)
	{
		doubleMapIterator = constructionVariables.labelledDoubles.find("layerThickness");
		if( (doubleMapIterator == constructionVariables.labelledDoubles.end()) )
		{
			const char *const layerThicknessErrorMessage = "The variable layerThickness is needed for the construction of StrataSorting.";
			throw(layerThicknessErrorMessage);
		}
		else { layerThickness = doubleMapIterator->second.at(0); }
	}

	double incrementLayerThickness, valueOrFactorForLowerThresholdForActiveLayerThickness, valueOrFactorForUpperThresholdForActiveLayerThickness, minimumLowerThresholdForActiveLayerThickness, minimumDifferenceBetweenThresholdsForActiveLayerThickness, minimumLayerThicknessPerUnitLayerThickness, maximumLayerThicknessPerUnitLayerThickness;
	bool dynamic;
	bool dynamicBreakUpConditions;
	double layerThicknessFactor = 1.0;
	double referenceGrainSizePercentile = 84.0;
	CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod;

	switch (typeOfStrataSorting)
	{
	case CombinerVariables::StratigraphyWithThresholdBasedUpdate:

		doubleMapIterator = constructionVariables.labelledDoubles.find("incrementLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const incrementLayerThicknessErrorMessage = "The variable incrementLayerThickness is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
			throw(incrementLayerThicknessErrorMessage);
		}
		else { incrementLayerThickness = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("dynamicThresholds");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const dynamicThresholdsErrorMessage = "The variable dynamicThresholds is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
			throw(dynamicThresholdsErrorMessage);
		}
		else { dynamic = boolMapIterator->second.at(0); }

		if(dynamic)
		{
			doubleMapIterator = constructionVariables.labelledDoubles.find("referenceGrainSizePercentile");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const referenceGrainSizePercentileErrorMessage = "The variable referenceGrainSizePercentile is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
				throw(referenceGrainSizePercentileErrorMessage);
			}
			else { referenceGrainSizePercentile = doubleMapIterator->second.at(0); }
		}

		doubleMapIterator = constructionVariables.labelledDoubles.find("valueOrFactorForLowerThresholdForActiveLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage = "The variable valueOrFactorForLowerThresholdForActiveLayerThickness is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
			throw(valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage);
		}
		else { valueOrFactorForLowerThresholdForActiveLayerThickness = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("valueOrFactorForUpperThresholdForActiveLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage = "The variable valueOrFactorForUpperThresholdForActiveLayerThickness is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
			throw(valueOrFactorForLowerThresholdForActiveLayerThicknessErrorMessage);
		}
		else { valueOrFactorForUpperThresholdForActiveLayerThickness = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumLowerThresholdForActiveLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumLowerThresholdForActiveLayerThicknessErrorMessage = "The variable minimumLowerThresholdForActiveLayerThickness is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
			throw(minimumLowerThresholdForActiveLayerThicknessErrorMessage);
		}
		else { minimumLowerThresholdForActiveLayerThickness = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumDifferenceBetweenThresholdsForActiveLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumDifferenceBetweenThresholdsForActiveLayerThicknessErrorMessage = "The variable minimumDifferenceBetweenThresholdsForActiveLayerThickness is needed for the construction of the StrataSorting StratigraphyWithThresholdBasedUpdate.";
			throw(minimumDifferenceBetweenThresholdsForActiveLayerThicknessErrorMessage);
		}
		else { minimumDifferenceBetweenThresholdsForActiveLayerThickness = doubleMapIterator->second.at(0); }

		result = new StratigraphyWithThresholdBasedUpdate(incrementLayerThickness,dynamic,referenceGrainSizePercentile,valueOrFactorForLowerThresholdForActiveLayerThickness,valueOrFactorForUpperThresholdForActiveLayerThickness,minimumLowerThresholdForActiveLayerThickness,minimumDifferenceBetweenThresholdsForActiveLayerThickness);
		break;

	case CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate:

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumLayerThicknessPerUnitLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumLayerThicknessPerUnitLayerThicknessErrorMessage = "The variable minimumLayerThicknessPerUnitLayerThickness is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate.";
			throw(minimumLayerThicknessPerUnitLayerThicknessErrorMessage);
		}
		else { minimumLayerThicknessPerUnitLayerThickness = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumLayerThicknessPerUnitLayerThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumLayerThicknessPerUnitLayerThicknessErrorMessage = "The variable maximumLayerThicknessPerUnitLayerThickness is needed for the construction of the StrataSorting StratigraphyWithOLDConstantThresholdBasedUpdate.";
			throw(maximumLayerThicknessPerUnitLayerThicknessErrorMessage);
		}
		else { maximumLayerThicknessPerUnitLayerThickness = doubleMapIterator->second.at(0); }

		result = new StratigraphyWithOLDConstantThresholdBasedUpdate(layerThickness, minimumLayerThicknessPerUnitLayerThickness, maximumLayerThicknessPerUnitLayerThickness);
		break;

	case CombinerVariables::TwoLayerWithShearStressBasedUpdate:
	case CombinerVariables::TwoLayerWithContinuousUpdate:

		boolMapIterator = constructionVariables.labelledBools.find("dynamicLayerThickness");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const dynamicLayerThicknessErrorMessage = "The variable dynamicLayerThickness is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate or TwoLayerWithShearStressBasedUpdate.";
			throw(dynamicLayerThicknessErrorMessage);
		}
		else { dynamic = boolMapIterator->second.at(0); }

		if(dynamic)
		{
			doubleMapIterator = constructionVariables.labelledDoubles.find("layerThicknessFactor");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const layerThicknessFactorErrorMessage = "The variable layerThicknessFactor is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate or TwoLayerWithShearStressBasedUpdate.";
				throw(layerThicknessFactorErrorMessage);
			}
			else { layerThicknessFactor = doubleMapIterator->second.at(0); }

			doubleMapIterator = constructionVariables.labelledDoubles.find("referenceGrainSizePercentile");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const referenceGrainSizePercentileErrorMessage = "The variable referenceGrainSizePercentile is needed for the construction of the StrataSorting TwoLayerWithContinuousUpdate or TwoLayerWithShearStressBasedUpdate.";
				throw(referenceGrainSizePercentileErrorMessage);
			}
			else { referenceGrainSizePercentile = doubleMapIterator->second.at(0); }
		}

		if(typeOfStrataSorting == CombinerVariables::TwoLayerWithContinuousUpdate)
		{
			result = new TwoLayerWithContinuousUpdate(layerThickness, dynamic, layerThicknessFactor, referenceGrainSizePercentile);
		}
		else
		{
			boolMapIterator = constructionVariables.labelledBools.find("dynamicBreakUpConditions");
			if(boolMapIterator == constructionVariables.labelledBools.end() )
			{
				const char *const dynamicBreakUpConditionsErrorMessage = "The variable dynamicBreakUpConditions is needed for the construction of the StrataSorting TwoLayerWithShearStressBasedUpdate.";
				throw(dynamicBreakUpConditionsErrorMessage);
			}
			else { dynamicBreakUpConditions = boolMapIterator->second.at(0); }

			objectMapIterator = labelledObjects.find("thresholdCalculationMethod");
			if(objectMapIterator == labelledObjects.end() )
			{
				const char *const thresholdCalculationMethodErrorMessage = "The variable thresholdCalculationMethod is needed for the construction of the StrataSorting TwoLayerWithShearStressBasedUpdate.";
				throw(thresholdCalculationMethodErrorMessage);
			}
			else { thresholdCalculationMethod = static_cast<CalcThresholdForInitiationOfBedloadMotion*>( objectMapIterator->second.at(0) ); }

			bool usePredefinedBreakUpConditions;
			double referenceMedianDiameter, thetaCriticalForActiveLayer, thetaCriticalForSublayer;

			boolMapIterator = constructionVariables.labelledBools.find("usePredefinedBreakUpConditions");
			if(boolMapIterator == constructionVariables.labelledBools.end() )
			{
				const char *const usePredefinedBreakUpConditionsErrorMessage = "The variable usePredefinedBreakUpConditions is needed for the construction of the StrataSorting TwoLayerWithShearStressBasedUpdate.";
				throw(usePredefinedBreakUpConditionsErrorMessage);
			}
			else { usePredefinedBreakUpConditions = boolMapIterator->second.at(0); }

			if(usePredefinedBreakUpConditions)
			{
				doubleMapIterator = constructionVariables.labelledDoubles.find("referenceMedianDiameter");
				if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
				{
					const char *const referenceMedianDiameterErrorMessage = "The variable referenceMedianDiameter is needed for the construction of the StrataSorting TwoLayerWithShearStressBasedUpdate.";
					throw(referenceMedianDiameterErrorMessage);
				}
				else { referenceMedianDiameter = doubleMapIterator->second.at(0); }

				doubleMapIterator = constructionVariables.labelledDoubles.find("thetaCriticalForActiveLayer");
				if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
				{
					const char *const thetaCriticalForActiveLayerErrorMessage = "The variable thetaCriticalForActiveLayer is needed for the construction of the StrataSorting TwoLayerWithShearStressBasedUpdate.";
					throw(thetaCriticalForActiveLayerErrorMessage);
				}
				else { thetaCriticalForActiveLayer = doubleMapIterator->second.at(0); }

				doubleMapIterator = constructionVariables.labelledDoubles.find("thetaCriticalForSublayer");
				if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
				{
					const char *const thetaCriticalForSublayerErrorMessage = "The variable thetaCriticalForSublayer is needed for the construction of the StrataSorting TwoLayerWithShearStressBasedUpdate.";
					throw(thetaCriticalForSublayerErrorMessage);
				}
				else { thetaCriticalForSublayer = doubleMapIterator->second.at(0); }

				result = new TwoLayerWithShearStressBasedUpdate(thresholdCalculationMethod, dynamicBreakUpConditions, usePredefinedBreakUpConditions, referenceMedianDiameter, thetaCriticalForActiveLayer, thetaCriticalForSublayer, layerThickness, dynamic, layerThicknessFactor, referenceGrainSizePercentile);
			}
			else
			{
				result = new TwoLayerWithShearStressBasedUpdate(thresholdCalculationMethod, dynamicBreakUpConditions, usePredefinedBreakUpConditions, layerThickness, dynamic, layerThicknessFactor, referenceGrainSizePercentile);
			}
		}
		break;

	case CombinerVariables::SingleLayerNoSorting:

		result = new SingleLayerNoSorting(layerThickness);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Strata Sorting Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

SillProperties* SedFlowInterfaceRealisationBuilders::sillPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::SillProperties)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (sillPropertiesBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfSillProperties typeOfSillProperties = CombinerVariables::stringToTypeOfSillProperties(constructionVariables.realisationType);

	SillProperties* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;

	double overfallWidth,poleniFactor;

	switch (typeOfSillProperties)
	{
	case CombinerVariables::PoleniSill:

		doubleMapIterator = constructionVariables.labelledDoubles.find("overfallWidth");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const overfallWidthErrorMessage = "The variable overfallWidth is needed for the construction of the SillProperties PoleniSill.";
			throw(overfallWidthErrorMessage);
		}
		else { overfallWidth = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("poleniFactor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			result = new PoleniSill(overfallWidth);
		}
		else
		{
			poleniFactor = doubleMapIterator->second.at(0);
			result = new PoleniSill(overfallWidth,poleniFactor);
		}
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Sill Properties Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

CalcGradient* SedFlowInterfaceRealisationBuilders::calcGradientBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcGradient)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcGradientBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfGradientCalculationMethod typeOfGradientCalculationMethod = CombinerVariables::stringToTypeOfGradientCalculationMethod(constructionVariables.realisationType);

	CalcGradient* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;
	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;

	double stressPartitioningExponent,maximumFroudeNumber,minimumSlope,minimumInputSlope;
	bool calculationBasedOnqInsteadOfh,ensureMinimumInputSlope;
	CalcGradient* simpleWaterEnergyslopeCalculationMethod;
	CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest, weightingProperty;

	switch (typeOfGradientCalculationMethod)
	{
	case CombinerVariables::SimpleDownstreamTwoCellGradient:
		stringMapIterator = constructionVariables.labelledStrings.find("propertyOfInterest");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const propertyOfInterestErrorMessage = "The variable propertyOfInterest is needed for the construction of the CalcGradient SimpleDownstreamTwoCellGradient.";
			throw(propertyOfInterestErrorMessage);
		}
		else { propertyOfInterest = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }

		result = new SimpleDownstreamTwoCellGradient(propertyOfInterest);
		break;

	case CombinerVariables::SimpleThreeCellGradient:
		stringMapIterator = constructionVariables.labelledStrings.find("propertyOfInterest");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const propertyOfInterestErrorMessage = "The variable propertyOfInterest is needed for the construction of the CalcGradient SimpleThreeCellGradient.";
			throw(propertyOfInterestErrorMessage);
		}
		else { propertyOfInterest = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }

		stringMapIterator = constructionVariables.labelledStrings.find("weightingProperty");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const weightingPropertyErrorMessage = "The variable weightingProperty is needed for the construction of the CalcGradient SimpleThreeCellGradient.";
			throw(weightingPropertyErrorMessage);
		}
		else { weightingProperty = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }

		result = new SimpleThreeCellGradient(propertyOfInterest,weightingProperty);
		break;

	case CombinerVariables::SimpleDownstreamTwoCellGradientWithCenteredValues:
		stringMapIterator = constructionVariables.labelledStrings.find("propertyOfInterest");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const propertyOfInterestErrorMessage = "The variable propertyOfInterest is needed for the construction of the CalcGradient SimpleDownstreamTwoCellGradientWithCenteredValues.";
			throw(propertyOfInterestErrorMessage);
		}
		else { propertyOfInterest = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }

		result = new SimpleDownstreamTwoCellGradientWithCenteredValues(propertyOfInterest);
		break;

	case CombinerVariables::SimpleThreeCellGradientWithCenteredValues:
		stringMapIterator = constructionVariables.labelledStrings.find("propertyOfInterest");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const propertyOfInterestErrorMessage = "The variable propertyOfInterest is needed for the construction of the CalcGradient SimpleThreeCellGradientWithCenteredValues.";
			throw(propertyOfInterestErrorMessage);
		}
		else { propertyOfInterest = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }

		stringMapIterator = constructionVariables.labelledStrings.find("weightingProperty");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const weightingPropertyErrorMessage = "The variable weightingProperty is needed for the construction of the CalcGradient SimpleThreeCellGradientWithCenteredValues.";
			throw(weightingPropertyErrorMessage);
		}
		else { weightingProperty = CombinerVariables::stringToTypeOfRegularRiverReachProperties( stringMapIterator->second.at(0) ); }

		result = new SimpleThreeCellGradientWithCenteredValues(propertyOfInterest,weightingProperty);
		break;

	case CombinerVariables::ReducedWaterEnergyslope:

		doubleMapIterator = constructionVariables.labelledDoubles.find("stressPartitioningExponent");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const stressPartitioningExponentErrorMessage = "The variable stressPartitioingExponent is needed for the construction of the CalcGradient ReducedWaterEnergyslope.";
			throw(stressPartitioningExponentErrorMessage);
		}
		else { stressPartitioningExponent = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("calculationBasedOnqInsteadOfh");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const calculationBasedOnqInsteadOfhErrorMessage = "The variable calculationBasedOnqInsteadOfh is needed for the construction of the CalcGradient ReducedWaterEnergyslope.";
			throw(calculationBasedOnqInsteadOfhErrorMessage);
		}
		else { calculationBasedOnqInsteadOfh = boolMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumFroudeNumber");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumFroudeNumberErrorMessage = "The variable maximumFroudeNumber is needed for the construction of the CalcGradient ReducedWaterEnergyslope.";
			throw(maximumFroudeNumberErrorMessage);
		}
		else { maximumFroudeNumber = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("ensureMinimumInputSlope");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const ensureMinimumInputSlopeErrorMessage = "The variable ensureMinimumInputSlope is needed for the construction of the CalcGradient ReducedWaterEnergyslope.";
			throw(ensureMinimumInputSlopeErrorMessage);
		}
		else { ensureMinimumInputSlope = boolMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumInputSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumInputSlopeErrorMessage = "The variable minimumInputSlope is needed for the construction of the CalcGradient ReducedWaterEnergyslope.";
			throw(minimumInputSlopeErrorMessage);
		}
		else { minimumInputSlope = doubleMapIterator->second.at(0); }

		result = new ReducedWaterEnergyslope(stressPartitioningExponent,calculationBasedOnqInsteadOfh,maximumFroudeNumber,ensureMinimumInputSlope,minimumInputSlope);
		break;

	case CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable:
		objectMapIterator = labelledObjects.find("simpleWaterEnergyslopeCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const simpleWaterEnergyslopeCalculationMethodErrorMessage = "The variable simpleWaterEnergyslopeCalculationMethod is needed for the construction of the CalcGradient ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.";
			throw(simpleWaterEnergyslopeCalculationMethodErrorMessage);
		}
		else { simpleWaterEnergyslopeCalculationMethod = static_cast<CalcGradient*>( objectMapIterator->second.at(0) ); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("stressPartitioningExponent");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const stressPartitioningExponentErrorMessage = "The variable stressPartitioningExponent is needed for the construction of the CalcGradient ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.";
			throw(stressPartitioningExponentErrorMessage);
		}
		else { stressPartitioningExponent = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("calculationBasedOnqInsteadOfh");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const calculationBasedOnqInsteadOfhErrorMessage = "The variable calculationBasedOnqInsteadOfh is needed for the construction of the CalcGradient ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.";
			throw(calculationBasedOnqInsteadOfhErrorMessage);
		}
		else { calculationBasedOnqInsteadOfh = boolMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumFroudeNumber");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumFroudeNumberErrorMessage = "The variable maximumFroudeNumber is needed for the construction of the CalcGradient ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.";
			throw(maximumFroudeNumberErrorMessage);
		}
		else { maximumFroudeNumber = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("ensureMinimumInputSlope");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const ensureMinimumInputSlopeErrorMessage = "The variable ensureMinimumInputSlope is needed for the construction of the CalcGradient ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.";
			throw(ensureMinimumInputSlopeErrorMessage);
		}
		else { ensureMinimumInputSlope = boolMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumInputSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumInputSlopeErrorMessage = "The variable minimumInputSlope is needed for the construction of the CalcGradient ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.";
			throw(minimumInputSlopeErrorMessage);
		}
		else { minimumInputSlope = doubleMapIterator->second.at(0); }

		result = new ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable(simpleWaterEnergyslopeCalculationMethod,stressPartitioningExponent,calculationBasedOnqInsteadOfh,maximumFroudeNumber,ensureMinimumInputSlope,minimumInputSlope);
		break;

	case CombinerVariables::ReturnBedslope:
		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumSlope");
		if(doubleMapIterator != constructionVariables.labelledDoubles.end() )
		{
			minimumSlope = doubleMapIterator->second.at(0);
			result = new ReturnBedslope(minimumSlope);
		}
		else
		{
			result = new ReturnBedslope();
		}
		break;

	case CombinerVariables::ReturnWaterEnergyslope:
		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumSlope");
		if(doubleMapIterator != constructionVariables.labelledDoubles.end() )
		{
			minimumSlope = doubleMapIterator->second.at(0);
			result = new ReturnWaterEnergyslope(minimumSlope);
		}
		else
		{
			result = new ReturnWaterEnergyslope();
		}
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Gradient Calculation Method Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

FlowResistance* SedFlowInterfaceRealisationBuilders::flowResistanceBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::FlowResistance)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (flowResistanceBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfFlowResistance typeOfFlowResistance = CombinerVariables::stringToTypeOfFlowResistance(constructionVariables.realisationType);

	FlowResistance* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<int> >::const_iterator intMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	double startingValueForIteration, accuracyForTerminatingIteration, maximumFroudeNumber, minimumHydraulicSlope, darcyWeisbachFrictionFactorForBedrock, factor, grainsFactor, grainsPercentile, exponent, darcyWeisbachFrictionFactorForGravelbed, turbulenceLossFactor;
	int maximumNumberOfIterations;
	bool useApproximationsForHydraulicRadius = false;
	CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder;

	doubleMapIterator = constructionVariables.labelledDoubles.find("startingValueForIteration");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const startingValueForIterationErrorMessage = "The variable startingValueForIteration is needed for the construction of FlowResistance.";
		throw(startingValueForIterationErrorMessage);
	}
	else { startingValueForIteration = doubleMapIterator->second.at(0); }

	doubleMapIterator = constructionVariables.labelledDoubles.find("accuracyForTerminatingIteration");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const accuracyForTerminatingIterationErrorMessage = "The variable accuracyForTerminatingIteration is needed for the construction of FlowResistance.";
		throw(accuracyForTerminatingIterationErrorMessage);
	}
	else { accuracyForTerminatingIteration = doubleMapIterator->second.at(0); }

	intMapIterator = constructionVariables.labelledInts.find("maximumNumberOfIterations");
	if(intMapIterator == constructionVariables.labelledInts.end() )
	{
		const char *const maximumNumberOfIterationsErrorMessage = "The variable maximumNumberOfIterations is needed for the construction of FlowResistance.";
		throw(maximumNumberOfIterationsErrorMessage);
	}
	else { maximumNumberOfIterations = intMapIterator->second.at(0); }

	stringMapIterator = constructionVariables.labelledStrings.find("typeOfNumericRootFinder");
	if(stringMapIterator == constructionVariables.labelledStrings.end() )
	{
		const char *const typeOfNumericRootFinderErrorMessage = "The variable typeOfNumericRootFinder is needed for the construction of FlowResistance.";
		throw(typeOfNumericRootFinderErrorMessage);
	}
	else { typeOfNumericRootFinder = CombinerVariables::stringToTypeOfNumericRootFinder(stringMapIterator->second.at(0)); }

	boolMapIterator = constructionVariables.labelledBools.find("useApproximationsForHydraulicRadius");
	if(boolMapIterator != constructionVariables.labelledBools.end() )
	{
		useApproximationsForHydraulicRadius = boolMapIterator->second.at(0);
	}

	doubleMapIterator = constructionVariables.labelledDoubles.find("maximumFroudeNumber");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const maximumFroudeNumberErrorMessage = "The variable maximumFroudeNumber is needed for the construction of FlowResistance.";
		throw(maximumFroudeNumberErrorMessage);
	}
	else { maximumFroudeNumber = doubleMapIterator->second.at(0); }

	doubleMapIterator = constructionVariables.labelledDoubles.find("minimumHydraulicSlope");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const minimumHydraulicSlopeErrorMessage = "The variable minimumHydraulicSlope is needed for the construction of FlowResistance.";
		throw(minimumHydraulicSlopeErrorMessage);
	}
	else { minimumHydraulicSlope = doubleMapIterator->second.at(0); }

	FlowResistance* usedFlowResistanceRelation;

	switch (typeOfFlowResistance)
	{
	case CombinerVariables::VariablePowerLawFlowResistance:
		result = new VariablePowerLawFlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder);
		break;

	case CombinerVariables::FixedPowerLawFlowResistance:
		doubleMapIterator = constructionVariables.labelledDoubles.find("factor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const factorErrorMessage = "The variable factor is needed for the construction of the FlowResistance FixedPowerLawFlowResistance.";
			throw(factorErrorMessage);
		}
		else { factor = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("grainsFactor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const grainsFactorErrorMessage = "The variable grainsFactor is needed for the construction of the FlowResistance FixedPowerLawFlowResistance.";
			throw(grainsFactorErrorMessage);
		}
		else { grainsFactor = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("grainsPercentile");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const grainsPercentileErrorMessage = "The variable grainsPercentile is needed for the construction of the FlowResistance FixedPowerLawFlowResistance.";
			throw(grainsPercentileErrorMessage);
		}
		else { grainsPercentile = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("exponent");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const exponentErrorMessage = "The variable exponent is needed for the construction of the FlowResistance FixedPowerLawFlowResistance.";
			throw(exponentErrorMessage);
		}
		else { exponent = doubleMapIterator->second.at(0); }

		result = new FixedPowerLawFlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,factor,grainsFactor,grainsPercentile,exponent);
		break;

	case CombinerVariables::DarcyWeisbachFlowResistance:
		doubleMapIterator = constructionVariables.labelledDoubles.find("darcyWeisbachFrictionFactorForBedrock");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const darcyWeisbachFrictionFactorForBedrockErrorMessage = "The variable darcyWeisbachFrictionFactorForBedrock is needed for the construction of FlowResistance.";
			throw(darcyWeisbachFrictionFactorForBedrockErrorMessage);
		}
		else { darcyWeisbachFrictionFactorForBedrock = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("darcyWeisbachFrictionFactorForGravelbed");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const darcyWeisbachFrictionFactorForGravelbedErrorMessage = "The variable darcyWeisbachFrictionFactorForGravelbed is needed for the construction of the FlowResistance DarcyWeisbachFlowResistance.";
			throw(darcyWeisbachFrictionFactorForGravelbedErrorMessage);
		}
		else { darcyWeisbachFrictionFactorForGravelbed = doubleMapIterator->second.at(0); }

		result = new DarcyWeisbachFlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,darcyWeisbachFrictionFactorForBedrock,darcyWeisbachFrictionFactorForGravelbed);
		break;

	case CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead:
		objectMapIterator = labelledObjects.find("usedFlowResistanceRelation");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const usedFlowResistanceRelationErrorMessage = "The variable usedFlowResistanceRelation is needed for the construction of the FlowResistance SolveForWaterEnergyslopeBasedOnHydraulicHead.";
			throw(usedFlowResistanceRelationErrorMessage);
		}
		else { usedFlowResistanceRelation = static_cast<FlowResistance*>( objectMapIterator->second.at(0) ); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("turbulenceLossFactor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const turbulenceLossFactorErrorMessage = "The variable turbulenceLossFactor is needed for the construction of the FlowResistance SolveForWaterEnergyslopeBasedOnHydraulicHead.";
			throw(turbulenceLossFactorErrorMessage);
		}
		else { turbulenceLossFactor = doubleMapIterator->second.at(0); }

		result = new SolveForWaterEnergyslopeBasedOnHydraulicHead(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,usedFlowResistanceRelation,turbulenceLossFactor);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of flow resistance";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

AdditionalRiverSystemMethodType* SedFlowInterfaceRealisationBuilders::additionalRiverSystemMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::AdditionalRiverSystemMethodType)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (additionalRiverSystemMethodTypeBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod typeOfAdditionalRiverSystemPropertyAndMethod = CombinerVariables::stringToTypeOfAdditionalRiverSystemPropertyAndMethod(constructionVariables.realisationType);

	AdditionalRiverSystemMethodType* result;

	// Switch this switch-case back on again, whenever there are real cases besides the default.
	/*
	switch (typeOfAdditionalRiverSystemPropertyAndMethod)
	{
	case CombinerVariables::???:
		result = new ???(???, constructionVariables.unlabelledDoubles);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Additional River System Property and Method Type";
		throw (invalidTypeErrorMessage);
	}
	*/

	return result;
}

FlowTypeMethods* SedFlowInterfaceRealisationBuilders::flowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::FlowTypeMethods)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (flowTypeMethodsBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfFlowMethods typeOfFlowMethods = CombinerVariables::stringToTypeOfFlowMethods(constructionVariables.realisationType);

	FlowTypeMethods* result;

	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;

	std::vector<SedimentFlowTypeMethods*> constitutingSedimentFlowMethodsTypes;
	bool updateErosionRatesInMarginCells,checkForCourantFriedrichsLewy;
	double maximumTimeStep;

	switch (typeOfFlowMethods)
	{
	case CombinerVariables::ImplicitKinematicWave:

		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumTimeStep");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumTimeStepImplicitKinematicWaveErrorMessage = "The variable maximumTimeStep is needed for the construction of FlowTypeMethods ImplicitKinematicWave.";
			throw(maximumTimeStepImplicitKinematicWaveErrorMessage);
		}
		else { maximumTimeStep = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("checkForCourantFriedrichsLewy");
		if(boolMapIterator == constructionVariables.labelledBools.end() ) { checkForCourantFriedrichsLewy = false; }
		else { checkForCourantFriedrichsLewy = boolMapIterator->second.at(0); }

		result = new ImplicitKinematicWave( maximumTimeStep, checkForCourantFriedrichsLewy, *(highestOrderStructuresPointers.overallMethods) );
		break;

	case CombinerVariables::ExplicitKinematicWave:
		result = new ExplicitKinematicWave( *(highestOrderStructuresPointers.overallMethods) );
		break;

	case CombinerVariables::sedimentFlowMethods:
		objectMapIterator = labelledObjects.find("constitutingSedimentFlowMethodsTypes");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const constitutingSedimentFlowMethodsTypesErrorMessage = "The variable constitutingSedimentFlowMethodsTypes is needed for the construction of the FlowTypeMethods SedimentFlowMethods.";
			throw(constitutingSedimentFlowMethodsTypesErrorMessage);
		}
		else {
				constitutingSedimentFlowMethodsTypes.reserve(objectMapIterator->second.size());
				for( std::vector<void*>::const_iterator currentSedimentFlowMethodsType = objectMapIterator->second.begin(); currentSedimentFlowMethodsType < objectMapIterator->second.end(); ++currentSedimentFlowMethodsType)
						{ constitutingSedimentFlowMethodsTypes.push_back( static_cast<SedimentFlowTypeMethods*>( *currentSedimentFlowMethodsType ) ); }
		}

		boolMapIterator = constructionVariables.labelledBools.find("updateErosionRatesInMarginCells");
		if(boolMapIterator == constructionVariables.labelledBools.end() ) { updateErosionRatesInMarginCells = false; }
		else { updateErosionRatesInMarginCells = boolMapIterator->second.at(0); }

		result = new SedimentFlowMethods(constitutingSedimentFlowMethodsTypes, updateErosionRatesInMarginCells);
		break;

	case CombinerVariables::UniformDischarge:
		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumTimeStep");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumTimeStepUniformDischargeErrorMessage = "The variable maximumTimeStep is needed for the construction of FlowTypeMethods UniformDischarge.";
			throw(maximumTimeStepUniformDischargeErrorMessage);
		}
		else { maximumTimeStep = doubleMapIterator->second.at(0); }

		result = new UniformDischarge( maximumTimeStep, (*(highestOrderStructuresPointers.overallMethods)) );
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Flow Method Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

SedimentFlowTypeMethods* SedFlowInterfaceRealisationBuilders::sedimentFlowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::SedimentFlowTypeMethods)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (sedimentFlowTypeMethodsBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfSedimentFlowMethods typeOfSedimentFlowMethods = CombinerVariables::stringToTypeOfSedimentFlowMethods(constructionVariables.realisationType);

	SedimentFlowTypeMethods* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;

	double maximumFractionOfActiveLayerToBeEroded;
	doubleMapIterator = constructionVariables.labelledDoubles.find("maximumFractionOfActiveLayerToBeEroded");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const maximumFractionOfActiveLayerToBeErodedErrorMessage = "The variable maximumFractionOfActiveLayerToBeEroded is needed for the construction of SedimentFlowTypeMethods.";
		throw(maximumFractionOfActiveLayerToBeErodedErrorMessage);
	}
	else { maximumFractionOfActiveLayerToBeEroded = doubleMapIterator->second.at(0); }

	bool preventZeroOrNegativeBedSlopes;
	boolMapIterator = constructionVariables.labelledBools.find("preventZeroOrNegativeBedSlopes");
	if(boolMapIterator == constructionVariables.labelledBools.end() )
	{
		const char *const preventZeroOrNegativeBedSlopesErrorMessage = "The variable preventZeroOrNegativeBedSlopes is needed for the construction of SedimentFlowTypeMethods.";
		throw(preventZeroOrNegativeBedSlopesErrorMessage);
	}
	else { preventZeroOrNegativeBedSlopes = boolMapIterator->second.at(0); }

	double maximumRelativeTwoCellBedSlopeChange;
	doubleMapIterator = constructionVariables.labelledDoubles.find("maximumRelativeTwoCellBedSlopeChange");
	if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const maximumRelativeTwoCellBedSlopeChangeErrorMessage = "The variable maximumRelativeTwoCellBedSlopeChange is needed for the construction of SedimentFlowTypeMethods.";
		throw(maximumRelativeTwoCellBedSlopeChangeErrorMessage);
	}
	else { maximumRelativeTwoCellBedSlopeChange = doubleMapIterator->second.at(0); }

	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	CalcBedloadCapacity* bedLoadCapacityCalculationMethod;

	switch (typeOfSedimentFlowMethods)
	{
	case CombinerVariables::SuspensionLoadFlowMethods:
		result = new SuspensionLoadFlowMethods(maximumFractionOfActiveLayerToBeEroded,preventZeroOrNegativeBedSlopes,maximumRelativeTwoCellBedSlopeChange, *(highestOrderStructuresPointers.overallMethods));
		break;

	case CombinerVariables::BedloadFlowMethods:
		objectMapIterator = labelledObjects.find("bedLoadCapacityCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const bedLoadCapacityCalculationMethodErrorMessage = "The variable bedLoadCapacityCalculationMethod is needed for the construction of the SedimentFlowTypeMethods BedloadFlowMethods.";
			throw(bedLoadCapacityCalculationMethodErrorMessage);
		}
		else { bedLoadCapacityCalculationMethod = static_cast<CalcBedloadCapacity*>( objectMapIterator->second.at(0) ); }

		result = new BedloadFlowMethods(maximumFractionOfActiveLayerToBeEroded,preventZeroOrNegativeBedSlopes,maximumRelativeTwoCellBedSlopeChange, *(highestOrderStructuresPointers.overallMethods), bedLoadCapacityCalculationMethod);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Sediment Flow Method Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

CalcBedloadCapacity* SedFlowInterfaceRealisationBuilders::calcBedloadCapacityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcBedloadCapacity)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcBedloadCapacityBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfBedloadCapacityCalculationMethod typeOfBedloadCapacityCalculationMethod = CombinerVariables::stringToTypeOfBedloadCapacityCalculationMethod(constructionVariables.realisationType);

	CalcBedloadCapacity* result;

	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	CalcThresholdForInitiationOfBedloadMotion* thresholdCalculationMethod;
	CalcHidingFactors* hidingFactorsCalculationMethod;
	bool correctionForBedloadWeightAtSteepSlopes, takeArmourLayerIntoAccount;
	bool referenceShearStressBasedOnSlopeANDGrainsizeDistribution = false;
	bool useOnePointOneAsExponentForFroudeNumber = false;
	bool simplifiedEquation = true;
	bool thetaCriticalBasedOnConstantSred = false;
	bool useConstantSandFraction = false;
	double constantSandFraction = 0.4;
	double minimumDimensionlessReferenceShearStressForMeanDiameter;

	switch (typeOfBedloadCapacityCalculationMethod)
	{
	case CombinerVariables::ChengBedloadCapacity:

		objectMapIterator = labelledObjects.find("thresholdCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const thresholdCalculationMethodErrorMessage = "The variable thresholdCalculationMethod is needed for the construction of the bedload capacity calculation method ChengBedloadCapacity";
			throw(thresholdCalculationMethodErrorMessage);
		}
		else { thresholdCalculationMethod = static_cast<CalcThresholdForInitiationOfBedloadMotion*>( objectMapIterator->second.at(0) ); }

		objectMapIterator = labelledObjects.find("hidingFactorsCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const hidingFactorsCalculationMethodErrorMessage = "The variable hidingFactorsCalculationMethod is needed for the construction of the bedload capacity calculation method ChengBedloadCapacity";
			throw(hidingFactorsCalculationMethodErrorMessage);
		}
		else { hidingFactorsCalculationMethod = static_cast<CalcHidingFactors*>( objectMapIterator->second.at(0) ); }

		boolMapIterator = constructionVariables.labelledBools.find("thetaCriticalBasedOnConstantSred");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const thetaCriticalBasedOnConstantSredErrorMessage = "The variable thetaCriticalBasedOnConstantSred is needed for the construction of the bedload capacity calculation method ChengBedloadCapacity";
			throw(thetaCriticalBasedOnConstantSredErrorMessage);
		}
		else { thetaCriticalBasedOnConstantSred = boolMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("factor");
		if(doubleMapIterator != constructionVariables.labelledDoubles.end() )
		{
			result = new ChengBedloadCapacity(thresholdCalculationMethod,hidingFactorsCalculationMethod,thetaCriticalBasedOnConstantSred,(doubleMapIterator->second.at(0)));
		}
		else
		{
			result = new ChengBedloadCapacity(thresholdCalculationMethod,hidingFactorsCalculationMethod,thetaCriticalBasedOnConstantSred);
		}
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnTheta:

		objectMapIterator = labelledObjects.find("thresholdCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const thresholdCalculationMethodErrorMessage = "The variable thresholdCalculationMethod is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnTheta";
			throw(thresholdCalculationMethodErrorMessage);
		}
		else { thresholdCalculationMethod = static_cast<CalcThresholdForInitiationOfBedloadMotion*>( objectMapIterator->second.at(0) ); }

		objectMapIterator = labelledObjects.find("hidingFactorsCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const hidingFactorsCalculationMethodErrorMessage = "The variable hidingFactorsCalculationMethod is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnTheta";
			throw(hidingFactorsCalculationMethodErrorMessage);
		}
		else { hidingFactorsCalculationMethod = static_cast<CalcHidingFactors*>( objectMapIterator->second.at(0) ); }

		boolMapIterator = constructionVariables.labelledBools.find("useOnePointOneAsExponentForFroudeNumber");
		if(boolMapIterator != constructionVariables.labelledBools.end() )
		{ useOnePointOneAsExponentForFroudeNumber = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("simplifiedEquation");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const simplifiedEquationErrorMessage = "The variable simplifiedEquation is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnTheta";
			throw(simplifiedEquationErrorMessage);
		}
		else { simplifiedEquation = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("thetaCriticalBasedOnConstantSred");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const thetaCriticalBasedOnConstantSredErrorMessage = "The variable thetaCriticalBasedOnConstantSred is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnTheta";
			throw(thetaCriticalBasedOnConstantSredErrorMessage);
		}
		else { thetaCriticalBasedOnConstantSred = boolMapIterator->second.at(0); }

		result = new RickenmannBedloadCapacityBasedOnTheta(thresholdCalculationMethod,hidingFactorsCalculationMethod,useOnePointOneAsExponentForFroudeNumber,simplifiedEquation,thetaCriticalBasedOnConstantSred);
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnThetaNonFractional:

		objectMapIterator = labelledObjects.find("thresholdCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const thresholdCalculationMethodErrorMessage = "The variable thresholdCalculationMethod is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnThetaNonFractional";
			throw(thresholdCalculationMethodErrorMessage);
		}
		else { thresholdCalculationMethod = static_cast<CalcThresholdForInitiationOfBedloadMotion*>( objectMapIterator->second.at(0) ); }

		boolMapIterator = constructionVariables.labelledBools.find("takeArmourLayerIntoAccount");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const takeArmourLayerIntoAccountErrorMessage = "The variable takeArmourLayerIntoAccount is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnThetaNonFractional";
			throw(takeArmourLayerIntoAccountErrorMessage);
		}
		else { takeArmourLayerIntoAccount = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("useOnePointOneAsExponentForFroudeNumber");
		if(boolMapIterator != constructionVariables.labelledBools.end() )
		{ useOnePointOneAsExponentForFroudeNumber = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("simplifiedEquation");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const simplifiedEquationErrorMessage = "The variable simplifiedEquation is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnTheta";
			throw(simplifiedEquationErrorMessage);
		}
		else { simplifiedEquation = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("thetaCriticalBasedOnConstantSred");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const thetaCriticalBasedOnConstantSredErrorMessage = "The variable thetaCriticalBasedOnConstantSred is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnThetaNonFractional";
			throw(thetaCriticalBasedOnConstantSredErrorMessage);
		}
		else { thetaCriticalBasedOnConstantSred = boolMapIterator->second.at(0); }

		result = new RickenmannBedloadCapacityBasedOnThetaNonFractional(thresholdCalculationMethod,takeArmourLayerIntoAccount,useOnePointOneAsExponentForFroudeNumber,simplifiedEquation,thetaCriticalBasedOnConstantSred);
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnq:

		boolMapIterator = constructionVariables.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const correctionForBedloadWeightAtSteepSlopesErrorMessage = "The variable correctionForBedloadWeightAtSteepSlopes is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnq.";
			throw(correctionForBedloadWeightAtSteepSlopesErrorMessage);
		}
		else { correctionForBedloadWeightAtSteepSlopes = boolMapIterator->second.at(0); }

		objectMapIterator = labelledObjects.find("hidingFactorsCalculationMethod");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const hidingFactorsCalculationMethodErrorMessage = "The variable hidingFactorsCalculationMethod is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnTheta";
			throw(hidingFactorsCalculationMethodErrorMessage);
		}
		else { hidingFactorsCalculationMethod = static_cast<CalcHidingFactors*>( objectMapIterator->second.at(0) ); }

		result = new RickenmannBedloadCapacityBasedOnq(correctionForBedloadWeightAtSteepSlopes,hidingFactorsCalculationMethod);
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnqNonFractional:

		boolMapIterator = constructionVariables.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const correctionForBedloadWeightAtSteepSlopesErrorMessage = "The variable correctionForBedloadWeightAtSteepSlopes is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnqNonFractional.";
			throw(correctionForBedloadWeightAtSteepSlopesErrorMessage);
		}
		else { correctionForBedloadWeightAtSteepSlopes = boolMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("takeArmourLayerIntoAccount");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const takeArmourLayerIntoAccountErrorMessage = "The variable takeArmourLayerIntoAccount is needed for the construction of the bedload capacity calculation method RickenmannBedloadCapacityBasedOnqNonFractional";
			throw(takeArmourLayerIntoAccountErrorMessage);
		}
		else { takeArmourLayerIntoAccount = boolMapIterator->second.at(0); }

		result = new RickenmannBedloadCapacityBasedOnqNonFractional(correctionForBedloadWeightAtSteepSlopes,takeArmourLayerIntoAccount);
		break;

	case CombinerVariables::WilcockCroweBedloadCapacity:
		boolMapIterator = constructionVariables.labelledBools.find("useConstantSandFraction");
		if(boolMapIterator != constructionVariables.labelledBools.end() )
		{ useConstantSandFraction = boolMapIterator->second.at(0); }

		if(useConstantSandFraction)
		{
			doubleMapIterator = constructionVariables.labelledDoubles.find("constantSandFraction");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const constantSandFractionErrorMessage = "The variable constantSandFraction is needed for the construction of the bedload capacity calculation method WilcockCroweBedloadCapacity.";
				throw(constantSandFractionErrorMessage);
			}
			else { constantSandFraction = doubleMapIterator->second.at(0); }
		}

		result = new WilcockCroweBedloadCapacity(useConstantSandFraction,constantSandFraction);
		break;

	case CombinerVariables::SchneiderEtAlBedloadCapacity:
		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumDimensionlessReferenceShearStressForMeanDiameter");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumDimensionlessReferenceShearStressForMeanDiameterErrorMessage = "The variable minimumDimensionlessReferenceShearStressForMeanDiameter is needed for the construction of the bedload capacity calculation method SchneiderEtAlBedloadCapacity.";
			throw(minimumDimensionlessReferenceShearStressForMeanDiameterErrorMessage);
		}
		else { minimumDimensionlessReferenceShearStressForMeanDiameter = doubleMapIterator->second.at(0); }
		boolMapIterator = constructionVariables.labelledBools.find("referenceShearStressBasedOnSlopeANDGrainsizeDistribution");
		if(boolMapIterator != constructionVariables.labelledBools.end() )
		{ referenceShearStressBasedOnSlopeANDGrainsizeDistribution = boolMapIterator->second.at(0); }

		result = new SchneiderEtAlBedloadCapacity(minimumDimensionlessReferenceShearStressForMeanDiameter,referenceShearStressBasedOnSlopeANDGrainsizeDistribution);
		break;

	case CombinerVariables::ReckingBedloadCapacityNonFractional:
		result = new ReckingBedloadCapacityNonFractional();
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of bedload capacity calculation method.";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

ChangeRateModifiersType* SedFlowInterfaceRealisationBuilders::changeRateModifiersTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::ChangeRateModifiersType)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (changeRateModifiersTypeBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfChangeRateModifiers typeOfChangeRateModifiers = CombinerVariables::stringToTypeOfChangeRateModifiers(constructionVariables.realisationType);

	ChangeRateModifiersType* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;
	std::map<std::string, std::vector<std::string> >::const_iterator stringMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	double minimumBedSlope, maximumBedSlope, minimumWaterSurfaceSlope, maximumWaterSurfaceSlope, inputDivisor;
	bool ensureModificationAgainstOtherChangeRateModifiers;
	CombinerVariables::TypesOfGeneralFlowMethods correspondingGeneralFlowMethod;
	std::vector<TimeSeries> timeSeriesInputs;

	switch (typeOfChangeRateModifiers)
	{
	case CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins:

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumBedSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumBedSlopeErrorMessage = "The variable minimumBedSlope is needed for the construction of the ChangeRateModifiersType EnsureMinimumBedAndWaterSurfaceSlopeAtMargins.";
			throw(minimumBedSlopeErrorMessage);
		}
		else { minimumBedSlope = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumBedSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumBedSlopeErrorMessage = "The variable maximumBedSlope is needed for the construction of the ChangeRateModifiersType EnsureMinimumBedAndWaterSurfaceSlopeAtMargins.";
			throw(maximumBedSlopeErrorMessage);
		}
		else { maximumBedSlope = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumWaterSurfaceSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumWaterSurfaceSlopeErrorMessage = "The variable minimumWaterSurfaceSlope is needed for the construction of the ChangeRateModifiersType EnsureMinimumBedAndWaterSurfaceSlopeAtMargins.";
			throw(minimumWaterSurfaceSlopeErrorMessage);
		}
		else { minimumWaterSurfaceSlope = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("maximumWaterSurfaceSlope");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const maximumWaterSurfaceSlopeErrorMessage = "The variable maximumWaterSurfaceSlope is needed for the construction of the ChangeRateModifiersType EnsureMinimumBedAndWaterSurfaceSlopeAtMargins.";
			throw(maximumWaterSurfaceSlopeErrorMessage);
		}
		else { maximumWaterSurfaceSlope = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("ensureModificationAgainstOtherChangeRateModifiers");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const ensureModificationAgainstOtherChangeRateModifiersErrorMessage = "The variable ensureModificationAgainstOtherChangeRateModifiers is needed for the construction of the ChangeRateModifiersType EnsureMinimumBedAndWaterSurfaceSlopeAtMargins.";
			throw(ensureModificationAgainstOtherChangeRateModifiersErrorMessage);
		}
		else { ensureModificationAgainstOtherChangeRateModifiers = boolMapIterator->second.at(0); }

		stringMapIterator = constructionVariables.labelledStrings.find("correspondingGeneralFlowMethod");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const correspondingGeneralFlowMethodErrorMessage = "The variable correspondingGeneralFlowMethod is needed for the construction of the ChangeRateModifiersType EnsureMinimumBedAndWaterSurfaceSlopeAtMargins.";
			throw(correspondingGeneralFlowMethodErrorMessage);
		}
		else { correspondingGeneralFlowMethod = CombinerVariables::stringToTypeOfGeneralFlowMethods( (stringMapIterator->second.at(0)) ) ; }

		result = new AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins(minimumBedSlope,maximumBedSlope,minimumWaterSurfaceSlope,maximumWaterSurfaceSlope,ensureModificationAgainstOtherChangeRateModifiers,correspondingGeneralFlowMethod);
		break;

	case CombinerVariables::RecirculateWater:

		doubleMapIterator = constructionVariables.labelledDoubles.find("inputDivisor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const inputDivisorErrorMessage = "The variable inputDivisor is needed for the construction of the ChangeRateModifiersType RecirculateWater.";
			throw(inputDivisorErrorMessage);
		}
		else { inputDivisor = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("ensureModificationAgainstOtherChangeRateModifiers");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const ensureModificationAgainstOtherChangeRateModifiersErrorMessage = "The variable ensureModificationAgainstOtherChangeRateModifiers is needed for the construction of the ChangeRateModifiersType RecirculateWater.";
			throw(ensureModificationAgainstOtherChangeRateModifiersErrorMessage);
		}
		else { ensureModificationAgainstOtherChangeRateModifiers = boolMapIterator->second.at(0); }

		result = new RecirculateWater(inputDivisor,ensureModificationAgainstOtherChangeRateModifiers);
		break;


	case CombinerVariables::RecirculateSediment:

		doubleMapIterator = constructionVariables.labelledDoubles.find("inputDivisor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const inputDivisorErrorMessage = "The variable inputDivisor is needed for the construction of the ChangeRateModifiersType RecirculateSediment.";
			throw(inputDivisorErrorMessage);
		}
		else { inputDivisor = doubleMapIterator->second.at(0); }

		boolMapIterator = constructionVariables.labelledBools.find("ensureModificationAgainstOtherChangeRateModifiers");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const ensureModificationAgainstOtherChangeRateModifiersErrorMessage = "The variable ensureModificationAgainstOtherChangeRateModifiers is needed for the construction of the ChangeRateModifiersType RecirculateSediment.";
			throw(ensureModificationAgainstOtherChangeRateModifiersErrorMessage);
		}
		else { ensureModificationAgainstOtherChangeRateModifiers = boolMapIterator->second.at(0); }

		result = new RecirculateSediment(inputDivisor,ensureModificationAgainstOtherChangeRateModifiers);
		break;

	case CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated:

		stringMapIterator = constructionVariables.labelledStrings.find("correspondingGeneralFlowMethod");
		if(stringMapIterator == constructionVariables.labelledStrings.end() )
		{
			const char *const correspondingGeneralFlowMethodErrorMessage = "The variable correspondingGeneralFlowMethod is needed for the construction of the ChangeRateModifiersType InputPropertyTimeSeriesLinearlyInterpolated.";
			throw(correspondingGeneralFlowMethodErrorMessage);
		}
		else { correspondingGeneralFlowMethod = CombinerVariables::stringToTypeOfGeneralFlowMethods( (stringMapIterator->second.at(0)) ) ; }

		objectMapIterator = labelledObjects.find("timeSeriesInputs");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const timeSeriesInputsErrorMessage = "The variable timeSeriesInputs is needed for the construction of the ChangeRateModifiersType InputPropertyTimeSeriesLinearlyInterpolated.";
			throw(timeSeriesInputsErrorMessage);
		}
		else {
			timeSeriesInputs.reserve(objectMapIterator->second.size());
				for( std::vector<void*>::const_iterator currentTimeSeries = objectMapIterator->second.begin(); currentTimeSeries < objectMapIterator->second.end(); ++currentTimeSeries)
						{ timeSeriesInputs.push_back( *(static_cast<TimeSeries*>( *currentTimeSeries )) ); }
		}

		boolMapIterator = constructionVariables.labelledBools.find("ensureModificationAgainstOtherChangeRateModifiers");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const ensureModificationAgainstOtherChangeRateModifiersErrorMessage = "The variable ensureModificationAgainstOtherChangeRateModifiers is needed for the construction of the ChangeRateModifiersType InputPropertyTimeSeriesLinearlyInterpolated.";
			throw(ensureModificationAgainstOtherChangeRateModifiersErrorMessage);
		}
		else { ensureModificationAgainstOtherChangeRateModifiers = boolMapIterator->second.at(0); }

		result = ( new InputPropertyTimeSeriesLinearlyInterpolated(correspondingGeneralFlowMethod,timeSeriesInputs,ensureModificationAgainstOtherChangeRateModifiers,*(highestOrderStructuresPointers.overallMethods)) );
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Change Rate Modifiers Type";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

AdditionalRiverReachMethodType* SedFlowInterfaceRealisationBuilders::additionalRiverReachMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	try
	{
		if (constructionVariables.interfaceOrCombinerType != CombinerVariables::AdditionalRiverReachMethodType)
		{
			const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (additionalRiverReachMethodTypeBuilder)";
			throw(methodCompatibilityErrorMessage);
		}

		CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalRiverReachPropertyAndMethod = CombinerVariables::stringToTypeOfAdditionalRiverReachPropertyAndMethod(constructionVariables.realisationType);

		AdditionalRiverReachMethodType* result;

		std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
		std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

		double sternbergAbrasionCoefficient;
		std::vector<double> elapsedSeconds;
		std::vector<Grains> sedimentInputs;

		char* errorMessage;
		switch (typeOfAdditionalRiverReachPropertyAndMethod)
		{
		case CombinerVariables::PreventLocalGrainSizeDistributionChanges:
			result = new PreventLocalGrainSizeDistributionChanges();
			break;

		case CombinerVariables::InstantaneousSedimentInputs:
			doubleMapIterator = constructionVariables.labelledDoubles.find("elapsedSeconds");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const elapsedSecondsErrorMessage = "The variable elapsedSeconds is needed for the construction of the AdditionalRiverReachMethodType InstantaneousSedimentInputs.";
				throw(elapsedSecondsErrorMessage);
			}
			else { elapsedSeconds = doubleMapIterator->second; }

			objectMapIterator = labelledObjects.find("sedimentInputs");
			if(objectMapIterator == labelledObjects.end() )
			{
				const char *const sedimentInputsErrorMessage = "The variable sedimentInputs is needed for the construction of the AdditionalRiverReachMethodType InstantaneousSedimentInputs.";
				throw(sedimentInputsErrorMessage);
			}
			else {
				sedimentInputs.reserve(objectMapIterator->second.size());
					for( std::vector<void*>::const_iterator currentSedimentInput = objectMapIterator->second.begin(); currentSedimentInput < objectMapIterator->second.end(); ++currentSedimentInput)
							{ sedimentInputs.push_back( *(static_cast<Grains*>( *currentSedimentInput )) ); }
			}

			result = new InstantaneousSedimentInputs(elapsedSeconds,sedimentInputs);
			break;

		case CombinerVariables::SternbergAbrasionWithoutFining:
			doubleMapIterator = constructionVariables.labelledDoubles.find("sternbergAbrasionCoefficient");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const firstSternbergAbrasionCoefficientErrorMessage = "The variable sternbergAbrasionCoefficient is needed for the construction of the AdditionalRiverReachMethodType SternbergAbrasionWithoutFining.";
				throw(firstSternbergAbrasionCoefficientErrorMessage);
			}
			else { sternbergAbrasionCoefficient = doubleMapIterator->second.at(0); }

			result = new SternbergAbrasionWithoutFining(sternbergAbrasionCoefficient);
			break;

		case CombinerVariables::SternbergAbrasionIncludingFining:
			doubleMapIterator = constructionVariables.labelledDoubles.find("sternbergAbrasionCoefficient");
			if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
			{
				const char *const secondSternbergAbrasionCoefficientErrorMessage = "The variable sternbergAbrasionCoefficient is needed for the construction of the AdditionalRiverReachMethodType SternbergAbrasionIncludingFining.";
				throw(secondSternbergAbrasionCoefficientErrorMessage);
			}
			else { sternbergAbrasionCoefficient = doubleMapIterator->second.at(0); }

			result = new SternbergAbrasionIncludingFining(sternbergAbrasionCoefficient);
			break;

		case CombinerVariables::ScourChain:
			//TODO Less Important: Implement method
			errorMessage = "The build method for ScourChainMethods has not been implemented yet.";
			throw(errorMessage);
			break;

		default:
			errorMessage = "Invalid Additional River Reach Property and Method Type";
			throw (errorMessage);
		}

		return result;

	} catch (char* msg) {
		const char *const constMsg = msg;
		throw(constMsg);
	}
}

CalcTau* SedFlowInterfaceRealisationBuilders::calcTauBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcTau)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcTauBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfTauCalculationMethod typeOfTauCalculationMethod = CombinerVariables::stringToTypeOfTauCalculationMethod(constructionVariables.realisationType);

	CalcTau* result;

	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;

	bool correctionForBedloadWeightAtSteepSlopes;

	switch (typeOfTauCalculationMethod)
	{
	case CombinerVariables::EnergyslopeTauBasedOnFlowDepth:
		boolMapIterator = constructionVariables.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const correctionForBedloadWeightAtSteepSlopesErrorMessage = "The variable correctionForBedloadWeightAtSteepSlopes is needed for the construction of the tau calculation method EnergyslopeTauBasedOnFlowDepth.";
			throw(correctionForBedloadWeightAtSteepSlopesErrorMessage);
		}
		else { correctionForBedloadWeightAtSteepSlopes = boolMapIterator->second.at(0); }

		result = new EnergyslopeTauBasedOnFlowDepth(correctionForBedloadWeightAtSteepSlopes);
		break;

	case CombinerVariables::EnergyslopeTau:
		boolMapIterator = constructionVariables.labelledBools.find("correctionForBedloadWeightAtSteepSlopes");
		if(boolMapIterator == constructionVariables.labelledBools.end() )
		{
			const char *const correctionForBedloadWeightAtSteepSlopesErrorMessage = "The variable correctionForBedloadWeightAtSteepSlopes is needed for the construction of the tau calculation method EnergyslopeTau.";
			throw(correctionForBedloadWeightAtSteepSlopesErrorMessage);
		}
		else { correctionForBedloadWeightAtSteepSlopes = boolMapIterator->second.at(0); }

		result = new EnergyslopeTau(correctionForBedloadWeightAtSteepSlopes);
		break;

	case CombinerVariables::FlowVelocityTau:
		result = new FlowVelocityTau();
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of tau calculation method.";
		throw (invalidTypeErrorMessage);
	}

	return result;
}

CalcBedloadVelocity* SedFlowInterfaceRealisationBuilders::calcBedloadVelocityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcBedloadVelocity)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcBedloadVelocityBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfBedloadVelocityCalculationMethod typeOfBedloadVelocityCalculationMethod = CombinerVariables::stringToTypeOfBedloadVelocityCalculationMethod(constructionVariables.realisationType);

	CalcBedloadVelocity* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<void*> >::const_iterator objectMapIterator;

	double roughnessDiameterPercentile, roughnessDiameterFactor;
	EstimateThicknessOfMovingSedimentLayer* estimateThicknessOfMovingSedimentLayer;

	switch (typeOfBedloadVelocityCalculationMethod)
	{
	case CombinerVariables::JulienBounvilayRollingParticlesVelocity:

		doubleMapIterator = constructionVariables.labelledDoubles.find("roughnessDiameterPercentile");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const roughnessDiameterPercentileErrorMessage = "The variable roughnessDiameterPercentile is needed for the construction of the CalcBedloadVelocity JulienBounvilayRollingParticlesVelocity.";
			throw(roughnessDiameterPercentileErrorMessage);
		}
		else { roughnessDiameterPercentile = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("roughnessDiameterFactor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const roughnessDiameterFactorErrorMessage = "The variable roughnessDiameterFactor is needed for the construction of the CalcBedloadVelocity JulienBounvilayRollingParticlesVelocity.";
			throw(roughnessDiameterFactorErrorMessage);
		}
		else { roughnessDiameterFactor = doubleMapIterator->second.at(0); }

		result = new JulienBounvilayRollingParticlesVelocity(roughnessDiameterPercentile,roughnessDiameterFactor);
		break;

	case CombinerVariables::VelocityAsTransportRatePerUnitCrossSectionalArea:

		objectMapIterator = labelledObjects.find("estimateThicknessOfMovingSedimentLayer");
		if(objectMapIterator == labelledObjects.end() )
		{
			const char *const estimateThicknessOfMovingSedimentLayerErrorMessage = "The variable estimateThicknessOfMovingSedimentLayer is needed for the construction of the CalcBedloadVelocity VelocityAsTransportRatePerUnitCrossSectionalArea.";
			throw(estimateThicknessOfMovingSedimentLayerErrorMessage);
		}
		else { estimateThicknessOfMovingSedimentLayer = static_cast<EstimateThicknessOfMovingSedimentLayer*>( objectMapIterator->second.at(0) ); }

		result = new VelocityAsTransportRatePerUnitCrossSectionalArea(estimateThicknessOfMovingSedimentLayer);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of bedload velocity calculation method.";
		throw (invalidTypeErrorMessage);
	}
	return result;
}

EstimateThicknessOfMovingSedimentLayer* SedFlowInterfaceRealisationBuilders::estimateThicknessOfMovingSedimentLayerBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::EstimateThicknessOfMovingSedimentLayer)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (estimateThicknessOfMovingSedimentLayerBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfEstimationMethodForThicknessOfMovingSedimentLayer typeOfEstimationMethodForThicknessOfMovingSedimentLayer = CombinerVariables::stringToTypeOfEstimationMethodForThicknessOfMovingSedimentLayer(constructionVariables.realisationType);

	EstimateThicknessOfMovingSedimentLayer* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;

	double factor,minimumTransportRateForFractionToBeConsideredMoving,referenceDiameterPercentile,defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream,constantThickness;

	switch (typeOfEstimationMethodForThicknessOfMovingSedimentLayer)
	{
	case CombinerVariables::MultipleDiameterOfCoarsestGrainMoved:

		doubleMapIterator = constructionVariables.labelledDoubles.find("factor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const factorMultipleDiameterOfCoarsestGrainMovedErrorMessage = "The variable factor is needed for the construction of the EstimateThicknessOfMovingSedimentLayer MultipleDiameterOfCoarsestGrainMoved.";
			throw(factorMultipleDiameterOfCoarsestGrainMovedErrorMessage);
		}
		else { factor = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumTransportRateForFractionToBeConsideredMoving");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumTransportRateForFractionToBeConsideredMovingErrorMessage = "The variable minimumTransportRateForFractionToBeConsideredMoving is needed for the construction of the EstimateThicknessOfMovingSedimentLayer MultipleDiameterOfCoarsestGrainMoved.";
			throw(minimumTransportRateForFractionToBeConsideredMovingErrorMessage);
		}
		else { minimumTransportRateForFractionToBeConsideredMoving = doubleMapIterator->second.at(0); }

		result = new MultipleDiameterOfCoarsestGrainMoved(factor,minimumTransportRateForFractionToBeConsideredMoving);
		break;

	case CombinerVariables::MultipleReferenceGrainDiameter:

		doubleMapIterator = constructionVariables.labelledDoubles.find("factor");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const factorMultipleReferenceGrainDiameterErrorMessage = "The variable factor is needed for the construction of the EstimateThicknessOfMovingSedimentLayer MultipleReferenceGrainDiameter.";
			throw(factorMultipleReferenceGrainDiameterErrorMessage);
		}
		else { factor = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("referenceDiameterPercentile");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const referenceDiameterPercentileErrorMessage = "The variable referenceDiameterPercentile is needed for the construction of the EstimateThicknessOfMovingSedimentLayer MultipleReferenceGrainDiameter.";
			throw(referenceDiameterPercentileErrorMessage);
		}
		else { referenceDiameterPercentile = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const defaultThicknessForCasesWithNoAlluviumOrInputFromUpstreamErrorMessage = "The variable defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream is needed for the construction of the EstimateThicknessOfMovingSedimentLayer MultipleReferenceGrainDiameter.";
			throw(defaultThicknessForCasesWithNoAlluviumOrInputFromUpstreamErrorMessage);
		}
		else { defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream = doubleMapIterator->second.at(0); }

		result = new MultipleReferenceGrainDiameter(factor,referenceDiameterPercentile,defaultThicknessForCasesWithNoAlluviumOrInputFromUpstream);
		break;

	case CombinerVariables::ConstantThicknessOfMovingSedimentLayer:

		doubleMapIterator = constructionVariables.labelledDoubles.find("constantThickness");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const constantThicknessErrorMessage = "The variable constantThickness is needed for the construction of the EstimateThicknessOfMovingSedimentLayer ConstantThicknessOfMovingSedimentLayer.";
			throw(constantThicknessErrorMessage);
		}
		else { constantThickness = doubleMapIterator->second.at(0); }

		result = new ConstantThicknessOfMovingSedimentLayer(constantThickness);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of estimation method for thickness of moving sediment layer.";
		throw (invalidTypeErrorMessage);
	}
	return result;
}

CalcActiveWidth* SedFlowInterfaceRealisationBuilders::calcActiveWidthBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcActiveWidth)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcActiveWidthBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfActiveWidthCalculationMethod typeOfActiveWidthCalculationMethod = CombinerVariables::stringToTypeOfActiveWidthCalculationMethod(constructionVariables.realisationType);

	CalcActiveWidth* result;

	switch (typeOfActiveWidthCalculationMethod)
	{
	case CombinerVariables::SetActiveWidthEqualFlowWidth:
		result = new SetActiveWidthEqualFlowWidth();
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of active width calculation method.";
		throw (invalidTypeErrorMessage);
	}
	return result;
}

CalcThresholdForInitiationOfBedloadMotion* SedFlowInterfaceRealisationBuilders::calcThresholdForInitiationOfBedloadMotionBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcThresholdForInitiationOfBedloadMotion)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcThresholdForInitiationOfBedloadMotionBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfCalculationMethodForBedloadMotionInitiationThreshold typeOfCalculationMethodForBedloadMotionInitiationThreshold = CombinerVariables::stringToTypeOfCalculationMethodForBedloadMotionInitiationThreshold(constructionVariables.realisationType);

	CalcThresholdForInitiationOfBedloadMotion* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator;

	boolMapIterator = constructionVariables.labelledBools.find("correctionForBedloadWeightAtSteepCounterSlopes");
	bool correctionForBedloadWeightAtSteepCounterSlopes;
	if(boolMapIterator == constructionVariables.labelledBools.end() )
	{
		const char *const correctionForBedloadWeightAtSteepCounterSlopesErrorMessage = "The variable correctionForBedloadWeightAtSteepCounterSlopes is needed for the construction of a CalcThresholdForInitiationOfBedloadMotion.";
		throw(correctionForBedloadWeightAtSteepCounterSlopesErrorMessage);
	}
	else { correctionForBedloadWeightAtSteepCounterSlopes = boolMapIterator->second.at(0); }

	double minimumCriticalDimensionlessShearStress, constantThreshold;

	switch (typeOfCalculationMethodForBedloadMotionInitiationThreshold)
	{
	case CombinerVariables::LambEtAlCriticalTheta:

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumCriticalDimensionlessShearStress");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumCriticalDimensionlessShearStressErrorMessage = "The variable minimumCriticalDimensionlessShearStress is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion LambEtAlCriticalTheta.";
			throw(minimumCriticalDimensionlessShearStressErrorMessage);
		}
		else { minimumCriticalDimensionlessShearStress = doubleMapIterator->second.at(0); }

		result = new LambEtAlCriticalTheta(minimumCriticalDimensionlessShearStress,correctionForBedloadWeightAtSteepCounterSlopes);
		break;

	case CombinerVariables::ConstantThresholdForInitiationOfBedloadMotion:

		doubleMapIterator = constructionVariables.labelledDoubles.find("constantThreshold");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const constantThresholdErrorMessage = "The variable constantThreshold is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion ConstantThresholdForInitiationOfBedloadMotion.";
			throw(constantThresholdErrorMessage);
		}
		else { constantThreshold = doubleMapIterator->second.at(0); }

		result = new ConstantThresholdForInitiationOfBedloadMotion(constantThreshold,correctionForBedloadWeightAtSteepCounterSlopes);
		break;

	case CombinerVariables::StochasticThresholdForInitiationOfBedloadMotion:

	{
		std::map<std::string, std::vector<int> >::const_iterator intMapIterator;
		double minimumThresholdValue,miu,beta,weightForCurrent,weightForPrevious,weightForPrePrevious; int seed;
		std::vector<double> widthsForSpecialValues, miuSpecialValues, betaSpecialValues;

		doubleMapIterator = constructionVariables.labelledDoubles.find("minimumThresholdValue");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const minimumThresholdValueErrorMessage = "The variable minimumThresholdValue is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(minimumThresholdValueErrorMessage);
		}
		else { minimumThresholdValue = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("miu");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const miuErrorMessage = "The variable miu is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(miuErrorMessage);
		}
		else { miu = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("beta");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const betaErrorMessage = "The variable beta is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(betaErrorMessage);
		}
		else { beta = doubleMapIterator->second.at(0); }

		intMapIterator = constructionVariables.labelledInts.find("seed");
		if(intMapIterator == constructionVariables.labelledInts.end() )
		{
			const char *const seedErrorMessage = "The variable seed is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(seedErrorMessage);
		}
		else { seed = intMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("weightForCurrent");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const weightForCurrentErrorMessage = "The variable weightForCurrent is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(weightForCurrentErrorMessage);
		}
		else { weightForCurrent = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("weightForPrevious");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const weightForPreviousErrorMessage = "The variable weightForPrevious is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(weightForPreviousErrorMessage);
		}
		else { weightForPrevious = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("weightForPrePrevious");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const weightForPrePreviousErrorMessage = "The variable weightForPrePrevious is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(weightForPrePreviousErrorMessage);
		}
		else { weightForPrePrevious = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("widthsForSpecialValues");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const widthsForSpecialValuesErrorMessage = "The variable widthsForSpecialValues is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(widthsForSpecialValuesErrorMessage);
		}
		else { widthsForSpecialValues = doubleMapIterator->second; }

		doubleMapIterator = constructionVariables.labelledDoubles.find("miuSpecialValues");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const miuSpecialValuesErrorMessage = "The variable miuSpecialValues is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(miuSpecialValuesErrorMessage);
		}
		else { miuSpecialValues = doubleMapIterator->second; }

		doubleMapIterator = constructionVariables.labelledDoubles.find("betaSpecialValues");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const betaSpecialValuesErrorMessage = "The variable betaSpecialValues is needed for the construction of the CalcThresholdForInitiationOfBedloadMotion StochasticThresholdForInitiationOfBedloadMotion.";
			throw(betaSpecialValuesErrorMessage);
		}
		else { betaSpecialValues = doubleMapIterator->second; }

		result = new StochasticThresholdForInitiationOfBedloadMotion(minimumThresholdValue,miu,beta,seed,weightForCurrent,weightForPrevious,weightForPrePrevious,correctionForBedloadWeightAtSteepCounterSlopes,widthsForSpecialValues,miuSpecialValues,betaSpecialValues);
		break;
	}

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of calculation method for bedload motion initiation threshold.";
		throw (invalidTypeErrorMessage);
	}
	return result;
}

CalcHidingFactors* SedFlowInterfaceRealisationBuilders::calcHidingFactorsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map<  std::string, std::vector<void*> > labelledObjects)
{
	if (constructionVariables.interfaceOrCombinerType != CombinerVariables::CalcHidingFactors)
	{
		const char *const methodCompatibilityErrorMessage = "Application of not matching interface builder. (calcHidingFactorsBuilder)";
		throw(methodCompatibilityErrorMessage);
	}

	CombinerVariables::TypesOfHidingFactorsCalculationMethod typeOfHidingFactorsCalculationMethod = CombinerVariables::stringToTypeOfHidingFactorsCalculationMethod(constructionVariables.realisationType);

	CalcHidingFactors* result;

	std::map<std::string, std::vector<double> >::const_iterator doubleMapIterator;

	double referenceDiameterPercentile, exponent;

	switch (typeOfHidingFactorsCalculationMethod)
	{
	case CombinerVariables::PowerLawHidingFunction:

		doubleMapIterator = constructionVariables.labelledDoubles.find("referenceDiameterPercentile");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const referenceDiameterPercentileErrorMessage = "The variable referenceDiameterPercentile is needed for the construction of the CalcHidingFactors PowerLawHidingFunction.";
			throw(referenceDiameterPercentileErrorMessage);
		}
		else { referenceDiameterPercentile = doubleMapIterator->second.at(0); }

		doubleMapIterator = constructionVariables.labelledDoubles.find("exponent");
		if(doubleMapIterator == constructionVariables.labelledDoubles.end() )
		{
			const char *const exponentErrorMessage = "The variable exponent is needed for the construction of the CalcHidingFactors PowerLawHidingFunction.";
			throw(exponentErrorMessage);
		}
		else { exponent = doubleMapIterator->second.at(0); }

		result = new PowerLawHidingFunction(referenceDiameterPercentile,exponent);
		break;

	case CombinerVariables::WilcockCroweHidingFunction:

		result = new WilcockCroweHidingFunction();
		break;

	case CombinerVariables::NoHiding:

		result = new NoHiding();
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid type of hiding factors calculation method.";
		throw (invalidTypeErrorMessage);
	}
	return result;
}


}
