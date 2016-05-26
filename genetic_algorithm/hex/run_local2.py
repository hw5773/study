import os
import sys

graph_type = ["maxcut50.txt", "maxcut100.txt", "maxcut500.txt"]
g = graph_type[int(sys.argv[2])]
graph = "../graph/" + g
representation = ["chromosome.o"]
selection = ["tornament_selection.o"]
crossover = ["reverse_crossover.o"]
mutation = ["general_mutation.o"]
replacement = ["general_replacement.o"]
stop = ["rate_stop_condition.o"]
common = "cost.o maxcut_local2.o common.o"
new_dir = sys.argv[1]

os.system("rm -rf *.o")
os.system("mkdir " + sys.argv[1])
os.system("gcc -c *.c")

#for root, dirs, files in os.walk("./"):
#	for f in files:
#		if "fixedonepoint_crossover.o" in f:
#			crossover.append(f)
#		elif "decrease_mutation.o" in f:
#			mutation.append(f)
#		elif "general_replacement.o" in f:
#			replacement.append(f)
#		elif "chromosome.o" in f:
#			representation.append(f)
#		elif "stop_condition.o" in f:
#			stop.append(f)
#		elif "roulette_selection.o" in f:
#			selection.append(f)
#		else:
#			continue

print crossover
print mutation
print replacement
print representation
print stop
print selection

for b in representation:
	for c in crossover:
		for m in mutation:
			for r in replacement:
				for st in stop:
					for se in selection:
						os.system("gcc -o maxcut " + common + " " + b + " " + " " + c + " " + m + " " + r + " " + st + " " + " local_optimization2.o " + se)
						
						for num in range(int(sys.argv[3]), int(sys.argv[3])+5):
							file_prefix = new_dir + "/" + g.split(".")[0] + "_binary_" + c.split("_")[0] + "_" + m.split("_")[0] + "_" + r.split("_")[0] + "_" + st.split("_")[0] + "_" + se.split("_")[0] + "_" + "LOCAL2_S0.7_M0.3_N1000_K1_P00.51_P4_KF4_T0.7_C0.5"
							os.system("./maxcut " + graph + " " + file_prefix + "_" + str(num) + ".res " + file_prefix + "_" + str(num) + ".csv")
