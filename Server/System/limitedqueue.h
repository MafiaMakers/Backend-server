#ifndef LIMITEDQUEUE_H
#define LIMITEDQUEUE_H
#include <iostream>
namespace Mafia {

    template <class T>
    /*! Структура элемента ограниченной очереди
     * ДА КОЛЬЦЕВОЙ БУФЕР И ЧТО
     */
    struct QueueElement{
        //! Значение элемента очереди
        T value;
        //! Указатель на следующий элемент очереди
        QueueElement* next;
        //! Указатель на предыдущий элемент очереди
        QueueElement* previous;
    };


    template <class T>
    /*! \brief Класс ограниченной очереди. Это очередь, в которой может храниться не более определенного количества элементов.
     * При достижении максимума добавление нового элемента будет удалять самый старый элемент из находящихся в очереди
     */
    class LimitedQueue
    {
    public:
        /*!
         * \brief Единственный конструктор
         * \param limit Максимальное количество элементов в очереди
         */
        LimitedQueue(int limit) : elementsCountLimit(limit){
            first = 0;
            last = 0;
            elementsCount = 0;
        }

        /*!
         * \brief Функция, проверяющая содержится ли в очереди данный элемент
         * \param value Элемент для проверки
         * \return true, если содержится и false, если нет
         */
        bool contains(T value){
            QueueElement<T>* current = first;
            for(int i = 0; i < elementsCount; i++){
                if(current->value == value){
                    return true;
                }
                current = current->next;
            }
            return false;
        }
        /*!
         * \brief Функция добавления в очередь элемента
         * \param value Элемент для добавления
         */
        void append(T value){
            QueueElement<T>* newElement = new QueueElement<T>();
            newElement->value = value;
            newElement->next = 0;
            newElement->previous = last;

            if(elementsCount == 0){
                newElement->previous = 0;
                first = newElement;
            }
            if(elementsCount != 0 && elementsCount < elementsCountLimit){
                last->next = newElement;
                newElement->previous = last;
            }
            if(elementsCount == elementsCountLimit){
                newElement->previous = last;
                last->next = newElement;
                QueueElement<T>* cur = first;
                first = first->next;
                delete cur;
                elementsCount--;
            }
            last = newElement;
            elementsCount++;
        }
    private:
        //! Указатель на первый элемент очереди. До появления такового равен нулю
        QueueElement<T>* first = 0;
        //! Указатель на последний элемент очереди. До появления такового равен нулю
        QueueElement<T>* last = 0;
        //! Текущее количество элементов в очереди
        int elementsCount = 0;
        //! Ограничение на количество элементов в очереди
        const int elementsCountLimit;
    };
}



#endif // LIMITEDQUEUE_H
