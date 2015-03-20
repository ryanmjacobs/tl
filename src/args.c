/**
 * @file    args.c
 * @brief   Parse and process command line arguments.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 19, 2015
 * @bug     No known bugs.
 */

#include "args.h"

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

enum {
    OPT_VERSION
};

static const char *help_msg =
    "Usage: %s [-h]\n"
    "Record and render timelapses.\n\n"
    "  -h,   --help               Display this help and exit\n"
    "        --version            Display version information and exit\n\n";

static const struct option long_options[] = {
    { "help",        optional_argument, NULL, 'h'          },
    { "version",     no_argument,       NULL,  OPT_VERSION },
    { NULL, 0, NULL, 0 }
};

struct args_t parse_args(int argc, char **argv) {
    struct args_t args;

    if (argc < 2) {
        fprintf(stderr, help_msg, argv[0]);
        exit(1);
    }

    char c;
    while ((c = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
        switch (c) {
            case 'h':
                fprintf(stdout, help_msg, argv[0]);
                exit(0);
                break;
            default:
                break;
        }
    }

    return args;
}
