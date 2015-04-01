/*
 * InputOutputTesting.cpp
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

#include "SedFlowCore.h"
#include "OutputMethods.h"
#include "ConsoleTools.h"

int main (int argc, char* argv[])
{
	const char* inputXMLfile;
	try{

		if (argc != 2)
		{
			const char *const errorMessage = "InputOutputTesting must be called with exactly one input variable, which is the input XML file name.";
			throw(errorMessage);
		}
		inputXMLfile = argv[1];

	} catch (const char *const msg) {
		std::cerr << msg << std::endl;
		std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
		ConsoleTools::wait();
		exit(1);
	} catch (...) {
	std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
	ConsoleTools::wait();
	exit(1); }

	try{

		std::cout << std::endl << "Processing..." << std::endl;
		SedFlow::SedFlow sedFlow = SedFlow::SedFlow::initialise(inputXMLfile);
		const SedFlow::OutputMethods* outputMethods = sedFlow.getConstantOutputMethodsPointer();
		outputMethods->forcedWriteOutputLine();
		outputMethods->finaliseOutput();

	} catch (...) {
	std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
	ConsoleTools::wait();
	exit(1); }

	std::cout << std::endl << "Finished successfully..." << std::endl;
	ConsoleTools::wait();
	return 0;
}
