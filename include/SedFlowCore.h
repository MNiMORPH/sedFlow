/*
 * SedFlowCore.h
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

#ifndef SEDFLOWCORE_H_
#define SEDFLOWCORE_H_

#include "SedFlowHeaders.h"

#include <vector>

namespace SedFlow {

class SedFlowCore {
private:
	OverallParameters* overallParameters;
	OverallMethods* overallMethods;
	RiverSystemProperties* riverSystemProperties;
	RiverSystemMethods* riverSystemMethods;
	OutputMethods* outputMethods;


public:
	static inline int runSimulation(const char* inputXMLfile)
	{
		try
		{
			SedFlowCore sedFlow = initialise(inputXMLfile);

			try
			{
				sedFlow.riverSystemMethods->throwExceptionIfWaterFlowIsNotHighestOrderFlowMethod();
				while (sedFlow.overallParameters->getElapsedSeconds() < sedFlow.overallParameters->getFinishSeconds())
				{
					sedFlow.checkForTooSmallTimeSteps();
					sedFlow.performTimeStep();
				}
				sedFlow.checkForInfiniteOrNaNTimeSteps();

			} catch (...) {
				sedFlow.outputMethods->forcedWriteOutputLine();
				sedFlow.finish();
				throw; }

			sedFlow.finish();

		} catch (const char *const msg) {
			std::cerr << msg << std::endl;
			throw;
		}

		return 0;
	}

	SedFlowCore(const char* inputXMLfile);

	static inline SedFlowCore initialise(const char* inputXMLfile)
	{
		SedFlowCore sedFlow = SedFlowCore(inputXMLfile);

		RegularRiverReachProperties& downstreamMarginProperties = (sedFlow.riverSystemProperties->regularRiverSystemProperties.cellProperties.back()).regularRiverReachProperties;
		downstreamMarginProperties.waterEnergyslope = downstreamMarginProperties.bedslope;

		if (sedFlow.overallParameters->updateRegularPropertiesAfterInitialisation) { sedFlow.riverSystemMethods->updateRegularProperties(); }
		if (sedFlow.overallParameters->updateAdditionalRiverReachPropertiesAfterInitialisation) { sedFlow.riverSystemMethods->updateAdditionalRiverReachProperties(); }
		if (sedFlow.overallParameters->updateOutputMethodsAfterInitialisation) { sedFlow.outputMethods->update(); }
		sedFlow.outputMethods->initialiseOutput();
		return sedFlow;
	}

	inline void performTimeStep()
	{
		riverSystemMethods->calculateAndModifyChangeRates();
			/*
			 * More complex succession of function calls
			 */
		overallParameters->currentTimeStepLengthInSeconds = riverSystemMethods->calculateTimeStep() * overallParameters->getTimeStepFactor();
			/* i.e.
			 * riverSystem.flowMethods.getExtremeChangeRates()
			 * riverSystem.flowMethods.calculateTimeStep()
			 */
		riverSystemMethods->calculateAndHandDownChanges();
			/* i.e.
			 * riverSystem.flowMethods.calculateAndHandDownChanges()
			 * riverSystem.flowMethods.handDownChanges()
			 */
		riverSystemMethods->performAdditionalReachActions();
		riverSystemMethods->performAdditionalRiverSystemActions();
			/* i.e.
			 * riverSystem.parameters.additionalCellParameters.typeSpecificActions()
			 */
		riverSystemMethods->applyChanges();
			/* i.e.
			 * riverSystem.flowMethods.applyChanges()
			 */
		riverSystemMethods->updateRegularProperties();
			/* i.e.
			 * riverSystem.updateBedSlopes();
			 * riverSystem.flowMethods.updateOtherParameters()
			 * riverSystem.flowMethods.handDownOtherParameters()
			 * riverSystem.updateEnergySlopes();
			 * riverSystem.updateTaus();
			 * riverSystem.updateActiveWidths();
			 */
		riverSystemMethods->updateAdditionalRiverReachProperties();
			/* i.e.
			 * riverSystem.parameters.additionalCellParameters.update()
			 */

		overallParameters->elapsedSeconds += overallParameters->currentTimeStepLengthInSeconds;

		outputMethods->update();
		outputMethods->writeOutputLineIfScheduled();

	}

	inline void finish()
	{
		outputMethods->finaliseOutput();
	}

	//The method { void dispose(); } is represented by the destructor.
	virtual ~SedFlowCore()
	{
		delete outputMethods;
		delete riverSystemMethods;
		delete overallMethods;
		delete riverSystemProperties;
		delete overallParameters;
	}

	ConstructionVariables createConstructionVariables()const;

	inline const OverallParameters* getConstantOverallParametersPointer() const { return overallParameters; }
	inline const OverallMethods* getConstantOverallMethodsPointer() const { return overallMethods; }
	inline const RiverSystemProperties* getConstantRiverSystemPropertiesPointer() const { return riverSystemProperties; }
	inline const RiverSystemMethods* getConstantRiverSystemMethodsPointer() const { return riverSystemMethods; }
	inline const OutputMethods* getConstantOutputMethodsPointer() const { return outputMethods; }

	void checkForInfiniteOrNaNTimeSteps() const;
	void checkForTooSmallTimeSteps() const;

};

}


#endif /* SEDFLOWCORE_H_ */
