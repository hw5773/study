import math

def gaussian(x, mu, sigma_sq):
	c = ((1.0)/math.sqrt(2.0*sigma_sq*math.pi))
	e = -1.0 * (((x - mu) ** 2) / (2 * sigma_sq))
	return c * math.exp(e)

def x2x3(x2, x3):
	return g1x2x3(1,x2,x3) + g1x2x3(2,x2,x3)
#	return 0.81*gaussian(x2, 50, 10)*gaussian(x3, 50, 10) + 0.09*gaussian(x2, 50, 10)*gaussian(x3, 60, 10) + 0.09*gaussian(x2, 60, 10)*gaussian(x3, 50, 10) + 0.01*gaussian(x2, 60, 10)*gaussian(x3, 60, 10)

def g1x2x3(g1,x2,x3):
	if g1 == 1:
		return 0.5*(0.9*gaussian(x3,50,10)+0.1*gaussian(x3,60,10))*(0.9*gaussian(x2,50,10)+0.1*gaussian(x2,60,10))
	elif g1 == 2:
		return 0.5*(0.1*gaussian(x3,50,10)+0.9*gaussian(x3,60,10))*(0.1*gaussian(x2,50,10)+0.9*gaussian(x2,60,10))

def main():
	print g1x2x3(1,50,50)/x2x3(50,50)
	print g1x2x3(2,50,50)/x2x3(50,50)
	print "\n"

	print g1x2x3(1,60,60)/x2x3(60,60)
	print g1x2x3(2,60,60)/x2x3(60,60)
	print "\n"

	print g1x2x3(1,50,60)/x2x3(50,60)
	print g1x2x3(2,50,60)/x2x3(50,60)

if __name__ == "__main__":
	main()
