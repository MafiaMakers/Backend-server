#ifndef REQUEST_H
#define REQUEST_H
#include <QObject>
namespace Mafia {
    /*!
     * \brief Базовый класс запроса. От него будут наследоваться классы сетевого запроса, запроса БД, запроса API субсерверов и т.п.
     */
    class Request : public QObject
    {
        Q_OBJECT
    public:
        /*!
         * \brief Пустой конструктор
         */
        Request();
        /*!
         * \brief Функция проверки, готов ли сейчас запрос выдать результат
         * \return true, если готов и false, если результата запроса еще нет
         */
        virtual bool is_finished();
        /*!
         * \brief Функция, которая ждет в том же потоке, откуда ее вызвали когда данные будут получены.
         * Внимательно работайте с ней, так как она может повесить всю систему
         */
        virtual void sleep_untill_finished();
        /*! \brief Функция, возвращающая значение, когда получены данные.
         * Она тоже ждет в том же потоке, где ее вызвали, так что будьте с ней осторожны
         */
        template <class T>
        T get_when_finished(){ return T();}
    private slots:
        /*!
         * \brief Слот для получения ответа на запрос, который может подходить
         */
        virtual void tryClose();
    protected:
        /*!
         * \brief Функция завершения работы запроса по получении данных
         */
        virtual void close_request();
        //! Отвечает на вопрос: готовы ли результаты запроса?
        bool finished = false;
    };
}


#endif // REQUEST_H
