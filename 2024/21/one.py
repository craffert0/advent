# -*- compile-command: "python3 one.py < sample.txt" -*-

import sys

def mix(x, y):
    return x ^ y

def prune(x):
    return x % 16777216

def evolve(x, n):
    for i in range(n):
        x = prune(mix(x, x * 64))
        x = prune(mix(x, int(x / 32)))
        x = prune(mix(x, x * 2048))
    return x

sum = 0
for x in [int(x) for x in sys.stdin]:
    e = evolve(x, 2000)
    print(f'{x}: {e}')
    sum = sum + e

print(sum)
