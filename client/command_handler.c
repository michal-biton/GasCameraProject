#include "command_handler.h"
#include "command_line.h"

void match_function(int function_name)
{
    switch (function_name)
    {
    case START_RECORD:
        GAS_API_START_RECORD_CLI();
    case STOP_RECORD:
        GAS_API_STOP_RECORD_CLI();
    case DO_SNAPSHOT:
        GAS_API_DO_SNAPSHOT_CLI();
        //case GET_RECORD_PARAMETERS:
        //    GET_RECORD_PARAMETERS_CLI();
    }
}
int decoder(char *name_func)
{
    if(!strcmp(name_func,"strtrcrd"))
        return START_RECORD;
    if(!strcmp(name_func,"stprcrd"))
        return STOP_RECORD;
    if(!strcmp(name_func,"dsnpsht"))
        return DO_SNAPSHOT;
    if(!strcmp(name_func,"gtrcrdprms"))
        return GET_RECORD_PARAMETERS;
    return SUCCESS;
}
void input_and_activate()
{
    char path[MAXPARAMETERS];
    while (1)
    {
        fgets(path,MAXPARAMETERS,stdin);
        char *temp=(char*)calloc(strchr(path," ")-path,sizeof (char));
        assert(temp);
        temp=strchr(path," ");
        if(temp && decoder(temp) == 4)
            continue;
        //SET_RECORD_PARAMPS(split(path));
        match_function(decoder(path));
    }
}
