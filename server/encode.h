#ifndef ENCODE_H
#define ENCODE_H
#include <libavcodec/avcodec.h>
#include "assert.h"

typedef struct
{
    AVCodecContext *enc_ctx;
    AVFrame *frame;
    AVPacket *pkt;
}encoder_t;

encoder_t * init_encoder(int w,int h,int gop);
int encoder(encoder_t *p_encoder,FILE * outline);
#endif // ENCODE_H

