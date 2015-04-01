/*
 * OutputMethods.h
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

#ifndef OUTPUTMETHODS_H_
#define OUTPUTMETHODS_H_

#include "OutputMethodType.h"
#include "RiverSystemProperties.h"
#include "OverallParameters.h"
#include "ConstructionVariables.h"

namespace SedFlow {

class OutputMethods {
private:
	std::vector<OutputMethodType*> constitutingOutputMethodTypes;

	static std::vector<OutputMethodType*> forConstructorsCheckAndCopySingleOutputMethodTypes (const std::vector<OutputMethodType*>& singleOutputMethodTypes);

public:
	OutputMethods(){}
	OutputMethods(const std::vector<OutputMethodType*>& singleOutputMethodTypes);
	OutputMethods(const OutputMethods& toCopy);
	virtual ~OutputMethods();

	ConstructionVariables createConstructionVariables()const;

	void initialiseOutput()const;
	void update();
	void writeOutputLineIfScheduled();
	void forcedWriteOutputLine()const;
	void finaliseOutput()const;

	OutputMethods& operator = (const OutputMethods& newOutputMethods)
	{
		if (this != &newOutputMethods) {
			while(!(this->constitutingOutputMethodTypes.empty()))
			{
				delete this->constitutingOutputMethodTypes.back();
				this->constitutingOutputMethodTypes.pop_back();
			}

			this->constitutingOutputMethodTypes = OutputMethods::forConstructorsCheckAndCopySingleOutputMethodTypes( newOutputMethods.constitutingOutputMethodTypes );
			}
			return *this;
	}
};

}

#endif /* OUTPUTMETHODS_H_ */
