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
