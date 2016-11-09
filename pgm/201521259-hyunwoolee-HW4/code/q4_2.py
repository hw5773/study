from params import *
import matplotlib.pyplot as plt
import numpy as np

# Implement the Viterbi algorithm.
def q4_2():
	t1 = {}
	t2 = {}
	x = {}

	for i in range(3):
		t1[(i, 0)] = prior[i] * emission[i][observation[0]-1]
		t2[(i, 0)] = 0

	for t in range(1, time):
		for i in range(3):
			t1[i, t] = max([t1[k, t-1] * transition[k][i] * emission[i][observation[t]-1] for k in range(3)])
			lst = [t1[k, t-1] * transition[k][i] for k in range(3)]
			t2[i, t] = lst.index(max(lst))

	lst = [t1[0, time-1], t1[1, time-1], t2[1, time-1]]
	x[time-1] = lst.index(max(lst))

	for t in range(time-1, 0, -1):
		x[t-1] = t2[x[t], t]

	xp = list(x.keys())
	yp = [k + 1 for k in list(x.values())]
	color = []

	# If the expected hidden state is state 1, then the point is red-colored, 2 is green-colored, 3 is blue-colored

	for i in yp:
		if i == 1:
			color.append("red")
		elif i == 2:
			color.append("green")
		elif i == 3:
			color.append("blue")
		else:
			pass

	plt.scatter(xp, yp, c=color, s=50)
	plt.xlabel("Time")
	plt.ylabel("State")
	plt.xticks(np.arange(0, 110, 10))
	plt.yticks(np.arange(1, 4, 1))

	plt.savefig("q4_2.png")
	plt.close("all")

	print ("Question 2> The result plot is saved as 'q4_2.png'")
