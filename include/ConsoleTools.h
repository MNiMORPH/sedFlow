/*
 * ConsoleTools.h
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

#ifndef CONSOLETOOLS_H_
#define CONSOLETOOLS_H_

#include <iostream>
#include <string>

class ConsoleTools {
public:
	static void inputWait ()
	{
		std::cout << std::endl << "----------------------" << std::endl << "Enter anything to continue..." << std::endl;
		// Clears any possible error states, which might prevent the read in
		std::cin.clear();
		// Ignores as many chars as are available in buffer (i.e. ignores complete buffer)
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		//Finally read in a place holder.
		std::string placeHolder;
		std::cin >> placeHolder;
	}

	static void wait ()
	{
		std::cout << std::endl << "----------------------" << std::endl << "Hit Enter to continue..." << std::endl;
		// Clears any possible error states, which might prevent the read in
		std::cin.clear();
		// Ignores as many chars as are available in buffer (i.e. ignores complete buffer)
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		//Finally ask for the input
		std::cin.get();
	}
};

#endif /* CONSOLETOOLS_H_ */
