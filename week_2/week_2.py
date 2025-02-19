from abc import abstractmethod


# Абстрактный базовый класс (Абстракция)
class SnowEntity():
    """
    Абстрактный класс.
    Определяет общий интерфейс для всех снежных объектов.
    """

    @abstractmethod
    def display(self):
        """Абстрактный метод для отображения информации о сущности."""
        pass


# Класс SnowflakeCounter (Наследование от SnowEntity)
class SnowflakeCounter(SnowEntity):
    """
    Класс для подсчета снежинок. Позволяет выполнять арифметические операции
    с количеством снежинок и создавать визуальное представление снежинок.

    Атрибуты:
        __count (int): Количество снежинок.
    """

    def __init__(self, count):
        """
        Инициализирует объект SnowflakeCounter с заданным количеством снежинок.

        Параметры:
            count (int): Начальное количество снежинок.
        """
        self.__count = count  # Инкапсуляция: скрываем атрибут __count

    def __add__(self, other):
        """
        Перегрузка оператора сложения.

        Параметры:
            other (int или SnowflakeCounter): Значение или объект для сложения.

        Результат:
            SnowflakeCounter: Новый объект с обновленным количеством снежинок.
        """
        if isinstance(other, int):
            return SnowflakeCounter(self.__count + other)
        elif isinstance(other, SnowflakeCounter):
            return SnowflakeCounter(self.__count + other.__count)

    def __sub__(self, other):
        """
        Перегрузка оператора вычитания.

        Параметры:
            other (int или SnowflakeCounter): Значение или объект для вычитания.

        Результат:
            SnowflakeCounter: Новый объект с уменьшенным количеством снежинок.
        """
        if isinstance(other, int):
            return SnowflakeCounter(self.__count - other)
        elif isinstance(other, SnowflakeCounter):
            return SnowflakeCounter(self.__count - other.__count)

    def __mul__(self, other):
        """
        Перегрузка оператора умножения.

        Параметры:
            other (int или SnowflakeCounter): Значение или объект для умножения.

        Результат:
            SnowflakeCounter: Новый объект с увеличенным количеством снежинок.
        """
        if isinstance(other, int):
            return SnowflakeCounter(self.__count * other)
        elif isinstance(other, SnowflakeCounter):
            return SnowflakeCounter(self.__count * other.__count)

    def __truediv__(self, other):
        """
        Перегрузка оператора деления.

        Параметры:
            other (int или SnowflakeCounter): Значение или объект для деления.

        Результат:
            SnowflakeCounter: Новый объект с уменьшенным количеством снежинок (целочисленное деление).
        """
        if isinstance(other, int):
            return SnowflakeCounter(self.__count // other)
        elif isinstance(other, SnowflakeCounter):
            return SnowflakeCounter(self.__count // other.__count)

    def makeSnow(self, row):
        """
        Создает текстовое представление снежинок в виде строк.

        Параметры:
            row (int): Количество строк для отображения снежинок.

        Результат:
            str: Строка, содержащая снежинки, распределенные по строкам.
        """
        result = []
        base = self.__count // row
        extra = self.__count % row
        for i in range(row):
            if i < extra:
                result.append("*" * (base + 1))
            else:
                result.append("*" * base)
        return "\n".join(result)

    def get_count(self):
        """
        Возвращает текущее количество снежинок.

        Результат:
            int: Количество снежинок.
        """
        return self.__count

    def display(self):
        """
        Реализация абстрактного метода. Выводит количество снежинок.
        """
        print(f"SnowflakeCounter: {self.__count} snowflakes")

    def __call__(self):
        """
        Позволяет объекту SnowflakeCounter быть вызываемым как функция.

        Результат:
            int: Количество снежинок.
        """
        return self.__count


# Класс Snowman (Наследование от SnowEntity и Композиция)
class Snowman(SnowEntity):
    """
    Класс для создания снеговика. Использует снежинки из SnowflakeCounter.

    Атрибуты:
        __snowflake_counter (SnowflakeCounter): Объект для хранения снежинок.
    """

    def __init__(self, snowflake_counter):
        """
        Инициализирует объект Snowman с заданным количеством снежинок.

        Параметры:
            snowflake_counter (SnowflakeCounter): Объект счетчика снежинок.
        """
        self.__snowflake_counter = snowflake_counter  # Композиция: используем объект SnowflakeCounter

    def build(self):
        """
        Строит снеговика, используя 3 снежинки, если их достаточно.

        Результат:
            Выводит сообщение о постройке или нехватке снежинок.
        """
        if self.__snowflake_counter.get_count() >= 3:
            print("Building a snowman ")
            self.__snowflake_counter = self.__snowflake_counter - 3  # Используем 3 снежинки
        else:
            print("Not enough snowflakes to build a snowman")

    def display(self):
        """
        Реализация абстрактного метода. Выводит количество доступных снежинок для снеговика.
        """
        print(f"Snowman: {self.__snowflake_counter.get_count()} snowflakes available")


# Пример использования
snow1 = SnowflakeCounter(10)
snow2 = SnowflakeCounter(5)


result1 = snow1 + snow2
result2 = snow1 + 3
result1.display()
result2.display()
snowflakes = SnowflakeCounter(10)
snowflakes.display()

snowman = Snowman(snowflakes)
snowman.display()

snowman.build()
snowman.display()
print(snowflakes())
# Использование метода makeSnow
print(snowflakes.makeSnow(3))
