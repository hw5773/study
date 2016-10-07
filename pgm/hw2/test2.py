import copy

def check(z, f):
	ret = False
	if not type(f) is list:
		if z == f:
			return True
		else:
			return False
	elif len(f) > 0:
		for e in f:
			ret = ret | check(z, e)
		return ret
	else:
		return False

def replace_val(z, f, v):
	ret = []
	if type(f) is str:
		if z in f:
			return v
		else:
			return f
	elif len(f) > 0:
		for e in f:
			ret.append(replace_val(z, e,v))
		return ret
	else:
		return []
		

def main():
	lst = [[[["a"], ["a", "b"]], "a"], "b", "c"]
	v = "a1"
	a = replace_val("a", lst, v)
	print a
	v = "a2"
	print replace_val("a", lst, v)
	print a

if __name__ == "__main__":
	main()
