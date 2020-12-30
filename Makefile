FLAGS = -g -o
MODULES =  parse.c datatypes.c hashmap.c set.c tuplist.c negcl.c BOW/stringOps.c BOW/vectorOps.c BOW/stopwords.c BOW/bow.c BOW/dictionary.c TF-IDF/idfVectorOps.c TF-IDF/tf.c logreg.c -lm


medium:
	gcc $(FLAGS) main main.c $(MODULES) -DDATASET=\"sigmod_medium_labelled_dataset.csv\" -DVECTORS=29788 -DTFVECTORS=29787

large:
	gcc $(FLAGS) main main.c $(MODULES) -DVECTORS=29788 -DTFVECTORS=29787

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
	gcc -g -o tests/ctype tests/ctypetest.c tuplist.c BOW/stringOps.c BOW/stopwords.c BOW/dictionary.c BOW/bow.c BOW/vectorOps.c -DVECTORS=2

vectest:
	gcc $(FLAGS) tests/vectest tests/vectest.c BOW/vectorOps.c

bowtest:
	gcc $(FLAGS) tests/bowtest tests/bow_test.c BOW/vectorOps.c hashmap.c datatypes.c tuplist.c negcl.c logreg.c -DIMPWORDS=3 -DCAPACITY=5 -DVECTORS=3 BOW/bow.c TF-IDF/idfVectorOps.c TF-IDF/tf.c -DTFVECTORS=2 -DFILES=2 -lm

dicttest:
	gcc $(FLAGS) tests/dicttest tests/dicttest.c BOW/dictionary.c

allbowstructstest:
	gcc $(FLAGS) tests/allbowtest tests/allBowStructsTest.c BOW/dictionary.c BOW/stopwords.c BOW/bow.c BOW/vectorOps.c BOW/stringOps.c -DVECTORS=6 TF-IDF/idfVectorOps.c -DFILES=5 TF-IDF/tf.c -lm

trainingSetParse:
	gcc $(FLAGS) parseTrain tests/parseTrainingSetTest.c $(MODULES)

run_all_tests:
	make hashtest
	make datatest
	make cliquetest
	make csvparsetest
	make ngltest
	make arrayparsetest
	make bowtest
	make dicttest
	./tests/hash_test
	./tests/data_test
	./tests/cliques
	./tests/csvparse
	./tests/ngltest
	./tests/arrayparse
	./tests/ctype
	./tests/bowtest
	./tests/dicttest

clean:
	rm -f *.o main
	rm -f *.o splitter

clean_tests:
	rm -f tests/*.o tests/hash_test tests/data_test tests/cliques tests/csvparse tests/arrayparse tests/ctype tests/bowtest tests/dicttest tests/allbowtest
