/*
 * sedFlow.cpp
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
#include "ConsoleTools.h"
#include <stdexcept>
#include <cstring>
#include <string.h>

#define myStringify(s) myStringifySubsidiary(s)
#define myStringifySubsidiary(s) #s

int main (int argc, char* argv[])
{
	std::cout << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << "~               sedFlow                ~" << std::endl;
	std::cout << "~                                      ~" << std::endl;
	std::cout << "~ Bedload dynamics in mountain streams ~" << std::endl;
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	std::cout << std::endl;
	#if defined SEDFLOWVERSION
	std::cout << "Version " << myStringify(SEDFLOWVERSION) << std::endl;
	#endif
	std::cout << "Copyright (C) 2014 Swiss Federal Research Institute WSL (http://www.wsl.ch)" << std::endl;
	std::cout << "Developed by F.U.M. Heimann" << std::endl;
	std::cout << "Published by the Swiss Federal Research Institute WSL" << std::endl;
	std::cout << std::endl;
	std::cout << "This software is based on pugixml library (http://pugixml.org)." << std::endl;
	std::cout << "pugixml is Copyright (C) 2006-2012 Arseny Kapoulkine." << std::endl;
	#if defined __GNUC__
	std::cout << std::endl;
	std::cout << "This software is further based on libgcc and libstdc++ libraries." << std::endl;
	std::cout << std::endl;
	std::cout << "This program has been compiled with ";
	#if defined __MINGW32__
	std::cout << "MinGW ";
	#endif
	std::cout << "G++ version " << __GNUC__ << "." << __GNUC_MINOR__;
	#if defined __GNUC_PATCHLEVEL__
	std::cout << "." << __GNUC_PATCHLEVEL__;
	#endif
	std::cout << " ." << std::endl;
	#endif
	std::cout << std::endl;
	std::cout << "This program should be applied only by users who are experienced in the assessment of mountain stream bedload dynamics. The simulation results may differ substantially from sediment transport volumes in nature." << std::endl;
	std::cout << std::endl;
	std::cout << "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3 as published by the Free Software Foundation." << std::endl;
	std::cout << std::endl;
	std::cout << "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details." << std::endl;
	std::cout << std::endl;
	std::cout << "You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses" << std::endl;
	std::cout << "For details on sedFlow see http://www.wsl.ch/sedFlow" << std::endl;
	std::cout << std::endl;

	bool licenseAlreadyAccepted = ((argc > 2) && ((std::strcmp(argv[2],"acceptLicense") == 0) || (std::strcmp(argv[2],"AcceptLicense") == 0)));
	if(!licenseAlreadyAccepted)
	{
		std::cout << "By continuing the user accepts the license terms mentioned above." << std::endl;
		ConsoleTools::wait();
		std::cout << std::endl;
	}

	std::cout << "The license terms have been accepted by the user." << std::endl;
	std::cout << std::endl;

	char* inputXMLfile;
	bool newHasBeenCalledForInputXMLfile = false;
	if (argc > 1)
	{
		inputXMLfile = argv[1];
	}
	else
	{
		std::cout << "Please enter an input file including its path:" << std::endl;
		// Clears any possible error states, which might prevent the read in
		std::cin.clear();
		// Ignores as many chars as are available in buffer (i.e. ignores complete buffer)
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		//Finally read in the file name.
		std::string fileNameAsString;
		std::getline(std::cin,fileNameAsString);
		inputXMLfile = new char [fileNameAsString.size()+1];
		newHasBeenCalledForInputXMLfile = true;
		std::strcpy(inputXMLfile, fileNameAsString.c_str());
	}

	int i;
	try{

		std::cout << std::endl << "Processing..." << std::endl << std::endl;
		i = SedFlow::SedFlowCore::runSimulation(inputXMLfile);
		if(newHasBeenCalledForInputXMLfile) { delete[] inputXMLfile; }

	} catch (std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << std::endl;
		std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
		if(!licenseAlreadyAccepted) { ConsoleTools::wait(); }
		exit(1);
	} catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
		if(!licenseAlreadyAccepted) { ConsoleTools::wait(); }
		exit(1);
	} catch (...) {
		std::cerr << std::endl << std::endl << "This is an error exit." << std::endl;
		if(!licenseAlreadyAccepted) { ConsoleTools::wait(); }
		exit(1);
	}

	std::cout << "Finished successfully..." << std::endl;
	if(!licenseAlreadyAccepted) { ConsoleTools::wait(); }
	return i;
}
