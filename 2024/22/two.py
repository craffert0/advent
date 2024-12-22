# -*- compile-command: "python3 two.py < sample2.txt" -*-

import sys

def mix(x, y):
    return x ^ y

def prune(x):
    return x % 16777216

def evolve(x):
    x = prune(mix(x, x * 64))
    x = prune(mix(x, int(x / 32)))
    x = prune(mix(x, x * 2048))
    return x

def key(a):
    return '.'.join([str(x) for x in a])

def calc(x):
    m = {}
    a = []
    prev = x
    for i in range(4):
        n = evolve(prev)
        a = a + [(n % 10) - (prev % 10)]
        prev = n
    m[key(a)] = prev % 10
    for i in range(4, 2000):
        n = evolve(prev)
        a = a[1:] + [(n % 10) - (prev % 10)]
        prev = n
        k = key(a)
        if not k in m:
            m[k] = prev % 10
    return m

maps = [calc(int(x)) for x in sys.stdin]
keys = set()
for m in maps:
    keys = keys | set(m.keys())

print(len(keys))

best = -1
best_k = ""
for k in keys:
    sum = 0
    for m in maps:
        if k in m:
            sum = sum + m[k]
    if sum > best:
      best = sum
      best_k = k
print(f'{best_k}: {best}')
