import re

if __name__ == "__main__":
    with open("./test.txt", "r") as f:
        temp = f.readline()
        while temp:
            myline = temp.split(" ")
            r1 = eval(re.sub(r'\b0+(?!\b)', '', myline[0]))
            r2 = eval(re.sub(r'\b0+(?!\b)', '', myline[1].rstrip()))

            if r1 == r2:
                print("{}: {}".format(myline[0], "passed"))
            else:
                print("{}: {}".format(myline[0], "failed"))
            temp = f.readline()
        f.close()
