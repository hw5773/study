ciphertext = "JGRMQOYGHMVBJWRWQFPWHGFFDQGFPFZRKBEEBJIZQQOCIBZKLFAFGQVFZFWWE" \
"OGWOPFGFHWOLPHLRLOLFDMFGQWBLWBWQOLKFWBYLBLYLFSFLJGRMQBOLWJVFP" \
"FWQVHQWFFPQOQVFPQOCFPOGFWFJIGFQVHLHLROQVFGWJVFPFOLFHGQVQVFILE" \
"OGQILHQFQGIQVVOSFAFGBWQVHQWIJVWJVFPFWHGFIWIHZZRQGBABHZQOCGFHX"

frequency = {}

for w in ciphertext:
	if w in frequency.keys():
		frequency[w] = frequency[w] + 1
	else:
		frequency[w] = 1

lst = []
for i in range(65, 91):
	if chr(i) in frequency.keys():
		lst.append(round(1.0 * frequency[chr(i)]/len(ciphertext), 4))
	else:
		lst.append(0)
print lst

coincidence = 0
for v in lst:
	coincidence = coincidence + v * v
print coincidence

for i in range(len(lst)):
	coincidence = 0
	for j in range(len(lst)):
		coincidence = coincidence + lst[i] * lst[(i+j)%26]
	print "%d: %.3f" % (i, coincidence)

refer = []
refer.append(8.2)
refer.append(1.5)
refer.append(2.8)
refer.append(4.3)
refer.append(12.7)
refer.append(2.2)
refer.append(2.0)
refer.append(6.1)
refer.append(7.0)
refer.append(0.2)
refer.append(0.8)
refer.append(4.0)
refer.append(2.4)
refer.append(6.7)
refer.append(7.5)
refer.append(1.9)
refer.append(0.1)
refer.append(6.0)
refer.append(6.3)
refer.append(9.1)
refer.append(2.8)
refer.append(1.0)
refer.append(2.3)
refer.append(0.1)
refer.append(2.0)
refer.append(0.1)

values = []

for v in refer:
	values.append(round(v/100, 4))
print values

sum_of_vals = 0

for i in range(len(values)):
	sum_of_vals = 0
	for j in range(len(values)):
		sum_of_vals = sum_of_vals + values[j] * values[(j + i) % 26]
	print "%d: %.3f" % (i, round(sum_of_vals, 3))
