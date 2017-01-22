import sys
from etc import domains

def main():
    f = sys.argv[1]
    addr = domains(f)
    print ("addr: ", addr)

if __name__ == "__main__":
    main()
