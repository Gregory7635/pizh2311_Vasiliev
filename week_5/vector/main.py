from vector_module import Vector, VectorCollection

# Создаем объекты Vector
v1 = Vector(1, 2)
v2 = Vector(3, 4)
v3 = Vector(5, 6)

# Создаем контейнер и добавляем векторы
container = VectorCollection()
container.add(v1)
container.add(v2)
container.add(v3)

# Выводим содержимое контейнера
print("Содержимое контейнера после добавления векторов:")
print(container)

# Проверяем метод __getitem__ (индексация)
print("\nПроверка индексации (второй элемент):")
print(container[1])  # Должен вывести Vector(3, 4)

# Удаляем элемент по индексу
print("\nУдаляем второй элемент:")
container.remove(1)
print("Содержимое контейнера после удаления:")
print(container)

# Сохраняем контейнер в файл
print("\nСохраняем контейнер в файл 'vectors.json'...")
container.save("vectors.json")

# Загружаем контейнер из файла
print("Загружаем контейнер из файла 'vectors.json'...")
new_container = VectorCollection()
new_container.load("vectors.json")
print("Содержимое контейнера после загрузки:")
print(new_container)