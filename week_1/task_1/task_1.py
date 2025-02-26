from random import randint


class Warrior:
    def healthWarrior(self, value):
        self.health = value

    def hit(opponent):
        opponent.health -= 20


unit1 = Warrior()
unit2 = Warrior()
unit1.healthWarrior(100)
unit2.healthWarrior(100)

while (unit1.health > 0 and unit2.health > 0):
    n = randint(1, 2)
    if n == 1:
        Warrior.hit(unit2)
        print(f"Атаковал unit1. У unit2 осталось {unit2.health} здоровья")
        print()
    else:
        Warrior.hit(unit1)
        print(f"Атаковал unit2. У unit1 осталось {unit1.health} здоровья")
        print()

if (unit1.health == 0):
    print("unit2 победил")

if (unit2.health == 0):
    print("unit1 победил")
