# -*- compile-command: "python3 two.py < input.txt" -*-

import sys
from safe import safe

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
