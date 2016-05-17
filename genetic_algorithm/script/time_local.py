import os
import numpy
import sys

d = []
out_file = open("time.csv", "a")

num = 0
S_RATE = 0.7
M_THRE = 0.015
N = 100
K = 10
P0 = 0.6
P = 4
KF = 4
T = 0.7
C = 0.5
start = False
graph = {}
graph["maxcut50"] = {}
graph["maxcut100"] = {}
graph["maxcut500"] = {}
graph["maxcut50"]["node"] = 50
graph["maxcut50"]["edge"] = 123
graph["maxcut100"]["node"] = 100
graph["maxcut100"]["edge"] = 495
graph["maxcut500"]["node"] = 500
graph["maxcut500"]["edge"] = 4990

csv_files = []

for root, dirs, files in os.walk("../hw2/"):
	for directory in dirs:
		if sys.argv[1] in directory:
			d.append(directory)

for directory in d:
	for r, dirs, files in os.walk("../hw2/" + directory):
		prefix = ""

		for f in files:
			if ".csv" in f:
				prefix_lst = f.split("_")
				prefix = '_'.join(prefix_lst[0:-1])
				csv_files.append("%s/%s"%(r,f))

		csv_files.sort()

r = []		# rate list
et = []		# elapsed time list
mv = [] 	# max val list
m = 0
t = 0
mlist = []
tlist = []

for csv in csv_files:
	tmp_lst = (csv.split("/")[-1]).split("_")
	tmp = "_".join(tmp_lst[0:-1])

	if prefix != tmp:

		if start:
			num = num + 1
			line = "%s, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n" % (sys.argv[1], numpy.mean(mlist), numpy.mean(tlist), max(tlist), min(tlist), numpy.std(tlist))
			out_file.write(line)

		start = True

		prefix = tmp
		prefix_lst = tmp_lst

		r = []
		et = []
		mv = []

	f = open(csv, "r")
	f.readline()

	for l in f:
		last = l.split(",")
	
		if m < int(last[2]):
			m = int(last[2])
			t = int(last[1])

	mlist.append(m)
	tlist.append(t)

	f.close()

num = num + 1
line = "%s, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf\n" % (sys.argv[1], numpy.mean(mlist), numpy.mean(tlist), max(tlist), min(tlist), numpy.std(tlist))
out_file.write(line)
out_file.close()
