import os
import numpy
import sys 

d = []
out_file = open(sys.argv[1] + ".csv", "w")
out_file2 = open(sys.argv[1] + "_max.csv", "w")

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

out_file.write("number, avg elapsed time (s), avg val, max val, min val, stdev, graph, nodes, edges, selection, crossover, mutation, replacement, N, K, S_RATE, M_THRE, P0, POINTS, K_FIT, T, C\n")

csv_files = []

for root, dirs, files in os.walk("./"):
	for directory in dirs:
		if sys.argv[1] in directory:
			d.append(directory)

for directory in d:
	for r, dirs, files in os.walk("./" + directory):
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
g = []		# generation list
#prefix = ""
#prefix_lst = []

for csv in csv_files:
	tmp_lst = (csv.split("/")[-1]).split("_")
	tmp = "_".join(tmp_lst[0:-1])

	if prefix != tmp:

		if start:
			num = num + 1
			
			line = "%d, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, " % (num, sum(et)/len(et), numpy.mean(mv), max(mv), min(mv), numpy.std(mv))
			line = line + "%s, %d, %d, " % (prefix_lst[0], graph[prefix_lst[0]]["node"], graph[prefix_lst[0]]["edge"])
			line = line + "%s, %s, %s, %s, %d, %d, %.2lf, %.2lf, %.2lf, %d, %d, %.2lf, %.2lf\n" % (prefix_lst[6], prefix_lst[2], prefix_lst[3], prefix_lst[4], int(prefix_lst[10][1:]), int(prefix_lst[11][1:]), float(prefix_lst[8][1:]), float(prefix_lst[9][1:]), float(prefix_lst[12][2:]), int(prefix_lst[13][1:]), float(prefix_lst[14][2:]), float(prefix_lst[15][1:]), float(prefix_lst[16][1:]))

# mutation name is not included

			out_file.write(line)

		start = True

		prefix = tmp
		prefix_lst = tmp_lst

		r = []
		et = []
		mv = []

	f = open(csv, "r")

	loop = 0
	for l in f:
		loop = loop + 1
		pass
	last = l.split(",")
	r.append(float(last[0]))
	et.append(int(last[1]))
	mv.append(int(last[2]))
	g.append(loop)

	f.close()

num = num + 1
			
line = "%d, %.2lf, %.2lf, %.2lf, %.2lf, %.2lf, " % (num, sum(et)/len(et), numpy.mean(mv), max(mv), min(mv), numpy.std(mv))
line = line + "%s, %d, %d, " % (prefix_lst[0], graph[prefix_lst[0]]["node"], graph[prefix_lst[0]]["edge"])
line = line + "%s, %s, %s, %s, %d, %d, %.2lf, %.2lf, %.2lf, %d, %d, %.2lf, %.2lf\n" % (prefix_lst[6], prefix_lst[2], prefix_lst[3], prefix_lst[4], int(prefix_lst[10][1:]), int(prefix_lst[11][1:]), float(prefix_lst[8][1:]), float(prefix_lst[9][1:]), float(prefix_lst[12][2:]), int(prefix_lst[13][1:]), float(prefix_lst[14][2:]), float(prefix_lst[15][1:]), float(prefix_lst[16][1:]))

# mutation name is not included

out_file.write(line)

out_file.close()

out_file2.write("num, max val, generation\n")

n = 0

for v in mv:
	generation = g[n]
	n = n+1
	out_file2.write("%d, %d, %d\n" % (n, v, generation))

out_file2.write("%.2lf\n" % (1.0*sum(g)/len(g)))

out_file2.close()
