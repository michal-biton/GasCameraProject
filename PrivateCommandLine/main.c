#include<string.h>
#include"PrivateCommandLine.h"
#include<stdio.h>
#include"HashTable.h"
#define MAXPARAMETERS 100
#define CAPACITY 500 // Size of the Hash Table
#include"commandHandler.h"
#include <signal.h>
gas_api* p_gaz;
p_handler handler;
int flash_active;

int main()
{
    signal(SIGINT, handle_sigint);
    flash_active=1;
    // while(flash_active){
    p_gaz=load_library();
    record_t record;
    snapshot_t snapshot;
    handler=p_gaz->init_dll();
    p_gaz->start_record(handler,record);
    p_gaz->do_snapshot(handler,snapshot);
    //}
    HashTable* ht = create_table(CAPACITY);
    init_hash_table(ht);
    char**splitArray=0;
    char path[MAXPARAMETERS];
    while (1) {
        fgets(path,MAXPARAMETERS,stdin);
        splitArray=split(path);
        decoderfunction(ht,splitArray);
    }
    free_table(ht);
    return 0;
}

