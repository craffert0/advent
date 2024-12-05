# -*- compile-command: "python3 one.py < input.txt" -*-

import sys
from collections import defaultdict

after = defaultdict(set)
before = defaultdict(set)
for line in sys.stdin:
    if line == '\n':
        break
    both = line.split('|')
    assert len(both) == 2, f'[{both}]'
    after[int(both[0])].add(int(both[1]))
    before[int(both[1])].add(int(both[0]))

sum = 0
for line in sys.stdin:
    pages = [int(x) for x in line.split(',')]
    good = True
    for i in range(len(pages)):
        p = pages[i]
        a = pages[i+1:]
        b = pages[0:i-1] if i > 0 else []
        if any([x in before[p] for x in a]) or any([x in after[p] for x in b]):
            good = False
            break
    if good:
        sum += pages[int((len(pages)-1)/2)]
print(sum)
