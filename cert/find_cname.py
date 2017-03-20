import os
import sys

def usage():
    print ("Find the domain which has the cname")
    print ("python3 find_cname.py <nslookup file> <cname>")
    exit(1)

def parsing(nslookup):
    f = open(nslookup, "r")
    ns = {}

    for line in f:
        tmp = line.split(",")
        ns[tmp[0]] = tmp[-1].strip().split("/")

    return ns

def find_cname(ns, cname):
    domain_lst = []
    
    for k in ns.keys():
        for d in ns[k]:
            if cname in d:
                domain_lst.append(k)
                break

    return domain_lst

def main():
    if len(sys.argv) != 3:
        usage()

    ns = parsing(sys.argv[1])
    cname = sys.argv[2]

    domain_lst = find_cname(ns, cname)

    print (domain_lst)

if __name__ == "__main__":
    main()
