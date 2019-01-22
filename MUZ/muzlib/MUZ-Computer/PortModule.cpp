/*
 * PortModule.cpp
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */
#include "pch.h"
#include "MUZ-Computer/PortModule.h"

namespace MUZ {

	PortModule::PortModule() {
	}

	PortModule::~PortModule() {
	}

	/** Generic input: returns a data. */
	DATATYPE PortModule::In(void)
	{
		return 0;
	}

	/** generic output: sends a data. */
	void PortModule::Out(DATATYPE /*data*/)
	{
	}

	/** Add a reference to a module. A reference number is given, and assigning nullptr to the number will
	 delete the module reference.
	 @param reference a reference number for the port use
	 @param module the module to be used, nullptr to delete a reference
	 */
	void PortModule::AssignModule(int reference, Module* module)
	{
		if (module == nullptr) {
			std::map<int, Module*>::iterator iter = m_modules.find(reference);
			if (iter == m_modules.end()) return; // nothing to do
			m_modules.erase(iter);
		} else {
			m_modules[reference] = module;
		}
	}

	/** Displays on a given peripheral. */
	void PortModule::DisplayOn(Peripheral* /*peripheral*/)
	{
	}
}
