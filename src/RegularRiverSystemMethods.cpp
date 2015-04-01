/*
 * RegularRiverSystemMethods.cpp
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

#include "RegularRiverSystemMethods.h"

#include <vector>
#include <algorithm>

namespace SedFlow {

RegularRiverSystemMethods::RegularRiverSystemMethods(std::vector<RiverReachMethods> riverReachMethods, FlowMethods flowMethods, ChangeRateModifiers changeRateModifiers):
	riverReachMethods(riverReachMethods),
	flowMethods(flowMethods),
	changeRateModifiers(changeRateModifiers)
{
	std::sort(this->riverReachMethods.begin(),this->riverReachMethods.end());
}

RegularRiverSystemMethods::~RegularRiverSystemMethods() {}

ConstructionVariables RegularRiverSystemMethods::createConstructionVariables(const RiverSystemProperties& parameters)const
{
	std::map<std::string, std::vector<int> >::iterator labelledIntsIterator;
	ConstructionVariables* currentConstructionVariablesVectorEnd;
	std::vector<int> intVector;
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RegularRiverSystemMethods;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.reserve( riverReachMethods.size() );
	for(std::vector<RiverReachMethods>::const_iterator iterator = riverReachMethods.begin(); iterator < riverReachMethods.end(); ++iterator)
	{
		constructionVariablesVector.push_back( (*iterator).createConstructionVariables() );
		currentConstructionVariablesVectorEnd = &(constructionVariablesVector.back());
		labelledIntsIterator = currentConstructionVariablesVectorEnd->labelledInts.find("internalCellID");
		intVector.push_back( parameters.regularRiverSystemProperties.getUserCellIDcorrespondingToRealCellID( (labelledIntsIterator->second).at(0) )  );
		currentConstructionVariablesVectorEnd->labelledInts["cellID"] = intVector;
		intVector.clear();
	}
	result.labelledObjects["riverReachMethods"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( flowMethods.createConstructionVariables() );
	result.labelledObjects["flowMethods"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( changeRateModifiers.createConstructionVariables() );
	result.labelledObjects["changeRateModifiers"] = constructionVariablesVector;
	return result;
}

void RegularRiverSystemMethods::throwExceptionIfWaterFlowIsNotHighestOrderFlowMethod()
{
	if ( !(flowMethods.isFirstWaterFlow()) )
	{
		const char *const errorMessage = "WaterFlow is not highest order FlowMethod!!";
		throw(errorMessage);
	}
}

void RegularRiverSystemMethods::calculateAndModifyChangeRates(RiverSystemProperties& parameters, const OverallMethods& overallMethods)
{
	FlowTypeMethods* sedimentFlowMethodsAsFlowTypeMethods = flowMethods.createPointerCopyOfSingleFlowTypeMethods(CombinerVariables::sedimentFlowMethods);
	SedimentFlowMethods& sedimentFlowMethods = *( static_cast<SedimentFlowMethods*>(sedimentFlowMethodsAsFlowTypeMethods) );
	RiverReachProperties* currentRiverReachProperties;
	RiverReachProperties* currentUpstreamProperties;
	RiverReachMethods* currentRiverReachMethods;
	ChangeRateModifiersForSingleFlowMethod* currentModifiers;
	CombinerVariables::TypesOfGeneralFlowMethods currentGeneralFlowMethodType;
	for(std::vector<FlowTypeMethods*>::const_iterator currentFlowMethod = flowMethods.getBeginFlowMethodsTypeConstIterator(); currentFlowMethod < flowMethods.getEndFlowMethodsTypeConstIterator(); ++currentFlowMethod)
	{
		currentGeneralFlowMethodType = (*(*currentFlowMethod)).getTypeOfGeneralFlowMethods();

		if (changeRateModifiers.checkForGeneralFlowMethodTreatment(currentGeneralFlowMethodType))
		{
			currentModifiers = changeRateModifiers.getChangeRateModifiersCorrespondingToGeneralFlowMethodPointer(currentGeneralFlowMethodType);

#if defined SEDFLOWPARALLEL
			//TODO Delete this debugging line.
			std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_1" << std::endl << std::endl;
#endif
			#pragma omp parallel for private(currentRiverReachProperties)
			for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
			{
				currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
				currentModifiers->inputModification(*currentRiverReachProperties);
			}
#if defined SEDFLOWPARALLEL
			//TODO Delete this debugging line.
			std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_2" << std::endl << std::endl;
#endif
			#pragma omp parallel for private(currentRiverReachMethods, currentRiverReachProperties, currentUpstreamProperties)
			for(int i = 0; i < riverReachMethods.size(); ++i)
			{
				currentRiverReachMethods = &(riverReachMethods[i]);
				currentRiverReachProperties = currentRiverReachMethods->getPointerToCorrespondingRiverReachProperties();
				currentUpstreamProperties = (currentRiverReachProperties->getUpstreamCellPointers()).at(0);
				if(currentUpstreamProperties->isUpstreamMargin())
				{
					updateBedSlope(*currentUpstreamProperties, overallMethods);
					updateWaterEnergySlope(*currentUpstreamProperties, overallMethods);
					updateSedimentEnergySlope(*currentUpstreamProperties, overallMethods);
					RegularRiverReachMethods regularRiverReachMethods = currentRiverReachMethods->getRegularRiverReachMethods();
					regularRiverReachMethods.calculateTau(*currentUpstreamProperties, overallMethods);
					regularRiverReachMethods.calculateActiveWidth(*currentUpstreamProperties, sedimentFlowMethods, overallMethods);
				}
				updateBedSlope(*currentRiverReachProperties, overallMethods);
				updateWaterEnergySlope(*currentRiverReachProperties, overallMethods);
				updateSedimentEnergySlope(*currentRiverReachProperties, overallMethods);
				currentRiverReachMethods->calculateTau();
				currentRiverReachMethods->calculateActiveWidth(sedimentFlowMethods);
			}

			do {

#if defined SEDFLOWPARALLEL
				//TODO Delete this debugging line.
				std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_3" << std::endl << std::endl;
#endif
				#pragma omp parallel for private (currentRiverReachProperties)
				for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
				{
					currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
					(*currentFlowMethod)->calculateChangeRate(*currentRiverReachProperties);
#if defined SEDFLOWPARALLEL
				}
				#pragma omp parallel for private (currentRiverReachProperties)
				for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
				{
					currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
#endif
					(*currentFlowMethod)->handDownChangeRate(*currentRiverReachProperties);
					currentModifiers->modificationBeforeUpdates(*currentRiverReachProperties);
				}
#if defined SEDFLOWPARALLEL
				//TODO Delete this debugging line.
				std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_4" << std::endl << std::endl;
#endif
				#pragma omp parallel for private(currentRiverReachMethods, currentRiverReachProperties, currentUpstreamProperties)
				for(int i = (riverReachMethods.size()-1); i > -1; --i)
				{
					currentRiverReachMethods = &(riverReachMethods[i]);
					currentRiverReachProperties = currentRiverReachMethods->getPointerToCorrespondingRiverReachProperties();
					currentUpstreamProperties = (currentRiverReachProperties->getUpstreamCellPointers()).at(0);
					updateBedSlope(*currentRiverReachProperties, overallMethods);
					flowMethods.updateOtherParameters(*currentRiverReachProperties);
					if(currentUpstreamProperties->isUpstreamMargin())
					{
						updateBedSlope(*currentUpstreamProperties, overallMethods);
						flowMethods.updateOtherParameters(*currentUpstreamProperties);
					}
				}
#if defined SEDFLOWPARALLEL
				//TODO Delete this debugging line.
				std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_5" << std::endl << std::endl;
#endif
				#pragma omp parallel for private(currentRiverReachMethods, currentRiverReachProperties, currentUpstreamProperties)
				for(int i = 0; i < riverReachMethods.size(); ++i)
				{
					currentRiverReachMethods = &(riverReachMethods[i]);
					currentRiverReachProperties = currentRiverReachMethods->getPointerToCorrespondingRiverReachProperties();
					currentUpstreamProperties = (currentRiverReachProperties->getUpstreamCellPointers()).at(0);
					if(currentUpstreamProperties->isUpstreamMargin())
					{
						flowMethods.handDownOtherParameters(*currentUpstreamProperties);
						updateWaterEnergySlope(*currentUpstreamProperties, overallMethods);
						updateSedimentEnergySlope(*currentUpstreamProperties, overallMethods);
						RegularRiverReachMethods regularRiverReachMethods = currentRiverReachMethods->getRegularRiverReachMethods();
						regularRiverReachMethods.calculateTau(*currentUpstreamProperties, overallMethods);
						regularRiverReachMethods.calculateActiveWidth(*currentUpstreamProperties, sedimentFlowMethods, overallMethods);
						(*(*currentFlowMethod)).updateChangeRateDependingParameters(*currentUpstreamProperties);
					}
					flowMethods.handDownOtherParameters(*currentRiverReachProperties);
					currentRiverReachMethods->updateAdditionalRiverReachProperties();
					updateWaterEnergySlope(*currentRiverReachProperties, overallMethods);
					updateSedimentEnergySlope(*currentRiverReachProperties, overallMethods);
					currentRiverReachMethods->calculateTau();
					currentRiverReachMethods->calculateActiveWidth(sedimentFlowMethods);
					(*(*currentFlowMethod)).updateChangeRateDependingParameters(*currentRiverReachProperties);
				}

			} while ( currentModifiers->furtherModificationIterationNecessary(parameters) );

#if defined SEDFLOWPARALLEL
			//TODO Delete this debugging line.
			std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_6" << std::endl << std::endl;
#endif
			#pragma omp parallel for private(currentRiverReachProperties)
			for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
			{
				currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
				currentModifiers->finalModification(*currentRiverReachProperties);
			}
#if defined SEDFLOWPARALLEL
			//TODO Delete this debugging line.
			std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_7" << std::endl << std::endl;
#endif
			#pragma omp parallel for private(currentRiverReachMethods, currentRiverReachProperties, currentUpstreamProperties)
			for(int i = 0; i < riverReachMethods.size(); ++i)
			{
				currentRiverReachMethods = &(riverReachMethods[i]);
				currentRiverReachProperties = currentRiverReachMethods->getPointerToCorrespondingRiverReachProperties();
				currentUpstreamProperties = (currentRiverReachProperties->getUpstreamCellPointers()).at(0);
				if(currentUpstreamProperties->isUpstreamMargin())
				{
					updateBedSlope(*currentUpstreamProperties, overallMethods);
					updateWaterEnergySlope(*currentUpstreamProperties, overallMethods);
					updateSedimentEnergySlope(*currentUpstreamProperties, overallMethods);
					RegularRiverReachMethods regularRiverReachMethods = currentRiverReachMethods->getRegularRiverReachMethods();
					regularRiverReachMethods.calculateTau(*currentUpstreamProperties, overallMethods);
					regularRiverReachMethods.calculateActiveWidth(*currentUpstreamProperties, sedimentFlowMethods, overallMethods);
				}
				updateBedSlope(*currentRiverReachProperties, overallMethods);
				updateWaterEnergySlope(*currentRiverReachProperties, overallMethods);
				updateSedimentEnergySlope(*currentRiverReachProperties, overallMethods);
				currentRiverReachMethods->calculateTau();
				currentRiverReachMethods->calculateActiveWidth(sedimentFlowMethods);
			}
		}
		else
		{
#if defined SEDFLOWPARALLEL
			//TODO Delete this debugging line.
			std::cout << "OMP RegularRiverSystemMethods::calculateAndModifyChangeRates_8" << std::endl << std::endl;
#endif
			#pragma omp parallel for private(currentRiverReachProperties)
			for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
			{
				currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
				(*(*currentFlowMethod)).calculateChangeRate(*currentRiverReachProperties);
#if defined SEDFLOWPARALLEL
			}
			#pragma omp parallel for private(currentRiverReachProperties)
			for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
			{
				currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
#endif
				(*(*currentFlowMethod)).handDownChangeRate(*currentRiverReachProperties);
				(*(*currentFlowMethod)).updateChangeRateDependingParameters(*currentRiverReachProperties);
			}
		}
	}
	delete sedimentFlowMethodsAsFlowTypeMethods;
}

double RegularRiverSystemMethods::calculateTimeStep(const RiverSystemProperties& parameters)
{
	return flowMethods.calculateTimeStep(parameters);
}

void RegularRiverSystemMethods::calculateAndHandDownChanges(RiverSystemProperties& parameters)
{
	double currentTimeStepLength = parameters.overallParameters.getCurrentTimeStepLengthInSeconds();

	//The serial execution of calculateChanges is done on purpose, as ImplicitKinematicWave::calculateChange depends on a serial execution.
	for(std::vector<RiverReachProperties>::iterator currentRiverReachPropertiesIterator = parameters.regularRiverSystemProperties.cellProperties.begin(); currentRiverReachPropertiesIterator < parameters.regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachPropertiesIterator)
	{
		flowMethods.calculateChanges(*currentRiverReachPropertiesIterator,currentTimeStepLength);
	}

	//This serial execution of handDownChanges after the complete execution of calculateChanges is done on purpose.
	//If calculateChanges and handDownChanges were executed in the same loop, the calc method would overwrite the the deposition which has been defined by the handDown method of the previous cell.
	//handDownChanges needs to be executed serial, since the deposition is handed downward and so the execution for single reach depends on the previous execution for the upstream reach.
	for(std::vector<RiverReachProperties>::iterator currentRiverReachPropertiesIterator = parameters.regularRiverSystemProperties.cellProperties.begin(); currentRiverReachPropertiesIterator < parameters.regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachPropertiesIterator)
	{
		flowMethods.handDownChanges(*currentRiverReachPropertiesIterator);
	}
}

void RegularRiverSystemMethods::performAdditionalReachActions()
{
	RiverReachMethods* currentRiverReachMethods;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP RegularRiverSystemMethods::performAdditionalReachActions" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentRiverReachMethods)
	for(int i = 0; i < riverReachMethods.size(); ++i)
	{
		currentRiverReachMethods = &(riverReachMethods[i]);
		(*currentRiverReachMethods).performAdditionalRiverReachActions();
	}
}

void RegularRiverSystemMethods::applyChanges(RiverSystemProperties& parameters)
{
	RiverReachProperties* currentRiverReachProperties;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP RegularRiverSystemMethods::applyChanges" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentRiverReachProperties)
	for(int i = 0; i < parameters.regularRiverSystemProperties.cellProperties.size(); ++i)
	{
		currentRiverReachProperties = &(parameters.regularRiverSystemProperties.cellProperties[i]);
		if( !(currentRiverReachProperties->isMargin()) )
		{
			flowMethods.applyChanges((*currentRiverReachProperties));
		}
	}
}

void RegularRiverSystemMethods::updateRegularProperties(RiverSystemProperties& parameters)
{
	for(std::vector<RiverReachProperties>::reverse_iterator currentRiverReachPropertiesBackward = parameters.regularRiverSystemProperties.cellProperties.rbegin(); currentRiverReachPropertiesBackward < parameters.regularRiverSystemProperties.cellProperties.rend(); ++currentRiverReachPropertiesBackward)
	{
		flowMethods.updateOtherParameters((*currentRiverReachPropertiesBackward));
	}

	for(std::vector<RiverReachProperties>::iterator currentRiverReachPropertiesForward = parameters.regularRiverSystemProperties.cellProperties.begin(); currentRiverReachPropertiesForward < parameters.regularRiverSystemProperties.cellProperties.end(); ++currentRiverReachPropertiesForward)
	{
		flowMethods.handDownOtherParameters(*currentRiverReachPropertiesForward);
	}
}

void RegularRiverSystemMethods::updateAdditionalRiverReachProperties()
{
	RiverReachMethods* currentRiverReachMethods;
#if defined SEDFLOWPARALLEL
	//TODO Delete this debugging line.
	std::cout << "OMP RegularRiverSystemMethods::updateAdditionalRiverReachProperties" << std::endl << std::endl;
#endif
	#pragma omp parallel for private(currentRiverReachMethods)
	for(int i = 0; i < riverReachMethods.size(); ++i)
	{
		currentRiverReachMethods = &(riverReachMethods[i]);
		(*currentRiverReachMethods).updateAdditionalRiverReachProperties(); }
}

void RegularRiverSystemMethods::updateBedSlopes(RiverSystemProperties& parameters, const OverallMethods& overallMethods)const
{
	parameters.regularRiverSystemProperties.setSingleRegularParameter(CombinerVariables::bedslope, overallMethods.bedSlopeCalculationMethod->calculate(parameters) );

	if( overallMethods.waterEnergySlopeCalculationMethod->dependsOnBedslope() )
	{
		this->updateWaterEnergySlopes(parameters,overallMethods);
	}

	if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnBedslope() )
	{
		this->updateSedimentEnergySlopes(parameters,overallMethods);
	}
}

void RegularRiverSystemMethods::updateBedSlope(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods)const
{
	riverReachProperties.regularRiverReachProperties.bedslope = overallMethods.bedSlopeCalculationMethod->calculate(riverReachProperties);

	if( overallMethods.waterEnergySlopeCalculationMethod->dependsOnBedslope() )
	{
		this->updateWaterEnergySlope(riverReachProperties,overallMethods);
	}

	if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnBedslope() )
	{
		this->updateSedimentEnergySlope(riverReachProperties,overallMethods);
	}
}

void RegularRiverSystemMethods::updateWaterEnergySlopes(RiverSystemProperties& parameters, const OverallMethods& overallMethods)const
{
	parameters.regularRiverSystemProperties.setSingleRegularParameter(CombinerVariables::waterEnergyslope, overallMethods.waterEnergySlopeCalculationMethod->calculate(parameters) );

	if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnWaterEnergyslope() )
	{
		this->updateSedimentEnergySlopes(parameters,overallMethods);
	}

}

void RegularRiverSystemMethods::updateWaterEnergySlope(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods)const
{
	riverReachProperties.regularRiverReachProperties.waterEnergyslope = overallMethods.waterEnergySlopeCalculationMethod->calculate(riverReachProperties);

	if( overallMethods.sedimentEnergySlopeCalculationMethod->dependsOnWaterEnergyslope() )
	{
		this->updateSedimentEnergySlope(riverReachProperties,overallMethods);
	}
}

void RegularRiverSystemMethods::updateSedimentEnergySlopes(RiverSystemProperties& parameters, const OverallMethods& overallMethods)const
{
	std::pair< std::vector<double>, std::vector<double> > reducedAndUnreducedSlopes = overallMethods.sedimentEnergySlopeCalculationMethod->calculateReducedAndUnreducedSedimentEnergyslope(parameters);
	parameters.regularRiverSystemProperties.setSingleRegularParameter(CombinerVariables::sedimentEnergyslope, reducedAndUnreducedSlopes.first );
	parameters.regularRiverSystemProperties.setSingleRegularParameter(CombinerVariables::unreducedSedimentEnergyslope, reducedAndUnreducedSlopes.second );
}

void RegularRiverSystemMethods::updateSedimentEnergySlope(RiverReachProperties& riverReachProperties, const OverallMethods& overallMethods)const
{
	std::pair<double,double> reducedAndUnreducedSlope =  overallMethods.sedimentEnergySlopeCalculationMethod->calculateReducedAndUnreducedSedimentEnergyslope(riverReachProperties);
	riverReachProperties.regularRiverReachProperties.sedimentEnergyslope = reducedAndUnreducedSlope.first;
	riverReachProperties.regularRiverReachProperties.unreducedSedimentEnergyslope = reducedAndUnreducedSlope.second;
}

void RegularRiverSystemMethods::updateTaus()const
{
	for(std::vector<RiverReachMethods>::const_iterator currentRiverReachMethods = riverReachMethods.begin(); currentRiverReachMethods < riverReachMethods.end(); ++currentRiverReachMethods)
			{ (*currentRiverReachMethods).calculateTau(); }
}

void RegularRiverSystemMethods::updateActiveWidths()const
{
	FlowTypeMethods* sedimentFlowMethods = flowMethods.createPointerCopyOfSingleFlowTypeMethods(CombinerVariables::sedimentFlowMethods);
	for(std::vector<RiverReachMethods>::const_iterator currentRiverReachMethods = riverReachMethods.begin(); currentRiverReachMethods < riverReachMethods.end(); ++currentRiverReachMethods)
			{ currentRiverReachMethods->calculateActiveWidth(*(static_cast<SedimentFlowMethods*>(sedimentFlowMethods))); }
	delete sedimentFlowMethods;
}

}
