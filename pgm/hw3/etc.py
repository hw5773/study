import math
import numpy as np
import scipy as sc

def gaussian(x, k, mu, cov):
	c = ((1.0)/math.sqrt(((2.0*math.pi)**k)*np.linalg.det(cov)))
	e = -(1.0/2) * (x - mu) * np.linalg.inv(cov) * (x - mu).transpose()
	return c * math.exp(e)

def init_pi(pi, k):
	order = np.random.permutation(k)
	rest = 1

	for i in range(k-1):
		p = np.random.random() * rest
		pi[order[i]] = p
		rest = rest - p

	pi[order[k-1]] = rest

def init_mu(mu, k):
	for i in range(k):
		mu[i] = np.matrix([np.random.random(), np.random.random()])

def init_cov(cov, k, dim):
	for i in range(k):
		random_mat = np.matrix([[np.random.random(), np.random.random()],[np.random.random(), np.random.random()]])
		random_cov = np.dot(random_mat, random_mat.transpose())
		cov[i] = random_cov
