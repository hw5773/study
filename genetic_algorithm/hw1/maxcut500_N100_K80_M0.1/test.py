import os

for root, dirs, files in os.walk('.'):
	for f in files:
		fi = open(f, "r")
		a = fi.readline()
		if len(a) < 5:
			os.system("mv %s %s" % (f, f.replace("csv", "bak")))
