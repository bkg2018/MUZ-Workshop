/*
 * ROMPagingPort.h
 *
 *  Created on: Nov 28, 2018
 *      Author: bkg2018
 */

#ifndef MUZ_ROMPAGINGPORT_H_
#define MUZ_ROMPAGINGPORT_H_

#include "MUZ-Computer/Computer.h"
#include "MUZ-Common/Exceptions.h"
#include "MUZ-Computer/PortModule.h"

namespace MUZ {

class ROMPagingPort: public PortModule {

	// store access to the computer for paging functions
	Computer*	m_computer = nullptr;
	bool		m_switchmode = false;

public:
	ROMPagingPort();

	// Store access to computer at construction
	ROMPagingPort(Computer* computer, bool switchmode) {
		m_computer = computer;
		m_switchmode = switchmode;
	}
	virtual ~ROMPagingPort();
	
	/** Enable or disable the switching mode.*/
	void SetSwitchMode(bool yes) {
		m_switchmode = yes;
	}

	/** Does the paging depending on the data. The official RC2014 ROM Module does paging by switching at each output access,
	 * while other boards take 0 to page in and 1 to page out. The working mode is selected at construction.
	 */
	virtual void Out(DATATYPE data) {
		if (!m_computer) throw BUGNoComputerException();
		if (m_switchmode) {
			m_computer->PageROMswitch();
		} else {
			if (data == 0) {
				m_computer->PageROMin();
			} else {
				m_computer->PageROMout();
			}
		}
	}


};

} /* namespace MUZ */

#endif /* MUZ_ROMPAGINGPORT_H_ */
