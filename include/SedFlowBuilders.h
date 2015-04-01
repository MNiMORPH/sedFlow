/*
 * SedFlowBuilders.h
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

#ifndef SEDFLOWBUILDERS_H_
#define SEDFLOWBUILDERS_H_

#include "ConstructionVariables.h"
#include "HighestOrderStructuresPointers.h"
#include "SedFlowInterfaceRealisationBuilders.h"

#include "OutputMethodType.h"
#include "AdditionalRiverSystemProperties.h"
#include "AdditionalRiverSystemPropertyType.h"
#include "RegularRiverSystemProperties.h"
#include "RiverReachProperties.h"
#include "AdditionalRiverReachProperties.h"
#include "AdditionalRiverReachPropertyType.h"
#include "RegularRiverReachProperties.h"
#include "Grains.h"
#include "GrainType.h"
#include "GeometricalChannelBehaviour.h"
#include "ChannelGeometry.h"
#include "StrataSorting.h"
#include "SillProperties.h"
#include "CalcGradient.h"
#include "FlowResistance.h"
#include "AdditionalRiverSystemMethods.h"
#include "AdditionalRiverSystemMethodType.h"
#include "RegularRiverSystemMethods.h"
#include "FlowMethods.h"
#include "FlowTypeMethods.h"
#include "SedimentFlowMethods.h"
#include "SedimentFlowTypeMethods.h"
#include "CalcBedloadCapacity.h"
#include "ChangeRateModifiers.h"
#include "ChangeRateModifiersForSingleFlowMethod.h"
#include "ChangeRateModifiersType.h"
#include "RiverReachMethods.h"
#include "AdditionalRiverReachMethods.h"
#include "AdditionalRiverReachMethodType.h"
#include "RegularRiverReachMethods.h"
#include "CalcTau.h"
#include "CalcBedloadVelocity.h"
#include "EstimateThicknessOfMovingSedimentLayer.h"
#include "CalcActiveWidth.h"
#include "CalcThresholdForInitiationOfBedloadMotion.h"
#include "CalcHidingFactors.h"
#include "TimeSeriesEntry.h"
#include "TimeSeries.h"

namespace SedFlow {

class SedFlowBuilders {
public:

	static void* generalBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers);
	static void* specificBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers,
			std::map< std::string, std::vector<void*> > labelledObjects);
	static void specificDelete (CombinerVariables::TypesOfCombinersAndInterfaces interfaceOrCombinerType, void* toDelete);

	static OverallParameters* overallParametersBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers);
	static OverallMethods* overallMethodsBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers);
	static RiverSystemProperties* riverSystemPropertiesBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers);
	static RiverSystemMethods* riverSystemMethodsBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers);
	static OutputMethods* outputMethodsBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers);

	static OutputMethodType* outputMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverSystemProperties* additionalRiverSystemPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverSystemPropertyType* additionalRiverSystemPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static RegularRiverSystemProperties* regularRiverSystemPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static RiverReachProperties* riverReachPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverReachProperties* additionalRiverReachPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverReachPropertyType* additionalRiverReachPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static RegularRiverReachProperties* regularRiverReachPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static Grains* grainsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static GrainType* grainTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static GeometricalChannelBehaviour* geometricalChannelBehaviourBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static ChannelGeometry* channelGeometryBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static StrataSorting* strataSortingBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static SillProperties* sillPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcGradient* calcGradientBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static FlowResistance* flowResistanceBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverSystemMethods* additionalRiverSystemMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverSystemMethodType* additionalRiverSystemMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static RegularRiverSystemMethods* regularRiverSystemMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static FlowMethods* flowMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static FlowTypeMethods* flowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static SedimentFlowMethods* sedimentFlowMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static SedimentFlowTypeMethods* sedimentFlowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcBedloadCapacity* calcBedloadCapacityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static ChangeRateModifiers* changeRateModifiersBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static ChangeRateModifiersForSingleFlowMethod* changeRateModifiersForSingleFlowMethodBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static ChangeRateModifiersType* changeRateModifiersTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static RiverReachMethods* riverReachMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverReachMethods* additionalRiverReachMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverReachMethodType* additionalRiverReachMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static RegularRiverReachMethods* regularRiverReachMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcTau* calcTauBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcBedloadVelocity* calcBedloadVelocityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static EstimateThicknessOfMovingSedimentLayer* estimateThicknessOfMovingSedimentLayerBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcActiveWidth* calcActiveWidthBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcThresholdForInitiationOfBedloadMotion* calcThresholdForInitiationOfBedloadMotionBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcHidingFactors* calcHidingFactorsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static TimeSeriesEntry* timeSeriesEntryBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static TimeSeries* timeSeriesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);

};

}

#endif /* SEDFLOWBUILDERS_H_ */
