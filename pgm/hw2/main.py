# I refer to the Koller & Friedman's PGM textbook for the VE algorithm

import sys
import copy
from etc import var_e, replace_factor, sum_over, calculation, check
from cpt import parent

variables = set(["X1", "X2", "X3", "X4", "X5"])
# they are parent variables except the first one
set_of_factor = [["X1"], ["X2"], ["X3", "X1", "X2"], ["X4", "X3"], ["X5", "X3"]]
order = ["X1", "X2", "X3", "X4", "X5"]

# Sum-product variable elimination.
# This function process steps with each variables to be eliminated.
# Input 
#   sf		The set of factors
#   zlst 	The set of variables to be eliminated
#	elst	The set of evidences
def sum_product_ve(sf, zlst, elst):
	for z in zlst:
		sf = sum_product_eliminate_var(sf, z)

	return sf

# sum-product-eliminate-var function. 
# This function eliminates the variable by summing over it.
def sum_product_eliminate_var(rf, z):
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
		
	return set2
	

# computing the conditional probabilities
def cond_prob_ve(y, elst):

	sf = copy.deepcopy(set_of_factor)

	# replace applying the evidence to each factor
	replaced_set = replace_factor(sf, elst)

	# extract the set of variables to be eliminated
	zlst = variables - set([y]) - set(var_e(elst))
	
	result_factor = sum_product_ve(replaced_set, zlst, elst)
	alpha = calculation(result_factor, y)

	if len(elst) > 0:
		result_str = "P(%s=1|" % y
		for e in elst:
			result_str = result_str + e + ","
		result_str = result_str[0:-1] + ") = %lf" % alpha

	else:
		result_str = "P(%s=1) = %lf" % (y, alpha)
	print result_str

# error messages for inputs
def error_exit():
	print "Usage: python main.py <query_variable> <evidence 1> <evidence 2> ..."
	sys.exit(0)

# check whether the variable used is valid
def check_variable(v):
	if v not in variables:
		print "Error: The variable needs to be one of the element in the set, {X1, X2, X3, X4, X5}"
		error_exit()

# check whether the evidences are valid
def check_evidences(q, elst):
	ret = []
	for e in elst:
		if "=" not in e:
			print "Error: the evidence form is wrong. There is no value"
			error_exit()
		else:
			lst = e.split("=")
			var = lst[0].strip()
			check_variable(var)
			if var not in parent[q]:
				continue
			val = lst[1].strip()
			if val not in [0,1]:
				print "Error: the evidence form is wrong. The event must be 0 or 1"
				error_exit()
			ret.append(e)

	return ret

def main():
	args = len(sys.argv)
	if args == 1:
		cond_prob_ve("X4", [])
		cond_prob_ve("X4", ["X3=1"])
		cond_prob_ve("X3", ["X1=1", "X2=0"])
	elif args >= 3:
		x = sys.argv[1]
		check_variable(x)

		elst = []
		for i in range(2, args):
			elst.append(sys.argv[i])
		elst = check_evidences(x, elst)

		cond_prob_ve(x, elst)
	else:
		error_exit()

if __name__ == "__main__":
	main()
