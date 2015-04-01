/*
 * SedFlowBuilders.cpp
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

#include "SedFlowBuilders.h"

namespace SedFlow {

void* SedFlowBuilders::generalBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers)
{
	std::vector<void*> emptyVector;
	std::map< std::string, std::vector<void*> >::iterator tempLabelledObjectsIterator;
	std::map< std::string, std::vector<void*> > tempLabelledObjects;
	std::vector< std::vector<void*> > tempUnlabelledObjects;

	//Create the temporary labelled objects
	for(std::map< std::string, std::vector<ConstructionVariables> >::const_iterator currentOuterIterator = constructionVariables.labelledObjects.begin(); currentOuterIterator != constructionVariables.labelledObjects.end(); ++currentOuterIterator)
	{
		tempLabelledObjects[(*currentOuterIterator).first] = emptyVector;
		tempLabelledObjectsIterator = tempLabelledObjects.find((*currentOuterIterator).first);

		for(std::vector<ConstructionVariables>::const_iterator currentInnerIterator = (*currentOuterIterator).second.begin(); currentInnerIterator < (*currentOuterIterator).second.end(); ++currentInnerIterator)
				{ (*tempLabelledObjectsIterator).second.push_back( SedFlowBuilders::generalBuilder( (*currentInnerIterator), highestOrderStructuresPointers ) ); }
	}

	//Create the result
	void* result = SedFlowBuilders::specificBuilder(constructionVariables,highestOrderStructuresPointers,tempLabelledObjects);

	std::vector<ConstructionVariables>::const_reverse_iterator currentInnerNameIterator;
	//Delete the temporary labelled objects
	{
		std::map< std::string, std::vector<void*> >::reverse_iterator currentOuterIterator = tempLabelledObjects.rbegin();
		std::map< std::string, std::vector<ConstructionVariables> >::const_reverse_iterator currentOuterNameIterator = constructionVariables.labelledObjects.rbegin();
		for(; currentOuterIterator != tempLabelledObjects.rend(); ++currentOuterIterator, ++currentOuterNameIterator)
		{
			currentInnerNameIterator = ( (*currentOuterNameIterator).second.rbegin() );
			while( ! ((*currentOuterIterator).second.empty()) )
			{
				SedFlowBuilders::specificDelete( (*currentInnerNameIterator).interfaceOrCombinerType, (*currentOuterIterator).second.back() );
				(*currentOuterIterator).second.pop_back();
				++currentInnerNameIterator;
			}
		}
	}

	return result;
}

void* SedFlowBuilders::specificBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	void* result;
	switch (constructionVariables.interfaceOrCombinerType)
	{
	case CombinerVariables::OutputMethodType:
		result = dynamic_cast<void*>( SedFlowBuilders::outputMethodTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverSystemProperties:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverSystemPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverSystemPropertyType:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverSystemPropertyTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::RegularRiverSystemProperties:
		result = dynamic_cast<void*>( SedFlowBuilders::regularRiverSystemPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::RiverReachProperties:
		result = dynamic_cast<void*>( SedFlowBuilders::riverReachPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverReachProperties:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverReachPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverReachPropertyType:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverReachPropertyTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::RegularRiverReachProperties:
		result = dynamic_cast<void*>( SedFlowBuilders::regularRiverReachPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::Grains:
		result = dynamic_cast<void*>( SedFlowBuilders::grainsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::GrainType:
		result = dynamic_cast<void*>( SedFlowBuilders::grainTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::GeometricalChannelBehaviour:
		result = dynamic_cast<void*>( SedFlowBuilders::geometricalChannelBehaviourBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::ChannelGeometry:
		result = dynamic_cast<void*>( SedFlowBuilders::channelGeometryBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::StrataSorting:
		result = dynamic_cast<void*>( SedFlowBuilders::strataSortingBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::SillProperties:
		result = dynamic_cast<void*>( SedFlowBuilders::sillPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcGradient:
		result = dynamic_cast<void*>( SedFlowBuilders::calcGradientBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::FlowResistance:
		result = dynamic_cast<void*>( SedFlowBuilders::flowResistanceBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverSystemMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverSystemMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverSystemMethodType:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverSystemMethodTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::RegularRiverSystemMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::regularRiverSystemMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::FlowMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::flowMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::FlowTypeMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::flowTypeMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::SedimentFlowMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::sedimentFlowMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::SedimentFlowTypeMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::sedimentFlowTypeMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcBedloadCapacity:
		result = dynamic_cast<void*>( SedFlowBuilders::calcBedloadCapacityBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::ChangeRateModifiers:
		result = dynamic_cast<void*>( SedFlowBuilders::changeRateModifiersBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::ChangeRateModifiersForSingleFlowMethod:
		result = dynamic_cast<void*>( SedFlowBuilders::changeRateModifiersForSingleFlowMethodBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::ChangeRateModifiersType:
		result = dynamic_cast<void*>( SedFlowBuilders::changeRateModifiersTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::RiverReachMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::riverReachMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverReachMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverReachMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::AdditionalRiverReachMethodType:
		result = dynamic_cast<void*>( SedFlowBuilders::additionalRiverReachMethodTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::RegularRiverReachMethods:
		result = dynamic_cast<void*>( SedFlowBuilders::regularRiverReachMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcTau:
		result = dynamic_cast<void*>( SedFlowBuilders::calcTauBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcBedloadVelocity:
		result = dynamic_cast<void*>( SedFlowBuilders::calcBedloadVelocityBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::EstimateThicknessOfMovingSedimentLayer:
		result = dynamic_cast<void*>( SedFlowBuilders::estimateThicknessOfMovingSedimentLayerBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcActiveWidth:
		result = dynamic_cast<void*>( SedFlowBuilders::calcActiveWidthBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcThresholdForInitiationOfBedloadMotion:
		result = dynamic_cast<void*>( SedFlowBuilders::calcThresholdForInitiationOfBedloadMotionBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::CalcHidingFactors:
		result = dynamic_cast<void*>( SedFlowBuilders::calcHidingFactorsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::TimeSeriesEntry:
		result = dynamic_cast<void*>( SedFlowBuilders::timeSeriesEntryBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	case CombinerVariables::TimeSeries:
		result = dynamic_cast<void*>( SedFlowBuilders::timeSeriesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects) );
		break;

	default:
		const char *const errorMessage = "Invalid type of combiners and interfaces for construction.";
		throw (errorMessage);
	}
	return result;
}

void SedFlowBuilders::specificDelete (CombinerVariables::TypesOfCombinersAndInterfaces interfaceOrCombinerType, void* toDelete)
{
	switch (interfaceOrCombinerType)
	{
	case CombinerVariables::OutputMethods:
		delete static_cast<OutputMethods*>(toDelete);
		break;

	case CombinerVariables::OutputMethodType:
		delete static_cast<OutputMethodType*>(toDelete);
		break;

	case CombinerVariables::RiverSystemProperties:
		delete static_cast<RiverSystemProperties*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverSystemProperties:
		delete static_cast<AdditionalRiverSystemProperties*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverSystemPropertyType:
		delete static_cast<AdditionalRiverSystemPropertyType*>(toDelete);
		break;

	case CombinerVariables::RegularRiverSystemProperties:
		delete static_cast<RegularRiverSystemProperties*>(toDelete);
		break;

	case CombinerVariables::RiverReachProperties:
		delete static_cast<RiverReachProperties*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverReachProperties:
		delete static_cast<AdditionalRiverReachProperties*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverReachPropertyType:
		delete static_cast<AdditionalRiverReachPropertyType*>(toDelete);
		break;

	case CombinerVariables::RegularRiverReachProperties:
		delete static_cast<RegularRiverReachProperties*>(toDelete);
		break;

	case CombinerVariables::Grains:
		delete static_cast<Grains*>(toDelete);
		break;

	case CombinerVariables::GrainType:
		delete static_cast<GrainType*>(toDelete);
		break;

	case CombinerVariables::GeometricalChannelBehaviour:
		delete static_cast<GeometricalChannelBehaviour*>(toDelete);
		break;

	case CombinerVariables::ChannelGeometry:
		delete static_cast<ChannelGeometry*>(toDelete);
		break;

	case CombinerVariables::StrataSorting:
		delete static_cast<StrataSorting*>(toDelete);
		break;

	case CombinerVariables::SillProperties:
		delete static_cast<SillProperties*>(toDelete);
		break;

	case CombinerVariables::OverallMethods:
		delete static_cast<OverallMethods*>(toDelete);
		break;

	case CombinerVariables::CalcGradient:
		delete static_cast<CalcGradient*>(toDelete);
		break;

	case CombinerVariables::FlowResistance:
		delete static_cast<FlowResistance*>(toDelete);
		break;

	case CombinerVariables::RiverSystemMethods:
		delete static_cast<RiverSystemMethods*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverSystemMethods:
		delete static_cast<AdditionalRiverSystemMethods*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverSystemMethodType:
		delete static_cast<AdditionalRiverSystemMethodType*>(toDelete);
		break;

	case CombinerVariables::RegularRiverSystemMethods:
		delete static_cast<RegularRiverSystemMethods*>(toDelete);
		break;

	case CombinerVariables::FlowMethods:
		delete static_cast<FlowMethods*>(toDelete);
		break;

	case CombinerVariables::FlowTypeMethods:
		delete static_cast<FlowTypeMethods*>(toDelete);
		break;

	case CombinerVariables::SedimentFlowMethods:
		delete static_cast<SedimentFlowMethods*>(toDelete);
		break;

	case CombinerVariables::SedimentFlowTypeMethods:
		delete static_cast<SedimentFlowTypeMethods*>(toDelete);
		break;

	case CombinerVariables::CalcBedloadCapacity:
		delete static_cast<CalcBedloadCapacity*>(toDelete);
		break;

	case CombinerVariables::ChangeRateModifiers:
		delete static_cast<ChangeRateModifiers*>(toDelete);
		break;

	case CombinerVariables::ChangeRateModifiersForSingleFlowMethod:
		delete static_cast<ChangeRateModifiersForSingleFlowMethod*>(toDelete);
		break;

	case CombinerVariables::ChangeRateModifiersType:
		delete static_cast<ChangeRateModifiersType*>(toDelete);
		break;

	case CombinerVariables::RiverReachMethods:
		delete static_cast<RiverReachMethods*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverReachMethods:
		delete static_cast<AdditionalRiverReachMethods*>(toDelete);
		break;

	case CombinerVariables::AdditionalRiverReachMethodType:
		delete static_cast<AdditionalRiverReachMethodType*>(toDelete);
		break;

	case CombinerVariables::RegularRiverReachMethods:
		delete static_cast<RegularRiverReachMethods*>(toDelete);
		break;

	case CombinerVariables::CalcTau:
		delete static_cast<CalcTau*>(toDelete);
		break;

	case CombinerVariables::CalcBedloadVelocity:
		delete static_cast<CalcBedloadVelocity*>(toDelete);
		break;

	case CombinerVariables::EstimateThicknessOfMovingSedimentLayer:
		delete static_cast<EstimateThicknessOfMovingSedimentLayer*>(toDelete);
		break;

	case CombinerVariables::CalcActiveWidth:
		delete static_cast<CalcActiveWidth*>(toDelete);
		break;

	case CombinerVariables::CalcThresholdForInitiationOfBedloadMotion:
		delete static_cast<CalcThresholdForInitiationOfBedloadMotion*>(toDelete);
		break;

	case CombinerVariables::CalcHidingFactors:
		delete static_cast<CalcHidingFactors*>(toDelete);
		break;

	case CombinerVariables::TimeSeriesEntry:
		delete static_cast<TimeSeriesEntry*>(toDelete);
		break;

	case CombinerVariables::TimeSeries:
		delete static_cast<TimeSeries*>(toDelete);
		break;

	default:
		const char *const errorMessage = "Invalid type of combiners and interfaces for deletion";
		throw (errorMessage);
	}
}

OverallParameters* SedFlowBuilders::overallParametersBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers)
{
	std::map< std::string, std::vector<double> >::const_iterator tempDoubleIterator;

	tempDoubleIterator = constructionVariables.labelledDoubles.find("densityWater");
	double densityWater;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const densityWaterErrorMessage = "The variable densityWater is needed for the construction of OverallParameters";
		throw(densityWaterErrorMessage);
	}
	else { densityWater = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("densitySediment");
	double densitySediment;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const densitySedimentErrorMessage = "The variable densitySediment is needed for the construction of OverallParameters";
		throw(densitySedimentErrorMessage);
	}
	else { densitySediment = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("poreVolumeFraction");
	double poreVolumeFraction;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const poreVolumeFractionErrorMessage = "The variable poreVolumeFraction is needed for the construction of OverallParameters";
		throw(poreVolumeFractionErrorMessage);
	}
	else { poreVolumeFraction = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("gravityAcceleration");
	double gravityAcceleration;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const gravityAccelerationErrorMessage = "The variable gravityAcceleration is needed for the construction of OverallParameters";
		throw(gravityAccelerationErrorMessage);
	}
	else { gravityAcceleration = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("angleOfReposeInDegree");
	double angleOfReposeInDegree;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const angleOfReposeInDegreeErrorMessage = "The variable angleOfReposeInDegree is needed for the construction of OverallParameters";
		throw(angleOfReposeInDegreeErrorMessage);
	}
	else { angleOfReposeInDegree = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("fractionalGrainDiameters");
	std::vector<double> fractionalGrainDiameters;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const fractionalGrainDiametersErrorMessage = "The variable fractionalGrainDiameters is needed for the construction of OverallParameters";
		throw(fractionalGrainDiametersErrorMessage);
	}
	else { fractionalGrainDiameters = (*tempDoubleIterator).second; }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("currentTimeStepLengthInSeconds");
	double currentTimeStepLengthInSeconds;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const currentTimeStepLengthInSecondsErrorMessage = "The variable currentTimeStepLengthInSeconds is needed for the construction of OverallParameters";
		throw(currentTimeStepLengthInSecondsErrorMessage);
	}
	else { currentTimeStepLengthInSeconds = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("elapsedSeconds");
	double elapsedSeconds;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const elapsedSecondsErrorMessage = "The variable elapsedSeconds is needed for the construction of OverallParameters";
		throw(elapsedSecondsErrorMessage);
	}
	else { elapsedSeconds = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("finishSeconds");
	double finishSeconds;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const finishSecondsErrorMessage = "The variable finishSeconds is needed for the construction of OverallParameters";
		throw(finishSecondsErrorMessage);
	}
	else { finishSeconds = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("courantFriedrichsLewyNumber");
	double courantFriedrichsLewyNumber;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const courantFriedrichsLewyNumberErrorMessage = "The variable courantFriedrichsLewyNumber is needed for the construction of OverallParameters";
		throw(courantFriedrichsLewyNumberErrorMessage);
	}
	else { courantFriedrichsLewyNumber = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("timeStepThresholdForTerminatingSimulation");
	double timeStepThresholdForTerminatingSimulation;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const timeStepThresholdForTerminatingSimulationErrorMessage = "The variable timeStepThresholdForTerminatingSimulation is needed for the construction of OverallParameters";
		throw(timeStepThresholdForTerminatingSimulationErrorMessage);
	}
	else { timeStepThresholdForTerminatingSimulation = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("timeStepFactor");
	double timeStepFactor;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() ) {  timeStepFactor = 1.0; }
	else { timeStepFactor = ((*tempDoubleIterator).second).at(0); }

	std::map<std::string, std::vector<bool> >::const_iterator tempBoolIterator;

	bool updateRegularPropertiesAfterInitialisation;
	tempBoolIterator = constructionVariables.labelledBools.find("updateRegularPropertiesAfterInitialisation");
	if(tempBoolIterator == constructionVariables.labelledBools.end() )
	{
		const char *const updateRegularPropertiesAfterInitialisationErrorMessage = "The variable updateRegularPropertiesAfterInitialisation is needed for the construction of OverallParameters";
		throw(updateRegularPropertiesAfterInitialisationErrorMessage);
	}
	else { updateRegularPropertiesAfterInitialisation = tempBoolIterator->second.at(0); }

	bool updateAdditionalRiverReachPropertiesAfterInitialisation;
	tempBoolIterator = constructionVariables.labelledBools.find("updateAdditionalRiverReachPropertiesAfterInitialisation");
	if(tempBoolIterator == constructionVariables.labelledBools.end() )
	{
		const char *const updateAdditionalRiverReachPropertiesAfterInitialisationErrorMessage = "The variable updateAdditionalRiverReachPropertiesAfterInitialisation is needed for the construction of OverallParameters";
		throw(updateAdditionalRiverReachPropertiesAfterInitialisationErrorMessage);
	}
	else { updateAdditionalRiverReachPropertiesAfterInitialisation = tempBoolIterator->second.at(0); }

	bool updateOutputMethodsAfterInitialisation;
	tempBoolIterator = constructionVariables.labelledBools.find("updateOutputMethodsAfterInitialisation");
	if(tempBoolIterator == constructionVariables.labelledBools.end() )
	{
		const char *const updateOutputMethodsAfterInitialisationErrorMessage = "The variable updateOutputMethodsAfterInitialisation is needed for the construction of OverallParameters";
		throw(updateOutputMethodsAfterInitialisationErrorMessage);
	}
	else { updateOutputMethodsAfterInitialisation = tempBoolIterator->second.at(0); }

	return ( new OverallParameters(densityWater, densitySediment, poreVolumeFraction, gravityAcceleration, angleOfReposeInDegree, fractionalGrainDiameters, currentTimeStepLengthInSeconds, elapsedSeconds, finishSeconds, courantFriedrichsLewyNumber, timeStepThresholdForTerminatingSimulation, timeStepFactor, updateRegularPropertiesAfterInitialisation, updateAdditionalRiverReachPropertiesAfterInitialisation, updateOutputMethodsAfterInitialisation) );
}

OverallMethods* SedFlowBuilders::overallMethodsBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers)
{
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = constructionVariables.labelledObjects.find("bedSlopeCalculationMethod");
	CalcGradient* bedSlopeCalculationMethod;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const bedSlopeCalculationMethodErrorMessage = "The variable bedSlopeCalculationMethod is needed for the construction of OverallMethods";
		throw(bedSlopeCalculationMethodErrorMessage);
	}
	else { bedSlopeCalculationMethod = static_cast<CalcGradient*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	tempObjectsIterator = constructionVariables.labelledObjects.find("waterEnergySlopeCalculationMethod");
	CalcGradient* waterEnergySlopeCalculationMethod;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const waterEnergySlopeCalculationMethodErrorMessage = "The variable waterEnergySlopeCalculationMethod is needed for the construction of OverallMethods";
		throw(waterEnergySlopeCalculationMethodErrorMessage);
	}
	else { waterEnergySlopeCalculationMethod = static_cast<CalcGradient*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	tempObjectsIterator = constructionVariables.labelledObjects.find("sedimentEnergySlopeCalculationMethod");
	CalcGradient* sedimentEnergySlopeCalculationMethod;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const sedimentEnergySlopeCalculationMethodErrorMessage = "The variable sedimentEnergySlopeCalculationMethod is needed for the construction of OverallMethods";
		throw(sedimentEnergySlopeCalculationMethodErrorMessage);
	}
	else { sedimentEnergySlopeCalculationMethod = static_cast<CalcGradient*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	tempObjectsIterator = constructionVariables.labelledObjects.find("flowResistance");
	FlowResistance* flowResistance;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const flowResistanceErrorMessage = "The variable flowResistance is needed for the construction of OverallMethods";
		throw(flowResistanceErrorMessage);
	}
	else { flowResistance = static_cast<FlowResistance*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	tempObjectsIterator = constructionVariables.labelledObjects.find("bedloadVelocityCalculationMethod");
	CalcBedloadVelocity* bedloadVelocityCalculationMethod;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const bedloadVelocityCalculationMethodErrorMessage = "The variable bedloadVelocityCalculationMethod is needed for the construction of OverallMethods";
		throw(bedloadVelocityCalculationMethodErrorMessage);
	}
	else { bedloadVelocityCalculationMethod = static_cast<CalcBedloadVelocity*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	OverallMethods* result = new OverallMethods(bedSlopeCalculationMethod, waterEnergySlopeCalculationMethod, sedimentEnergySlopeCalculationMethod, flowResistance, bedloadVelocityCalculationMethod);

	delete bedSlopeCalculationMethod;
	delete waterEnergySlopeCalculationMethod;
	delete sedimentEnergySlopeCalculationMethod;
	delete flowResistance;
	delete bedloadVelocityCalculationMethod;

	return result;
}

RiverSystemProperties* SedFlowBuilders::riverSystemPropertiesBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers)
{
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = constructionVariables.labelledObjects.find("regularRiverSystemProperties");
	RegularRiverSystemProperties* regularRiverSystemProperties;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const regularRiverSystemPropertiesErrorMessage = "The variable regularRiverSystemProperties is needed for the construction of RiverSystemProperties";
		throw(regularRiverSystemPropertiesErrorMessage);
	}
	else { regularRiverSystemProperties = static_cast<RegularRiverSystemProperties*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	tempObjectsIterator = constructionVariables.labelledObjects.find("additionalRiverSystemProperties");
	AdditionalRiverSystemProperties* additionalRiverSystemProperties;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const additionalRiverSystemPropertiesErrorMessage = "The variable additionalRiverSystemProperties is needed for the construction of RiverSystemProperties";
		throw(additionalRiverSystemPropertiesErrorMessage);
	}
	else { additionalRiverSystemProperties = static_cast<AdditionalRiverSystemProperties*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	RiverSystemProperties* result = new RiverSystemProperties(*regularRiverSystemProperties, *additionalRiverSystemProperties, *(highestOrderStructuresPointers.overallParameters));

	delete regularRiverSystemProperties;
	delete additionalRiverSystemProperties;

	return result;
}

RiverSystemMethods* SedFlowBuilders::riverSystemMethodsBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers)
{
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = constructionVariables.labelledObjects.find("regularRiverSystemMethods");
	RegularRiverSystemMethods* regularRiverSystemMethods;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const regularRiverSystemMethodsErrorMessage = "The variable regularRiverSystemMethods is needed for the construction of RiverSystemMethods";
		throw(regularRiverSystemMethodsErrorMessage);
	}
	else { regularRiverSystemMethods = static_cast<RegularRiverSystemMethods*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	tempObjectsIterator = constructionVariables.labelledObjects.find("additionalRiverSystemMethods");
	AdditionalRiverSystemMethods* additionalRiverSystemMethods;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const additionalRiverSystemMethodsErrorMessage = "The variable additionalRiverSystemMethods is needed for the construction of RiverSystemMethods";
		throw(additionalRiverSystemMethodsErrorMessage);
	}
	else { additionalRiverSystemMethods = static_cast<AdditionalRiverSystemMethods*>( SedFlowBuilders::generalBuilder( ((*tempObjectsIterator).second).at(0), highestOrderStructuresPointers) ); }

	RiverSystemMethods* result = new RiverSystemMethods(*regularRiverSystemMethods, *additionalRiverSystemMethods, *(highestOrderStructuresPointers.riverSystemProperties), *(highestOrderStructuresPointers.overallMethods));

	delete regularRiverSystemMethods;
	delete additionalRiverSystemMethods;

	return result;
}

OutputMethods* SedFlowBuilders::outputMethodsBuilder (const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers)
{
	std::map< std::string, std::vector<ConstructionVariables> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = constructionVariables.labelledObjects.find("constitutingOutputMethodTypes");
	std::vector<OutputMethodType*> singleOutputMethodTypes;
	if(tempObjectsIterator == constructionVariables.labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingOutputMethodTypes is needed for the construction of OutputMethods";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<ConstructionVariables>::const_iterator currentInnerIterator = (*tempObjectsIterator).second.begin(); currentInnerIterator < (*tempObjectsIterator).second.end(); ++currentInnerIterator)

			{ singleOutputMethodTypes.push_back( static_cast<OutputMethodType*>( SedFlowBuilders::generalBuilder( (*currentInnerIterator), highestOrderStructuresPointers) ) ); }
	}

	OutputMethods* result = new OutputMethods(singleOutputMethodTypes);

	while(!(singleOutputMethodTypes.empty()))
	{
		delete singleOutputMethodTypes.back();
		singleOutputMethodTypes.pop_back();
	}

	return result;
}

OutputMethodType* SedFlowBuilders::outputMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::outputMethodTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

AdditionalRiverSystemProperties* SedFlowBuilders::additionalRiverSystemPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingAdditionalRiverSystemPropertyTypes");
	std::vector<AdditionalRiverSystemPropertyType*> singleAdditionalRiverSystemPropertyTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingAdditionalRiverSystemPropertyTypes is needed for the construction of AdditionalRiverSystemProperties";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleAdditionalRiverSystemPropertyTypes.push_back(static_cast<AdditionalRiverSystemPropertyType*>(*currentVoidPointer) ); }
	}
	return ( new AdditionalRiverSystemProperties(singleAdditionalRiverSystemPropertyTypes) );
}

AdditionalRiverSystemPropertyType* SedFlowBuilders::additionalRiverSystemPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::additionalRiverSystemPropertyTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

RegularRiverSystemProperties* SedFlowBuilders::regularRiverSystemPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = labelledObjects.find("cellProperties");
	std::vector<RiverReachProperties> cellProperties;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable cellProperties is needed for the construction of RegularRiverSystemProperties";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ cellProperties.push_back( *(static_cast<RiverReachProperties*>(*currentVoidPointer)) ); }
	}
	return ( new RegularRiverSystemProperties(cellProperties) );
}

RiverReachProperties* SedFlowBuilders::riverReachPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<int> >::const_iterator tempIntIterator;

	tempIntIterator = constructionVariables.labelledInts.find("cellID");
	int cellID;
	if(tempIntIterator == constructionVariables.labelledInts.end() )
	{
		const char *const cellIDErrorMessage = "The variable cellID is needed for the construction of RiverReachProperties";
		throw(cellIDErrorMessage);
	}
	else { cellID = ((*tempIntIterator).second).at(0); }

	tempIntIterator = constructionVariables.labelledInts.find("downstreamCellID");
	int downstreamCellID;
	if(tempIntIterator == constructionVariables.labelledInts.end() )
	{
		const char *const downstreamCellIDErrorMessage = "The variable downstreamCellID is needed for the construction of RiverReachProperties";
		throw(downstreamCellIDErrorMessage);
	}
	else { downstreamCellID = ((*tempIntIterator).second).at(0); }

	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = labelledObjects.find("regularRiverReachProperties");
	RegularRiverReachProperties regularRiverReachProperties = RegularRiverReachProperties(highestOrderStructuresPointers.overallParameters);
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const regularRiverReachPropertiesErrorMessage = "The variable regularRiverReachProperties is needed for the construction of RiverReachProperties";
		throw(regularRiverReachPropertiesErrorMessage);
	}
	else { regularRiverReachProperties = *(static_cast<RegularRiverReachProperties*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("additionalRiverReachProperties");
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const additionalRiverReachPropertiesErrorMessage = "The variable additionalRiverReachProperties is needed for the construction of RiverReachProperties";
		throw(additionalRiverReachPropertiesErrorMessage);
	}
	AdditionalRiverReachProperties additionalRiverReachProperties = *(static_cast<AdditionalRiverReachProperties*>( ((*tempObjectsIterator).second).at(0) ));

	tempObjectsIterator = labelledObjects.find("strataSorting");
	StrataSorting* strataSorting;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const strataSortingErrorMessage = "The variable strataSorting is needed for the construction of RiverReachProperties";
		throw(strataSortingErrorMessage);
	}
	else { strataSorting = static_cast<StrataSorting*>( ((*tempObjectsIterator).second).at(0) ); }

	tempObjectsIterator = labelledObjects.find("geometricalChannelBehaviour");
	GeometricalChannelBehaviour* geometricalChannelBehaviour;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const geometricalChannelBehaviourErrorMessage = "The variable geometricalChannelBehaviour is needed for the construction of RiverReachProperties";
		throw(geometricalChannelBehaviourErrorMessage);
	}
	else { geometricalChannelBehaviour = static_cast<GeometricalChannelBehaviour*>( ((*tempObjectsIterator).second).at(0) ); }

	tempObjectsIterator = labelledObjects.find("sillProperties");
	SillProperties* sillProperties;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const sillPropertiesErrorMessage = "The variable sillProperties is needed for the construction of RiverReachProperties";
		throw(sillPropertiesErrorMessage);
	}
	else { sillProperties = static_cast<SillProperties*>( ((*tempObjectsIterator).second).at(0) ); }

	return ( new RiverReachProperties(cellID, downstreamCellID, regularRiverReachProperties, additionalRiverReachProperties, strataSorting, geometricalChannelBehaviour, sillProperties, highestOrderStructuresPointers.overallParameters) );
}

AdditionalRiverReachProperties* SedFlowBuilders::additionalRiverReachPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingAdditionalRiverReachPropertyTypes");
	std::vector<AdditionalRiverReachPropertyType*> singleAdditionalRiverReachPropertyTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingAdditionalRiverReachPropertyTypes is needed for the construction of AdditionalRiverReachProperties";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleAdditionalRiverReachPropertyTypes.push_back(static_cast<AdditionalRiverReachPropertyType*>(*currentVoidPointer) ); }
	}
	return ( new AdditionalRiverReachProperties(singleAdditionalRiverReachPropertyTypes) );
}

AdditionalRiverReachPropertyType* SedFlowBuilders::additionalRiverReachPropertyTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::additionalRiverReachPropertyTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

RegularRiverReachProperties* SedFlowBuilders::regularRiverReachPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<double> >::const_iterator tempDoubleIterator;
	std::map< std::string, std::vector<bool> >::const_iterator tempBoolIterator;
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempDoubleIterator = constructionVariables.labelledDoubles.find("elevation");
	double elevation;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const elevationErrorMessage = "The variable elevation is needed for the construction of RegularRiverReachProperties";
		throw(elevationErrorMessage);
	}
	else { elevation = ((*tempDoubleIterator).second).at(0); }

	tempBoolIterator = constructionVariables.labelledBools.find("sillOccurence");
	bool sillOccurence;
	if(tempBoolIterator == constructionVariables.labelledBools.end() )
	{
		const char *const sillOccurenceErrorMessage = "The variable sillOccurence is needed for the construction of RegularRiverReachProperties";
		throw(sillOccurenceErrorMessage);
	}
	else { sillOccurence = ((*tempBoolIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("sillTopEdgeElevation");
	double sillTopEdgeElevation;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const sillTopEdgeElevationErrorMessage = "The variable sillTopEdgeElevation is needed for the construction of RegularRiverReachProperties";
		throw(sillTopEdgeElevationErrorMessage);
	}
	else { sillTopEdgeElevation = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("length");
	double length;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const lengthErrorMessage = "The variable length is needed for the construction of RegularRiverReachProperties";
		throw(lengthErrorMessage);
	}
	else { length = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("discharge");
	double discharge;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const dischargeErrorMessage = "The variable discharge is needed for the construction of RegularRiverReachProperties";
		throw(dischargeErrorMessage);
	}
	else { discharge = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("flowVelocity");
	double flowVelocity;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const flowVelocityErrorMessage = "The variable flowVelocity is needed for the construction of RegularRiverReachProperties";
		throw(flowVelocityErrorMessage);
	}
	else { flowVelocity = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("maximumWaterdepth");
	double maximumWaterdepth;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const maximumWaterdepthErrorMessage = "The variable maximumWaterdepth is needed for the construction of RegularRiverReachProperties";
		throw(maximumWaterdepthErrorMessage);
	}
	else { maximumWaterdepth = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("bedShearStress");
	double bedShearStress;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const bedShearStressErrorMessage = "The variable bedShearStress is needed for the construction of RegularRiverReachProperties";
		throw(bedShearStressErrorMessage);
	}
	else { bedShearStress = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("activeWidth");
	double activeWidth;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const activeWidthErrorMessage = "The variable activeWidth is needed for the construction of RegularRiverReachProperties";
		throw(activeWidthErrorMessage);
	}
	else { activeWidth = ((*tempDoubleIterator).second).at(0); }

	tempObjectsIterator = labelledObjects.find("strataPerUnitBedSurface");
	std::vector<Grains> strataPerUnitBedSurface;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const strataPerUnitBedSurfaceErrorMessage = "The variable strataPerUnitBedSurface is needed for the construction of RegularRiverReachProperties";
		throw(strataPerUnitBedSurfaceErrorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ strataPerUnitBedSurface.push_back( (*(static_cast<Grains*>(*currentVoidPointer))) ); }
	}

	tempDoubleIterator = constructionVariables.labelledDoubles.find("bedslope");
	double bedslope;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const bedslopeErrorMessage = "The variable bedslope is needed for the construction of RegularRiverReachProperties";
		throw(bedslopeErrorMessage);
	}
	else { bedslope = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("waterEnergyslope");
	double waterEnergyslope;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const waterEnergyslopeErrorMessage = "The variable waterEnergyslope is needed for the construction of RegularRiverReachProperties";
		throw(waterEnergyslopeErrorMessage);
	}
	else { waterEnergyslope = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("sedimentEnergyslope");
	double sedimentEnergyslope;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const sedimentEnergyslopeErrorMessage = "The variable sedimentEnergyslope is needed for the construction of RegularRiverReachProperties";
		throw(sedimentEnergyslopeErrorMessage);
	}
	else { sedimentEnergyslope = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("unreducedSedimentEnergyslope");
	double unreducedSedimentEnergyslope;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const unreducedSedimentEnergyslopeErrorMessage = "The variable unreducedSedimentEnergyslope is needed for the construction of RegularRiverReachProperties";
		throw(unreducedSedimentEnergyslopeErrorMessage);
	}
	else { unreducedSedimentEnergyslope = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("waterVolumeChangeRate");
	double waterVolumeChangeRate;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const waterVolumeChangeRateErrorMessage = "The variable waterVolumeChangeRate is needed for the construction of RegularRiverReachProperties";
		throw(waterVolumeChangeRateErrorMessage);
	}
	else { waterVolumeChangeRate = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("waterVolumeChange");
	double waterVolumeChange;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const waterVolumeChangeErrorMessage = "The variable waterVolumeChange is needed for the construction of RegularRiverReachProperties";
		throw(waterVolumeChangeErrorMessage);
	}
	else { waterVolumeChange = ((*tempDoubleIterator).second).at(0); }

	tempObjectsIterator = labelledObjects.find("erosionRate");
	Grains erosionRate;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const erosionRateErrorMessage = "The variable erosionRate is needed for the construction of RegularRiverReachProperties";
		throw(erosionRateErrorMessage);
	}
	else { erosionRate = *(static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("depositionRate");
	Grains depositionRate;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const depositionRateErrorMessage = "The variable depositionRate is needed for the construction of RegularRiverReachProperties";
		throw(depositionRateErrorMessage);
	}
	else { depositionRate = *(static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("erosion");
	Grains erosion;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const erosionErrorMessage = "The variable erosion is needed for the construction of RegularRiverReachProperties";
		throw(erosionErrorMessage);
	}
	else { erosion = *(static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("deposition");
	Grains deposition;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const depositionErrorMessage = "The variable deposition is needed for the construction of RegularRiverReachProperties";
		throw(depositionErrorMessage);
	}
	else { deposition = *(static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("erosionPerUnitBedSurface");
	Grains erosionPerUnitBedSurface;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const erosionPerUnitBedSurfaceErrorMessage = "The variable erosionPerUnitBedSurface is needed for the construction of RegularRiverReachProperties";
		throw(erosionPerUnitBedSurfaceErrorMessage);
	}
	else { erosionPerUnitBedSurface = *(static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("depositionPerUnitBedSurface");
	Grains depositionPerUnitBedSurface;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const depositionPerUnitBedSurfaceErrorMessage = "The variable depositionPerUnitBedSurface is needed for the construction of RegularRiverReachProperties";
		throw(depositionPerUnitBedSurfaceErrorMessage);
	}
	else { depositionPerUnitBedSurface = *(static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) )); }

	return ( new RegularRiverReachProperties(highestOrderStructuresPointers.overallParameters,
												elevation,
												length,
												sillOccurence,
												sillTopEdgeElevation,
												discharge,
												flowVelocity,
												maximumWaterdepth,
												bedShearStress,
												activeWidth,
												strataPerUnitBedSurface,
												bedslope,
												waterEnergyslope,
												sedimentEnergyslope,
												unreducedSedimentEnergyslope,
												waterVolumeChangeRate,
												waterVolumeChange,
												erosionRate,
												depositionRate,
												erosion,
												deposition,
												erosionPerUnitBedSurface,
												depositionPerUnitBedSurface) );
}

Grains* SedFlowBuilders::grainsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingGrainTypes");
	std::vector<GrainType*> singleGrainTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingGrainTypes is needed for the construction of Grains";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleGrainTypes.push_back(static_cast<GrainType*>(*currentVoidPointer) ); }
	}
	return ( new Grains(singleGrainTypes) );
}

GrainType* SedFlowBuilders::grainTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::grainTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

GeometricalChannelBehaviour* SedFlowBuilders::geometricalChannelBehaviourBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::geometricalChannelBehaviourBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

ChannelGeometry* SedFlowBuilders::channelGeometryBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::channelGeometryBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

StrataSorting* SedFlowBuilders::strataSortingBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::strataSortingBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

SillProperties* SedFlowBuilders::sillPropertiesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::sillPropertiesBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

CalcGradient* SedFlowBuilders::calcGradientBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcGradientBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

FlowResistance* SedFlowBuilders::flowResistanceBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::flowResistanceBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

AdditionalRiverSystemMethods* SedFlowBuilders::additionalRiverSystemMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingAdditionalRiverSystemMethodTypes");
	std::vector<AdditionalRiverSystemMethodType*> constitutingAdditionalRiverSystemMethodTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingAdditionalRiverSystemMethodTypes is needed for the construction of AdditionalRiverSystemMethods";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ constitutingAdditionalRiverSystemMethodTypes.push_back(static_cast<AdditionalRiverSystemMethodType*>(*currentVoidPointer) ); }
	}
	return ( new AdditionalRiverSystemMethods(constitutingAdditionalRiverSystemMethodTypes) );
}

AdditionalRiverSystemMethodType* SedFlowBuilders::additionalRiverSystemMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::additionalRiverSystemMethodTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

RegularRiverSystemMethods* SedFlowBuilders::regularRiverSystemMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = labelledObjects.find("riverReachMethods");
	std::vector<RiverReachMethods> riverReachMethods;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const riverReachMethodsErrorMessage = "The variable riverReachMethods is needed for the construction of RegularRiverSystemMethods";
		throw(riverReachMethodsErrorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ riverReachMethods.push_back( (*(static_cast<RiverReachMethods*>(*currentVoidPointer))) ); }
	}

	tempObjectsIterator = labelledObjects.find("flowMethods");
	FlowMethods flowMethods;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const flowMethodsErrorMessage = "The variable flowMethods is needed for the construction of RegularRiverSystemMethods";
		throw(flowMethodsErrorMessage);
	}
	else { flowMethods = *(static_cast<FlowMethods*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("changeRateModifiers");
	ChangeRateModifiers changeRateModifiers;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const changeRateModifiersErrorMessage = "The variable changeRateModifiers is needed for the construction of RegularRiverSystemMethods";
		throw(changeRateModifiersErrorMessage);
	}
	else { changeRateModifiers = *(static_cast<ChangeRateModifiers*>( ((*tempObjectsIterator).second).at(0) )); }

	return ( new RegularRiverSystemMethods(riverReachMethods, flowMethods, changeRateModifiers) );
}

FlowMethods* SedFlowBuilders::flowMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingFlowMethodsTypes");
	std::vector<FlowTypeMethods*> singleFlowMethodsTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingFlowMethodsTypes is needed for the construction of FlowMethods";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleFlowMethodsTypes.push_back(static_cast<FlowTypeMethods*>(*currentVoidPointer) ); }
	}
	return ( new FlowMethods(singleFlowMethodsTypes) );
}

FlowTypeMethods* SedFlowBuilders::flowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::flowTypeMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

SedimentFlowMethods* SedFlowBuilders::sedimentFlowMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingSedimentFlowMethodsTypes");
	std::vector<SedimentFlowTypeMethods*> singleSedimentFlowMethodsTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingSedimentFlowMethodsTypes is needed for the construction of SedimentFlowMethods";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleSedimentFlowMethodsTypes.push_back( static_cast<SedimentFlowTypeMethods*>(*currentVoidPointer) ); }
	}

	bool updateErosionRatesInMarginCells;
	std::map<std::string, std::vector<bool> >::const_iterator boolMapIterator = constructionVariables.labelledBools.find("updateErosionRatesInMarginCells");
	if(boolMapIterator == constructionVariables.labelledBools.end() ) { updateErosionRatesInMarginCells = false; }
	else { updateErosionRatesInMarginCells = boolMapIterator->second.at(0); }

	return ( new SedimentFlowMethods(singleSedimentFlowMethodsTypes,updateErosionRatesInMarginCells) );
}

SedimentFlowTypeMethods* SedFlowBuilders::sedimentFlowTypeMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::sedimentFlowTypeMethodsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

CalcBedloadCapacity* SedFlowBuilders::calcBedloadCapacityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcBedloadCapacityBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

ChangeRateModifiers* SedFlowBuilders::changeRateModifiersBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingChangeRateModifiersForSingleFlowMethods");
	std::vector<ChangeRateModifiersForSingleFlowMethod> singleChangeRateModifiersForSingleFlowMethod;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingChangeRateModifiersForSingleFlowMethods is needed for the construction of ChangeRateModifiers";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleChangeRateModifiersForSingleFlowMethod.push_back( *(static_cast<ChangeRateModifiersForSingleFlowMethod*>(*currentVoidPointer)) ); }
	}
	return ( new ChangeRateModifiers(singleChangeRateModifiersForSingleFlowMethod) );
}

ChangeRateModifiersForSingleFlowMethod* SedFlowBuilders::changeRateModifiersForSingleFlowMethodBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingChangeRateModifiersTypes");
	std::vector<ChangeRateModifiersType*> singleChangeRateModifiersTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingChangeRateModifiersTypes is needed for the construction of ChangeRateModifiersForSingleFlowMethod";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleChangeRateModifiersTypes.push_back( static_cast<ChangeRateModifiersType*>(*currentVoidPointer) ); }
	}
	return ( new ChangeRateModifiersForSingleFlowMethod(singleChangeRateModifiersTypes) );
}

ChangeRateModifiersType* SedFlowBuilders::changeRateModifiersTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::changeRateModifiersTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

RiverReachMethods* SedFlowBuilders::riverReachMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = labelledObjects.find("regularRiverReachMethods");
	RegularRiverReachMethods regularRiverReachMethods;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const regularRiverReachMethodsErrorMessage = "The variable regularRiverReachMethods is needed for the construction of RiverReachMethods";
		throw(regularRiverReachMethodsErrorMessage);
	}
	else { regularRiverReachMethods = *(static_cast<RegularRiverReachMethods*>( ((*tempObjectsIterator).second).at(0) )); }

	tempObjectsIterator = labelledObjects.find("additionalRiverReachMethods");
	AdditionalRiverReachMethods additionalRiverReachMethods;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const additionalRiverReachMethodsErrorMessage = "The variable additionalRiverReachMethods is needed for the construction of RiverReachMethods";
		throw(additionalRiverReachMethodsErrorMessage);
	}
	else { additionalRiverReachMethods = *(static_cast<AdditionalRiverReachMethods*>( ((*tempObjectsIterator).second).at(0) )); }

	std::map< std::string, std::vector<int> >::const_iterator tempIntIterator = constructionVariables.labelledInts.find("cellID");
	int userCellID;
	if(tempIntIterator == constructionVariables.labelledInts.end() )
	{
		const char *const cellIDErrorMessage = "The variable cellID is needed for the construction of RiverReachMethods";
		throw(cellIDErrorMessage);
	}
	else { userCellID = (tempIntIterator->second).at(0); }

	RiverReachProperties* riverReachProperties = highestOrderStructuresPointers.riverSystemProperties->regularRiverSystemProperties.getReachPropertiesPointerCorrespondingToUserCellID(userCellID);

	return ( new RiverReachMethods(highestOrderStructuresPointers.overallMethods, regularRiverReachMethods, additionalRiverReachMethods, *riverReachProperties) );
}

AdditionalRiverReachMethods* SedFlowBuilders::additionalRiverReachMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator = labelledObjects.find("constitutingAdditionalRiverReachMethodTypes");
	std::vector<AdditionalRiverReachMethodType*> singleAdditionalRiverReachMethodTypes;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const errorMessage = "The variable constitutingAdditionalRiverReachMethodTypes is needed for the construction of AdditionalRiverReachMethods";
		throw(errorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ singleAdditionalRiverReachMethodTypes.push_back(static_cast<AdditionalRiverReachMethodType*>(*currentVoidPointer) ); }
	}
	return ( new AdditionalRiverReachMethods(singleAdditionalRiverReachMethodTypes) );
}

AdditionalRiverReachMethodType* SedFlowBuilders::additionalRiverReachMethodTypeBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::additionalRiverReachMethodTypeBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

RegularRiverReachMethods* SedFlowBuilders::regularRiverReachMethodsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempObjectsIterator = labelledObjects.find("tauCalculationMethod");
	CalcTau* tauCalculationMethod;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const tauCalculationMethodErrorMessage = "The variable tauCalculationMethod is needed for the construction of RegularRiverReachMethods";
		throw(tauCalculationMethodErrorMessage);
	}
	else { tauCalculationMethod = static_cast<CalcTau*>( ((*tempObjectsIterator).second).at(0) ); }

	tempObjectsIterator = labelledObjects.find("activeWidthCalculationMethod");
	CalcActiveWidth* activeWidthCalculationMethod;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const activeWidthCalculationMethodErrorMessage = "The variable activeWidthCalculationMethod is needed for the construction of RegularRiverReachMethods";
		throw(activeWidthCalculationMethodErrorMessage);
	}
	else { activeWidthCalculationMethod = static_cast<CalcActiveWidth*>( ((*tempObjectsIterator).second).at(0) ); }

	return ( new RegularRiverReachMethods(tauCalculationMethod, activeWidthCalculationMethod) );
}

CalcTau* SedFlowBuilders::calcTauBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcTauBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

CalcBedloadVelocity* SedFlowBuilders::calcBedloadVelocityBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcBedloadVelocityBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

EstimateThicknessOfMovingSedimentLayer* SedFlowBuilders::estimateThicknessOfMovingSedimentLayerBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::estimateThicknessOfMovingSedimentLayerBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

CalcActiveWidth* SedFlowBuilders::calcActiveWidthBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcActiveWidthBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

CalcThresholdForInitiationOfBedloadMotion* SedFlowBuilders::calcThresholdForInitiationOfBedloadMotionBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcThresholdForInitiationOfBedloadMotionBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}

CalcHidingFactors* SedFlowBuilders::calcHidingFactorsBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	return SedFlowInterfaceRealisationBuilders::calcHidingFactorsBuilder(constructionVariables, highestOrderStructuresPointers, labelledObjects);
}


TimeSeriesEntry* SedFlowBuilders::timeSeriesEntryBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	TimeSeriesEntry* result;
	bool grainsEntry = false;

	std::map< std::string, std::vector<double> >::const_iterator tempDoubleIterator;
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempDoubleIterator = constructionVariables.labelledDoubles.find("elapsedSeconds");
	double elapsedSeconds;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() )
	{
		const char *const elapsedSecondsErrorMessage = "The variable elapsedSeconds is needed for the construction of a TimeSeriesEntry.";
		throw(elapsedSecondsErrorMessage);
	}
	else { elapsedSeconds = ((*tempDoubleIterator).second).at(0); }

	tempDoubleIterator = constructionVariables.labelledDoubles.find("doubleValue");
	double doubleValue;
	if(tempDoubleIterator == constructionVariables.labelledDoubles.end() ) { grainsEntry = true; }
	else { doubleValue = ((*tempDoubleIterator).second).at(0); }

	if (grainsEntry)
	{
		tempObjectsIterator = labelledObjects.find("grainsValue");
		if(tempObjectsIterator == labelledObjects.end() )
		{
				const char *const valueErrorMessage = "Either the variable doubleValue or the variable grainsValue is needed for the construction of a TimeSeriesEntry.";
				throw(valueErrorMessage);
		}
		else
		{
			Grains grainsValue = *( static_cast<Grains*>( ((*tempObjectsIterator).second).at(0) ) );
			result = new TimeSeriesEntry(elapsedSeconds,grainsValue);
		}
	}
	else
	{
		result = new TimeSeriesEntry(elapsedSeconds,doubleValue);
	}

	return result;
}

TimeSeries* SedFlowBuilders::timeSeriesBuilder(const ConstructionVariables& constructionVariables, const HighestOrderStructuresPointers& highestOrderStructuresPointers, std::map< std::string, std::vector<void*> > labelledObjects)
{
	std::map< std::string, std::vector<int> >::const_iterator tempIntIterator;
	std::map< std::string, std::vector<bool> >::const_iterator tempBoolIterator;
	std::map< std::string, std::vector<std::string> >::const_iterator tempStringIterator;
	std::map< std::string, std::vector<void*> >::const_iterator tempObjectsIterator;

	tempIntIterator = constructionVariables.labelledInts.find("userCellID");
	int userCellID;
	if(tempIntIterator == constructionVariables.labelledInts.end() )
	{
		const char *const userCellIDErrorMessage = "The variable userCellID is needed for the construction of a TimeSeries.";
		throw(userCellIDErrorMessage);
	}
	else { userCellID = ((*tempIntIterator).second).at(0); }

	tempBoolIterator = constructionVariables.labelledBools.find("margin");
	bool margin;
	if(tempBoolIterator == constructionVariables.labelledBools.end() )
	{
		const char *const marginErrorMessage = "The variable margin is needed for the construction of a TimeSeries.";
		throw(marginErrorMessage);
	}
	else { margin = ((*tempBoolIterator).second).at(0); }

	tempStringIterator = constructionVariables.labelledStrings.find("property");
	CombinerVariables::TypesOfRegularRiverReachProperties property;
	if(tempStringIterator == constructionVariables.labelledStrings.end() )
	{
		const char *const propertyErrorMessage = "The variable property is needed for the construction of a TimeSeries.";
		throw(propertyErrorMessage);
	}
	else { property = CombinerVariables::stringToTypeOfRegularRiverReachProperties( ((*tempStringIterator).second).at(0) ); }

	tempObjectsIterator = labelledObjects.find("timeSeries");
	std::vector<TimeSeriesEntry> timeSeries;
	if(tempObjectsIterator == labelledObjects.end() )
	{
		const char *const timeSeriesErrorMessage = "The variable timeSeries is needed for the construction of a TimeSeries.";
		throw(timeSeriesErrorMessage);
	}
	else
	{
		for(std::vector<void*>::const_iterator currentVoidPointer = ((*tempObjectsIterator).second).begin(); currentVoidPointer < ((*tempObjectsIterator).second).end(); ++currentVoidPointer)
			{ timeSeries.push_back( *(static_cast<TimeSeriesEntry*>(*currentVoidPointer)) ); }
	}

	tempBoolIterator = constructionVariables.labelledBools.find("surplusInsteadOfAbsoluteValues");
	bool surplusInsteadOfAbsoluteValues;
	if(tempBoolIterator == constructionVariables.labelledBools.end() ) { surplusInsteadOfAbsoluteValues = false; }
	else { surplusInsteadOfAbsoluteValues = ((*tempBoolIterator).second).at(0); }

	tempBoolIterator = constructionVariables.labelledBools.find("finalInsteadOfInitialAdding");
	bool finalInsteadOfInitialAdding;
	if(tempBoolIterator == constructionVariables.labelledBools.end() )
	{
		if( surplusInsteadOfAbsoluteValues )
		{
			const char *const finalInsteadOfInitialAddingErrorMessage = "The variable finalInsteadOfInitialAdding is needed for the construction of a TimeSeries if it is surplusInsteadOfAbsoluteValues.";
			throw(finalInsteadOfInitialAddingErrorMessage);
		}
		else
		{
			finalInsteadOfInitialAdding = false;
		}
	}
	else
	{
		finalInsteadOfInitialAdding = ((*tempBoolIterator).second).at(0);
	}

	return (new TimeSeries(userCellID,margin,property,timeSeries,surplusInsteadOfAbsoluteValues,finalInsteadOfInitialAdding,highestOrderStructuresPointers.riverSystemProperties->regularRiverSystemProperties) );
}

}
