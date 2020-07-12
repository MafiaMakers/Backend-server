#ifndef SHA256TEST_H
#define SHA256TEST_H
#include <QTest>
#include <QObject>
namespace MafiaTests {
	namespace SystemTests {
		class SHA256Tests : public QObject
		{
			Q_OBJECT
		public:
			explicit SHA256Tests(QObject *parent = nullptr);

			private slots:
				void different_hashes();
				void equal_hashes();
				void long_string_hash();
		};
	}
}


#endif // SHA256TEST_H
