# -*- compile-command: "python3 one.py < input.txt" -*-

import re
import sys

sum = 0
for line in sys.stdin:
    for x in re.findall('mul\\((\\d+),(\\d+)\\)', line):
        sum = sum + int(x[0]) * int(x[1])
print(sum)
