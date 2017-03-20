import sys
import matplotlib.pyplot as plt

def average(lst):
    return sum(lst) / len(lst)

def parsing(f):
    f.readline()
    val = []
    time = []

    for line in f:
        tmp = line.split(",")
        num = int(tmp[0].strip())
        val.append(int(tmp[1].strip()))
        time.append(int(tmp[2].strip()))

    return val, time

def analysis(v, t):
    print ("val: ", average(v))
    print ("time: ", average(t))

def main():
    f = open(sys.argv[1], "r")
    val, time = parsing(f)
    analysis(val, time)

    f.close()

if __name__ == "__main__":
    main()
