/**
 * @file    frame.h
 * @brief   Generate each frame.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#ifndef FRAME_H
#define FRAME_H

#include <Imlib2.h>

int get_frame_width(void);
int get_frame_height(void);
unsigned char *grab_frame(void);
int init_x_and_imlib(char *dispstr, int screen_num);

#endif /* FRAME_H */
