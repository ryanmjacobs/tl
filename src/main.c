/**
 * @file    main.c
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include <libavcodec/avcodec.h>

#include "args.h"
#include "frame.h"
#include "encode.h"

int CAUGHT_SIGINT = 0;

static void sigint_handler(int signal) {
    CAUGHT_SIGINT = 1;
}

int main(int argc, char **argv) {
    struct args_t args = parse_args(argc, argv);

    signal(SIGINT, sigint_handler);

    init_x();
    avcodec_register_all();
    encode_loop(args.fname, args.frames, args.delay, args.framerate);
    free_x();

    /* quick hack to get an mp4 container (if we have ffmpeg) */
    if (!system("which ffmpeg &>/dev/null")) {
        char cmd[256];
        sprintf(cmd, "ffmpeg -y -i '%s' %s.mp4", args.fname, args.fname);
        system(cmd);
        unlink(args.fname);
        puts("\n\nEncoding complete! Play w/ `mpv timelapse.h264.mp4`");
    } else {
        puts("\n\nWasn't able to dump to an MP4 container :(");
        puts("But... you can still play it w/ `mpv timelapse.h264`\n");
        puts("Next time install ffmpeg first: `sudo apt-get install ffmpeg`");
        puts("(It's optional, but will shrink the file size and play more smoothly.");
    }

    return 0;
}
