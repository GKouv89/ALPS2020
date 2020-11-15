make:
	gcc -g -o main main.c parse.c datatypes.c hashmap.c set.c

test:
	gcc -o tests/hash_test tests/HashMap_test.c hashmap.c

datatest:
	gcc -o tests/data_test tests/datatypes_test.c datatypes.c

run_tests:
	./tests/hash_test

set:
	gcc -g -o tests/set_test tests/set_test.c set.c datatypes.c hashmap.c -DBUCKETS=3

clean:
	rm -f tests/*.o *.o main tests/data_test tests/set_test
