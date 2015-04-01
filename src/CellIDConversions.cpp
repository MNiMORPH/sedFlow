/*
 * CellIDConversions.cpp
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

#include "CellIDConversions.h"

#include <limits>
#include <iostream>

namespace SedFlow {


int CellIDConversions::getRealCellIDcorrespondingToUserCellID (int userCellID, const std::map<int,int>& mapFromRealCellIDtoUserCellID)
{
	std::map<int,int>::const_iterator it;
	int result;

	for(it = mapFromRealCellIDtoUserCellID.begin(); it != mapFromRealCellIDtoUserCellID.end(); ++it)
	{
	if (it->second == userCellID)
			{ result = it->first; 	break; }
	}

	if ( it == mapFromRealCellIDtoUserCellID.end() )
	{
		const char *const errorMessage = "User cell ID not mapped to real cell ID.";
		throw(errorMessage);
	}

	return result;
}

int CellIDConversions::getUserCellIDcorrespondingToRealCellID (int realCellID, const std::map<int,int>& mapFromRealCellIDtoUserCellID)
{
	int result;
	std::map<int,int>::const_iterator resultIterator = mapFromRealCellIDtoUserCellID.find(realCellID);
	if (resultIterator == mapFromRealCellIDtoUserCellID.end() ) { result = std::numeric_limits<int>::min(); }
	else { result = resultIterator->second; }
	return result;
}

}
