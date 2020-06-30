#include "clientinfo.h"
using namespace Mafia;

bool ClientInfo::operator == (ClientInfo a)
{
    return (this->statistics.id == a.statistics.id) && (this->client == a.client);
}

bool ClientInfo::operator != (ClientInfo a)
{
    return !(*this == a);
}

