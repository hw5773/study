import sys
import math
import matplotlib.pyplot as plt

def average(lst):
    return sum(lst) / len(lst)

def variance(lst):
    square = 0
    avg = average(lst)
    for e in lst:
        square = square + (e - avg) * (e - avg)
    return square / len(lst)

def stddev(lst):
    return math.sqrt(variance(lst))

def parsing(f):
    f.readline()
    val = []
    time = []

    for line in f:
        tmp = line.split(",")
        num = int(tmp[0].strip())
        val.append(int(tmp[2].strip()))

    return val, time

def analysis(v, t):
    print ("val: ", average(v))
    print ("val var: ", variance(v))
    print ("val stddev: ", stddev(v))

def main():
    f = open(sys.argv[1], "r")
    val, time = parsing(f)
    analysis(val, time)

    f.close()

if __name__ == "__main__":
    main()
