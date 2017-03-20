import sys
import os
import struct
import socket
import subprocess
import time

def usage():
    print ("Full IPv4 Scanning to find the hosts which open the 443 port.")
    print ("python3 port.py <output filename> <min int> <max int>")
    exit(1)

def range_error():
    print ("max_int must bigger or equal to min_int")
    print ("python3 port.py <output filename> <min int> <max int>")
    exit(1)

def full_scan(min_int, max_int):
    ip_lst = []
    
    for addr in range(min_int, max_int+1):
        ip = socket.inet_ntoa(struct.pack("!I", addr))
        process = subprocess.Popen(["nmap", "-p443", "-Pn", ip], stdout=subprocess.PIPE)
        output = process.communicate()[0].decode('ascii').replace("\t", "").split("\n")
        print (ip)

        if "443/tcp open  https" in output:
            ip_lst.append(ip)

    return ip_lst

def main():
    if len(sys.argv) != 4:
        usage()

    of = open(sys.argv[1], "w")
    min_int = int(sys.argv[2])
    max_int = int(sys.argv[3])

    if min_int > max_int:
        range_error()

    start = time.time()
    print ("start: ", start)
    ip_lst = full_scan(min_int, max_int)
    end = time.time()
    print ("end: ", end)
    print ("elapsed time: ", end - start)
    print ("total ip addresses: ", max_int - min_int)
    print ("filtered ip addresses: ", len(ip_lst))

    for ip in ip_lst:
        of.write(ip + "\n")

    of.close()

if __name__ == "__main__":
    main()
