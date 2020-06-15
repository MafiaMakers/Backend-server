#ifndef RUNPROGRAM_H
//! Набор системных функций для запуска и завершения программ
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
    /*! \brief Пространство имен, в котором находятся системные функции.
     * Все общие функции и классы, которые могут понадобиться где угодно, находятся здесь
     */
    namespace System {
        /*!
         * \brief Запускает указанную программу
         * \param path Путь к программе, включая название файла
         * \param name Имя процесса, который будет запущен
         * \param args Дополнительные аргументы, которые стоит передать в программу
         * \return PID запущенного процесса. По этому PID можно позже завершить процесс
         */
        int run_app(const String path, const String name, const String args = String(""));

        /*!
         * \brief Выполнение cmd-команды.
         * (На самом деле эту функцию я просто со stackoverflow скопипастил и не разобрался, как она работает, так что соре)
         * Но потом обязательно разберусь
         * \param cmd Полностью команда, которую следует выполнить
         * \return Результат выполнения команды, записанный в std::string (По сути то, что терминал выводит при запросе этой команды)
         */
        std::string exec(const char* cmd);

        /*!
         * \brief Получение списка всех PID процессов с указанным именем
         * \param name имя процесса
         * \return список PID всех процессов, у которых имя совпадает с переданным именем
         */
        QList<int> get_all_pids_by_name(const String name);

        /*!
         * \brief Завершение процесса с указанным PID
         * \param pid PID процесса, который следует завершить
         */
        void kill(int pid);

    }

}


#endif // RUNPROGRAM_H
