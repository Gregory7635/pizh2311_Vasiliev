from module import Room

def user_interface():  
    print("Добро пожаловать в программу расчета площади стен комнаты!")  
    length = float(input("Введите длину комнаты: "))  
    width = float(input("Введите ширину комнаты: "))  
    height = float(input("Введите высоту комнаты: "))  

    room = Room(length, width, height)  

    while True:  
        action = input("\nВыберите действие:\n1. Добавить окно/дверь\n2. Рассчитать рабочую площадь\n3. Рассчитать количество рулонов\n4. Выход\nВаш выбор: ")  

        if action == '1':  
            room.workspaceCalculation()  
        
        elif action == '2':  
            print("\nРабочая площадь:", room.workSurface())  

        elif action == '3':  
            roll_length = float(input("Введите длину рулона: "))  
            roll_width = float(input("Введите ширину рулона: "))  
            rolls_needed = room.requiredRolls(roll_length, roll_width)  
            print(f"Необходимо рулонов: {rolls_needed}")  

        elif action == '4':  
            print("Выход из программы.")  
            break  

        else:  
            print("Неверный выбор, попробуйте еще раз.")


if __name__ == "__main__":  
    user_interface()