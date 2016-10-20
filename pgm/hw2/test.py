from cpt import p
from gaussian import gaussian
def main():
	for i in range(100):
		val = gaussian(i, 50, 10)

		if val < 0 or val > 1:
			print "Error(val): ", val

if __name__ == "__main__":
	main()
