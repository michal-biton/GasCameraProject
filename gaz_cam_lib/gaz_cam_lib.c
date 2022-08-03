#include "gaz_cam_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include<assert.h>
#include "queue.h"


int GAS_API_GET_DLL_VERSION(p_handler handler)
{
    return 1;
}
int GAS_API_START_RECORD(p_handler handler,record_t record)
{
    handler->record_status=1;
    pthread_create(&handler->stg_capture->thread_id,NULL,capture,handler);
    pthread_create(&handler->stg_rgb_convertor->thread_id,NULL,rgb_convertor,handler);
    pthread_create(&handler->stg_yuv_convertor->thread_id,NULL,convert_yuv,handler);
    return 1;
}
int GAS_API_STOP_RECORD(p_handler handler)
{
    p_handler hand=(p_handler)handler;
    //flash_active=0;
    hand->record_status=0;
    while(hand->record_status!=2);
    pthread_cancel(handler->stg_capture->thread_id);
    pthread_cancel(handler->stg_rgb_convertor->thread_id);
    pthread_cancel(handler->stg_yuv_convertor->thread_id);
    pthread_cancel(handler->stg_decoder->thread_id);
    pthread_cancel(handler->stg_write->thread_id);
    int i=hand->RGB_static_mat[0][0];
    printf("GAS_API_STOP_RECORD\n");
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
    p_handler hand=(p_handler)handler;
    int i=hand->RGB_static_mat[0][0];
    return i;
}
handler_t * INIT_DLL(){
    handler_t * handler=(handler_t *)calloc(1,sizeof (handler_t));
    handler->stg_capture=(p_stage)calloc(1,sizeof(stage_t));
    handler->stg_capture->src_queue=NULL;
    handler->stg_capture->dest_queue=createQueue(10);
    handler->stg_rgb_convertor=(p_stage)calloc(1,sizeof(stage_t));
    handler->stg_rgb_convertor->src_queue=handler->stg_capture->dest_queue;
    handler->stg_rgb_convertor->dest_queue=createQueue(10);
    handler->stg_yuv_convertor=(p_stage)calloc(1,sizeof (stage_t));
    handler->stg_yuv_convertor->src_queue=handler->stg_rgb_convertor->dest_queue;
    handler->stg_yuv_convertor->dest_queue=createQueue(10);
    handler->stg_decoder=(p_stage)calloc(1,sizeof (stage_t));
    handler->stg_decoder->src_queue=handler->stg_yuv_convertor->dest_queue;
    handler->stg_decoder->dest_queue=createQueue(10);
    handler->stg_write=(p_stage)calloc(1,sizeof (stage_t));
    handler->stg_write->src_queue=handler->stg_decoder->dest_queue;
    handler->stg_write->dest_queue=NULL;
    initRGB_static_mat(handler);
    return handler;
}
int GAS_API_DO_SNAPSHOT(p_handler handler,snapshot_t snapshot)
{
    char* mat=snapshot_capture(handler,snapshot);
    char* RGB_mat=init_mat_by_RGB_Static_mat(handler,mat);
    ppm_image ppm={ppm.height=LENGTH,ppm.width=WIDTH,ppm.data=RGB_mat,ppm.size=LENGTH*WIDTH*3};
    FILE* snapshot_file=fopen(SNAPSHOTFILEPATH,"wb+");
    assert(snapshot_file);
    ppm_save(&ppm,snapshot_file);
    fclose(snapshot_file);
    return 1;
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
    int i=0,j=0;
    for (i=0;i<WIDTH ;i++ ) {
        for (j=0;j<LENGTH ;j++ ) {
            *(mat+(i*LENGTH)+j)=rand()%79;
        }
    }
    return mat;
}
size_t ppm_save(ppm_image *img, FILE *outfile) {
    size_t n = 0;
    n += fprintf(outfile, "P6\n# THIS IS A COMMENT\n%d %d\n%d\n",
                 img->width, img->height, 0xFF);
    n += fwrite(img->data,1, img->width * img->height * 3, outfile);
    return n;
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
    char *mat;
    hand->stg_capture->is_active=1;
    do{mat= random_degrees();
        printf("%s.%d mat %p \n",__FUNCTION__,__LINE__,mat);
        enqueue(hand->stg_capture->dest_queue,createNode(mat)); //no while until succsess, spend data if full
        //sleep(10);
    }while(hand->record_status);
    hand->stg_capture->is_active=0;
    return NULL;
}
void* rgb_convertor(void* handler){
    handler_t hand=*(p_handler)handler;
    hand.stg_rgb_convertor->is_active=1;
    char* sensors_mat = 0;char* rgb_mat=0;
    while(hand.record_status||!isEmpty((&hand)->stg_rgb_convertor->src_queue)){
        while(isEmpty(hand.stg_rgb_convertor->src_queue)==1)
            sleep(1);
        sensors_mat=(char*)(dequeue(hand.stg_capture->dest_queue)->data);
        rgb_mat= init_mat_by_RGB_Static_mat(&hand,sensors_mat);
        printf("%s.%d rgb_mat 1 %p \n",__FUNCTION__,__LINE__,rgb_mat);
        if(!enqueue(hand.stg_rgb_convertor->dest_queue,createNode(rgb_mat))){
            sleep(1);
        }
    }
    hand.stg_rgb_convertor->is_active=0;
    return NULL;
}
//char* mat=0;
char* init_mat_by_RGB_Static_mat(p_handler handler,char* base_mat)
{
   char*  mat=(char*)calloc(sizeof (char),WIDTH*LENGTH*3);
    int mat_offset=0,tmp1=0,i=0,j=0;
    for( i=0;i<WIDTH;i++){
        for( j=0;j<LENGTH;j++){
            mat_offset=(i*LENGTH*3)+j*3;
            tmp1=(int) base_mat [i*LENGTH + j];
            mat [mat_offset]=(char)(handler->RGB_static_mat[0][tmp1]);
            mat[mat_offset+1]=(char)(handler->RGB_static_mat[1][tmp1]);
            mat [mat_offset+2]=(char)(handler->RGB_static_mat[2][tmp1]);
        }
    }
    return mat;
}

void* convert_yuv(void* handler){
    handler_t hand=*(p_handler)handler;
    hand.stg_yuv_convertor->is_active=1;
    YUV* yuv=0;
    char* rgb_mat=0;
    printf("%s.%d rgb_mat 0 %p \n",__FUNCTION__,__LINE__,rgb_mat);
    while(hand.record_status||!isEmpty((&hand)->stg_rgb_convertor->src_queue))
    {
        while(isEmpty((&hand)->stg_yuv_convertor->src_queue)){
            //    printf("busy wait yuv\n");
            //  sleep(1);
        }
        Node* p_node =dequeue(hand.stg_yuv_convertor->src_queue);
        rgb_mat=(char*)(p_node->data);
        printf("%s.%d rgb_mat 2 %p \n",__FUNCTION__,__LINE__,rgb_mat);
        yuv=(YUV*)calloc(1,sizeof(YUV));
        assert(yuv);
        for(int i=0;i<WIDTH/2;i++)
        {
            for(int j=0;j<LENGTH/2;j++)
            {
                unsigned char R=rgb_mat[i*LENGTH*3+j*3];
                unsigned char G=rgb_mat[i*LENGTH*3+j*3+1];
                unsigned char B=rgb_mat[i*LENGTH*3+j*3+2];
                yuv->y[LENGTH*i+j]= (0.257 * R) + (0.504 * G) + (0.098 * B) + 16+'0';
                if(i%2==0&&j%2==0)
                {
                    yuv->u[(LENGTH*i+j)/4]= (0.439 * R) - (0.368 * G) - (0.071 * B) + 128+'0';
                    yuv->v[(LENGTH*i+j)/4]= -(0.148 * R) - (0.291 * G) + (0.439 * B) + 128+'0';
                }
            }
        }
        //  if(rgb_mat)
        //     free(rgb_mat);
        if(!enqueue((&hand)->stg_yuv_convertor->dest_queue,createNode(yuv))){
            free (yuv);
            yuv=0;
        }
    }
    hand.stg_yuv_convertor->is_active=0;
    hand.record_status=2;//terminetd
    return NULL;
}
void* decoder(void* handler){
    p_handler hand=(p_handler)handler;
    hand->stg_decoder->is_active=0;
    printf("decoder");
    return NULL;
}
void* stg_write(void* handler){
    p_handler hand=(p_handler)handler;
    hand->stg_write->is_active=0;
    printf("write");
    return NULL;
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
