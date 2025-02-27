# Подбор банковского вклада

Этот проект представляет консольное приложение для подбора банковского вклада по заданным пользователем параметрам.

## Основные классы

```python
class TimeDeposit:
    def __init__(self, name: str, interest_rate: float, period_limit: tuple[int, int], sum_limit: tuple[float, float]):
        # Инициализация базового срочного вклада
        pass

    def get_profit(self, initial_sum: float, period: int) -> float:
        # Рассчет прибыли по простым процентам
        pass

    def get_sum(self, initial_sum: float, period: int) -> float:
        # Возвращает итоговую сумму после начисления процентов
        pass
```

```python
class BonusTimeDeposit(TimeDeposit):
    def __init__(self, name: str, interest_rate: float, period_limit: tuple[int, int], sum_limit: tuple[float, float], bonus: dict):
        # Инициализация бонусного вклада
        pass

    def get_profit(self, initial_sum: float, period: int) -> float:
        # Рассчет прибыли с учетом бонуса
        pass
```

```python
class CompoundTimeDeposit(TimeDeposit):
    def get_profit(self, initial_sum: float, period: int) -> float:
        # Рассчет прибыли с капитализацией процентов
        pass
```

## Использование

```sh
python main.py
```

Программа запросит у пользователя:
- начальную сумму вклада;
- срок размещения вклада в месяцах.

После этого будут выведены подходящие варианты вкладов с расчетом прибыли и итоговой суммы.



