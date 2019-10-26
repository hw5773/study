all:
	gcc -DDEBUG -c json_parser.c
	gcc -DDEBUG -c json_internal.c
	gcc -DDEBUG -c test_json.c
	gcc -o test_json test_json.o json_parser.o json_internal.o
clean:
	rm test_json *.o
