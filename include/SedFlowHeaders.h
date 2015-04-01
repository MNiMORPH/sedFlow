/*
 * SedFlowHeaders.h
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

#ifndef SEDFLOWHEADERS_H_
#define SEDFLOWHEADERS_H_

#include "ScratchClass.h"

#include "AdditionalRiverReachMethods.h"
#include "AdditionalRiverReachMethodType.h"
#include "AdditionalRiverReachProperties.h"
#include "AdditionalRiverReachPropertyType.h"
#include "AdditionalRiverSystemMethods.h"
#include "AdditionalRiverSystemMethodType.h"
#include "AdditionalRiverSystemProperties.h"
#include "AdditionalRiverSystemPropertyType.h"
#include "AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.h"
#include "BasicCalculations.h"
#include "BedloadFlowMethods.h"
#include "BedrockRoughnessContribution.h"
#include "BedrockRoughnessEquivalentRepresentativeGrainDiameter.h"
#include "BisectionMethod.h"
#include "BrentMethod.h"
#include "CalcActiveWidth.h"
#include "CalcBedloadCapacity.h"
#include "CalcBedloadVelocity.h"
#include "CalcGradient.h"
#include "CalcHidingFactors.h"
#include "CalcTau.h"
#include "CalcThresholdForInitiationOfBedloadMotion.h"
#include "CellIDConversions.h"
#include "ChangeRateModifiers.h"
#include "ChangeRateModifiersForSingleFlowMethod.h"
#include "ChangeRateModifiersType.h"
#include "ChannelGeometry.h"
#include "ChengBedloadCapacity.h"
#include "CombinerVariables.h"
#include "ConsoleTools.h"
#include "ConstantThicknessOfMovingSedimentLayer.h"
#include "ConstructionVariables.h"
#include "CorrectionForBedloadWeightAtSteepSlopes.h"
#include "DarcyWeisbachFlowResistance.h"
#include "EnergyslopeTauBasedOnFlowDepth.h"
#include "EnergyslopeTau.h"
#include "EstimateThicknessOfMovingSedimentLayer.h"
#include "ExplicitKinematicWave.h"
#include "FalsePositionMethod.h"
#include "FishEggs.h"
#include "FlowMethods.h"
#include "FlowTypeMethods.h"
#include "FlowVelocityTau.h"
#include "FixedPowerLawFlowResistance.h"
#include "GeometricalChannelBehaviour.h"
#include "Grains.h"
#include "GrainType.h"
#include "HighestOrderStructuresPointers.h"
#include "InfinitelyDeepRectangularChannel.h"
#include "InfinitelyDeepVShapedChannel.h"
#include "InputPropertyTimeSeriesLinearlyInterpolated.h"
#include "InstantaneousSedimentInputs.h"
#include "JulienBounvilayRollingParticlesVelocity.h"
#include "LambEtAlCriticalTheta.h"
#include "MultipleDiameterOfCoarsestGrainMoved.h"
#include "MultipleReferenceGrainDiameter.h"
#include "NormalGrains.h"
#include "NoHiding.h"
#include "NumericRootFinder.h"
#include "OutputAccumulatedBedloadTransport.h"
#include "OutputMethods.h"
#include "OutputMethodType.h"
#include "OutputRegularRiverReachProperties.h"
#include "OutputRegularRiverReachPropertiesForVisualInterpretation.h"
#include "OutputSimulationSetup.h"
#include "OutputVerbatimTranslationOfConstructionVariablesToXML.h"
#include "OverallMethods.h"
#include "OverallParameters.h"
#include "ParallelShiftOfBasicGeometry.h"
#include "FlowResistance.h"
#include "UniformDischarge.h"
#include "PoleniSill.h"
#include "PowerLawRelation.h"
#include "PreventLocalGrainSizeDistributionChanges.h"
#include "RecirculateSediment.h"
#include "RecirculateWater.h"
#include "ReckingBedloadCapacityNonFractional.h"
#include "ReducedWaterEnergyslope.h"
#include "ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.h"
#include "RegularRiverReachMethods.h"
#include "RegularRiverReachProperties.h"
#include "RegularRiverSystemMethods.h"
#include "RegularRiverSystemProperties.h"
#include "PowerLawHidingFunction.h"
#include "ReturnBedslope.h"
#include "ConstantThresholdForInitiationOfBedloadMotion.h"
#include "ReturnWaterEnergyslope.h"
#include "RickenmannBedloadCapacityBasedOnq.h"
#include "RickenmannBedloadCapacityBasedOnqNonFractional.h"
#include "RickenmannBedloadCapacityBasedOnTheta.h"
#include "RickenmannBedloadCapacityBasedOnThetaNonFractional.h"
#include "VariablePowerLawFlowResistance.h"
#include "RiddersMethod.h"
#include "RiverReachMethods.h"
#include "RiverReachProperties.h"
#include "RiverSystemMethods.h"
#include "RiverSystemProperties.h"
#include "SchneiderEtAlBedloadCapacity.h"
#include "ScourChainMethods.h"
#include "ScourChainProperties.h"
#include "SecantMethod.h"
#include "SedFlowCore.h"
#include "SedFlowBuilders.h"
#include "SedFlowInterfaceRealisationBuilders.h"
#include "SedFlowNumericSolverRealisationBuilders.h"
#include "SedimentFlowMethods.h"
#include "SedimentFlowTypeMethods.h"
#include "SetActiveWidthEqualFlowWidth.h"
#include "SillProperties.h"
#include "SimpleDownstreamTwoCellGradient.h"
#include "SimpleDownstreamTwoCellGradientWithCenteredValues.h"
#include "SimpleThreeCellGradient.h"
#include "SimpleThreeCellGradientWithCenteredValues.h"
#include "StandardInput.h"
#include "SingleLayerNoSorting.h"
#include "SolveForWaterEnergyslopeBasedOnHydraulicHead.h"
#include "SternbergAbrasionIncludingFining.h"
#include "SternbergAbrasionWithoutFining.h"
#include "StochasticThresholdForInitiationOfBedloadMotion.h"
#include "StrataSorting.h"
#include "StratigraphyWithOLDConstantThresholdBasedUpdate.h"
#include "StratigraphyWithThresholdBasedUpdate.h"
#include "StringTools.h"
#include "SuspensionLoadFlowMethods.h"
#include "TimeSeries.h"
#include "TimeSeriesEntry.h"
#include "ImplicitKinematicWave.h"
#include "TracerGrains.h"
#include "TwoLayerWithContinuousUpdate.h"
#include "TwoLayerWithShearStressBasedUpdate.h"
#include "UserInputReader.h"
#include "VelocityAsTransportRatePerUnitCrossSectionalArea.h"
#include "VerbatimTranslationFromXMLToConstructionVariables.h"
#include "WilcockCroweBedloadCapacity.h"
#include "WilcockCroweHidingFunction.h"

#endif /* SEDFLOWHEADERS_H_ */
