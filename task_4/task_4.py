class Number:
    def __init__(self, value):
        self.value = value

    def __add__(self, other):
        if isinstance(other, Number):
            return Number(self.value + other.value)
        if isinstance(other, int):
            return Number(self.value + other)

    def __str__(self):
        return str(self.value)


num1 = Number(5)
num2 = Number(10)
num3 = num1 + num2
num4 = num1 + 3
print(num3)
print(num4)
