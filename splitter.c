#include <stdio.h>
#include <stdlib.h>

void main(){
    FILE *fp1; // for positive relationship cliques
    FILE *fp2; // for negative relationship cliques
    FILE *fp3; // For random order results

    FILE *trainp; // Pointer to Training Set
    FILE *validp; // Pointer to Validation Set
    FILE *testp; // Pointer to Test Set
    
    
    
    trainp = fopen("TrainingSet.csv","w+");
    validp = fopen("ValidationSet.csv","w+");
    testp = fopen("TestSet.csv","w+");
    
    if(trainp == NULL){
        fprintf(stderr, "Couldn't open TrainingSet.csv file.\n");
    }
    if(validp == NULL){
        fprintf(stderr, "Couldn't open ValidationSet.csv file.\n");
    }
    if(testp == NULL){
        fprintf(stderr, "Couldn't open TestSet.csv file.\n");
    }
    
    fp1 = fopen("poscliques.csv","r");
    fp2 = fopen("negcliques.csv","r");
    fp3 = fopen("randresults.csv","w+");
    
    if(fp1 == NULL){
        fprintf(stderr, "Couldn't open poscliques.csv file.\n");
    }
    if(fp2 == NULL){
        fprintf(stderr, "Couldn't open negcliques.csv file.\n");
    }
    if(fp3 == NULL){
        fprintf(stderr, "Couldn't randresults.csv file.\n");
    }
    
    //Creating random order results file
    int randomizer;
    double setnum;
    char *temps;
    size_t *n;
    temps = malloc(300*sizeof(char));
    // fseek(fp1, 0, SEEK_SET);
    // fseek(fp2, 0, SEEK_SET);
    randomizer = (rand()%2);
    while(feof(fp1)!=1 && feof(fp2)!=1){
        if(randomizer==1){
            getline(&temps,n,fp1);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }else{
            getline(&temps,n,fp2);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }
    }
    if(feof(fp1)){
        while(feof(fp2)!=1){
            getline(&temps,n,fp2);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
                // ntc--;
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }
    }else if(feof(fp2)){
        while(feof(fp1)!=1){
            getline(&temps,n,fp1);
            fprintf(fp3,"%s",temps);
            setnum =(double)rand()/(double)RAND_MAX;
            if(setnum<0.6){ //add line to training set
                fprintf(trainp,"%s",temps);
                // ptc--;
            }else if(0.6<=setnum && setnum<0.8){ //add line to validation set
                fprintf(validp,"%s",temps);
            }else{ //add line to test set
                fprintf(testp,"%s",temps);
            }
            free(temps);
            temps = malloc(300*sizeof(char));
            randomizer = (rand()%2);
        }
    }
    
    if(fclose(fp1)!= 0){
        fprintf(stderr, "Couldn't close poscliques.csv file.\n");
    }
    if(fclose(fp2)!= 0){
        fprintf(stderr, "Couldn't close negcliques.csv file.\n");
    }
    if(fclose(fp3)!= 0){
        fprintf(stderr, "Couldn't close randresults.csv file.\n");
    }
    
    if(fclose(trainp)!= 0){
        fprintf(stderr, "Couldn't close TrainingSet.csv file.\n");
    }
    if(fclose(validp)!= 0){
        fprintf(stderr, "Couldn't close ValidationSet.csv file.\n");
    }
    if(fclose(testp)!= 0){
        fprintf(stderr, "Couldn't close TestSet.csv file.\n");
    }
    
    
}