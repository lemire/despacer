print("(1<<16) * 16 = ", (1<<16)*16)
for i in range(1<<15):
    solution = []
    lastbit = 0
    for bit in range(16):
        if ((i & (1<<bit)) == 0):
            solution.append(bit)
            lastbit = bit
    while(len(solution) < 16): solution.append(lastbit),
    s = ""
    for j in range(16):
        s+=hex(solution[j])+","
    print(s)
