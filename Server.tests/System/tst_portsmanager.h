#ifndef PORTSMANAGERTESTS_H
#define PORTSMANAGERTESTS_H
#include <QTest>
#include <QObject>
#include "System/portsmanager.h"

namespace MafiaTests {
	namespace SystemTests {
		class PortsManagerTests : public QObject
		{
			Q_OBJECT
		public:
			explicit PortsManagerTests(QObject *parent = nullptr);

		private slots:
			void allocate_ports();
			void check_free_first_returns_false();
			void check_free_second_returns_false();
			void free_ports();
			void free_not_allocated_port_exception();

		private:
			int firstPort;
			int secondPort;

			Mafia::System::PortsManager* manager;
		};
	}
}


#endif // PORTSMANAGERTESTS_H
