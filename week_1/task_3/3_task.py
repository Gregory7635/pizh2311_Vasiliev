from random import randint


class All:
    id = 0

    def __init__(self, group):
        self.id = All.id
        All.id += 1
        self.group = group


class Soldier(All):
    def __init__(self, group):
        All.__init__(self, group)

    def followFromHero(self, hero):
            self.followhero = hero.id


class Hero(All):
    def __init__(self, group, level):
        All.__init__(self, group)
        self.level = level

    def Levelup(self):
            self.level += 1


hero1 = Hero(1, 5)
hero2 = Hero(2, 6)
command1 = []
command2 = []
i = 1
for i in range(9):
     n = randint(1, 2)
     if n == 1:
          command1.append(Soldier(n))
     else:
          command2.append(Soldier(n))

if (len(command1) > len(command2)):
     print("в первой команде больше солдат")
     hero1.Levelup()
     print(f"первый герой повышает уровень, теперь уровень равен {hero1.level}")
else:
     print("во второй команде больше солдат")
     hero2.Levelup()
     print(f"второй герой повышает уровень, теперь уровень равен {hero2.level}")

if (len(command1) > 0):
     command1[0].followFromHero(hero1)
     print(f"номер солдата = {command1[0].id}, номер героя = {hero1.id}")
else: print("В первой команде нет солдат")