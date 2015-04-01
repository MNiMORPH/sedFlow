/*
 * RegularRiverReachProperties.cpp
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

#include "RegularRiverReachProperties.h"

#include <vector>
#include <limits>
#include <math.h>
#include "BasicCalculations.h"

namespace SedFlow {
RegularRiverReachProperties::RegularRiverReachProperties(const OverallParameters* overallParameters):
	overallParameters(overallParameters)
{}

RegularRiverReachProperties::RegularRiverReachProperties(const OverallParameters* overallParameters, double elevation, double length, bool sillOccurence, double sillTopEdgeElevation, double discharge, double flowVelocity, double maximumWaterdepth, double bedShearStress, double activeWidth, std::vector<Grains> strataPerUnitBedSurface, double bedslope, double waterEnergyslope, double sedimentEnergyslope, double unreducedSedimentEnergyslope, double waterVolumeChangeRate, double waterVolumeChange, Grains erosionRate, Grains depositionRate, Grains erosion, Grains deposition, Grains erosionPerUnitBedSurface, Grains depositionPerUnitBedSurface):
		overallParameters(overallParameters),
		elevation(elevation),
		length(length),
		sillOccurence(sillOccurence),
		sillTopEdgeElevation(sillTopEdgeElevation),
		discharge(discharge),
		flowVelocity(flowVelocity),
		maximumWaterdepth(maximumWaterdepth),
		bedShearStress(bedShearStress),
		activeWidth(activeWidth),
		strataPerUnitBedSurface(strataPerUnitBedSurface),
		bedslope(bedslope),
		waterEnergyslope(waterEnergyslope),
		sedimentEnergyslope(sedimentEnergyslope),
		unreducedSedimentEnergyslope(unreducedSedimentEnergyslope),
		waterVolumeChangeRate(waterVolumeChangeRate),
		waterVolumeChange(waterVolumeChange)
{
	this->erosionRate = erosionRate;
	this->depositionRate = depositionRate;
	this->erosion = erosion;
	this->deposition = deposition;
	this->erosionPerUnitBedSurface = erosionPerUnitBedSurface;
	this->depositionPerUnitBedSurface = depositionPerUnitBedSurface;
}

RegularRiverReachProperties::~RegularRiverReachProperties()
{}

double RegularRiverReachProperties::alluviumThicknessIncludingPoreVolume()const
{
	double result = 0.0;
	for(std::vector<Grains>::const_iterator currentLayer = strataPerUnitBedSurface.begin(); currentLayer < strataPerUnitBedSurface.end(); ++currentLayer)
	{
		result += currentLayer->getOverallVolume();
	}
	result /= ( 1.0 - overallParameters->getPoreVolumeFraction() );
	return result;
}

double RegularRiverReachProperties::hydraulicHead()const
{
	return ( BasicCalculations::hydraulicHeadBernoulliEquation(elevation,maximumWaterdepth,flowVelocity,(overallParameters->getGravityAcceleration())) );
}

double RegularRiverReachProperties::kineticComponentOfHydraulicHead()const
{
	return ( BasicCalculations::hydraulicHeadBernoulliEquationKineticComponent(flowVelocity,(overallParameters->getGravityAcceleration())) );
}

ConstructionVariables RegularRiverReachProperties::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::RegularRiverReachProperties;
	std::vector<double> doubleVector;
	std::vector<bool> boolVector;
	std::vector<ConstructionVariables> constructionVariablesVector;
	doubleVector.push_back(elevation);
	result.labelledDoubles["elevation"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(length);
	result.labelledDoubles["length"] = doubleVector;
	doubleVector.clear();
	boolVector.push_back(sillOccurence);
	result.labelledBools["sillOccurence"] = boolVector;
	boolVector.clear();
	doubleVector.push_back(sillTopEdgeElevation);
	result.labelledDoubles["sillTopEdgeElevation"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(discharge);
	result.labelledDoubles["discharge"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(flowVelocity);
	result.labelledDoubles["flowVelocity"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(maximumWaterdepth);
	result.labelledDoubles["maximumWaterdepth"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(bedShearStress);
	result.labelledDoubles["bedShearStress"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(activeWidth);
	result.labelledDoubles["activeWidth"] = doubleVector;
	doubleVector.clear();
	constructionVariablesVector.reserve( strataPerUnitBedSurface.size() );
	for(std::vector<Grains>::const_iterator iterator = strataPerUnitBedSurface.begin(); iterator < strataPerUnitBedSurface.end(); ++iterator)
			{ constructionVariablesVector.push_back( (*iterator).createConstructionVariables() ); }
	result.labelledObjects["strataPerUnitBedSurface"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	doubleVector.push_back(bedslope);
	result.labelledDoubles["bedslope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(waterEnergyslope);
	result.labelledDoubles["waterEnergyslope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(sedimentEnergyslope);
	result.labelledDoubles["sedimentEnergyslope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(unreducedSedimentEnergyslope);
	result.labelledDoubles["unreducedSedimentEnergyslope"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(waterVolumeChangeRate);
	result.labelledDoubles["waterVolumeChangeRate"] = doubleVector;
	doubleVector.clear();
	doubleVector.push_back(waterVolumeChange);
	result.labelledDoubles["waterVolumeChange"] = doubleVector;
	constructionVariablesVector.push_back( erosionRate.createConstructionVariables() );
	result.labelledObjects["erosionRate"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( depositionRate.createConstructionVariables() );
	result.labelledObjects["depositionRate"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( erosion.createConstructionVariables() );
	result.labelledObjects["erosion"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( deposition.createConstructionVariables() );
	result.labelledObjects["deposition"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( erosionPerUnitBedSurface.createConstructionVariables() );
	result.labelledObjects["erosionPerUnitBedSurface"] = constructionVariablesVector;
	constructionVariablesVector.clear();
	constructionVariablesVector.push_back( depositionPerUnitBedSurface.createConstructionVariables() );
	result.labelledObjects["depositionPerUnitBedSurface"] = constructionVariablesVector;
	return result;
}

double RegularRiverReachProperties::getDoubleProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties) const
{
	double result;
	const char *const nonDoubleErrorMessage = "Trying to use a non-double RegularRiverReachProperty as double.";
	switch (typeOfRegularRiverReachProperties)
	{
	case CombinerVariables::elevation:
		result = elevation;
		break;

	case CombinerVariables::length:
		result = length;
		break;

	case CombinerVariables::sillOccurence:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::sillTopEdgeElevation:
		result = sillTopEdgeElevation;
		break;

	case CombinerVariables::discharge:
		result = discharge;
		break;

	case CombinerVariables::flowVelocity:
		result = flowVelocity;
		break;

	case CombinerVariables::maximumWaterdepth:
		result = maximumWaterdepth;
		break;

	case CombinerVariables::waterLevel:
		result = waterLevel();
		break;

	case CombinerVariables::hydraulicHead:
		result = hydraulicHead();
		break;

	case CombinerVariables::kineticComponentOfHydraulicHead:
		result = kineticComponentOfHydraulicHead();
		break;

	case CombinerVariables::bedShearStress:
		result = bedShearStress;
		break;

	case CombinerVariables::activeWidth:
		result = activeWidth;
		break;

	case CombinerVariables::strataPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::alluviumThicknessIncludingPoreVolume:
		result = alluviumThicknessIncludingPoreVolume();
		break;

	case CombinerVariables::bedrockElevation:
		result = bedrockElevation();
		break;

	case CombinerVariables::activeLayerPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume:
		result = (strataPerUnitBedSurface[0]).getOverallVolume();
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result = ((strataPerUnitBedSurface[0]).getOverallVolume()) / ( 1.0 / overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter:
		result = (strataPerUnitBedSurface[0]).getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result = (strataPerUnitBedSurface[0]).getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result = (strataPerUnitBedSurface[0]).getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD20:
		result = (strataPerUnitBedSurface[0]).getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD30:
		result = (strataPerUnitBedSurface[0]).getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD50:
		result = (strataPerUnitBedSurface[0]).getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD84:
		result = (strataPerUnitBedSurface[0]).getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD90:
		result = (strataPerUnitBedSurface[0]).getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	case CombinerVariables::bedslope:
		result = bedslope;
		break;

	case CombinerVariables::waterEnergyslope:
		result = waterEnergyslope;
		break;

	case CombinerVariables::sedimentEnergyslope:
		result = sedimentEnergyslope;
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		result = unreducedSedimentEnergyslope;
		break;

	case CombinerVariables::waterVolumeChangeRate:
		result = waterVolumeChangeRate;
		break;

	case CombinerVariables::waterVolumeChange:
		result = waterVolumeChange;
		break;

	case CombinerVariables::erosionRate:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionRateIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionRateOverallVolume:
		result = erosionRate.getOverallVolume();
		break;

	case CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume:
		result = erosionRate.getOverallVolume() / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionRateMedianGrainDiameter:
		result = erosionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::erosionRateArithmeticMeanGrainDiameter:
		result = erosionRate.getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::erosionRateGeometricMeanGrainDiameter:
		result = erosionRate.getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::erosionRateD20:
		result = erosionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::erosionRateD30:
		result = erosionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::erosionRateD50:
		result = erosionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::erosionRateD84:
		result = erosionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::erosionRateD90:
		result = erosionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	case CombinerVariables::depositionRate:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionRateIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionRateOverallVolume:
		result = depositionRate.getOverallVolume();
		break;

	case CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume:
		result = depositionRate.getOverallVolume() / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionRateMedianGrainDiameter:
		result = depositionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::depositionRateArithmeticMeanGrainDiameter:
		result = depositionRate.getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::depositionRateGeometricMeanGrainDiameter:
		result = depositionRate.getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::depositionRateD20:
		result = depositionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::depositionRateD30:
		result = depositionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::depositionRateD50:
		result = depositionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::depositionRateD84:
		result = depositionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::depositionRateD90:
		result = depositionRate.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	case CombinerVariables::erosion:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionOverallVolume:
		result = erosion.getOverallVolume();
		break;

	case CombinerVariables::erosionOverallVolumeIncludingPoreVolume:
		result = erosion.getOverallVolume() / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionMedianGrainDiameter:
		result = erosion.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::erosionArithmeticMeanGrainDiameter:
		result = erosion.getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::erosionGeometricMeanGrainDiameter:
		result = erosion.getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::erosionD20:
		result = erosion.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::erosionD30:
		result = erosion.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::erosionD50:
		result = erosion.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::erosionD84:
		result = erosion.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::erosionD90:
		result = erosion.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	case CombinerVariables::deposition:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionOverallVolume:
		result = deposition.getOverallVolume();
		break;

	case CombinerVariables::depositionOverallVolumeIncludingPoreVolume:
		result = deposition.getOverallVolume() / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionMedianGrainDiameter:
		result = deposition.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::depositionArithmeticMeanGrainDiameter:
		result = deposition.getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::depositionGeometricMeanGrainDiameter:
		result = deposition.getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::depositionD20:
		result = deposition.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::depositionD30:
		result = deposition.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::depositionD50:
		result = deposition.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::depositionD84:
		result = deposition.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::depositionD90:
		result = deposition.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	case CombinerVariables::erosionPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolume:
		result = erosionPerUnitBedSurface.getOverallVolume();
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result = erosionPerUnitBedSurface.getOverallVolume() / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter:
		result = erosionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result = erosionPerUnitBedSurface.getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result = erosionPerUnitBedSurface.getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD20:
		result = erosionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD30:
		result = erosionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD50:
		result = erosionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD84:
		result = erosionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD90:
		result = erosionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	case CombinerVariables::depositionPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolume:
		result = depositionPerUnitBedSurface.getOverallVolume();
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		result = depositionPerUnitBedSurface.getOverallVolume() / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter:
		result = depositionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		result = depositionPerUnitBedSurface.getArithmeticMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		result = depositionPerUnitBedSurface.getGeometricMeanGrainDiameter(overallParameters->getFractionalGrainDiameters());
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD20:
		result = depositionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),20.0);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD30:
		result = depositionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),30.0);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD50:
		result = depositionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),50.0);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD84:
		result = depositionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),84.0);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD90:
		result = depositionPerUnitBedSurface.getPercentileGrainDiameter(overallParameters->getFractionalGrainDiameters(),90.0);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Regular River Reach Property";
		throw (invalidTypeErrorMessage);
	}
	return result;
}

void RegularRiverReachProperties::setDoubleProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties, double newValue)
{
	const char *const nonDoubleErrorMessage = "Trying to use a non-double RegularRiverReachProperty as double.";
	const char *const getterPropertyErrorMessage = "Trying to use a pure getter property (RegularRiverReachProperty) in a setting context.";
	switch (typeOfRegularRiverReachProperties)
	{
	case CombinerVariables::elevation:
		this->elevation = newValue;
		break;

	case CombinerVariables::length:
		this->length = newValue;
		break;

	case CombinerVariables::sillOccurence:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::sillTopEdgeElevation:
		this->sillOccurence = !( (newValue!=newValue) || (fabs(newValue) == std::numeric_limits<double>::infinity()) );
		this->sillTopEdgeElevation = newValue;
		break;

	case CombinerVariables::discharge:
		this->discharge = newValue;
		break;

	case CombinerVariables::flowVelocity:
		this->flowVelocity = newValue;
		break;

	case CombinerVariables::maximumWaterdepth:
		this->maximumWaterdepth = newValue;
		break;

	case CombinerVariables::waterLevel:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::hydraulicHead:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::kineticComponentOfHydraulicHead:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::bedShearStress:
		this->bedShearStress = newValue;
		break;

	case CombinerVariables::activeWidth:
		this->activeWidth = newValue;
		break;

	case CombinerVariables::strataPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::alluviumThicknessIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::bedrockElevation:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::bedslope:
		this->bedslope = newValue;
		break;

	case CombinerVariables::waterEnergyslope:
		this->waterEnergyslope = newValue;
		break;

	case CombinerVariables::sedimentEnergyslope:
		this->sedimentEnergyslope = newValue;
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		this->unreducedSedimentEnergyslope = newValue;
		break;

	case CombinerVariables::waterVolumeChangeRate:
		this->waterVolumeChangeRate = newValue;
		break;

	case CombinerVariables::waterVolumeChange:
		this->waterVolumeChange = newValue;
		break;

	case CombinerVariables::erosionRate:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionRateIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionRateOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRate:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionRateIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionRateOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosion:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::deposition:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurface:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume:
		throw(nonDoubleErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD90:
		throw(getterPropertyErrorMessage);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Regular River Reach Property";
		throw (invalidTypeErrorMessage);
	}
}

Grains RegularRiverReachProperties::getGrainsProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties) const
{
	Grains result;
	const char *const nonGrainsErrorMessage = "Trying to use a non-Grains RegularRiverReachProperty as Grains.";
	const char *const vectorInsteadOfIndividualErrorMessage = "Trying to use a Grains vector as Grains.";
	switch (typeOfRegularRiverReachProperties)
	{
	case CombinerVariables::elevation:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::length:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::sillOccurence:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::sillTopEdgeElevation:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::discharge:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::flowVelocity:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::maximumWaterdepth:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterLevel:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::hydraulicHead:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::kineticComponentOfHydraulicHead:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::bedShearStress:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeWidth:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::strataPerUnitBedSurface:
		throw(vectorInsteadOfIndividualErrorMessage);
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		throw(vectorInsteadOfIndividualErrorMessage);
		break;

	case CombinerVariables::alluviumThicknessIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::bedrockElevation:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurface:
		result = strataPerUnitBedSurface[0];
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume:
		result = strataPerUnitBedSurface[0] / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD90:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::bedslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterEnergyslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::sedimentEnergyslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterVolumeChangeRate:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterVolumeChange:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRate:
		result = erosionRate;
		break;

	case CombinerVariables::erosionRateIncludingPoreVolume:
		result = erosionRate / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionRateOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRateD90:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRate:
		result = depositionRate;
		break;

	case CombinerVariables::depositionRateIncludingPoreVolume:
		result = depositionRate / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionRateOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionRateD90:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosion:
		result = erosion;
		break;

	case CombinerVariables::erosionIncludingPoreVolume:
		result = erosion / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionD90:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::deposition:
		result = deposition;
		break;

	case CombinerVariables::depositionIncludingPoreVolume:
		result = deposition / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionD90:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurface:
		result = erosionPerUnitBedSurface;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume:
		result = erosionPerUnitBedSurface / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD90:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurface:
		result = depositionPerUnitBedSurface;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume:
		result = depositionPerUnitBedSurface / ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD20:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD30:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD50:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD84:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD90:
		throw(nonGrainsErrorMessage);
		break;

	default:
		const char *const errorMessage = "Invalid Regular River Reach Property";
		throw (errorMessage);
	}
	return result;
}

void RegularRiverReachProperties::setGrainsProperty (CombinerVariables::TypesOfRegularRiverReachProperties typeOfRegularRiverReachProperties, Grains newValue)
{
	const char *const nonGrainsErrorMessage = "Trying to use a non-Grains RegularRiverReachProperty as Grains.";
	const char *const vectorInsteadOfIndividualErrorMessage = "Trying to use a Grains vector as Grains.";
	const char *const getterPropertyErrorMessage = "Trying to use a pure getter property (RegularRiverReachProperty) in a setting context.";
	switch (typeOfRegularRiverReachProperties)
	{
	case CombinerVariables::elevation:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::length:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::sillOccurence:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::sillTopEdgeElevation:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::discharge:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::flowVelocity:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::maximumWaterdepth:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterLevel:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::hydraulicHead:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::kineticComponentOfHydraulicHead:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::bedShearStress:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeWidth:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::strataPerUnitBedSurface:
		throw(vectorInsteadOfIndividualErrorMessage);
		break;

	case CombinerVariables::strataPerUnitBedSurfaceIncludingPoreVolume:
		throw(vectorInsteadOfIndividualErrorMessage);
		break;

	case CombinerVariables::alluviumThicknessIncludingPoreVolume:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::bedrockElevation:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurface:
		this->strataPerUnitBedSurface[0] = newValue;
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceIncludingPoreVolume:
		this->strataPerUnitBedSurface[0] = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::activeLayerPerUnitBedSurfaceD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::bedslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterEnergyslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::sedimentEnergyslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::unreducedSedimentEnergyslope:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterVolumeChangeRate:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::waterVolumeChange:
		throw(nonGrainsErrorMessage);
		break;

	case CombinerVariables::erosionRate:
		this->erosionRate = newValue;
		break;

	case CombinerVariables::erosionRateIncludingPoreVolume:
		this->erosionRate = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionRateOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionRateD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRate:
		this->depositionRate = newValue;
		break;

	case CombinerVariables::depositionRateIncludingPoreVolume:
		this->depositionRate = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionRateOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionRateD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosion:
		this->erosion = newValue;
		break;

	case CombinerVariables::erosionIncludingPoreVolume:
		this->erosion = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::deposition:
		this->deposition = newValue;
		break;

	case CombinerVariables::depositionIncludingPoreVolume:
		this->deposition = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurface:
		this->erosionPerUnitBedSurface = newValue;
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceIncludingPoreVolume:
		this->erosionPerUnitBedSurface = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::erosionPerUnitBedSurfaceD90:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurface:
		this->depositionPerUnitBedSurface = newValue;
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceIncludingPoreVolume:
		this->depositionPerUnitBedSurface = newValue * ( 1.0 - overallParameters->getPoreVolumeFraction() );
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceOverallVolumeIncludingPoreVolume:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceMedianGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceArithmeticMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceGeometricMeanGrainDiameter:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD20:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD30:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD50:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD84:
		throw(getterPropertyErrorMessage);
		break;

	case CombinerVariables::depositionPerUnitBedSurfaceD90:
		throw(getterPropertyErrorMessage);
		break;

	default:
		const char *const invalidTypeErrorMessage = "Invalid Regular River Reach Property";
		throw (invalidTypeErrorMessage);
	}
}

std::vector<double> RegularRiverReachProperties::getActiveOverallFractionalAbundance() const
{
	return strataPerUnitBedSurface.at(0).getOverallFractionalAbundance();
}

}
