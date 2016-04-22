import os

d = []
out_file = open("result.csv", "w")

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

out_file.write("number, avg elapsed time (s), avg max val, avg rate, max val1, rate 1, max val2, rate 2, max val 3, rate 3, graph, nodes, edges, selection, crossover, mutation, replacement, N, K, S_RATE, M_THRE, P0, POINTS, K_FIT, T\n")

for root, dirs, files in os.walk("../hw1/"):
	for directory in dirs:
		if "maxcut" in directory:
			d.append(directory)

for directory in d:
	for r, dirs, files in os.walk("../hw1/" + directory):
		csv_files = []
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
prefix = ""
prefix_lst = []

for csv in csv_files:
	tmp_lst = (csv.split("/")[-1]).split("_")
	tmp = "_".join(tmp_lst[0:-1])

	if prefix != tmp:
		prefix = tmp
		prefix_lst = tmp_lst

		if start:
			num = num + 1
			
			line = "%d, %.2lf, %.2lf, %.6lf, %d, %.6lf, %d, %.6lf, %d, %.6lf, " % (num, sum(et)/len(et), sum(mv)/len(mv), sum(r)/len(r), mv[0], r[0], mv[1], r[1], mv[2], r[2])
			line = line + "%s, %d, %d, " % (prefix_lst[0], graph[prefix_lst[0]]["node"], graph[prefix_lst[0]]["edge"])
			line = line + "%s, %s, %s, %s, %d, %d, %f, %f, %f, %d, %d, %f\n" % (prefix_lst[5], prefix_lst[2], "general", prefix_lst[3], int(prefix_lst[8][1:]), int(prefix_lst[9][1:]), float(prefix_lst[6][1:]), float(prefix_lst[7][1:]), float(prefix_lst[10][2:]), int(prefix_lst[11][1:]), int(prefix_lst[12][2:]), float(prefix_lst[13][1:]))

# mutation name is not included

			out_file.write(line)

		start = True

		r = []
		et = []
		mv = []

	f = open(csv, "r")

	for l in f:
		pass
	last = l.split(",")
	r.append(float(last[0]))
	et.append(int(last[1]))
	mv.append(int(last[2]))

	f.close()

out_file.close()
