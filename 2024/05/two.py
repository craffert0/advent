# -*- compile-command: "python3 two.py < input.txt" -*-

import sys
from collections import defaultdict
from functools import cmp_to_key

after = defaultdict(set)
before = defaultdict(set)

def compare(x, y):
    if y in after[x] or x in before[y]:
        return -1
    if x in after[y] or y in before[x]:
        return +1
    return 0

def proper(pages):
    good = True
    for i in range(len(pages)):
        p = pages[i]
        a = pages[i+1:]
        b = pages[0:i-1] if i > 0 else []
        if any([x in before[p] for x in a]) or any([x in after[p] for x in b]):
            good = False
            break
    return good

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
    if not proper(pages):
        pages.sort(key=cmp_to_key(compare))
        assert(proper(pages))
        sum += pages[int((len(pages)-1)/2)]
print(sum)
