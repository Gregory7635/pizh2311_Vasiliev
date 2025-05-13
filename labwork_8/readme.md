# Лабораторная работа №8 — Бинарное дерево поиска и обходы

##  Описание

Данная лабораторная работа реализует **бинарное дерево поиска (BST)** как STL-совместимый контейнер, поддерживающий различные типы обхода дерева (in-order, pre-order, post-order) через итераторы.  
Контейнер шаблонизирован и соответствует требованиям стандартных контейнеров C++:
- Container
- AssociativeContainer
- ReversibleContainer
- AllocatorAwareContainer
- Использует BidirectionalIterator

Контейнер поддерживает алфавитную сортировку элементов на основе заданного компаратора.

---

##  Особенности реализации

- Использован **Tag Dispatch Idiom** для выбора типа обхода.
- Написаны три разных итератора для:
  - **InOrder** (по возрастанию)
  - **PreOrder** (обход в глубину слева направо)
  - **PostOrder** (обход снизу вверх)

---

##  Проверка на данных о песнях

В качестве примера используется структура `Song`, содержащая:
- Название песни (`title`)
- Исполнитель (`artist`)
- Альбом (`album`)
- Год выхода (`year`)

Элементы вставляются в дерево и сортируются по алфавиту названий песен.

Пример данных 
```cpp
std::vector<Song> songs = {
    {"Moonlight Sonata", "Beethoven", "Classics", 1801},
    {"Bohemian Rhapsody", "Queen", "A Night at the Opera", 1975},
    {"Imagine", "John Lennon", "Imagine", 1971},
    {"Billie Jean", "Michael Jackson", "Thriller", 1982},
    {"Clocks", "Coldplay", "A Rush of Blood to the Head", 2002}
};
