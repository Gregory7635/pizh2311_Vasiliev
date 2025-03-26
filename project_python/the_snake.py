from random import choice, randint
import pygame

# Константы для размеров поля и сетки:
SCREEN_WIDTH, SCREEN_HEIGHT = 640, 480  # Ширина и высота игрового поля
GRID_SIZE = 20  # Размер одной ячейки сетки
GRID_WIDTH = SCREEN_WIDTH // GRID_SIZE  # Количество ячеек по ширине
GRID_HEIGHT = SCREEN_HEIGHT // GRID_SIZE  # Количество ячеек по высоте

# Направления движения:
UP = (0, -1)  # Движение вверх
DOWN = (0, 1)  # Движение вниз
LEFT = (-1, 0)  # Движение влево
RIGHT = (1, 0)  # Движение вправо

# Цвет фона - черный:
BOARD_BACKGROUND_COLOR = (0, 0, 0)

# Цвет границы ячейки:
BORDER_COLOR = (93, 216, 228)

# Цвет яблока:
APPLE_COLOR = (255, 0, 0)

# Цвет змейки:
SNAKE_COLOR = (0, 255, 0)

# Скорость движения змейки:
SPEED = 20

# Настройка игрового окна:
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT), 0, 32)

# Заголовок окна игрового поля:
pygame.display.set_caption('Змейка')

# Настройка времени:
clock = pygame.time.Clock()


class GameObject:
    """
    Базовый класс для всех игровых объектов.

    Атрибуты:
        position (tuple): Позиция объекта на игровом поле.
        body_color (tuple): Цвет объекта.

    Методы:
        __init__: Инициализация объекта.
        draw: Абстрактный метод для отрисовки объекта.
    """

    def __init__(self, position=(0, 0), body_color=None):
        """
        Инициализация объекта.

        Аргументы:
            position (tuple): Позиция объекта на игровом поле (по умолчанию (0, 0)).
            body_color (tuple): Цвет объекта (по умолчанию None).
        """
        self.position = position
        self.body_color = body_color

    def draw(self):
        """
        Абстрактный метод для отрисовки объекта.
        Должен быть переопределён в дочерних классах.
        """
        pass


class Apple(GameObject):
    """Класс для яблока на игровом поле."""

    def __init__(self, body_color=APPLE_COLOR):
        """
        Инициализация яблока.

        :param body_color: Цвет яблока (по умолчанию APPLE_COLOR).
        """
        super().__init__(body_color=body_color)
        self.randomize_position()

    def randomize_position(self, snake_positions=None):
        """
        Устанавливает случайное положение яблока на игровом поле.
        Если переданы позиции змейки, яблоко не появится на её теле.

        :param snake_positions: Список позиций змейки (по умолчанию None).
        """
        while True:
            self.position = (
                randint(0, GRID_WIDTH - 1) * GRID_SIZE,
                randint(0, GRID_HEIGHT - 1) * GRID_SIZE
            )
            # Если позиции змейки переданы, проверяем, не совпадает ли яблоко с её телом
            if snake_positions is None or self.position not in snake_positions:
                break

    def draw(self):
        """
        Отрисовывает яблоко на игровом поле.
        """
        rect = pygame.Rect(self.position, (GRID_SIZE, GRID_SIZE))
        pygame.draw.rect(screen, self.body_color, rect)
        pygame.draw.rect(screen, BORDER_COLOR, rect, 1)


class Snake(GameObject):
    """
    Класс для змейки на игровом поле.

    Атрибуты:
        Наследует атрибуты от GameObject.
        positions (list): Список позиций сегментов змейки.
        direction (tuple): Текущее направление движения змейки.
        next_direction (tuple): Следующее направление движения змейки.
        last (tuple): Последняя позиция хвоста змейки.

    Методы:
        __init__: Инициализация змейки.
        reset: Сбрасывает змейку в начальное состояние.
        get_head_position: Возвращает позицию головы змейки.
        update_direction: Обновляет направление движения змейки.
        move: Обновляет позицию змейки на игровом поле.
        draw: Отрисовывает змейку на игровом поле.
    """

    def __init__(self, body_color=SNAKE_COLOR):
        """
        Инициализация змейки.

        Аргументы:
            body_color (tuple): Цвет змейки (по умолчанию SNAKE_COLOR).
        """
        super().__init__(body_color=body_color)
        self.reset()

    def reset(self):
        """
        Сбрасывает змейку в начальное состояние.
        """
        self.positions = [((SCREEN_WIDTH // 2), (SCREEN_HEIGHT // 2))]
        self.direction = RIGHT
        self.next_direction = None
        self.last = None

    def get_head_position(self):
        """
        Возвращает позицию головы змейки.

        Возвращает:
            tuple: Позиция головы змейки.
        """
        return self.positions[0]

    def update_direction(self):
        """
        Обновляет направление движения змейки.
        """
        if self.next_direction:
            self.direction = self.next_direction
            self.next_direction = None

    def move(self):
        """
        Обновляет позицию змейки на игровом поле.
        """
        head_x, head_y = self.get_head_position()
        new_head = (
            (head_x + self.direction[0] * GRID_SIZE) % SCREEN_WIDTH,
            (head_y + self.direction[1] * GRID_SIZE) % SCREEN_HEIGHT
        )
        self.positions.insert(0, new_head)
        self.last = self.positions.pop()

    def draw(self):
        """
        Отрисовывает змейку на игровом поле.
        """
        for position in self.positions[:-1]:
            rect = pygame.Rect(position, (GRID_SIZE, GRID_SIZE))
            pygame.draw.rect(screen, self.body_color, rect)
            pygame.draw.rect(screen, BORDER_COLOR, rect, 1)

        # Отрисовка головы змейки
        head_rect = pygame.Rect(self.positions[0], (GRID_SIZE, GRID_SIZE))
        pygame.draw.rect(screen, self.body_color, head_rect)
        pygame.draw.rect(screen, BORDER_COLOR, head_rect, 1)

        # Затирание последнего сегмента
        if self.last:
            last_rect = pygame.Rect(self.last, (GRID_SIZE, GRID_SIZE))
            pygame.draw.rect(screen, BOARD_BACKGROUND_COLOR, last_rect)


def handle_keys(snake):
    """
    Обрабатывает нажатия клавиш для управления змейкой.

    Аргументы:
        snake (Snake): Объект змейки.
    """
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            raise SystemExit
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP and snake.direction != DOWN:
                snake.next_direction = UP
            elif event.key == pygame.K_DOWN and snake.direction != UP:
                snake.next_direction = DOWN
            elif event.key == pygame.K_LEFT and snake.direction != RIGHT:
                snake.next_direction = LEFT
            elif event.key == pygame.K_RIGHT and snake.direction != LEFT:
                snake.next_direction = RIGHT
            elif event.key == pygame.K_ESCAPE:  # Закрытие игры на ESC
                pygame.quit()
                raise SystemExit


def main():
    """
    Основная функция игры.
    """
    pygame.init()

    # Создание объектов змейки и яблока
    snake = Snake()
    apple = Apple()

    # Переменная для хранения рекордного размера змейки
    record_length = 1

    while True:
        clock.tick(SPEED)

        # Обработка нажатий клавиш
        handle_keys(snake)

        # Обновление направления и движения змейки
        snake.update_direction()
        snake.move()

        # Проверка на съедание яблока
        if snake.get_head_position() == apple.position:
            snake.positions.append(snake.last)
            # Передаём позиции змейки, чтобы яблоко не появилось внутри неё
            apple.randomize_position(snake.positions)

            # Обновление рекордного размера змейки
            if len(snake.positions) > record_length:
                record_length = len(snake.positions)
                pygame.display.set_caption(f'Змейка | Рекорд: {record_length}')

        # Проверка на столкновение с самой собой
        if snake.get_head_position() in snake.positions[1:]:
            snake.reset()

        # Отрисовка объектов
        screen.fill(BOARD_BACKGROUND_COLOR)
        snake.draw()
        apple.draw()

        # Обновление экрана
        pygame.display.update()


if __name__ == '__main__':
    main()