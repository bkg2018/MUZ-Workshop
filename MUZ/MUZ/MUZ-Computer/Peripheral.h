/*
 * Peripheral.h
 *
 *  Created on: Nov 26, 2018
 *      Author: Francis Piérot
 */

#ifndef SRC_MUZ_PERIPHERAL_H_
#define SRC_MUZ_PERIPHERAL_H_

#include "MUZ-Computer/Module.h"

namespace MUZ {

class Peripheral: public Module {
public:
	Peripheral();
	virtual ~Peripheral();

	/** A peripheral never calls */
	virtual void DisplayOn(Peripheral* /*peripheral*/) {}
};

} /* namespace MUZ */

#endif /* SRC_MUZ_PERIPHERAL_H_ */
