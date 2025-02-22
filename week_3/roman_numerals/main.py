from roman import Roman

# Создаем объекты Roman
r1 = Roman("X")  # 10
r2 = Roman("V")  # 5
r3 = Roman(3)    # 3

# Выполняем операции
print(r1 + r2)  # Сложение: X + V = XV (15)
print(r1 - r2)  # Вычитание: X - V = V (5)
print(r1 * r2)  # Умножение: X * V = L (50)
print(r1 / r2)  # Деление: X / V = II (2)
print(r3)       # Вывод: III