
"""Модуль для расчета площади стен комнаты с учетом окон и дверей, а также 
количества рулонов обоев, необходимых для оклеивания.

Классы:
- WinDoor: Представляет собой окно или дверь с вычисляемой площадью.
- Room: Представляет собой комнату и позволяет рассчитать рабочую площадь стен, 
учитывая окна и двери, а также определить необходимое количество рулонов обоев."""


import math


class WinDoor:
    """
    Класс для представления окна или двери.
    """
    def __init__(self, x: float, y: float):
        """
        Инициализация экземпляра окна или двери.
        """
        self.square = x * y


class Room:
    """
    Класс для расчета площади стен комнаты и количества рулонов обоев.
    
    Атрибуты:
    - length (float): Длина комнаты.
    - width (float): Ширина комнаты.
    - height (float): Высота комнаты.
    - wd (list): Список объектов WinDoor (окон и дверей).
    """
    def __init__(self, x: float, y: float, z: float):
        """
        Инициализация экземпляра комнаты.
        """
        self.length = x
        self.width = y
        self.height = z
        self.wd = []

    def _defaultSurface(self) -> float:
        """
        Вычисляет общую площадь стен комнаты без учета окон и дверей.
        
        Возвращает:
        - float: Площадь стен комнаты.
        """
        return 2 * self.height * (self.length + self.width)

    def _addWD(self, w: float, h: float):
        """
        Добавляет окно или дверь в список.
        """
        self.wd.append(WinDoor(w, h))

    def workSurface(self) -> float:
        """
        Вычисляет рабочую площадь стен (без учета окон и дверей).
        """
        new_square = self._defaultSurface()
        for i in self.wd:
            new_square -= i.square
        return new_square
    
    def workspaceCalculation(self):
        """
        Запрашивает у пользователя размеры окна/двери и добавляет их в расчет.
        """
        print("Введите размеры окна/двери (ширина и высота):")  
        g = float(input("Ширина: "))   
        u = float(input("Высота: "))   
        self._addWD(g, u)

    def requiredRolls(self, l: float, w: float) -> int:
        """
        Рассчитывает количество рулонов обоев, необходимых для оклеивания комнаты.
        
        Параметры:
        - l (float): Длина рулона обоев.
        - w (float): Ширина рулона обоев.
        
        Возвращает:
        - int: Округленное вверх количество рулонов.
        """
        roll_area = l * w  
        remaining_surface = self.workSurface()  
        return math.ceil(remaining_surface / roll_area)
