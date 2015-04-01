/*
 * JulienBounvilayRollingParticlesVelocity.h
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

#ifndef JULIENBOUNVILAYROLLINGPARTICLESVELOCITY_H_
#define JULIENBOUNVILAYROLLINGPARTICLESVELOCITY_H_

#include "CalcBedloadVelocity.h"

//This method is based on equation 11 of:
// Pierre Y. Julien and Bounthanh Bounvilay (2013): Velocity of Rolling Bed Load Particles. Journal of Hydraulic Engineering, 139(2), 177-186.

namespace SedFlow {

class JulienBounvilayRollingParticlesVelocity: public CalcBedloadVelocity {
private:
	double roughnessDiameterPercentile;
	double roughnessDiameterFactor;

public:
	JulienBounvilayRollingParticlesVelocity(double roughnessDiameterPercentile, double roughnessDiameterFactor);
	virtual ~JulienBounvilayRollingParticlesVelocity();

	CalcBedloadVelocity* createCalcBedloadVelocityPointerCopy() const; //This method HAS TO BE implemented.

	ConstructionVariables createConstructionVariables()const;

	double calculate (const RiverReachProperties& riverReachProperties) const;
};

}

#endif /* JULIENBOUNVILAYROLLINGPARTICLESVELOCITY_H_ */
