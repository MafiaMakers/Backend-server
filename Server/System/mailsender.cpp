#include "mailsender.h"
#include "Exceptions/systemexception.h"
using namespace Mafia;
using namespace System;
const std::string MailSender::programPath = "..\\MessageSender.py";

void MailSender::send_email(QString receiver, QString subject, QString text)
{
    std::string cmd = programPath + " \"" + text.toStdString() + "\" " + receiver.toStdString() + " \"" + subject.toStdString() + "\"";

    std::string error = exec(cmd.c_str());

    if(error.length() > 0){
        throw new Exceptions::SystemException(String("Error in running email sender. \nError: " + error),
                                              Exceptions::SystemExceptionId_ErrorInCmd);
    }
}
