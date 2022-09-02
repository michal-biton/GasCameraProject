#include <stdio.h>
#include <stdlib.h>
#include "gas_cam_lib.h"
#include "rpc_protocol.h"
#include "config.h"

gas_api* p_gaz;
p_handler handler;
CFG* cfg;

int main()
{
    cfg = (CFG*)calloc(1, sizeof(CFG));
    record_t record;
    snapshot_t snapshot;
    gpio_t gpio;
    streaming_t streaming;
    WIDGET widget;
    widget.record = &record;
    widget.snapshot = &snapshot;
    widget.gpio = &gpio;
    widget.streaming = &streaming;
    cfg->widget = &widget;
    read_file(cfg);
    init_application();
    init_socket();
    communicate_with_client();
    printf("finish server!!!!!!!!!\n");
}
