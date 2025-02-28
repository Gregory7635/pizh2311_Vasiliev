import json
from datetime import datetime


class DateTime:
    """
    Класс для работы с датой и временем.
    Поддерживает создание объектов из строк, сохранение и загрузку в/из JSON-файлов,
    а также методы для проверки високосного года, получения дня недели и времени.
    """

    def __init__(self, day: int, month: int, year: int, hour: int = 0, minute: int = 0, second: int = 0):
        """
        Инициализация объекта DateTime.

        :param day: День месяца (1-31)
        :param month: Месяц (1-12)
        :param year: Год (например, 2023)
        :param hour: Час (0-23)
        :param minute: Минута (0-59)
        :param second: Секунда (0-59)
        """
        self.day = day
        self.month = month
        self.year = year
        self.hour = hour
        self.minute = minute
        self.second = second

    def __str__(self) -> str:
        """
        Возвращает строковое представление объекта DateTime в формате 'DD-MM-YYYY HH:MM:SS'.

        :return: Строка с датой и временем
        """
        return f"{self.day:02d}-{self.month:02d}-{self.year} {self.hour:02d}:{self.minute:02d}:{self.second:02d}"

    @classmethod
    def from_string(cls, str_value: str) -> 'DateTime':
        """
        Создает объект DateTime из строки в формате 'DD-MM-YYYY HH:MM:SS'.

        :param str_value: Строка с датой и временем
        :return: Объект DateTime
        :raises ValueError: Если строка имеет неверный формат
        """
        try:
            dt = datetime.strptime(str_value, "%d-%m-%Y %H:%M:%S")
            return cls(dt.day, dt.month, dt.year, dt.hour, dt.minute, dt.second)
        except ValueError:
            raise ValueError("Invalid date string format. Expected 'DD-MM-YYYY HH:MM:SS'")

    def save(self, filename: str) -> None:
        """
        Сохраняет объект DateTime в JSON-файл.

        :param filename: Имя файла для сохранения
        """
        data = {
            "day": self.day,
            "month": self.month,
            "year": self.year,
            "hour": self.hour,
            "minute": self.minute,
            "second": self.second
        }
        try:
            with open(filename, 'w') as file:
                json.dump(data, file)
            print(f"Файл {filename} успешно создан.")
        except Exception as e:
            print(f"Ошибка при сохранении файла: {e}")

    @classmethod
    def load(cls, filename: str) -> 'DateTime':
        """
        Загружает объект DateTime из JSON-файла.

        :param filename: Имя файла для загрузки
        :return: Объект DateTime
        """
        with open(filename, 'r') as file:
            data: Dict[str, Any] = json.load(file)
        return cls(data["day"], data["month"], data["year"], data["hour"], data["minute"], data["second"])

    def is_leap_year(self) -> bool:
        """
        Проверяет, является ли год високосным.

        :return: True, если год високосный, иначе False
        """
        return self.year % 4 == 0 and (self.year % 100 != 0 or self.year % 400 == 0)

    def get_weekday(self) -> str:
        """
        Возвращает день недели для текущей даты.

        :return: Название дня недели (например, "Monday")
        """
        dt = datetime(self.year, self.month, self.day)
        return dt.strftime("%A")

    def get_time(self) -> str:
        """
        Возвращает время в формате 'HH:MM:SS'.

        :return: Строка с временем
        """
        return f"{self.hour:02d}:{self.minute:02d}:{self.second:02d}"