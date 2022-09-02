#include "command_handler.h"

//global variables
extern gas_api* p_gaz;
extern p_handler handler;
extern CFG cfg;

void init_application(){
    signal(SIGINT, handle_sigint);//CTRL + C
    p_gaz = load_library();
    handler = p_gaz->init_dll();
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
    signal(SIGINT,handle_sigint);
    p_gaz->stop_record(handler);
}
