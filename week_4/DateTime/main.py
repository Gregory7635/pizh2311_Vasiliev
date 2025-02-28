from datetime_class import DateTime
import os

# Создание объекта DateTime
dt1 = DateTime(2, 3, 2025, 14, 30, 45)
print(dt1)

# Создание объекта из строки
dt2 = DateTime.from_string("06-03-2025 15:45:30")
print(dt2)

# Сохранение объекта в JSON-файл
dt2.save("datetime.json")

# Загрузка объекта из файла
dt5 = DateTime.load("dt1.json")
print(f" {dt5}")

# Проверка на високосный год
print(f"Високосный: {dt1.is_leap_year()}")

# Получение дня недели
print(f"Weekday: {dt1.get_weekday()}")

# Получение времени
print(f"Time: {dt1.get_time()}")

# Получение текущей рабочей директории
print(os.getcwd())
