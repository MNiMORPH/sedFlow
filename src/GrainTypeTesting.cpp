/*
 * GrainTypeTesting.cpp
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

#include "ConsoleTools.h"

#include "SedFlowHeaders.h"

using namespace SedFlow;

int main (int argc, char* argv[])
{
std::cout << "###########################################" << std::endl;
std::cout << "############GrainType Testing##############" << std::endl;
std::cout << "###########################################" << std::endl << std::endl;

std::cout << "############Construction###################" << std::endl;
	double myInitialFractionalAbundancesValues[] = {1.5,2.5,3.6,4.7};
	std::vector<double> myInitialFractionalAbundances;
	for(int i = 0; i < (sizeof(myInitialFractionalAbundancesValues)/sizeof(myInitialFractionalAbundancesValues[0])); ++i) {myInitialFractionalAbundances.push_back(myInitialFractionalAbundancesValues[i]);}

	std::cout << "Number of input abundances: " << (sizeof(myInitialFractionalAbundancesValues)/sizeof(myInitialFractionalAbundancesValues[0])) << std::endl;
	std::cout << "Input abundances:";
	for(int i=0; i<sizeof(myInitialFractionalAbundancesValues)/sizeof(myInitialFractionalAbundancesValues[0]); ++i){std::cout << " " <<  myInitialFractionalAbundancesValues[i];}
	std::cout << std::endl << std::endl ;

	GrainType* myGrains = new NormalGrains(myInitialFractionalAbundances);

	std::cout << "numberOfFractions: "<< myGrains->getNumberOfFractions() << std::endl;
	std::cout << "fractionalAbundance:";
	for(int i = 0; i < myGrains->getNumberOfFractions(); ++i) {std::cout << " " << myGrains->getFraction(i);}
	std::cout << std::endl;

std::cout << std::endl << std::endl << "############Checking for Zeros##################" << std::endl << std::endl;

	std::cout << "fractionalAbundance:";
	for(int i = 0; i < myGrains->getNumberOfFractions(); ++i) {std::cout << " " << myGrains->getFraction(i);}
	std::cout << " areFractionsZero: " << myGrains->areFractionsZero() << std::endl;

	GrainType* myTempZeros = new NormalGrains(myInitialFractionalAbundances);
	myTempZeros->zeroFractions();
	myTempZeros->setFraction((myTempZeros->getNumberOfFractions()- 1), 2.5);
	std::cout << "fractionalAbundance:";
	for(int i = 0; i < myTempZeros->getNumberOfFractions(); ++i) {std::cout << " " << myTempZeros->getFraction(i);}
	std::cout << " areFractionsZero: " << myTempZeros->areFractionsZero() << std::endl;

	myTempZeros->zeroFractions();
	std::cout << "fractionalAbundance:";
	for(int i = 0; i < myTempZeros->getNumberOfFractions(); ++i) {std::cout << " " << myTempZeros->getFraction(i);}
	std::cout << " areFractionsZero: " << myTempZeros->areFractionsZero() << std::endl;

std::cout << std::endl << std::endl << "############Insertion Erosion Deposition##################" << std::endl << std::endl;

	//Create sediment Column and print it
	std::vector<GrainType*> sedimentColumn;
	for(int i = 0; i < 4; ++i) {sedimentColumn.push_back(myGrains->createGrainTypePointerCopy());}
	std::cout << "Sediment Column:" << std::endl;
	for(std::vector<GrainType*>::iterator i = sedimentColumn.begin(); i < sedimentColumn.end(); ++i)
	{
		for(int j = 0; j < (*(*i)).getNumberOfFractions(); j++)
		{
			std::cout << " " << (*(*i)).getFraction(j);
		}
		std::cout << std::endl;
	}

	//Create sediment to be inserted and print it
	std::vector<double> myZeros(4);
	GrainType* mobileGrains = new NormalGrains(myZeros);
	mobileGrains->setFraction(2, 1.0);
	std::cout << std::endl << "Inserted:";
	for(int i = 0; i < mobileGrains->getNumberOfFractions(); ++i) {std::cout << " " << mobileGrains->getFraction(i);}
	std::cout << std::endl << std::endl;

	mobileGrains->insert(sedimentColumn);

	//Print new sediment column
	std::cout << "Sediment Column:" << std::endl;
	for(std::vector<GrainType*>::iterator i = sedimentColumn.begin(); i < sedimentColumn.end(); ++i)
	{
		for(int j = 0; j < (*(*i)).getNumberOfFractions(); j++)
		{
			std::cout << " " << (*(*i)).getFraction(j);
		}
		std::cout << std::endl;
	}

	//Adjust sediment to be eroded and print it
	mobileGrains->zeroFractions();
	mobileGrains->setFraction(1, (2 * (*(sedimentColumn.at(0))).getFraction(1)));
	std::cout << std::endl << "Eroded:";
	for(int i = 0; i < mobileGrains->getNumberOfFractions(); ++i) {std::cout << " " << mobileGrains->getFraction(i);}
	std::cout << std::endl << "Eroded Grain Type: " << mobileGrains->getTypeOfGrains();
	std::cout << std::endl << std::endl;

	GrainType* erodedGrains = mobileGrains->erode(sedimentColumn);

	//Print new sediment column
	std::cout << "Sediment Column:" << std::endl;
	for(std::vector<GrainType*>::iterator i = sedimentColumn.begin(); i < sedimentColumn.end(); ++i)
	{
		for(int j = 0; j < (*(*i)).getNumberOfFractions(); j++)
		{
			std::cout << " " << (*(*i)).getFraction(j);
		}
		std::cout << std::endl;
	}

	//Print actually eroded sediment
	std::cout << std::endl << "Actually eroded:";
	for(int i = 0; i < (*erodedGrains).getNumberOfFractions(); ++i) {std::cout << " " << (*erodedGrains).getFraction(i);}
	std::cout << std::endl << "Actually eroded Grain Type: " << (*erodedGrains).getTypeOfGrains();
	std::cout << std::endl << std::endl;

	//Adjust sediment to be eroded and print it
	mobileGrains->zeroFractions();
	mobileGrains->setFraction(0, 4.0);
	std::cout << std::endl << "Deposited:";
	for(int i = 0; i < mobileGrains->getNumberOfFractions(); ++i) {std::cout << " " << mobileGrains->getFraction(i);}
	std::cout << std::endl << std::endl;

	mobileGrains->deposit(sedimentColumn);

	//Print new sediment column
	std::cout << "Sediment Column:" << std::endl;
	for(std::vector<GrainType*>::iterator i = sedimentColumn.begin(); i < sedimentColumn.end(); ++i)
	{
		for(int j = 0; j < (*(*i)).getNumberOfFractions(); j++)
		{
			std::cout << " " << (*(*i)).getFraction(j);
		}
		std::cout << std::endl;
	}

std::cout << std::endl << std::endl << "############Checking for matching grain types##################" << std::endl << std::endl;

	std::cout << "TODO";
	//To Do: Test methods

std::cout << std::endl << std::endl << "############Fish Eggs##################" << std::endl << std::endl;

	std::cout << "TODO";
	//To Do: Test methods
	//Insertion Erosion Deposition

delete myGrains;
delete myTempZeros;
delete mobileGrains;

ConsoleTools::inputWait();
return 0;
}
