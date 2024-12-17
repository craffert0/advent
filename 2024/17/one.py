# -*- compile-command: "python3 one.py < sample.txt" -*-

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

    def run(self):
        self.I = 0
        self.Out = []
        while self.I < len(self.Program):
            ins = self.Program[self.I]
            op = self.Program[self.I + 1]
            self.I = self.I + 2
            self.Instruction[ins](op)
        print(','.join([str(x) for x in self.Out]))

    def read_register(istr):
        return int(istr.readline()[12:-1])

    def dump(self):
        print(f'{self.A}/{self.B}/{self.C}: {self.Program}.')

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
        self.Out.append(self.Combo[op]() % 8)

    def bdv(self, op):
        self.B = int(self.A / (2**self.Combo[op]()))

    def cdv(self, op):
        self.C = int(self.A / (2**self.Combo[op]()))


w = What(sys.stdin)
w.dump()
w.run()
