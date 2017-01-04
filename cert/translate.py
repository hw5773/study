import os
import sys
import filecmp
from etc import domains

# Replacing the "." to "_" in the domain name
def domains(f):
	fi = open(f, "r")
	addr = []

	for line in fi:
		lst = line.split(",")
		addr.append(lst[1].replace("\"", "").replace("/", ""). replace(".", "_"))

	return addr

# Extracting the unique certificates
def distinct_certs(addr):
	certs = []
	ret = []

	# Finding the crt files
	for root, dirs, files in os.walk("./" + cert_dir):
		for f in files:
			if ".crt" in f:
				certs.append(cert_dir + f)

	# Assembling the certificates in the same domain
	for a in addr:
		lst = []
		n = len(a)
		for c in certs:
			# Removing the certificate with the size 0
			if os.path.getsize(c) == 0:
				continue
			if a in c.split("/")[1][0:n]:
				lst.append(c)

		# Removing the same certificates
		while len(lst) > 0:
			c1 = lst.pop()
			if len(c1) > 0:
				ret.append(c1.split("/")[1])
			comp = True

			for c2 in lst:
				comp = filecmp.cmp(c1, c2)
				if comp:
					lst.remove(c2)

			if not comp:
				break

	return ret

def make_txt(lst):
	for c in lst:
		os.system("openssl x509 -in %s -text -noout > %s" % (cert_dir + c, txt_dir + c))

def usage():
	print ("python3 compare_certs.py <domain names> <certificate directory>")
	exit(1)

def main():
	global cert_dir
	global txt_dir

	if len(sys.argv) != 3:
		usage()

	if not "/" in sys.argv[2]:
		sys.argv[2] = sys.argv[2] + "/"
	cert_dir = sys.argv[2]
	tmp = cert_dir.split("_")
	tmp[0] = "txt"
	txt_dir = '_'.join(tmp)

	if not os.path.isdir(txt_dir):
		os.mkdir(txt_dir)

	addr = domains(sys.argv[1])

	lst = distinct_certs(addr)
	make_txt(lst)

if __name__ == "__main__":
	main()
