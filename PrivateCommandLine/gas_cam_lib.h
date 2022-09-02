#ifndef GAS_LIB_H
#define GAS_LIB_H
#include <pthread.h>
#include <stdio.h>
#include "queue.h"
#include <stdint.h>
#define LENGTH 320
#define WIDTH 240
#define COLOR 3
#define DEGREES 80
#define FRAMESPERSECOND 100
#define MACROSECEND 1000000
#define WAIT 4000
#define SLEEP_THREAD_TRACKING 50000
#define SNAPSHOTFILEPATHBMP "/mnt/hgfs/GasCameraProject/Images/%s.bmp"
#define SNAPSHOTFILEPATHJPG "/mnt/hgfs/GasCameraProject/Images/%s.jpg"
#define VIDIOPATH "/mnt/hgfs/GasCameraProject/Images/%s.ts"
#define MY_DEBUG
#ifndef MY_DEBUG
#define PRINTF_DBG printf
#else
#define PRINTF_DBG(m...)
#endif

enum status{
    CAPTURE_ACTIVE      = 1,
    RECORD_ACTIVE       = 2,
    STOP_RECORD_ACTIVE  = 4,
    SNAPSHOT_ACTIVE     = 8
};
typedef struct working_amount_for_thread{
    int counter_capture;
    double capture_really_work;
    double capture_wait_to_push;
    double waiting_to_keep_up;
    int counter_rgb_converet;
    double rgb_converet_really_work;
    double rgb_converet_wait_to_push;
    double rgb_converet_wait_queue_fill;
    int counter_yuv_convert;
    double yuv_convert_really_work;
    double yuv_convert_wait_to_push;
    double yuv_convert_wait_queue_fill;
    int counter_decoder;
    double decoder_really_work;
    double decoder_wait_queue_fill;
} working_amount,*p_working_amount;
enum state_record{
    WORKER_STATE,
    STOPED_STATE,
    FINISHED_STATE
};
typedef struct streaming_t{
    int ip;
    int port;
}streaming_t;

typedef struct gpio_t{
    int num;
    int dir;
    int value;
    int is_blink;
    int opertion;
}gpio_t;

typedef struct record_t{
    char* file_name;
    int codec;
    int width;
    int height;
    int fps;
}record_t;

typedef struct snapshot_t{
    char* file_name;
    int width;
    int height;
    int type;
}snapshot_t;

typedef struct {
    int width;
    int height;
    char *data;
    size_t size;
} ppm_image;

typedef struct stage
{
    pthread_t thread_id;
    int is_active;
    Queue* src_queue;
    Queue* dest_queue;
}stage_t,*p_stage;

typedef struct handler
{
    char RGB_static_mat[COLOR][DEGREES];
    p_stage stg_capture;
    p_stage stg_rgb_convertor;
    p_stage stg_yuv_convertor;
    p_stage stg_decoder;
    int status_lib;
    enum state_record record_status;
    working_amount* counter_thread;
}handler_t,*p_handler;

typedef struct yuv{
    char y[WIDTH*LENGTH];
    char u[WIDTH*LENGTH/4];
    char v[WIDTH*LENGTH/4];
}YUV;

typedef struct {
    p_handler (*init_dll)();
    int (*start_record)(p_handler,record_t);
    int (*stop_record)(p_handler);
    int (*start_streaming)(p_handler,streaming_t);
    int (*stop_streaming)(p_handler);
    int (*do_snapshot)(p_handler,snapshot_t);
    int (*get_status)(p_handler);
    int (*get_video_statistic)(p_handler);
    int (*get_dll_version)(p_handler);
    int (*msleep)(long);
}gas_api;
int msleep(long);
void running_thread();
p_handler INIT_DLL();
void* capture(void*);
void* rgb_convertor(void*);
void* convert_yuv(void*);
void* decoder(void*);
void* stg_write(void*);
char* snapshot_capture(handler_t*,snapshot_t);
void ppm_save(char*) ;
void jpeg_save(uint8_t *pRGBBuffer);
char* random_degrees();
uint64_t GetMHClock(void);
void* thread_tracking(void* count);
int GAS_API_DO_SNAPSHOT(p_handler,snapshot_t);
int GAS_API_GET_DLL_VERSION(p_handler);
int GAS_API_START_RECORD(p_handler,record_t);
int GAS_API_STOP_RECORD(p_handler);
int GAS_API_START_STREAMING(p_handler,streaming_t);
int GAS_API_STOP_STREAMING(p_handler);
int GAS_API_GET_VIDEO_STATISTIC(p_handler);
int GAS_API_GET_STATUS (p_handler);
void initRGB_static_mat(p_handler );
char* init_mat_by_RGB_Static_mat(p_handler ,char*);
#endif // GAS_LIB_H
