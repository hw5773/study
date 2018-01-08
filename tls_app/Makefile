
#testing:
#	gcc -Wall -o server.out server/TLS_Server.c ../log/logger.c -L/usr/lib -lssl -lcrypto
#	gcc -Wall -o client.out client/TLS_Client.c ../log/logger.c -L/usr/lib -lssl -lcrypto

#printing:
#	gcc -Wall -o server.out server/TLS_Server.c ../log/logger.c -L/usr/lib -lssl -lcrypto -DPRINT_ON
#	gcc -Wall -o client.out client/TLS_Client.c ../log/logger.c -L/usr/lib -lssl -lcrypto -DPRINT_ON

crypto:
	gcc -Wall -o server_on.out server/TLS_Server.c -DLOG_ON -I../../openssl_crypto/include -L../../openssl_crypto/lib -lssl  -lcrypto -ldl  
	gcc -Wall -o client_aes.out client/TLS_Client.c -I../../openssl_aes/include -L../../openssl_aes/lib -lssl -lcrypto -ldl -DLOG_ON
	gcc -Wall -o client_on.out client/TLS_Client.c -I../../openssl_crypto/include -L../../openssl_crypto/lib -lssl -lcrypto -ldl -DLOG_ON
	gcc -Wall -o client_on2.out client/TLS_Client_crypto.c -I../../openssl_crypto/include -L../../openssl_crypto/lib -lssl -lcrypto -ldl -DLOG_ON

nocrypto:
	gcc -Wall -o server_off.out server/TLS_Server.c -I../../openssl_nocrypto/include/ -L../../openssl_nocrypto/lib -lssl -lcrypto -ldl -DLOG_ON
	gcc -Wall -o client_off.out client/TLS_Client.c -I../../openssl_nocrypto/include/ -L../../openssl_nocrypto/lib -lssl -lcrypto -ldl -DLOG_ON
	
wolfssl:
	gcc -Wall -o wserver4.out server/TLS_WServer4.c -L/usr/lib -lwolfssl -lm
	gcc -Wall -o wclient.out client/TLS_WClient.c -L/usr/lib -lwolfssl -lm
#	gcc -Wall -o wserver.out server/TLS_WServer.c -L/usr/lib -lwolfssl -lm
#	gcc -Wall -o wclient.out client/TLS_WClient.c -L/usr/lib -lwolfssl -lm
#	gcc -Wall -o eclient.out client/ecc.c -L/usr/lib -lwolfssl -lm
#	gcc -Wall -o eserver.out server/ecc.c -L/usr/lib -lwolfssl -lm

server_test:
	echo "server run source for test"	
	sudo ./server_off.out 7080 ../valid_server_cert.pem ../cert/demoCA/cacert.pem ../cert/valid_server/privkey.pem a.txt

client_test:
	echo "client run source for test"	
	./client_aes.out fe80::7edd:90ff:fe7c:3411%eth0 7080 ../valid_client_cert.pem ../cert/demoCA/cacert.pem ../cert/valid_client/privkey.pem b.txt

wserver_test:
	echo "server run source for test"
	./wserver4.out 8081 ../cert/ecc/server_cert.pem ../cert/ecc/serverpriv.pem a.txt ../cert/ecc/cacert.pem
#	./wserver4.out 8081 ../cert/ecc/server.pem ../cert/ecc/serverpriv.pem a.txt ../cert/ecc/cacert.pem

wclient_test:
	echo "client run source for test"
	./wclient.out 147.46.215.152 8081 ../cert/2016/client.pem ../cert/2016/cacert.pem ../cert/2016/clientpriv.pem b.txt

eclient_test:
	echo "client run source for test"
	./eclient.out 147.46.215.152 3435 ../cert/ecc3/client.pem ../cert/ecc3/cacert.pem ../cert/ecc3/clientpriv.pem b.txt

eserver_test:
	echo "server run source for test"
	sudo ./eserver.out 3435 ../cert/ecc3/server.pem ../cert/ecc3/serverpriv.pem a.txt
