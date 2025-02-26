class Snow:
    def __init__(self, count):
        self.count = int(count)

    def __add__(self, other):
        return Snow(self.count + other)

    def __sub__(self, other):
        return Snow(self.count - other)

    def __mul__(self, other):
        return Snow(self.count * other)

    def __truediv__(self, other):
        return Snow(round(self.count / other))

    def makeSnow(self, rows):
        base = self.count // rows
        remainder = self.count % rows
        result = []
        for i in range(rows):
            if i < remainder:
                result.append('*' * (base + 1))
            else:
                result.append('*' * base)
        return "\n".join(result)

    def __call__(self, new_value):
        self.count = int(new_value)


if __name__ == "__main__":
    s = Snow(16)
    s(12)
    print(s.makeSnow(3))
    s2 = s + 4
    s3 = s2 * 2
    s4 = s3 - 3
    s5 = s4 / 2
    print("Количество снежинок после операций:", s5.count)
