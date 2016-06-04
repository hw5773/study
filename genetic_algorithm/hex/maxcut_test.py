import os
import sys

start = float(sys.argv[1])
end = float(sys.argv[2])
inc = float(sys.argv[3])

m = start

while m < end:
	os.system("./maxcut_test ../graph/maxcut500.txt original_test/mutation" + str(m) + ".res original_test/mutation" + str(m) +".csv " + str(m))
	m = m + inc


