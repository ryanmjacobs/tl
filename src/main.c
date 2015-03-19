/**
 * @file    main.c
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <Imlib2.h>
#include "frame.h"

int main(int argc, char **argv) {
    init_x_and_imlib(":0", 0);

    grab_frame();
    imlib_free_image();

    return 0;
}
