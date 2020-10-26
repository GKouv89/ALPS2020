// Data Types
// Dataset: A list of all json specs that will be parsed
// spec: Each spec contains id (site//id), info_list
// info_list: list of tuples "name, value" (values is a string, not a number)
// info_tuple: item of above list

typedef struct tuple{
    char* name;
    char* value;
} info_tuple;

// will have head
// double connected
// gets initialized to one node containing page title

typedef struct node{
    info_tuple content;
    struct node *previous;
    struct node *next;
} list_node;

typedef struct list{
    list_node *front;
} info_list;

typedef struct json{
    char* id;
    info_list information;
} spec;

 

