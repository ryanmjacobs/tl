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

#include "frame.h"

Display *disp = NULL;
Visual *vis = NULL;
Screen *scr = NULL;
Colormap cm;
int depth;
Window root = 0;

DATA32 *grab_frame(void) {
    DATA32 *buf;
    Imlib_Image im;

    im = imlib_create_image(scr->width, scr->height);

    imlib_context_set_image(im);
    imlib_context_set_display(disp);
    imlib_context_set_visual(vis);
    imlib_context_set_drawable(root);
    imlib_context_set_colormap(cm);
    imlib_context_set_color_modifier(NULL);
    imlib_context_set_operation(IMLIB_OP_COPY);

    imlib_copy_drawable_to_image(0, 0, 0, scr->width, scr->height, 0, 0, 1);

    buf = imlib_image_get_data_for_reading_only();

    return buf;
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
