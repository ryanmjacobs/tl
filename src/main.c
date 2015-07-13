/**
 * @file    main.c
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <stdio.h>
#include <string.h>
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

static int mp4_wrapper(const char *ifname);

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
    int ret = mp4_wrapper(args.fname);
    if (ret != 0) {
        puts("We were not able to dump to an MP4 container :(");
        puts("You can still play it, but some video players will choke on it.\n");

        if (ret == -1) {
            puts("Next time install ffmpeg/avconv first: e.g. `sudo apt-get install ffmpeg`");
            puts("(It's optional, but will shrink the file size and play more smoothly.");
        }
    }

    return 0;
}

/**
 * Quick hack... see main() for more info.
 */
static int mp4_wrapper(const char *ifname) {
    int len;
    const char *encoder;

    /* use ffmpeg/avconv (whichever is available) */
    if (!system("which ffmpeg &>/dev/null")) {
        encoder = "ffmpeg";
    } else if (!system("which avconv &>/dev/null")) {
        encoder = "avconv";
    } else {
        return -1;
    }

    /* output filename = input filename + ".mp4" */
    len = strlen(ifname);
    char *ofname = strndup(ifname, len +1 +4);
    if (ofname == NULL) {
        perror("strndup");
        return -2;
    }
    strcat(ofname, ".mp4");

    /* create command string */
    const char *fmt = "%s -y -i %s -c:v copy -an -loglevel quiet %s";
    len = snprintf(NULL, 0, fmt, encoder, ifname, ofname);
    char *cmd = malloc(++len);
    if (cmd == NULL) {
        perror("malloc");
        return -2;
    }
    snprintf(cmd, len, fmt, encoder, ifname, ofname);

    /* run command and if successful delete input file */
    if (system(cmd) == 0) {
        unlink(ifname);
        rename(ofname, ifname);

        printf("# Run `ffmpeg -i %s -c:v libx264 small.mp4` shrink the "
               "filesize by about 75%%.\n", ifname);
    }

    free(cmd);
    free(ofname);

    return 0;
}
