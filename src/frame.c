/**
 * @file    frame.c
 * @brief   Generate each frame.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <stdio.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>
#include <stdint.h>

#include "frame.h"

Display *disp = NULL;
Visual *vis = NULL;
Screen *scr = NULL;
Colormap cm;
int depth;
Window root = 0;

static unsigned char *data32_to_rgb(DATA32 *data32, int num_pixels);

unsigned char *grab_frame(void) {
    DATA32 *data32;
    Imlib_Image im;
    unsigned char *rgb_buf;

    im = imlib_create_image(scr->width, scr->height);

    imlib_context_set_image(im);
    imlib_context_set_display(disp);
    imlib_context_set_visual(vis);
    imlib_context_set_drawable(root);
    imlib_context_set_colormap(cm);
    imlib_context_set_color_modifier(NULL);
    imlib_context_set_operation(IMLIB_OP_COPY);

    imlib_copy_drawable_to_image(0, 0, 0, scr->width, scr->height, 0, 0, 1);

    data32  = imlib_image_get_data_for_reading_only();
    rgb_buf = data32_to_rgb(data32, scr->width*scr->height);

    imlib_free_image();
    return rgb_buf;
}

int get_frame_width(void) {
    return scr->width;
}

int get_frame_height(void) {
    return scr->height;
}

static unsigned char *data32_to_rgb(DATA32 *data32, int num_pixels) {
    unsigned int i;
    unsigned char *rgb;

    rgb = (unsigned char *) malloc(3*num_pixels);
    assert(rgb);

    for (i = 0; i < num_pixels; i++) {
        unsigned char a, r, g, b;

        a = (data32[i] >> 24) & 0xFF;
        r = (data32[i] >> 16) & 0xFF;
        g = (data32[i] >>  8) & 0xFF;
        b = (data32[i] >>  0) & 0xFF;

        rgb[3*i+0] = r;
        rgb[3*i+1] = g;
        rgb[3*i+2] = b;
    }

    return rgb;
}

int init_x_and_imlib(char *dispstr, int screen_num) {
    disp = XOpenDisplay(dispstr);
    
    if (!disp) {
        fputs("error: could not open display", stderr);
        return 1;
    }
    
    if (screen_num)
       scr = ScreenOfDisplay(disp, screen_num);
    else
       scr = ScreenOfDisplay(disp, DefaultScreen(disp));
    
    vis   = DefaultVisual  (disp, XScreenNumberOfScreen(scr));
    depth = DefaultDepth   (disp, XScreenNumberOfScreen(scr));
    cm    = DefaultColormap(disp, XScreenNumberOfScreen(scr));
    root  = RootWindow     (disp, XScreenNumberOfScreen(scr));
    
    return 0;
}
