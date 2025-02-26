class EncapsulatedClass:
    def __init__(self, value):
        self.__value = value

    def get_value(self):
        return self.__value

    def set_value(self, new_value):
        self.__value = new_value


obj = EncapsulatedClass(10)
print(obj.get_value())

obj.set_value(20)
print(obj.get_value())
