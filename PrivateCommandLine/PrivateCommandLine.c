#include<stdio.h>
#include<stdlib.h>
#include"PrivateCommandLine.h"
#include<ctype.h>
#include<string.h>
#include"gaz_cam_lib.h"
#include"commandHandler.h"

extern gas_api *p_gaz;
int GAS_API_GET_DLL_VERSION_CLI()
{

    return 1;
}
int GAS_API_START_RECORD_CLI()
{
    return 1;
}
int GAS_API_STOP_RECORD_CLI()
{
    return 1;
}
int GAS_API_START_STREAMING_CLI()
{
    return 1;
}
int GAS_API_STOP_STREAMING_CLI()
{
    return 1;

}
int GAS_API_DO_SNAPSHOT_CLI()
{
    return 1;

}
int GAS_API_GET_VIDEO_STATISTIC_CLI()
{
    return 1;

}
int GAS_API_GET_STATUS_CLI ()
{
 return 1;
}

