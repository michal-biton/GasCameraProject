#include "command_line.h"

extern gas_api* p_gaz;
extern CFG* cfg;
extern p_handler handler;

int GAS_API_START_RECORD_WRAPER()
{
    p_gaz->start_record(handler,*(cfg->widget->record));
    return SUCCESS;
}

int GAS_API_STOP_RECORD_WRAPER()
{
    p_gaz->stop_record(handler);
    return SUCCESS;
}

int GAS_API_DO_SNAPSHOT_WRAPER()
{
    p_gaz->do_snapshot(handler,*(cfg->widget->snapshot));
    return SUCCESS;
}

int GAS_API_GET_STATUS_WRAPER()
{
    return p_gaz->get_status(handler);
}

int GAS_API_GET_RECORD_PARAMS_WRAPER()
{
    char data[MAXLINE] = {0};
    read_file(cfg);
    printf("%s\n",cfg->widget->record->file_name);
    memcpy(data,(void*)cfg->widget->record,sizeof(record_t));
    create_response(REPLY_MESSAGE,GET_RECORD_PARAMS,sizeof(record_t),data);
    return SUCCESS;
}

int GAS_API_SET_RECORD_PARAMS_WRAPER(char data[MAXLINE])
{
    record_t *changed_params = (record_t*)malloc(sizeof(record_t));
    assert(changed_params);
    memcpy(changed_params,data,sizeof(record_t));
    if(changed_params->file_name)
        strcpy(cfg->widget->record->file_name,changed_params->file_name);
    if(changed_params->codec)
        cfg->widget->record->codec=changed_params->codec;
    if(changed_params->height)
        cfg->widget->record->height=changed_params->height;
    if(changed_params->width)
        cfg->widget->record->width=changed_params->width;
    if(changed_params->fps)
        cfg->widget->record->fps=changed_params->fps;
    //if(changed_params->gop)
        //record->gop=changed_params->gop;
    //if(changed_params->gpio)
        //strcpy(record->gpio,changed_params->gpio);
    free(changed_params);
    write_file(cfg);
    acknowledge(ACK,0,0);
    return SUCCESS;
}

int GAS_API_GET_SNAPSHOT_PARAMS_WRAPER()
{
    char data[MAXLINE]={0};
    read_file(cfg);
    memcpy(data,cfg->widget->snapshot,sizeof(snapshot_t));
    create_response(REPLY_MESSAGE,GET_SNAPSHOT_PARAMS,sizeof(snapshot_t),data);
    return SUCCESS;
}

int GAS_API_SET_SNAPSHOT_PARAMS_WRAPER(char data[MAXLINE])
{
    snapshot_t* changed_params=(snapshot_t*)malloc(sizeof(snapshot_t));
    assert(changed_params);
    memcpy(changed_params,data,sizeof(snapshot_t));
    if(changed_params->file_name)
        //strcpy(cfg->widget->snapshot->file_name,changed_params->file_name);
    if(changed_params->height)
        cfg->widget->snapshot->height=changed_params->height;
    if(changed_params->width)
        cfg->widget->snapshot->width=changed_params->width;
    if(changed_params->type)
        cfg->widget->snapshot->type=changed_params->type;
    //if(changed_params->gpio)
    //strcpy(cfg->widget->snapshot->gpio,changed_params->gpio);
    free(changed_params);
    write_file(cfg);
    acknowledge(ACK,0,0);
    return SUCCESS;
}
