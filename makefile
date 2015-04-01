###################################################################################################
#                                 Makefile for the sedFlow model                                  #
###################################################################################################
#
#   Copyright (C) 2014 Swiss Federal Research Institute WSL (http://www.wsl.ch)
#   Developed by F.U.M. Heimann
#   Published by the Swiss Federal Research Institute WSL
#   
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License version 3
#   as published by the Free Software Foundation.
#   
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
#   without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
#   See the GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with this program. If not, see http://www.gnu.org/licenses
#   
#   This software is part of the model sedFlow,
#   which is intended for the simulation of bedload dynamics in mountain streams.
#   
#   For details on sedFlow see http://www.wsl.ch/sedFlow


CURRENT_DIR = $(dir $(lastword $(MAKEFILE_LIST)))
PROGRAM_NAME = sedFlow
PROGRAM_VERSION	= 1.00
LIB_NAME = SedFlow

BIN_PATH = $(CURRENT_DIR)/bin
TMP_PATH = $(CURRENT_DIR)/tmp
SRC_PATH = $(CURRENT_DIR)/src
LIB_PATH = $(CURRENT_DIR)/lib
SUBLIB_PATH = $(LIB_PATH)/auxiliaryLibraries
INCLUDE_PATH = $(CURRENT_DIR)/include
DOC_PATH = $(CURRENT_DIR)/doc

ALL_INCLUDE_PATHS = -I$(INCLUDE_PATH)
ALL_LIB_PATHS = -L$(LIB_PATH)
ALL_EXTERNAL_LIBS = 
ALL_LIBS = -l$(LIB_NAME) $(ALL_EXTERNAL_LIBS)

CXX = g++
AR = ar rucs
TeX = pdflatex

OPTIMISATION_LEVEL = 1
# On Windows there is no efficiency gain from O1 to O2, but loss in accuracy. On Linux O2 is more efficient with no significant loss in accuracy.
# Aggressive but stable (??) flags: -ffast-math -fmath-errno -fno-finite-math-only
# Less aggressive but equally efficient flag: -funsafe-math-optimizations
# Which of the unsafe-math-optimizations is the most efficient depends on the gcc version.
# Usually parallelisation is not worth doing it.

ifdef DEBUG
   ifdef SystemRoot
               CXX_FLAGS = -g -static -DDEBUG -O0 -Wuninitialized -Winit-self
   else
      ifeq ($(shell uname), Linux)
               CXX_FLAGS = -g -DDEBUG -O0 -Wuninitialized -Winit-self
      endif
   endif
else
   ifdef SystemRoot
         ifndef PARALLEL
               CXX_FLAGS = -static
         endif
               CXX_FLAGS += -static-libgcc -static-libstdc++ -O$(OPTIMISATION_LEVEL)
         ifdef PARALLEL
               CXX_FLAGS += -fopenmp -DSEDFLOWPARALLEL
         endif
   else
      ifeq ($(shell uname), Linux)
               CXX_FLAGS = -O$(OPTIMISATION_LEVEL)
            ifdef PARALLEL
               CXX_FLAGS += -openmp -DSEDFLOWPARALLEL
            endif
      endif
   endif
endif

ifdef TIMESTEPANALYSIS
   CXX_FLAGS += -DTIMESTEPANALYSIS=$(TIMESTEPANALYSIS)
endif

ifdef SystemRoot
   CXX_FLAGS += -DCURRENTLYWINDOWS
else
   CXX_FLAGS += -DCURRENTLYUNIX
endif

CXX_FLAGS += -DSEDFLOWVERSION=$(PROGRAM_VERSION)

ifdef SystemRoot
   RM = del /Q
   FixPath = $(subst /,\,$1)
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = $1
   endif
endif

PROGRAM = $(BIN_PATH)/$(PROGRAM_NAME)
LIBRARY = $(LIB_PATH)/lib$(LIB_NAME).a
DOCUMENTATION = $(DOC_PATH)/$(PROGRAM_NAME).html
LIB_DOCUMENTATION = $(DOC_PATH)/$(LIB_NAME).html

OBJECTS = $(METHOD_COMBINEROBJECTS) $(COMPLEMENTARY_METHOD_IMPLEMENTATIONOBJECTS) $(MUTUALLYEXCLUSIVE_METHOD_IMPLEMENTATIONOBJECTS) $(COMPLEMENTARY_METHOD_INTERFACEOBJECTS) $(MUTUALLYEXCLUSIVE_METHOD_INTERFACEOBJECTS) $(SIMPLE_METHODSET_OBJECTS) $(PARAMETER_COMBINEROBJECTS) $(COMPLEMENTARY_PARAMETER_IMPLEMENTATIONOBJECTS) $(MUTUALLYEXCLUSIVE_PARAMETER_IMPLEMENTATIONOBJECTS) $(COMPLEMENTARY_PARAMETER_INTERFACEOBJECTS) $(MUTUALLYEXCLUSIVE_PARAMETER_INTERFACEOBJECTS) $(SIMPLE_PARAMETERSET_OBJECTS) 
METHOD_COMBINEROBJECTS = $(TMP_PATH)/SedFlowCore.o $(TMP_PATH)/SedFlowBuilders.o $(TMP_PATH)/SedFlowInterfaceRealisationBuilders.o $(TMP_PATH)/SedFlowNumericSolverRealisationBuilders.o $(TMP_PATH)/HighestOrderStructuresPointers.o $(TMP_PATH)/OutputMethods.o $(TMP_PATH)/RiverSystemMethods.o $(TMP_PATH)/AdditionalRiverSystemMethods.o $(TMP_PATH)/RegularRiverSystemMethods.o $(TMP_PATH)/FlowMethods.o $(TMP_PATH)/SedimentFlowMethods.o $(TMP_PATH)/RegularRiverSystemMethods.o $(TMP_PATH)/RiverReachMethods.o $(TMP_PATH)/RegularRiverReachMethods.o  $(TMP_PATH)/ChangeRateModifiers.o $(TMP_PATH)/ChangeRateModifiersForSingleFlowMethod.o $(TMP_PATH)/AdditionalRiverReachMethods.o $(TMP_PATH)/OverallMethods.o
COMPLEMENTARY_METHOD_IMPLEMENTATIONOBJECTS = $(TMP_PATH)/SuspensionLoadFlowMethods.o $(TMP_PATH)/BedloadFlowMethods.o $(TMP_PATH)/ImplicitKinematicWave.o $(TMP_PATH)/ExplicitKinematicWave.o $(TMP_PATH)/UniformDischarge.o $(TMP_PATH)/ScourChainMethods.o $(TMP_PATH)/PreventLocalGrainSizeDistributionChanges.o $(TMP_PATH)/InstantaneousSedimentInputs.o $(TMP_PATH)/SternbergAbrasionWithoutFining.o $(TMP_PATH)/SternbergAbrasionIncludingFining.o $(TMP_PATH)/OutputVerbatimTranslationOfConstructionVariablesToXML.o $(TMP_PATH)/OutputRegularRiverReachProperties.o $(TMP_PATH)/OutputRegularRiverReachPropertiesForVisualInterpretation.o $(TMP_PATH)/OutputAccumulatedBedloadTransport.o $(TMP_PATH)/OutputSimulationSetup.o $(TMP_PATH)/AdjustDownstreamTwoCellBedAndWaterSurfaceSlopeAtMargins.o $(TMP_PATH)/RecirculateWater.o $(TMP_PATH)/RecirculateSediment.o $(TMP_PATH)/InputPropertyTimeSeriesLinearlyInterpolated.o
MUTUALLYEXCLUSIVE_METHOD_IMPLEMENTATIONOBJECTS = $(TMP_PATH)/VelocityAsTransportRatePerUnitCrossSectionalArea.o $(TMP_PATH)/JulienBounvilayRollingParticlesVelocity.o $(TMP_PATH)/MultipleDiameterOfCoarsestGrainMoved.o $(TMP_PATH)/MultipleReferenceGrainDiameter.o $(TMP_PATH)/ConstantThicknessOfMovingSedimentLayer.o $(TMP_PATH)/EnergyslopeTau.o $(TMP_PATH)/EnergyslopeTauBasedOnFlowDepth.o $(TMP_PATH)/FlowVelocityTau.o $(TMP_PATH)/ReducedWaterEnergyslopeNotUsingWaterEnergyslopeVariable.o $(TMP_PATH)/ReducedWaterEnergyslope.o $(TMP_PATH)/SimpleThreeCellGradient.o $(TMP_PATH)/SimpleThreeCellGradientWithCenteredValues.o $(TMP_PATH)/SimpleDownstreamTwoCellGradient.o $(TMP_PATH)/SimpleDownstreamTwoCellGradientWithCenteredValues.o $(TMP_PATH)/ChengBedloadCapacity.o $(TMP_PATH)/RickenmannBedloadCapacityBasedOnTheta.o $(TMP_PATH)/RickenmannBedloadCapacityBasedOnThetaNonFractional.o $(TMP_PATH)/RickenmannBedloadCapacityBasedOnq.o $(TMP_PATH)/RickenmannBedloadCapacityBasedOnqNonFractional.o $(TMP_PATH)/WilcockCroweBedloadCapacity.o $(TMP_PATH)/SchneiderEtAlBedloadCapacity.o $(TMP_PATH)/ReckingBedloadCapacityNonFractional.o $(TMP_PATH)/SolveForWaterEnergyslopeBasedOnHydraulicHead.o $(TMP_PATH)/ReturnBedslope.o $(TMP_PATH)/ReturnWaterEnergyslope.o $(TMP_PATH)/VariablePowerLawFlowResistance.o $(TMP_PATH)/FixedPowerLawFlowResistance.o $(TMP_PATH)/DarcyWeisbachFlowResistance.o $(TMP_PATH)/SetActiveWidthEqualFlowWidth.o $(TMP_PATH)/LambEtAlCriticalTheta.o $(TMP_PATH)/ConstantThresholdForInitiationOfBedloadMotion.o $(TMP_PATH)/StochasticThresholdForInitiationOfBedloadMotion.o $(TMP_PATH)/PowerLawHidingFunction.o $(TMP_PATH)/WilcockCroweHidingFunction.o $(TMP_PATH)/NoHiding.o $(TMP_PATH)/ParallelShiftOfBasicGeometry.o $(TMP_PATH)/InfinitelyDeepRectangularChannel.o $(TMP_PATH)/InfinitelyDeepVShapedChannel.o $(TMP_PATH)/VerbatimTranslationFromXMLToConstructionVariables.o $(TMP_PATH)/StandardInput.o $(TMP_PATH)/SingleLayerNoSorting.o $(TMP_PATH)/TwoLayerWithShearStressBasedUpdate.o $(TMP_PATH)/TwoLayerWithContinuousUpdate.o $(TMP_PATH)/StratigraphyWithThresholdBasedUpdate.o $(TMP_PATH)/StratigraphyWithOLDConstantThresholdBasedUpdate.o $(TMP_PATH)/PoleniSill.o $(TMP_PATH)/BisectionMethod.o $(TMP_PATH)/SecantMethod.o $(TMP_PATH)/FalsePositionMethod.o $(TMP_PATH)/RiddersMethod.o $(TMP_PATH)/BrentMethod.o
COMPLEMENTARY_METHOD_INTERFACEOBJECTS = $(TMP_PATH)/AdditionalRiverSystemMethodType.o $(TMP_PATH)/AdditionalRiverReachMethodType.o $(TMP_PATH)/SedimentFlowTypeMethods.o $(TMP_PATH)/FlowTypeMethods.o $(TMP_PATH)/OutputMethodType.o $(TMP_PATH)/ChangeRateModifiersType.o $(TMP_PATH)/UserInputReader.o
MUTUALLYEXCLUSIVE_METHOD_INTERFACEOBJECTS = $(TMP_PATH)/CalcBedloadVelocity.o $(TMP_PATH)/EstimateThicknessOfMovingSedimentLayer.o $(TMP_PATH)/CalcActiveWidth.o $(TMP_PATH)/CalcBedloadCapacity.o $(TMP_PATH)/CalcGradient.o $(TMP_PATH)/CalcTau.o $(TMP_PATH)/CalcThresholdForInitiationOfBedloadMotion.o $(TMP_PATH)/CalcHidingFactors.o $(TMP_PATH)/FlowResistance.o $(TMP_PATH)/GeometricalChannelBehaviour.o $(TMP_PATH)/ChannelGeometry.o $(TMP_PATH)/SillProperties.o $(TMP_PATH)/NumericRootFinder.o
SIMPLE_METHODSET_OBJECTS = $(TMP_PATH)/BasicCalculations.o $(TMP_PATH)/CorrectionForBedloadWeightAtSteepSlopes.o $(TMP_PATH)/CellIDConversions.o $(TMP_PATH)/BedrockRoughnessContribution.o $(TMP_PATH)/StringTools.o
PARAMETER_COMBINEROBJECTS = $(TMP_PATH)/RiverSystemProperties.o $(TMP_PATH)/RegularRiverSystemProperties.o $(TMP_PATH)/AdditionalRiverSystemProperties.o $(TMP_PATH)/RiverReachProperties.o $(TMP_PATH)/AdditionalRiverReachProperties.o $(TMP_PATH)/RegularRiverReachProperties.o $(TMP_PATH)/TimeSeries.o $(TMP_PATH)/TimeSeriesEntry.o $(TMP_PATH)/StrataSorting.o $(TMP_PATH)/Grains.o
COMPLEMENTARY_PARAMETER_IMPLEMENTATIONOBJECTS = $(TMP_PATH)/FishEggs.o $(TMP_PATH)/TracerGrains.o $(TMP_PATH)/NormalGrains.o $(TMP_PATH)/BedrockRoughnessEquivalentRepresentativeGrainDiameter.o $(TMP_PATH)/ScourChainProperties.o
MUTUALLYEXCLUSIVE_PARAMETER_IMPLEMENTATIONOBJECTS = 
COMPLEMENTARY_PARAMETER_INTERFACEOBJECTS = $(TMP_PATH)/AdditionalRiverSystemPropertyType.o $(TMP_PATH)/AdditionalRiverReachPropertyType.o $(TMP_PATH)/GrainType.o
MUTUALLYEXCLUSIVE_PARAMETER_INTERFACEOBJECTS = 
SIMPLE_PARAMETERSET_OBJECTS = $(TMP_PATH)/OverallParameters.o $(TMP_PATH)/ConstructionVariables.o $(TMP_PATH)/CombinerVariables.o $(TMP_PATH)/PowerLawRelation.o $(TMP_PATH)/pugixml.o

SUBLIBRARIES = $(SUBLIB_PATH)/libMethodCombiners.a $(SUBLIB_PATH)/libComplementaryMethodImplementations.a $(SUBLIB_PATH)/libMutuallyExclusiveMethodImplementations.a $(SUBLIB_PATH)/libComplementaryMethodInterfaces.a $(SUBLIB_PATH)/libMutuallyExclusiveMethodInterfaces.a $(SUBLIB_PATH)/libSimpleMethodsets.a $(SUBLIB_PATH)/libParameterCombiners.a $(SUBLIB_PATH)/libComplementaryParameterImplementations.a $(SUBLIB_PATH)/libMutuallyExclusiveParameterImplementations.a $(SUBLIB_PATH)/libComplementaryParameterInterfaces.a $(SUBLIB_PATH)/libMutuallyExclusiveParameterInterfaces.a $(SUBLIB_PATH)/libSimpleParametersets.a

all:
	make $(PROGRAM)
ifneq ($(DEBUG),1)
	make CLEAN
endif

$(PROGRAM): $(SRC_PATH)/$(PROGRAM_NAME).cpp $(LIBRARY)
	$(CXX) $(CXX_FLAGS) $(ALL_INCLUDE_PATHS) -o $(BIN_PATH)/$(PROGRAM_NAME) $(SRC_PATH)/$(PROGRAM_NAME).cpp $(ALL_LIB_PATHS) $(ALL_LIBS)
	
$(LIBRARY): $(OBJECTS) $(SUBLIBRARIES)
	$(AR) $@ $(OBJECTS)

INFO:
	$(CXX) -dumpmachine
	$(CXX) -v
	$(CXX) --version

CLEAN:
	$(RM) $(call FixPath, $(TMP_PATH)/*.*)
	
CLEAR_ALL:
	make CLEAN
	$(RM) $(call FixPath, $(BIN_PATH)/*.*)
	$(RM) $(call FixPath, $(LIB_PATH)/*.*)
	$(RM) $(call FixPath, $(SUBLIB_PATH)/*.*)
ifndef SystemRoot
   ifeq ($(shell uname), Linux)
	$(RM) $(call FixPath, $(BIN_PATH)/$(PROGRAM_NAME))
   endif
endif

####################### Individual sublibraries ####################################################

$(SUBLIB_PATH)/libMethodCombiners.a: $(METHOD_COMBINEROBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libComplementaryMethodImplementations.a: $(COMPLEMENTARY_METHOD_IMPLEMENTATIONOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libMutuallyExclusiveMethodImplementations.a: $(MUTUALLYEXCLUSIVE_METHOD_IMPLEMENTATIONOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libComplementaryMethodInterfaces.a: $(COMPLEMENTARY_METHOD_INTERFACEOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libMutuallyExclusiveMethodInterfaces.a: $(MUTUALLYEXCLUSIVE_METHOD_INTERFACEOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libSimpleMethodsets.a: $(SIMPLE_METHODSET_OBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libParameterCombiners.a: $(PARAMETER_COMBINEROBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libComplementaryParameterImplementations.a: $(COMPLEMENTARY_PARAMETER_IMPLEMENTATIONOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libMutuallyExclusiveParameterImplementations.a: $(MUTUALLYEXCLUSIVE_PARAMETER_IMPLEMENTATIONOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libComplementaryParameterInterfaces.a: $(COMPLEMENTARY_PARAMETER_INTERFACEOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libMutuallyExclusiveParameterInterfaces.a: $(MUTUALLYEXCLUSIVE_PARAMETER_INTERFACEOBJECTS)
	$(AR) $@ $^

$(SUBLIB_PATH)/libSimpleParametersets.a: $(SIMPLE_PARAMETERSET_OBJECTS)
	$(AR) $@ $^


####################### End of Individual libraries ####################################################

$(TMP_PATH)/%.o: $(SRC_PATH)/%.cpp $(INCLUDE_PATH)/%.h
	$(CXX) -c $(CXX_FLAGS) $(ALL_INCLUDE_PATHS) -o $(TMP_PATH)/$*.o $< $(ALL_LIB_PATHS) $(ALL_EXTERNAL_LIBS)
