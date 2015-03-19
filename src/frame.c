/**
 * @file    frame.c
 * @brief   Generate each frame.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <assert.h>

#include "frame.h"

int get_shift(int mask);

struct frame_t grab_frame(void) {
    XImage  *img;
    Display *dpl;
    Screen  *scr;
    struct frame_t frame;
    int x, y;

    dpl = XOpenDisplay(NULL);
    assert(dpl);

    scr = XDefaultScreenOfDisplay(dpl);
    assert(scr);

    frame.width  = XWidthOfScreen(scr);
    frame.height = XHeightOfScreen(scr);

    img = XGetImage(dpl, RootWindow(dpl, DefaultScreen(dpl)),
          0, 0, frame.width, frame.height, AllPlanes, ZPixmap);

    fprintf(stdout,"P6\n%d %d\n255\n",img->width, img->height);
    for (x = 0; x < frame.width; x++) {
        for (y = 0; y < frame.height; y++) {
            unsigned long pixel;

            pixel = XGetPixel(img, x, y);
        }
    }

    return frame;
}

#if 0
void init_x_and_imlib(char *dispstr, int screen_num)
   disp = XOpenDisplay(dispstr);
   if (!disp)
      gib_eprintf("Can't open X display. It *is* running, yeah?");
   if (screen_num)
      scr = ScreenOfDisplay(disp, screen_num);
   else
      scr = ScreenOfDisplay(disp, DefaultScreen(disp));

   vis = DefaultVisual(disp, XScreenNumberOfScreen(scr));
   depth = DefaultDepth(disp, XScreenNumberOfScreen(scr));
   cm = DefaultColormap(disp, XScreenNumberOfScreen(scr));
   root = RootWindow(disp, XScreenNumberOfScreen(scr));

   imlib_context_set_display(disp);
   imlib_context_set_visual(vis);
   imlib_context_set_colormap(cm);
   imlib_context_set_color_modifier(NULL);
   imlib_context_set_operation(IMLIB_OP_COPY);
}
#end if
