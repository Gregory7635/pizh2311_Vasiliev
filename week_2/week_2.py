class Snow:
    def __init__(self, n):
        self.value = n

    def __add__(self, other):
        return Snow(self.value + other)

    def __sub__(self, other):
        return Snow(self.value - other)

    def __mul__(self, other):
        return Snow(self.value * other)

    def __truediv__(self, other):
        return Snow(self.value // other)

    def makeSnow(self, row):
        result = []
        base = self.value // row
        extra = self.value % row
        for i in range(row):
            if i < extra:
                result.append("*" * (base + 1))
            else:
                result.append("*" * base)
        return "\n".join(result)


a1 = Snow(14)
print(a1.makeSnow(3))
a2 = a1 + 6
print(a2.value)
a3 = a2 - 5
print(a3.value)
a4 = a3 * 3
print(a4.value)
a5 = a4 / 2
print(a5.value)

