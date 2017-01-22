import sys
import os
import copy

def usage():
    print ("python3 crawl.py <top domains> <output directory>")
    exit(1)

def parsing(f):
    fi = open(f, "r")
    fi.readline()

    addr = []

    for line in fi:
        lst = line.split(",")
        addr.append(lst[1].replace("\"", "").replace("/", "").strip())
        
    return addr

def main():
    if len(sys.argv) != 3:
        usage()

    ret = parsing(sys.argv[1])
    cert_dir = sys.argv[2]
    
    if not os.path.exists(cert_dir):
        os.mkdir(cert_dir)
    else:
        if not os.path.isdir(cert_dir):
            print ("Please use another directory")
            usage()

    for i in range(1):
        addr = copy.deepcopy(ret)
        n = 0
        for a in addr:
            n = n + 1
            cert = cert_dir + "/" + a.replace(".", "_") + "_" + str(i) + ".crt"
            a = "www." + a
            os.system("echo | timeout 20 openssl s_client -CApath /etc/ssl/certs -connect %s:443 2>&1 | sed --quiet '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > %s" % (a, cert))
            statinfo = os.stat(cert)
            print ("%d:\t%s is generated with %d bytes." % (n, cert, statinfo.st_size))

if __name__ == "__main__":
    main()
