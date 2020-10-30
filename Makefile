make:
	gcc -o main main.c parse.c

clean:
	rm -f *.o main
