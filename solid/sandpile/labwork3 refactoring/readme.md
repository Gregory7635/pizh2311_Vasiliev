## Отчет по рефакторингу Sandpile с применением SOLID

# Цель
Рефакторинг программы моделирования песчаной кучи с целью улучшения архитектуры согласно принципам **SOLID**.


### **O — Open/Closed Principle (Принцип открытости/закрытости)**

**До:**

* Для изменения поведения топплинга нужно было править глобальную функцию `topple()`.

**После:**

* Введён интерфейс `ISandTopplingStrategy`, и конкретная реализация передаётся через `std::unique_ptr`.

```cpp
std::unique_ptr<ISandTopplingStrategy> strategy = std::make_unique<ClassicToppling>();
```

> Теперь можно добавить, например, `DiagonalToppling`, не трогая остальной код.

---

### **L — Liskov Substitution Principle (Принцип подстановки Лисков)**
**концептуальная реализация**, код готов к добавлению новых классов через абстрактный класс, не изменяя логику.

* `ClassicToppling` реализует `ISandTopplingStrategy` и может быть подставлена вместо любой другой реализации, не нарушая поведение системы.

```cpp
std::unique_ptr<ISandTopplingStrategy> strategy = std::make_unique<ClassicToppling>();
```

> В будущем можно заменить на `strategy = std::make_unique<DiagonalicToppling>();` — логика сохранится. Принцип соблюдён.

---


### **D — Dependency Inversion Principle (Принцип инверсии зависимостей)**

**До:**

* `main()` напрямую вызывал функцию `topple()` и зависел от её реализации.

```cpp
topple(grid);
```

**После:**

* Зависимость инвертирована: `main()` работает с интерфейсом `ISandTopplingStrategy`.

```cpp
std::unique_ptr<ISandTopplingStrategy> strategy = std::make_unique<ClassicToppling>();
strategy->topple(grid);
```

> `main()` теперь зависит от абстракции, а не от конкретной реализации.

