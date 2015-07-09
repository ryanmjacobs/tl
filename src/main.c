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

static int mp4_wrapper(const char *input);

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
    if (mp4_wrapper(args.fname) != 0) {
        puts("We were not able to dump to an MP4 container :(");
        puts("You can still play it, but some video players will choke on it.\n");
        puts("Next time install ffmpeg/avconv first: e.g. `sudo apt-get install ffmpeg`");
        puts("(It's optional, but will shrink the file size and play more smoothly.");
    }

    return 0;
}

/**
 * Quick hack... see main() for more info.
 */
static int mp4_wrapper(const char *input) {
    char cmd[512] = "";

    /* use ffmpeg/avconv (whichever is available) */
    if (!system("which ffmpeg &>/dev/null")) {
        strcat(cmd, "ffmpeg -y -i ");
    } else if (!system("which avconv &>/dev/null")) {
        strcat(cmd, "avconv -y -i ");
    } else {
        return -1;
    }
    strcat(cmd, input);
    strcat(cmd, " -c:v copy -an -loglevel quiet ");

    /* determine output filename */
    const char *last4 = input+strlen(input)-4;
    if (!strcmp(last4, "h264")) {
        strncat(cmd, input, strlen(input)-5);
    } else {
        strcat(cmd, input);
    }
    strcat(cmd, ".mp4");

    /* run command and if successful delete input file */
    puts(cmd);
    if (system(cmd) == 0) {
        unlink(input);

        /* remove double .mp4.mp4 extensions */
        if (!strcmp(last4, ".mp4")) {
            char *output = malloc(strlen(input) + 5);
            strcpy(output, input);
            strcat(output, ".mp4");
            rename(output, input);
        }
    }

    return 0;
}
