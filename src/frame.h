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

DATA32 *grab_frame(void);
int init_x_and_imlib(char *dispstr, int screen_num);

#endif /* FRAME_H */
