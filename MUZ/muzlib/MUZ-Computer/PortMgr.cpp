/*
 * PortManager.cpp
 *
 *  Created on: Nov 28, 2018
 *      Author: bkg2018
 */
#include "pch.h"
#include "MUZ-Computer/PortMgr.h"

namespace MUZ {

	PortMgr::PortMgr() {
	}

	PortMgr::~PortMgr() {
	}

	/** Displays on a given peripheral. */
	void PortMgr::DisplayOn(Peripheral* /*peripheral*/) {}

	/** Assigns a port module to a port address. If nullptr is given, the port address releases the port it is assigned. */
	void PortMgr::Assign(int address, PortModule* module) {
		if (module == nullptr) {
			std::map<int, PortModule*>::iterator iter = m_ports.find(address);
			if (iter == m_ports.end()) return; // nothing to do
			m_ports.erase(iter);
		} else {
			m_ports[address] = module;
		}
	}

	/** Generic input: returns a data. */
	DATATYPE PortMgr::In(int address) {
		std::map<int, PortModule*>::iterator iter = m_ports.find(address);
		if (iter == m_ports.end()) throw UnassignedPortException();
		if (iter->second == nullptr) throw BUGNullAssignedPortException();
		return iter->second->In();
	}

	/** Generic output: sends a data. */
	void PortMgr::Out(int address, DATATYPE data) {
		std::map<int, PortModule*>::iterator iter = m_ports.find(address);
		if (iter == m_ports.end()) throw UnassignedPortException();
		if (iter->second == nullptr) throw BUGNullAssignedPortException();
		iter->second->Out(data);
	}




} /* namespace MUZ */
