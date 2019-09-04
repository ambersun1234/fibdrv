myfib = [0, 1]

def fib():
    start = 2
    maxFib = 151
    for counter in range(start, maxFib):
        myfib.append(myfib[counter - 1] + myfib[counter - 2])

if __name__ == "__main__":
    fib()

    with open("./kresult.txt", "r") as f:
        temp = f.readline()
        while temp:
            myline = temp.rstrip().split(" ")
            r1 = myfib[eval(myline[0])]
            r2 = eval(myline[1])

            if r1 == r2:
                print("fib({:03d}): {}".format(eval(myline[0]), "passed"))
            else:
                print("fib({:03d}): {}".format(eval(myline[0]), "failed"))
            temp = f.readline()
        f.close()
