medium:
	gcc -g -o main main.c parse.c datatypes.c hashmap.c set.c tuplist.c -DDATASET=\"sigmod_medium_labelled_dataset.csv\"

large:
	gcc -g -o main main.c parse.c datatypes.c hashmap.c set.c tuplist.c

run:
	./main

run_out_file:
	rm -f output.csv
	./main >> output.csv

hashtest:
	gcc -o tests/hash_test tests/HashMap_test.c hashmap.c datatypes.c tuplist.c

datatest:
	gcc -o tests/data_test tests/datatypes_test.c datatypes.c tuplist.c

cliquetest:
	gcc -g -o tests/cliques tests/set_test.c set.c datatypes.c hashmap.c -DBUCKETS=3 tuplist.c

csvparsetest:
	gcc -o tests/csvparse tests/csvparse.c tuplist.c

arrayparsetest:
	gcc -o tests/arrayparse tests/arrayparse.c

run_all_tests:
	make hashtest
	make datatest
	make cliquetest
	make csvparsetest
	make arrayparsetest
	./tests/hash_test
	./tests/data_test
	./tests/cliques
	./tests/csvparse
	./tests/arrayparse

clean:
	rm -f *.o main

clean_tests:
	rm -f tests/*.o tests/hash_test tests/data_test tests/cliques tests/csvparse tests/arrayparse
