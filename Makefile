make:
	gcc -g -o main main.c parse.c datatypes.c hashmap.c

test:
	gcc -o tests/hash_test tests/HashMap_test.c hashmap.c

datatest:
	gcc -o tests/data_test tests/datatypes_test.c datatypes.c

run_tests:
	./tests/hash_test

clean:
	rm -f tests/*.o *.o main tests/data_test
