from datetime import datetime

class ProezdnoyBilet:
    """
    Базовый класс для проездного билета.
    """
    def __init__(self, owner_name: str):
        """
        Инициализация проездного билета.

        Аргументы:
            owner_name (str): Имя владельца билета.
        """
        self.owner_name = owner_name  # Общедоступное поле
        self._remaining_trips = 0  # Не общедоступное поле
        self.__is_active = True  # Закрытое поле

    def write_off_the_trip(self) -> None:
        """
        Списать поездку с билета.
        """
        if self.__is_active and self._remaining_trips > 0:
            self._remaining_trips -= 1
            print(f"Поездка списана. Осталось поездок: {self._remaining_trips}")
        else:
            print("Билет неактивен или поездки закончились.")

    def activate(self) -> None:
        """
        Активировать билет.
        """
        self.__is_active = True
        print("Билет активирован.")

    def deactivate(self) -> None:
        """
        Деактивировать билет.
        """
        self.__is_active = False
        print("Билет деактивирован.")


class BezlimitnyBilet(ProezdnoyBilet):
    """
    Класс для безлимитного билета.
    """
    def __init__(self, owner_name: str):
        """
        Инициализация безлимитного билета.

        Аргументы:
            owner_name (str): Имя владельца билета.
        """
        super().__init__(owner_name)
        self._remaining_trips = float('inf')  # Бесконечное количество поездок

    def write_off_the_trip(self) -> None:
        """
        Списать поездку с безлимитного билета.
        """
        print(f"Безлимитный билет. Поездка списана. Осталось поездок: ∞")


class BiletSOgranicheniem(ProezdnoyBilet):
    """
    Класс для билета с ограничением по времени.
    """
    def __init__(self, owner_name: str, expiration_date: str):
        """
        Инициализация билета с ограничением по времени.

        Аргументы:
            owner_name (str): Имя владельца билета.
            expiration_date (str): Дата окончания действия билета в формате 'YYYY-MM-DD'.
        """
        super().__init__(owner_name)
        self.expiration_date = datetime.strptime(expiration_date, "%Y-%m-%d")  # Преобразуем строку в дату
        self._remaining_trips = float('inf')  # Неограниченное количество поездок в пределах срока

    def write_off_the_trip(self) -> None:
        """
        Списать поездку с билета с ограничением по времени.
        """
        if datetime.now() > self.expiration_date:
            print("Билет просрочен. Поездки больше недоступны.")
            self.deactivate()
        else:
            print(f"Поездка списана. Билет действителен до {self.expiration_date.strftime('%Y-%m-%d')}.")


class BiletSOgranicheniemPoezdok(ProezdnoyBilet):
    """
    Класс для билета с ограничением по количеству поездок.
    """
    def __init__(self, owner_name: str, max_trips: int):
        """
        Инициализация билета с ограничением по количеству поездок.

        Аргументы:
            owner_name (str): Имя владельца билета.
            max_trips (int): Максимальное количество поездок.
        """
        super().__init__(owner_name)
        self._remaining_trips = max_trips  # Не общедоступное поле

    def write_off_the_trip(self) -> None:
        """
        Списать поездку с билета с ограничением по количеству поездок.
        """
        super().write_off_the_trip()