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