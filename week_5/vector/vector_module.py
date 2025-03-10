import json
from typing import List

class Vector:
    """
    Класс, представляющий вектор в двумерном пространстве.

    Атрибуты:
        x (float): Координата x вектора.
        y (float): Координата y вектора.
    """

    def __init__(self, x: float, y: float):
        """
        Инициализация вектора с координатами x и y.

        Аргументы:
            x (float): Координата x.
            y (float): Координата y.
        """
        self.x = x
        self.y = y

    def __str__(self) -> str:
        """
        Возвращает строковое представление вектора.

        Возвращает:
            str: Строка в формате "Vector(x, y)".
        """
        return f"Vector({self.x}, {self.y})"

class VectorCollection:
    """
    Класс-контейнер для хранения и управления набором объектов Vector.

    Атрибуты:
        _data (List[Vector]): Список объектов Vector.
    """

    def __init__(self):
        """Инициализация пустого контейнера."""
        self._data: List[Vector] = []

    def __str__(self) -> str:
        """
        Возвращает строковое представление контейнера.

        Возвращает:
            str: Строка, содержащая строковые представления всех векторов в контейнере.
        """
        return "\n".join(str(item) for item in self._data)

    def __getitem__(self, index: int) -> Vector:
        """
        Возвращает вектор по индексу.

        Аргументы:
            index (int): Индекс вектора.

        Возвращает:
            Vector: Вектор по указанному индексу.
        """
        return self._data[index]

    def add(self, value: Vector) -> None:
        """
        Добавляет вектор в контейнер.

        Аргументы:
            value (Vector): Вектор для добавления.
        """
        self._data.append(value)

    def remove(self, index: int) -> None:
        """
        Удаляет вектор из контейнера по индексу.

        Аргументы:
            index (int): Индекс вектора для удаления.

        Исключения:
            IndexError: Если индекс выходит за пределы диапазона.
        """
        if 0 <= index < len(self._data):
            self._data.pop(index)
        else:
            raise IndexError("Index out of range")

    def save(self, filename: str) -> None:
        """
        Сохраняет контейнер в JSON-файл.

        Аргументы:
            filename (str): Имя файла для сохранения.
        """
        with open(filename, 'w') as f:
            json.dump([{'x': v.x, 'y': v.y} for v in self._data], f)

    def load(self, filename: str) -> None:
        """
        Загружает контейнер из JSON-файла.

        Аргументы:
            filename (str): Имя файла для загрузки.
        """
        with open(filename, 'r') as f:
            data = json.load(f)
            self._data = [Vector(item['x'], item['y']) for item in data]