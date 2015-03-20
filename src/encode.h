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
 */
void encode_loop(const char *filename);

#endif /* ENCODE_H */
