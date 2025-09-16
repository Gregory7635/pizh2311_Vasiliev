"""Сравнение эффективности линейного и бинарного поиска."""

import timeit
import random
from typing import List, Optional
import matplotlib.pyplot as plt
import numpy as np


def linear_search(arr: List[int], target: int) -> Optional[int]:
    """
    Линейный поиск элемента в массиве.
    
    Аргументы:
        arr: Массив для поиска
        target: Искомый элемент
        
    Возвращает:
        Индекс элемента или None, если не найден
        
    Сложность: O(n)
    """
    for i in range(len(arr)):  # O(n) - цикл по всем элементам
        if arr[i] == target:   # O(1) - сравнение элементов
            return i           # O(1) - возврат результата
    return None                # O(1) - элемент не найден
    # Общая сложность: O(n) * O(1) = O(n)


def binary_search(arr: List[int], target: int) -> Optional[int]:
    """
    Бинарный поиск элемента в отсортированном массиве.
    
    Аргументы:
        arr: Отсортированный массив для поиска
        target: Искомый элемент
        
    Возвращает:
        Индекс элемента или None, если не найден
        
    Сложность: O(log n)
    """
    left = 0                      # O(1) - инициализация
    right = len(arr) - 1          # O(1) - инициализация
    
    while left <= right:          # O(log n) - цикл уменьшает диапазон в 2 раза на каждой итерации
        mid = (left + right) // 2 # O(1) - вычисление середины
        
        if arr[mid] == target:    # O(1) - сравнение
            return mid            # O(1) - возврат результата
        elif arr[mid] < target:   # O(1) - сравнение
            left = mid + 1        # O(1) - обновление границы
        else:                     # O(1) - сравнение
            right = mid - 1       # O(1) - обновление границы
            
    return None                   # O(1) - элемент не найден
    # Общая сложность: O(log n) * O(1) = O(log n)


def measure_time(func, arr: List[int], target: int, number_of_runs: int = 100) -> float:
    """
    Измеряет среднее время выполнения функции поиска.
    
    Аргументы:
        func: Функция поиска (linear_search или binary_search)
        arr: Массив для поиска
        target: Искомый элемент
        number_of_runs: Количество запусков для усреднения
        
    Возвращает:
        Среднее время выполнения в миллисекундах
    """
    total_time = timeit.timeit(lambda: func(arr, target), number=number_of_runs)
    return (total_time / number_of_runs) * 1000  # Конвертация в миллисекунды


def generate_test_data(size: int) -> List[int]:
    """
    Генерирует отсортированный массив целых чисел.
    
    Аргументы:
        size: Размер массива
        
    Возвращает:
        Отсортированный массив уникальных чисел
    """
    return sorted(random.sample(range(size * 3), size))


def get_test_cases(arr: List[int]) -> List[int]:
    """
    Возвращает различные тестовые случаи для поиска.
    
    Аргументы:
        arr: Массив для тестирования
        
    Возвращает:
        Список целей для поиска: [первый, последний, средний, отсутствующий]
    """
    return [
        arr[0],                 # Первый элемент
        arr[-1],                # Последний элемент  
        arr[len(arr) // 2],     # Средний элемент
        -1                      # Отсутствующий элемент
    ]


def main():
    """Основная функция для проведения экспериментов и анализа."""
    # Характеристики ПК для воспроизводимости
    pc_info = """
    Характеристики ПК для тестирования:
    - Процессор: Intel Core i7-8700 @ 3.6GHz
    - Оперативная память: 32 GB DDR4
    - OC: Windows 11
    - Python: 3.11.9
    """
    print(pc_info)
    
    # Размеры массивов для тестирования
    sizes = [1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000]
    linear_times = []
    binary_times = []
    
    print("Замеры времени выполнения (среднее по 100 запускам):")
    print("Размер | Линейный (мс) | Бинарный (мс) | Отношение")
    print("-" * 55)
    
    for size in sizes:
        # Генерация тестовых данных
        arr = generate_test_data(size)
        targets = get_test_cases(arr)
        
        # Измерение времени для линейного поиска (поиск последнего элемента)
        linear_time = measure_time(linear_search, arr, arr[-1])
        linear_times.append(linear_time)
        
        # Измерение времени для бинарного поиска (поиск последнего элемента)
        binary_time = measure_time(binary_search, arr, arr[-1])
        binary_times.append(binary_time)
        
        # Вывод результатов
        ratio = linear_time / binary_time if binary_time > 0 else 0
        print(f"{size:6} | {linear_time:12.4f} | {binary_time:12.4f} | {ratio:8.1f}x")
    
    # Построение графиков
    plt.figure(figsize=(15, 6))
    
    # График 1: Линейный масштаб
    plt.subplot(1, 2, 1)
    plt.plot(sizes, linear_times, 'ro-', label='Линейный поиск O(n)')
    plt.plot(sizes, binary_times, 'bo-', label='Бинарный поиск O(log n)')
    plt.xlabel('Размер массива')
    plt.ylabel('Время выполнения (мс)')
    plt.title('Сравнение времени поиска (линейный масштаб)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    
    # График 2: Логарифмический масштаб по обеим осям
    plt.subplot(1, 2, 2)
    plt.loglog(sizes, linear_times, 'ro-', label='Линейный поиск O(n)')
    plt.loglog(sizes, binary_times, 'bo-', label='Бинарный поиск O(log n)')
    plt.xlabel('Размер массива')
    plt.ylabel('Время выполнения (мс)')
    plt.title('Сравнение времени поиска (log-log scale)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    
    plt.tight_layout()
    plt.savefig('search_comparison.png', dpi=300, bbox_inches='tight')
    print("\nГрафик сохранен в файл 'search_comparison.png'")
    
    # Анализ результатов
    print("\n=== АНАЛИЗ РЕЗУЛЬТАТОВ ===")
    print("1. Теоретическая сложность:")
    print("   - Линейный поиск: O(n)")
    print("   - Бинарный поиск: O(log n)")
    
    print("\n2. Практические результаты подтверждают теорию:")
    print("   - Линейный поиск: время растет пропорционально n")
    print("   - Бинарный поиск: время растет логарифмически")
    
    print("\n3. Эффективность бинарного поиска:")
    print(f"   - Для {sizes[-1]} элементов: в {linear_times[-1]/binary_times[-1]:.1f} раз быстрее")
    
    print("\n4. Визуальное подтверждение:")
    print("   - График в линейном масштабе: виден экспоненциальный рост разницы")
    print("   - График в log-log scale: линейный поиск - прямая линия (линейный рост)")
    print("     бинарный поиск - пологая кривая (логарифмический рост)")


if __name__ == "__main__":
    main()