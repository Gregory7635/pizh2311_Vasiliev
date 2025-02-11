import math


class WinDoor:
    def __init__(self, x, y):
        self.square = x * y


class Room:
    def __init__(self, x, y, z):
        self.length = x
        self.width = y
        self.height = z
        self.wd = []

    def _defaultSurface(self):
        return 2 * self.height * (self.length + self.width)

    def _addWD(self, w, h):
        self.wd.append(WinDoor(w, h))

    def workSurface(self):
        new_square = self._defaultSurface()
        for i in self.wd:
            new_square -= i.square
        return new_square
    
    def workspaceCalculation(self):
          print("Введите размеры окна/двери (ширина и высота):")  
          g = float(input("Ширина: "))   
          u = float(input("Высота: "))   
          self._addWD(g, u)

    def requiredRolls(self, l, w):  
        roll_area = l * w  
        remaining_surface = self.workSurface()  
        return math.ceil(remaining_surface / roll_area)
    