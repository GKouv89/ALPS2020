make:
	gcc -g -o main main.c parse.c datatypes.c hashmap.c set.c

run:
	./main

run_out_file:
	rm -f output.txt
	make run >> output.txt

hashtest:
	gcc -o tests/hash_test tests/HashMap_test.c hashmap.c datatypes.c

datatest:
	gcc -o tests/data_test tests/datatypes_test.c datatypes.c

cliquetest:
	gcc -g -o tests/cliques tests/set_test.c set.c datatypes.c hashmap.c -DBUCKETS=3

csvparsetest:
	gcc -o tests/csvparse tests/csvparse.c 

run_all_tests:
	make hashtest
	make datatest
	make cliquetest
	make csvparsetest
	./tests/hash_test
	./tests/data_test
	./tests/cliques
	./tests/csvparse

clean:
	rm -f *.o main

clean_tests:
	rm -f tests/*.o tests/hash_test tests/data_test tests/cliques tests/csvparse
