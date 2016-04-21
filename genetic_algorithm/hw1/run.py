import os
import sys

graph_type = ["maxcut50.txt", "maxcut100.txt", "maxcut500.txt"]
g = graph_type[int(sys.argv[2])]
graph = "../graph/" + g
representation = []
selection = []
crossover = []
mutation = []
replacement = []
stop = []
common = "cost.o maxcut.o common.o"
new_dir = sys.argv[1]

os.system("rm -rf *.o")
os.system("mkdir " + sys.argv[1])
os.system("gcc -c *.c")

for root, dirs, files in os.walk("./"):
	for f in files:
		if "crossover.o" in f:
			crossover.append(f)
		elif "mutation.o" in f:
			mutation.append(f)
		elif "replacement.o" in f:
			replacement.append(f)
		elif "chromosome.o" in f:
			representation.append(f)
		elif "stop_condition.o" in f:
			stop.append(f)
		elif "selection.o" in f:
			selection.append(f)
		else:
			continue

for b in representation:
	for c in crossover:
		for m in mutation:
			for r in replacement:
				for st in stop:
					for se in selection:
						os.system("gcc -o maxcut " + common + " " + b + " " + " " + c + " " + m + " " + r + " " + st + " " + se)
						
						for num in range(3):
							file_prefix = new_dir + "/" + g.split(".")[0] + "_binary_" + c.split("_")[0] + "_" + r.split("_")[0] + "_" + st.split("_")[0] + "_" + se.split("_")[0] + "_" + "S0.7_M0.1_N100_K10_P00.6_P4_KF4_T0.7"
							os.system("./maxcut " + graph + " " + file_prefix + "_" + str(num) + ".res " + file_prefix + "_" + str(num) + ".csv")
