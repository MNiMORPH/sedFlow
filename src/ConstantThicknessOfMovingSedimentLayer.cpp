/*
 * ConstantThicknessOfMovingSedimentLayer.cpp
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

#include "ConstantThicknessOfMovingSedimentLayer.h"

namespace SedFlow {

ConstantThicknessOfMovingSedimentLayer::ConstantThicknessOfMovingSedimentLayer(double constantThickness):
	constantThickness(constantThickness)
{}

ConstantThicknessOfMovingSedimentLayer::~ConstantThicknessOfMovingSedimentLayer() {}

EstimateThicknessOfMovingSedimentLayer* ConstantThicknessOfMovingSedimentLayer::createEstimateThicknessOfMovingSedimentLayerPointerCopy() const
{
	EstimateThicknessOfMovingSedimentLayer* result = new ConstantThicknessOfMovingSedimentLayer(this->constantThickness);
	return result;
}

ConstructionVariables ConstantThicknessOfMovingSedimentLayer::createConstructionVariables()const
{
	ConstructionVariables result = ConstructionVariables();
	result.interfaceOrCombinerType = CombinerVariables::EstimateThicknessOfMovingSedimentLayer;
	result.realisationType = CombinerVariables::typeOfEstimationMethodForThicknessOfMovingSedimentLayerToString(CombinerVariables::ConstantThicknessOfMovingSedimentLayer);
	std::vector<double> doubleVector;
	doubleVector.push_back(this->constantThickness);
	result.labelledDoubles["constantThickness"] = doubleVector;
	return result;
}

}
