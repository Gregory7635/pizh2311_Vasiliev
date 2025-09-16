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