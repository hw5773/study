from etc import domains
import os
import sys
import socket
import subprocess

def usage():
    print ("python3 nslookup.py <domains> <output file>")
    exit(1)

def lookup(lst, of):
    f = open(of, "w")
    num = 0

    for e in lst:
        addr = ""
        cname = ""
        auth = "auth"
        dns_server = ""
        dns_addr = False
        name = "same"

        e = e.replace("_", ".")
        e = "www." + e
        process = subprocess.Popen(["nslookup", e], stdout=subprocess.PIPE)
        output = process.communicate()[0].decode('ascii').replace("\t", "").split("\n")
        num = num + 1

        for elem in output:
            if ":" in elem:
                key = elem.split(":")[0].strip()
                val = elem.split(":")[1].strip()

                if "Server" in key:
                    dns_server = val
                elif "Address" in key:
                    if not dns_addr:
                        dns_addr = True
                    else:
                        if len(addr) < 1:
                            addr = val
                        else:
                            addr = addr + "/" + val
                elif "Non-authoritative answer" in key:
                    auth = "non-auth"
                elif "Name" in key:
                    if e != val:
                        name = "different"
            else:
                if elem == '':
                    continue
                elif "canonical" in elem:
                    val = elem.split("=")[1].strip()
                    if len(cname) < 1:
                        cname = val
                    else:
                        cname = cname + "/" + val
                else:
                    continue

        s = str(num) + "," + e + "," + addr + "," + dns_server + "," + auth + "," + name + "," + cname

        print (s)
        f.write(s + "\n")
                
        if num == 1000:
            break

    f.close()

def main():
    if len(sys.argv) != 3:
        usage()

    lst = domains(sys.argv[1])
    of = sys.argv[2]
    lookup(lst, of)

if __name__ == "__main__":
    main()
