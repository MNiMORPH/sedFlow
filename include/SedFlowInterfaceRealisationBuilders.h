/*
 * SedFlowInterfaceRealisationBuilders.h
 *
 *   This software is based on pugixml library (http://pugixml.org).
 *   pugixml is Copyright (C) 2006-2012 Arseny Kapoulkine.
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

#ifndef SEDFLOWINTERFACEREALISATIONBUILDERS_H_
#define SEDFLOWINTERFACEREALISATIONBUILDERS_H_

#include <string>

#include "ConstructionVariables.h"
#include "HighestOrderStructuresPointers.h"

#include "UserInputReader.h"
#include "OutputMethodType.h"
#include "AdditionalRiverSystemPropertyType.h"
#include "AdditionalRiverReachPropertyType.h"
#include "GrainType.h"
#include "GeometricalChannelBehaviour.h"
#include "ChannelGeometry.h"
#include "StrataSorting.h"
#include "SillProperties.h"
#include "CalcGradient.h"
#include "FlowResistance.h"
#include "AdditionalRiverSystemMethodType.h"
#include "FlowTypeMethods.h"
#include "SedimentFlowTypeMethods.h"
#include "CalcBedloadCapacity.h"
#include "ChangeRateModifiersType.h"
#include "AdditionalRiverReachMethodType.h"
#include "CalcTau.h"
#include "CalcBedloadVelocity.h"
#include "EstimateThicknessOfMovingSedimentLayer.h"
#include "CalcActiveWidth.h"
#include "CalcThresholdForInitiationOfBedloadMotion.h"
#include "CalcHidingFactors.h"

namespace SedFlow {

class SedFlowInterfaceRealisationBuilders {
public:
	SedFlowInterfaceRealisationBuilders(){}
	virtual ~SedFlowInterfaceRealisationBuilders(){}

	static UserInputReader* userInputReaderBuilder (std::string xmlRootName);
	static OutputMethodType* outputMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverSystemPropertyType* additionalRiverSystemPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverReachPropertyType* additionalRiverReachPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static GrainType* grainTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static GeometricalChannelBehaviour* geometricalChannelBehaviourBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static ChannelGeometry* channelGeometryBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static StrataSorting* strataSortingBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static SillProperties* sillPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcGradient* calcGradientBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static FlowResistance* flowResistanceBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverSystemMethodType* additionalRiverSystemMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static FlowTypeMethods* flowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static SedimentFlowTypeMethods* sedimentFlowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcBedloadCapacity* calcBedloadCapacityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static ChangeRateModifiersType* changeRateModifiersTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static AdditionalRiverReachMethodType* additionalRiverReachMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcTau* calcTauBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcBedloadVelocity* calcBedloadVelocityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static EstimateThicknessOfMovingSedimentLayer* estimateThicknessOfMovingSedimentLayerBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcActiveWidth* calcActiveWidthBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcThresholdForInitiationOfBedloadMotion* calcThresholdForInitiationOfBedloadMotionBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);
	static CalcHidingFactors* calcHidingFactorsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects);

};

}

#endif /* SEDFLOWINTERFACEREALISATIONBUILDERS_H_ */
