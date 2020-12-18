FLAGS = -g -o
MODULES =  main.c parse.c datatypes.c hashmap.c set.c tuplist.c negcl.c BOW/stringOps.c BOW/vectorOps.c BOW/stopwords.c

medium:
	gcc $(FLAGS) main $(MODULES) -DDATASET=\"sigmod_medium_labelled_dataset.csv\"

large:
	gcc $(FLAGS) main $(MODULES)

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

ngltest:
	gcc $(FLAGS) tests/ngltest tests/ngltest.c negcl.c set.c datatypes.c hashmap.c -DBUCKETS=3 parse.c tuplist.c -DDATASET=\"sigmod_medium_labelled_dataset.csv\"

ctypetest:
	gcc -g -o tests/ctype tests/ctypetest.c tuplist.c BOW/stringOps.c BOW/stopwords.c

vectest:
	gcc $(FLAGS) tests/vectest tests/vectest.c BOW/vectorOps.c

bowtest:
	gcc $(FLAGS) tests/bowtest tests/bow_test.c BOW/vectorOps.c -DCAPACITY=5 -DVECTORS=3 BOW/bow.c

run_all_tests:
	make hashtest
	make datatest
	make cliquetest
	make csvparsetest
	make ngltest
	make arrayparsetest
	make bowtest
	./tests/hash_test
	./tests/data_test
	./tests/cliques
	./tests/csvparse
	./tests/ngltest
	./tests/arrayparse
	./tests/ctype
	./tests/bowtest

clean:
	rm -f *.o main

clean_tests:
	rm -f tests/*.o tests/hash_test tests/data_test tests/cliques tests/csvparse tests/arrayparse tests/ctype tests/bowtest
