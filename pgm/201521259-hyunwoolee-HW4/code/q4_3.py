from params import *
import numpy as np

alpha = {}
gamma = {}
times = range(100, 128)

# forward algorithn fro prediction
def forward(t, i):
	if t == 0:
		return prior[i]
	else:
		ret = 0
		for j in range(num_of_states):
			ret = ret + alpha[(t-1, j)] * transition[j][i] * emission[j][observation[t-1]-1]
		return ret

# calculate the P(O)
def prob_o(t):
	ret = 0
	for i in range(num_of_states):
		ret = ret + alpha[(t, i)]

	return ret

# calculate the posterior probability
def calc_gamma(t, i):
	return (alpha[(t, i)] / prob_o(t))[0]

# pre-computes for alpha and gamma
def pre_compute():
	for t in range(128):
		for i in range(num_of_states):
			alpha[(t, i)] = forward(t, i)

	for t in times:
		for i in range(num_of_states):
			gamma[(t, i)] = calc_gamma(t, i)

# Question 3.
def q4_3():
	pre_compute()
	prob = []
	ground = [str(observation[t]) for t in times]
	out = open("q4_3.txt", "w")

	out.write("Posterior Probabilitiy P(Zt | O)\n")
	for t in times:
		s = 0
		for i in range(num_of_states):
			s = s + gamma[(t, i)]

		out.write("P(Z%d | O): %f, %f, %f\n" % (t, gamma[(t, 0)], gamma[(t, 1)], gamma[(t, 2)]))
			
	out.write("\nGround Truth:\t [%s]\n" % ', '.join(ground))

	for trials in range(100):
		sample = []
		for t in times:
			r = np.random.random_sample()
			if r < gamma[(t, 0)]:
				sample.append("1")
			elif r < (gamma[(t, 0)] + gamma[(t, 1)]):
				sample.append("2")
			else:
				sample.append("3")

		p = len([i for i, j in zip(sample, ground) if i == j])/float(len(sample))
		prob.append(p)
		out.write("Sample %3d:\t[%s]\tcorrect prob.: %f\n" % ((trials+1), ', '.join(sample), p))

	print("Question 3> The result file is saved as 'q4_3.txt'")
	out.write("\nAverage: %f, Var: %f, Max: %f, Min: %f\n" % (np.average(prob), np.var(prob), max(prob), min(prob)))

	out.close()
