#include "image.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

/**
 * Initialize the image @p `img` with width @p `w` and height @p `h`.
 */
struct image* image_init(int const w, int const h) {
    // DO NOT EDIT
    struct image* img = malloc(sizeof(struct image));
    img->w = w;
    img->h = h;
    img->pixels = malloc(w * h * sizeof(*img->pixels));
    memset(img->pixels, 0, w * h * sizeof(*img->pixels));
    return img;
}

/**
 * Destroy the image @p `img` by freeing its pixels field and by freeing @p
 * `img` itself. Don't use @p img afterwards.
 */
void image_destroy(struct image* img) {
    free(img->pixels);
    free(img);
}

/**
 * Read an image from the file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 * @returns the image that was read.
 */
struct image* image_read_from_file(const char* filename) {
    // DO NOT EDIT
    FILE* f = fopen(filename, "r");
    if (f == NULL) exit(EXIT_FAILURE);

    if (fscanf(f, "P3") == EOF) {
        fclose(f);
        exit(EXIT_FAILURE);
    }

    int w, h;
    if (fscanf(f, "%d %d 255 ", &w, &h) == EOF) {
        fclose(f);
        exit(EXIT_FAILURE);
    }
    if (w <= 0 || h <= 0) {
        fclose(f);
        exit(EXIT_FAILURE);
    }

    struct image* img = image_init(w, h);
    struct pixel* pixels = img->pixels;

    for (int y = 0; y < img->h; ++y) {
        unsigned int r, g, b;
        for (int x = 0; x < img->w; ++x, ++pixels) {
            if (fscanf(f, "%u %u %u ", &r, &g, &b) == EOF) {
                image_destroy(img);
                fclose(f);
                exit(EXIT_FAILURE);
            }
            pixels->r = r;
            pixels->g = g;
            pixels->b = b;
        }
    }

    if (fgetc(f) != EOF) {
        image_destroy(img);
        fclose(f);
        exit(EXIT_FAILURE);
    }

    fclose(f);
    return img;
}

/**
 * Write the image @p `img` to file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 */
void image_write_to_file(struct image* img, const char* filename) {
    int w,h;
    w = img->w;
    h = img->h;
    FILE * imgFile = fopen(filename,"wb");
    fprintf(imgFile,"P3\n");
    fprintf(imgFile, "%d %d\n", w, h);
    fprintf(imgFile, "255\n");
    int i,j;
    
    for(i=0;i<h;i++){
        for(j=0;j<w;j++) {
            int cr = yx_index(i,j, w);
            fprintf(imgFile, "%d %d %d ", img->pixels[cr].r, img->pixels[cr].g,  img->pixels[cr].b );
    }
    fprintf(imgFile, "\n");
    }
    fclose(imgFile);
    return;
}

/**
 * Compute the brightness of the image @p `img`.
 */
uint8_t image_brightness(struct image* img) {
    // TODO implement (assignment 3.1)
    int  w, h, i;
    w = img->w;
    h = img->h;

    int sum = 0;
    int localSum  = 0;
    for (i = 0; i < w * h; i++){
        localSum  = 0;
        localSum += img->pixels[i].r;
        localSum += img->pixels[i].g;
        localSum += img->pixels[i].b;
        localSum /= 3;
        sum += localSum;
    }
    return sum/(w*h);
}

/**
 * Carve out the path @p `seam` from the image @p `img`,
 * where only the @p `w` left columns are considered.
 * Move all pixels right of it one to the left and fill the rightmost row with
 * black (0,0,0). Columns with index >= `w` are not considered as part of the
 * image.
 */
void carve_path(struct image* const img, int const w,
                uint32_t const* const seam) {
    int h;
    h = img->h;
    int wid  = img->w;
    for(int i=0;i<h;i++){
        int seamId  = seam[i];
        for(int j=seamId;j<w-1;j++){
            img->pixels[i*wid + j].r = img->pixels[i*wid + j+1].r;
            img->pixels[i*wid + j].g = img->pixels[i*wid + j+1].g;
            img->pixels[i*wid + j].b = img->pixels[i*wid + j+1].b;
        }
        img->pixels[i*wid + w-1].r = 0;
        img->pixels[i*wid + w-1].g = 0;
        img->pixels[i*wid + w-1].b = 0;
    }
}
