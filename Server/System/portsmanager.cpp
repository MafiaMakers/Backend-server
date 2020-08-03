#include "portsmanager.h"
#include "simplestring.h"
#include "Exceptions/systemexception.h"
using namespace Mafia;
using namespace System;

MafiaList<int> PortsManager::allocatedPorts = MafiaList<int>();



bool PortsManager::is_port_free(int port)
{
    return (MIN_PORT <= port) && (port <= MAX_PORT) && !allocatedPorts.contains(port);
}

int PortsManager::allocate_port()
{
    for(int i = MIN_PORT; i < MAX_PORT; i++){
        if(is_port_free(i)){
            allocatedPorts.append(i);
            return i;
        }
    }

	throw Exceptions::Exception::generate(String("No free ports!"), Exceptions::SystemExceptionId_PortAllocation);
    return -1;
}

void PortsManager::free_port(int port)
{
    if(allocatedPorts.contains(port)){
        allocatedPorts.removeOne(port);
    } else{
		throw Exceptions::Exception::generate(String("This port wasn't be allocated"), Exceptions::SystemExceptionId_PortFreeing);
    }
}
