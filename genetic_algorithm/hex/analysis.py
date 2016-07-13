import os 
import sys

out_file = open(sys.argv[1] + ".csv", "w")

d = []
csv_files = []

for r, dirs, files in os.walk("./" + sys.argv[1]):
	for f in files:
		if ".csv" in f:
			csv_files.append(f)

	csv_files.sort()

mv = []

for csv in csv_files:
	f = open(sys.argv[1] + "/" + csv, "r")
	for l in f:
		pass
	last = l.split(",")
	out_file.write("%s, %d\n" % (csv.replace("mutation", "").replace(".csv", ""), int(last[2])))
	f.close()

out_file.close()
