import sys
import numpy as np
import scipy as sc
import etc as e
import math

data_file = sys.argv[1]
x = []
k = int(sys.argv[2])
mu = {}
pi = {}
cov = {}
dim = 2

def parsing():
	f = open(data_file, "r")
	num_of_data = 0

	for line in f:
		p = line.strip().split(" ")
		x.append(np.matrix([float(p[0].strip()), float(p[1].strip())]))

	return num_of_data

def initialize():
	e.init_pi(pi, k)
	e.init_mu(mu, k)
	e.init_cov(cov, k, 2)

def e_step():
	gamma = {}

	for i in range(len(x)):
		tmp = []
		for j in range(k):
			tmp.append(pi[j] * e.gaussian(x[i], k, mu[j], cov[j]))
		gamma[i] = []
		total = sum(tmp)
		for j in range(k):
			gamma[i].append(tmp[j]/total)

	return gamma

def m_step(gamma):
	N = len(x)
	converged = True

	for i in range(k):
		N_k = 0
		mu_sum = 0
		for n in range(N):
			tmp = gamma[n][i]
			N_k = N_k + tmp
			mu_sum = mu_sum + tmp * x[n]
			
		mu_new = (1.0/N_k) * mu_sum

		cov_sum = 0
		for n in range(N):
			cov_sum = cov_sum + gamma[n][i] * (x[n] - mu_new).transpose() * (x[n] - mu_new)

		cov_new = (1.0/N_k) * cov_sum
		pi_new = N_k / N

		if (mu_new.all() == mu[i].all()) and (cov_new.all() == cov[i].all()) and (pi_new == pi[i]):
			converged = converged and True
		else:
			converged = False
			mu[i] = mu_new
			cov[i] = cov_new
			pi[i] = pi_new

	return converged

def likelihood(old):
	l = 0.0
	converged = False

	for n in range(len(x)):
		s = 0
		for i in range(k):
			s = s + pi[i] * e.gaussian(x[n], k, mu[i], cov[i])
		l = l + math.log(s)

	if l == old:
		converged = True
	else:
		converged = False		
	
	return converged

def emgmm():
	initialize()
	converged = False
	old = 100

	while not converged:
		gamma = e_step()
		converged = m_step(gamma)
		converged = likelihood(old)
		old = converged

	print "converged: ", converged

def main():
	parsing()
	emgmm()
#	display_result()


if __name__ == "__main__":
	main()
