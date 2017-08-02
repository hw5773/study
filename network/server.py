import socket
import threading

bind_ip = "0.0.0.0"
bind_port = 9999

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))

server.listen(2)

print "[*] Server is Listening on %s:%d" % (bind_ip, bind_port)

def handler(client_sock):
	request = client_sock.recv(1024)
	print "[*] Received: %s" % request

	client_sock.send("Hello!")
	client_sock.close()

while True:
	client,addr = server.accept()

	print "[*] Server accept the connection from %s:%d" % (addr[0], addr[1])

	client_handle = threading.Thread(target=handler, args=(client,))
	client_handle.start()			
