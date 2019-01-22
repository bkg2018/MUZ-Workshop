/*
 * CPU.h
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_CPU_H_
#define SRC_MUZ_CPU_H_

#include "MUZ-Computer/Module.h"
#include "MUZ-Computer/Peripheral.h"

namespace MUZ {

class CPU: public Module {
public:
	CPU();
	virtual ~CPU();

	/** Displays on a given peripheral. */
	virtual void DisplayOn(Peripheral* /*peripheral*/) ;
};

} /* namespace MUZ */

#endif /* SRC_MUZ_CPU_H_ */
