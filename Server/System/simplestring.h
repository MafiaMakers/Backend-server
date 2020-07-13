#ifndef SIMPLESTRING_H
#define SIMPLESTRING_H
#include <iostream>
#include <QString>

namespace Mafia {
    namespace System {
        #define NAME(variable) (#variable)
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
				this->data = new char[data.length() + 1];
				this->data[data.length()] = '\0';
				this->size = data.length();
                for(int i = 0; i < this->size; i++){
                    this->data[i] = data[i];
                }
            }
            /*!
             * \brief String пустой конструктор, который создает строку, к которой НЕЛЬЗЯ обращаться, так как в ней пустой указатель.
             * То есть строка прям совсем пустая. Сделано для того, чтобы не выделять лишнюю память.
             * Можно использовать в случае, когда строка тут же будет инициализирована какой-нибудь функцией или типа того
             */
            String(){
                data = 0;
                size = 0;
            }

            /*!
             * \brief Оператор глубокого копирования
             * \param source строка, из которой копировать
             * \return Строку, полученную в результате копирования
             */
            String operator = (String source){
                this->size = source.size;
                this->data = new char[this->size];
                for(int i = 0; i < this->size; i++){
                    this->data[i] = source.data[i];
                }
                return source;
            }
            /*!
             * \brief Функция, которая вставляет строку в данную строку начиная с необходимого индекса
             * Например, если надо вставить в строку "Hello ______" строку "world!".
             * firstString.paste(secondString, 6);
             * \param data Строка, которую необходимо вставить
             * \param index Индекс этой строки (не той, которая data, а именно этой), с которого необходимо начать вставлять данные
             */
            void paste(String data, int index = 0){
				for(int i = index; i < index + data.size; i++){
					if(i == this->size){
						this->data[i] = '\0';
						break;
					}
                    this->data[i] = data.data[i - index];
                }
            }
            //! массив данных строки
            char* data;
            //! длина массива данных
            int size;
        };
    }

}



#endif // SIMPLESTRING_H
