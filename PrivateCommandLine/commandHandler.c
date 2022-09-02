#include<string.h>
#include"PrivateCommandLine.h"
#include<stdio.h>
#include<HashTable.h>
#define MAXPARAMETERS 100
#define CAPACITY 500 // Size of the Hash Table
#include <dlfcn.h>
#include<stdlib.h>
#include<assert.h>
#include"gaz_cam_lib.h"
#include"commandHandler.h"
extern gas_api* p_gaz;//load_library();
extern p_handler handler;

char** split(char path[MAXPARAMETERS]){
    int index=0;
    char space[3]=" ";
    char** splitArray=NULL,*token;
    token=strtok(path,space);
    while(token!=NULL)
    {
        splitArray=(char**)realloc(splitArray,sizeof (char*)*(++index));
        splitArray[index-1]=token;
        printf(" %s\n",splitArray[index-1]);
        token=strtok(NULL,space);
    }
    return splitArray;
}

void decoderfunction(HashTable* ht,char** splitArray){
    if(strcmp(splitArray[1],"--help"))
        if((sizeof(splitArray)/sizeof (char*))>2)
        {
            printf("error!!");
            return;
        }
        else
        {
            char*h=ht_search(ht,splitArray[0]).help;
            if(h)
                printf("%s",h);
            else
            {
                printf("error!!");
                return;
            }
        }
    int cnt = 0;
    while (splitArray[cnt] !=NULL)
        cnt++;
    int(*func)(int,char**)=ht_search(ht,splitArray[0]).func;
    func(cnt-1,++splitArray);
}

void init_hash_table(HashTable* ht)
{
    ht_insert(ht,"vrsn",GAS_API_GET_DLL_VERSION_CLI,"GAS_API_GET_DLL_VERSION_CLI");
    ht_insert(ht,"strtrcrd",GAS_API_START_RECORD_CLI,"GAS_API_START_RECORD_CLI");
    ht_insert(ht,"stprcrd",GAS_API_STOP_RECORD_CLI,"GAS_API_STOP_RECORD_CLI");
    ht_insert(ht,"strtstrmng",GAS_API_START_STREAMING_CLI,"GAS_API_GET_DLL_VERSION_CLI");
    ht_insert(ht,"stpstrmng",GAS_API_STOP_STREAMING_CLI,"GAS_API_START_STREAMING_CLI");
    ht_insert(ht,"dsnpsht",GAS_API_DO_SNAPSHOT_CLI,"GAS_API_STOP_STREAMING_CLI");
    ht_insert(ht,"gtvdsttstc",GAS_API_GET_DLL_VERSION_CLI,"GAS_API_DO_SNAPSHOT_CLI");
    ht_insert(ht,"vrsn",GAS_API_GET_VIDEO_STATISTIC_CLI,"GAS_API_GET_VIDEO_STATISTIC_CLI");
    ht_insert(ht,"gtstts",GAS_API_GET_STATUS_CLI,"GAS_API_GET_STATUS_CLI");
    print_table(ht);
}

gas_api* load_library()
{
    void* libHandle= dlopen("/mnt/hgfs/GasCameraProject/gaz_cam_lib/libgaz_cam_lib.so.1.0.0",RTLD_LAZY);
    assert (libHandle);
    gas_api *p_gaz;
    p_gaz = (gas_api*)dlsym(libHandle,"gas_api_lib");
    return p_gaz;
}
void handle_sigint(int sig)
{
    if(sig==SIGINT)
        p_gaz->stop_record(handler);
}



