﻿# Ανάπτυξη Λογισμικού για Πληροφοριακά Συστήματα 2020-21

Μέλη:

Γεωργία Κουτίβα, ΑΜ: 1115201700060
Αλέξανδρος Νεοφώτιστος, ΑΜ: 1115201700270

## Νέες Προσθήκες - 2η Άσκηση

### Δομές
 * **Stopword List**: Μία απλή συνδεδεμένη λίστα που περιέχει τις λέξεις που δίνονται στο αρχείο που παρέχεται στην εκφώνηση της εργασίας. 
 * **Vector**: Ένα απλό διάνυσμα με στοιχεία ακέραιους αριθμούς. Ο τύπος αυτός χρησιμοποιείται για όλα τα διανύσματα του Bag of Words και για το πρώτο διάνυσμα στο TF-IDF array. 
 * **IDFVector**: Ένα απλό διάνυσμα με στοιχεία αριθμούς κινητής υποδιαστολής διπλής ακρίβειας. Χρησιμοποιείται για τα διανύσματα του TF-IDF array εκτός από το πρώτο.
 * **Dictionary**: Ένα απλό δυαδικό δένδρο αναζήτησης. Οι λέξεις εισάγονται με κριτήριο την αλφαβητική τους σειρά. Οι κόμβοι του δένδρου, πέρα από δείκτες στο αριστερό και δεξί υποδένδρο τους, περιέχουν και το payload της λέξης: ένας αύξων αριθμός που υποδεικνύει την σειρά εισαγωγής της λέξης στο δένδρο. 
 * **Bag Of Words**: Ένας πίνακας από σταθερό πλήθος Vectors (ίσο με τον αριθμό των αρχείων στο dataset X, συν άλλο ένα που περιέχει τα payloads των λέξεων, όπως αυτές εμφανίζονται στο λεξικό).
 * **TF**: Ένας πίνακας που περιέχει τις τιμές TF-IDF για όλα τα κείμενα που περιέχονται στο dataset X. Περιέχει ένα απλό Vector, με τα payloads των λέξεων όπως αυτές εμφανίζονται στο λεξικό, και σταθερό πλήθος IDFVectors που περιέχουν τα TF-IDF values για κάθε κείμενο.
* **neg_list**: Μια λίστα που περιέχει τους κόμβους αρνητικής συσχέτισης (neg_nodes) ενός αντιπροσώπου θετικής κλίκας.
	* Ο όρος Αρνητική Συσχέτιση θα χρησιμοποιείται για στοιχεία μεταξύ των οποίων το matching value είναι 0.
* **neg_node**: Αντιπροσωπεύει έναν κόμβο αντιπρόσωπο θετικής κλίκας, ο οποίος έχει αρνητική συσχέτιση με έναν άλλο αντιπρόσωπο θετικής κλίκας. Μέσα περιέχεται ο “αρνητικός” αντιπρόσωπος μιας άλλης θετικής κλίκας, δείκτης για τον επόμενο της λίστας καθώς και το flag printed το οποίο χρησιμοποιείται για αποφυγή εκτύπωσης διπλοτύπων (η χρήση της αναφέρεται στην συνέχεια).
### Πως λειτουργούν οι νέες προσθήκες στο πρόγραμμά μας

 1. Για αρχή, δημιουργείται το stopword list. 
 2. Στον parser, ο οποίος διαβάζει τα αρχεία και εξακολουθεί να κρατά την tuple list, έχει ενσωματωθεί η δημιουργία του Bag of Words και του Λεξικού.
	 * Κάθε ένα από τα buffer που περιέχουν είτε το όνομα ενός attribute είτε την τιμή του δίνεται στην συνάρτηση bow_it. Αυτή με την σειρά της χωρίζει το string στο whitespace και αφαιρεί αριθμούς και σημεία στίξης, καθώς μετατρέπει επίσης όλα τα γράμματα σε lowercase. 
	 * Όταν συναντήσει whitespace και έχει επομένως διαβάσει μία λέξη, πάει να την εισάγει στο δένδρο. Αν αυτή ήδη υπάρχει, βρίσκει το payload της.
	 * Αν η λέξη δεν υπήρχε στο λεξικό, τότε πρέπει να προστεθεί μία νέα λέξη στο Bag of Words. Το payload προστίθεται στο πρώτο διάνυσμα και, αν αυτό έφτασε στην μέγιστη χωρητικότητά του, διπλασιάζεται σε μέγεθος τόσο αυτό όσο και τα υπόλοιπα διανύσματα του BoW. Κατόπιν, η κατάλληλη θέση του διανύσματος /κειμένου το οποίο επεξεργαζόμαστε γίνεται 1 (1η εμφάνιση της λέξης)
	 * Αν η λέξη υπήρχε στο λεξικό, τότε βρίσκουμε την θέση payload στο τρέχον διάνυσμα και αυξάνουμε την συχνότητα εμφάνισης κατά 1.
	 * Αν η λέξη δεν είχε ξαναεμφανιστεί στο κείμενο, αυξάνουμε το word_count του διανύσματος. Αυτό θα μας χρησιμεύσει στον υπολογισμό του TF.
	 * Παράλληλα, δημιουργούμε και το διάνυσμα που περιέχει τα idf values για όλες τις λέξεις.
		 * Για κάθε κείμενο στο οποίο εμφανίζεται μία λέξη, αυξάνουμε το αντίστοιχο στοιχείο ενός  IDFVector κατά 1.
3. Μετά την λήξη του parsing, αντιστρέφουμε τα στοιχεία του διανύσματος του IDFVector, πολλαπλασιάζουμε με το πλήθος των αρχείων, και λογαριθμούμε - υπολογίζουμε τα IDF values των λέξεων.
4. Κατόπιν, φτιάχνουμε τον πίνακα TF-IDF. Διαιρούμε τα στοιχεία κάθε vector του BoW με το word_count του vector και πολλαπλασιάζουμε με το IDFVector. 
5. Ταυτόχρονα, υπολογίζουμε το μέσο TF-IDF για κάθε λέξη
	* Αφού υπολογιστεί αυτό, αποδεσμεύουμε το BoW.
6. Αντιγράφουμε το Vector του TF-IDF πίνακα με τα payloads των λέξεων και ταξινομούμε τόσο αυτό όσο και το IDFVector με τα μέσα TF-IDF σε φθίνουσα σειρά.
	* Χρησιμοποιείται ο αλγόριθμος Quicksort.
7. Κάνουμε crop το Vector με τα payloads, κρατώντας τα πρώτα IMPWORDS στοιχεία (ορισμένο στα 1000)
8. Κάνουμε sort το Vector αυτό με αύξουσα σειρά αυτή την φορά. Γνωρίζουμε επακριβώς τις στήλες που θα κρατήσουμε μετά το clipping του TF-IDF πίνακα
9. Δημιουργούμε έναν νέο πίνακα τύπου TF με ίδιο πλήθος διανυσμάτων ίσο με του παλιού και νέο μήκος διανυσμάτων ίσο με IMPWORDS. Αντιγράφουμε τις στήλες του παλιού πίνακα που αναγράφονται στο Vector που ταξινομήσαμε και αποδεσμεύουμε το  παλιό TF.
10. Ξεκινά η διαδικασία του training. Κάνουμε 5 epochs επάνω στο training set: βρίσκουμε μέσω του hashmap τον αριθμό vector του κάθε JSON file μέσα στον TF-IDF πίνακα, κάνουμε concatenate τα 2 διανύσματα, και υπολογίζουμε την τιμή σ(f(x)). Ανανεώνουμε τις συνιστώσες (ένας απλός πίνακας στοιχείων double) με βάση τον τύπο *(prediction - ground truth) x learning rate x xi* όπου xi η i-οστή τιμή TF-IDF του concatenated διανύσματος. Μετά από κάθε epoch, γίνεται μία επανάληψη του Validation Set. Εκτυπώνονται ορισμένα στατιστικά τόσο κατά την διάρκεια του validation όσο και κατά το testing (θα αναφερθούμε σε αυτά σύντομα). Τέλος, γίνονται προβλέψεις επάνω στο  Test Set. 
	* Τα Training, Validation, και Test sets είναι μερίδια derived dataset που προκύπτει από τον 'υπολογισμό' θετικών και αρνητικών κλικών, μεγέθους 60%, 20% και 20%,  αντίστοιχα.

### Εκτύπωση κλικών αρνητικής συσχέτισης αντιπροσώπου και η μεταβλητή printed:
* Κατά την δημιουργία κάθε κόμβου list_node, η μεταβλητή printed αρχικοποιείται με  1.
* Κατά την διάρκεια της συνάρτησης neglist_print, πριν ξεκινήσει η εκτύπωση, πραγματοποιείται ελεγχος της τιμής της μεταβλητής printed, η οποία καθορίζει αν ο συγκεκριμένος αντιπρόσωπος και η θετική κλίκα του, θα εκτυπωθούν.
	* Αν η printed του δεύτερου αντιπροσώπου είναι 1 τότε, αναζητούμε μέσα στους κόμβους της λίστας αρνητικής συσχέτισης του, τον πρώτο αντιπρόσωπο, θέτουμε την printed του 0 και συνεχίζουμε με την εκτύπωση.
	* Διαφορετικά, αν η printed είναι 0 τότε δεν γίνεται εκτύπωση και η συνάρτηση συνεχίζει στον επόμενο αντιπρόσωπο στην λίστα αρνητικής συσχέτισης με παρόμοιο τρόπο.
* Πρώτα θα εκτυπωθούν οι  δυο αντιπρόσωποι κλικών που περιέχουν στοιχεία που γνωρίζουμε οτι είναι διαφορετικά μοντέλα μεταξύ τους, στην συνέχεια ο πρώτος αντιπρόσωπος θα εκτυπωθεί με τα στοιχεία της δεύτερης κλίκας και τέλος τα στοιχεία της κλίκας του πρώτου αντιπροσώπου θα εκτυπωθούν με τον δεύτερο αντιπρόσωπο και τα στοιχεία της κλίκας αυτου.

### Τυχαιοποίηση Αποτελεσμάτων
Αφού τελειώσει το χώρισμα των αποτελεσμάτων στα αρχεία θετικών και αρνητικής συσχέτισης κλικών, το splitter.c αναλαμβάνει να συμπτύξει τα δύο προαναφερθέντα αρχεία σε ένα αρχείο που θα περιέχει όλες τις εισόδους των προηγουμένων με τυχαία σειρά, ενώ παράλληλα με την χρήση μιας rand γίνεται η επιλογή του σετ (training/validation/test) στο οποίο θα πάει.

```
void print_all_cliques(int opt, clique_list* l){
    clique_list_node *temp = l->front;

    int posc = 0;

    FILE *fp1; // for positive relationship cliques
    FILE *fp2; // for negative relationship cliques
    
    // Preparing files for result splitting 
    fp1 = fopen("poscliques.csv","w+");
    fp2 = fopen("negcliques.csv","w+");
    
    if(fp1 == NULL){
        fprintf(stderr, "Couldn't open poscliques.csv file.\n");
    }
    if(fp2 == NULL){
        fprintf(stderr, "Couldn't open negcliques.csv file.\n");
    }
    
    printf("left_spec_id, right_spec_id\n");
        while(temp){
            print_clique(opt, temp->representative, fp1);
            temp = temp->next;
        }
        temp = l->front;
        while(temp){
            neglist_print(temp->representative, fp2);
            temp = temp->next;
        }
    
    if(fclose(fp1)!= 0){
        fprintf(stderr, "Couldn't close poscliques.csv file.\n");
    }
    if(fclose(fp2)!= 0){
        fprintf(stderr, "Couldn't close negcliques.csv file.\n");
    }
}
```
***ΣΗΜΕΙΩΣΗ***: Αυτή η διαδικασία έγινε μια φορά για να δημιουργηθούν τα 3 σετς, για αυτό και η μη διατήρηση των δυο file pointers στην print_all_cliques.

### Μερικά συμπεράσματα για το Training

 - Επιλέξαμε την λήξη του training κατόπιν συγκεκριμένου αριθμού από epochs έναντι της σύγκλισης της διαφοράς των νέων με των παλιών συντελεστών καθώς παρατηρήσαμε ότι με τον δεύτερο τρόπο η διαδικασία έπαυε πολύ πριν γίνει το πρώτο πέρασμα του training set, ακόμα και αν το ε που ορίζαμε ήταν 0.001.
 - Η επιλογή του συγκεκριμένου threshold (=0.00041) έγινε μετά από απάντηση σε σχετική ερώτηση συναδέλφων στο Piazza (συγκεκριμένα, αναφερόμαστε στην ερώτηση 67). Παρατηρήσαμε τα εξής:
	 - Αρχικά, όσο διαβάζουμε τόσο label = 1 όσο και label = 0, οι τιμές είναι πιο κοντά στο 0.4. Όμως, καθώς υπάρχουν πολλά περισσότερα μηδενικά στο τέλος του αρχείου (όπου η εκτύπωση άσσων έχει εξαντληθεί), οι αριθμοί πέφτουν πάρα πολύ και πλησιάζουν το 0.0005.
	 - Επίσης, ο λόγος γραμμών με label = 1 προς γραμμές με label = 0 ήταν πάρα πολύ μικρός (για την ακρίβεια κοντά στο 0.07 για το test set  και κοντά στο 0.08 για το validation set).
	 - Επομένως, πειραματιστήκαμε με τιμές κοντά στο 0.0005. Καταλήξαμε στο συγκεκεριμένο threshold διότι:
		 -  Η τιμή για την οποία ο αριθμός σωστών προβλέψεων για label = 1 και ο αριθμός σωστών προβλέψεων για label = 0 ήταν ο πιο ισορροπημένος, και πλησιέστερος στο συνολικό ποσοστό accuracy. (Πάλι, ο αριθμός σωστών προβλέψεων για label = 1 ήταν χαμηλότερος, αλλά ελαφρά)
		 - Από περίπου 57% accuracy για το συγκεκριμένο threshold, πέφτουμε στο 42% για threshold = 0.00040, ενώ για αύξηση στο 0.00042 ανεβαίνουμε μόλις 3%. Ανάλογα μικρές είναι οι αυξήσεις κατά 0.00001 έως ότου φτάσουμε το 0.0005 όπου η ακρίβεια είναι ίδια σχεδόν με το 0.5 (δηλαδή ελάχιστες προβλέψεις μεγαλύτερες του 0.0005). Θεωρήσαμε ότι αυτή η απότομη πτώση στην απόδοση υποδεικνύει το threshold που πρέπει να πάρουμε λόγω του bias που έχει εισαχθεί στο μοντέλο. 
	 - Αν επιλέξετε να τρέξετε το πρόγραμμά μας για threshold = 0.5, θα δείτε ότι το ποσοστό επιτυχίας είναι 92% - μαντεύει σωστά όλα τα μηδενικά και κανέναν άσσο. Αν τρέξετε το πρόγραμμά μας για threshold = 0.00041 έχουμε accuracy περίπου 57% στο validation set (across all epochs) με περίπου 52% σωστές προβλέψεις για label = 1 και 56% σωστές προβλέψεις για label = 0. Παρόμοια αποτελέσματα λαμβάνουμε και για το test set. 

 

 

## 1η Άσκηση

### Οι δομές μας και τι αντιπροσωπεύουν

* list_node: Αντιπροσωπεύει ένα αρχείο .json, με το id του (με την μορφή με την οποία εμφανίζεται στα .csv αρχεία), τα attribute/key pairs του, και κάποιους δείκτες που θα εξηγηθούν παρακάτω
* info_list: Μία λίστα από list_nodes. Υπάρχει μία τέτοια για κάθε bucket του hashmap μας.
* node_tuple: Ένας κόμβος που περιέχει ένα attribute/key pair, μία γραμμή σε κάποιο αρχείο .json. Συνδέεται με τις υπόλοιπες γραμμές του εν λόγω αρχείου.
* tuplelist: Μία λίστα από node_tuples. Υπάρχει μία τέτοια για κάθε list_node. Καθώς δεν χρησιμοποιήσαμε το description των json files τους, αλλά μόνο το url/id τους, καταχωρίσαμε αυτά τα ζευγάρια σε μία απλή λίστα.
* hash_map/hash_bucket: Υλοποίηση ενός τυπικού στατικού πίνακα κατακερματισμού. Κάθε bucket περιέχει μία λίστα των list_nodes των οποίων τα ids αντιστοιχούν σε αυτόν.
	* Η επιλογή της συνάρτησης κατακερματισμού, καθώς και του αριθμού των buckets, έγινε μετά από αναζήτηση στο διαδίκτυο, εφόσον αναφέρθηκε ότι επιτρέπεται.
		* Διαβάσαμε πως είναι καλό κάθε χαρακτήρας του string id να συνεισφέρει στο αποτέλεσμα της hash function, 
		* καθώς και πως ο πολλαπλασιασμός με πρώτο και η επιλογή πρώτου αριθμού για αριθμό buckets δίνουν καλύτερα αποτελέσματα.
			* Σχετικές πηγές: https://stackoverflow.com/questions/2624192/good-hash-function-for-strings
			* https://computinglife.wordpress.com/2008/11/20/why-do-hash-functions-use-prime-numbers/
		* Μέσω της συνάρτησης print_bucket_no_of_entries, παρατηρήσαμε ότι ο μέσος όρος entries/bucket ήταν λογικός, καθώς και ότι δεν υπήρχαν τρομερές αποκλίσεις από αυτών.
		* Επιλέξαμε αυτή την δομή καθώς 'σπάει' το dataset σε πολλά μικρότερα κομμάτια που δεν είναι δύσκολο να διατρεχθούν γραμμικά.

Η δομή που επιλέχθηκε για την δημιουργία των κλικών είναι στην πραγματικότητα τα disjoint sets. Δεν υπάρχει ξεχωριστή δομή από το hashmap: κάθε spec σε μία γραμμή
στα .csv files με label = 1 περνά από την hash_function, βρίσκουμε τον αντίστοιχο κόμβο στην λίστα του bucket, και επάνω σε έναν δείκτη σε αυτόν τον κόμβο κάνουμε
πράξεις disjoint sets ανάλογες με την υλοποίηση των ανοδικών δένδρων. Το πεδίο parent σε έναν list_node είναι αυτό που χρησιμοποιείται για την υλοποίηση της εύρεσης ρίζας και της ένωσης συνόλων.
Όμως, για επιτάχυνση της εκτύπωσης των αποτελεσμάτων κρατάμε μία λίστα με τους αντιπροσώπους των συνόλων/κλικών. Αυτή είναι η clique_list. Τα nodes της είναι
οι αντιπρόσωποι των disjoint sets/cliques και το πεδίο next_in_line ακολουθείται για τον καθέναν από αυτούς, σχηματίζοντας μία λίστα με όλους τους κόμβους που
ανήκουν στην κλίκα. Αυτό το πεδίο ανανεώνεται με κάθε ένωση συνόλων.

### Πως λειτουργεί το πρόγραμμά μας

Πρώτα, γίνεται το parsing του dataset X. Δημιουργείται το list_node, το tuplelist για κάθε αρχείο και ο κόμβος προστίθεται στον hashmap.
Μετά, γίνεται το parsing του dataset W ή Υ, ανάλογα με το πιο έχει επιλεχθεί (περισσότερα για αυτό σύντομα). Σε κάθε γραμμή με label = 1, βρίσκουμε τους κόμβους
μέσα στο hashmap των left_spec, right_spec και κάνουμε ένωση των κλικών στις οποίες ανήκουν. Τέλος, εκτυπώνεται η λίστα με όλες τις κλίκες.

## Πως να τρέξετε το πρόγραμμά μας

Το path για τα dataset είναι defined, και πρέπει και τα 3 να βρίσκονται στον ίδιο φάκελο με τα .c αρχεία μας.

* Για να μεταγλωττίσετε το πρόγραμμα με σκοπό να τρέξει για το medium dataset,
        ```
                make medium
        ```

* Για να μεταγλωττίσετε το πρόγραμμα με σκοπό να τρέξει για το large dataset,
        ```
                make large
        ```
* Για να τρέξετε το πρόγραμμα με έξοδο στο stdout,
        ```
                make run
        ```
* Για να τρέξετε το πρόγραμμα με έξοδο στο αρχείο output.txt
        ```
                make run_out_file
        ```
## Unit testing

* Για τον parser του dataset X, δεν υπάρχει κάποιο διαθέσιμο unit test, ο έλεγχος για την ορθότητά του έγινε χειρωνακτικά μέσω αλλεπάλληλων εκτυπώσεων.

* Για το map και το info_list, το testing είναι trivial

* Για τις κλίκες, δοκιμάζεται:
        * Η δημιουργία μίας κλίκας από ορισμένα test_vectors
        * Η εισαγωγή και η διαγραφή από την λίστα των αντιπροσώπων κλικών (όταν δύο κλίκες συνενώνονται, η μεγαλύτερη 'απορροφά' την μικρότερη και έτσι ο αντιπρόσωπος αυτής αφαιρείται από την συνολική >        * Η δημιουργία δύο κλικών από ορισμένα test_vectors

* Για τις αρνητικές κλίκες, δοκιμάζεται:
	* Η δημιουργία θετικής και αρνητικής κλίκας από ορισμένα test_vectors
	* Η εισαγωγή κόμβων-αντιπροσώπων της θετικής κλίκας, στην κλίκα αρνητικών συσχετίσεων ενός αντιπροσώπου

* Για τις αρνητικές κλίκες, δοκιμάζεται:
	* Η δημιουργία θετικής και αρνητικής κλίκας από ορισμένα test_vectors
	* Η εισαγωγή κόμβων-αντιπροσώπων της θετικής κλίκας, στην κλίκα αρνητικών συσχετίσεων ενός αντιπροσώπου

* Για το parsing των csv αρχείων, γίνονται έλεγχοι ότι διαβάζονται όλες οι γραμμές (φτάνουμε στο EOF με ασφάλεια), καθώς και ότι ο αριθμός γραμμών με label = 1 είναι ο αναμενόμενος.
* Για το BoW, τον πίνακα TF-IDF, και τις συναρτήσεις του logistic regression, δημιουργήσαμε ένα παράδειγμα με 2 test vectors που περιείχαν payloads λέξεων. Δοκιμάζεται:
	* Η εισαγωγή νέων λέξεων/payloads στο bag of words, καθώς και λέξεων που εμφανίζονται πάνω από μία φορά.
	* Η καταγραφή των λέξεων που κάθε κείμενο περιέχει.
	* Ο αριθμός εμφανίσεων κάθε λέξης σε κάθε κείμενο.
	* Η τιμή των TF-IDF διανυσμάτων (δεν επιβεβαιώνεται μέσω assert, λόγω της επιλογής αριθμών διπλής ακρίβειας, αλλά έχει επιβεβαιωθεί χειρωνακτικά από εμάς)
	* Ο υπολογισμός του μέσου TF-IDF
	* Η ταξινόμηση των διανυσμάτων μέσου TF-IDF (με φθίνουσα σειρά) και payloads σημαντικότερων λέξεων (με αύξουσα σειρά)
	* Η ισότητα των στηλών που κρατούνται στο νέο, μικρό IDF με τις αρχικές στήλες.
	* Η σωστή τιμή των συναρτήσεων σ(f(x)) και η σωστή ανανέωση των συνιστωσών (αυτά τα αποτελέσματα δεν επιβεβαιώνονται μέσω assert, λόγω της επιλογής αριθμών διπλής ακρίβειας, αλλά έχουν επιβεβαιωθεί χειρωνακτικά από εμάς).

Για να τρέξετε όλα τα tests, αρκεί να τρέξετε:
        ```
                make run_all_tests
        ```

Τέλος,
	```
        make clean
        ```
διαγράφει το εκτελέσιμο του προγράμματος και
        ```
                make clean_tests
        ```
διαγράφει τα εκτελέσιμα των tests.
