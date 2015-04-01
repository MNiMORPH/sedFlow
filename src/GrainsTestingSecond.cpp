/*
 * GrainsTestingSecond.cpp
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

#include <iostream>

#include "SedFlowHeaders.h"

using namespace SedFlow;

void printDoubleVector(std::vector<double> vector)
{
	for(std::vector<double>::const_iterator currentValue = vector.begin(); currentValue < vector.end(); ++ currentValue)
	{
		std::cout << " \t " << *currentValue;
	}
	std::cout << ";" << std::endl;
}

void printGrains(const Grains& grains)
{
	std::vector<double> doubleVector;

	std::cout << std::endl << "==========================" << std::endl << std::endl << "Start Abundances ---------" << std::endl;

	for(std::vector<GrainType*>::const_iterator currentGrainType = grains.getBeginGrainTypeConstIterator(); currentGrainType < grains.getEndGrainTypeConstIterator(); ++currentGrainType)
	{
		std::cout << (*(*currentGrainType)).getTypeOfGrainsAsString();
		doubleVector = (*(*currentGrainType)).getFractions();
		printDoubleVector(doubleVector);
		doubleVector.clear();

	}

	std::cout << "--------- End Abundances" << std::endl << std::endl << "Start Additional Parameters ---------" << std::endl;

	for(std::vector<GrainType*>::const_iterator currentGrainType = grains.getBeginGrainTypeConstIterator(); currentGrainType < grains.getEndGrainTypeConstIterator(); ++currentGrainType)
	{
		if ( (*(*currentGrainType)).hasTypeSpecificGetter())
		{
			std::cout << CombinerVariables::typeOfGrainsToString( (*(*currentGrainType)).typeSpecificGetter().first );
			doubleVector = (*(*currentGrainType)).typeSpecificGetter().second;
			printDoubleVector(doubleVector);
			doubleVector.clear();
		}
	}

	std::cout << "--------- End Additional Parameters" << std::endl << std::endl << "==========================" << std::endl << std::endl;
}

void printStrata(std::vector<Grains> strata)
{
	std::cout << std::endl << ":::::::::::::::::::::::::::::::::::::::::::" << std::endl << std::endl;
	for(int i = 0; i < strata.size(); ++i)
	{
		std::cout << "Layer " << i << ":" << std::endl;
		printGrains(strata.at(i));
	}
	std::cout << std::endl << ":::::::::::::::::::::::::::::::::::::::::::" << std::endl << std::endl << std::endl;
}


int main (int argc, char* argv[])
{
	//////////////////////////////////////////////////////
	///////////// Input Section //////////////////////////
	//////////////////////////////////////////////////////

	// Input of Grain Diameters
	std::vector<double> fractionalGrainDiameters;
	fractionalGrainDiameters.push_back(0.05); //Initialise

	// Input of Strata

	int numberOfLayers=3; //Initialise

	double maximumLayerVolumePerUnitLayerVolume=2.0; //Initialise
	double minimumLayerVolumePerUnitLayerVolume=0.5; //Initialise

	std::vector<double> strataNormalGrainsFractionalAbundances;
	strataNormalGrainsFractionalAbundances.push_back(1.0); //Initialise

	std::vector<double> strataTracerGrainsFractionalAbundances;
	strataTracerGrainsFractionalAbundances.push_back(1.0); //Initialise

	std::vector<double> strataFishEggsFractionalAbundances;
	strataFishEggsFractionalAbundances.push_back(0.1); //Initialise

	int strataFishEggsInsertionLayer=0; //Initialise

	//Input of Update Grains

	std::vector<double> updateGrainsNormalGrainsFractionalAbundances;
	updateGrainsNormalGrainsFractionalAbundances.push_back(2.0); //Initialise

	std::vector<double> updateGrainsTracerGrainsFractionalAbundances;
	updateGrainsTracerGrainsFractionalAbundances.push_back(0.7); //Initialise

	std::vector<double> updateGrainsFishEggsFractionalAbundances;
	updateGrainsFishEggsFractionalAbundances.push_back(0.2); //Initialise

	int updateGrainsFishEggsInsertionLayer=0; //Initialise


	//////////////////////////////////////////////////////
	///////////// Construction Section ///////////////////
	//////////////////////////////////////////////////////

	// Construction of Strata

	NormalGrains strataNormalGrains = NormalGrains(strataNormalGrainsFractionalAbundances);
	TracerGrains strataTracerGrains = TracerGrains(strataTracerGrainsFractionalAbundances);
	FishEggs strataFishEggs = FishEggs(strataFishEggsFractionalAbundances, strataFishEggsInsertionLayer);

	std::vector<GrainType*> strataLayerSingleGrainTypes;
	strataLayerSingleGrainTypes.reserve(3);
	strataLayerSingleGrainTypes.push_back(&strataNormalGrains);
	strataLayerSingleGrainTypes.push_back(&strataTracerGrains);
	strataLayerSingleGrainTypes.push_back(&strataFishEggs);

	Grains strataLayer = Grains(strataLayerSingleGrainTypes);

	double layerVolume = strataLayer.getOverallVolume();

	std::vector<Grains> strata (numberOfLayers, strataLayer);

	// Construction of Update Grains

	NormalGrains updateGrainsNormalGrains = NormalGrains(updateGrainsNormalGrainsFractionalAbundances);
	TracerGrains updateGrainsTracerGrains = TracerGrains(updateGrainsTracerGrainsFractionalAbundances);
	FishEggs updateGrainsFishEggs = FishEggs(updateGrainsFishEggsFractionalAbundances, updateGrainsFishEggsInsertionLayer);

	std::vector<GrainType*> updateGrainsSingleGrainTypes;
	updateGrainsSingleGrainTypes.reserve(3);
	updateGrainsSingleGrainTypes.push_back(&updateGrainsTracerGrains); //Changed Order for checking model behaviour
	updateGrainsSingleGrainTypes.push_back(&updateGrainsFishEggs);
	updateGrainsSingleGrainTypes.push_back(&updateGrainsNormalGrains);

	Grains updateGrains = Grains(updateGrainsSingleGrainTypes);


	//////////////////////////////////////////////////////
	//////////////// Testing Section /////////////////////
	//////////////////////////////////////////////////////
	
	//Test copy constructor
	
	//...

	// Test assignment

	Grains copyOfUpdateGrains = updateGrains;
			//The following two lines are for checking deep vs. flat copy
	copyOfUpdateGrains.setSingleFraction(CombinerVariables::TracerGrains, 0, 2.0);
	double abundanceOfSecondTracerGrainsFractionInCopyOfUpdateGrains = copyOfUpdateGrains.getSingleFraction(CombinerVariables::TracerGrains,0);
	double abundanceOfSecondTracerGrainsFractionInUpdateGrains = updateGrains.getSingleFraction(CombinerVariables::TracerGrains,0);


	// Test multiplication and division operator

	double multiplicatorOrDivisor=1.1; //Initialise
	Grains multipliedUpdateGrains = (updateGrains * multiplicatorOrDivisor);
	Grains dividedUpdateGrains = (updateGrains / multiplicatorOrDivisor);

	std::vector<double> multiplicatorsOrDivisorsForEachFraction;
	multiplicatorsOrDivisorsForEachFraction.push_back(2.0); //Initialise
	Grains individuallyMultipliedUpdateGrains = (multiplicatorsOrDivisorsForEachFraction * updateGrains); // Switched order for checking commutative property
	Grains individuallyDividedUpdateGrains = (updateGrains / multiplicatorsOrDivisorsForEachFraction);


	// Test addition operator

	Grains addedUpdateGrains = (updateGrains + dividedUpdateGrains);


	// Test insertion

	updateGrains.insert(strata);

	// Test deposition

	updateGrains.deposit(strata);

	// Test potential erosion and erosion

	Grains potentialErosion = updateGrains.getPotentialErosion(strata);

	Grains actualErosion = updateGrains.erode(strata);

	// Test modifications in the middle of the strata, in order to have marker for sortStrata

	//(strata.at(...)).add(updateGrains);
	//Grains potentialSubtraction = (strata.at(...)).getPotentialSubtraction(updateGrains);
	//Grains actualSubtraction = (strata.at(...)).subtract(updateGrains);

	// Test mean and percentile Grain Diameter

	double meanGrainDiameter = (strata.at(0)).getArithmeticMeanGrainDiameter(fractionalGrainDiameters);

	double percentileRank=0.2; //Initialise
	double percentileGrainDiameter = (strata.at(0)).getPercentileGrainDiameter(fractionalGrainDiameters,percentileRank);

	// Test overall volume within diameter range

	double firstDiameterLimit=0.07; //Initialise
	double secondDiameterLimit=0.03; //Initialise
	double overallVolumeWithinDiameterRange = (strata.at(0)).getOverallVolumeWithinDiameterRange(fractionalGrainDiameters,firstDiameterLimit,secondDiameterLimit);

	// Test sort strata

	Grains::sortStrata(strata, layerVolume, maximumLayerVolumePerUnitLayerVolume, minimumLayerVolumePerUnitLayerVolume);

	return 0;
}
