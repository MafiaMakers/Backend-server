#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QString>
#include <QDateTime>
#include <QList>
#include "user.h"
#include "Exceptions/databaseworkingexception.h"
//#include "chatsettingsdatabasemanager.h"
#include "chatdatabasemanager.h"
#include <QVariant>
#include "databasemanager.h"



namespace Mafia{
    namespace Database{
        //! \brief Размер строки для хранения в БД
        const int STRING_SIZE = 256;

        QString get_sql_filter(FilterType filter);


        template<class T>
        /*!
         * \brief Функция получения строки с sql названием типа данных, переданного в функцию
         * \param T Тип, который надо преобразовать в sql-тип
         * \return SQL-тип в виде строки
         */
        QString get_sql_type();

        template<class T>
        /*!
         * \brief Функция получения строки с sql названием типа данных, переданного в функцию (эта функция для списков)
         * \param T Тип, который надо преобразовать в sql-тип
         * \param maxListCount Если этот тип - список, то также надо передать максимальный размер, которого может достигать этот список
         * \return SQL-тип в виде строки
         */
        QString get_sql_type(int mesListCount);

        /*!
         * \brief Функция преобразования QByteArray, полученного из БД в QList<T>
         * \param data данные, полученные из БД в виде QByteArray
         * \return список, сгенерированный на основе этих данных
         */
        template<class T>
        MafiaList<T> qlist_from_byte_array(QByteArray data)
        {
            char* dataCharPtr = data.data();

            char current = 0;

            char* trueData = new char[data.size() / 2];

            for(int i = 0; i < data.size(); i++){
                if(i % 2 == 0){
                    current += 16 * (dataCharPtr[i] - 'M');
                } else{
                    current += (dataCharPtr[i] - 'M');
                    trueData[i / 2] = current;
                    current = 0;
                }
            }

            T*dataPtr = (T*)trueData;
            if(data.size() % (2 * sizeof(T)) == 0){
                int size = data.size() / sizeof(T) / 2;
                MafiaList<T> res = MafiaList<T>();
                for(int i = 0; i < size; i++){
                    res.append(dataPtr[i]);
                }

                return res;
            } else{
                throw new Exceptions::DatabaseWorkingException(System::String("Request answer was null"), Exceptions::DatabaseWorkingExceptionId_EmptyQueryResult);
                return MafiaList<T>();
            }
        }

        template<class T>
        /*!
         * \brief Перевод из данных, полученных в ответе на sql-запрос в нормальный тип данных T
         * \param data Данные, полученные функцией вида query->value(someIndex);
         * \return Данные типа T, преобразованные из данных запроса
         */
        T query_value_to_variable(QVariant data);


        /*!
         * \brief Функция преобразования QList<T>, полученного из программы в QByteArray для записи в БД
         * \param data Список, который следует преобразовать
         * \return QByteArray, готовый для записи в БД
         */
        template<class T>
        QByteArray qbytearray_from_qlist(MafiaList<T> data)
        {
            char* dataPtr = new char[data.length() * sizeof (T) * 2];

            for(int i = 0; i < data.length(); i++){
                for(unsigned int j = 0; j < sizeof(T); j++){
                    dataPtr[i * sizeof(T) * 2 + j * 2] = 'M' + (((char*)&(data[i]))[j] / 16);
                    dataPtr[i * sizeof(T) * 2 + j * 2 + 1] = 'M' + (((char*)&(data[i]))[j] % 16);
                }
            }

            return QByteArray(dataPtr, data.length() * sizeof (T) * 2);
        }

    }
}

#endif // DATABASEHELPER_H
