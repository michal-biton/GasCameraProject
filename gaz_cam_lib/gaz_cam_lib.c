#include "gaz_cam_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include<assert.h>
#include "queue.h"
#include <time.h>
#include <errno.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <libavcodec/avcodec.h>
#include "encode.h"

int msleep(long msec)
{
    /* msleep(): Sleep for the requested number of milliseconds. */
    struct timespec ts;
    int res;
    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
    return res;
}
int GAS_API_GET_DLL_VERSION(p_handler handler)
{
    return 1;
}
void* thread_tracking(void* count){
    working_amount * pcount=(working_amount*)count;
    while(1)
    {
        sleep(1);
        printf("********************************************************************\n");
        printf("----------capture---------\n");
        printf("thread capture works %d time\n",pcount->counter_capture);
        pcount->counter_capture=0;
        pcount->capture_really_work=MACROSECEND-pcount->capture_wait_to_push;
        printf("thread capture really work %f \n",pcount->capture_really_work/MACROSECEND);
        pcount->capture_really_work=0;
        printf("thread capture wait to push %f \n",pcount->capture_wait_to_push/MACROSECEND);
        pcount->capture_wait_to_push=0;
        printf("----------rgb_convert---------\n");
        printf("thread rgb_convert works %d time\n",pcount->counter_rgb_converet);
        pcount->counter_rgb_converet=0;
        pcount->rgb_converet_really_work=MACROSECEND-pcount->rgb_converet_wait_queue_fill-
                pcount->rgb_converet_wait_to_push;
        printf("thread rgb_convert really work %f \n",pcount->rgb_converet_really_work/MACROSECEND);
        pcount->rgb_converet_really_work=0;
        printf("thread rgb_convert wait queue fill %f \n",pcount->rgb_converet_wait_queue_fill/MACROSECEND);
        pcount->rgb_converet_wait_queue_fill=0;
        printf("thread rgb_convert wait to push %f \n",pcount->rgb_converet_wait_to_push/MACROSECEND);
        pcount->rgb_converet_wait_to_push=0;
        printf("----------yov_convert---------\n");
        printf("thread yov_convert works %d time\n",pcount->counter_yuv_convert);
        pcount->counter_yuv_convert=0;
        pcount->yuv_convert_really_work=MACROSECEND-pcount->yuv_convert_wait_queue_fill-
                pcount->yuv_convert_wait_to_push;
        printf("thread yov_convert really work %f \n",pcount->yuv_convert_really_work/MACROSECEND);
        pcount->yuv_convert_really_work=0;
        printf("thread yov_convert wait queue fill %f \n",pcount->yuv_convert_wait_queue_fill/MACROSECEND);
        pcount->yuv_convert_wait_queue_fill=0;
        printf("thread yov_convert wait to push %f \n",pcount->yuv_convert_wait_to_push/MACROSECEND);
        pcount->yuv_convert_wait_to_push=0;
        printf("----------decoder---------\n");
        printf("thread decoder works %d time\n",pcount->counter_decoder);
        pcount->counter_decoder=0;
        pcount->decoder_really_work=MACROSECEND-pcount->decoder_wait_queue_fill;
        printf("thread decoder really work %f \n",pcount->decoder_really_work/MACROSECEND);
        pcount->decoder_really_work=0;
        printf("thread decoder wait queue fill %f \n",pcount->decoder_wait_queue_fill/MACROSECEND);
        pcount->decoder_wait_queue_fill=0;
    }
}
int GAS_API_START_RECORD(p_handler handler,record_t record)
{
    p_handler hand=(p_handler)handler;
    pthread_t thread_main_tracking;
    handler->status_lib=handler->status_lib|RECORD_ACTIVE;
    handler->record_status=WORKER_STATE;
    pthread_create(&thread_main_tracking,NULL,thread_tracking,hand->counter_thread);
    // running_thread();
    pthread_create(&handler->stg_capture->thread_id,NULL,capture,handler);
    pthread_create(&handler->stg_rgb_convertor->thread_id,NULL,rgb_convertor,handler);
    pthread_create(&handler->stg_yuv_convertor->thread_id,NULL,convert_yuv,handler);
    pthread_create(&handler->stg_decoder->thread_id,NULL,decoder,handler);
    return 1;
}
void running_thread(){
    // pthread_t thread_main_gpio;
    // pthread_create(&thread_main_gpio,NULL,main_gpio,NULL);
}
int GAS_API_STOP_RECORD(p_handler handler)
{
    handler->status_lib=handler->status_lib|STOP_RECORD_ACTIVE;
    p_handler hand=(p_handler)handler;
    hand->record_status=STOPED_STATE;
    while(hand->record_status!=FINISHED_STATE);
    pthread_cancel(handler->stg_capture->thread_id);
    pthread_cancel(handler->stg_rgb_convertor->thread_id);
    pthread_cancel(handler->stg_yuv_convertor->thread_id);
    pthread_cancel(handler->stg_decoder->thread_id);
    int i=hand->RGB_static_mat[0][0];
    PRINTF_DBG("GAS_API_STOP_RECORD\n");
    handler->record_status=STOPED_STATE;
    return i;
}
int GAS_API_START_STREAMING(p_handler handler,streaming_t streaming)
{
    p_handler hand=(p_handler)handler;
    int i=hand->RGB_static_mat[0][0];
    // streaming_t* stream=()
    return i;
}
int GAS_API_STOP_STREAMING(p_handler handler)
{
    p_handler hand=(p_handler)handler;
    int i=hand->RGB_static_mat[0][0];
    return i;
}
int GAS_API_GET_VIDEO_STATISTIC(p_handler handler)
{
    p_handler hand=(p_handler)handler;
    int i=hand->RGB_static_mat[0][0];
    return i;
}
int GAS_API_GET_STATUS (p_handler handler)
{
    return handler->status_lib;
}
handler_t * INIT_DLL(){
    handler_t * handler=(handler_t *)calloc(1,sizeof (handler_t));
    assert(handler);
    handler->stg_capture=(p_stage)calloc(1,sizeof(stage_t));
    assert(handler->stg_capture);
    handler->stg_capture->src_queue=NULL;
    handler->stg_capture->dest_queue=createQueue(1000);
    handler->counter_thread=(working_amount*)calloc(1,sizeof(working_amount));
    assert(handler->counter_thread);
    handler->stg_rgb_convertor=(p_stage)calloc(1,sizeof(stage_t));
    assert(handler->stg_rgb_convertor);
    handler->stg_rgb_convertor->src_queue=handler->stg_capture->dest_queue;
    handler->stg_rgb_convertor->dest_queue=createQueue(1000);
    handler->stg_yuv_convertor=(p_stage)calloc(1,sizeof (stage_t));
    assert(handler->stg_yuv_convertor);
    handler->stg_yuv_convertor->src_queue=handler->stg_rgb_convertor->dest_queue;
    handler->stg_yuv_convertor->dest_queue=createQueue(1000);
    handler->stg_decoder=(p_stage)calloc(1,sizeof (stage_t));
    assert(handler->stg_decoder);
    handler->stg_decoder->src_queue=handler->stg_yuv_convertor->dest_queue;
    handler->stg_decoder->dest_queue=NULL;
    initRGB_static_mat(handler);
    return handler;
}
int GAS_API_DO_SNAPSHOT(p_handler handler,snapshot_t snapshot)
{
    handler->status_lib=handler->status_lib|SNAPSHOT_ACTIVE;
    char* mat=snapshot_capture(handler,snapshot);
    char* RGB_mat=init_mat_by_RGB_Static_mat(handler,mat);
    if(handler->stg_capture->is_active&&mat)
        free(mat);
    //if(setting config ppm)
    //ppm_save(RGB_mat);
    //if(setting config jpeg)
    jpeg_save((uint8_t*)RGB_mat);
    return 1;
}
void jpeg_save(uint8_t *RGB_mat)
{
    time_t now ;time(&now);
    struct tm now_t = *localtime(&now);
    char buff[100];
    strftime (buff, 100, "%d-%m-%Y %H:%M:%S", &now_t);
    char* Filename=(char*)calloc(strlen(SNAPSHOTFILEPATHJPG)+strlen(ctime(&now) ),1);
    sprintf(Filename,SNAPSHOTFILEPATHJPG ,buff);
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    int row_stride;
    JSAMPROW row_pointer[1];
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    FILE* snapshot_file = fopen(Filename, "wb");
    assert(snapshot_file);
    jpeg_stdio_dest(&cinfo, snapshot_file);
    cinfo.image_width = WIDTH;
    cinfo.image_height = LENGTH;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality (&cinfo, 80, 1);
    jpeg_start_compress(&cinfo, TRUE);
    row_stride = cinfo.image_width * 3;
    while (cinfo.next_scanline <cinfo.image_height) {
        row_pointer[0] = &(RGB_mat[cinfo.next_scanline * row_stride]);
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(snapshot_file);
    if(Filename)
        free(Filename);
    if(RGB_mat)
        free(RGB_mat);
}
void ppm_save(char* RGB_mat)
{
    time_t now ;time(&now);
    struct tm now_t = *localtime(&now);
    char buff[100];
    strftime (buff, 100, "%d-%m-%Y %H:%M:%S", &now_t);
    char* Filename=(char*)calloc(strlen(SNAPSHOTFILEPATHBMP)+strlen(ctime(&now) ),1);
    sprintf(Filename,SNAPSHOTFILEPATHBMP ,buff);
    ppm_image ppmimg={ppmimg.height=LENGTH,ppmimg.width=WIDTH,ppmimg.data=RGB_mat,ppmimg.size=LENGTH*WIDTH*3};
    FILE* snapshot_file=fopen(Filename,"wb+");
    assert(snapshot_file);
    size_t n = 0;
    n += fprintf(snapshot_file, "P6\n# THIS IS A COMMENT\n%d %d\n%d\n",
                 ppmimg.width, ppmimg.height, 0xFF);
    n += fwrite(ppmimg.data,1, ppmimg.width * ppmimg.height * 3, snapshot_file);
    fclose(snapshot_file);
    if(Filename)
        free(Filename);
    if(RGB_mat)
        free(RGB_mat);
}
char* snapshot_capture(handler_t* handler,snapshot_t snapshot){
    char* mat;
    if(handler->stg_capture->is_active){
        mat=top(handler->stg_capture->dest_queue)->data;
        return mat;
    }
    capture(handler);
    mat=(char*)(dequeue(handler->stg_capture->dest_queue)->data);
    return mat;
}
char* random_degrees(){
    char* mat = (char *)calloc(WIDTH*LENGTH,sizeof(char));
    assert(mat);
    int i=0,j=0;
    for (i=0;i<LENGTH ;i++ ) {
        for (j=0;j<WIDTH ;j++ ) {
            *(mat+(i*WIDTH)+j)=rand()%79;
        }
    }
    return mat;
}
void initRGB_static_mat(p_handler handler)
{
    int i=0;
    char x=0,y=0,z=(char)255;
    for ( i=0;i<20 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        y+=10;
    }
    for ( i=20;i<40 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        x+=12;y+=2;z-=8;
    }
    x=(char)255;y=(char)255;z=95;
    for ( i=40;i<60 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        y-=3;z-=5;
    }
    x=(char)255;y=(char)200;z=0;
    for ( i=60;i<80 ; i++) {
        handler->RGB_static_mat[0][i]=x;
        handler->RGB_static_mat[1][i]=y;
        handler->RGB_static_mat[2][i]=z;
        y-=10;
    }
}
void* capture(void* handler){
    p_handler hand=(p_handler)handler;
    clock_t start_time,end_time;
    hand->status_lib=hand->status_lib|CAPTURE_ACTIVE;
    char *mat;Node* node;
    hand->stg_capture->is_active=1;
    do{
        start_time = clock();
        hand->counter_thread->counter_capture++;
        mat= random_degrees();
        createNode(mat,&node);
        while (enqueue(hand->stg_capture->dest_queue,node))
        {
            hand->counter_thread->capture_wait_to_push+=WAIT;
            usleep(WAIT);
        }
        end_time = clock();
        hand->counter_thread->waiting_to_keep_up+=(double)((MACROSECEND/FRAMESPERSECOND)-((end_time - start_time)/1000));
        usleep((double)((MACROSECEND/FRAMESPERSECOND)-((end_time - start_time)/1000)));
    }while(hand->status_lib& RECORD_ACTIVE);
    hand->stg_capture->is_active=0;
    return NULL;
}
void* rgb_convertor(void* handler){
    handler_t hand=*(p_handler)handler;
    hand.stg_rgb_convertor->is_active=1;
    Node* enqueued_node;Node* dequeued_node;
    char* sensors_mat = 0;char* rgb_mat=0;
    while(hand.status_lib& RECORD_ACTIVE||!isEmpty(hand.stg_rgb_convertor->src_queue))
    {
        PRINTF_DBG("rgb_convertor");
        hand.counter_thread->counter_rgb_converet++;
        while(isEmpty(hand.stg_rgb_convertor->src_queue))
        {
            hand.counter_thread->rgb_converet_wait_queue_fill+=WAIT;
            usleep(WAIT);
        }
        dequeued_node = (Node*)dequeue(hand.stg_rgb_convertor->src_queue);
        sensors_mat = (char*)dequeued_node->data;
        rgb_mat= init_mat_by_RGB_Static_mat(&hand,sensors_mat);
        createNode(rgb_mat,&enqueued_node);
        while(enqueue(hand.stg_rgb_convertor->dest_queue,enqueued_node))
        {
            hand.counter_thread->rgb_converet_wait_to_push+=WAIT;
            usleep(WAIT);
        }
        if(dequeued_node){
            if(sensors_mat)
                free(sensors_mat);
            free(dequeued_node);
        }
    }
    hand.stg_rgb_convertor->is_active=0;
    return NULL;
}
char* init_mat_by_RGB_Static_mat(p_handler handler,char* base_mat)
{
    char*  mat=(char*)calloc(WIDTH*LENGTH*3,sizeof (char));
    assert(mat);
    int mat_offset=0,tmp1=0,i=0,j=0;
    for( i=0;i<LENGTH;i++){
        for( j=0;j<WIDTH;j++){
            mat_offset=(i*WIDTH*3)+j*3;
            tmp1=(int) base_mat [i*WIDTH + j];
            mat [mat_offset]=(char)(handler->RGB_static_mat[0][tmp1]);
            mat[mat_offset+1]=(char)(handler->RGB_static_mat[1][tmp1]);
            mat [mat_offset+2]=(char)(handler->RGB_static_mat[2][tmp1]);
        }
    }
    return mat;
}
void* convert_yuv(void* handler)
{
    handler_t hand=*(p_handler)handler;
    hand.stg_yuv_convertor->is_active=1;
    YUV* yuv=0;char* rgb_mat=0;Node* node;Node* dequeued_node;
    while(hand.status_lib&RECORD_ACTIVE||!isEmpty((&hand)->stg_rgb_convertor->src_queue))
    {
        hand.counter_thread->counter_yuv_convert++;
        PRINTF_DBG("convert_yuv");
        while(isEmpty(hand.stg_yuv_convertor->src_queue))
        {
            hand.counter_thread->yuv_convert_wait_queue_fill+=WAIT;
            usleep(WAIT);
        }
        dequeued_node = (Node*)dequeue(hand.stg_yuv_convertor->src_queue);
        rgb_mat=(char*)dequeued_node->data;
        yuv=(YUV*)calloc(1,sizeof(YUV));
        int offset;
        assert(yuv);
        for(int i=0;i<LENGTH;i++)
        {
            for(int j=0;j<WIDTH;j++)
            {
                offset=i*WIDTH*3+j*3;
                unsigned char R=rgb_mat[offset];
                unsigned char G=rgb_mat[offset+1];
                unsigned char B=rgb_mat[offset+2];
                yuv->y[WIDTH*i+j]= (0.257 * R) + (0.504 * G) + (0.098 * B) + 16;
                if(i%2==0&&j%2==0)
                {
                    yuv->u[(WIDTH*i+j)/4]= (0.439 * R) - (0.368 * G) - (0.071 * B) + 128;
                    yuv->v[(WIDTH*i+j)/4]= -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128;
                }
            }
        }
        createNode(yuv,&node);
        while (enqueue(hand.stg_yuv_convertor->dest_queue,node))
        {
            hand.counter_thread->yuv_convert_wait_to_push+=WAIT;
            usleep(WAIT);
        }
        if(dequeued_node){
            if(rgb_mat)
                free(rgb_mat);
            free(dequeued_node);
        }
    }
    hand.stg_yuv_convertor->is_active=0;
    return NULL;
}
void* decoder(void* handler)
{
    p_handler hand=(p_handler)handler;
    hand->stg_decoder->is_active=1;
    int ret=0;
    Node* node;
    AVFrame *frame=NULL;
    FILE *outline=NULL;
    encoder_t *p_encoder;
    uint64_t start_time=0;
    uint8_t footer_ts[] = { 0, 0, 1, 0xb7 };
    fflush(stdout);
    p_encoder=init_encoder(WIDTH,LENGTH,12);
    frame=p_encoder->frame;
    time_t now ;time(&now);
    struct tm now_t = *localtime(&now);
    char buff[100];Node* dequeued_node;
    strftime (buff, 100, "%d-%m-%Y %H:%M:%S", &now_t);
    char* Filename=(char*)calloc(strlen(VIDIOPATH)+strlen(ctime(&now) ),1);
    sprintf(Filename,VIDIOPATH ,buff);
    outline=fopen(Filename,"wb");
    assert(outline);
    YUV *my_yuv;int offset;int offset2;
    while(hand->status_lib&RECORD_ACTIVE||!isEmpty(hand->stg_decoder->src_queue))
    {
        hand->counter_thread->counter_decoder++;
        PRINTF_DBG("%s.%d\n\r" ,__FUNCTION__,__LINE__);
        while(isEmpty(hand->stg_decoder->src_queue))
        {
            hand->counter_thread->decoder_wait_queue_fill+=WAIT;
            usleep(WAIT);
        }
        dequeued_node=(Node*)dequeue(hand->stg_decoder->src_queue);
        my_yuv=(YUV*)dequeued_node->data;
        ret = av_frame_make_writable(frame);
        assert(ret >=0);
        for (int i=0;i<frame->height;i++)
            for (int j=0;j<frame->width;j++)
            {
                offset=i*frame->width+j/2;
                offset2=i*frame->linesize[0]+j;
                frame->data[0][offset2]=my_yuv->y[offset];
            }
        for(int i=0;i<p_encoder->enc_ctx->height/2;i++)
            for(int j=0;j<p_encoder->enc_ctx->width/2;j++)
            {
                offset=(i/2)*frame->width+(j/2);
                frame->data [1][i*frame->linesize[1]+j]=my_yuv->u[offset];
                frame->data [2][i*frame->linesize[2]+j]=my_yuv->v[offset];
            }
        if (start_time==0){
            frame->pts=0;
            start_time=GetMHClock();
        }
        else
            frame->pts = GetMHClock()-start_time;
        encoder(p_encoder,outline);
        if(dequeued_node){
            if(my_yuv)
                free(my_yuv);
            free(dequeued_node);
        }
    }
    fwrite(footer_ts, 1, sizeof(footer_ts), outline);
    fclose(outline);
    createNode(my_yuv,&node);
    if(dequeued_node)
    {
        if(my_yuv)
            free(my_yuv);
        free(dequeued_node);
    }
    if(Filename)
        free(Filename);
    hand->status_lib=FINISHED_STATE;
    return NULL;
}
uint64_t GetMHClock(void){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint64_t)tv.tv_sec * 1000000L + (uint64_t)tv.tv_usec;
}
gas_api gas_api_lib={
    .get_dll_version=GAS_API_GET_DLL_VERSION,
    .start_record=GAS_API_START_RECORD,
    .stop_record=GAS_API_STOP_RECORD,
    .start_streaming=GAS_API_START_STREAMING,
    .stop_streaming=GAS_API_STOP_STREAMING,
    .do_snapshot=GAS_API_DO_SNAPSHOT,
    .get_video_statistic=GAS_API_GET_VIDEO_STATISTIC,
    .get_status=GAS_API_GET_STATUS,
    .init_dll=INIT_DLL
};
