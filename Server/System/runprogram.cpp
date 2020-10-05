#include "runprogram.h"
#include "sstream"
#include "stdio.h"
//#include "unique_ptr.h"
#include "System/functions.h"
using namespace Mafia;
using namespace System;
int Mafia::System::run_app(const String pathToFile, const String processName, const String args)
{

	//Получаем PID всех процессов с таким же именем, какое будет у нашего нового процесса
    MafiaList<int> previousAppsPIDs = get_all_pids_by_name(processName);

	//Собираем cmd-команду, которую будем выполнять
    std::string command = "start ";
    for(int i = 0; i < pathToFile.size; i++){
        command += pathToFile.data[i];
    }

    command += " ";

    for(int i = 0; i < args.size; i++){
        command += args.data[i];
    }

    system(command.c_str());

	//Получаем список PID всех процессов с таким же именем, как и наш новый (он в этом списке уже есть)
    MafiaList<int> nowAppsPIDs = get_all_pids_by_name(processName);

	//Теперь находим разницу этих двух списков
    MafiaList<int> difference = MafiaList<int>();

    for(int i = 0; i < nowAppsPIDs.length(); i++){
        if(!previousAppsPIDs.contains(nowAppsPIDs[i])){
            difference.append(nowAppsPIDs[i]);
            //std::cout << "difference! - " << nowAppsPIDs[i] << std::endl;
        }
    }
    int a = -1;

	//Списки должны отличаться лишь на один элемент - это и есть наш процесс
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
	//Вызываем команду в cmd
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

	//Если все ок, то продолжаем
    if (!pipe) {
		std::cout << "Cringe\n";
       throw std::runtime_error("popen() failed!");
    }

	//Пока консоль что-то выводит, мы это добавляем к строке результата
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
       result += buffer.data();
    }
    return result;
}

MafiaList<int> Mafia::System::get_all_pids_by_name(const String name)
{
	//cmd-команда, которая получает список процессов с таким именем
    std::string command = "tasklist /v /fo csv | findstr /i \"";
    for(int i = 0; i < name.size; i++){
        command += name.data[i];
    }
    command += "\"";

	//Получаем все такие процессы
    std::string data = Mafia::System::exec(command.c_str());

	//Дальше надо парсить строку с ответом. Там точно написаны все нужные PID
	//Вот так выглядит такая строка:
	//"chrome.exe","4964","Console","1","21 908 КБ","Unknown","DESKTOP-TCNG0P2\Andru","0:00:00","Н/Д"
    MafiaList<int> result = MafiaList<int>();
	//Идем по строкам. То есть каждая строка - это один процесс
	//Индекс символа в строке (разделенной \n) символа, с которым мы работаем
    int indexInLine = 0;
    int lastIndex = -1;
	//Индекс общей строки (data), с которого начинается строка, которую мы обрабатываем сейчас
    int firstLineIndex = 0;

    for(unsigned int i = 0; i < data.length(); i++){
		//PID заключен в кавычки
		//Поэтому если мы встретили кавычку, которая стоит дальше, чем имя процесса
		//и мы еще ни разу не встречали ее раньше, то стоит обработать
        if(lastIndex == -1 && data[i] == '\"' && indexInLine > name.size + 3){
			//Все, что между именем процесса и кавычкой - PID
            lastIndex = i;

			//Копируем в отдельную строку подстроку, содержащую PID
			//Начнем с name.size + 4, закончим символом кавычки
            int firstIndex = (firstLineIndex + name.size + 4);

			char* source;
			SAFE_NEW(source, char[lastIndex - firstIndex]);

            for(int j = firstIndex; j < lastIndex; j++){
                source[j - firstIndex] = data[j];
            }

			//Преобразуем строку в число и добавляем в список
            int res = atoi(source);
			SAFE_DELETE_ARRAY(source);
            result.append(res);
        }
		//Если встречаем смену строки, то обнуляем все предыдущие параметры и парсим новую строку
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
	//Преобразуем число в строку
    std::stringstream ss;
    ss << pid;
	std::string pid_str;
	ss >> pid_str;
	//Собираем cmd-команду для убийства процесса
    std::string command = "taskkill /f /t /pid ";
    command += pid_str;

	//Выполняем команду
    system(command.c_str());
    std::cout << "killed" << std::endl;
}
