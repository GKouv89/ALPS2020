make:
	gcc -o main main.c tuplist.c parse.c

parsetest:
	gcc -o parsetest test/parsetest.c parse.c tuplist.c
	./parsetest

newparsetest:
	gcc -o newparsetest test/newparsetest.c parse.c tuplist.c
	./newparsetest

clean:
	rm -f *.o main
