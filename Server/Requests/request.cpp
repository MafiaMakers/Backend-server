#include "request.h"
#include <thread>
#include<chrono>
using namespace Mafia;


Request::Request()
{
    this->finished = false;
}

bool Mafia::Request::is_finished()
{
    return this->finished;
}

void Mafia::Request::close_request()
{
    this->finished = true;
    emit this->on_ready();
    emit this->on_ready_me(this);
}

void Request::sleep_untill_finished()
{
    while(! this->finished){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Request::tryClose()
{
    close_request();
}
