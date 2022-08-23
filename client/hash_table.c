#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAPACITY 500 // Size of the Hash Table

unsigned long hash_function(char* str)
{
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}
Ht_item* create_item(char* key,int(*func)(int,char**), char* help)
{
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = (char*) malloc (strlen(key) + 1);
    struct value val={val.func=func,val.help=(char*)malloc(strlen(help)+1)};
    item->value = val;
    strcpy(item->key, key);
    strcpy(item->value.help, help);
    return item;
}

HashTable* create_table(int size)
{
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    return table;
}
void free_item(Ht_item* item) {
    free(item->key);
    free(item->value.help);
    free(item);
}

void free_table(HashTable* table)
{
    for (int i=0; i<table->size; i++)
    {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }
    free(table->items);
    free(table);
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item)
{
}

void ht_insert(HashTable* table, char* key,int(*func)(int,char**), char* help)
{
    Ht_item* item = create_item(key,func,help);
    unsigned long index = hash_function(key);
    Ht_item* current_item = table->items[index];
    if (current_item == NULL)
    {
        if (table->count == table->size)
        {
            printf("Insert Error: Hash Table is full\n");
            free_item(item);
            return;
        }
        table->items[index] = item;
        table->count++;
    }
    else
    {
            if (strcmp(current_item->key, key) == 0)
            {
                table->items[index]->value.func=func;
                strcpy(table->items[index]->value.help,help);
                return;
            }
        else
            {
            handle_collision(table, index, item);
            return;
        }
    }
}

struct value ht_search(HashTable* table, char* key)
{
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->value;
    }
    struct value val;
    val.func=NULL;
    return val;
}

void print_search(HashTable* table, char* key)
{
    struct value val;
    if ((val = ht_search(table, key)).func == NULL)
    {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else
        printf("Key:%s, Value:%s\n", key, val);
}

void print_table(HashTable* table)
{
    printf("\nHash Table\n-------------------\n");
    for (int i=0; i<table->size; i++)
        if (table->items[i])
            printf("Index:%d, Key:%s, Value:%s\n", i, table->items[i]->key, table->items[i]->value.help);
    printf("-------------------\n\n");
}
