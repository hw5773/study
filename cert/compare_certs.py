import os
import sys
import filecmp

def parsing(f):
	fi = open(f, "r")
	addr = []

	for line in fi:
		lst = line.split(",")
		addr.append(lst[1].replace("\"", "").replace("/", ""). replace(".", "_"))

	return addr

def compare_certs(addr):
	certs = []
	ret = []

	for root, dirs, files in os.walk("./" + cert_dir):
		for f in files:
			if ".crt" in f:
				certs.append(cert_dir + "/" + f)

	for a in addr:
		lst = []
		for c in certs:
			if a in c:
				lst.append(c)

		while len(lst) > 0:
			c1 = lst.pop()
			comp = True

			for c2 in lst:
				comp = filecmp.cmp(c1, c2)

				if not comp:
					ret.append(a)
					break

			if not comp:
				break

	return ret

def make_txt(lst):
	for c in lst:
		#os.system("openssl x509 -in %s -text -noout > %s" % (cert_dir + "/" + c + ".crt", txt_dir + "/" + c))
		for idx in range(5):
			os.system("openssl x509 -in %s -text -noout > %s" % (cert_dir + "/" + c + "_" + str(idx) + ".crt", txt_dir + "/" + c + "_" + str(idx)))

def usage():
	print ("python3 compare_certs.py <sites> <certificate directory> <target directory>")
	exit(1)

def main():
	global cert_dir
	global txt_dir 

	if len(sys.argv) != 4:
		usage()

	cert_dir = sys.argv[2]
	txt_dir = sys.argv[3]

	addr = parsing(sys.argv[1])

	lst = compare_certs(addr)
	make_txt(lst)

if __name__ == "__main__":
	main()
