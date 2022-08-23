#ifndef HASH_TABLE_H
#define HASH_TABLE_H
unsigned long hash_function(char*);
typedef struct Ht_item Ht_item;
struct value{
    int (*func)(int ,char**);
    char*help;
};
struct Ht_item {
    char* key;
    struct value value;
};
typedef struct HashTable HashTable;

struct HashTable {
    Ht_item** items;
    int size;
    int count;
};
Ht_item* create_item(char*,int(*func)(int,char**), char* );
HashTable* create_table(int);
void free_item(Ht_item*);
void free_table(HashTable*);
void handle_collision(HashTable*, unsigned long, Ht_item*);
void ht_insert(HashTable*, char*,int(*func)(int,char**), char*);
struct value ht_search(HashTable* table, char* key);
void print_search(HashTable*, char*);
void print_table(HashTable*);

#endif // HASH_TABLE_H
