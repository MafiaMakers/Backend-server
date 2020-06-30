#ifndef TUPLE_H
#define TUPLE_H
namespace Mafia {
    namespace System {
        template<class T1, class T2>
        //! \brief Структура для объединения двух объектов в один
        struct Tuple{
          //! \brief Первый объект
          T1 item1;
          //! \brief Второй объект
          T2 item2;

          /*!
           * \brief Пустой конструктор !ТРЕБУЕТ НАЛИЧИЯ ПУСТЫХ КОНСТРУКТОРОВ У ОБОИХ ТИПОВ ОБЪЕКТОВ!
           */
          Tuple(){
              item1 = T1();
              item2 = T2();
          }

          /*!
           * \brief Основной конструктор
           * \param item1 Первый объект
           * \param item2 Второй объект
           */
          Tuple(T1 item1, T2 item2){
              this->item1 = item1;
              this->item2 = item2;
          }

          /*!
           * \brief Оператор сравнения !ТРЕБУЕТ ОПРЕДЕЛЕНИЯ ОПЕРАТОРОВ СРАВНЕНИЯ У ОБОИХ ТИПОВ ОБЪЕКТОВ!
           * \param a Объект, с которым сравниваем
           * \return true, если объекты равны, иначе - false.
           */
          bool operator == (Tuple<T1, T2> a){
              return (a.item1 == this->item1) && (a.item2 == this->item2);
          }

          /*!
           * \brief Обратный оператор сравнения !ТРЕБУЕТ ОПРЕДЕЛЕНИЯ ОПЕРАТОРОВ СРАВНЕНИЯ У ОБОИХ ТИПОВ ОБЪЕКТОВ!
           * \param a Объект, с которым сравниваем
           * \return true, если объесты отличаются хоть в чем-то, иначе - false
           */
          bool operator != (Tuple<T1, T2> a){
            return !(*this == a);
          }
        };
    }
}

#endif // TUPLE_H
