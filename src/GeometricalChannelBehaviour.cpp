/*
 * GeometricalChannelBehaviour.cpp
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

#include "GeometricalChannelBehaviour.h"

#include <iostream>

namespace SedFlow {

GeometricalChannelBehaviour::GeometricalChannelBehaviour():
	alluviumChannel(NULL)
{}

GeometricalChannelBehaviour::GeometricalChannelBehaviour(const ChannelGeometry* alluviumChannel):
	alluviumChannel(alluviumChannel->createChannelGeometryPointerCopy())
{}

GeometricalChannelBehaviour::~GeometricalChannelBehaviour()
{
	delete this->alluviumChannel;
}

Grains GeometricalChannelBehaviour::convertActiveWidthAndOverallSedimentVolumeIncrementIntoSedimentVolumeIncrementPerUnitBedSurface (double activeWidth, Grains sedimentVolumeIncrement) const
{
	double activePerimeter = alluviumChannel->convertActiveWidthIntoActivePerimeter(activeWidth);
	return (sedimentVolumeIncrement / activePerimeter);
}

Grains GeometricalChannelBehaviour::convertActiveWidthAndSedimentVolumePerUnitBedSurfaceIntoOverallActiveSedimentVolume (double activeWidth, Grains sedimentVolumePerUnitBedSurface) const
{
	double activePerimeter = alluviumChannel->convertActiveWidthIntoActivePerimeter(activeWidth);
	return (sedimentVolumePerUnitBedSurface * activePerimeter);
}

}
