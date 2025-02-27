from deposit import deposits

if __name__ == "__main__":
    print("Добро пожаловать в систему подбора вкладов!")

    while True:
        print("\n-----")
        print("Нажмите 1, чтобы подобрать вклад, или что угодно для выхода.")
        answer = input()
        if answer == "1":
            try:
                initial_sum = float(input("1/2: Введите начальную сумму вклада: "))
                period = int(input("2/2: Введите срок вклада (мес.): "))
            except ValueError:
                print("Неверный ввод! Попробуйте снова.")
                continue

            matched_deposits = []
            for deposit in deposits:
                try:
                    deposit._check_user_params(initial_sum, period)
                    matched_deposits.append(deposit)
                except AssertionError:
                    pass

            if matched_deposits:
                print("{0:18} | {1:13} | {2:13}".format("Вклад", "Прибыль", "Итоговая сумма"))
                for deposit in matched_deposits:
                    profit = deposit.get_profit(initial_sum, period)
                    total = deposit.get_sum(initial_sum, period)
                    print("{0:18} | {1:8,.2f} {3:4} | {2:8,.2f} {3:4}".format(
                        deposit.name, profit, total, deposit.currency))
            else:
                print("К сожалению, нет подходящих Вам вкладов.")
        else:
            break

    print("\nСпасибо, что воспользовались терминалом банка! До встречи!")
    