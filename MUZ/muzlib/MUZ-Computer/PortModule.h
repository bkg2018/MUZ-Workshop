/*
 * PortModule.h - Specialized input/output port
 *
 * General services:
 *
 *      receive a byte (output, from external view)
 *      return a byte (input, from external view)
 *      initializes
 *      receive links to other Modules (i.e. reference or pointer to the Module instance)
 *
 * Ports can be linked to other modules, derived classes can use this to keep references to the modules they control.
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_PORTMODULE_H_
#define SRC_MUZ_PORTMODULE_H_

#include <map>

#include "MUZ-Computer/Module.h"

namespace MUZ {

	class PortModule: public Module {

		std::map<int, Module*> m_modules;

	public:
		PortModule();
		virtual ~PortModule();

		/** Generic input: returns a data. */
		virtual DATATYPE In(void);

		/** generic output: sends a data. */
		virtual void Out(DATATYPE /*data*/);

		/** Add a reference to a module. A reference number is given, and assigning nullptr to the number will
		 delete the module reference.
		 @param reference a reference number for the port use
		 @param module the module to be used, nullptr to delete a reference
		 */
		virtual void AssignModule(int reference, Module* module);

		/** Displays on a given peripheral. */
		virtual void DisplayOn(Peripheral* /*peripheral*/);
	};

};
#endif /* SRC_MUZ_PORTMODULE_H_ */
