#ifndef RUNPROGRAM_H
#define RUNPROGRAM_H

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <QList>

#include "simplestring.h"

namespace Mafia {

    int run_app(const String path, const String name, const String args);

    std::string exec(const char* cmd);

    QList<int> get_all_pids_by_name(const String name);

    void kill(int pid);

}


#endif // RUNPROGRAM_H
