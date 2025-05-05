## Добавление аргументов

Регистрация аргументов для парсинга:

    AddStringArgument()
    Добавляет строковый аргумент (--file, -f).

parser.AddStringArgument("--input").Default("config.txt");

AddIntArgument()
Добавляет целочисленный аргумент (--port, -p).

parser.AddIntArgument("--port").StoreValue(port);

AddFlag()
Добавляет флаг (--verbose, -v).

    parser.AddFlag('v', "verbose").StoreValue(is_verbose);

## Настройка поведения

Дополнительные правила для аргументов:

    .Default(value)
    Устанавливает значение по умолчанию, если аргумент не передан.
    

parser.AddStringArgument("--mode").Default("debug");

.StoreValue(var)
Привязывает аргумент к переменной в программе.


std::string filename;
parser.AddStringArgument("--file").StoreValue(filename);

.MultiValue()
Разрешает несколько значений для одного аргумента (например, --num 1 2 3).


parser.AddIntArgument("--values").MultiValue();

.Positional()
Помечает аргумент как позиционный (значение без ключа, например, ./app file.txt).


    parser.AddStringArgument("file").Positional();

## Парсинг аргументов

Основной метод для обработки ввода:

    Parse(argc, argv)
    Анализирует аргументы командной строки. Возвращает false при ошибке.
    

    if (!parser.Parse(argc, argv)) {
        std::cerr << "Ошибка в аргументах!\n";
    }

## Получение значений

Доступ к результатам парсинга:

    GetStringValue(name)
    Возвращает строковое значение аргумента.
    

std::string mode = parser.GetStringValue("--mode");

GetIntValue(name)
Возвращает целочисленное значение.


int port = parser.GetIntValue("--port");

GetFlag(name)
Проверяет, передан ли флаг.


    if (parser.GetFlag("--verbose")) { ... }

## Работа со справкой

Генерация помощи:

    AddHelp()
    Добавляет аргумент для вывода справки (--help, -h).
    
parser.AddHelp('h', "help", "Показать справку");

HelpDescription()
Возвращает форматированное описание всех аргументов.


if (parser.Help()) {
    std::cout << parser.HelpDescription();
}