from params import *
import numpy as np

alpha = {}
gamma = {}
times = range(100, 128)

def forward(t, i): # alpha_i(t)
	if t == 0:
		return prior[i]
	else:
		ret = 0
		for j in range(num_of_states):
			ret = ret + alpha[(t-1, j)] * transition[j][i] * emission[j][observation[t-1]-1]
		return ret

def prob_o(t):
	ret = 0
	for i in range(num_of_states):
		ret = ret + alpha[(t, i)]

	return ret

def calc_gamma(t, i):
	return (alpha[(t, i)] / prob_o(t))[0]

def pre_compute():
	for t in range(128):
		for i in range(num_of_states):
			alpha[(t, i)] = forward(t, i)

	for t in times:
		for i in range(num_of_states):
			gamma[(t, i)] = calc_gamma(t, i)

def q4_3():
	pre_compute()
	prob = []
	ground = [observation[t] for t in times]

	print ("Posterior Probabilitiy P(Zt | O)")
	for t in times:
		s = 0
		for i in range(num_of_states):
			s = s + gamma[(t, i)]

		print ("P(Z%d | O): " % t, gamma[(t, 0)], ", ", gamma[(t, 1)], ", ", gamma[(t, 2)])
			
	print ("\nGround Truth:\t", ground)

	for trials in range(100):
		sample = []
		for t in times:
			r = np.random.random_sample()
			if r < gamma[(t, 0)]:
				sample.append(1)
			elif r < (gamma[(t, 0)] + gamma[(t, 1)]):
				sample.append(2)
			else:
				sample.append(3)

		p = len([i for i, j in zip(sample, ground) if i == j])/float(len(sample))
		prob.append(p)
		print ("Sample %3d:\t" % (trials+1), sample, " correct prob.: ", p)

	print ("\nAverage: ", np.average(prob), ", Var: ", np.var(prob), ", Max: ", max(prob), ", Min: ", min(prob))
