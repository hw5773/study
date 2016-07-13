import os
import sys

graph_type = ["maxcut50.txt", "maxcut100.txt", "maxcut500.txt"]
g = graph_type[int(sys.argv[2])]
graph = "../graph/" + g
representation = []
selection = ["order_based_selection.o"]
crossover = ["reverse_crossover.o"]
mutation = ["general_mutation.o"]
replacement = ["preselection_replacement.o"]
stop = ["rate_stop_condition.o"]
common = "cost.o maxcut.o common.o"
new_dir = sys.argv[1]

os.system("rm -rf *.o")
os.system("mkdir " + sys.argv[1])
os.system("gcc -c *.c")

for b in representation:
	for c in crossover:
		for m in mutation:
			for r in replacement:
				for st in stop:
					for se in selection:
						os.system("gcc -o maxcut " + common + " " + b + " " + " " + c + " " + m + " " + r + " " + st + " " + se)
						
						for num in range(3):
							file_prefix = new_dir + "/" + g.split(".")[0] + "_binary_" + c.split("_")[0] + "_" + m.split("_")[0] + "_" + r.split("_")[0] + "_" + st.split("_")[0] + "_" + se.split("_")[0] + "_" + "S0.7_M0.1_N100_K10_P00.6_P4_KF4_T0.7_C0.5"
							os.system("./maxcut " + graph + " " + file_prefix + "_" + str(num) + ".res " + file_prefix + "_" + str(num) + ".csv")
