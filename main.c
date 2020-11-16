#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "datatypes.h"
#include "parse.h"
#include "hashmap.h"
#include "tuplist.h"
#include "set.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    
    hash_map* map = create_map();
    clique_list* all_cliques;
    create_clique_list(&all_cliques);

    parser(map);
    csvparser(map, all_cliques);
    
    print_all_cliques(0, all_cliques);
    destroy_map(&map);
    destroy_clique_list(&all_cliques);
}
