/*
 * FishEggs.h
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

#ifndef FISHEGGS_H_
#define FISHEGGS_H_

#include <vector>
#include <utility>

#include "GrainType.h"

namespace SedFlow {

class FishEggs: public GrainType {

protected:
	unsigned int insertionLayer;

public:
	FishEggs();
	FishEggs(const std::vector<double>& fractionalAbundance, const std::vector<double>& additionalParameters);
	FishEggs(const std::vector<double>& fractionalAbundance, int insertionLayer);
	FishEggs(const std::vector<double> &fractionalAbundance);
	FishEggs(const GrainType* toCopy);
	virtual ~FishEggs();

	virtual GrainType* createGrainTypePointerCopy() const;

	ConstructionVariables createConstructionVariables()const;

	void insert (std::vector<GrainType*>& strata) const;
	//Fish eggs are destroyed when eroded, the returned fractionalAbundance should thus be an array of zeros.
	//Fish eggs are destroyed by bed mobility. So all Fractions of active layer are set to zero when there is any bed mobility i.e. erosion.
	GrainType* erode (std::vector<GrainType*>& strata) const;
	GrainType* getPotentialErosion (const std::vector<GrainType*>& strata) const;
	void deposit (std::vector<GrainType*>& strata) const; //Should check whether inserted fractionalAbundance equals zero. Otherwise throw an error, since fish eggs should disappear when eroded.

	unsigned int getInsertionLayer() const;

	std::pair<CombinerVariables::TypesOfGrains , std::vector<double> > typeSpecificGetter() const; //Here used to get insertionLayer
};

}

#endif /* FISHEGGS_H_ */
