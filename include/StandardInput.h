/*
 * StandardInput.h
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

#ifndef STANDARDINPUT_H_
#define STANDARDINPUT_H_

#include "UserInputReader.h"

#include <utility>
#include <map>
#include <limits>
#include "pugixml.h"

namespace SedFlow {

class RiverBranchForStandardInput {
public:
	int branchID;
	int downstreamBranchID;
	int numberOfCells;
	int topmostCellID;
	double topmostKilometrage;
	double initialDischarge;
	bool upstreamMargin;
	bool downstreamMargin;
	inline bool isMargin()const { return ( upstreamMargin || downstreamMargin ); }

	RiverBranchForStandardInput():
		branchID(0),
		downstreamBranchID(0),
		numberOfCells(0),
		topmostCellID(0),
		topmostKilometrage(0),
		initialDischarge(0),
		upstreamMargin(false),
		downstreamMargin(false)
	{}
	RiverBranchForStandardInput(int branchID, int downstreamBranchID):
		branchID(branchID),
		downstreamBranchID(downstreamBranchID),
		numberOfCells(0),
		topmostCellID(0),
		topmostKilometrage(0),
		initialDischarge(0),
		upstreamMargin(false),
		downstreamMargin(false)
	{}
	~RiverBranchForStandardInput(){}

	static int getTopmostCellIDofCertainBranch(const std::vector<RiverBranchForStandardInput>& branches, int branchID)
	{
		int result = std::numeric_limits<int>::max();
		for(std::vector<RiverBranchForStandardInput>::const_iterator currentBranch = branches.begin(); currentBranch < branches.end(); ++currentBranch)
		{
			if( currentBranch->branchID == branchID ) { result = currentBranch->topmostCellID; }
		}
		return result;
	}

	static double getTopmostKilometrageofCertainBranch(const std::vector<RiverBranchForStandardInput>& branches, int branchID)
	{
		double result = 0.0;
		for(std::vector<RiverBranchForStandardInput>::const_iterator currentBranch = branches.begin(); currentBranch < branches.end(); ++currentBranch)
		{
			if( currentBranch->branchID == branchID ) { result = currentBranch->topmostKilometrage; }
		}
		return result;
	}

	friend bool operator < (const RiverBranchForStandardInput& riverBranch1, const RiverBranchForStandardInput& riverBranch2)
	{
		return (riverBranch1.branchID < riverBranch2.branchID);
	}
};

class RiverSystemInformation {
public:
	std::vector<RiverBranchForStandardInput> riverBranches;
	std::map<std::pair<int,std::string>,int> mapFromBranchIDAndKilometrageToUserCellID;
	std::vector<CombinerVariables::TypesOfGrains> usedTypesOfGrains;
	std::vector<double> usedFractionalGrainDiameters;
};

class StandardOutputCharacteristics {
public:
	bool forVisualInterpretation;
	std::vector<double> explicitTimesForOutput;
	double outputInterval;
	int precisionForOutput;
	std::vector<int> reachIDsForOutput;
	bool writeLineEachTimeStep;
	bool outputTimeStepLength;
	bool outputInitialValues;
	bool printUpstreamMargins;
	bool printDownstreamMargin;
	bool useSecondaryOutputInterval;
	int referenceCellUserCellID;
	std::string referenceProperty;
	double thresholdToBeExceeded;
	double secondaryOutputInterval;
};


class StandardInput: public UserInputReader {
private:
	ConstructionVariables createOverallParameters(pugi::xml_node rootNode, std::string path)const;
	ConstructionVariables createOverallMethods(pugi::xml_node rootNode)const;
	std::pair<ConstructionVariables, RiverSystemInformation> createRiverSystemPropertiesAndRiverSystemInformation(pugi::xml_node rootNode, std::string path, const ConstructionVariables& overallParameters, const ConstructionVariables& overallMethods)const;
	ConstructionVariables createRiverSystemMethods(pugi::xml_node rootNode, std::string path, const RiverSystemInformation& riverSystemInformation)const;
	ConstructionVariables createOutputMethods(pugi::xml_node rootNode, std::string path, const RiverSystemInformation& riverSystemInformation)const;

	static ConstructionVariables createReturnBedslope();
	static ConstructionVariables createSimpleDownstreamTwoCellGradient(CombinerVariables::TypesOfRegularRiverReachProperties propertyOfInterest);
	static bool addCalcGradientToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, bool isSedimentEnergySlope, const ConstructionVariables& flowResistance, CombinerVariables::TypesOfFlowMethods typeOfWaterFlowMethods, const ConstructionVariables& defaultValue);
	static bool addFlowResistanceToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, CombinerVariables::TypesOfFlowMethods typeOfWaterFlowMethods);
	static ConstructionVariables createWaterFlowMethods (pugi::xml_node rootNode);
	static std::pair<ConstructionVariables,bool> createBedLoadCapacityCalculationMethodWithBoolNonFractional (pugi::xml_node rootNode);
	static bool addAdditionalRiverReachMethodsTypes(std::vector<ConstructionVariables>& targetConstitutingAdditionalMethodTypes, pugi::xml_node riverSystemMethodsNode);
	static bool addCalcThresholdForInitiationOfBedloadMotionToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name);
	static bool addCalcHidingFactorsToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name);
	static ConstructionVariables createRiverReachMethodsWithCertainCellID (pugi::xml_node riverSystemMethodsNode, int cellID, bool nonFractional, const std::map<int,ConstructionVariables>& mapFromCellIDToInstantaneousSedimentInputs);
	static std::vector<ConstructionVariables> createDischargeTimeSeriesFromSpreadsheets (std::string path, const RiverSystemInformation& riverSystemInformation);
	static std::vector<ConstructionVariables> createSedimentInputTimeSeriesFromSpreadsheets (std::string path, const RiverSystemInformation& riverSystemInformation, bool inputUpperBoundaryInsteadOfMeanGrainDiameter, bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters, double lowerDiameterBoundaryForFinestFractionInCM, double poreVolumeFraction);
	static bool addCalcTauToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, CombinerVariables::TypesOfTauCalculationMethod defaultValue);
	static bool addCalcBedloadVelocityToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name);
	static bool addEstimateThicknessOfMovingSedimentLayerToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name);
	static bool addCalcActiveWidthToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, CombinerVariables::TypesOfActiveWidthCalculationMethod defaultValue);
	static ConstructionVariables createInfinitelyDeepRectangularRiverReachProperties(int cellID, int downstreamCellID, double elevation, double channelWidth, double length, double bedrockRoughnessEquivalentRepresentativeGrainDiameter, bool sillOccurence, double sillTopEdgeElevation, double initialDischarge, double alluviumThickness, bool upstreamOfSillsWedgeShapedInsteadOfParallelUpdate, ConstructionVariables armourGrains, ConstructionVariables strataGrains, ConstructionVariables strataSorting, ConstructionVariables sillProperties, bool useInitialGrainSizesForConstantLayerThickness, const std::vector<double>& fractionalGrainDiameters, int numberOfLayers);
	static std::pair<std::vector<double>,ConstructionVariables> createFractionalGrainDiametersAndGrainsConstructionVariablesFromSpreadsheet(const std::string& fileName, bool inputUpperBoundaryInsteadOfMeanGrainDiameter, bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters, double lowerDiameterBoundaryForFinestFractionInCM, const std::vector<CombinerVariables::TypesOfGrains>& expectedTypesOfGrains);
	static ConstructionVariables createSingleRegularOutputMethod(pugi::xml_node rootNode, const std::string& outputPath, const std::string& defaultFileName, const RiverSystemInformation& riverSystemInformation, const StandardOutputCharacteristics& standardOutputCharacteristics, bool useDefaultPropertyForOutput, const std::string& defaultPropertyForOutput);
	static ConstructionVariables createSingleOutputAccumulatedBedloadTransport(pugi::xml_node rootNode, const std::string& outputPath, const std::string& defaultFileName, const RiverSystemInformation& riverSystemInformation, const StandardOutputCharacteristics& standardOutputCharacteristics);
	static std::pair<ConstructionVariables,std::pair<bool,int> > createStrataSortingWITHBoolUseInitialGrainSizesForConstantLayerThicknessANDNumberOfLayers (pugi::xml_node riverSystemMethodsNode, bool thicknessInputsIncludingPoreVolume, double poreVolumeFraction);
	static std::map<int,ConstructionVariables> createMapFromCellIDToInstantaneousSedimentInputsFromSpreadsheet (std::string path, const RiverSystemInformation& riverSystemInformation, bool inputUpperBoundaryInsteadOfMeanGrainDiameter, bool useArithmeticMeanInsteadOfGeometricMeanForFractionGrainDiameters, double lowerDiameterBoundaryForFinestFractionInCM, double poreVolumeFraction);

	static bool addDoubleToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, double defaultValue);
	static bool addIntToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, int defaultValue);
	static bool addBoolToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, bool defaultValue);
	static bool addStringToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, const std::string& defaultValue);
	static bool addStringWithInternalBlanksToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, const std::string& defaultValue);

	static bool addDoubleVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, std::vector<double> defaultValue);
	static bool addIntVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, std::vector<int> defaultValue);
	static bool addReachIDVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name, const RiverSystemInformation& riverSystemInformation);
	static bool addStringVectorToConstructionVariables(ConstructionVariables& target, pugi::xml_node rootNode, std::string name);

	static bool checkIfIntVectorContainsValue(const std::vector<int>& vector, int value);

	static std::vector<int> extractUserCellIDsFromRiverSystemProperties(const ConstructionVariables& riverSystemProperties);

public:
	StandardInput(){}
	virtual ~StandardInput(){}

	ConstructionVariables createConstructionVariables()const;

	ConstructionVariables convertUserInputXMLIntoConstructionVariables (pugi::xml_document& userInputXMLDocument, const std::string& folderContainingXML) const;
};

}

#endif /* STANDARDINPUT_H_ */
