/**
 * @file    args.h
 * @brief   Parse and process command line arguments.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 19, 2015
 * @bug     No known bugs.
 */

#ifndef ARGS_H
#define ARGS_H

struct args_t {
    int frames;           /* number of frames to record. 0 means infinite */
    int delay;            /* delay between each screenshot (in seconds) */
    int framerate;        /* encoded playback framerate */
    char *fname;          /* name of output file */
    char *x_display_name; /* x display name, e.g. :0 */
};

struct args_t parse_args(int argc, char **argv);

#endif /* ARGS_H */
