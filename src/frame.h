/**
 * @file    frame.h
 * @brief   Generate each frame.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#ifndef FRAME_H
#define FRAME_H

struct frame_t {
    int width;
    int height;
    char *buf;
};

struct frame_t grab_frame(void);

#endif /* FRAME_H */
