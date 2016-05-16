import os

for root, dirs, files in os.walk("../hw2/"):
	for d in dirs:
		os.system("python time_local.py " + d)
