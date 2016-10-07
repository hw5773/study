# I refer to the Koller & Friedman's PGM textbook for the VE algorithm

import sys
from etc import var_e, replace_factor, sum_over, calculation, check
from pprint import pprint

variables = set(["X1", "X2", "X3", "X4", "X5"])
# they are parent variables except the first one
set_of_factor = [["X1"], ["X2"], ["X3", "X1", "X2"], ["X4", "X3"], ["X5", "X3"]]
order = ["X1", "X2", "X3", "X4", "X5"]

# sum-product variable elimination
def sum_product_ve(sf, zset, elst):
	for z in zset:
		sf = sum_product_eliminate_var(sf, z)
	print sf

	return sf

# sum-product-eliminate-var function
def sum_product_eliminate_var(rf, z):
	print "to be eliminated var: %s" % z
	set1 = []
	set2 = []

	for f in rf:
		if check(z, f):
			set1.append(f)
		else:
			set2.append(f)

	if len(set1) > 0:
		tau = sum_over(set1, z)
		set2.append(tau)
		
	print "active factors:" 
	pprint(set2)

	return set2
	

# computing the conditional probabilities
def cond_prob_ve(y, elst):

	# replace applying the evidence to each factor
	replaced_set = replace_factor(set_of_factor, elst)

	# extract the set of variables to be eliminated
	zset = variables - set([y]) - set(var_e(elst))
	
	result_factor = sum_product_ve(replaced_set, zset, elst)
	print "complete result_factor: "
	pprint(result_factor)
	alpha = calculation(result_factor, y)

	if len(elst) > 0:
		result_str = "P(%s=1|" % y
		for e in elst:
			result_str = result_str + e + ","
		result_str = result_str[0:-1] + ") = %lf" % alpha

	else:
		result_str = "P(%s=1) = %lf" % (y, alpha)
	print result_str

def main():
	args = len(sys.argv)
	if args == 1:
		cond_prob_ve("X4", [])
		cond_prob_ve("X4", ["X3=1"])
		cond_prob_ve("X3", ["X1=1", "X2=0"])
	elif args >= 3:
		x = sys.argv[2]
		elst = []
		for i in range(3, len(args)+1):
			elst.append(sys.argv[i])
		cond_prob_ve(x, elst)
	else:
		print "Usage: python main.py <query variable> <evidence 1> <evidence 2> ..."

if __name__ == "__main__":
	main()
