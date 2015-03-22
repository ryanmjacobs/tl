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

#include <stdint.h>
#include <sys/param.h>

#include "frame.h"

struct pixel_t {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

typedef uint16_t WORD;

static Screen  *scr;
static Display *dpl;
static struct pixel_t grab_pixel(XImage *img, int x, int y);

int init_x(const char *display_name) {
    dpl = XOpenDisplay(display_name);
    if (dpl == NULL) {
        fprintf(stderr, "error: could not open X display %s\n", display_name);
        exit(1);
    }

    scr = XDefaultScreenOfDisplay(dpl);
    assert(scr);

    return 0;
}

int free_x(void) {
    XCloseDisplay(dpl);

    return 0;
}

int get_frame_width(void) {
    return XWidthOfScreen(scr);
}

int get_frame_height(void) {
    return XHeightOfScreen(scr);
}

unsigned char *grab_frame(void) {
    int width  = XWidthOfScreen(scr);
    int height = XHeightOfScreen(scr);

    XImage *img = XGetImage(dpl, RootWindow(dpl, DefaultScreen(dpl)), 0, 0,
                            width, height, AllPlanes, ZPixmap);

    unsigned char *rgb = malloc(3*width*height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            struct pixel_t p = grab_pixel(img, x, y);

            rgb[3*(x + width*y) + 0] = p.red;
            rgb[3*(x + width*y) + 1] = p.green;
            rgb[3*(x + width*y) + 2] = p.blue;
        }
    }

    XDestroyImage(img);
    return rgb;
}

// http://www.opensource.apple.com/source/tcl/tcl-95/tk/tk/win/tkWinImage.c
static struct pixel_t grab_pixel(XImage *img, int x, int y) {
    struct pixel_t p = {0};
    unsigned char *srcPtr = (unsigned char *) &(img->data[(y * img->bytes_per_line)
        + ((x * img->bits_per_pixel) / NBBY)]);

    switch (img->bits_per_pixel) {
        case 32:
        case 24:
            p.red   = srcPtr[2];
            p.green = srcPtr[1];
            p.blue  = srcPtr[0];
            break;
        case 16:
            p.red   = ((((WORD*)srcPtr)[0]) >> 7) & 0xf8;
            p.green = ((((WORD*)srcPtr)[0]) >> 2) & 0xf8;
            p.blue  = ((((WORD*)srcPtr)[0]) << 3) & 0xf8;
            break;
      //case 8:
      //    pixel = srcPtr[0];
      //    break;
      //case 4:
      //    pixel = ((x%2) ? (*srcPtr) : ((*srcPtr) >> 4)) & 0x0f;
      //    break;
      //case 1:
      //    pixel = ((*srcPtr) & (0x80 >> (x%8))) ? 1 : 0;
      //    break;
        default:
            fprintf(stderr, "error: %d-bit pixel displays are not yet "
                    "supported. Go file a bug.\n", img->bits_per_pixel);
            exit(1);
            break;
    }

    return p;
}
