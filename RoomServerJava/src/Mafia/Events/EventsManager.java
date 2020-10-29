package Mafia.Events;

import java.lang.ref.WeakReference;
import java.util.Collection;
import java.util.HashSet;

/*! \brief Класс, управляющий системой событий. Отвечает за вызов соответствующих методов на слушателях и регистрацию событий */
public class EventsManager {
    static {
        //Создаем статические объекты (в данном случае лишь реконструктор)
        EventsManagerReconstructor r = new EventsManagerReconstructor();
    }

    //Храним слабые ссылки на слушателей, чтобы в случае удаления слушателя, GC мог спокойно забирать объект и не переживать насчет этих ссылок
    private static Collection<WeakReference<MafiaEventListener>> listeners = new HashSet<WeakReference<MafiaEventListener>>();

    /*! \brief Основная функция регистрации события. Вызывается объектами событий.
        \param event Событие, которое вызывает функцию
     */
    public static void register_event(MafiaEvent event){
        //System.out.println("Event registered");
        for (WeakReference<MafiaEventListener> listener : listeners){
            if(listener != null){
                listener.get().on_event(event);
            }
        }
    }

    /*! \brief Функция добавления нового слушателя
        \param listener слушатель, которого мы добавляем
     */
    public static void add_listener(MafiaEventListener listener){
        listeners.add(new WeakReference<MafiaEventListener>(listener));
    }

    /*! \brief Функция, которая удаляет пустые ссылки из множества слушателей.
        Т.к. мы храним слабые ссылки, при удалении объекта ссылка на него сохраняется, поэтому, чтобы множество слушателей
        не разрасталось до бесконечности, иногда проводится чистка, при которой удаляются все "мертвые" ссылки

     */
    public static void restructure(){
        Collection<WeakReference<MafiaEventListener>> n_listeners = new HashSet<WeakReference<MafiaEventListener>>();
        for(WeakReference<MafiaEventListener> listener : listeners){
            if(listener.get() != null){
                n_listeners.add(listener);
            }
        }

        listeners = n_listeners;
    }
}
