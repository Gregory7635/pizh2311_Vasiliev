# Работа с датой и временем через класс DateTime

Класс `DateTime` предоставляет функционал для работы с датой и временем, включая сериализацию в JSON, проверку високосного года и получение дня недели. Файл json будет создаваться в определенной директории, которую надо необхожимо узнать при помощи os.getcwd(). json который считывает программа необходимо положить в ту же директорию

## Основные методы класса

```python
class DateTime:
    def __init__(self, day: int, month: int, year: int, hour: int = 0, minute: int = 0, second: int = 0):
        # Инициализация объекта даты и времени
        pass

    def __str__(self) -> str:
        # Возвращает дату в формате "DD-MM-YYYY HH:MM:SS"
        pass

    @classmethod
    def from_string(cls, str_value: str) -> 'DateTime':
        # Создает объект из строки формата "DD-MM-YYYY HH:MM:SS"
        pass

    def save(self, filename: str) -> None:
        # Сохраняет объект в JSON-файл
        pass

    @classmethod
    def load(cls, filename: str) -> 'DateTime':
        # Загружает объект из JSON-файла
        pass

    def is_leap_year(self) -> bool:
        # Проверяет, является ли год високосным
        pass

    def get_weekday(self) -> str:
        # Возвращает название дня недели (например, "Monday")
        pass

    def get_time(self) -> str:
        # Возвращает время в формате "HH:MM:SS"
        pass