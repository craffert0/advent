# -*- compile-command: "python3 one.py < sample.txt" -*-

import functools
import math
import sys

def incr(stone):
    if stone == 0:
        return [1]
    digits = 1 + int(math.log10(stone))
    if digits % 2 == 0:
        half = digits / 2
        bighalf = int(math.pow(10, half))
        return [int(stone / bighalf), int(stone % bighalf)]
    return [2024 * stone]


stones = [int(x) for x in sys.stdin.readline().split()]
for i in range(25):
#    print(stones)
    stones = functools.reduce(lambda x, y: x + y, [incr(x) for x in stones])
print(len(stones))
