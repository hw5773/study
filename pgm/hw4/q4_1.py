from params import *
import matplotlib.pyplot as plt

alpha = {}
beta = {}
p_o = 0

def forward(t, i): # alpha_i(t)
	if t == 0:
		return prior[i]
	else:
		ret = 0
		for j in range(num_of_states):
			ret = ret + alpha[(t-1, j)] * transition[j][i] * emission[j][observation[t-1]-1]
		return ret

def backward(t, i): # beta_i(t)
	if t == (time-1):
		return 1
	else:
		ret = 0
		for j in range(num_of_states):
			ret = ret + beta[(t+1, j)] * transition[i][j] * emission[i][observation[t]-1]
		return ret

def prob_o():
	ret = 0
	for i in range(num_of_states):
		ret = ret + alpha[(0, i)] * beta[(0, i)]

	return ret

def calc_gamma(t, i):
	return (alpha[(t, i)] * beta[(t, i)]) / p_o

def pre_compute():
	global p_o

	for t in range(time):
		for i in range(num_of_states):
			alpha[(t, i)] = forward(t, i)
			beta[(time-t-1, i)] = backward(time-t-1, i)
	p_o = prob_o()

def q4_1():
	pre_compute()
	xp0 = []
	yp0 = []
	xp1 = []
	yp1 = []
	xp2 = []
	yp2 = []

	for t in range(time):
		for i in range(num_of_states):
			ret = calc_gamma(t, i)
			if i == 0:
				xp0.append(t) 
				yp0.append(ret)
			elif i == 1:
				xp1.append(t)
				yp1.append(ret)
			elif i == 2:
				xp2.append(t)
				yp2.append(ret)
			else:
				pass

	g0 = plt.scatter(xp0, yp0, s=50, c="red", edgecolors="none", marker="s")
	g1 = plt.scatter(xp1, yp1, s=50, c="green", edgecolors="none", marker="^")
	g2 = plt.scatter(xp2, yp2, s=50, c="blue", edgecolors="none")
	plt.legend((g0, g1, g2), ("State 1", "State 2", "State 3"))
	plt.xlabel("Time")
	plt.ylabel("Probability")
	plt.show()

