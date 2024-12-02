for i in range(1, 17):
    row = 100 * i
    row = str(row)
    command = "./gen_supermarble " + row + " " + row + " " + str(500) + " > testfile" + str(i) + ".in"
    print(command)
