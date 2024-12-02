# -*- compile-command: "python3 02.2.py < 02.input.1.txt" -*-

import sys

def safe(report):
    increase = False
    decrease = False
    for a, b in zip(report[0:-1], report[1:]):
        if a < b:
            if b - a > 3:
                return False
            increase = True
        elif b < a:
            if a - b > 3:
                return False
            decrease = True
        else:
            return False
    return not (increase and decrease)

def safer(report):
    if safe(report):
        return True
    for i in range(len(report)):
        b = list(report)
        b.pop(i)
        if safe(b):
            return True
    return False


count = 0
for line in sys.stdin:
    report = [int(x) for x in line.split()]
    if safer(report):
        count = count + 1
print(count)
