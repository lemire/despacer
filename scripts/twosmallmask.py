## use two loads instead
### first the low bits
for i in range(1<<8):
    solution = []
    for bit in range(16):
        if (((i|(0xFF<<8)) & (1<<bit)) == 0):
            solution.append(bit)
    while(len(solution) < 16): solution.append(0xFF),
    solution.reverse()
    s = ""
    for j in range(16):
        s+=hex(solution[j])+","
    print(s)

print()
print()
print()

### next the high bits
for i in range(1<<8):
    solution = []
    for z in range(8):
            solution.append(0xFF)
    for bit in range(16):
        if ((((i<<8)+0xFF) & (1<<bit)) == 0):
            solution.append(bit)
    while(len(solution) < 16): solution.append(0xFF)
    if(len(solution) > 16 ): raise "oh"
    solution.reverse()
    s = ""
    for j in range(len(solution)):
        s+=hex(solution[j])+","
    print(s)
