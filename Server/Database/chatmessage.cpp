#include "chatmessage.h"
#include "databasehelper.h"
using namespace Mafia;
using namespace Database;

void ChatMessage::show()
{
    std::cout << "-------------------------\nMESSAGE" <<
                 "\nid = " << this->id <<
                 "\nfrom = " << this->from <<
                 "\ndata = " << this->data.toStdString() <<
                 "\ndate = " << this->timestamp.toString(SQL_DATETIME_FORMAT).toStdString() <<
                 "\nto chat = " << this->toChat <<
                 "\nfeature = " << this->feature <<
                 "\nreadUsers:";
    for(int j = 0; j < this->readUsers.length(); j++){
        std::cout << "\n    " << this->readUsers[j];
    }
    std::cout << "\nanswer for:";
    for(int j = 0; j < this->answerFor.length(); j++){
        std::cout << "\n    " << this->answerFor[j];
    }
    std::cout << std::endl;
}
