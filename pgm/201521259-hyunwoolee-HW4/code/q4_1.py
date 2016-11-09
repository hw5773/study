from params import *
import matplotlib.pyplot as plt

alpha = {}
beta = {}
p_o = 0

# alpha_i(t)
# implement the forward algorithm
def forward(t, i):
	if t == 0:	# when t = 0, alpha_i(0) = pi_i
		return prior[i]
	else:
		ret = 0
		for j in range(num_of_states):
			ret = ret + alpha[(t-1, j)] * transition[j][i] * emission[j][observation[t-1]-1]
		return ret

# beta_i(t)
# implement the backward algorithm
def backward(t, i):
	if t == (time-1):
		return 1
	else:
		ret = 0
		for j in range(num_of_states):
			ret = ret + beta[(t+1, j)] * transition[i][j] * emission[i][observation[t]-1]
		return ret

# P(O)
# calculate the probability of the observations, just by summing the alpha times beta at a specific time.
def prob_o():
	ret = 0
	for i in range(num_of_states):
		ret = ret + alpha[(0, i)] * beta[(0, i)]

	return ret

# gamma_i(t)
# calculate the posterior probability given the observations.
def calc_gamma(t, i):
	return (alpha[(t, i)] * beta[(t, i)]) / p_o

# pre-compute the table - alpha, beta, P(O)
def pre_compute():
	global p_o

	for t in range(time):
		for i in range(num_of_states):
			alpha[(t, i)] = forward(t, i)
			beta[(time-t-1, i)] = backward(time-t-1, i)
	p_o = prob_o()

# Question number 1.
def q4_1():
	pre_compute()
	xp0 = []
	yp0 = []
	xp1 = []
	yp1 = []
	xp2 = []
	yp2 = []
	ret = {}

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

	# plotting the 3 time series
	g0 = plt.scatter(xp0, yp0, s=50, c="red", marker="s")
	g1 = plt.scatter(xp1, yp1, s=50, c="green", marker="^")
	g2 = plt.scatter(xp2, yp2, s=50, c="blue")
	plt.legend((g0, g1, g2), ("State 1", "State 2", "State 3"))
	plt.xlabel("Time")
	plt.ylabel("Probability")

	plt.savefig("q4_1.png")
	plt.close("all")

	print ("Question 1> The result plot is saved as 'q4_1.png'")
