#!/usr/bin/python
"""
http://thecodelesscode.com/case/82

The Imperial Army wishes to catalog its many weapons. It recognizes four
types: the staff, the spear, the sword, and the knife. Each has a length,
which may vary from weapon to weapon. Each has a weight, which is calculated
from the length according to the weapon's type. In all respects, a knife is
merely a sword whose length is exactly two chi. Now: show me the classes you
would create!
"""

class Weapon(object):
    def __init__(self, length):
        self.length = length

class Staff(Weapon):
    def __init__(self, length):
        super(Staff, self).__init__(length)
        self.weight = self.length * 1.1

class Spear(Weapon):
    def __init__(self, length):
        super(Spear, self).__init__(length)
        self.weight = self.length * 1.5

class Sword(Weapon):
    def __init__(self, length):
        super(Sword, self).__init__(length)
        self.weight = self.length * 2.0

class Knife(Sword):
    def __init__(self):
        super(Knife, self).__init__(2.0)

mystaff = Staff(5.0)
print "staff length = {0}, weight = {1}".format(mystaff.length, mystaff.weight)

myspear = Spear(5.0)
print "spear length = {0}, weight = {1}".format(myspear.length, myspear.weight)

mysword = Sword(6.0)
print "sword length = {0}, weight = {1}".format(mysword.length, mysword.weight)

myknife = Knife()
print "knife length = {0}, weight = {1}".format(myknife.length, myknife.weight)
