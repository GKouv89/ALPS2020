make:
	gcc -g -o main main.c parse.c datatypes.c hashmap.c

clean:
	rm -f *.o main
