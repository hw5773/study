import os
import sys

start = float(sys.argv[1])
end = float(sys.argv[2])
inc = float(sys.argv[3])

m = start

while m < end:
	os.system("./maxcut ../graph/maxcut500.txt test/m" + m + ".res test/m" + m +".csv " + m
	m = m + inc


