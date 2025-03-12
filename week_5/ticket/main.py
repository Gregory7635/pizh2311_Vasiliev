from ticket_module import ProezdnoyBilet, BezlimitnyBilet, BiletSOgranicheniem, BiletSOgranicheniemPoezdok
from datetime import datetime, timedelta

def main():
    # Создаем билеты
    bezlimitny = BezlimitnyBilet("Иван Иванов")
    ogranichenie_vremeni = BiletSOgranicheniem("Петр Петров", (datetime.now() + timedelta(days=365 * 2)).strftime('%Y-%m-%d'))  # Билет действителен 2 года 
    ogranichenie_poezdok = BiletSOgranicheniemPoezdok("Сидор Сидоров", 10)

    # Тестируем безлимитный билет
    print("\nТестируем безлимитный билет:")
    bezlimitny.write_off_the_trip()
    bezlimitny.write_off_the_trip()

    # Тестируем билет с ограничением по времени
    print("\nТестируем билет с ограничением по времени:")
    for _ in range(3):
        ogranichenie_vremeni.write_off_the_trip()

    # Тестируем билет с ограничением по количеству поездок
    print("\nТестируем билет с ограничением по количеству поездок:")
    for _ in range(12):  # Пытаемся списать 12 поездок при 10 доступных
        ogranichenie_poezdok.write_off_the_trip()

    # Деактивируем билет и пробуем списать поездку
    print("\nДеактивируем билет и пробуем списать поездку:")
    ogranichenie_poezdok.deactivate()
    ogranichenie_poezdok.write_off_the_trip()

if __name__ == "__main__":
    main()