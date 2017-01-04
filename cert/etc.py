# Replacing the "." to "_" in the domain name
def domains(f):
	fi = open(f, "r")
	addr = []
	fi.readline()

	for line in fi:
		lst = line.split(",")
		addr.append(lst[1].replace("\"", "").replace("/", ""). replace(".", "_"))

	return addr

