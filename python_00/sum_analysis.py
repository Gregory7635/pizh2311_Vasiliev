# sum_analysis.py
"""Модуль для анализа сложности алгоритма суммирования."""

import timeit
import random
from typing import List, Callable
import matplotlib
matplotlib.use('Agg')  # Используем неинтерактивный бэкенд
import matplotlib.pyplot as plt


def read_numbers_from_file(filename: str) -> List[int]:
    """
    Читает целые числа из файла и возвращает их в виде списка.

    Аргументы:
        filename (str): Имя файла для чтения.

    Возвращает:
        List[int]: Список целых чисел из файла.

    Сложность: O(N), где N - количество чисел в файле.
    """
    numbers = []  # O(1) - инициализация списка
    try:
        with open(filename, 'r') as file:  # O(1) - открытие файла
            print(f"Содержимое файла '{filename}':")
            for line in file:  # O(N) - чтение каждой строки файла
                line = line.strip()  # O(1) - удаление пробельных символов
                print(f"  {line}")  # O(1) - вывод строки с отступом
                if line:  # O(1) - проверка на пустую строку
                    numbers.append(int(line))  # O(1) - преобразование
    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
    except ValueError as e:
        print(f"Ошибка: В файле содержатся некорректные данные. {e}")

    return numbers  # O(1) - возврат результата


def calculate_sum() -> None:
    """
    Считывает два целых числа из stdin и выводит их сумму.

    Сложность: O(1), так как количество операций не зависит от входных данных.
    Все операции (ввод, преобразование, сложение, вывод) являются константными.
    """
    a = int(input())  # O(1) - чтение одной строки и преобразование в int
    b = int(input())  # O(1)
    result = a + b    # O(1) - арифметическая операция
    print(result)     # O(1) - вывод одной строки


def sum_array(arr: List[int]) -> int:
    """
    Возвращает сумму всех элементов массива.

    Аргументы:
        arr (List[int]): Список целых чисел для суммирования.

    Возвращает:
        int: Сумма элементов массива.

    Сложность: O(N), где N - длина массива.
    """
    total = 0      # O(1) - инициализация переменной
    # O(N) - цикл по всем N элементам массива
    for num in arr:
        total += num  # O(1) - сложение и присваивание на каждой итерации
    return total   # O(1) - возврат результата
    # Общая сложность: O(1) + O(N) * O(1) + O(1) = O(N)


def measure_time(func: Callable[[List[int]], int], data: List[int]) -> float:
    """
    Измеряет среднее время выполнения функции в миллисекундах.

    Аргументы:
        func (Callable): Функция, время выполнения которой измеряется.
        data (List[int]): Данные для передачи в функцию.

    Возвращает:
        float: Среднее время выполнения в миллисекундах.
    """
    number_of_runs = 10  # Количество запусков для усреднения
    total_time = timeit.timeit(lambda: func(data), number=number_of_runs)
    # Конвертация в мс
    average_time_ms = (total_time / number_of_runs) * 1000
    return average_time_ms


def main() -> None:
    """Основная функция для проведения экспериментов и анализа."""
    # Характеристики ПК (ЗАПОЛНИТЕ СВОИМИ ДАННЫМИ!)
    pc_info = """
    Характеристики ПК для тестирования:
    - Процессор: Intel Core i7-8700 @ 3.6GHz
    - Оперативная память: 32 GB DDR4
    - OC: Windows 11
    - Python: 3.13.0
    """
    print(pc_info)

    # 1. Демонстрация работы базовой функции
    print("=== Базовая задача: суммирование двух чисел ===")

    # calculate_sum()  # Раскомментировать для интерактивного ввода

    # 1.1. Чтение чисел из файла
    print("\n=== Чтение чисел из файла ===")
    filename = "numbers.txt"  # Имя файла с числами
    # O(N) - чтение файла
    numbers_from_file = read_numbers_from_file(filename)

    if numbers_from_file:
        print(f"\nПрочитано чисел из файла: {len(numbers_from_file)}")
        if len(numbers_from_file) >= 2:
            # Вычисляем сумму первых двух чисел из файла
            file_sum = numbers_from_file[0] + numbers_from_file[1]  # O(1)
            print(f"Сумма первых двух чисел: {file_sum}")
        else:
            print("В файле < 2 чисел для вычисления суммы.")
    else:
        print("Не удалось прочитать числа из файла.")

    # 2. Анализ производительности усложненной задачи
    print("\n=== Анализ производительности (sum_array) ===")
    # Размеры массивов
    sizes = [1000, 5000, 10000, 50000, 100000, 200000, 350000, 500000]
    times = []  # Список для хранения среднего времени

    print("Замеры времени выполнения для алгоритма суммирования массива:")
    print("{:>10} | {:>12} | {:>15}".format(
        "Размер (N)", "Время (мс)", "Время/N (мкс)"))
    print("-" * 45)

    for size in sizes:
        # Генерация случайного массива заданного размера
        data = [random.randint(1, 1000) for _ in range(size)]  # O(N)

        # Замер времени выполнения с усреднением
        execution_time = measure_time(sum_array, data)
        times.append(execution_time)

        # Расчет времени на один элемент (в микросекундах)
        time_per_el = (execution_time * 1000) / size if size > 0 else 0

        # Форматированный вывод результатов замера
        print("{:>10} | {:>12.4f} | {:>15.4f}".format(
            size, execution_time, time_per_el
        ))

    # 3. Построение графика
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, times, 'bo-', label='Измеренное время')
    plt.xlabel('Размер массива (N)')
    plt.ylabel('Время выполнения (мс)')
    plt.title('Зависимость времени выполнения от размера массива\n'
              'Сложность: O(N)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend()
    # Сохранение графика в файл
    plt.savefig('time_complexity_plot.png', dpi=300, bbox_inches='tight')

    print("График сохранен в файл 'time_complexity_plot.png'")

    # 4. Анализ результатов
    print("\n=== Анализ результатов ===")
    print("1. Теоретическая сложность алгоритма sum_array: O(N)")
    print("2. Практические замеры показывают линейную зависимость времени "
          "от N,\n   что подтверждается графиком.")


# Точка входа в программу
if __name__ == "__main__":
    main()
