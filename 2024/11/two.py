# -*- compile-command: "python3 two.py < sample.txt" -*-

import datetime
import math
import sys

from functools import cache

@cache
def calc(s, depth):
    while depth > 0:
        depth = depth - 1
        if s == 0:
            s = 1
        else:
            digits = 1 + int(math.log10(s))
            if digits % 2 == 1:
                s = 2024 * s
            else:
                half = digits / 2
                bighalf = 10**half
                return calc(int(s / bighalf), depth) + calc(int(s % bighalf), depth)
    return 1

stones = [int(x) for x in sys.stdin.readline().split()]
sum = 0
for s in stones:
    sum = sum + calc(s, 75)
print(sum)
