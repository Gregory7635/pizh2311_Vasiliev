class Person:
    def __init__(self, n, s, q=1):
        self.name = n
        self.surname = s
        self.qual = q

    def stringinfo(self):
        return f"{self.name} {self.surname} {self.qual}"

    def __del__(self):
        print(f"До свидания, мистер {self.name} {self.surname}")


people1 = Person("Иван", "Иванов")
people2 = Person("Петр", "Петров", 5)
people3 = Person("Денис", "Денисов", 4)

print(people1.stringinfo())
print(people2.stringinfo())
print(people3.stringinfo())
del people1
print("Нажмите Enter для выхода...")
input()
