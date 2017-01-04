import sys
import os
import copy

def parsing(f):
	fi = open(f, "r")
	fi.readline()

	addr = []

	for line in fi:
		lst = line.split(",")
		addr.append(lst[1].replace("\"", "").replace("/", ""))
		
	return addr

def main():
	ret = parsing(sys.argv[1])

	for i in range(5):
		addr = copy.deepcopy(ret)
		n = 0
		for a in addr:
			n = n + 1
			cert = "certs/" + a.replace(".", "_") + "_" + str(i) + ".crt"
			os.system("echo | timeout 10 openssl s_client -connect %s:443 2>&1 | sed --quiet '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' > %s" % (a, cert))
			statinfo = os.stat(cert)
			print ("%d:\t%s is generated with %d bytes." % (n, cert, statinfo.st_size))

if __name__ == "__main__":
	main()
