/**
 * @file    main.c
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <Imlib2.h>
#include <libavcodec/avcodec.h>

#include "args.h"
#include "frame.h"
#include "encode.h"

int main(int argc, char **argv) {
    struct args_t args = parse_args(argc, argv);

    init_x_and_imlib(":0", 0);

    avcodec_register_all();
    encode_video("out.h264", AV_CODEC_ID_H264);

    return 0;
}
