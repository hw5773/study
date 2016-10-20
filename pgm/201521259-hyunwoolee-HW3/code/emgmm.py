import sys
import time
import numpy as np
import scipy as sc
import etc as e
import math
import matplotlib.pyplot as plt

def print_usage():
	print ("Usage: python emgmm.py <input file> <output file> <# of clusters> <display on(1)/off(0)>")
	print ("Example (display on): python emgmm.py mouse.txt emgmm_result.txt 3 1")
	print ("Example (display off): python emgmm.py mouse.txt emgmm_result.txt 3 0")
	sys.exit(1)

if len(sys.argv) != 5:
	print_usage()

data_file = sys.argv[1]
output_file = sys.argv[2]
x = []
mu = {}
pi = {}
cov = {}
try:
	K = int(sys.argv[3])
	display = int(sys.argv[4])
except:
	print_usage()

if K < 1:
	print ("The number of clusters needs to be positive integer")
	print_usage()

if display > 1 or display < 0:
	print ("If you want to see the result by matplotlib, please insert 1 for display")
	print_usage()

# Parse the input file and store the data into x
def parsing():
	try:
		f = open(data_file, "r")
	except:
		print ("file not found: ", data_file)
		print_usage()

	for line in f:
		p = line.strip().split(" ")
		x.append(np.array([float(p[0].strip()), float(p[1].strip())]))

# Initialize the parameters
def initialize():
	e.init_pi(pi, K, 0)
	e.init_mu(x, mu, K)
	e.init_cov(x, cov, K, 0)

# E-step
def e_step():
	gamma = {}

	for n in range(len(x)):
		gamma[n] = {}
		total = 0.0
		for j in range(K):
			total = total + pi[j] * e.gaussian(x[n], mu[j], cov[j])

		for k in range(K):
			gamma[n][k] = pi[k] * e.gaussian(x[n], mu[k], cov[k])/total

	return gamma

# M-step
def m_step(gamma):
	N = len(x)
	converged = True

	for k in range(K):
		Nk = 0.0
		for n in range(N):
			Nk = Nk + gamma[n][k]

		# generate a new mean
		mu_sum = 0
		for n in range(N):
			mu_sum = mu_sum + gamma[n][k] * x[n]
			
		mu_new = (1.0/Nk) * mu_sum

		# generate a new covariance
		cov_sum = 0
		for n in range(N):
			cov_sum = cov_sum + (gamma[n][k] * np.matrix((x[n] - mu_new)).T).dot((np.matrix(x[n] - mu_new)))

		cov_new = (1.0/Nk) * cov_sum

		# generate a new responsibility
		pi_new = Nk / N

		# updates
		mu[k] = mu_new
		pi[k] = pi_new
		cov[k] = cov_new

# Calculate the likelihood
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

# Display the result in the figure and print the result in the output file
def display_result(gamma):
	g = open(output_file, "w")

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
		
# Just print the result in the output file
def print_result(gamma):
	g = open(output_file, "w")

	for n in range(len(x)):
		g.write(str(max(gamma[n], key=lambda k:gamma[n][k])) + "\n")

	g.close()

# EM-GMM
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
	print ("The result file is '{0}' of which line is the cluster number assigned to each line in the input file".format(output_file))

def main():
	parsing()
	emgmm()

if __name__ == "__main__":
	main()
