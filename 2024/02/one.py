# -*- compile-command: "python3 one.py < input.txt" -*-

import sys
from safe import safe

count = 0
for line in sys.stdin:
    report = [int(x) for x in line.split()]
    if safe(report):
        count = count + 1
print(count)
