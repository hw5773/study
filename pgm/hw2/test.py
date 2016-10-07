from cpt import p
from etc import potential_val, calculation
'''
# calculate the potential value
def potential_val(f):
	ret = 1
	v = f.pop(0)
	print "v: %s" % v

	if "+" in v:
		tmp = 0
		for e in f:
			tmp = tmp + potential_val(e)
		ret = tmp
	else:
		k = ""
		if "=" in v:
			var = v.split("=")[0].strip()
			val = v.split("=")[1].strip()
		else:
			var = v.strip()
			val = 1
		k = k + str(val)

		for e in f:
			if "=" in e:
				val = e.split("=")[1]
				k = k + str(val)
			else:
				k = k + str(1) # need some test

		print "p: p[%s][%s]" % (var, k)
		ret = p[var][k]

	return ret

# calculate the probability from the resultant factor
def calculation(rf, y):
	ret = 1
	for f in rf:
		ret = ret * potential_val(f)

	return ret
'''
def main():
	print calculation([['+', [['X4=0', 'X3']], [['X4=1', 'X3']]]], 'X3')

if __name__ == "__main__":
	main()
