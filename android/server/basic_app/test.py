import os
import sys

java_files = []				# the list of java files
rel_set = []				# the relation list
instance_to_class = {}		# the mapping between instance and class
curr_class = ""				# the name of the current class

keywords = ["try", "catch", "while"]
types = ["String", "int", "float", "double"]

def parse_left(left):
	left_lst = left.strip().split(" ")
	var = left_lst[-1]

	if len(left_lst) == 2:
		pre = left_lst[0].strip()
		if pre not in types:
			instance_to_class[var] = pre
	elif len(left_lst) > 2:
		print "more than 2: %s" % left
	
	return var	

def parse_right(var, right):
	s1 = set([])
	s1.add(var)
	if "new" in right:
		idx = right.index("new")
		num = idx + 3
		val = right[num:num+right[num:].index("(")].strip()
		instance_to_class[var] = val
		s1.add(val)
		
	elif right[0] == "(":
		val = right[right.index(")")+1:].strip()
		s1.add(val)

		try:
			idx1 = val.index("(")
		except:
			idx1 = -1

		if idx1 > 0:
			try:
				idx2 = val.index(")")
			except:
				idx2 = -1
			if idx2 > 0:
				val = val[idx1+1:idx2]
				s1.add(val)

	add_rel(s1)

def parse_line(line, cname, fname):
	line = line.strip()

	if "=" in line:
		idx = line.index("=")
		left = line[0:idx].strip()
		var = parse_left(left)
		right = line[idx+1:].strip()
		parse_right(var, right)

	if "return" in line:
		idx = line.index(" ")
		val = line[idx+1:]
		s1 = set([])
		s1.add(fname)
		s1.add(val)
		add_rel(s1)
		
def add_rel(s1):
	contain = False
	for elem in rel_set:
		s2 = set(elem)

		if s1 & s2:
			contain = True
			rel_set.remove(elem)
			rel_set.append(list(s1 | s2))

	if not contain:
		if list(s1):
			rel_set.append(list(s1))

def parsing(blk, cname, fname):
	c = ""
	l = ""
	i = 0

	skip = False
	comment = False
	in_str = False

	while len(blk) > 0:
		c = blk[0]
		blk = blk[1:]

		if skip:
			if c == "\n":
				skip = False
			continue
		elif comment:
			if c == "*" and blk[0] == "/":
				comment = False
				blk = blk[1:]
			continue
		else:
			if c == "@":
				skip = True
				l = ""
				continue
			elif c == "\"":
				if in_str == True:
					in_str = False
				else:
					in_str = True
			elif c == "/" and blk[0] == "/" and not (in_str == True):
				skip = True
				l = ""
				blk = blk[1:]
				continue
			elif c == "/" and blk[0] == "*":
				comment = True
				blk = blk[1:]
				l = ""
				continue
			elif c == "(":
				stack = [1]
				b = c
				while len(stack) > 0:
					c = blk[0]
					blk = blk[1:]
					b = b + c
					if c == "(":
						stack.append(1)
					elif c == ")":
						stack.pop()
					
					if len(stack) == 0:
						c = b
			elif c == "\n" or c == "\r":
				continue
			elif c == ";":
				print l.strip()
				parse_line(l, cname, fname)
				l = ""
				continue
			elif c == "{":
				stack = [1]
				b = ""
				l = l.strip()
				print "before block: %s" % l
				print "block start"
				while len(stack) > 0:
					c = blk[0]
					blk = blk[1:]
					if c == "{":
						stack.append(1)
					elif c == "}":
						stack.pop()

					if len(stack) > 0:
						b = b + c

				try:
					idx = l.index("(")
					tmp = l[0:idx].strip()
					fname = tmp.split(" ")[-1]
					print "fname: %s" % fname
					if fname in keywords:
						print "it's in keywords"
						parsing(b, cname, "")
					else:
						print "it's function name"
						parsing(b, cname, fname)
				except:
					parsing(b, cname, "")
				l = ""
				print "block end"
				continue

			l = l + c

for root, dirs, files in os.walk("."):
	for f in files:
		if ".java" in f:
			java_files.append(f)

print "java_files: "
print java_files

l = ""

#java_files = ["GpsInfo.java"]

for f in java_files:
	fi = open(f, "r")
	l = fi.read()
	cname = f.split(".")[0]
	print "cname: %s" % cname

	print fi
	parsing(l, cname, "")
	print "\n"

print "rel_set: "
for s in rel_set:
	print s

print "instance_to_class: "
print instance_to_class
