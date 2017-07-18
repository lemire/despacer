print("(1<<16) * 16 = ", (1<<16)*16)
for i in range(1<<16):
    solution = []
    for bit in range(16):
        if ((i & (1<<bit)) == 0):
            solution.append(bit)
    while(len(solution) < 16): solution.append(0xFF),
    s = ""
    for j in range(16):
        s+=hex(solution[j])+","
    print(s)
