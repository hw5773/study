import numpy as np

epsilon	=	0.2
delta 	=	0.1

def location_release(e, d, m, p, x):
	return "test"

f = open("random_walk_matrix.csv", "r")

print "Make the transition matrix from csv file"

mat = [] # the list for the matrix

for line in f:
	idx = line.index(",,")
	lst = line[0:idx].split(",")
	l = [float(i) for i in lst]
	mat.append(l)

m = np.matrix(mat)

print m

print "Make the init state vector"

init = [0 for i in range(25)]
init[11] = 0.5
init[12] = 0.5
v = np.matrix(init)

print v

pre = []
result = v

i = 0
while not (np.array_equal(pre, result)):
	print "Multiplication (%d): " % i
	pre = result
	result = result * m
	print result
	i = i + 1

print "result: "
print result

xlst = []
x = 1
xlst.append(x)

posterior = ""

for i in range(25):
	print location_release(epsilon, delta, m, posterior, x)

f.close()
