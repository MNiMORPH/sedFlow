/*
 * SolveForWaterEnergyslopeBasedOnHydraulicHead.cpp
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

#include "SolveForWaterEnergyslopeBasedOnHydraulicHead.h"

#include <limits>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <cstring>

namespace SedFlow {

SolveForWaterEnergyslopeBasedOnHydraulicHead::SolveForWaterEnergyslopeBasedOnHydraulicHead(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder),
	usedFlowResistanceRelation( usedFlowResistanceRelation->createFlowResistancePointerCopy() ),
	maximumFroudeNumber(maximumFroudeNumber),
	turbulenceLossFactor(0.0),
	elapsedSeconds( std::numeric_limits<double>::quiet_NaN() )
{
	this->typeOfFlowResistance = CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead;
}

SolveForWaterEnergyslopeBasedOnHydraulicHead::SolveForWaterEnergyslopeBasedOnHydraulicHead(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation, double turbulenceLossFactor):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder),
	usedFlowResistanceRelation( usedFlowResistanceRelation->createFlowResistancePointerCopy() ),
	maximumFroudeNumber(maximumFroudeNumber),
	turbulenceLossFactor(turbulenceLossFactor),
	elapsedSeconds( std::numeric_limits<double>::quiet_NaN() )
{
	this->typeOfFlowResistance = CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead;
}

SolveForWaterEnergyslopeBasedOnHydraulicHead::SolveForWaterEnergyslopeBasedOnHydraulicHead(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const FlowResistance* const usedFlowResistanceRelation, double turbulenceLossFactor, double elapsedSeconds, const std::set<int>& alreadyTreatedCellIDs, const std::set<int>& everTreatedCellIDs):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,everTreatedCellIDs),
	usedFlowResistanceRelation( usedFlowResistanceRelation->createFlowResistancePointerCopy() ),
	maximumFroudeNumber(maximumFroudeNumber),
	turbulenceLossFactor(turbulenceLossFactor),
	elapsedSeconds(elapsedSeconds),
	alreadyTreatedCellIDs(alreadyTreatedCellIDs)
{
	this->typeOfFlowResistance = CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead;
}



SolveForWaterEnergyslopeBasedOnHydraulicHead::~SolveForWaterEnergyslopeBasedOnHydraulicHead()
{
	delete usedFlowResistanceRelation;
}

FlowResistance* SolveForWaterEnergyslopeBasedOnHydraulicHead::createFlowResistancePointerCopy() const
{
	FlowResistance* result = new SolveForWaterEnergyslopeBasedOnHydraulicHead(this->startingValueForIteration,this->accuracyForTerminatingIteration,this->maximumNumberOfIterations,this->useApproximationsForHydraulicRadius,this->maximumFroudeNumber,this->minimumHydraulicSlope,this->typeOfNumericRootFinder,this->usedFlowResistanceRelation,this->turbulenceLossFactor,this->elapsedSeconds,this->alreadyTreatedCellIDs,this->everTreatedCellIDs);
	return result;
}

ConstructionVariables SolveForWaterEnergyslopeBasedOnHydraulicHead::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowResistance;
	result.realisationType = CombinerVariables::typeOfFlowResistanceToString(CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead);
	std::vector<double> doubleVector;
	doubleVector.push_back(startingValueForIteration);
	result.labelledDoubles["startingValueForIteration"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(accuracyForTerminatingIteration);
	result.labelledDoubles["accuracyForTerminatingIteration"] = doubleVector;
	std::vector<int> intVector;
	intVector.push_back(maximumNumberOfIterations);
	result.labelledInts["maximumNumberOfIterations"] = intVector;
	std::vector<bool> boolVector;
	boolVector.push_back(useApproximationsForHydraulicRadius);
	result.labelledBools["useApproximationsForHydraulicRadius"] = boolVector;
	doubleVector.clear();
	doubleVector.push_back(maximumFroudeNumber);
	result.labelledDoubles["maximumFroudeNumber"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(minimumHydraulicSlope);
	result.labelledDoubles["minimumHydraulicSlope"] = doubleVector;
	std::vector<std::string> stringVector;
	stringVector.push_back(getTypeOfNumericRootFinderAsString());
	result.labelledStrings["typeOfNumericRootFinder"] = stringVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	constructionVariablesVector.push_back( usedFlowResistanceRelation->createConstructionVariables() );
	result.labelledObjects["usedFlowResistanceRelation"] = constructionVariablesVector;
	doubleVector.clear();
	doubleVector.push_back(turbulenceLossFactor);
	result.labelledDoubles["turbulenceLossFactor"] = doubleVector;
	return result;
}

PowerLawRelation SolveForWaterEnergyslopeBasedOnHydraulicHead::dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method SolveForWaterEnergyslopeBasedOnHydraulicHead::dischargeAsPowerLawFunctionOfWaterVolumeInReach has not been implemented yet.";
	throw(errorMessage);
	PowerLawRelation fakeResult = PowerLawRelation(false);
	return fakeResult;
}

std::pair<double,double> SolveForWaterEnergyslopeBasedOnHydraulicHead::calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(double discharge, const RiverReachProperties& riverReachProperties) const
{
	double reallyElapsedSeconds = (riverReachProperties.getOverallParameters())->getElapsedSeconds();
	if( !( this->elapsedSeconds < (0.0001 + reallyElapsedSeconds) ) )//i.e. if we reached a new time step
	{
		(const_cast<SolveForWaterEnergyslopeBasedOnHydraulicHead*>(this))->alreadyTreatedCellIDs.clear();
		(const_cast<SolveForWaterEnergyslopeBasedOnHydraulicHead*>(this))->elapsedSeconds = reallyElapsedSeconds;
	}

	std::pair<double,double> result (0.0,0.0);
	if(discharge > 0.0)
	{
		if( riverReachProperties.isDownstreamMargin() )
		{
			result = usedFlowResistanceRelation->calculateFlowDepthAndFlowVelocityUsingDischargeAsInput(discharge,riverReachProperties);
			(const_cast<SolveForWaterEnergyslopeBasedOnHydraulicHead*>(this))->alreadyTreatedCellIDs.insert( riverReachProperties.getCellID() );
		}
		else
		{
			const RiverReachProperties& downstreamRiverReachProperties ( (*(riverReachProperties.getDownstreamCellPointer())) );

			std::set<int>::const_iterator downstreamIDAmongAlreadyTreated = this->alreadyTreatedCellIDs.find( downstreamRiverReachProperties.getCellID() );
			if( downstreamIDAmongAlreadyTreated != alreadyTreatedCellIDs.end() )//Continue only if it makes sense.
			{
				double gravityAcceleration = (riverReachProperties.getOverallParameters())->getGravityAcceleration();
				double downstreamFrictionSlope = usedFlowResistanceRelation->returnlocalFrictionSlope(downstreamRiverReachProperties);
				double downstreamKineticHydraulicHead = downstreamRiverReachProperties.regularRiverReachProperties.kineticComponentOfHydraulicHead();
				double downstreamHydraulicHead = downstreamRiverReachProperties.regularRiverReachProperties.hydraulicHead();
				double length = riverReachProperties.regularRiverReachProperties.length;
				double localElevation = riverReachProperties.regularRiverReachProperties.elevation;
				bool downstreamSillOccurence = downstreamRiverReachProperties.regularRiverReachProperties.sillOccurence;
				if( downstreamSillOccurence )
				{
					downstreamHydraulicHead = std::max(downstreamHydraulicHead,downstreamRiverReachProperties.sillProperties->calculateFreeOverfallHydraulicHeadAtSill(discharge, downstreamRiverReachProperties.regularRiverReachProperties, downstreamRiverReachProperties.additionalRiverReachProperties));
				}

				double outputFlowDepth;

				double inputFlowDepth = this->getStartingFlowDepthForIteration(riverReachProperties);
				double tempCrosssectionalArea, tempFlowVelocity, tempLocalKineticHydraulicHead, tempLocalFrictionSlope, tempAverageFrictionSlope, tempFrictionLoss, tempTurbulenceLoss, tempLocalHydraulicHead, tempLocalPotentialHydraulicHead;
				double tempAccuracy = std::numeric_limits<double>::max() - 2.0;
				double previousTempAccuracy =  std::numeric_limits<double>::max() - 1.0;
				bool converging = true;
				bool acceptableNumberOfIterations = true;
				bool targetNOTreachedYet = true;

				int currentIteration = 1;
				while ( converging && acceptableNumberOfIterations && targetNOTreachedYet )
				{
					tempCrosssectionalArea = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(inputFlowDepth);
					tempFlowVelocity = discharge / tempCrosssectionalArea;
					tempLocalKineticHydraulicHead = BasicCalculations::hydraulicHeadBernoulliEquationKineticComponent(tempFlowVelocity,gravityAcceleration);
					tempLocalFrictionSlope = this->usedFlowResistanceRelation->returnlocalFrictionSlopeNotUsingLocalFlowDepth(inputFlowDepth,riverReachProperties);
					if(downstreamSillOccurence)
						{ tempAverageFrictionSlope  = tempLocalFrictionSlope; }
					else
						{ tempAverageFrictionSlope  = 0.5 * (tempLocalFrictionSlope + downstreamFrictionSlope); }
					tempFrictionLoss = tempAverageFrictionSlope * length;
					//In case of a downstream sill occurence the downstream kinetic head is unknown. In this case the tempTurbulenceLoss is just a lower boundary for the actual value.
					tempTurbulenceLoss = fabs( this->turbulenceLossFactor * (tempLocalKineticHydraulicHead - downstreamKineticHydraulicHead) );
					tempLocalHydraulicHead = downstreamHydraulicHead + tempFrictionLoss + tempTurbulenceLoss;
					tempLocalPotentialHydraulicHead = tempLocalHydraulicHead - tempLocalKineticHydraulicHead;
					outputFlowDepth = tempLocalPotentialHydraulicHead - localElevation;

					tempAccuracy = fabs( (inputFlowDepth - outputFlowDepth) );

					if(tempAccuracy >= previousTempAccuracy)
						{ converging = false; }

					inputFlowDepth = outputFlowDepth;
					previousTempAccuracy = tempAccuracy;
					++currentIteration;

					if(tempAccuracy <= accuracyForTerminatingIteration)
						{ targetNOTreachedYet = false; }
					if( targetNOTreachedYet && (currentIteration > this->maximumNumberOfIterations) )
						{ acceptableNumberOfIterations = false; }
				}

				if( outputFlowDepth <= 0.0 )
				{
					outputFlowDepth = accuracyForTerminatingIteration;
				}
				double flowVelocity = discharge / riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(outputFlowDepth);
				result = std::make_pair(outputFlowDepth,flowVelocity);

				if(!( converging && acceptableNumberOfIterations ))
				{
					std::string badIterationErrorMessageAsString;
					std::ostringstream oStringStream;
					if(converging)
					{
						oStringStream << "Maximum number of iterations (i.e. " << this->maximumNumberOfIterations << ") has been reached" << std::flush;
					}
					else
					{
						oStringStream << "Not converging iteration" << std::flush;
					}
					oStringStream << " at ";
					if(riverReachProperties.isMargin())
					{
						if(riverReachProperties.isDownstreamMargin())
						{
							oStringStream << "downstream margin";
						}
						else
						{
							oStringStream << "upstream margin at reach " << riverReachProperties.getDownstreamUserCellID();
						}
					}
					else
					{
						oStringStream << "reach " << riverReachProperties.getUserCellID();
					}
					oStringStream << ". (SolveForWaterEnergyslopeBasedOnHydraulicHead::calculateFlowDepthAndFlowVelocityUsingDischargeAsInput)" << std::flush;

					char* tmpChar = new char [(oStringStream.str()).size()+1];
					std::strcpy(tmpChar, (oStringStream.str()).c_str());
					const char *const badIterationErrorMessage = tmpChar;
					throw(badIterationErrorMessage);
				}
				(const_cast<SolveForWaterEnergyslopeBasedOnHydraulicHead*>(this))->alreadyTreatedCellIDs.insert( riverReachProperties.getCellID() );
			}
			else
			{
				result = std::make_pair(riverReachProperties.regularRiverReachProperties.maximumWaterdepth,riverReachProperties.regularRiverReachProperties.flowVelocity);
			}
		}
	}
	return result;
}

std::pair<double,double> SolveForWaterEnergyslopeBasedOnHydraulicHead::calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method SolveForWaterEnergyslopeBasedOnHydraulicHead::calculateDischargeAndFlowVelocityUsingFlowDepthAsInput has not been implemented yet.";
	throw(errorMessage);
	std::pair<double,double> fakeResult = std::make_pair(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN());
	return fakeResult;
}

std::pair<double,double> SolveForWaterEnergyslopeBasedOnHydraulicHead::calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method SolveForWaterEnergyslopeBasedOnHydraulicHead::calculateDischargeAndFlowDepthUsingFlowVelocityAsInput has not been implemented yet.";
	throw(errorMessage);
	std::pair<double,double> fakeResult = std::make_pair(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN());
	return fakeResult;
}

double SolveForWaterEnergyslopeBasedOnHydraulicHead::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method SolveForWaterEnergyslopeBasedOnHydraulicHead::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth has not been implemented yet.";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

PowerLawRelation SolveForWaterEnergyslopeBasedOnHydraulicHead::darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method SolveForWaterEnergyslopeBasedOnHydraulicHead::darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth has not been implemented yet.";
	throw(errorMessage);
	PowerLawRelation fakeResult (false);
	return fakeResult;
}

double SolveForWaterEnergyslopeBasedOnHydraulicHead::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method SolveForWaterEnergyslopeBasedOnHydraulicHead::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity has not been implemented yet.";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > SolveForWaterEnergyslopeBasedOnHydraulicHead::getInternalParameters() const
{
	std::vector<double> empty;
	return std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > (CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead, empty);
}

void SolveForWaterEnergyslopeBasedOnHydraulicHead::setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters)
{
	if( newParameters.first != CombinerVariables::SolveForWaterEnergyslopeBasedOnHydraulicHead)
	{
		const char *const errorMessage = "Action on not matching types of flow resistance";
		throw(errorMessage);
	}
}


}
