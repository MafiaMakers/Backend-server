#ifndef CRYPTOTESTS_H
#define CRYPTOTESTS_H
#include <QTest>
#include <QObject>

namespace MafiaTests {
	namespace NetworkTests {
		class CryptoTests : public QObject
		{
			Q_OBJECT
		public:
			explicit CryptoTests(QObject *parent = nullptr);

		private slots:
			void wrap_parse_text_message();
			void wrap_parse_notext_message();
			void wrap_parse_empty_message();
			void dich_key_test();
		};
	}
}

#endif // CRYPTOTESTS_H
