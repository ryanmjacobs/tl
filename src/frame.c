/**
 * @file    frame.c
 * @brief   Generate each frame.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "frame.h"

static Screen  *scr;
static Display *dpl;

int init_x(void) {
    dpl = XOpenDisplay(NULL);
    assert(dpl);

    scr = XDefaultScreenOfDisplay(dpl);
    assert(scr);

    return 0;
}

unsigned char *grab_frame(void) {
    int width  = XWidthOfScreen(scr);
    int height = XHeightOfScreen(scr);

    XImage *img = XGetImage(dpl, RootWindow(dpl, DefaultScreen(dpl)), 0, 0,
                            width, height, AllPlanes, ZPixmap);

    unsigned char *rgb = malloc(3*width*height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned long pixel = XGetPixel(img, x, y);
            unsigned char blue  = (pixel & img->blue_mask)  >> 0;
            unsigned char green = (pixel & img->green_mask) >> 8;
            unsigned char red   = (pixel & img->red_mask)   >> 16;

            rgb[3*(x + width*y) + 0] = red;
            rgb[3*(x + width*y) + 1] = green;
            rgb[3*(x + width*y) + 2] = blue;
        }
    }

    XDestroyImage(img);
    return rgb;
}

int get_frame_width(void) {
    return XWidthOfScreen(scr);
}

int get_frame_height(void) {
    return XHeightOfScreen(scr);
}
