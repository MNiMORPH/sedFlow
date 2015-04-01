/*
 * VariablePowerLawFlowResistance.cpp
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

#include "VariablePowerLawFlowResistance.h"

#include <math.h>
#include <limits>
#include <sstream>
#include <iostream>
#include <cstring>

#include "BedrockRoughnessContribution.h"

namespace SedFlow {

VariablePowerLawFlowResistance::VariablePowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder)
{
	this->typeOfFlowResistance = CombinerVariables::VariablePowerLawFlowResistance;
}

VariablePowerLawFlowResistance::VariablePowerLawFlowResistance(double startingValueForIteration, double accuracyForTerminatingIteration, int maximumNumberOfIterations, bool useApproximationsForHydraulicRadius, double maximumFroudeNumber, double minimumHydraulicSlope, CombinerVariables::TypesOfNumericRootFinder typeOfNumericRootFinder, const std::set<int>& everTreatedCellIDs):
	FlowResistance(startingValueForIteration,accuracyForTerminatingIteration,maximumNumberOfIterations,useApproximationsForHydraulicRadius,maximumFroudeNumber,minimumHydraulicSlope,typeOfNumericRootFinder,everTreatedCellIDs)
{
	this->typeOfFlowResistance = CombinerVariables::VariablePowerLawFlowResistance;
}

double VariablePowerLawFlowResistance::getInterpolatedDEightyfour(const RiverReachProperties& riverReachProperties) const
{
	double dEightyfour = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(),84.0);
	double bedrockContribution = BedrockRoughnessContribution::getBedrockRoughnessContribution(riverReachProperties);
	if( bedrockContribution > 0.0 )
	{
		double bedrockRepresentativeGrainDiameter = BedrockRoughnessContribution::getBedrockRoughnessEquivalentRepresentativeGrainDiameterValue(riverReachProperties);
		if( dEightyfour != dEightyfour )
		{
			dEightyfour = bedrockRepresentativeGrainDiameter;
		}
		else
		{
			dEightyfour = ( (dEightyfour * (1.0-bedrockContribution) ) + (bedrockRepresentativeGrainDiameter * bedrockContribution) );
		}
	}
	return dEightyfour;
}


FlowResistance* VariablePowerLawFlowResistance::createFlowResistancePointerCopy() const
{
	FlowResistance* result = new VariablePowerLawFlowResistance(this->startingValueForIteration,this->accuracyForTerminatingIteration,this->maximumNumberOfIterations,this->useApproximationsForHydraulicRadius,this->maximumFroudeNumber,this->minimumHydraulicSlope,this->typeOfNumericRootFinder,this->everTreatedCellIDs);
	return result;
}

ConstructionVariables VariablePowerLawFlowResistance::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::FlowResistance;
	result.realisationType = CombinerVariables::typeOfFlowResistanceToString(CombinerVariables::VariablePowerLawFlowResistance);
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
	return result;
}

PowerLawRelation VariablePowerLawFlowResistance::dischargeAsPowerLawFunctionOfWaterVolumeInReach(const RiverReachProperties& riverReachProperties) const
{
	return PowerLawRelation(false);
}

double VariablePowerLawFlowResistance::calculateFlowVelocityUsingDischargeAsInputForGivenDEightyfourAndFlowWidth(double discharge, const RiverReachProperties& riverReachProperties, double dEightyfour, double flowWidth) const
{
	if(riverReachProperties.regularRiverReachProperties.waterEnergyslope < 0.0)
	{
		std::ostringstream oStringStream;
		oStringStream << "At ";
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
		oStringStream << " occurence of negative water energyslope. (VariablePowerLawFlowResistance)" << std::flush;

		char* tmpChar = new char [(oStringStream.str()).size()+1];
		std::strcpy(tmpChar, (oStringStream.str()).c_str());
		const char *const negativeWaterEnergyslopeErrorMessage = tmpChar;
		throw(negativeWaterEnergyslopeErrorMessage);
	}

	double dimensionlessDischarge = discharge / ( flowWidth * sqrt( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * riverReachProperties.regularRiverReachProperties.waterEnergyslope * pow(dEightyfour, 3.0) ) );
	//The following line represents equation 22 in Rickenmann & Recking 2011
	double flowVelocity = 1.443 * sqrt( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * riverReachProperties.regularRiverReachProperties.waterEnergyslope * dEightyfour ) * pow(dimensionlessDischarge,0.6) * pow( ( 1+pow( (dimensionlessDischarge/43.78) , 0.821) ) , -0.2435 );
	return flowVelocity;
}

std::pair<double,double> VariablePowerLawFlowResistance::calculateFlowDepthAndFlowVelocityUsingDischargeAsInputWithoutPostprocessingChecks(double discharge, const RiverReachProperties& riverReachProperties) const
{
	double flowVelocity = 0.0;
	double flowDepth = 0.0;
	if ( discharge > 0.0 )
	{
		double dEightyfour = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getPercentileGrainDiameter((riverReachProperties.getOverallParameters())->getFractionalGrainDiameters(),84.0);

		if( riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.size() == 1 || (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(1)).getOverallVolume() < 0.0000001 ) //i.e. There is definitely not more than the active layer left.
		{
			double bedrockRoughnessEquivalentRepresentativeGrainDiameterValue;
			const AdditionalRiverReachPropertyType* bedrockRoughnessEquivalentRepresentativeGrainDiameter = riverReachProperties.additionalRiverReachProperties.getAdditionalPropertyConstPointer(CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter);
			if(bedrockRoughnessEquivalentRepresentativeGrainDiameter)
			{
				#if defined _DEBUG || defined DEBUG
				if((bedrockRoughnessEquivalentRepresentativeGrainDiameter->typeSpecificGetter()).first != CombinerVariables::BedrockRoughnessEquivalentRepresentativeGrainDiameter)
				{
					const char *const typeMismatchErrorMessage = "Other than requested type of AdditionalRiverReachPropertyType has been returned.";
					throw(typeMismatchErrorMessage);
				}
				#endif
				bedrockRoughnessEquivalentRepresentativeGrainDiameterValue = ((bedrockRoughnessEquivalentRepresentativeGrainDiameter->typeSpecificGetter()).second).at(0);
			}
			else
			{
				const char *const bedrockRoughnessEquivalentRepresentativeGrainDiameterErrorMessage = "The AdditionalRiverReachPropertyType BedrockRoughnessEquivalentRepresentativeGrainDiameter is needed for VariablePowerLawFlowResistance.";
				throw(bedrockRoughnessEquivalentRepresentativeGrainDiameterErrorMessage);
			}

			if( dEightyfour != dEightyfour ) //i.e. The river is running over bedrock and there are no grains left to define the dEightyfour.
			{
				dEightyfour = bedrockRoughnessEquivalentRepresentativeGrainDiameterValue;
			}
			else //i.e. Transition with beginning bedrock exposure.
			{
				double activeLayerRelativeThickness = (riverReachProperties.regularRiverReachProperties.strataPerUnitBedSurface.at(0)).getOverallVolume() / riverReachProperties.strataSorting->getLayerThickness(riverReachProperties.regularRiverReachProperties);
				if( activeLayerRelativeThickness > 1.0) {activeLayerRelativeThickness = 1.0;}
				dEightyfour = bedrockRoughnessEquivalentRepresentativeGrainDiameterValue + ( activeLayerRelativeThickness * (dEightyfour - bedrockRoughnessEquivalentRepresentativeGrainDiameterValue) );
			}

		}

		ChannelGeometry* tempChannelGeometry = riverReachProperties.geometricalChannelBehaviour->alluviumChannel->createChannelGeometryPointerCopy();
		std::pair< CombinerVariables::TypesOfChannelGeometry, std::vector< std::vector<double> > > channelGeometryGetter = tempChannelGeometry->getInternalParameters();
		if (channelGeometryGetter.first == CombinerVariables::InfinitelyDeepRectangularChannel)
		{
			flowVelocity = this->calculateFlowVelocityUsingDischargeAsInputForGivenDEightyfourAndFlowWidth(discharge, riverReachProperties, dEightyfour, ((channelGeometryGetter.second).at(0)).at(0) );
		}
		else
		{
			//TODO Work this method over
			const char *const errorMessage = "This method is not worked over yet.";
			throw(errorMessage);
			/*
			double inputFlowWidth = startingValueForIteration;
			double tempFlowDepth, tempWaterSurfaceWidth, outputFlowWidth;
			double tempAccuracy = std::numeric_limits<double>::max() - 2.0;
			double previousTempAccuracy =  std::numeric_limits<double>::max() - 1.0;
			while (tempAccuracy > accuracyForTerminatingIteration)
			{
				flowVelocity = calculateFlowVelocityUsingDischargeAsInputForGivenDEightyfourAndFlowWidth(discharge, riverReachProperties,dEightyfour,inputFlowWidth);
				tempFlowDepth = tempChannelGeometry->convertCrossSectionalAreaIntoMaximumFlowDepth( (discharge / flowVelocity) );
				tempChannelGeometry->updateBasicGeometryAccordingToMaximumWaterdepth(tempFlowDepth, riverReachProperties.regularRiverReachProperties);
				tempFlowDepth = tempChannelGeometry->basicGeometry.convertCrossSectionalAreaIntoMaximumFlowDepth( (discharge / flowVelocity) );
				tempWaterSurfaceWidth = tempChannelGeometry->basicGeometry.getChannelWidthAtCertainLevel(tempFlowDepth);

					switch(tempChannelGeometry->basicGeometry.getTypeOfBasicGeometry())
					{
					case BasicGeometry::infinitelyDeepRectangularChannel:
						outputFlowWidth = tempWaterSurfaceWidth;
						break;

					case BasicGeometry::infinitelyDeepVShapedChannel:
						outputFlowWidth = 0.75 * tempWaterSurfaceWidth;
						break;

					default:
						const char *const firstErrorMessage = "Invalid Type of Basic Geometry";
						throw (firstErrorMessage);
					}

				tempAccuracy = fabs( (inputFlowWidth - outputFlowWidth) );

				if(tempAccuracy > (previousTempAccuracy+0.1))
				{
					const char *const secondErrorMessage = "Diverging iteration.";
					throw(secondErrorMessage);
				}

				inputFlowWidth = outputFlowWidth;
				previousTempAccuracy = tempAccuracy;
			}
			 */
		}

		flowDepth = tempChannelGeometry->convertCrossSectionalAreaIntoMaximumFlowDepth( (discharge / flowVelocity) );
		delete tempChannelGeometry;
	}
	std::pair<double,double> result (flowDepth,flowVelocity);

	return result;
}

std::pair<double,double> VariablePowerLawFlowResistance::calculateDischargeAndFlowVelocityUsingFlowDepthAsInput(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	double dEightyfour = getInterpolatedDEightyfour(riverReachProperties);

	double hydraulicRadius = this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties);
	double flowVelocity = sqrt( ( (riverReachProperties.getOverallParameters())->getGravityAcceleration() * hydraulicRadius * riverReachProperties.regularRiverReachProperties.waterEnergyslope ) ) * 16.25 * (hydraulicRadius/dEightyfour) / sqrt( (42.25 + 6.25 * pow( (hydraulicRadius/dEightyfour) , (5.0/3.0) ) ) );
	double discharge = flowVelocity * riverReachProperties.geometricalChannelBehaviour->alluviumChannel->convertMaximumFlowDepthIntoCrossSectionalArea(flowDepth);
	std::pair<double,double> result (discharge,flowVelocity);
	return result;
}

std::pair<double,double> VariablePowerLawFlowResistance::calculateDischargeAndFlowDepthUsingFlowVelocityAsInput(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method VariablePowerLawFlowResistance::calculateDischargeAndFlowDepthUsingFlowVelocityAsInput has not been implemented yet.";
	throw(errorMessage);
	std::pair<double,double> fakeResult = std::make_pair(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN());
	return fakeResult;
}

double VariablePowerLawFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowDepth(double flowDepth, const RiverReachProperties& riverReachProperties) const
{
	double dEightyfour = getInterpolatedDEightyfour(riverReachProperties);

	double hydraulicRadius = this->convertMaximumFlowDepthIntoHydraulicRadius(flowDepth,riverReachProperties);
	//The following line represents equation 10a in Rickenmann & Recking 2011, with a1=6.5 and a2=2.5.
	double sqrtEightOverF = 16.25 * (hydraulicRadius/dEightyfour) / sqrt( (42.25 + 6.25 * pow( (hydraulicRadius/dEightyfour) , (5.0/3.0) ) ) );

	return (8.0 / (sqrtEightOverF * sqrtEightOverF));
}

PowerLawRelation VariablePowerLawFlowResistance::darcyWeisbachFrictionFactorFAsPowerLawFunctionOfFlowDepth(const RiverReachProperties& riverReachProperties) const
{
	return PowerLawRelation(false);
}

double VariablePowerLawFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity(double flowVelocity, const RiverReachProperties& riverReachProperties) const
{
	//TODO Less Important: Implement method
	const char *const errorMessage = "The method VariablePowerLawFlowResistance::returnCurrentDarcyWeisbachFrictionFactorFBasedOnFlowVelocity has not been implemented yet.";
	throw(errorMessage);
	double fakeResult = std::numeric_limits<double>::quiet_NaN();
	return fakeResult;
}

std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > VariablePowerLawFlowResistance::getInternalParameters() const
{
	std::vector<double> empty;
	return std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > (CombinerVariables::VariablePowerLawFlowResistance, empty);
}

void VariablePowerLawFlowResistance::setInternalParameters(const std::pair< CombinerVariables::TypesOfFlowResistance , std::vector<double> > newParameters)
{
	if( newParameters.first != CombinerVariables::VariablePowerLawFlowResistance)
	{
		const char *const errorMessage = "Action on not matching types of flow resistance";
		throw(errorMessage);
	}
}


}
