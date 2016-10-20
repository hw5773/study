import math
import numpy as np

# Multivariate Gaussian Probability Density Function.
def gaussian(x, mu, cov):
	x = np.array(x)
	c = ((2 * np.pi) ** (-len(mu)/2)) * (np.linalg.det(cov) ** (-1/2))
	e = (-1/2) * (np.matrix(x-mu) * np.linalg.inv(cov) * np.matrix(x-mu).T)
	val = float(c * np.exp(e))
	
	return val

# Initialize the responsibilities of the Gaussian Mixture Model.
# Inputs
#  - pi: the dictionary type repository of the responsibility
#  - K: the number of clusters
#  - n: If 0, the random resposibilities are generated and assigned. Else, the fixed resposibilities are assigned
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

# Initialize the means of the Gaussian Mixture Model.
# Inputs
#  - x: the input data
#  - mu: the dictionary-type repository of the means
#  - K: the number clusters
# Initially, the mean points are selected randomly among the points in the dataset
def init_mu(x, mu, K):
	for k in range(K):
		r = np.array(x[int(np.random.random()*len(x))])
		mu[k] = r

# Initialize the covariances of the Gaussian Mixture Model
# Inputs
#  - x: the input data
#  - cov: the dictionary-type repository of the covariances
#  - K: the number of clusters
#  - n: If 0, the initial covariances are the covariance of the whole data, Else, the initial covariances are the covariances of which the diagonal terms are the variances of each random variables
def init_cov(x, cov, K, n):
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
