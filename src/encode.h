/**
 * @file    encode.h
 * @brief   Encode raw frames to video.
 * @author  Ryan Jacobs <ryan.mjacobs@gmail.com>
 * @date    March 18, 2015
 * @bug     No known bugs.
 */

#ifndef ENCODE_H
#define ENCODE_H

/**
 * Main encode loop.
 * Captures RGB frames and writes them to an h264 stream.
 *
 * @param filename   Output filename.
 * @param frames     Number of frames to record. Use 0 to record forever.
 * @param delay      Delay in seconds between each screenshot.
 * @param framerate  FPS for video output.
 */
void encode_loop(const char *filename, long long int frames, unsigned int delay,
                 int framerate);

#endif /* ENCODE_H */
