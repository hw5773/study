from etc import gaussian
import numpy as np

def main():
	mu = np.matrix([10, 1])
	cov = np.matrix([[0.00001, 0], [0, 0.005]])
	for i in range(100):
		val = gaussian(np.matrix([i*0.01, i*0.01]), mu, cov)

		if val > 1 or val < 0:
			print "Error(val): ", val

if __name__ == "__main__":
	main()
