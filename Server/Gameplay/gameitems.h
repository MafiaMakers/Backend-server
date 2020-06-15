#ifndef GAMEITEMS_H
#define GAMEITEMS_H
namespace Mafia {
    //! \brief Пространство имен для работы с процессом игры и игровыми элементами
    namespace Gameplay {

        //! \brief Возможные роли
        enum Role{
            //! \brief Индекс первой роли (см. \ref Mafia::Database::Status_First)
            Role_First = 1,
            //! \brief Жирный митель
            Role_Civilian = 1,
            //! \brief Мафия
            Role_Mafia = 2,
            //! \brief Отсутствие роли
            Role_None = 3,
            //! \brief Комиссар
            Role_Sheriff = 4,
            //! \brief Доктор
            Role_Doctor = 5,
            //! \brief Последний индекс роли (см. \ref Mafia::Database::Status_First)
            Role_Last = 6
        };

        //! \brief Возможные личные результаты игры
        enum GamePersonalResult{
            //! \brief Первый индекс личного результата (см. \ref Mafia::Database::Status_First)
            GamePersonalResult_First = 0,
            //! \brief Поражение
            GamePersonalResult_Defeat = 0,
            //! \brief Победа
            GamePersonalResult_Victory = 1,
            //! \brief Отсутствие результата
            GamePersonalResult_None = 2,
            //! \brief Последний индекс личного результата (см. \ref Mafia::Database::Status_First)
            GamePersonalResult_Last = 3
        };

        //! \brief Возможные глобальные результаты игры
        enum GameResult{
            //! \brief Первый индекс глобального результата (см. \ref Mafia::Database::Status_First)
            GameResult_First = 0,
            //! \brief Отсутствие результата игры (раннее завершение и т.п.)
            GameResult_None = 0,
            //! \brief Победа мафии
            GameResult_MafiaWins = 1,
            //! \brief Победа мирных
            GameResult_CiviliansWin = 2,
            //! \brief Последний индекс глобального результата (см. \ref Mafia::Database::Status_First)
            GameResult_Last = 3
        };
    }
}


#endif // GAMEITEMS_H
