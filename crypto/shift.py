ciphertext = "JGRMQOYGHMVBJWRWQFPWHGFFDQGFPFZRKBEEBJIZQQOCIBZKLFAFGQVFZFWWE\n" \
"OGWOPFGFHWOLPHLRLOLFDMFGQWBLWBWQOLKFWBYLBLYLFSFLJGRMQBOLWJVFP\n" \
"FWQVHQWFFPQOQVFPQOCFPOGFWFJIGFQVHLHLROQVFGWJVFPFOLFHGQVQVFILE\n" \
"OGQILHQFQGIQVVOSFAFGBWQVHQWIJVWJVFPFWHGFIWIHZZRQGBABHZQOCGFHX\n"

print ciphertext

frequency = {}
refer = {}
double = {}
triple = {}
quad = {}

for w in ciphertext:
	if w in frequency.keys():
		frequency[w] = frequency[w] + 1
	else:
		frequency[w] = 1

for k in frequency.keys():
	frequency[k] = round((1.0 * frequency[k]) / len(ciphertext) * 100, 2)

print frequency

order = sorted(frequency.keys(), key = lambda x : -1 * frequency[x])

for i in range(len(ciphertext)-1):
	k = ciphertext[i] + ciphertext[i+1]
	if k in double.keys():
		double[k] = double[k] + 1
	else:
		double[k] = 1

#double = sorted(double.keys(), key = lambda x : -1 * double[x])
lst = []
for k in double.keys():
	if double[k] > 1 and len(k) == 2:
		lst.append(k)
lst.sort(reverse = True)
print lst

for i in range(len(ciphertext)-2):
	k = ciphertext[i] + ciphertext[i+1] + ciphertext[i+2]
	if k in triple.keys():
		triple[k] = triple[k] + 1
	else:
		triple[k] = 1

#triple = sorted(triple.keys(), key = lambda x : -1 * triple[x])
lst = []
for k in triple.keys():
	if (triple[k] > 1) and (len(k) == 3):
		lst.append(k)
lst.sort(reverse = True)
print lst

for i in range(len(ciphertext)-3):
	k = ciphertext[i] + ciphertext[i+1] + ciphertext[i+2] + ciphertext[i+3]
	if k in quad.keys():
		quad[k] = quad[k] + 1
	else:
		quad[k] = 1

#quad = sorted(quad.keys(), key = lambda x : -1 * quad[x])
lst = []
for k in quad.keys():
	if (quad[k] > 1) and (len(k) == 4):
		lst.append(k)

lst.sort(reverse = True)
print lst

refer["a"] = 8.2
refer["b"] = 1.5
refer["c"] = 2.8
refer["d"] = 4.3
refer["e"] = 12.7
refer["f"] = 2.2
refer["g"] = 2.0
refer["h"] = 6.1
refer["i"] = 7.0
refer["j"] = 0.2
refer["k"] = 0.8
refer["l"] = 4.0
refer["m"] = 2.4
refer["n"] = 6.7
refer["o"] = 1.5
refer["p"] = 1.9
refer["q"] = 0.1
refer["r"] = 6.0
refer["s"] = 6.3
refer["t"] = 9.1
refer["u"] = 2.8
refer["v"] = 1.0
refer["w"] = 2.4
refer["x"] = 0.2
refer["y"] = 2.0
refer["z"] = 0.1

print order

reference = sorted(refer.keys(), key = lambda x : -1 * refer[x])
print reference
result = ""

print ciphertext

for w in ciphertext:
	if w == "F":
		result = result + "e"
	elif w == "W":
		result = result + "a"
	elif w == "V":
		result = result + "h"
	elif w == "Q":
		result = result + "t"
	elif w == "A":
		result = result + "s"
	elif w == "Z":
		result = result + "r"
	elif w == "\n":
		result = result + "\n"
	else:
		result = result + "_"

print result
