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

    init_X(args.x_display_name);
    avcodec_register_all();
    encode_loop(args.fname, args.frames, args.delay, args.framerate);
    free_X();

    /**
     * This is just a quick hack to get an MP4 container by using
     * ffmpeg externally. If anyone knows how we can get an MP4
     * container without doing that, *please* submit a pull request
     * or contact me!
     */
    if (!system("which ffmpeg &>/dev/null")) {
        char cmd[256];

        if (!strcmp(args.fname, "timelapse.h264"))
            sprintf(cmd, "ffmpeg -y -i timelapse.h264 timelapse.mp4");
        else
            sprintf(cmd, "ffmpeg -y -i '%s' %s.mp4", args.fname, args.fname);

        system(cmd);
        unlink(args.fname);
        puts("\n\nEncode complete!");
    } else {
        puts("\n\nWe were not able to dump to an MP4 container :(");
        puts("You can still play it, but some video players will choke on it.\n");
        puts("Next time install ffmpeg first: e.g. `sudo apt-get install ffmpeg`");
        puts("(It's optional, but will shrink the file size and play more smoothly.");
    }

    return 0;
}
