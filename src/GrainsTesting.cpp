/*
 * GrainsTesting.cpp
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
#include <vector>

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

int main (int argc, char* argv[])
{

double tempDouble;
std::vector<double> tempDoubleVector;

std::cout << "###########################################" << std::endl;
std::cout << "#############Grains Testing################" << std::endl;
std::cout << "###########################################" << std::endl << std::endl;



std::cout << "############Construction###################" << std::endl << std::endl;

std::vector<double> normalGrainsAbundances;
for(double i = 1.0; i < 4.5; ++i) {normalGrainsAbundances.push_back(i);}
std::cout << "normalGrainsAbundances:";
for(std::vector<double>::const_iterator currentAbundance = normalGrainsAbundances.begin(); currentAbundance < normalGrainsAbundances.end(); ++currentAbundance) {std::cout << " \t " << *currentAbundance;}
std::vector<double> tracerGrainsAbundances (4,1.0);
std::cout << ";" << std::endl << "tracerGrainsAbundances:";
for(std::vector<double>::const_iterator currentAbundance = tracerGrainsAbundances.begin(); currentAbundance < tracerGrainsAbundances.end(); ++currentAbundance) {std::cout << " \t " << *currentAbundance;}
std::vector<double> fishEggsAbundances (3,0.0); fishEggsAbundances.insert(fishEggsAbundances.begin()+2,0.1);
std::cout << ";" << std::endl << "fishEggsAbundances:";
for(std::vector<double>::const_iterator currentAbundance = fishEggsAbundances.begin(); currentAbundance < fishEggsAbundances.end(); ++currentAbundance) {std::cout << " \t " << *currentAbundance;}
int fishEggsInsertionLayer = 2;
std::cout << ";" << std::endl << "fishEggsInsertionLayer: " << fishEggsInsertionLayer << ";" << std::endl << std::endl;

std::vector<GrainType*> grainTypePointers;
grainTypePointers.push_back(new NormalGrains(normalGrainsAbundances));
grainTypePointers.push_back(new TracerGrains(tracerGrainsAbundances));
grainTypePointers.push_back(new FishEggs(fishEggsAbundances,fishEggsInsertionLayer));

Grains grains = Grains(grainTypePointers);
std::cout << "grains";
printGrains(grains);

Grains copyOfGrains = Grains(grains);
std::cout << std::endl << "copyOfGrains";
printGrains(copyOfGrains);

std::cout << std::endl << std::endl << "############Get overall abundances, cumulative abundances and volume##################" << std::endl << std::endl;

tempDoubleVector = grains.getOverallFractionalAbundance();
std::cout << "grains.getOverallFractionalAbundance():"<< std::endl;
printDoubleVector(tempDoubleVector);
std::cout << std::endl << std::endl;
tempDoubleVector.clear();

tempDoubleVector = grains.getOverallCumulativeFractionalAbundance();
std::cout << "grains.getOverallCumulativeFractionalAbundance():" << std::endl;
printDoubleVector(tempDoubleVector);
std::cout << std::endl << std::endl;
tempDoubleVector.clear();

tempDouble = grains.getOverallVolume();
std::cout << "grains.getOverallVolume():" << tempDouble << ";" << std::endl << std::endl;

std::cout << "TODO";
//To Do: Test methods

std::cout << std::endl << std::endl << "############Checking for Zeros##################" << std::endl << std::endl;

std::cout << "TODO";
//To Do: Test methods


std::cout << std::endl << std::endl << "############Insertion Erosion Deposition##################" << std::endl << std::endl;

std::cout << "TODO";
//To Do: Test methods


std::cout << std::endl << std::endl << "############Checking for matching grain types##################" << std::endl << std::endl;

std::cout << "TODO";
//To Do: Test methods

std::cout << std::endl << std::endl << "#####################Sorting strata############################" << std::endl << std::endl;

std::cout << "TODO";
//TO DO Test method Grains::sortStrata(std::vector<Grains>& strataPerUnitBedSurface, double layerVolume, double maximumLayerVolumePerUnitLayerVolume, double minimumLayerVolumePerUnitLayerVolume)


ConsoleTools::inputWait();
return 0;
}
