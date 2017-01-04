import os
import sys
from etc import domains

def usage():
	print ("python3 compare_ip.py <domain> <output file>")
	exit(1)

# Make the dictionary. Key is the domain name and Val is the list of IP addresses.
def make_dict(fi):
	f = open(fi, "r")
	d = {}
	for line in f:
		tmp = line.split(",")
		lst = tmp[1].split("/")
		d[tmp[0]] = lst

	return d

# Compare for each domain.
def compare_each(k, g, u):
	if len(k[0]) < 1:
		return "none"

	klst = k[0].split(".")
	glst = g[0].split(".")
	ulst = u[0].split(".")

	print ("klst: ", klst, " glst: ", glst, " ulst: ", ulst)

	if (klst[0] == glst[0]) and (klst[0] == ulst[0]):
		if (klst[1] == glst[1]) and (klst[1] == ulst[1]):
			if (klst[2] == glst[2]) and (klst[2] == ulst[2]):
				if (klst[3] == glst[3]) and (klst[3] == ulst[3]):
					ret = "same"
				else:
					ret = "s3"
			else:
				ret = "s2"
		else:
			ret = "s1"
	else:
		ret = "different"

	return ret

# Compare IP addresses among three regions.
def compare(lst, of):
	f = open(of, "w")

	d = {} # Dictionary for each country.
	for root, dirs, files in os.walk("."):
		for fi in files:
			if ".csv" in fi:
				country = fi.split(".")[0].split("_")[1]
				d[country] = make_dict(fi)

	for elem in lst:
		elem = elem.replace("_", ".")
		elem = "www." + elem
		kor = d["kor"][elem]
		gr = d["gr"][elem]
		us = d["us"][elem]

		print ("domain: ", elem)
		ret = compare_each(kor, gr, us)

		f.write(elem + "," + ret + "\n")

	f.close()

def main():
	if len(sys.argv) != 3:
		usage()

	of = sys.argv[2]
	lst = domains(sys.argv[1])
	compare(lst, of)

if __name__ == "__main__":
	main()
