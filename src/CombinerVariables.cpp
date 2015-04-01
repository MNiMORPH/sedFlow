/*
 * CombinerVariables.cpp
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

#include "CombinerVariables.h"

#include <string>
#include <iostream>
#include <cstdlib>

namespace SedFlow {

const std::string CombinerVariables::sedflowStringPrefix = "SEDFLOW_";

void CombinerVariables::convertInternalStringIntoExternalString(std::string& internalString)
{ internalString.insert(0,sedflowStringPrefix); }

void CombinerVariables::convertExternalStringIntoInternalString(std::string& externalString)
{ externalString.erase(0,sedflowStringPrefix.size()); }

std::string CombinerVariables::getInternalStringCorrespondingToExternalString(std::string externalString)
{
	convertExternalStringIntoInternalString(externalString);
	return externalString;
}

std::string CombinerVariables::getExternalStringCorrespondingToInternalString(std::string internalString)
{
	convertInternalStringIntoExternalString(internalString);
	return internalString;
}


std::map< std::string, CombinerVariables::TypesOfUserInputReaders> CombinerVariables::createMapForTypesOfUserInputReaders()
{
	std::map< std::string, TypesOfUserInputReaders> result;
	result["VerbatimTranslationFromXMLToConstructionVariables"] = CombinerVariables::VerbatimTranslationFromXMLToConstructionVariables;
	result["StandardInput"] = CombinerVariables::StandardInput;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfUserInputReaders> CombinerVariables::mapForTypesOfUserInputReaders(CombinerVariables::createMapForTypesOfUserInputReaders());

std::map< std::string, CombinerVariables::TypesOfCombinersAndInterfaces> CombinerVariables::createMapForTypesOfCombinersAndInterfaces()
{
	std::map< std::string, TypesOfCombinersAndInterfaces>  result;
	result["SedFlow"] = CombinerVariables::SedFlow;
	result["NumericRootFinder"] = CombinerVariables::NumericRootFinder;
	result["OutputMethods"] = CombinerVariables::OutputMethods;
	result["OutputMethodType"] = CombinerVariables::OutputMethodType;
	result["UserInputReader"] = CombinerVariables::UserInputReader;
	result["OverallParameters"] = CombinerVariables::OverallParameters;
	result["RiverSystemProperties"] = CombinerVariables::RiverSystemProperties;
	result["AdditionalRiverSystemProperties"] = CombinerVariables::AdditionalRiverSystemProperties;
	result["AdditionalRiverSystemPropertyType"] = CombinerVariables::AdditionalRiverSystemPropertyType;
	result["RegularRiverSystemProperties"] = CombinerVariables::RegularRiverSystemProperties;
	result["RiverReachProperties"] = CombinerVariables::RiverReachProperties;
	result["AdditionalRiverReachProperties"] = CombinerVariables::AdditionalRiverReachProperties;
	result["AdditionalRiverReachPropertyType"] = CombinerVariables::AdditionalRiverReachPropertyType;
	result["RegularRiverReachProperties"] = CombinerVariables::RegularRiverReachProperties;
	result["Grains"] = CombinerVariables::Grains;
	result["GrainType"] = CombinerVariables::GrainType;
	result["GeometricalChannelBehaviour"] = CombinerVariables::GeometricalChannelBehaviour;
	result["ChannelGeometry"] = CombinerVariables::ChannelGeometry;
	result["StrataSorting"] = CombinerVariables::StrataSorting;
	result["SillProperties"] = CombinerVariables::SillProperties;
	result["OverallMethods"] = CombinerVariables::OverallMethods;
	result["CalcGradient"] = CombinerVariables::CalcGradient;
	result["FlowResistance"] = CombinerVariables::FlowResistance;
	result["RiverSystemMethods"] = CombinerVariables::RiverSystemMethods;
	result["AdditionalRiverSystemMethods"] = CombinerVariables::AdditionalRiverSystemMethods;
	result["AdditionalRiverSystemMethodType"] = CombinerVariables::AdditionalRiverSystemMethodType;
	result["RegularRiverSystemMethods"] = CombinerVariables::RegularRiverSystemMethods;
	result["FlowMethods"] = CombinerVariables::FlowMethods;
	result["FlowTypeMethods"] = CombinerVariables::FlowTypeMethods;
	result["SedimentFlowMethods"] = CombinerVariables::SedimentFlowMethods;
	result["SedimentFlowTypeMethods"] = CombinerVariables::SedimentFlowTypeMethods;
	result["CalcBedloadCapacity"] = CombinerVariables::CalcBedloadCapacity;
	result["ChangeRateModifiers"] = CombinerVariables::ChangeRateModifiers;
	result["ChangeRateModifiersForSingleFlowMethod"] = CombinerVariables::ChangeRateModifiersForSingleFlowMethod;
	result["ChangeRateModifiersType"] = CombinerVariables::ChangeRateModifiersType;
	result["RiverReachMethods"] = CombinerVariables::RiverReachMethods;
	result["AdditionalRiverReachMethods"] = CombinerVariables::AdditionalRiverReachMethods;
	result["AdditionalRiverReachMethodType"] = CombinerVariables::AdditionalRiverReachMethodType;
	result["RegularRiverReachMethods"] = CombinerVariables::RegularRiverReachMethods;
	result["CalcTau"] = CombinerVariables::CalcTau;
	result["CalcBedloadVelocity"] = CombinerVariables::CalcBedloadVelocity;
	result["EstimateThicknessOfMovingSedimentLayer"] = CombinerVariables::EstimateThicknessOfMovingSedimentLayer;
	result["CalcActiveWidth"] = CombinerVariables::CalcActiveWidth;
	result["CalcThresholdForInitiationOfBedloadMotion"] = CombinerVariables::CalcThresholdForInitiationOfBedloadMotion;
	result["CalcHidingFactors"] = CombinerVariables::CalcHidingFactors;
	result["TimeSeriesEntry"] = CombinerVariables::TimeSeriesEntry;
	result["TimeSeries"] = CombinerVariables::TimeSeries;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfCombinersAndInterfaces> CombinerVariables::mapForTypesOfCombinersAndInterfaces(CombinerVariables::createMapForTypesOfCombinersAndInterfaces());

std::map< std::string, CombinerVariables::TypesOfNumericRootFinder> CombinerVariables::createMapForTypesOfNumericRootFinder()
{
	std::map< std::string, TypesOfNumericRootFinder> result;
	result["BisectionMethod"] = CombinerVariables::BisectionMethod;
	result["SecantMethod"] = CombinerVariables::SecantMethod;
	result["FalsePositionMethod"] = CombinerVariables::FalsePositionMethod;
	result["RiddersMethod"] = CombinerVariables::RiddersMethod;
	result["BrentMethod"] = CombinerVariables::BrentMethod;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfNumericRootFinder> CombinerVariables::mapForTypesOfNumericRootFinder(CombinerVariables::createMapForTypesOfNumericRootFinder());

std::map< std::string, CombinerVariables::TypesOfGrains> CombinerVariables::createMapForTypesOfGrains()
{
	std::map< std::string, TypesOfGrains> result;
	result["NormalGrains"] = CombinerVariables::NormalGrains;
	result["RelativeAbundance"] = CombinerVariables::NormalGrains;
	result["TracerGrains"] = CombinerVariables::TracerGrains;
	result["FishEggs"] = CombinerVariables::FishEggs;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfGrains> CombinerVariables::mapForTypesOfGrains(CombinerVariables::createMapForTypesOfGrains());

std::map< std::string, CombinerVariables::TypesOfRegularRiverReachProperties> CombinerVariables::createMapForTypesOfRegularRiverReachProperties()
{
	std::map< std::string, CombinerVariables::TypesOfRegularRiverReachProperties> result;
	result["elevation"] = CombinerVariables::elevation;
	result["length"] = CombinerVariables::length;
	result["sillOccurence"] = CombinerVariables::sillOccurence;
	result["sillTopEdgeElevation"] = CombinerVariables::sillTopEdgeElevation;
	result["discharge"] = CombinerVariables::discharge;
	result["flowVelocity"] = CombinerVariables::flowVelocity;
	result["maximumWaterdepth"] = CombinerVariables::maximumWaterdepth;
	result["waterLevel"] = CombinerVariables::waterLevel;
	result["hydraulicHead"] = CombinerVariables::hydraulicHead;
	result["kineticComponentOfHydraulicHead"] = CombinerVariables::kineticComponentOfHydraulicHead;
	result["bedShearStress"] = CombinerVariables::bedShearStress;
	result["activeWidth"] = CombinerVariables::activeWidth;
	result["strataPerUnitBedSurface"] = CombinerVariables::strataPerUnitBedSurface;
	result["strataPerUnitBedSurfaceIncludingPoreVolume"] = CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume;
	result["alluviumThicknessIncludingPoreVolume"] = CombinerVariables::alluviumThicknessIncludingPoreVolume;
	result["bedrockElevation"] = CombinerVariables::bedrockElevation;
	result["activeLayerPerUnitBedSurface"] = CombinerVariables::activeLayerPerUnitBedSurface;
	result["activeLayerPerUnitBedSurfaceIncludingPoreVolume"] = CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume;
	result["activeLayerPerUnitBedSurfaceOverallVolume"] = CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume;
	result["activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume"] = CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume;
	result["activeLayerPerUnitBedSurfaceMedianGrainDiameter"] = CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter;
	result["activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter"] = CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter;
	result["activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter"] = CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter;
	result["activeLayerPerUnitBedSurfaceD20"] = CombinerVariables::activeLayerPerUnitBedSurfaceD20;
	result["activeLayerPerUnitBedSurfaceD30"] = CombinerVariables::activeLayerPerUnitBedSurfaceD30;
	result["activeLayerPerUnitBedSurfaceD50"] = CombinerVariables::activeLayerPerUnitBedSurfaceD50;
	result["activeLayerPerUnitBedSurfaceD84"] = CombinerVariables::activeLayerPerUnitBedSurfaceD84;
	result["activeLayerPerUnitBedSurfaceD90"] = CombinerVariables::activeLayerPerUnitBedSurfaceD90;
	result["bedslope"] = CombinerVariables::bedslope;
	result["waterEnergyslope"] = CombinerVariables::waterEnergyslope;
	result["sedimentEnergyslope"] = CombinerVariables::sedimentEnergyslope;
	result["unreducedSedimentEnergyslope"] = CombinerVariables::unreducedSedimentEnergyslope;
	result["waterVolumeChangeRate"] = CombinerVariables::waterVolumeChangeRate;
	result["waterVolumeChange"] = CombinerVariables::waterVolumeChange;
	result["erosionRate"] = CombinerVariables::erosionRate;
	result["erosionRateIncludingPoreVolume"] = CombinerVariables::erosionRateIncludingPoreVolume;
	result["erosionRateOverallVolume"] = CombinerVariables::erosionRateOverallVolume;
	result["erosionRateOverallVolumeIncludingPoreVolume"] = CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume;
	result["erosionRateMedianGrainDiameter"] = CombinerVariables::erosionRateMedianGrainDiameter;
	result["erosionRateArithmeticMeanGrainDiameter"] = CombinerVariables::erosionRateArithmeticMeanGrainDiameter;
	result["erosionRateGeometricMeanGrainDiameter"] = CombinerVariables::erosionRateGeometricMeanGrainDiameter;
	result["erosionRateD20"] = CombinerVariables::erosionRateD20;
	result["erosionRateD30"] = CombinerVariables::erosionRateD30;
	result["erosionRateD50"] = CombinerVariables::erosionRateD50;
	result["erosionRateD84"] = CombinerVariables::erosionRateD84;
	result["erosionRateD90"] = CombinerVariables::erosionRateD90;
	result["depositionRate"] = CombinerVariables::depositionRate;
	result["depositionRateIncludingPoreVolume"] = CombinerVariables::depositionRateIncludingPoreVolume;
	result["depositionRateOverallVolume"] = CombinerVariables::depositionRateOverallVolume;
	result["depositionRateOverallVolumeIncludingPoreVolume"] = CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume;
	result["depositionRateMedianGrainDiameter"] = CombinerVariables::depositionRateMedianGrainDiameter;
	result["depositionRateArithmeticMeanGrainDiameter"] = CombinerVariables::depositionRateArithmeticMeanGrainDiameter;
	result["depositionRateGeometricMeanGrainDiameter"] = CombinerVariables::depositionRateGeometricMeanGrainDiameter;
	result["depositionRateD20"] = CombinerVariables::depositionRateD20;
	result["depositionRateD30"] = CombinerVariables::depositionRateD30;
	result["depositionRateD50"] = CombinerVariables::depositionRateD50;
	result["depositionRateD84"] = CombinerVariables::depositionRateD84;
	result["depositionRateD90"] = CombinerVariables::depositionRateD90;
	result["erosion"] = CombinerVariables::erosion;
	result["erosionIncludingPoreVolume"] = CombinerVariables::erosionIncludingPoreVolume;
	result["erosionOverallVolume"] = CombinerVariables::erosionOverallVolume;
	result["erosionOverallVolumeIncludingPoreVolume"] = CombinerVariables::erosionOverallVolumeIncludingPoreVolume;
	result["erosionMedianGrainDiameter"] = CombinerVariables::erosionMedianGrainDiameter;
	result["erosionArithmeticMeanGrainDiameter"] = CombinerVariables::erosionArithmeticMeanGrainDiameter;
	result["erosionGeometricMeanGrainDiameter"] = CombinerVariables::erosionGeometricMeanGrainDiameter;
	result["erosionD20"] = CombinerVariables::erosionD20;
	result["erosionD30"] = CombinerVariables::erosionD30;
	result["erosionD50"] = CombinerVariables::erosionD50;
	result["erosionD84"] = CombinerVariables::erosionD84;
	result["erosionD90"] = CombinerVariables::erosionD90;
	result["deposition"] = CombinerVariables::deposition;
	result["depositionIncludingPoreVolume"] = CombinerVariables::depositionIncludingPoreVolume;
	result["depositionOverallVolume"] = CombinerVariables::depositionOverallVolume;
	result["depositionOverallVolumeIncludingPoreVolume"] = CombinerVariables::depositionOverallVolumeIncludingPoreVolume;
	result["depositionMedianGrainDiameter"] = CombinerVariables::depositionMedianGrainDiameter;
	result["depositionArithmeticMeanGrainDiameter"] = CombinerVariables::depositionArithmeticMeanGrainDiameter;
	result["depositionGeometricMeanGrainDiameter"] = CombinerVariables::depositionGeometricMeanGrainDiameter;
	result["depositionD20"] = CombinerVariables::depositionD20;
	result["depositionD30"] = CombinerVariables::depositionD30;
	result["depositionD50"] = CombinerVariables::depositionD50;
	result["depositionD84"] = CombinerVariables::depositionD84;
	result["depositionD90"] = CombinerVariables::depositionD90;
	result["erosionPerUnitBedSurface"] = CombinerVariables::erosionPerUnitBedSurface;
	result["erosionPerUnitBedSurfaceIncludingPoreVolume"] = CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume;
	result["erosionPerUnitBedSurfaceOverallVolume"] = CombinerVariables::erosionPerUnitBedSurfaceOverallVolume;
	result["erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume"] = CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume;
	result["erosionPerUnitBedSurfaceMedianGrainDiameter"] = CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter;
	result["erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter"] = CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter;
	result["erosionPerUnitBedSurfaceGeometricMeanGrainDiameter"] = CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter;
	result["erosionPerUnitBedSurfaceD20"] = CombinerVariables::erosionPerUnitBedSurfaceD20;
	result["erosionPerUnitBedSurfaceD30"] = CombinerVariables::erosionPerUnitBedSurfaceD30;
	result["erosionPerUnitBedSurfaceD50"] = CombinerVariables::erosionPerUnitBedSurfaceD50;
	result["erosionPerUnitBedSurfaceD84"] = CombinerVariables::erosionPerUnitBedSurfaceD84;
	result["erosionPerUnitBedSurfaceD90"] = CombinerVariables::erosionPerUnitBedSurfaceD90;
	result["depositionPerUnitBedSurface"] = CombinerVariables::depositionPerUnitBedSurface;
	result["depositionPerUnitBedSurfaceIncludingPoreVolume"] = CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume;
	result["depositionPerUnitBedSurfaceOverallVolume"] = CombinerVariables::depositionPerUnitBedSurfaceOverallVolume;
	result["depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume"] = CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume;
	result["depositionPerUnitBedSurfaceMedianGrainDiameter"] = CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter;
	result["depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter"] = CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter;
	result["depositionPerUnitBedSurfaceGeometricMeanGrainDiameter"] = CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter;
	result["depositionPerUnitBedSurfaceD20"] = CombinerVariables::depositionPerUnitBedSurfaceD20;
	result["depositionPerUnitBedSurfaceD30"] = CombinerVariables::depositionPerUnitBedSurfaceD30;
	result["depositionPerUnitBedSurfaceD50"] = CombinerVariables::depositionPerUnitBedSurfaceD50;
	result["depositionPerUnitBedSurfaceD84"] = CombinerVariables::depositionPerUnitBedSurfaceD84;
	result["depositionPerUnitBedSurfaceD90"] = CombinerVariables::depositionPerUnitBedSurfaceD90;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfRegularRiverReachProperties> CombinerVariables::mapForTypesOfRegularRiverReachProperties(CombinerVariables::createMapForTypesOfRegularRiverReachProperties());

std::map< std::string, CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod> CombinerVariables::createMapForTypesOfAdditionalRiverReachPropertyAndMethod()
{
	std::map< std::string, TypesOfAdditionalRiverReachPropertyAndMethod> result;
	result["BedrockRoughnessEquivalentRepresentativeGrainDiameter"] = CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter;
	result["PreventLocalGrainSizeDistributionChanges"] = CombinerVariables::PreventLocalGrainSizeDistributionChanges;
	result["InstantaneousSedimentInputs"] = CombinerVariables::InstantaneousSedimentInputs;
	result["SternbergAbrasionWithoutFining"] = CombinerVariables::SternbergAbrasionWithoutFining;
	result["SternbergAbrasionIncludingFining"] = CombinerVariables::SternbergAbrasionIncludingFining;
	result["ScourChain"] = CombinerVariables::ScourChain;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod> CombinerVariables::mapForTypesOfAdditionalRiverReachPropertyAndMethod(CombinerVariables::createMapForTypesOfAdditionalRiverReachPropertyAndMethod());

std::map< std::string, CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod> CombinerVariables::createMapForTypesOfAdditionalRiverSystemPropertyAndMethod()
{
	std::map< std::string, TypesOfAdditionalRiverSystemPropertyAndMethod> result;
	//result["???"] = CombinerVariables::???;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod> CombinerVariables::mapForTypesOfAdditionalRiverSystemPropertyAndMethod(CombinerVariables::createMapForTypesOfAdditionalRiverSystemPropertyAndMethod());

std::map< std::string, CombinerVariables::TypesOfChangeRates> CombinerVariables::createMapForTypesOfChangeRates()
{
	std::map< std::string, TypesOfChangeRates> result;
	result["WaterdepthChange"] = CombinerVariables::WaterdepthChange;
	result["RelativeTwoCellBedSlopeChange"] = CombinerVariables::RelativeTwoCellBedSlopeChange;
	result["EnergySlopeChange"] = CombinerVariables::EnergySlopeChange;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfChangeRates> CombinerVariables::mapForTypesOfChangeRates(CombinerVariables::createMapForTypesOfChangeRates());

std::map< std::string, CombinerVariables::TypesOfGeneralFlowMethods> CombinerVariables::createMapForTypesOfGeneralFlowMethods()
{
	std::map< std::string, TypesOfGeneralFlowMethods> result;
	result["WaterFlowMethodsInGeneral"] = CombinerVariables::WaterFlowMethodsInGeneral;
	result["SedimentFlowMethodsInGeneral"] = CombinerVariables::SedimentFlowMethodsInGeneral;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfGeneralFlowMethods> CombinerVariables::mapForTypesOfGeneralFlowMethods(CombinerVariables::createMapForTypesOfGeneralFlowMethods());

std::map< std::string, CombinerVariables::TypesOfFlowMethods> CombinerVariables::createMapForTypesOfFlowMethods()
{
	std::map< std::string, TypesOfFlowMethods> result;
	result["ImplicitKinematicWave"] = CombinerVariables::ImplicitKinematicWave;
	result["ExplicitKinematicWave"] = CombinerVariables::ExplicitKinematicWave;
	result["UniformDischarge"] = CombinerVariables::UniformDischarge;
	result["SedimentFlowMethods"] = CombinerVariables::sedimentFlowMethods;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfFlowMethods> CombinerVariables::mapForTypesOfFlowMethods(CombinerVariables::createMapForTypesOfFlowMethods());

std::map< std::string, CombinerVariables::TypesOfSedimentFlowMethods> CombinerVariables::createMapForTypesOfSedimentFlowMethods()
{
	std::map< std::string, TypesOfSedimentFlowMethods> result;
	result["SuspensionLoadFlowMethods"] = CombinerVariables::SuspensionLoadFlowMethods;
	result["BedloadFlowMethods"] = CombinerVariables::BedloadFlowMethods;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfSedimentFlowMethods> CombinerVariables::mapForTypesOfSedimentFlowMethods(CombinerVariables::createMapForTypesOfSedimentFlowMethods());

std::map< std::string, CombinerVariables::TypesOfGradientCalculationMethod> CombinerVariables::createMapForTypesOfGradientCalculationMethod()
{
	std::map< std::string, TypesOfGradientCalculationMethod> result;
	result["SimpleDownstreamTwoCellGradient"] = CombinerVariables::SimpleDownstreamTwoCellGradient;
	result["SimpleThreeCellGradient"] = CombinerVariables::SimpleThreeCellGradient;
	result["SimpleDownstreamTwoCellGradientWithCenteredValues"] = CombinerVariables::SimpleDownstreamTwoCellGradientWithCenteredValues;
	result["SimpleThreeCellGradientWithCenteredValues"] = CombinerVariables::SimpleThreeCellGradientWithCenteredValues;
	result["ReducedWaterEnergyslope"] = CombinerVariables::ReducedWaterEnergyslope;
	result["ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable"] = CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable;
	result["ReturnBedslope"] = CombinerVariables::ReturnBedslope;
	result["ReturnWaterEnergyslope"] = CombinerVariables::ReturnWaterEnergyslope;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfGradientCalculationMethod> CombinerVariables::mapForTypesOfGradientCalculationMethod(CombinerVariables::createMapForTypesOfGradientCalculationMethod());

std::map< std::string, CombinerVariables::TypesOfChangeRateModifiers> CombinerVariables::createMapForTypesOfChangeRateModifiers()
{
	std::map< std::string, TypesOfChangeRateModifiers> result;
	result["AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins"] = CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins;
	result["RecirculateWater"] = CombinerVariables::RecirculateWater;
	result["RecirculateSediment"] = CombinerVariables::RecirculateSediment;
	result["InputPropertyTimeSeriesLinearlyInterpolated"] = CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfChangeRateModifiers> CombinerVariables::mapForTypesOfChangeRateModifiers(CombinerVariables::createMapForTypesOfChangeRateModifiers());

std::map< std::string, CombinerVariables::TypesOfOutputMethod> CombinerVariables::createMapForTypesOfOutputMethod()
{
	std::map< std::string, TypesOfOutputMethod> result;
	result["OutputVerbatimTranslationOfConstructionVariablesToXML"] = CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML;
	result["OutputRegularRiverReachProperties"] = CombinerVariables::OutputRegularRiverReachProperties;
	result["OutputRegularRiverReachPropertiesForVisualInterpretation"] = CombinerVariables::OutputRegularRiverReachPropertiesForVisualInterpretation;
	result["OutputAccumulatedBedloadTransport"] = CombinerVariables::OutputAccumulatedBedloadTransport;
	result["OutputSimulationSetup"] = CombinerVariables::OutputSimulationSetup;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfOutputMethod> CombinerVariables::mapForTypesOfOutputMethod(CombinerVariables::createMapForTypesOfOutputMethod());

std::map< std::string, CombinerVariables::TypesOfChannelGeometry> CombinerVariables::createMapForTypesOfChannelGeometry()
{
	std::map< std::string, TypesOfChannelGeometry> result;
	result["InfinitelyDeepRectangularChannel"] = CombinerVariables::InfinitelyDeepRectangularChannel;
	result["InfinitelyDeepVShapedChannel"] = CombinerVariables::InfinitelyDeepVShapedChannel;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfChannelGeometry> CombinerVariables::mapForTypesOfChannelGeometry(CombinerVariables::createMapForTypesOfChannelGeometry());

std::map< std::string, CombinerVariables::TypesOfGeometricalChannelBehaviour> CombinerVariables::createMapForTypesOfGeometricalChannelBehaviour()
{
	std::map< std::string, TypesOfGeometricalChannelBehaviour> result;
	result["ParallelShiftOfBasicGeometry"] = CombinerVariables::ParallelShiftOfBasicGeometry;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfGeometricalChannelBehaviour> CombinerVariables::mapForTypesOfGeometricalChannelBehaviour(CombinerVariables::createMapForTypesOfGeometricalChannelBehaviour());

std::map< std::string, CombinerVariables::TypesOfStrataSorting> CombinerVariables::createMapForTypesOfStrataSorting()
{
	std::map< std::string, TypesOfStrataSorting> result;
	result["TwoLayerWithShearStressBasedUpdate"] = CombinerVariables::TwoLayerWithShearStressBasedUpdate;
	result["StratigraphyWithThresholdBasedUpdate"] = CombinerVariables::StratigraphyWithThresholdBasedUpdate;
	result["StratigraphyWithOLDConstantThresholdBasedUpdate"] = CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate;
	result["TwoLayerWithContinuousUpdate"] = CombinerVariables::TwoLayerWithContinuousUpdate;
	result["SingleLayerNoSorting"] = CombinerVariables::SingleLayerNoSorting;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfStrataSorting> CombinerVariables::mapForTypesOfStrataSorting(CombinerVariables::createMapForTypesOfStrataSorting());

std::map< std::string, CombinerVariables::TypesOfSillProperties> CombinerVariables::createMapForTypesOfSillProperties()
{
	std::map< std::string, TypesOfSillProperties> result;
	result["PoleniSill"] = CombinerVariables::PoleniSill;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfSillProperties> CombinerVariables::mapForTypesOfSillProperties(CombinerVariables::createMapForTypesOfSillProperties());

std::map< std::string, CombinerVariables::TypesOfFlowResistance> CombinerVariables::createMapForTypesOfFlowResistance()
{
	std::map< std::string, TypesOfFlowResistance> result;
	result["VariablePowerLawFlowResistance"] = CombinerVariables::VariablePowerLawFlowResistance;
	result["FixedPowerLawFlowResistance"] = CombinerVariables::FixedPowerLawFlowResistance;
	result["DarcyWeisbachFlowResistance"] = CombinerVariables::DarcyWeisbachFlowResistance;
	result["SolveForWaterEnergyslopeBasedOnHydraulicHead"] = CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfFlowResistance> CombinerVariables::mapForTypesOfFlowResistance(CombinerVariables::createMapForTypesOfFlowResistance());

std::map< std::string, CombinerVariables::TypesOfBedloadCapacityCalculationMethod> CombinerVariables::createMapForTypesOfBedloadCapacityCalculationMethod()
{
	std::map< std::string, TypesOfBedloadCapacityCalculationMethod> result;
	result["ChengBedloadCapacity"] = CombinerVariables::ChengBedloadCapacity;
	result["RickenmannBedloadCapacityBasedOnTheta"] = CombinerVariables::RickenmannBedloadCapacityBasedOnTheta;
	result["RickenmannBedloadCapacityBasedOnThetaNonFractional"] = CombinerVariables::RickenmannBedloadCapacityBasedOnThetaNonFractional;
	result["RickenmannBedloadCapacityBasedOnq"] = CombinerVariables::RickenmannBedloadCapacityBasedOnq;
	result["RickenmannBedloadCapacityBasedOnqNonFractional"] = CombinerVariables::RickenmannBedloadCapacityBasedOnqNonFractional;
	result["WilcockCroweBedloadCapacity"] = CombinerVariables::WilcockCroweBedloadCapacity;
	result["SchneiderEtAlBedloadCapacity"] = CombinerVariables::SchneiderEtAlBedloadCapacity;
	result["ReckingBedloadCapacityNonFractional"] = CombinerVariables::ReckingBedloadCapacityNonFractional;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfBedloadCapacityCalculationMethod> CombinerVariables::mapForTypesOfBedloadCapacityCalculationMethod(CombinerVariables::createMapForTypesOfBedloadCapacityCalculationMethod());

std::map< std::string, CombinerVariables::TypesOfTauCalculationMethod> CombinerVariables::createMapForTypesOfTauCalculationMethod()
{
	std::map< std::string, TypesOfTauCalculationMethod> result;
	result["EnergyslopeTauBasedOnFlowDepth"] = CombinerVariables::EnergyslopeTauBasedOnFlowDepth;
	result["EnergyslopeTau"] = CombinerVariables::EnergyslopeTau;
	result["FlowVelocityTau"] = CombinerVariables::FlowVelocityTau;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfTauCalculationMethod> CombinerVariables::mapForTypesOfTauCalculationMethod(CombinerVariables::createMapForTypesOfTauCalculationMethod());

std::map< std::string, CombinerVariables::TypesOfBedloadVelocityCalculationMethod> CombinerVariables::createMapForTypesOfBedloadVelocityCalculationMethod()
{
	std::map< std::string, TypesOfBedloadVelocityCalculationMethod> result;
	result["JulienBounvilayRollingParticlesVelocity"] = CombinerVariables::JulienBounvilayRollingParticlesVelocity;
	result["VelocityAsTransportRatePerUnitCrossSectionalArea"] = CombinerVariables::VelocityAsTransportRatePerUnitCrossSectionalArea;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfBedloadVelocityCalculationMethod> CombinerVariables::mapForTypesOfBedloadVelocityCalculationMethod(CombinerVariables::createMapForTypesOfBedloadVelocityCalculationMethod());

std::map< std::string, CombinerVariables::TypesOfEstimationMethodForThicknessOfMovingSedimentLayer> CombinerVariables::createMapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer()
{
	std::map< std::string, TypesOfEstimationMethodForThicknessOfMovingSedimentLayer> result;
	result["MultipleDiameterOfCoarsestGrainMoved"] = CombinerVariables::MultipleDiameterOfCoarsestGrainMoved;
	result["MultipleReferenceGrainDiameter"] = CombinerVariables::MultipleReferenceGrainDiameter;
	result["ConstantThicknessOfMovingSedimentLayer"] = CombinerVariables::ConstantThicknessOfMovingSedimentLayer;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfEstimationMethodForThicknessOfMovingSedimentLayer> CombinerVariables::mapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer(CombinerVariables::createMapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer());

std::map< std::string, CombinerVariables::TypesOfActiveWidthCalculationMethod> CombinerVariables::createMapForTypesOfActiveWidthCalculationMethod()
{
	std::map< std::string, TypesOfActiveWidthCalculationMethod> result;
	result["SetActiveWidthEqualFlowWidth"] = CombinerVariables::SetActiveWidthEqualFlowWidth;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfActiveWidthCalculationMethod> CombinerVariables::mapForTypesOfActiveWidthCalculationMethod(CombinerVariables::createMapForTypesOfActiveWidthCalculationMethod());

std::map< std::string, CombinerVariables::TypesOfCalculationMethodForBedloadMotionInitiationThreshold> CombinerVariables::createMapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold()
{
	std::map< std::string, TypesOfCalculationMethodForBedloadMotionInitiationThreshold> result;
	result["LambEtAlCriticalTheta"] = CombinerVariables::LambEtAlCriticalTheta;
	result["ConstantThresholdForInitiationOfBedloadMotion"] = CombinerVariables::ConstantThresholdForInitiationOfBedloadMotion;
	result["StochasticThresholdForInitiationOfBedloadMotion"] = CombinerVariables::StochasticThresholdForInitiationOfBedloadMotion;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfCalculationMethodForBedloadMotionInitiationThreshold> CombinerVariables::mapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold(CombinerVariables::createMapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold());

std::map< std::string, CombinerVariables::TypesOfHidingFactorsCalculationMethod> CombinerVariables::createMapForTypesOfHidingFactorsCalculationMethod()
{
	std::map< std::string, TypesOfHidingFactorsCalculationMethod> result;
	result["PowerLawHidingFunction"] = CombinerVariables::PowerLawHidingFunction;
	result["WilcockCroweHidingFunction"] = CombinerVariables::WilcockCroweHidingFunction;
	result["NoHiding"] = CombinerVariables::NoHiding;
	return result;
}
std::map< std::string, CombinerVariables::TypesOfHidingFactorsCalculationMethod> CombinerVariables::mapForTypesOfHidingFactorsCalculationMethod(CombinerVariables::createMapForTypesOfHidingFactorsCalculationMethod());



CombinerVariables::TypesOfUserInputReaders CombinerVariables::stringToTypeOfUserInputReaders (std::string string)
{
	std::map< std::string, TypesOfUserInputReaders>::const_iterator resultIterator = mapForTypesOfUserInputReaders.find(string);
	if (resultIterator == mapForTypesOfUserInputReaders.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of user input readers.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfUserInputReadersToString (TypesOfUserInputReaders typeOfUserInputReaders)
{
	std::string result;
	switch (typeOfUserInputReaders)
	{
	case CombinerVariables::VerbatimTranslationFromXMLToConstructionVariables:
		result = "VerbatimTranslationFromXMLToConstructionVariables";
		break;

	case CombinerVariables::StandardInput:
		result = "StandardInput";
		break;

	default:
		const char *const errorMessage = "Invalid User Input Reader Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfCombinersAndInterfaces CombinerVariables::stringToTypeOfCombinersAndInterfaces (std::string string)
{
	std::map< std::string, TypesOfCombinersAndInterfaces>::const_iterator resultIterator = mapForTypesOfCombinersAndInterfaces.find(string);
	if (resultIterator == mapForTypesOfCombinersAndInterfaces.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of combiners and interfaces.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfCombinersAndInterfacesToString (TypesOfCombinersAndInterfaces typeOfCombinersAndInterfaces)
{
	std::string result;
	switch (typeOfCombinersAndInterfaces)
	{
	case CombinerVariables::SedFlow:
		result = "SedFlow";
		break;

	case CombinerVariables::NumericRootFinder:
		result = "NumericRootFinder";
		break;

	case CombinerVariables::OutputMethods:
		result = "OutputMethods";
		break;

	case CombinerVariables::OutputMethodType:
		result = "OutputMethodType";
		break;

	case CombinerVariables::OverallParameters:
		result = "OverallParameters";
		break;

	case CombinerVariables::UserInputReader:
		result = "UserInputReader";
		break;

	case CombinerVariables::RiverSystemProperties:
		result = "RiverSystemProperties";
		break;

	case CombinerVariables::AdditionalRiverSystemProperties:
		result = "AdditionalRiverSystemProperties";
		break;

	case CombinerVariables::AdditionalRiverSystemPropertyType:
		result = "AdditionalRiverSystemPropertyType";
		break;

	case CombinerVariables::RegularRiverSystemProperties:
		result = "RegularRiverSystemProperties";
		break;

	case CombinerVariables::RiverReachProperties:
		result = "RiverReachProperties";
		break;

	case CombinerVariables::AdditionalRiverReachProperties:
		result = "AdditionalRiverReachProperties";
		break;

	case CombinerVariables::AdditionalRiverReachPropertyType:
		result = "AdditionalRiverReachPropertyType";
		break;

	case CombinerVariables::RegularRiverReachProperties:
		result = "RegularRiverReachProperties";
		break;

	case CombinerVariables::Grains:
		result = "Grains";
		break;

	case CombinerVariables::GrainType:
		result = "GrainType";
		break;

	case CombinerVariables::GeometricalChannelBehaviour:
		result = "GeometricalChannelBehaviour";
		break;

	case CombinerVariables::ChannelGeometry:
		result = "ChannelGeometry";
		break;

	case CombinerVariables::StrataSorting:
		result = "StrataSorting";
		break;

	case CombinerVariables::SillProperties:
		result = "SillProperties";
		break;

	case CombinerVariables::OverallMethods:
		result = "OverallMethods";
		break;

	case CombinerVariables::CalcGradient:
		result = "CalcGradient";
		break;

	case CombinerVariables::FlowResistance:
		result = "FlowResistance";
		break;

	case CombinerVariables::RiverSystemMethods:
		result = "RiverSystemMethods";
		break;

	case CombinerVariables::AdditionalRiverSystemMethods:
		result = "AdditionalRiverSystemMethods";
		break;

	case CombinerVariables::AdditionalRiverSystemMethodType:
		result = "AdditionalRiverSystemMethodType";
		break;

	case CombinerVariables::RegularRiverSystemMethods:
		result = "RegularRiverSystemMethods";
		break;

	case CombinerVariables::FlowMethods:
		result = "FlowMethods";
		break;

	case CombinerVariables::FlowTypeMethods:
		result = "FlowTypeMethods";
		break;

	case CombinerVariables::SedimentFlowMethods:
		result = "SedimentFlowMethods";
		break;

	case CombinerVariables::SedimentFlowTypeMethods:
		result = "SedimentFlowTypeMethods";
		break;

	case CombinerVariables::CalcBedloadCapacity:
		result = "CalcBedloadCapacity";
		break;

	case CombinerVariables::ChangeRateModifiers:
		result = "ChangeRateModifiers";
		break;

	case CombinerVariables::ChangeRateModifiersForSingleFlowMethod:
		result = "ChangeRateModifiersForSingleFlowMethod";
		break;

	case CombinerVariables::ChangeRateModifiersType:
		result = "ChangeRateModifiersType";
		break;

	case CombinerVariables::RiverReachMethods:
		result = "RiverReachMethods";
		break;

	case CombinerVariables::AdditionalRiverReachMethods:
		result = "AdditionalRiverReachMethods";
		break;

	case CombinerVariables::AdditionalRiverReachMethodType:
		result = "AdditionalRiverReachMethodType";
		break;

	case CombinerVariables::RegularRiverReachMethods:
		result = "RegularRiverReachMethods";
		break;

	case CombinerVariables::CalcTau:
		result = "CalcTau";
		break;

	case CombinerVariables::CalcBedloadVelocity:
		result = "CalcBedloadVelocity";
		break;

	case CombinerVariables::EstimateThicknessOfMovingSedimentLayer:
		result = "EstimateThicknessOfMovingSedimentLayer";
		break;

	case CombinerVariables::CalcActiveWidth:
		result = "CalcActiveWidth";
		break;

	case CombinerVariables::CalcThresholdForInitiationOfBedloadMotion:
		result = "CalcThresholdForInitiationOfBedloadMotion";
		break;

	case CombinerVariables::CalcHidingFactors:
		result = "CalcHidingFactors";
		break;

	case CombinerVariables::TimeSeriesEntry:
		result = "TimeSeriesEntry";
		break;

	case CombinerVariables::TimeSeries:
		result = "TimeSeries";
		break;

	default:
		const char *const errorMessage = "Invalid type of combiners and interfaces";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfGrains CombinerVariables::stringToTypeOfGrains (std::string string)
{
	std::map< std::string, TypesOfGrains>::const_iterator resultIterator = mapForTypesOfGrains.find(string);
	if (resultIterator == mapForTypesOfGrains.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of grains.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfGrainsToString (CombinerVariables::TypesOfGrains typeOfGrains)
{
	std::string result;
	switch (typeOfGrains)
	{
	case CombinerVariables::NormalGrains:
		result ="NormalGrains";
		break;

	case CombinerVariables::TracerGrains:
		result ="TracerGrains";
		break;

	case CombinerVariables::FishEggs:
		result ="FishEggs";
		break;

	default:
		const char *const errorMessage = "Invalid Grain Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfNumericRootFinder CombinerVariables::stringToTypeOfNumericRootFinder (std::string string)
{
	std::map< std::string, TypesOfNumericRootFinder>::const_iterator resultIterator = mapForTypesOfNumericRootFinder.find(string);
	if (resultIterator == mapForTypesOfNumericRootFinder.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of numeric root finder.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfNumericRootFinderToString (CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder)
{
	std::string result;
	switch (typeOfNumericRootFinder)
	{
	case CombinerVariables::BisectionMethod:
		result ="BisectionMethod";
		break;

	case CombinerVariables::SecantMethod:
		result ="SecantMethod";
		break;

	case CombinerVariables::FalsePositionMethod:
		result ="FalsePositionMethod";
		break;

	case CombinerVariables::RiddersMethod:
		result ="RiddersMethod";
		break;

	case CombinerVariables::BrentMethod:
		result ="BrentMethod";
		break;

	default:
		const char *const errorMessage = "Invalid NumericRootFinder Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfRegularRiverReachProperties CombinerVariables::stringToTypeOfRegularRiverReachProperties (std::string string)
{
	std::map< std::string, TypesOfRegularRiverReachProperties>::const_iterator resultIterator = mapForTypesOfRegularRiverReachProperties.find(string);
	if (resultIterator == mapForTypesOfRegularRiverReachProperties.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of regular river parameters.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfRegularRiverReachPropertiesToString (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties)
{
	std::string result;
	switch (typeOfRegularRiverReachProperties)
	{
	case CombinerVariables::elevation:
		result ="elevation";
		break;

	case CombinerVariables::length:
		result ="length";
		break;

	case CombinerVariables::sillOccurence:
		result ="sillOccurence";
		break;

	case CombinerVariables::sillTopEdgeElevation:
		result ="sillTopEdgeElevation";
		break;

	case CombinerVariables::discharge:
		result ="discharge";
		break;

	case CombinerVariables::flowVelocity:
		result ="flowVelocity";
		break;

	case CombinerVariables::maximumWaterdepth:
		result ="maximumWaterdepth";
		break;

	case CombinerVariables::waterLevel:
		result ="waterLevel";
		break;

	case CombinerVariables::hydraulicHead:
		result ="hydraulicHead";
		break;

	case CombinerVariables::kineticComponentOfHydraulicHead:
		result ="kineticComponentOfHydraulicHead";
		break;

	case CombinerVariables::bedShearStress:
		result ="bedShearStress";
		break;

	case CombinerVariables::activeWidth:
		result ="activeWidth";
		break;

	case CombinerVariables::strataPerUnitBedSurface:
		result ="strataPerUnitBedSurface";
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		result ="strataPerUnitBedSurfaceIncludingPoreVolume";
		break;

	case CombinerVariables::alluviumThicknessIncludingPoreVolume:
		result ="alluviumThicknessIncludingPoreVolume";
		break;

	case CombinerVariables::bedrockElevation:
		result ="bedrockElevation";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurface:
		result ="activeLayerPerUnitBedSurface";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume:
		result ="activeLayerPerUnitBedSurfaceIncludingPoreVolume";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume:
		result ="activeLayerPerUnitBedSurfaceOverallVolume";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result ="activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter:
		result ="activeLayerPerUnitBedSurfaceMedianGrainDiameter";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result ="activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result ="activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD20:
		result ="activeLayerPerUnitBedSurfaceD20";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD30:
		result ="activeLayerPerUnitBedSurfaceD30";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD50:
		result ="activeLayerPerUnitBedSurfaceD50";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD84:
		result ="activeLayerPerUnitBedSurfaceD84";
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD90:
		result ="activeLayerPerUnitBedSurfaceD90";
		break;

	case CombinerVariables::bedslope:
		result ="bedslope";
		break;

	case CombinerVariables::waterEnergyslope:
		result ="waterEnergyslope";
		break;

	case CombinerVariables::sedimentEnergyslope:
		result ="sedimentEnergyslope";
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		result ="unreducedSedimentEnergyslope";
		break;

	case CombinerVariables::waterVolumeChangeRate:
		result ="waterVolumeChangeRate";
		break;

	case CombinerVariables::waterVolumeChange:
		result ="waterVolumeChange";
		break;

	case CombinerVariables::erosionRate:
		result ="erosionRate";
		break;

	case CombinerVariables::erosionRateIncludingPoreVolume:
		result ="erosionRateIncludingPoreVolume";
		break;

	case CombinerVariables::erosionRateOverallVolume:
		result ="erosionRateOverallVolume";
		break;

	case CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume:
		result ="erosionRateOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::erosionRateMedianGrainDiameter:
		result ="erosionRateMedianGrainDiameter";
		break;

	case CombinerVariables::erosionRateArithmeticMeanGrainDiameter:
		result ="erosionRateArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::erosionRateGeometricMeanGrainDiameter:
		result ="erosionRateGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::erosionRateD20:
		result ="erosionRateD20";
		break;

	case CombinerVariables::erosionRateD30:
		result ="erosionRateD30";
		break;

	case CombinerVariables::erosionRateD50:
		result ="erosionRateD50";
		break;

	case CombinerVariables::erosionRateD84:
		result ="erosionRateD84";
		break;

	case CombinerVariables::erosionRateD90:
		result ="erosionRateD90";
		break;

	case CombinerVariables::depositionRate:
		result ="depositionRate";
		break;

	case CombinerVariables::depositionRateIncludingPoreVolume:
		result ="depositionRateIncludingPoreVolume";
		break;

	case CombinerVariables::depositionRateOverallVolume:
		result ="depositionRateOverallVolume";
		break;

	case CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume:
		result ="depositionRateOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::depositionRateMedianGrainDiameter:
		result ="depositionRateMedianGrainDiameter";
		break;

	case CombinerVariables::depositionRateArithmeticMeanGrainDiameter:
		result ="depositionRateArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::depositionRateGeometricMeanGrainDiameter:
		result ="depositionRateGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::depositionRateD20:
		result ="depositionRateD20";
		break;

	case CombinerVariables::depositionRateD30:
		result ="depositionRateD30";
		break;

	case CombinerVariables::depositionRateD50:
		result ="depositionRateD50";
		break;

	case CombinerVariables::depositionRateD84:
		result ="depositionRateD84";
		break;

	case CombinerVariables::depositionRateD90:
		result ="depositionRateD90";
		break;

	case CombinerVariables::erosion:
		result ="erosion";
		break;

	case CombinerVariables::erosionIncludingPoreVolume:
		result ="erosionIncludingPoreVolume";
		break;

	case CombinerVariables::erosionOverallVolume:
		result ="erosionOverallVolume";
		break;

	case CombinerVariables::erosionOverallVolumeIncludingPoreVolume:
		result ="erosionOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::erosionMedianGrainDiameter:
		result ="erosionMedianGrainDiameter";
		break;

	case CombinerVariables::erosionArithmeticMeanGrainDiameter:
		result ="erosionArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::erosionGeometricMeanGrainDiameter:
		result ="erosionGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::erosionD20:
		result ="erosionD20";
		break;

	case CombinerVariables::erosionD30:
		result ="erosionD30";
		break;

	case CombinerVariables::erosionD50:
		result ="erosionD50";
		break;

	case CombinerVariables::erosionD84:
		result ="erosionD84";
		break;

	case CombinerVariables::erosionD90:
		result ="erosionD90";
		break;

	case CombinerVariables::deposition:
		result ="deposition";
		break;

	case CombinerVariables::depositionIncludingPoreVolume:
		result ="depositionIncludingPoreVolume";
		break;

	case CombinerVariables::depositionOverallVolume:
		result ="depositionOverallVolume";
		break;

	case CombinerVariables::depositionOverallVolumeIncludingPoreVolume:
		result ="depositionOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::depositionMedianGrainDiameter:
		result ="depositionMedianGrainDiameter";
		break;

	case CombinerVariables::depositionArithmeticMeanGrainDiameter:
		result ="depositionArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::depositionGeometricMeanGrainDiameter:
		result ="depositionGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::depositionD20:
		result ="depositionD20";
		break;

	case CombinerVariables::depositionD30:
		result ="depositionD30";
		break;

	case CombinerVariables::depositionD50:
		result ="depositionD50";
		break;

	case CombinerVariables::depositionD84:
		result ="depositionD84";
		break;

	case CombinerVariables::depositionD90:
		result ="depositionD90";
		break;

	case CombinerVariables::erosionPerUnitBedSurface:
		result ="erosionPerUnitBedSurface";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume:
		result ="erosionPerUnitBedSurfaceIncludingPoreVolume";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolume:
		result ="erosionPerUnitBedSurfaceOverallVolume";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result ="erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter:
		result ="erosionPerUnitBedSurfaceMedianGrainDiameter";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result ="erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result ="erosionPerUnitBedSurfaceGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD20:
		result ="erosionPerUnitBedSurfaceD20";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD30:
		result ="erosionPerUnitBedSurfaceD30";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD50:
		result ="erosionPerUnitBedSurfaceD50";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD84:
		result ="erosionPerUnitBedSurfaceD84";
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD90:
		result ="erosionPerUnitBedSurfaceD90";
		break;

	case CombinerVariables::depositionPerUnitBedSurface:
		result ="depositionPerUnitBedSurface";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume:
		result ="depositionPerUnitBedSurfaceIncludingPoreVolume";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolume:
		result ="depositionPerUnitBedSurfaceOverallVolume";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result ="depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter:
		result ="depositionPerUnitBedSurfaceMedianGrainDiameter";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result ="depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result ="depositionPerUnitBedSurfaceGeometricMeanGrainDiameter";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD20:
		result ="depositionPerUnitBedSurfaceD20";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD30:
		result ="depositionPerUnitBedSurfaceD30";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD50:
		result ="depositionPerUnitBedSurfaceD50";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD84:
		result ="depositionPerUnitBedSurfaceD84";
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD90:
		result ="depositionPerUnitBedSurfaceD90";
		break;

	default:
		const char *const errorMessage ="Invalid Regular River Reach Property";
		throw (errorMessage);
	}
	return result;
}

bool CombinerVariables::regularRiverReachPropertyIsGrains (TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties)
{
	bool result;
	switch (typeOfRegularRiverReachProperties)
	{
	case CombinerVariables::elevation:
		result = false;
		break;

	case CombinerVariables::length:
		result = false;
		break;

	case CombinerVariables::sillOccurence:
		result = false;
		break;

	case CombinerVariables::sillTopEdgeElevation:
		result = false;
		break;

	case CombinerVariables::discharge:
		result = false;
		break;

	case CombinerVariables::flowVelocity:
		result = false;
		break;

	case CombinerVariables::maximumWaterdepth:
		result = false;
		break;

	case CombinerVariables::waterLevel:
		result = false;
		break;

	case CombinerVariables::hydraulicHead:
		result = false;
		break;

	case CombinerVariables::kineticComponentOfHydraulicHead:
		result = false;
		break;

	case CombinerVariables::bedShearStress:
		result = false;
		break;

	case CombinerVariables::activeWidth:
		result = false;
		break;

	case CombinerVariables::strataPerUnitBedSurface:
		result = true;
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::alluviumThicknessIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::bedrockElevation:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurface:
		result = true;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD20:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD30:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD50:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD84:
		result = false;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD90:
		result = false;
		break;

	case CombinerVariables::bedslope:
		result = false;
		break;

	case CombinerVariables::waterEnergyslope:
		result = false;
		break;

	case CombinerVariables::sedimentEnergyslope:
		result = false;
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		result = false;
		break;

	case CombinerVariables::waterVolumeChangeRate:
		result = false;
		break;

	case CombinerVariables::waterVolumeChange:
		result = false;
		break;

	case CombinerVariables::erosionRate:
		result = true;
		break;

	case CombinerVariables::erosionRateIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::erosionRateOverallVolume:
		result = false;
		break;

	case CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::erosionRateMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionRateArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionRateGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionRateD20:
		result = false;
		break;

	case CombinerVariables::erosionRateD30:
		result = false;
		break;

	case CombinerVariables::erosionRateD50:
		result = false;
		break;

	case CombinerVariables::erosionRateD84:
		result = false;
		break;

	case CombinerVariables::erosionRateD90:
		result = false;
		break;

	case CombinerVariables::depositionRate:
		result = true;
		break;

	case CombinerVariables::depositionRateIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::depositionRateOverallVolume:
		result = false;
		break;

	case CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::depositionRateMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionRateArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionRateGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionRateD20:
		result = false;
		break;

	case CombinerVariables::depositionRateD30:
		result = false;
		break;

	case CombinerVariables::depositionRateD50:
		result = false;
		break;

	case CombinerVariables::depositionRateD84:
		result = false;
		break;

	case CombinerVariables::depositionRateD90:
		result = false;
		break;

	case CombinerVariables::erosion:
		result = true;
		break;

	case CombinerVariables::erosionIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::erosionOverallVolume:
		result = false;
		break;

	case CombinerVariables::erosionOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::erosionMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionD20:
		result = false;
		break;

	case CombinerVariables::erosionD30:
		result = false;
		break;

	case CombinerVariables::erosionD50:
		result = false;
		break;

	case CombinerVariables::erosionD84:
		result = false;
		break;

	case CombinerVariables::erosionD90:
		result = false;
		break;

	case CombinerVariables::deposition:
		result = true;
		break;

	case CombinerVariables::depositionIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::depositionOverallVolume:
		result = false;
		break;

	case CombinerVariables::depositionOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::depositionMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionD20:
		result = false;
		break;

	case CombinerVariables::depositionD30:
		result = false;
		break;

	case CombinerVariables::depositionD50:
		result = false;
		break;

	case CombinerVariables::depositionD84:
		result = false;
		break;

	case CombinerVariables::depositionD90:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurface:
		result = true;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolume:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD20:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD30:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD50:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD84:
		result = false;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD90:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurface:
		result = true;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume:
		result = true;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolume:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD20:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD30:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD50:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD84:
		result = false;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD90:
		result = false;
		break;

	default:
		const char *const errorMessage = "Invalid Regular River Reach Property";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod CombinerVariables::stringToTypeOfAdditionalRiverReachPropertyAndMethod (std::string string)
{
	std::map< std::string, TypesOfAdditionalRiverReachPropertyAndMethod>::const_iterator resultIterator = mapForTypesOfAdditionalRiverReachPropertyAndMethod.find(string);
	if (resultIterator == mapForTypesOfAdditionalRiverReachPropertyAndMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of additional river reach property and method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfAdditionalRiverReachPropertyAndMethodToString (CombinerVariables::TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalRiverReachPropertyAndMethod)
{
	std::string result;
	switch (typeOfAdditionalRiverReachPropertyAndMethod)
	{
	case CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter:
		result = "BedrockRoughnessEquivalentRepresentativeGrainDiameter";
		break;

	case CombinerVariables::PreventLocalGrainSizeDistributionChanges:
		result = "PreventLocalGrainSizeDistributionChanges";
		break;

	case CombinerVariables::InstantaneousSedimentInputs:
		result = "InstantaneousSedimentInputs";
		break;

	case CombinerVariables::SternbergAbrasionWithoutFining:
		result = "SternbergAbrasionWithoutFining";
		break;

	case CombinerVariables::SternbergAbrasionIncludingFining:
		result = "SternbergAbrasionIncludingFining";
		break;

	case CombinerVariables::ScourChain:
		result = "ScourChain";
		break;

	default:
		const char *const errorMessage = "Invalid Additional River Reach Property and Method Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod CombinerVariables::stringToTypeOfAdditionalRiverSystemPropertyAndMethod (std::string string)
{
	std::map< std::string, TypesOfAdditionalRiverSystemPropertyAndMethod>::const_iterator resultIterator = mapForTypesOfAdditionalRiverSystemPropertyAndMethod.find(string);
	if (resultIterator == mapForTypesOfAdditionalRiverSystemPropertyAndMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of additional river system property and method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfAdditionalRiverSystemPropertyAndMethodToString (CombinerVariables::TypesOfAdditionalRiverSystemPropertyAndMethod typeOfAdditionalRiverSystemPropertyAndMethod)
{
	std::string result;
	// Switch this switch-case back on again, whenever there are cases besides the default.
	/*
	switch (typeOfAdditionalRiverSystemPropertyAndMethod)
	{


	default:
		const char *const errorMessage = "Invalid Additional River System Property and Method Type";
		throw (errorMessage);
	}
	*/
	return result;
}

CombinerVariables::TypesOfChangeRates CombinerVariables::stringToTypeOfChangeRates (std::string string)
{
	std::map< std::string, TypesOfChangeRates>::const_iterator resultIterator = mapForTypesOfChangeRates.find(string);
	if (resultIterator == mapForTypesOfChangeRates.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of change rates.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfChangeRateToString (CombinerVariables::TypesOfChangeRates typeOfChangeRate)
{
	std::string result;
	switch (typeOfChangeRate)
	{
	case CombinerVariables::WaterdepthChange:
		result = "WaterdepthChange";
		break;

	case CombinerVariables::RelativeTwoCellBedSlopeChange:
		result = "RelativeTwoCellBedSlopeChange";
		break;

	case CombinerVariables::EnergySlopeChange:
		result = "EnergySlopeChange";
		break;


	default:
		const char *const errorMessage = "Invalid Additional River Parameter Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfGeneralFlowMethods CombinerVariables::stringToTypeOfGeneralFlowMethods (std::string string)
{
	std::map< std::string, TypesOfGeneralFlowMethods>::const_iterator resultIterator = mapForTypesOfGeneralFlowMethods.find(string);
	if (resultIterator == mapForTypesOfGeneralFlowMethods.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of general flow methods.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfGeneralFlowMethodsToString (CombinerVariables::TypesOfGeneralFlowMethods typeOfGeneralFlowMethods)
{
	std::string result;
	switch (typeOfGeneralFlowMethods)
	{
	case CombinerVariables::WaterFlowMethodsInGeneral:
		result = "WaterFlowMethodsInGeneral";
		break;

	case CombinerVariables::SedimentFlowMethodsInGeneral:
		result = "SedimentFlowMethodsInGeneral";
		break;

	default:
		const char *const errorMessage = "Invalid General Flow Method Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfFlowMethods CombinerVariables::stringToTypeOfFlowMethods (std::string string)
{
	std::map< std::string, TypesOfFlowMethods>::const_iterator resultIterator = mapForTypesOfFlowMethods.find(string);
	if (resultIterator == mapForTypesOfFlowMethods.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of flow methods.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfFlowMethodsToString (TypesOfFlowMethods typeOfFlowMethods)
{
	std::string result;
	switch (typeOfFlowMethods)
	{
	case CombinerVariables::ImplicitKinematicWave:
		result = "ImplicitKinematicWave";
		break;

	case CombinerVariables::ExplicitKinematicWave:
		result = "ExplicitKinematicWave";
		break;

	case CombinerVariables::UniformDischarge:
		result = "UniformDischarge";
		break;

	case CombinerVariables::sedimentFlowMethods:
		result = "SedimentFlowMethods";
		break;

	default:
		const char *const errorMessage = "Invalid Flow Method Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfSedimentFlowMethods CombinerVariables::stringToTypeOfSedimentFlowMethods (std::string string)
{
	std::map< std::string, TypesOfSedimentFlowMethods>::const_iterator resultIterator = mapForTypesOfSedimentFlowMethods.find(string);
	if (resultIterator == mapForTypesOfSedimentFlowMethods.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of sediment flow methods.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfSedimentFlowMethodsToString (TypesOfSedimentFlowMethods typeOfSedimentFlowMethods)
{
	std::string result;
	switch (typeOfSedimentFlowMethods)
	{
	case CombinerVariables::SuspensionLoadFlowMethods:
		result = "SuspensionLoadFlowMethods";
		break;

	case CombinerVariables::BedloadFlowMethods:
		result = "BedloadFlowMethods";
		break;

	default:
		const char *const errorMessage = "Invalid Sediment Flow Method Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfGradientCalculationMethod CombinerVariables::stringToTypeOfGradientCalculationMethod (std::string string)
{
	std::map< std::string, TypesOfGradientCalculationMethod>::const_iterator resultIterator = mapForTypesOfGradientCalculationMethod.find(string);
	if (resultIterator == mapForTypesOfGradientCalculationMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of gradient calculation method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfGradientCalculationMethodToString (TypesOfGradientCalculationMethod typeOfGradientCalculationMethod)
{
	std::string result;
	switch (typeOfGradientCalculationMethod)
	{
	case CombinerVariables::SimpleDownstreamTwoCellGradient:
		result = "SimpleDownstreamTwoCellGradient";
		break;

	case CombinerVariables::SimpleThreeCellGradient:
		result = "SimpleThreeCellGradient";
		break;

	case CombinerVariables::SimpleDownstreamTwoCellGradientWithCenteredValues:
		result = "SimpleDownstreamTwoCellGradientWithCenteredValues";
		break;

	case CombinerVariables::SimpleThreeCellGradientWithCenteredValues:
		result = "SimpleThreeCellGradientWithCenteredValues";
		break;

	case CombinerVariables::ReducedWaterEnergyslope:
		result = "ReducedWaterEnergyslope";
		break;

	case CombinerVariables::ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable:
		result = "ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable";
		break;

	case CombinerVariables::ReturnBedslope:
		result = "ReturnBedslope";
		break;

	case CombinerVariables::ReturnWaterEnergyslope:
		result = "ReturnWaterEnergyslope";
		break;

	default:
		const char *const errorMessage = "Invalid Gradient Calculation Method Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfChangeRateModifiers CombinerVariables::stringToTypeOfChangeRateModifiers (std::string string)
{
	std::map< std::string, TypesOfChangeRateModifiers>::const_iterator resultIterator = mapForTypesOfChangeRateModifiers.find(string);
	if (resultIterator == mapForTypesOfChangeRateModifiers.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of change rate modifiers.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfChangeRateModifiersToString (TypesOfChangeRateModifiers typeOfChangeRateModifiers)
{
	std::string result;
	switch (typeOfChangeRateModifiers)
	{
	case CombinerVariables::AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins:
		result = "AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins";
		break;

	case CombinerVariables::RecirculateWater:
		result = "RecirculateWater";
		break;


	case CombinerVariables::RecirculateSediment:
		result = "RecirculateSediment";
		break;

	case CombinerVariables::InputPropertyTimeSeriesLinearlyInterpolated:
		result = "InputPropertyTimeSeriesLinearlyInterpolated";
		break;

	default:
		const char *const errorMessage = "Invalid Change Rate Modifiers Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfOutputMethod CombinerVariables::stringToTypeOfOutputMethod (std::string string)
{
	std::map< std::string, TypesOfOutputMethod>::const_iterator resultIterator = mapForTypesOfOutputMethod.find(string);
	if (resultIterator == mapForTypesOfOutputMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of output method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfOutputMethodToString (TypesOfOutputMethod typeOfOutputMethod)
{
	std::string result;
	switch (typeOfOutputMethod)
	{
	case CombinerVariables::OutputVerbatimTranslationOfConstructionVariablesToXML:
		result = "OutputVerbatimTranslationOfConstructionVariablesToXML";
		break;

	case CombinerVariables::OutputRegularRiverReachProperties:
		result = "OutputRegularRiverReachProperties";
		break;

	case CombinerVariables::OutputRegularRiverReachPropertiesForVisualInterpretation:
		result = "OutputRegularRiverReachPropertiesForVisualInterpretation";
		break;

	case CombinerVariables::OutputAccumulatedBedloadTransport:
		result = "OutputAccumulatedBedloadTransport";
		break;

	case CombinerVariables::OutputSimulationSetup:
		result = "OutputSimulationSetup";
		break;

	default:
		const char *const errorMessage = "Invalid Output Method Type";
		throw (errorMessage);
		}
	return result;
}

CombinerVariables::TypesOfChannelGeometry CombinerVariables::stringToTypeOfChannelGeometry (std::string string)
{
	std::map< std::string, TypesOfChannelGeometry>::const_iterator resultIterator = mapForTypesOfChannelGeometry.find(string);
	if (resultIterator == mapForTypesOfChannelGeometry.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of channel geometry.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfChannelGeometryToString (TypesOfChannelGeometry typeOfChannelGeometry)
{
	std::string result;
	switch (typeOfChannelGeometry)
	{
	case CombinerVariables::InfinitelyDeepRectangularChannel:
		result = "InfinitelyDeepRectangularChannel";
		break;

	case CombinerVariables::InfinitelyDeepVShapedChannel:
		result = "InfinitelyDeepVShapedChannel";
		break;

	default:
		const char *const errorMessage = "Invalid Channel Geometry Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfGeometricalChannelBehaviour CombinerVariables::stringToTypeOfGeometricalChannelBehaviour (std::string string)
{
	std::map< std::string, TypesOfGeometricalChannelBehaviour>::const_iterator resultIterator = mapForTypesOfGeometricalChannelBehaviour.find(string);
	if (resultIterator == mapForTypesOfGeometricalChannelBehaviour.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of geometrical channel behaviour.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfGeometricalChannelBehaviourToString (TypesOfGeometricalChannelBehaviour typeOfGeometricalChannelBehaviour)
{
	std::string result;
	switch (typeOfGeometricalChannelBehaviour)
	{
	case CombinerVariables::ParallelShiftOfBasicGeometry:
		result = "ParallelShiftOfBasicGeometry";
		break;

	default:
		const char *const errorMessage = "Invalid Geometrical Channel Behaviour Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfStrataSorting CombinerVariables::stringToTypeOfStrataSorting (std::string string)
{
	std::map< std::string, TypesOfStrataSorting>::const_iterator resultIterator = mapForTypesOfStrataSorting.find(string);
	if (resultIterator == mapForTypesOfStrataSorting.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of strata sorting.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfStrataSortingToString (TypesOfStrataSorting typeOfStrataSorting)
{
	std::string result;
	switch (typeOfStrataSorting)
	{
	case CombinerVariables::TwoLayerWithShearStressBasedUpdate:
		result = "TwoLayerWithShearStressBasedUpdate";
		break;

	case CombinerVariables::StratigraphyWithThresholdBasedUpdate:
		result = "StratigraphyWithThresholdBasedUpdate";
		break;

	case CombinerVariables::StratigraphyWithOLDConstantThresholdBasedUpdate:
		result = "StratigraphyWithOLDConstantThresholdBasedUpdate";
		break;

	case CombinerVariables::TwoLayerWithContinuousUpdate:
		result = "TwoLayerWithContinuousUpdate";
		break;

	case CombinerVariables::SingleLayerNoSorting:
		result = "SingleLayerNoSorting";
		break;

	default:
		const char *const errorMessage = "Invalid Strata Sorting Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfSillProperties CombinerVariables::stringToTypeOfSillProperties (std::string string)
{
	std::map< std::string, TypesOfSillProperties>::const_iterator resultIterator = mapForTypesOfSillProperties.find(string);
	if (resultIterator == mapForTypesOfSillProperties.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of sill properties.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfSillPropertiesToString (TypesOfSillProperties typeOfSillProperties)
{
	std::string result;
	switch (typeOfSillProperties)
	{
	case CombinerVariables::PoleniSill:
		result = "PoleniSill";
		break;

	default:
		const char *const errorMessage = "Invalid Sill Properties Type";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfFlowResistance CombinerVariables::stringToTypeOfFlowResistance (std::string string)
{
	std::map< std::string, TypesOfFlowResistance>::const_iterator resultIterator = mapForTypesOfFlowResistance.find(string);
	if (resultIterator == mapForTypesOfFlowResistance.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of flow resistance.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfFlowResistanceToString (TypesOfFlowResistance typeOfFlowResistance)
{
	std::string result;
	switch (typeOfFlowResistance)
	{
	case CombinerVariables::VariablePowerLawFlowResistance:
		result = "VariablePowerLawFlowResistance";
		break;

	case CombinerVariables::FixedPowerLawFlowResistance:
		result = "FixedPowerLawFlowResistance";
		break;

	case CombinerVariables::DarcyWeisbachFlowResistance:
		result = "DarcyWeisbachFlowResistance";
		break;

	case CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead:
		result = "SolveForWaterEnergyslopeBasedOnHydraulicHead";
		break;

	default:
		const char *const errorMessage = "Invalid type of flow resistance";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfBedloadCapacityCalculationMethod CombinerVariables::stringToTypeOfBedloadCapacityCalculationMethod (std::string string)
{
	std::map< std::string, TypesOfBedloadCapacityCalculationMethod>::const_iterator resultIterator = mapForTypesOfBedloadCapacityCalculationMethod.find(string);
	if (resultIterator == mapForTypesOfBedloadCapacityCalculationMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of bedload capacity calculation method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfBedloadCapacityCalculationMethodToString (TypesOfBedloadCapacityCalculationMethod typeOfBedloadCapacityCalculationMethod)
{
	std::string result;
	switch (typeOfBedloadCapacityCalculationMethod)
	{
	case CombinerVariables::ChengBedloadCapacity:
		result = "ChengBedloadCapacity";
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnTheta:
		result = "RickenmannBedloadCapacityBasedOnTheta";
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnThetaNonFractional:
		result = "RickenmannBedloadCapacityBasedOnThetaNonFractional";
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnq:
		result = "RickenmannBedloadCapacityBasedOnq";
		break;

	case CombinerVariables::RickenmannBedloadCapacityBasedOnqNonFractional:
		result = "RickenmannBedloadCapacityBasedOnqNonFractional";
		break;

	case CombinerVariables::WilcockCroweBedloadCapacity:
		result = "WilcockCroweBedloadCapacity";
		break;

	case CombinerVariables::SchneiderEtAlBedloadCapacity:
		result = "SchneiderEtAlBedloadCapacity";
		break;

	case CombinerVariables::ReckingBedloadCapacityNonFractional:
		result = "ReckingBedloadCapacityNonFractional";
		break;

	default:
		const char *const errorMessage = "Invalid type of bedload capacity calculation method.";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfTauCalculationMethod CombinerVariables::stringToTypeOfTauCalculationMethod (std::string string)
{
	std::map< std::string, TypesOfTauCalculationMethod>::const_iterator resultIterator = mapForTypesOfTauCalculationMethod.find(string);
	if (resultIterator == mapForTypesOfTauCalculationMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of tau calculation method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfTauCalculationMethodToString (TypesOfTauCalculationMethod typeOfTauCalculationMethod)
{
	std::string result;
	switch (typeOfTauCalculationMethod)
	{
	case CombinerVariables::EnergyslopeTauBasedOnFlowDepth:
		result = "EnergyslopeTauBasedOnFlowDepth";
		break;

	case CombinerVariables::EnergyslopeTau:
		result = "EnergyslopeTau";
		break;

	case CombinerVariables::FlowVelocityTau:
		result = "FlowVelocityTau";
		break;

	default:
		const char *const errorMessage = "Invalid type of tau calculation method.";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfBedloadVelocityCalculationMethod CombinerVariables::stringToTypeOfBedloadVelocityCalculationMethod (std::string string)
{
	std::map< std::string, TypesOfBedloadVelocityCalculationMethod>::const_iterator resultIterator = mapForTypesOfBedloadVelocityCalculationMethod.find(string);
	if (resultIterator == mapForTypesOfBedloadVelocityCalculationMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of bedload velocity calculation method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfBedloadVelocityCalculationMethodToString (TypesOfBedloadVelocityCalculationMethod typeOfBedloadVelocityCalculationMethod)
{
	std::string result;
	switch (typeOfBedloadVelocityCalculationMethod)
	{
	case CombinerVariables::JulienBounvilayRollingParticlesVelocity:
		result = "JulienBounvilayRollingParticlesVelocity";
		break;

	case CombinerVariables::VelocityAsTransportRatePerUnitCrossSectionalArea:
		result = "VelocityAsTransportRatePerUnitCrossSectionalArea";
		break;

	default:
		const char *const errorMessage = "Invalid type of bedload velocity calculation method.";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfEstimationMethodForThicknessOfMovingSedimentLayer CombinerVariables::stringToTypeOfEstimationMethodForThicknessOfMovingSedimentLayer (std::string string)
{
	std::map< std::string, TypesOfEstimationMethodForThicknessOfMovingSedimentLayer>::const_iterator resultIterator = mapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer.find(string);
	if (resultIterator == mapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of estimation method for thickness of moving sediment layer.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfEstimationMethodForThicknessOfMovingSedimentLayerToString (TypesOfEstimationMethodForThicknessOfMovingSedimentLayer typeOfEstimationMethodForThicknessOfMovingSedimentLayer)
{
	std::string result;
	switch (typeOfEstimationMethodForThicknessOfMovingSedimentLayer)
	{
	case CombinerVariables::MultipleDiameterOfCoarsestGrainMoved:
		result = "MultipleDiameterOfCoarsestGrainMoved";
		break;

	case CombinerVariables::MultipleReferenceGrainDiameter:
		result = "MultipleReferenceGrainDiameter";
		break;

	case CombinerVariables::ConstantThicknessOfMovingSedimentLayer:
		result = "ConstantThicknessOfMovingSedimentLayer";
		break;

	default:
		const char *const errorMessage = "Invalid type of estimation method for thickness of moving sediment layer.";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfActiveWidthCalculationMethod CombinerVariables::stringToTypeOfActiveWidthCalculationMethod (std::string string)
{
	std::map< std::string, TypesOfActiveWidthCalculationMethod>::const_iterator resultIterator = mapForTypesOfActiveWidthCalculationMethod.find(string);
	if (resultIterator == mapForTypesOfActiveWidthCalculationMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of active width calculation method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfActiveWidthCalculationMethodToString (TypesOfActiveWidthCalculationMethod typeOfActiveWidthCalculationMethod)
{
	std::string result;
	switch (typeOfActiveWidthCalculationMethod)
	{
	case CombinerVariables::SetActiveWidthEqualFlowWidth:
		result = "SetActiveWidthEqualFlowWidth";
		break;

	default:
		const char *const errorMessage = "Invalid type of active width calculation method.";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfCalculationMethodForBedloadMotionInitiationThreshold CombinerVariables::stringToTypeOfCalculationMethodForBedloadMotionInitiationThreshold (std::string string)
{
	std::map< std::string, TypesOfCalculationMethodForBedloadMotionInitiationThreshold>::const_iterator resultIterator = mapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold.find(string);
	if (resultIterator == mapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of calculation method for bedload motion initiation threshold.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfCalculationMethodForBedloadMotionInitiationThresholdToString (TypesOfCalculationMethodForBedloadMotionInitiationThreshold typeOfCalculationMethodForBedloadMotionInitiationThreshold)
{
	std::string result;
	switch (typeOfCalculationMethodForBedloadMotionInitiationThreshold)
	{
	case CombinerVariables::LambEtAlCriticalTheta:
		result = "LambEtAlCriticalTheta";
		break;

	case CombinerVariables::ConstantThresholdForInitiationOfBedloadMotion:
		result = "ConstantThresholdForInitiationOfBedloadMotion";
		break;

	case CombinerVariables::StochasticThresholdForInitiationOfBedloadMotion:
		result = "StochasticThresholdForInitiationOfBedloadMotion";
		break;

	default:
		const char *const errorMessage = "Invalid type of calculation method for bedload motion initiation threshold.";
		throw (errorMessage);
	}
	return result;
}

CombinerVariables::TypesOfHidingFactorsCalculationMethod CombinerVariables::stringToTypeOfHidingFactorsCalculationMethod (std::string string)
{
	std::map< std::string, TypesOfHidingFactorsCalculationMethod>::const_iterator resultIterator = mapForTypesOfHidingFactorsCalculationMethod.find(string);
	if (resultIterator == mapForTypesOfHidingFactorsCalculationMethod.end())
	{
		std::string errorMessageAsString = "String \"";
		errorMessageAsString.append(string);
		errorMessageAsString.append("\" not mapped to type of hiding factors calculation method.");
		const char *const errorMessage = errorMessageAsString.c_str();
		throw(errorMessage);
	}
	else { return (*resultIterator).second; }
}

std::string CombinerVariables::typeOfHidingFactorsCalculationMethodToString (TypesOfHidingFactorsCalculationMethod typeOfHidingFactorsCalculationMethod)
{
	std::string result;
	switch (typeOfHidingFactorsCalculationMethod)
	{
	case CombinerVariables::PowerLawHidingFunction:
		result = "PowerLawHidingFunction";
		break;

	case CombinerVariables::WilcockCroweHidingFunction:
		result = "WilcockCroweHidingFunction";
		break;

	case CombinerVariables::NoHiding:
		result = "NoHiding";
		break;

	default:
		const char *const errorMessage = "Invalid type of hiding factors calculation method.";
		throw (errorMessage);
	}
	return result;
}

}
