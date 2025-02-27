class TimeDeposit:
    """

    https://ru.wikipedia.org/wiki/Срочный_вклад.

    Поля:
      - self.name (str): наименование;
      - self._interest_rate (float): процент по вкладу (0; 100];
      - self._period_limit (tuple[int, int]):
            допустимый срок вклада в месяцах [от; до);
      - self._sum_limit (tuple[float, float]):
            допустимая сумма вклада [от; до).
    Свойства:
      - self.currency (str): знак/наименование валюты.
    Методы:
      - self._check_self(): проверяет соответствие данных ограничениям вклада;
      - self.get_profit(initial_sum, period): возвращает прибыль по вкладу;
      - self.get_sum(initial_sum, period): возвращает сумму по окончании вклада.
    """

    def __init__(self, name: str, interest_rate: float,
                 period_limit: tuple[int, int],
                 sum_limit: tuple[float, float]) -> None:
        self.name = name
        self._interest_rate = interest_rate
        self._period_limit = period_limit
        self._sum_limit = sum_limit
        self._check_self()

    def __str__(self) -> str:
        return (f"Наименование:       {self.name}\n"
                f"Валюта:             {self.currency}\n"
                f"Процентная ставка:  {self._interest_rate}\n"
                f"Срок (мес.):        [{self._period_limit[0]}; {self._period_limit[1]})\n"
                f"Сумма:              [{self._sum_limit[0]:,}; {self._sum_limit[1]:,})")

    @property
    def currency(self) -> str:
        return "руб."

    def _check_self(self) -> None:
        assert 0 < self._interest_rate <= 100, "Неверно указан процент по вкладу!"
        assert 1 <= self._period_limit[0] < self._period_limit[1], "Неверно указаны ограничения по сроку вклада!"
        assert 0 < self._sum_limit[0] <= self._sum_limit[1], "Неверно указаны ограничения по сумме вклада!"

    def _check_user_params(self, initial_sum: float, period: int) -> None:
        is_sum_ok = self._sum_limit[0] <= initial_sum < self._sum_limit[1]
        is_period_ok = self._period_limit[0] <= period < self._period_limit[1]
        assert is_sum_ok and is_period_ok, "Условия вклада не соблюдены!"

    def get_profit(self, initial_sum: float, period: int) -> float:
        """Расчёт прибыли по формуле простых процентов:
           прибыль = initial_sum * процент / 100 * период / 12
        """
        self._check_user_params(initial_sum, period)
        return initial_sum * self._interest_rate / 100 * period / 12

    def get_sum(self, initial_sum: float, period: int) -> float:
        return initial_sum + self.get_profit(initial_sum, period)


class BonusTimeDeposit(TimeDeposit):
    """Срочный вклад с получением бонуса к концу срока вклада.

    Бонус начисляется как % от прибыли, если вклад больше определенной суммы.

    Атрибуты:
      - self._bonus (dict): с ключами "percent" (int) и "sum" (float).
    """

    def __init__(self, name: str, interest_rate: float,
                 period_limit: tuple[int, int],
                 sum_limit: tuple[float, float],
                 bonus: dict) -> None:
        self._bonus = bonus
        super().__init__(name, interest_rate, period_limit, sum_limit)

    def __str__(self) -> str:
        base_str = super().__str__()
        return (f"{base_str}\n"
                f"Бонус (%):          {self._bonus['percent']}\n"
                f"Бонус (мин. сумма): {self._bonus['sum']:,}")

    def _check_self(self) -> None:
        super()._check_self()
        assert "percent" in self._bonus and "sum" in self._bonus, \
            "Бонус должен содержать 'percent' и 'sum'"
        assert self._bonus["percent"] > 0, "Бонус процент должен быть больше 0"
        assert self._bonus["sum"] > 0, "Бонус минимальная сумма должна быть больше 0"

    def get_profit(self, initial_sum: float, period: int) -> float:
        """Расчёт прибыли с бонусом:
           Если initial_sum > bonus['sum'],
           итоговая прибыль = базовая прибыль * (1 + bonus['percent'] / 100)
        """
        self._check_user_params(initial_sum, period)
        base_profit = initial_sum * self._interest_rate / 100 * period / 12
        if initial_sum > self._bonus["sum"]:
            return base_profit * (1 + self._bonus["percent"] / 100)
        return base_profit


class CompoundTimeDeposit(TimeDeposit):
    """Срочный вклад с ежемесячной капитализацией процентов."""

    def __str__(self) -> str:
        base_str = super().__str__()
        return f"{base_str}\nКапитализация %   : Да"

    def get_profit(self, initial_sum: float, period: int) -> float:
        """Расчёт прибыли с капитализацией процентов:
           прибыль = initial_sum * ((1 + процент/100/12)**период - 1)
        """
        self._check_user_params(initial_sum, period)
        return initial_sum * ((1 + self._interest_rate / 100 / 12) ** period - 1)


# Данные для вкладов
deposits_data = dict(interest_rate=5, period_limit=(6, 18), sum_limit=(1000, 100000))

# Список доступных вкладов
deposits = (
    TimeDeposit("Сохраняй", interest_rate=5,
                period_limit=(6, 18),
                sum_limit=(1000, 100000)),
    BonusTimeDeposit("Бонусный", interest_rate=5,
                     period_limit=(6, 18),
                     sum_limit=(1000, 100000),
                     bonus={"percent": 5, "sum": 2000}),
    CompoundTimeDeposit("С капитализацией", interest_rate=5,
                        period_limit=(6, 18),
                        sum_limit=(1000, 100000))
)
