#ifndef SIMPLESTRING_H
#define SIMPLESTRING_H
#include <iostream>
#include <QString>

namespace Mafia {
    /*!
     * \brief The String class упрощенная строка
     * (мне просто страшно использовать QString или std::string, так как они сложные и там возникают кринжи с последним символом)
     */
    class String{
    public:
        /*!
         * \brief String конструктор, создающий строку из массива символов и его размера
         * \param d данные
         * \param s размер массива данных
         */
        String(char* d, int s){
            this->data = d;
            this->size = s;
        }
        /*!
         * \brief String конструктор, создающий строку из std::string
         * \param data данные из которых копировать строку
         */
        String(std::string data){
            this->data = (char*)data.c_str();
            this->size = data.length();
        }
        /*!
         * \brief String пустой конструктор, который создает строку, к которой НЕЛЬЗЯ обращаться, так как в ней пустой указатель.
         * То есть строка прям совсем пустая. Сделано для того, чтобы не выделять лишнюю память.
         * Можно использовать в случае, когда строка тут же будет инициализирована какой-нибудь функцией или типа того
         */
        String(){
            String(0, 0);
        }
        //! массив данных строки
        char* data;
        //! длина массива данных
        int size;
    };

}



#endif // SIMPLESTRING_H
