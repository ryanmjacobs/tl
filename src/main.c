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

// remove_ext: removes the "extension" from a file spec.
//   mystr is the string to process.
//   dot is the extension separator.
//   sep is the path separator (0 means to ignore).
// Returns an allocated string identical to the original but
//   with the extension removed. It must be freed when you're
//   finished with it.
// If you pass in NULL or the new string can't be allocated,
//   it returns NULL.

char *remove_ext (char* mystr) {
    char *retstr, *lastdot, *lastsep;

    // Error checks and allocate string.

    if (mystr == NULL)
        return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;

    // Make a copy and find the relevant characters.

    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    lastsep = strrchr (retstr, '/');

    // If it has an extension separator.

    if (lastdot != NULL) {
        // and it's before the extenstion separator.

        if (lastsep != NULL) {
            if (lastsep < lastdot) {
                // then remove it.

                *lastdot = '\0';
            }
        } else {
            // Has extension separator with no path separator.

            *lastdot = '\0';
        }
    }

    // Return the modified string.

    return retstr;
}

static int mp4_wrapper(const char *input) {
    char cmd[512] = "";
    const char *dot, *ext;

    /* filename extension */
    dot = strrchr(input, '.');
    if (!dot || dot == input)
        ext = "";
    else
        ext = dot+1;

    if (!system("which ffmpeg &>/dev/null")) {
        strcat(cmd, "ffmpeg -y -i ");
        strcat(cmd, input);
    } else if (!system("which avconv &>/dev/null")) {
        strcat(cmd, "avconv -y -i ");
        strcat(cmd, input);
    } else if (!system("which HandBrakeCLI &>/dev/null")) {
        strcat(cmd, "HandBrakeCLI -i ");
        strcat(cmd, input);
        strcat(cmd, " -o");
    } else {
        return -1;
    }
    strcat(cmd, " ");

    /* determine output filename */
    if (!strcmp(ext, "h264")) {
        char *basename = remove_ext(input);
        strcat(cmd, basename);
        free(basename);
    } else {
        strcat(cmd, input);
    }
    strcat(cmd, ".mp4");

    /* run command */
    puts(cmd);
    if (system(cmd) == 0) {
        unlink(input);
    }

    return 0;
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
        mp4_wrapper(args.fname);
        puts("\n\nEncode complete!");
    } else {
        puts("\n\nWe were not able to dump to an MP4 container :(");
        puts("You can still play it, but some video players will choke on it.\n");
        puts("Next time install ffmpeg first: e.g. `sudo apt-get install ffmpeg`");
        puts("(It's optional, but will shrink the file size and play more smoothly.");
    }

    return 0;
}
