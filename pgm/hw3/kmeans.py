import sys
import time
import numpy as np
import scipy as sc
import etc as e
import math
import matplotlib.pyplot as plt

def print_usage():
	print ("Usage: python kmeans.py <data file> <# of clusters> <display on(1)/off(0)>")
	print ("Example (display on): python kmeans.py mouse.txt 3 1")
	print ("Example (display off): python kmeans.py mouse.txt 3 0")
	sys.exit(1)

if len(sys.argv) != 4:
	print_usage()

data_file = sys.argv[1]
x = []
mu = {}
pi = {}
cov = {}
var = []
try:
	K = int(sys.argv[2])
	display = int(sys.argv[3])
except:
	print_usage()

if K < 1:
	print ("The number of clusters needs to be positive integer")
	print_usage()

if display > 1 or display < 0:
	print ("If you want to see the result by matplotlib, please insert 1 for display")
	print_usage()

def parsing():
	try:
		f = open(data_file, "r")
	except:
		print ("file not found: ", data_file)
		print_usage()

	for line in f:
		p = line.strip().split(" ")
		x.append(np.array([float(p[0].strip()), float(p[1].strip())]))

def initialize():
	e.init_pi(pi, K, 1)
	e.init_mu(x, mu, K, 1)
	e.init_cov(x, cov, var, K, 1)

def e_step():
	gamma = {}

	for n in range(len(x)):
		gamma[n] = {}
		total = 0.0
		lst = []
		for j in range(K):
			v = pi[j] * e.gaussian(x[n], mu[j], cov[j])
			total = total + v
			lst.append(v)
	
		idx = lst.index(max(lst))

		for k in range(K):
			if k == idx:
				gamma[n][k] = 1
			else:
				gamma[n][k] = 0

	return gamma

def m_step(gamma):
	N = len(x)
	converged = True

	for k in range(K):
		Nk = 0.0
		for n in range(N):
			Nk = Nk + gamma[n][k]

		mu_sum = 0
		for n in range(N):
			mu_sum = mu_sum + gamma[n][k] * x[n]
			
		mu_new = (1.0/Nk) * mu_sum
		mu[k] = mu_new

def likelihood(old):
	converged = False

	l = 0.0
	
	for n in range(len(x)):
		s = 0.0
		for k in range(K):
			s = s + pi[k] * e.gaussian(x[n], mu[k], cov[k])
		l = l + math.log(s)

	epsilon = 0.0001
	if np.abs(l - old) < epsilon:
		converged = True
	else:
		converged = False		
	
	return (converged, l)

def display_result(gamma):
	g = open("kmeans_result.txt", "w")

	xp = []
	yp = []
	color = []

	for n in range(len(x)):
		xp.append(float(x[n].item(0)))
		yp.append(float(x[n].item(1)))
		color.append(float(max(gamma[n], key=lambda k:gamma[n][k])) + 1)
		g.write(str(max(gamma[n], key=lambda k:gamma[n][k])) + "\n")

	g.close()

	plt.scatter(xp, yp, s=100, c=color)
	plt.show()
		
def print_result(gamma):
	g = open("kmeans_result.txt", "w")

	for n in range(len(x)):
		g.write(str(max(gamma[n], key=lambda k:gamma[n][k])) + "\n")

	g.close()

def emgmm():
	initialize()
	converged = False
	old = 0.0

	while not converged:
		gamma = e_step()
		m_step(gamma)
		ret = likelihood(old)
		converged = converged | ret[0]
		old = ret[1]

	if (display == 1):
		display_result(gamma)
	else:
		print_result(gamma)
	print ("The result file is 'kmeans_result.txt' of which line is the cluster number assigned to each line in the input file")

def main():
	parsing()
	emgmm()

if __name__ == "__main__":
	main()
