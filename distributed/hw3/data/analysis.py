import os
import sys
import math

def usage():
    print ("Analyze the csv files: Round, Time, Value")
    print ("USAGE: python3 analysis.py <output file>")
    exit(1)

def file_lst():
    lst = []

    for root, dirs, files in os.walk("./"):
        for f in files:
            if ".csv" in f:
                lst.append(f)
    lst.sort()

    return lst

def average(lst):
    return sum(lst) / len(lst)

def stdev(lst):
    avg = average(lst)
    var = sum([(e - avg) ** 2 for e in lst]) / len(lst)
    return math.sqrt(var)

def analysis(lst, of):
    ret_avg = {}
    ret_stdev = {}

    for i in range(1, 11):
        ret_avg[i] = {}
        ret_stdev[i] = {}

    for f in lst:
        data = open(f, "r")
        tmp = f.strip().split(".")[0].split("_")
        op = tmp[0].strip()
        threads = int(tmp[1].strip())
        data.readline()
        values = []

        for line in data:
            values.append(int(line.split(",")[1].strip()))

        ret_avg[threads][op] = average(values)
        ret_stdev[threads][op] = stdev(values)

    of.write("Threads, TAS(avg), TAS(stdev), TTAS(avg), TTAS(stdev), CAS(avg), CAS(stdev), FAA(avg), FAA(stdev)\n")

    for i in range(1, 11):
        out = str(i) + "," + str(ret_avg[i]["tas"]) + "," + str(ret_stdev[i]["tas"]) + "," + str(ret_avg[i]["ttas"]) + "," + str(ret_stdev[i]["ttas"]) + "," + str(ret_avg[i]["cas"]) + "," + str(ret_stdev[i]["cas"]) + "," + str(ret_avg[i]["faa"]) + "," + str(ret_stdev[i]["faa"]) + "\n"
        of.write(out)

def main():
    if len(sys.argv) != 2:
        usage()

    lst = file_lst()
    of = open(sys.argv[1], "w")
    analysis(lst, of)

    of.close()

if __name__ == "__main__":
    main()
