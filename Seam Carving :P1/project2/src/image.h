#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdio.h>

/**
 * A pixel is represented by three `uint8_t` (0-255) values for red (`r`), green
 * (`g`) and blue (`b`).
 */
struct pixel {
    uint8_t r, g, b;
};

/**
 * An image contains its width `w`, its height `h` and an array that holds its
 * pixels, row by row.
 */
struct image {
    int w, h;
    struct pixel* pixels;
};

/**
 * Initialize the image @p `img` with width @p `w` and height @p `h`.
 */
struct image* image_init(int w, int h);

/**
 * Destroy the image @p `img` by freeing its pixels field and by freeing @p
 * `img` itself. Don't use @p img afterwards.
 */
void image_destroy(struct image* img);

/**
 * Read an image from the file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 * @returns the image that was read.
 */
struct image* image_read_from_file(const char* filename);

/**
 * Write the image @p `img` to file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 */
void image_write_to_file(struct image* img, const char* filename);

/**
 * Compute the brightness of the image @p `img`.
 */
uint8_t image_brightness(struct image* img);

/**
 * Carve out the path @p `seam` from the image @p `img`,
 * where only the @p `w` left columns are considered.
 * Move all pixels right of it one to the left and fill the rightmost row with
 * black (0,0,0). Columns with index >= `w` are not considered as part of the
 * image.
 */
void carve_path(struct image* image, int w, uint32_t const* seam);

#endif
