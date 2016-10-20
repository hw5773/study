import copy
from cpt import p

# check whether the variable is included in the factor
def check(z, f):
	ret = False

	if type(f) is str:
		if z in f:
			return True
		else:
			return False
	else:
		for e in f:
			ret = ret | check(z, e)
		return ret

# extract variables involved in the evidence list
def var_e(elst):
	ret = []
	for e in elst:
		v = e.split("=")[0].strip()
		ret.append(v)

	return ret

# apply the evidence to each factor
def replace_factor(sf, elst):

	vlst = []
	for e in elst:
		lst = e.split("=")
		vlst.append(lst[0].strip())

	sf = [x for x in sf if x[0] not in vlst]

	for e in elst:
		lst = e.split("=")
		var = lst[0].strip()
		val = lst[1].strip()
		evidence = var + "=" + val

		for f in sf:
			if var in f:
				idx = f.index(var)
				f.remove(var)
				f.insert(idx, evidence)

	return sf

# sum over the factor by the variable
def sum_over(s, z):
	ret = ["+"]

	for val in range(2):
		v = z + "=" + str(val)

		tmp = copy.deepcopy(s)

		for f in tmp:
			if check(z, f):
				idx = tmp.index(f)
				tmp.pop(idx)
				tmp.insert(idx, replace_val(z, f, v))
		ret.append(tmp)

	return ret

# replace variable into val
def replace_val(z, f, v):
	ret = []
	if type(f) is str:
		if z in f:
			return v
		else:
			return f
	elif len(f) > 0:
		for e in f:
			ret.append(replace_val(z, e, v))
		return ret
	else:
		return f

# calculate the potential value
def potential_val(f, y):
	ret = 1
	v = f.pop(0)

	if "+" in v:
		tmp = 0
		for e in f:
			tmp = tmp + calculation(e, y)
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

		ret = p[var][k]

	return ret

# calculate the probability from the resultant factor
def calculation(rf, y):
	ret = 1
	for f in rf:
		v = potential_val(f, y)
		ret = ret * v

	return ret
