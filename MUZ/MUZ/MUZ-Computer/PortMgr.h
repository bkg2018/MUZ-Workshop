/*
 * PortManager.h
 *
 *  Created on: Nov 28, 2018
 *      Author: Francis Piérot
 */

#ifndef MUZ_PORTMANAGER_H_
#define MUZ_PORTMANAGER_H_

#include <map>

#include "MUZ-Common/Exceptions.h"
#include "MUZ-Computer/PortModule.h"

namespace MUZ {

class PortMgr: public Module {
	
	// map of each IO address to a port module
	std::map<int, PortModule*> m_ports;
	
public:
	PortMgr();
	virtual ~PortMgr();
	
	/** Displays on a given peripheral. */
	virtual void DisplayOn(Peripheral* /*peripheral*/) {}
	
	/** Assigns a port module to a port address. If nullptr is given, the port address releases the port it is assigned. */
	void Assign(int address, PortModule* module) {
		if (module == nullptr) {
			std::map<int, PortModule*>::iterator iter = m_ports.find(address);
			if (iter == m_ports.end()) return; // nothing to do
			m_ports.erase(iter);
		} else {
			m_ports[address] = module;
		}
	}

	/** Generic input: returns a data. */
	virtual DATATYPE In(int address) {
		std::map<int, PortModule*>::iterator iter = m_ports.find(address);
		if (iter == m_ports.end()) throw UnassignedPortException();
		if (iter->second == nullptr) throw BUGNullAssignedPortException();
		return iter->second->In();		
	}
	
	/** Generic output: sends a data. */
	virtual void Out(int address, DATATYPE data) {
		std::map<int, PortModule*>::iterator iter = m_ports.find(address);
		if (iter == m_ports.end()) throw UnassignedPortException();
		if (iter->second == nullptr) throw BUGNullAssignedPortException();
		iter->second->Out(data);
	}
	

};

} /* namespace MUZ */

#endif /* MUZ_PORTMANAGER_H_ */
