import os
import sys

def usage():
    print ("python3 find_cn.py <certs directory> <common name>")
    exit(1)

def get_certs(d):
    certs = []

    for root, dirs, files in os.walk(d):
        for f in files:
            if ".crt" in f:
                if os.stat(root + f).st_size == 0:
                    continue
                else:
                    certs.append(root + f)

    return certs

def find_subject_alternative_name(certs, dom):
    cert_lst = []

    for c in certs:
        state = os.popen("openssl x509 -in %s -text -noout | grep DNS" % c).read()
        names = state.split(",")[-1]

        if dom in names:
            cert_lst.append(c)
    
    return cert_lst

def main():
    if len(sys.argv) != 3:
        usage()

    directory = sys.argv[1]
    dom = sys.argv[2]

    certs = get_certs(directory)
    cert_lst = find_subject_alternative_name(certs, dom)
    print ("cert_lst: ", cert_lst)

if __name__ == "__main__":
    main()
