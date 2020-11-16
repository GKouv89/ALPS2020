make:
	gcc -g -o main main.c tuplist.c parse.c hashmap.c datatypes.c set.c

parsetest:
	gcc -o parsetest test/parsetest.c parse.c tuplist.c
	./parsetest

clean:
	rm -f *.o main
