#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QList>
#include <iostream>

//! \brief Определение списка, который мы будем далее использовать (может быть заменен с QList на самописный или библиотечный список)
#define MafiaList QList

namespace Mafia{
    namespace System{
        //! \brief Класс (по сути статический) для работы с указателями и отслеживания их создания и удаления
        class PointerManager{
        public:
            //! \brief Функция, проверяющая был ли выделен данный указатель
            //! \return true, если указатель был выделен и не был освобожден, иначе - false
            static bool hasPtr(void* pointer);
            //! \brief Функция добавления указателя в список при его выделении
            static void addPtr(void* pointer);
            //! \brief Функция удаления указателя из списка при его освобождении
            static void removePtr(void* pointer);
        private:
            //! \brief Список всех выделенных и не освобожденных на данный момент
            static MafiaList<void*> allocatedPointers;
        };
    }
}
//! \brief Проверка на то, какой из режимов используется в данный момент (debug или release)
#ifndef DEBUG_MODE

//! \brief Утверждение
//! \param condition Условие, которое должно быть верно
//! \param message Сообщение, которое будет выведено в консоль, если утверждение неверно
#define ASSERT(condition, message)  {\
                                        if(!(condition))\
                                        {\
                                            std::cout << "Assertion failed! \n[" << #condition << "]\n" << message;\
                                        }\
                                    }

//! \brief Безопасное выделение указателя (выделяет указатель и запоминает, что он был выделен)
//! Способ применения:
//! Вместо MyClass* a = new MyClass(someVar);
//! Используем SAFE_NEW(MyClass*, a, MyClass(someVar));
//! \param type Тип указателя (например, int*)
//! \param variable Имя переменной, которую вы выделяете
//! \param obj Способ создания объекта (например, MyClass(someVar))
#define SAFE_NEW(variable, obj) {\
									variable = new obj;\
									Mafia::System::PointerManager::addPtr((void*)variable);\
                                 }

//! \brief Безопасное удаление указателя (если указатель не был выделен, выдаст ошибку) (не используйте для массивов)
//! SAFE_NEW(int*, a, int());
//! ...some code with a...
//! SAFE_DELETE(a);
//! \param variable Переменная, которую следует освободить
#define SAFE_DELETE(variable) {\
							ASSERT(Mafia::System::PointerManager::hasPtr((void*)variable), "Pointer was not allocated!!!");\
							Mafia::System::PointerManager::removePtr((void*)variable);\
                            delete variable;\
							variable = 0;\
                         }

//! \brief То же, что и \ref SAFE_DELETE(variable), только для массивов
//! SAFE_NEW(int*, b, int[179]);
//! ...some code with b...
//! SAFE_DELETE_ARRAY(b);
#define SAFE_DELETE_ARRAY(variable) {\
                            ASSERT(PointerManager::hasPtr((void*)variable), "Pointer was not allocated!!!");\
							Mafia::System::PointerManager::removePtr((void*)variable);\
                            delete[] variable;\
							variable = 0;\
                         }
#else

//! \brief Утверждение
//! \param condition Условие, которое должно быть верно
//! \param message Сообщение, которое будет выведено в консоль, если утверждение неверно
#define ASSERT(condition, message)  {\
                                        if(!(condition))\
                                        {\
                                            std::cout << "Assertion failed! \n[" << #condition << "]\n" << message;\
                                            std::exit();\
                                        }\
                                    }

//! \brief Безопасное выделение указателя (выделяет указатель и запоминает, что он был выделен)
//! \param type Тип указателя (например, int*)
//! \param variable Имя переменной, которую вы выделяете
//! \param obj Способ создания объекта (например, MyClass(someVar))
//! \example Итак, способ применения:
//! Вместо MyClass* a = new MyClass(someVar);
//! Используем SAFE_NEW(MyClass*, a, MyClass(someVar));
#define SAFE_NEW(type, variable, obj) {\
                                    type variable = new obj;\
									Mafia::System::PointerManager::addPtr((void*)variable);\
                                 }

//! \brief Безопасное удаление указателя (если указатель не был выделен, выдаст ошибку) (не используйте для массивов)
//! \param variable Переменная, которую следует освободить
//! \example SAFE_NEW(int*, a, int());
//! ...some code with a...
//! SAFE_DELETE(a);
#define SAFE_DELETE(variable) {\
                            ASSERT(PointerManager::hasPtr((void*)variable), "Pointer was not allocated!!!");\
							Mafia::System::PointerManager::removePtr((void*)variable);\
                            delete variable;\
                         }

//! \brief То же, что и \ref SAFE_DELETE(variable), только для массивов
//! \example SAFE_NEW(int*, b, int[179]);
//! ...some code with b...
//! SAFE_DELETE_ARRAY(b);
#define SAFE_DELETE_ARRAY(variable) {\
                            ASSERT(PointerManager::hasPtr((void*)variable), "Pointer was not allocated!!!");\
							Mafia::System::PointerManager::removePtr((void*)variable);\
                            delete[] variable;\
                         }
#endif


#endif // FUNCTIONS_H
