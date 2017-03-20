import os

def file_finder():
	csv_files = []
	for root, dirs, files in os.walk("./"):
		for f in files:
			if ".csv" in f:
				csv_files.append(f)

	return csv_files

def main():
	csv_files = file_finder()
	print ("csv_files: ", csv_files)

if __name__ == "__main__":
	main()
