#ifndef MAILSENDER_H
#define MAILSENDER_H
#include "runprogram.h"
#include <QString>

namespace Mafia {
    namespace System {
        class MailSender
        {
        public:

            static void send_email(QString receiver, QString subject, QString text);

        private:

            static const std::string programPath;
        };
    }
}


#endif // MAILSENDER_H
