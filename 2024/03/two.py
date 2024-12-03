# -*- compile-command: "python3 two.py < input.txt" -*-

import re
import sys

sum = 0
doit = True
for line in sys.stdin:
    for (a, b, yes, no) in re.findall("mul\\((\\d+),(\\d+)\\)|(do)\\(\\)|(don't)\\(\\)", line):
        if yes:
            doit = True
        elif no:
            doit = False
        elif doit:
            sum = sum + int(a) * int(b)
print(sum)
