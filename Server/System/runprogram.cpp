#include "runprogram.h"
#include "sstream"
#include "System/functions.h"
using namespace Mafia;
using namespace System;

int Mafia::System::run_app(const String pathToFile, const String processName, const String args)
{

    MafiaList<int> previousAppsPIDs = get_all_pids_by_name(processName);

    std::string command = "start ";
    for(int i = 0; i < pathToFile.size; i++){
        command += pathToFile.data[i];
    }

    command += " ";

    for(int i = 0; i < args.size; i++){
        command += args.data[i];
    }

    system(command.c_str());

    MafiaList<int> nowAppsPIDs = get_all_pids_by_name(processName);

    MafiaList<int> difference = MafiaList<int>();

    for(int i = 0; i < nowAppsPIDs.length(); i++){
        if(!previousAppsPIDs.contains(nowAppsPIDs[i])){
            difference.append(nowAppsPIDs[i]);
            //std::cout << "difference! - " << nowAppsPIDs[i] << std::endl;
        }
    }
    int a = -1;

    if(difference.length() == 1){
        a = difference[0];
        std::cout << "started new app. " << a << " - pid" << std::endl;
    }
    return a;
}

std::string Mafia::System::exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
       throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
       result += buffer.data();
    }
    return result;
}

MafiaList<int> Mafia::System::get_all_pids_by_name(const String name)
{
    std::string command = "tasklist /v /fo csv | findstr /i \"";
    for(int i = 0; i < name.size; i++){
        command += name.data[i];
    }
    command += "\"";

    std::string data = Mafia::System::exec(command.c_str());

    //std::cout << data << std::endl << std::endl;

    MafiaList<int> result = MafiaList<int>();
    int indexInLine = 0;
    int lastIndex = -1;
    int firstLineIndex = 0;
    for(unsigned int i = 0; i < data.length(); i++){
        if(lastIndex == -1 && data[i] == '\"' && indexInLine > name.size + 3){
            lastIndex = i;
            int firstIndex = (firstLineIndex + name.size + 4);
            //std::cout << firstIndex - firstLineIndex << " " << lastIndex - firstLineIndex << std::endl;
            char* source = new char[lastIndex - firstIndex];
            for(int j = firstIndex; j < lastIndex; j++){
                source[j - firstIndex] = data[j];
            }
            //std::cout << source << std::endl;
            int res = atoi(source);
            //std::cout << res << " - res" << std::endl;
            result.append(res);
        }
        if(data[i] == '\n'){
            indexInLine = -1;
            lastIndex = -1;
            firstLineIndex = i+1;
        }
        indexInLine++;
    }
    return(result);
}

void Mafia::System::kill(int pid)
{
    std::stringstream ss;
    ss << pid;
    std::string command = "taskkill /f /t /pid ";
    std::string pid_str;
    ss >> pid_str;
    command += pid_str;
    system(command.c_str());
    std::cout << "killed" << std::endl;
}
