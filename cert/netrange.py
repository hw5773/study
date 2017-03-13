import sys
import struct
import socket
import subprocess
import time 

def usage():
	print ("Crawl the domain name with the netrange")
	print ("python3 netrange.py <init IP address> <end IP address>")
	exit(1)

# Check whether the input address is valid
def check_valid(addr):
	addr_lst = addr.split(".")
	dot_count = len(addr_lst) - 1

	if dot_count < 3:
		while dot_count < 3:
			addr_lst[dot_count+1] = "0"

	if addr_lst[-1] == "0":
		addr_lst[-1] = "1"
		addr = ".".join(addr_lst)
	return addr

# Translate the IP address to the number
def ip2int(addr):
	return struct.unpack("!I", socket.inet_aton(addr))[0]

# Translate the number to IP address
def int2ip(addr):
	return socket.inet_ntoa(struct.pack("!I", addr))

# Initialize the output file
def output_file_init(init, end):
	output_filename = init + "_" + end + ".csv"
	output_file = open(output_filename, "w")

	output_file.write("Start IP, End IP, Organization\n")

	return output_file

# Terminate the output file
def output_file_end(of):
	of.close()

# Add hosts on the network
def add_hosts(ip, number):
	return check_valid(int2ip(ip2int(ip) + number))

# Check the whois command
def lookup(init_addr, end_addr, output_file):
	addr = init_addr
	
	while addr < end_addr:
		try:
			process = subprocess.Popen(["whois", int2ip(addr)], stdout=subprocess.PIPE)
			output = process.communicate()[0].decode('ISO-8859-1').replace("\t", "").split("\n")

			organization_lst = []

			start_ip = addr
			end_ip = addr

			for elem in output:
				if ("NetRange" in elem) or ("inetnum" in elem):
					if "-" in elem:
						elem_lst = elem.split(":")[1].strip().split("-")
						start_ip = elem_lst[0].strip()
						end_ip = elem_lst[1].strip()
					if "/" in elem:
						elem_lst = elem.split(":")[1].strip().split("/")
						network_number = int(elem_lst[1].strip())
						start_ip = check_valid(elem_lst[0].strip())
						end_ip = add_hosts(start_ip, network_number)

				if "@" in elem:
					org = elem.split("@")[1]
					if org not in organization_lst:
						organization_lst.append(org)

			organization = ""
			for elem in organization_lst:
				organization = organization + elem + "/"

			if (start_ip == "") or (end_ip == "") or len(organization_lst) < 1:
				output_file.write(start_ip + "," + end_ip + ",Need to Check\n")
			else:
				output_file.write(start_ip + "," + end_ip + "," + organization + "\n")

			addr = ip2int(check_valid(int2ip(ip2int(end_ip) + 1)))
			print(start_ip + "," + end_ip + "," + organization)
		
		except:
			output_file.write(int2ip(addr) + "," + int2ip(addr) + ",Need to Check\n")
			addr = ip2int(check_valid(int2ip(ip2int(end_ip) + 1)))

# Initialize
def main():
	if len(sys.argv) != 3:
		usage()

	start_time = time.time()
	init_addr = ip2int(sys.argv[1])
	end_addr = ip2int(sys.argv[2]) + 1
	output_file = output_file_init(sys.argv[1], sys.argv[2])
	lookup(init_addr, end_addr, output_file)
	output_file_end(output_file)
	end_time = time.time()
	print ("Elapsed Time: ", end_time - start_time)

if __name__ == "__main__":
	main()
