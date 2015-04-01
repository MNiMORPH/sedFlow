/*
 * MarginTesting.cpp
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

void printCellIDsAndMarginInformation(const RiverSystemProperties& riverSystemProperties);

int main (int argc, char* argv[])
{
	try{
		//////////////////////////////////////////////////////
		///////////// Input Section //////////////////////////
		//////////////////////////////////////////////////////

		std::vector<int> cellIDs;
		// cellIDs.push_back(???); //Initialise

		std::vector<int> downstreamCellIDs;
		//downstreamCellIDs.push_back(???); //Initialise

		//////////////////////////////////////////////////////
		///////////// Construction Section ///////////////////
		//////////////////////////////////////////////////////

		try
		{
			if( cellIDs.size() != downstreamCellIDs.size() )
			{
				const char *const errorMessage = "cellIDs and downstreamCellIDs must have the same length.";
				throw(errorMessage);
			}
		} catch (const char *const msg) {
			std::cerr << msg << std::endl;
			throw;
		}



		//////////////////////////////////////////////////////
		//////////////// Testing Section /////////////////////
		//////////////////////////////////////////////////////



	} catch (...) {
	std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
	ConsoleTools::wait();
	exit(1); }

	ConsoleTools::wait();
	return 0;
}
