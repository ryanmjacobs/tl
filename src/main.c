/**
 * @file    main.c
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include <Imlib2.h>
#include <libavcodec/avcodec.h>

#include "args.h"
#include "frame.h"
#include "encode.h"

int STOP_ENCODE = 0;
struct args_t args;

static void sigint_handler(int signal) {
    if (!STOP_ENCODE)
        STOP_ENCODE = 1;
    else
        unlink(args.fname);
}

int main(int argc, char **argv) {
    args = parse_args(argc, argv);

    signal(SIGINT, sigint_handler);

    init_x_and_imlib(":0", 0);

    avcodec_register_all();
    encode_loop(args.fname, args.frames, args.delay, args.framerate);

    /* quick hack to get an mp4 container */
    char cmd[256];
    sprintf(cmd, "ffmpeg -y -i '%s' %s.mp4", args.fname, args.fname);
    system(cmd);
    unlink(args.fname);

    return 0;
}
