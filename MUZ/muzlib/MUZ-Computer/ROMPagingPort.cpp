/*
 * ROMPagingPort.cpp
 *
 *  Created on: Nov 28, 2018
 *      Author: bkg2018
 */
#include "pch.h"
#include "MUZ-Computer/ROMPagingPort.h"

namespace MUZ {

	ROMPagingPort::ROMPagingPort() {
	}

	ROMPagingPort::~ROMPagingPort() {
	}

	// Store access to computer at construction
	ROMPagingPort::ROMPagingPort(Computer* computer, bool switchmode) {
		m_computer = computer;
		m_switchmode = switchmode;
	}

	/** Enable or disable the switching mode.*/
	void ROMPagingPort::SetSwitchMode(bool yes) {
		m_switchmode = yes;
	}

	/** Does the paging depending on the data. The official RC2014 ROM Module does paging by switching at each output access,
	 * while other boards take 0 to page in and 1 to page out. The working mode is selected at construction.
	 */
	void ROMPagingPort::Out(DATATYPE data) {
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

} /* namespace MUZ */
