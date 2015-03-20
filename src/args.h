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
    unsigned int frame_rate;  /* encoded playback framerate */
    double delay;             /* delay between each screenshot (in seconds) */
};

struct args_t parse_args(int argc, char **argv);

#endif /* ARGS_H */
