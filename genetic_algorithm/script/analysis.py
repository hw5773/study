import os
import math

def mean(values):
	if len(values) == 0:
		return None
	return sum(values, 0.0) / len(values)

def stdev(values):
	if len(values) < 2:
		return None

	sd = 0.0
	sum_val = 0.0
	mean_val = mean(values)

	for i in range(0, len(values)):
		diff = values[i] - mean_val
		sum_val = sum_val + diff * diff

	sd = math.sqrt(sum_val / (len(values)))
	return sd

d = []
out_file = open("result.csv", "a")

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

out_file.write("number, avg elapsed time (s), avg max val, max val, stdev\n")

csv_files = []

for root, dirs, files in os.walk("../hw2/"):
	for directory in dirs:
		if "_local" in directory:
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

print csv_files

r = []		# rate list
et = []		# elapsed time list
mv = [] 	# max val list
prefix = ""
prefix_lst = []

for csv in csv_files:
	tmp_lst = (csv.split("/")[-1]).split("_")
	tmp = "_".join(tmp_lst[0:-1])

	if prefix != tmp:

		if start:
			num = num + 1
			
			line = "%d, %.2lf, %.2lf, %.2lf\n" % (num, sum(et)/len(et), sum(mv)/len(mv), max(mv))

# mutation name is not included

			print line
			out_file.write(line)

		start = True

		prefix = tmp
		prefix_lst = tmp_lst

		r = []
		et = []
		mv = []

	print prefix_lst

	for l in f:
		pass
	last = l.split(",")
	r.append(float(last[0]))
	et.append(int(last[1]))
	mv.append(int(last[2]))

	f.close()

line = "%d, %.2lf, %.2lf, %.2lf, %.2lf\n" % (num, sum(et)/len(et), sum(mv)/len(mv), max(mv), stdev(mv))
out_file.write(line)
out_file.close()
