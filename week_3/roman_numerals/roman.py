class Roman:
    """
    Класс Roman представляет римское число и поддерживает основные арифметические операции:
    сложение (+), вычитание (-), умножение (*) и деление (/).

    Атрибуты:
        value (int): Целое число, представляющее значение римского числа.

    Методы:
        __init__: Инициализирует объект Roman.
        __add__: Реализует операцию сложения.
        __sub__: Реализует операцию вычитания.
        __mul__: Реализует операцию умножения.
        __truediv__: Реализует операцию деления.
        __str__: Возвращает строковое представление римского числа.
        roman_to_int: Преобразует римское число (строку) в целое число.
        int_to_roman: Преобразует целое число в римское число (строку).
    """

    def __init__(self, value: str | int) -> None:
        """
        Инициализирует объект Roman.

        Параметры:
            value (str | int): Римское число в виде строки (например, "X") или целое число (например, 10).

        Исключения:
            ValueError: Если переданная строка не является допустимым римским числом.
        """
        if isinstance(value, str):
            self.value = self.roman_to_int(value)
        else:
            self.value = value

    def __add__(self, other: 'Roman | int') -> 'Roman':
        """
        Реализует операцию сложения.

        Параметры:
            other (Roman | int): Второе слагаемое (объект Roman или целое число).

        Возвращает:
            Roman: Новый объект Roman, представляющий сумму.
        """
        if isinstance(other, Roman):
            return Roman(self.value + other.value)
        else:
            return Roman(self.value + other)

    def __sub__(self, other: 'Roman | int') -> 'Roman':
        """
        Реализует операцию вычитания.

        Параметры:
            other (Roman | int): Вычитаемое (объект Roman или целое число).

        Возвращает:
            Roman: Новый объект Roman, представляющий разность.
        """
        if isinstance(other, Roman):
            return Roman(self.value - other.value)
        else:
            return Roman(self.value - other)

    def __mul__(self, other: 'Roman | int') -> 'Roman':
        """
        Реализует операцию умножения.

        Параметры:
            other (Roman | int): Второй множитель (объект Roman или целое число).

        Возвращает:
            Roman: Новый объект Roman, представляющий произведение.
        """
        if isinstance(other, Roman):
            return Roman(self.value * other.value)
        else:
            return Roman(self.value * other)

    def __truediv__(self, other: 'Roman | int') -> 'Roman':
        """
        Реализует операцию деления.

        Параметры:
            other (Roman | int): Делитель (объект Roman или целое число).

        Возвращает:
            Roman: Новый объект Roman, представляющий результат деления (целочисленное деление).
        """
        if isinstance(other, Roman):
            return Roman(self.value // other.value)
        else:
            return Roman(self.value // other)

    def __str__(self) -> str:
        """
        Возвращает строковое представление римского числа.

        Возвращает:
            str: Римское число в виде строки.
        """
        return self.int_to_roman(self.value)

    @staticmethod
    def roman_to_int(s: str) -> int:
        """
        Преобразует римское число (строку) в целое число.

        Параметры:
            s (str): Римское число в виде строки (например, "XIV").

        Возвращает:
            int: Целое число, соответствующее римскому числу.

        Исключения:
            ValueError: Если строка содержит недопустимые символы.
        """
        roman_dict = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
        total = 0
        prev_value = 0
        for char in reversed(s):
            if char not in roman_dict:
                raise ValueError(f"Недопустимый символ в римском числе: {char}")
            value = roman_dict[char]
            if value < prev_value:
                total -= value
            else:
                total += value
            prev_value = value
        return total

    @staticmethod
    def int_to_roman(num: int) -> str:
        """
        Преобразует целое число в римское число (строку).

        Параметры:
            num (int): Целое число для преобразования.

        Возвращает:
            str: Римское число в виде строки.

        Исключения:
            ValueError: Если число меньше 1 или больше 3999.
        """
        if not 1 <= num <= 3999:
            raise ValueError("Римские числа могут быть только в диапазоне от 1 до 3999.")

        val = [
            1000, 900, 500, 400,
            100, 90, 50, 40,
            10, 9, 5, 4,
            1
        ]
        syms = [
            "M", "CM", "D", "CD",
            "C", "XC", "L", "XL",
            "X", "IX", "V", "IV",
            "I"
        ]
        roman_num = ''
        i = 0
        while num > 0:
            for _ in range(num // val[i]):
                roman_num += syms[i]
                num -= val[i]
            i += 1
        return roman_num
