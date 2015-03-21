/**
 * @file    encode.c
 * @brief   Encode raw frames to video.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

#include <libswscale/swscale.h>

#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>

#include "frame.h"

extern int STOP_ENCODE;

#define RNDTO2(X) ( ( (X) & 0xFFFFFFFE )
#define RNDTO32(X) ( ( (X) % 32 ) ? ( ( (X) + 32 ) & 0xFFFFFFE0 ) : (X) )

// http://stackoverflow.com/questions/24057248/ffmpeg-undefined-references-to-av-frame-alloc
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc avcodec_alloc_frame
#define av_frame_free  avcodec_free_frame
#endif

void encode_loop(const char *filename, long long int frames, unsigned int delay,
                 int framerate)
{
    AVCodec *codec;
    AVCodecContext *c= NULL;
    int ret, got_output;
    unsigned int i = 0;
    FILE *f;
    AVFrame *frame;
    AVPacket pkt;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }
    /* put sample parameters */
  //c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width  = get_frame_width();
    c->height = get_frame_height();
    /* frames per second */
    c->time_base = (AVRational){1,framerate};
    c->gop_size = 10; /* emit one intra frame every ten frames */
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    av_opt_set(c->priv_data, "preset", "slow", 0);
    av_opt_set_int(c, "crf", 20, AV_OPT_SEARCH_CHILDREN);

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;
    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
                         c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        exit(1);
    }

    /* will we be running forever? */
    int inf = 0;
    if (frames == 0)
        inf=1;

    while (1) {
        if (STOP_ENCODE)
            break;

        if (!inf && --frames < 0)
            break;

        usleep(delay);

        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;
        fflush(stdout);

        struct SwsContext *ctx =
            sws_getContext(c->width, c->height, AV_PIX_FMT_RGB24,
                           c->width, c->height, AV_PIX_FMT_YUV420P,
                           0, 0, 0, 0);

        unsigned char *rgb_buf = grab_frame();
        const uint8_t *data_in[1] = { rgb_buf };
        int inline_size[1]  = { 3*c->width };
        sws_scale(ctx, data_in, inline_size, 0, c->height,
                  frame->data, frame->linesize);

        frame->pts = i;
        /* encode the image */
        ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            exit(1);
        }
        if (got_output) {
            printf("Wrote frame %d (size=%d)\n", i++, pkt.size);
            fwrite(pkt.data, 1, pkt.size, f);
            av_free_packet(&pkt);
        }

        free(rgb_buf);
    }
    /* get the delayed frames */
    for (got_output = 1; got_output; i++) {
        fflush(stdout);
        ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
        if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            exit(1);
        }
        if (got_output) {
            printf("Write frame %3d (size=%5d)\n", i, pkt.size);
            fwrite(pkt.data, 1, pkt.size, f);
            av_free_packet(&pkt);
        }
    }
    /* add sequence end code to have a real mpeg file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);
    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
    printf("\n");
}
