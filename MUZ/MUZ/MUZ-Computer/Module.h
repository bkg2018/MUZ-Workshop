/*
 * Module.h - base class for all emulator modules.
 *
 * A module have general services:
 *
 * 	    display itself
 *      return status
 *
 * Derived classes can be of the interface, memory or port type with
 * other services.
 *
 *  Created on: Nov 23, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_MODULE_H_
#define SRC_MUZ_MODULE_H_

#include "Types.h"

namespace MUZ {

// Including Peripheral.h would make a circular reference, so just forward declare the class.
class Peripheral;

/** Base class of any entity in MUZ. A module can display itself and return a status, and receive other modules.
 *  This class is abstract and declares minimum services that every module must define.
 */
class Module {
public:
	Module();
	virtual ~Module();

	/** Displays the module informations/content on a given peripheral. */
	virtual void DisplayOn(Peripheral* /*peripheral*/) = 0;
};

};

#endif /* SRC_MUZ_MODULE_H_ */
