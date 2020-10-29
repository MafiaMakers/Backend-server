package Mafia.Events;

import java.util.concurrent.ExecutionException;

//! \brief Класс-таймер для менеджера событий. Следит за тем, когда стоит проводить чистку мертвых ссылок. Работает во втором потоке
public class EventsManagerReconstructor extends Thread {
    //! \brief Время в мс, через которое производится чистка
    private static int delay = 5000;

    //! \brief Переменная для остановки потока. Класс совершает работу только пока running == true
    private boolean running = true;

    //! \brief Пустой основной конструктор. Запускает параллельную работу.
    public EventsManagerReconstructor(){
        start();
    }

    //! \brief Функция остановки работы
    public void terminate(){
        running = false;
        stop();
    }


    //! \brief Функция, наследованная от Thread. Основной процесс параллельной работы.
    @Override
    public void run(){
        while (running) {
            try {
                sleep(delay);
            } catch (Exception ex){
                System.out.println(ex.getMessage());
            }
            EventsManager.restructure();
        }
    }
}
