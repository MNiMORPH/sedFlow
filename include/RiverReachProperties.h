/*
 * RiverReachProperties.h
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

#ifndef RIVERREACHPROPERTIES_H_
#define RIVERREACHPROPERTIES_H_

#include <new>

#include "RegularRiverReachProperties.h"
#include "AdditionalRiverReachProperties.h"
#include "StrataSorting.h"
#include "GeometricalChannelBehaviour.h"
#include "SillProperties.h"
#include "OverallParameters.h"
#include "ConstructionVariables.h"
#include "CellIDConversions.h"

namespace SedFlow {

class RiverReachProperties {
	friend class RegularRiverSystemProperties;
private:
	int cellID;
	int downstreamCellID;
	RiverReachProperties* downstreamCellPointer;
	int numberOfUpstreamCells;
	std::vector<int> upstreamCellIDs;
	std::vector<RiverReachProperties*> upstreamCellPointers;
	bool upstreamMarginCell;
	bool downstreamMarginCell;
	const std::map<int,int>* mapFromRealCellIDtoUserCellID;
	OverallParameters* overallParameters;

public:
	RiverReachProperties(int cellID, RegularRiverReachProperties regularRiverReachProperties, AdditionalRiverReachProperties additionalRiverPropertiesAndMethods, const StrataSorting* strataSorting, const GeometricalChannelBehaviour* geometricalChannelBehaviour, const SillProperties* sillProperties, OverallParameters* overallParameters);
	RiverReachProperties(int cellID,
						int downstreamCellID,
						RegularRiverReachProperties regularRiverReachProperties,
						AdditionalRiverReachProperties additionalRiverPropertiesAndMethods,
						const StrataSorting* strataSorting,
						const GeometricalChannelBehaviour* geometricalChannelBehaviour,
						const SillProperties* sillProperties,
						OverallParameters* overallParameters);
	RiverReachProperties(const RiverReachProperties& toCopy);
	virtual ~RiverReachProperties();

	static RiverReachProperties createCopyAsUpstreamMarginCell(const RiverReachProperties& toCopy);
	static RiverReachProperties createCopyAsDownstreamMarginCell(const RiverReachProperties& toCopy);
	static RiverReachProperties createCopyAsNonMarginCell(const RiverReachProperties& toCopy);
	static std::vector<RiverReachProperties> createVectorCopy (const std::vector<RiverReachProperties>& toCopy);


	ConstructionVariables createConstructionVariables()const;

	RegularRiverReachProperties regularRiverReachProperties;
	AdditionalRiverReachProperties additionalRiverReachProperties;
	StrataSorting* strataSorting;
	GeometricalChannelBehaviour* geometricalChannelBehaviour;
	SillProperties* sillProperties;

	inline const OverallParameters* getOverallParameters() const { return overallParameters; }
	inline int getCellID() const { return cellID; }
	inline int getUserCellID() const { return CellIDConversions::getUserCellIDcorrespondingToRealCellID(cellID,(*mapFromRealCellIDtoUserCellID)); }
	inline int getDownstreamCellID() const { return downstreamCellID; }
	inline int getDownstreamUserCellID() const { return CellIDConversions::getUserCellIDcorrespondingToRealCellID(downstreamCellID,(*mapFromRealCellIDtoUserCellID)); }
	inline RiverReachProperties* getDownstreamCellPointer() const { return downstreamCellPointer; }
	inline std::vector<int> getUpstreamCellIDs() const { return upstreamCellIDs; }
	inline std::vector<RiverReachProperties*> getUpstreamCellPointers() const { return upstreamCellPointers; }
	inline int getNumberOfUpstreamCells() const { return numberOfUpstreamCells; }
	inline bool isMargin() const { return (upstreamMarginCell || downstreamMarginCell); }
	inline bool isUpstreamMargin() const { return upstreamMarginCell; }
	inline bool isDownstreamMargin() const { return downstreamMarginCell; }

	friend bool operator < (const RiverReachProperties& riverReachProperties1, const RiverReachProperties& riverReachProperties2)
	{
		return (riverReachProperties1.cellID < riverReachProperties2.cellID);
	}

	RiverReachProperties& operator = (const RiverReachProperties& toBeAssigned)
	{
		if (this != &toBeAssigned)
		{
			this->cellID = toBeAssigned.cellID;
			this->downstreamCellID = toBeAssigned.downstreamCellID;
			this->numberOfUpstreamCells = toBeAssigned.numberOfUpstreamCells;
			this->upstreamCellIDs = toBeAssigned.upstreamCellIDs;
			this->upstreamMarginCell = toBeAssigned.upstreamMarginCell;
			this->downstreamMarginCell = toBeAssigned.downstreamMarginCell;
			this->regularRiverReachProperties = toBeAssigned.regularRiverReachProperties;
			this->additionalRiverReachProperties = toBeAssigned.additionalRiverReachProperties;
			delete this->strataSorting;
			this->strataSorting = toBeAssigned.strataSorting->createStrataSortingPointerCopy();
			delete this->geometricalChannelBehaviour;
			this->geometricalChannelBehaviour = toBeAssigned.geometricalChannelBehaviour->createGeometricalChannelBehaviourPointerCopy();
			delete this->sillProperties;
			this->sillProperties = toBeAssigned.sillProperties->createSillPropertiesPointerCopy();
			this->overallParameters = toBeAssigned.overallParameters;
		}
		return *this;
	}

};

}

#endif /* RIVERREACHPROPERTIES_H_ */
