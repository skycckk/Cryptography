def iris_distance(code1, code2):
    n = 0
    code12 = code1 ^ code2
    for i in range(0, 64):
        n = n + ((code12 >> i) & 0x1)

    return n / 64.0