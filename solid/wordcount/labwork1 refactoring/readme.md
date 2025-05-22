## Отчет по рефакторингу WordCount с применением SOLID

# Цель
Рефакторинг программы wordcount.cpp с целью улучшения архитектуры согласно принципам SOLID. 

*S — Single Responsibility Principle (Принцип единственной ответственности)*

До:

* Функция main делает всё:

* Обрабатывает аргументы командной строки

* Открывает файлы

* Считает статистику

* Выводит результаты

**Пример**:

``` cpp

int main(int argc, char** argv) {
    // ...
    FileStats stats = CountFileStats(filename);
    PrintStats(stats, show_lines, show_words, show_bytes, show_chars, filenames.size() > 1);
    // ...
}
```

**Нарушение:** слишком много обязанностей в одной функции.

После:

* Каждая ответственность вынесена в отдельный класс:

* parseArgs — парсинг аргументов

* FileReader — чтение файла

* FileStatsCounter — подсчет статистики

* ConsoleFormatter — форматирование вывода

* ConsolePrinter — печать результата

```cpp
ProgramOptions opts = parseArgs(argc, argv, helpMsg);
auto content = reader->read(file);
FileStats stats = counter->count(content, file);
std::string outStr = formatter->format(stats, ...);
printer->print(outStr);
```
> Теперь каждый класс отвечает за одну вещь.

*O — Open/Closed Principle (Принцип открытости/закрытости)*

До:

Если бы мы захотели, например, заменить PrintStats на вывод в файл — пришлось бы менять main.

``` cpp 
void PrintStats(...) {
    std::cout << ...;
}
```
После:

Вывод реализован через интерфейс IPrinter. Мы можем добавить FilePrinter, не меняя main.

``` cpp
class IPrinter {
public:
    virtual void print(const std::string& output) = 0;
};

class FilePrinter : public IPrinter {
    public:
    void print(const std::string& output) override {
        std::ofstream("output.txt", std::ios::app) << output;
    }
};
```
**Код открыт для расширения (добавление новых реализаций), но закрыт для изменений.**

*L — Liskov Substitution Principle (Принцип подстановки Барбары Лисков)*

добавлены два класса, которые наследуются от интерфейса  **IPrinter**
```cpp
class ConsolePrinter : public IPrinter {
public:
    void print(const std::string& output) override {
        std::cout << output << std::endl;
    }
};

class FilePrinter : public IPrinter {
public:
    void print(const std::string& output) override {
        std::ofstream("output.txt", std::ios::app) << output;
    }
};
```
Это позволяет использовать объекты этих классов через базовый интерфейс без нарушения логики — то есть, они взаимозаменяемы.
А значит, соблюден принцип подстановки Лисков (LSP).
```cpp
std::unique_ptr< IPrinter> printer = std::make_unique<ConsolePrinter>(); 
```
можем заменить на 
```cpp
std::unique_ptr< IPrinter> printer = std::make_unique<FilePrinter>(); 
```

**Все реализации интерфейсов могут быть взаимозаменяемыми без поломки логики.**

*I — Interface Segregation Principle (Принцип разделения интерфейса)*
До:

в изначальном коде не было интерфейсов — всё реализовано напрямую через глобальные функции (PrintStats, CountFileStats и т.д.).

После:

Каждый интерфейс узко специализирован:

```cpp
class IFileReader {
    virtual std::vector<char> read(const std::string& path) = 0;
};

class IStatCounter {
    virtual FileStats count(const std::vector<char>& content, const std::string& filename) = 0;
};

class IFormatter {
    virtual std::string format(const FileStats& stats, ...) = 0;
};
...
```
**Каждый модуль зависит только от того, что ему действительно нужно.**

*D — Dependency Inversion Principle (Принцип инверсии зависимостей)*

До:

main зависел от конкретных функций PrintStats, CountFileStats и т.д.

```cpp
FileStats stats = CountFileStats(filename);
PrintStats(stats, ...);
```
После:

main зависит от абстракций (интерфейсов), а конкретные реализации создаются извне:

```cpp
std::unique_ptr<IFileReader>    reader = std::make_unique<FileReader>();
std::unique_ptr<IStatCounter>   counter = std::make_unique<FileStatsCounter>();
std::unique_ptr<IFormatter>     formatter = std::make_unique<ConsoleFormatter>();
std::unique_ptr<IPrinter>       printer = std::make_unique<ConsolePrinter>();
```
**Высокоуровневый модуль зависит от абстракций, а не от деталей реализации.**