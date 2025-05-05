## Основные функции парсинга

    omfl::parse(const std::filesystem::path& path)
    Загружает и парсит файл OMFL, возвращает объект OMFL.

    omfl::parse(const std::string& str)
    Парсит строку в формате OMFL, возвращает объект OMFL.

## Валидация и обработка данных

    KeyWork(std::string_view& line)
    Обрабатывает строку вида key = value, проверяет синтаксис и возвращает Unit с данными.

    FillUnit(Unit& unit, std::string_view& line, ...)
    Рекурсивно заполняет Unit в зависимости от типа данных (число, строка, массив и т. д.).

    CheckNameS(const std::string_view& line)
    Проверяет корректность имени ключа или секции (допустимые символы: буквы, цифры, _, -).

    DeleteTrash(std::string_view& line)
    Удаляет комментарии и лишние пробелы из строки.

## Работа с секциями и данными

    Section::PlaceUnit
    Рекурсивно размещает значение (Unit) в структуре секций по пути вида section.subsection.key.

    Section::Get
    Получает секцию или значение по пути (например, common.name).

## Проверка типов данных

    Unit::IsInt(), IsFloat(), IsString(), IsBool(), IsArray()
    Проверяют тип данных в Unit.

    AsInt(), AsFloat(), AsString(), AsBool()
    Возвращают данные в соответствующем типе.

## Экспорт в другие форматы

    ToXml(const std::string& name)
    Сохраняет данные в XML-файл.

    ToJson(const std::string& name)
    Сохраняет данные в JSON-файл.

    ToYaml(const std::string& name)
    Сохраняет данные в YAML-файл.

## Вспомогательные функции

    BoolIntNumber и BoolFloatNumber
    Проверяют, является ли строка целым или дробным числом.

    Tabulation
    Генерирует строку с отступами для форматированного вывода (XML/JSON/YAML).
