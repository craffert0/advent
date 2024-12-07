# -*- compile-command: "python3 two.py < sample.txt" -*-

import sys

def merge(x, y):
    return int(f'{x}{y}')

def guess(result, partial, rest):
    if len(rest) == 0:
        return result == partial
    return (guess(result, partial + rest[0], rest[1:]) or
            guess(result, partial * rest[0], rest[1:]) or
            guess(result, merge(partial, rest[0]), rest[1:]))

sum = 0
for line in sys.stdin:
    first = line.split(':')
    result = int(first[0])
    operands = [int(x) for x in first[1].split()]
    if guess(result, operands[0], operands[1:]):
        sum = sum + result
print(sum)
