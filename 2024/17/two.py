# -*- compile-command: "python3 two.py < sample2.txt" -*-

import sys

class What:
    def __init__(self, istr):
        self.A = What.read_register(istr)
        self.B = What.read_register(istr)
        self.C = What.read_register(istr)
        istr.readline()
        self.Program = [int(x) for x in istr.readline()[9:-1].split(',')]
        self.Combo = [
            lambda: 0,
            lambda: 1,
            lambda: 2,
            lambda: 3,
            lambda: self.A,
            lambda: self.B,
            lambda: self.C,
        ]
        self.Instruction = [
            self.adv,
            self.bxl,
            self.bst,
            self.jnz,
            self.bxc,
            self.out,
            self.bdv,
            self.cdv,
        ]

    def tryone(self, newA):
        self.A = newA
        self.I = 0
        self.Out = []
        self.Failed = False
        while not self.Failed and self.I < len(self.Program):
            ins = self.Program[self.I]
            op = self.Program[self.I + 1]
            self.I = self.I + 2
            self.Instruction[ins](op)
        return self.Failed or len(self.Out) != len(self.Program)

    def read_register(istr):
        return int(istr.readline()[12:-1])

    def adv(self, op):
        self.A = int(self.A / (2**self.Combo[op]()))

    def bxl(self, op):
        self.B = self.B^op

    def bst(self, op):
        self.B = self.Combo[op]() % 8

    def jnz(self, op):
        if self.A != 0:
            self.I = op

    def bxc(self, op):
        self.B = self.B^self.C

    def out(self, op):
        value = self.Combo[op]() % 8
        if value == self.Program[len(self.Out)]:
            self.Out.append(value)
        else:
            self.Failed = True

    def bdv(self, op):
        self.B = int(self.A / (2**self.Combo[op]()))

    def cdv(self, op):
        self.C = int(self.A / (2**self.Combo[op]()))


w = What(sys.stdin)
newA = 31782000000
while w.tryone(newA):
    newA = newA + 1
    if newA % 1000000 == 0:
        print(newA)
print(f'done: {newA}')
