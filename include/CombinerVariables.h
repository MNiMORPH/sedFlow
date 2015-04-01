/*
 * CombinerVariables.h
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

#ifndef COMBINERVARIABLES_H_
#define COMBINERVARIABLES_H_

#include <string>
#include <map>

namespace SedFlow {

class CombinerVariables {
private:
	static const std::string sedflowStringPrefix; //For initialisation please see CombinerVariables.cpp

public:
	CombinerVariables(){}
	~CombinerVariables(){}

	static void convertInternalStringIntoExternalString(std::string& internalString);
	static void convertExternalStringIntoInternalString(std::string& externalString);
	static std::string getInternalStringCorrespondingToExternalString(std::string externalString);
	static std::string getExternalStringCorrespondingToInternalString(std::string internalString);

	enum TypesOfUserInputReaders {VerbatimTranslationFromXMLToConstructionVariables, StandardInput};
	static TypesOfUserInputReaders stringToTypeOfUserInputReaders (std::string string);
	static std::string typeOfUserInputReadersToString (TypesOfUserInputReaders typeOfUserInputReaders);

	enum TypesOfCombinersAndInterfaces {SedFlow, NumericRootFinder, OutputMethods, OutputMethodType, UserInputReader, OverallParameters, RiverSystemProperties, AdditionalRiverSystemProperties, AdditionalRiverSystemPropertyType, RegularRiverSystemProperties,
										RiverReachProperties, AdditionalRiverReachProperties, AdditionalRiverReachPropertyType, RegularRiverReachProperties, Grains, GrainType,
										GeometricalChannelBehaviour, ChannelGeometry, StrataSorting, SillProperties, OverallMethods, CalcGradient, FlowResistance, RiverSystemMethods, AdditionalRiverSystemMethods, AdditionalRiverSystemMethodType,
										RegularRiverSystemMethods, FlowMethods, FlowTypeMethods, SedimentFlowMethods, SedimentFlowTypeMethods,CalcBedloadCapacity, ChangeRateModifiers, ChangeRateModifiersForSingleFlowMethod, ChangeRateModifiersType,
										RiverReachMethods, AdditionalRiverReachMethods, AdditionalRiverReachMethodType, RegularRiverReachMethods, CalcTau, CalcBedloadVelocity, EstimateThicknessOfMovingSedimentLayer, CalcActiveWidth, CalcThresholdForInitiationOfBedloadMotion, CalcHidingFactors, TimeSeriesEntry, TimeSeries};
	static TypesOfCombinersAndInterfaces stringToTypeOfCombinersAndInterfaces (std::string string);
	static std::string typeOfCombinersAndInterfacesToString (TypesOfCombinersAndInterfaces typeOfCombinersAndInterfaces);

	enum TypesOfNumericRootFinder {BisectionMethod, SecantMethod, FalsePositionMethod, RiddersMethod, BrentMethod};
	static TypesOfNumericRootFinder stringToTypeOfNumericRootFinder (std::string string);
	static std::string typeOfNumericRootFinderToString (TypesOfNumericRootFinder typeOfNumericRootFinder);

	enum TypesOfGrains {NormalGrains, TracerGrains, FishEggs};
	static TypesOfGrains stringToTypeOfGrains (std::string string);
	static std::string typeOfGrainsToString (TypesOfGrains typeOfGrains);

	enum TypesOfRegularRiverReachProperties {elevation, length, sillOccurence, sillTopEdgeElevation, discharge, flowVelocity, maximumWaterdepth, waterLevel, hydraulicHead, kineticComponentOfHydraulicHead, bedShearStress, activeWidth, strataPerUnitBedSurface, strataPerUnitBedSurfaceIncludingPoreVolume, alluviumThicknessIncludingPoreVolume, bedrockElevation, activeLayerPerUnitBedSurface, activeLayerPerUnitBedSurfaceIncludingPoreVolume, activeLayerPerUnitBedSurfaceOverallVolume, activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume, activeLayerPerUnitBedSurfaceMedianGrainDiameter, activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter, activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter, activeLayerPerUnitBedSurfaceD20, activeLayerPerUnitBedSurfaceD30, activeLayerPerUnitBedSurfaceD50, activeLayerPerUnitBedSurfaceD84, activeLayerPerUnitBedSurfaceD90, bedslope, waterEnergyslope, sedimentEnergyslope, unreducedSedimentEnergyslope, waterVolumeChangeRate, waterVolumeChange, erosionRate, erosionRateIncludingPoreVolume, erosionRateOverallVolume, erosionRateOverallVolumeIncludingPoreVolume, erosionRateMedianGrainDiameter, erosionRateArithmeticMeanGrainDiameter, erosionRateGeometricMeanGrainDiameter, erosionRateD20, erosionRateD30, erosionRateD50, erosionRateD84, erosionRateD90, depositionRate, depositionRateIncludingPoreVolume, depositionRateOverallVolume, depositionRateOverallVolumeIncludingPoreVolume, depositionRateMedianGrainDiameter, depositionRateArithmeticMeanGrainDiameter, depositionRateGeometricMeanGrainDiameter, depositionRateD20, depositionRateD30, depositionRateD50, depositionRateD84, depositionRateD90, erosion, erosionIncludingPoreVolume, erosionOverallVolume, erosionOverallVolumeIncludingPoreVolume, erosionMedianGrainDiameter, erosionArithmeticMeanGrainDiameter, erosionGeometricMeanGrainDiameter, erosionD20, erosionD30, erosionD50, erosionD84, erosionD90, deposition, depositionIncludingPoreVolume, depositionOverallVolume, depositionOverallVolumeIncludingPoreVolume, depositionMedianGrainDiameter, depositionArithmeticMeanGrainDiameter, depositionGeometricMeanGrainDiameter, depositionD20, depositionD30, depositionD50, depositionD84, depositionD90, erosionPerUnitBedSurface, erosionPerUnitBedSurfaceIncludingPoreVolume, erosionPerUnitBedSurfaceOverallVolume, erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume, erosionPerUnitBedSurfaceMedianGrainDiameter, erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter, erosionPerUnitBedSurfaceGeometricMeanGrainDiameter, erosionPerUnitBedSurfaceD20, erosionPerUnitBedSurfaceD30, erosionPerUnitBedSurfaceD50, erosionPerUnitBedSurfaceD84, erosionPerUnitBedSurfaceD90, depositionPerUnitBedSurface, depositionPerUnitBedSurfaceIncludingPoreVolume, depositionPerUnitBedSurfaceOverallVolume, depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume, depositionPerUnitBedSurfaceMedianGrainDiameter, depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter, depositionPerUnitBedSurfaceGeometricMeanGrainDiameter, depositionPerUnitBedSurfaceD20, depositionPerUnitBedSurfaceD30, depositionPerUnitBedSurfaceD50, depositionPerUnitBedSurfaceD84, depositionPerUnitBedSurfaceD90};
	static TypesOfRegularRiverReachProperties stringToTypeOfRegularRiverReachProperties (std::string string);
	static std::string typeOfRegularRiverReachPropertiesToString (TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties);
	static bool regularRiverReachPropertyIsGrains (TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties);

	enum TypesOfAdditionalRiverReachPropertyAndMethod {BedrockRoughnessEquivalentRepresentativeGrainDiameter, PreventLocalGrainSizeDistributionChanges, InstantaneousSedimentInputs, SternbergAbrasionWithoutFining, SternbergAbrasionIncludingFining, ScourChain};
	static TypesOfAdditionalRiverReachPropertyAndMethod stringToTypeOfAdditionalRiverReachPropertyAndMethod (std::string string);
	static std::string typeOfAdditionalRiverReachPropertyAndMethodToString (TypesOfAdditionalRiverReachPropertyAndMethod typeOfAdditionalRiverReachPropertyAndMethod);

	enum TypesOfAdditionalRiverSystemPropertyAndMethod {};
	static TypesOfAdditionalRiverSystemPropertyAndMethod stringToTypeOfAdditionalRiverSystemPropertyAndMethod (std::string string);
	static std::string typeOfAdditionalRiverSystemPropertyAndMethodToString (TypesOfAdditionalRiverSystemPropertyAndMethod typeOfAdditionalRiverSystemPropertyAndMethod);

	enum TypesOfChangeRates {WaterdepthChange, RelativeTwoCellBedSlopeChange, EnergySlopeChange};
	static TypesOfChangeRates stringToTypeOfChangeRates (std::string string);
	static std::string typeOfChangeRateToString (TypesOfChangeRates typeOfChangeRates);

	enum TypesOfGeneralFlowMethods {WaterFlowMethodsInGeneral, SedimentFlowMethodsInGeneral};
	static TypesOfGeneralFlowMethods stringToTypeOfGeneralFlowMethods (std::string string);
	static std::string typeOfGeneralFlowMethodsToString (TypesOfGeneralFlowMethods typeOfGeneralFlowMethods);

	enum TypesOfFlowMethods {ImplicitKinematicWave, ExplicitKinematicWave, UniformDischarge, sedimentFlowMethods};
	static TypesOfFlowMethods stringToTypeOfFlowMethods (std::string string);
	static std::string typeOfFlowMethodsToString (TypesOfFlowMethods typeOfFlowMethods);

	enum TypesOfSedimentFlowMethods {SuspensionLoadFlowMethods, BedloadFlowMethods};
	static TypesOfSedimentFlowMethods stringToTypeOfSedimentFlowMethods (std::string string);
	static std::string typeOfSedimentFlowMethodsToString (TypesOfSedimentFlowMethods typeOfSedimentFlowMethods);

	enum TypesOfGradientCalculationMethod {SimpleDownstreamTwoCellGradient, SimpleThreeCellGradient, SimpleDownstreamTwoCellGradientWithCenteredValues, SimpleThreeCellGradientWithCenteredValues, ReducedWaterEnergyslope, ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable, ReturnBedslope, ReturnWaterEnergyslope};
	static TypesOfGradientCalculationMethod stringToTypeOfGradientCalculationMethod (std::string string);
	static std::string typeOfGradientCalculationMethodToString (TypesOfGradientCalculationMethod typeOfGradientCalculationMethod);

	enum TypesOfChangeRateModifiers {AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins, RecirculateWater, RecirculateSediment, InputPropertyTimeSeriesLinearlyInterpolated};
	static TypesOfChangeRateModifiers stringToTypeOfChangeRateModifiers (std::string string);
	static std::string typeOfChangeRateModifiersToString (TypesOfChangeRateModifiers typeOfChangeRateModifiers);

	enum TypesOfOutputMethod {OutputVerbatimTranslationOfConstructionVariablesToXML, OutputRegularRiverReachProperties, OutputRegularRiverReachPropertiesForVisualInterpretation, OutputAccumulatedBedloadTransport, OutputSimulationSetup};
	static TypesOfOutputMethod stringToTypeOfOutputMethod (std::string string);
	static std::string typeOfOutputMethodToString (TypesOfOutputMethod typeOfOutputMethod);

	enum TypesOfChannelGeometry {InfinitelyDeepRectangularChannel, InfinitelyDeepVShapedChannel};
	static TypesOfChannelGeometry stringToTypeOfChannelGeometry (std::string string);
	static std::string typeOfChannelGeometryToString (TypesOfChannelGeometry typeOfChannelGeometry);

	enum TypesOfGeometricalChannelBehaviour {ParallelShiftOfBasicGeometry};
	static TypesOfGeometricalChannelBehaviour stringToTypeOfGeometricalChannelBehaviour (std::string string);
	static std::string typeOfGeometricalChannelBehaviourToString (TypesOfGeometricalChannelBehaviour typeOfGeometricalChannelBehaviour);

	enum TypesOfStrataSorting {TwoLayerWithShearStressBasedUpdate, StratigraphyWithThresholdBasedUpdate, StratigraphyWithOLDConstantThresholdBasedUpdate, TwoLayerWithContinuousUpdate, SingleLayerNoSorting};
	static TypesOfStrataSorting stringToTypeOfStrataSorting (std::string string);
	static std::string typeOfStrataSortingToString (TypesOfStrataSorting typeOfStrataSorting);

	enum TypesOfSillProperties {PoleniSill};
	static TypesOfSillProperties stringToTypeOfSillProperties (std::string string);
	static std::string typeOfSillPropertiesToString (TypesOfSillProperties typeOfSillProperties);

	enum TypesOfFlowResistance {VariablePowerLawFlowResistance, FixedPowerLawFlowResistance, DarcyWeisbachFlowResistance, SolveForWaterEnergyslopeBasedOnHydraulicHead};
	static TypesOfFlowResistance stringToTypeOfFlowResistance (std::string string);
	static std::string typeOfFlowResistanceToString (TypesOfFlowResistance typeOfFlowResistance);

	enum TypesOfBedloadCapacityCalculationMethod {ChengBedloadCapacity,RickenmannBedloadCapacityBasedOnTheta, RickenmannBedloadCapacityBasedOnThetaNonFractional, RickenmannBedloadCapacityBasedOnq, RickenmannBedloadCapacityBasedOnqNonFractional, WilcockCroweBedloadCapacity, SchneiderEtAlBedloadCapacity, ReckingBedloadCapacityNonFractional};
	static TypesOfBedloadCapacityCalculationMethod stringToTypeOfBedloadCapacityCalculationMethod (std::string string);
	static std::string typeOfBedloadCapacityCalculationMethodToString (TypesOfBedloadCapacityCalculationMethod typeOfBedloadCapacityCalculationMethod);

	enum TypesOfTauCalculationMethod {EnergyslopeTauBasedOnFlowDepth, EnergyslopeTau, FlowVelocityTau};
	static TypesOfTauCalculationMethod stringToTypeOfTauCalculationMethod (std::string string);
	static std::string typeOfTauCalculationMethodToString (TypesOfTauCalculationMethod typeOfTauCalculationMethod);

	enum TypesOfBedloadVelocityCalculationMethod {JulienBounvilayRollingParticlesVelocity, VelocityAsTransportRatePerUnitCrossSectionalArea};
	static TypesOfBedloadVelocityCalculationMethod stringToTypeOfBedloadVelocityCalculationMethod (std::string string);
	static std::string typeOfBedloadVelocityCalculationMethodToString (TypesOfBedloadVelocityCalculationMethod typeOfBedloadVelocityCalculationMethod);

	enum TypesOfEstimationMethodForThicknessOfMovingSedimentLayer {MultipleDiameterOfCoarsestGrainMoved,MultipleReferenceGrainDiameter,ConstantThicknessOfMovingSedimentLayer};
	static TypesOfEstimationMethodForThicknessOfMovingSedimentLayer stringToTypeOfEstimationMethodForThicknessOfMovingSedimentLayer (std::string string);
	static std::string typeOfEstimationMethodForThicknessOfMovingSedimentLayerToString (TypesOfEstimationMethodForThicknessOfMovingSedimentLayer typeOfEstimationMethodForThicknessOfMovingSedimentLayer);

	enum TypesOfActiveWidthCalculationMethod {SetActiveWidthEqualFlowWidth};
	static TypesOfActiveWidthCalculationMethod stringToTypeOfActiveWidthCalculationMethod (std::string string);
	static std::string typeOfActiveWidthCalculationMethodToString (TypesOfActiveWidthCalculationMethod typeOfActiveWidthCalculationMethod);

	enum TypesOfCalculationMethodForBedloadMotionInitiationThreshold {LambEtAlCriticalTheta, ConstantThresholdForInitiationOfBedloadMotion, StochasticThresholdForInitiationOfBedloadMotion};
	static TypesOfCalculationMethodForBedloadMotionInitiationThreshold stringToTypeOfCalculationMethodForBedloadMotionInitiationThreshold (std::string string);
	static std::string typeOfCalculationMethodForBedloadMotionInitiationThresholdToString (TypesOfCalculationMethodForBedloadMotionInitiationThreshold typeOfCalculationMethodForBedloadMotionInitiationThreshold);

	enum TypesOfHidingFactorsCalculationMethod {PowerLawHidingFunction, WilcockCroweHidingFunction, NoHiding};
	static TypesOfHidingFactorsCalculationMethod stringToTypeOfHidingFactorsCalculationMethod (std::string string);
	static std::string typeOfHidingFactorsCalculationMethodToString (TypesOfHidingFactorsCalculationMethod typeOfHidingFactorsCalculationMethod);

private:
	static std::map< std::string, TypesOfUserInputReaders> mapForTypesOfUserInputReaders;
	static std::map< std::string, TypesOfUserInputReaders> createMapForTypesOfUserInputReaders();
	static std::map< std::string, TypesOfCombinersAndInterfaces> mapForTypesOfCombinersAndInterfaces;
	static std::map< std::string, TypesOfCombinersAndInterfaces> createMapForTypesOfCombinersAndInterfaces();
	static std::map< std::string, TypesOfNumericRootFinder> mapForTypesOfNumericRootFinder;
	static std::map< std::string, TypesOfNumericRootFinder> createMapForTypesOfNumericRootFinder();
	static std::map< std::string, TypesOfGrains> mapForTypesOfGrains;
	static std::map< std::string, TypesOfGrains> createMapForTypesOfGrains();
	static std::map< std::string, TypesOfRegularRiverReachProperties> mapForTypesOfRegularRiverReachProperties;
	static std::map< std::string, TypesOfRegularRiverReachProperties> createMapForTypesOfRegularRiverReachProperties();
	static std::map< std::string, TypesOfAdditionalRiverReachPropertyAndMethod> mapForTypesOfAdditionalRiverReachPropertyAndMethod;
	static std::map< std::string, TypesOfAdditionalRiverReachPropertyAndMethod> createMapForTypesOfAdditionalRiverReachPropertyAndMethod();
	static std::map< std::string, TypesOfAdditionalRiverSystemPropertyAndMethod> mapForTypesOfAdditionalRiverSystemPropertyAndMethod;
	static std::map< std::string, TypesOfAdditionalRiverSystemPropertyAndMethod> createMapForTypesOfAdditionalRiverSystemPropertyAndMethod();
	static std::map< std::string, TypesOfChangeRates> mapForTypesOfChangeRates;
	static std::map< std::string, TypesOfChangeRates> createMapForTypesOfChangeRates();
	static std::map< std::string, TypesOfFlowMethods> mapForTypesOfFlowMethods;
	static std::map< std::string, TypesOfFlowMethods> createMapForTypesOfFlowMethods();
	static std::map< std::string, TypesOfGeneralFlowMethods> mapForTypesOfGeneralFlowMethods;
	static std::map< std::string, TypesOfGeneralFlowMethods> createMapForTypesOfGeneralFlowMethods();
	static std::map< std::string, TypesOfSedimentFlowMethods> mapForTypesOfSedimentFlowMethods;
	static std::map< std::string, TypesOfSedimentFlowMethods> createMapForTypesOfSedimentFlowMethods();
	static std::map< std::string, TypesOfGradientCalculationMethod> mapForTypesOfGradientCalculationMethod;
	static std::map< std::string, TypesOfGradientCalculationMethod> createMapForTypesOfGradientCalculationMethod();
	static std::map< std::string, TypesOfChangeRateModifiers> mapForTypesOfChangeRateModifiers;
	static std::map< std::string, TypesOfChangeRateModifiers> createMapForTypesOfChangeRateModifiers();
	static std::map< std::string, TypesOfOutputMethod> mapForTypesOfOutputMethod;
	static std::map< std::string, TypesOfOutputMethod> createMapForTypesOfOutputMethod();
	static std::map< std::string, TypesOfChannelGeometry> mapForTypesOfChannelGeometry;
	static std::map< std::string, TypesOfChannelGeometry> createMapForTypesOfChannelGeometry();
	static std::map< std::string, TypesOfGeometricalChannelBehaviour> mapForTypesOfGeometricalChannelBehaviour;
	static std::map< std::string, TypesOfGeometricalChannelBehaviour> createMapForTypesOfGeometricalChannelBehaviour();
	static std::map< std::string, TypesOfStrataSorting> mapForTypesOfStrataSorting;
	static std::map< std::string, TypesOfStrataSorting> createMapForTypesOfStrataSorting();
	static std::map< std::string, TypesOfSillProperties> mapForTypesOfSillProperties;
	static std::map< std::string, TypesOfSillProperties> createMapForTypesOfSillProperties();
	static std::map< std::string, TypesOfFlowResistance> mapForTypesOfFlowResistance;
	static std::map< std::string, TypesOfFlowResistance> createMapForTypesOfFlowResistance();
	static std::map< std::string, TypesOfBedloadCapacityCalculationMethod> mapForTypesOfBedloadCapacityCalculationMethod;
	static std::map< std::string, TypesOfBedloadCapacityCalculationMethod> createMapForTypesOfBedloadCapacityCalculationMethod();
	static std::map< std::string, TypesOfTauCalculationMethod> mapForTypesOfTauCalculationMethod;
	static std::map< std::string, TypesOfTauCalculationMethod> createMapForTypesOfTauCalculationMethod();
	static std::map< std::string, TypesOfBedloadVelocityCalculationMethod> mapForTypesOfBedloadVelocityCalculationMethod;
	static std::map< std::string, TypesOfBedloadVelocityCalculationMethod> createMapForTypesOfBedloadVelocityCalculationMethod();
	static std::map< std::string, TypesOfEstimationMethodForThicknessOfMovingSedimentLayer> mapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer;
	static std::map< std::string, TypesOfEstimationMethodForThicknessOfMovingSedimentLayer> createMapForTypesOfEstimationMethodForThicknessOfMovingSedimentLayer();
	static std::map< std::string, TypesOfActiveWidthCalculationMethod> mapForTypesOfActiveWidthCalculationMethod;
	static std::map< std::string, TypesOfActiveWidthCalculationMethod> createMapForTypesOfActiveWidthCalculationMethod();
	static std::map< std::string, TypesOfCalculationMethodForBedloadMotionInitiationThreshold> mapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold;
	static std::map< std::string, TypesOfCalculationMethodForBedloadMotionInitiationThreshold> createMapForTypesOfCalculationMethodForBedloadMotionInitiationThreshold();
	static std::map< std::string, TypesOfHidingFactorsCalculationMethod> mapForTypesOfHidingFactorsCalculationMethod;
	static std::map< std::string, TypesOfHidingFactorsCalculationMethod> createMapForTypesOfHidingFactorsCalculationMethod();
};

}

#endif /* COMBINERVARIABLES_H_ */
