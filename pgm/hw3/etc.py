import math
import numpy as np

def gaussian(x, mu, cov):
	x = np.array(x)
	c = ((2 * np.pi) ** (-len(mu)/2)) * (np.linalg.det(cov) ** (-1/2))
	e = (-1/2) * (np.matrix(x-mu) * np.linalg.inv(cov) * np.matrix(x-mu).T)
	val = float(c * np.exp(e))
	
	return val

def init_pi(pi, K, n):
	if n == 0:
		rest = 1
		order = np.random.permutation(K)
		for k in range(K-1):
			pi[order[k]] = np.random.random() * rest
			rest = rest - pi[order[k]]
		pi[order[K-1]] = rest
	else:
		for k in range(K):
			pi[k] = 1.0/K

def init_mu(x, mu, K, n):
	for k in range(K):
		r = np.array(x[int(np.random.random()*len(x))])
		mu[k] = r

def init_cov(x, cov, var, K, n):
	c1 = np.cov(np.vstack(x).T)
	dim = c1.shape[0]
	c2 = np.eye(dim)

	for d in range(dim):
		c2[d, d] = c1[d, d]

	for k in range(K):
		if n == 0:
			cov[k] = c1
		else:
			cov[k] = c2
