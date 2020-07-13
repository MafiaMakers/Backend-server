#ifndef KEYGENTESTS_H
#define KEYGENTESTS_H

#include <QTest>
#include <QObject>

namespace MafiaTests {
	namespace SystemTests {
		class KeyGenTests : public QObject
		{
			Q_OBJECT
		public:
			explicit KeyGenTests(QObject *parent = nullptr);

		private slots:
			void short_string_test_alpha_chars();
			void long_string_test_alpha_chars();

		private:
			bool is_alpha(char letter);
		};
	}
}


#endif // KEYGENTESTS_H
