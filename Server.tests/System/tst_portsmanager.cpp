#include "tst_portsmanager.h"
#include "Exceptions/exception.h"

using namespace MafiaTests;
using namespace SystemTests;

PortsManagerTests::PortsManagerTests(QObject *parent) : QObject(parent)
{
	manager = new Mafia::System::PortsManager();
}

void PortsManagerTests::allocate_ports()
{
	firstPort = manager->allocate_port();
	secondPort = manager->allocate_port();

	QVERIFY(firstPort != secondPort);
}

void PortsManagerTests::check_free_first_returns_false()
{
	QVERIFY(manager->is_port_free(firstPort) == false);
}

void PortsManagerTests::check_free_second_returns_false()
{
	QVERIFY(manager->is_port_free(secondPort) == false);
}

void PortsManagerTests::free_ports()
{
	manager->free_port(firstPort);
	manager->free_port(secondPort);

	QVERIFY(manager->is_port_free(firstPort) == true);
	QVERIFY(manager->is_port_free(secondPort) == true);
}

void PortsManagerTests::free_not_allocated_port_exception()
{
	int port = manager->allocate_port();
	manager->free_port(port);

	QVERIFY_EXCEPTION_THROWN(manager->free_port(port), Mafia::Exceptions::Exception*);
}
