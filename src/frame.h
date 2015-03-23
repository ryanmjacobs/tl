/**
 * @file    frame.h
 * @brief   Generate each frame.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#ifndef FRAME_H
#define FRAME_H

int init_x(const char *display_name);
int free_x(void);
unsigned char *grab_frame(void);
int get_frame_width(void);
int get_frame_height(void);

#endif /* FRAME_H */
