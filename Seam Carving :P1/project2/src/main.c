#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"
#include "energy.h"
#include "image.h"
#include "util.h"

/**
 * Compute the brightness and print the statistics of @p `img`,
 * i.e. width, height & brightness.
 */
void statistics(struct image *img)
{
    // TODO implement (assignment 3.1)
    /* implement and use the function:
     * `image_brightness`
     */
    printf("width: %u\n", img->w);
    printf("height: %u\n", img->h);
    int brightness = image_brightness(img);
    printf("brightness: %u\n",brightness);

}

uint32_t* energy_init(const int w, const int h) {
    return calloc(w * h, sizeof(uint32_t));
}

uint32_t* seam_init(const int h) { return calloc(h, sizeof(uint32_t)); }

/**
 * Find & print the minimal path of @p `img`.
 */
void find_print_min_path(struct image *img)
{
    int w,h;
    w = img->w;
    h = img->h;

    // store energy
    uint32_t* energy = energy_init(w,h);
    calculate_energy(energy,  img, w);
    int lastSeam = calculate_min_energy_column(energy,w,w,h);
    uint32_t* seam = seam_init(h);
    calculate_optimal_path(energy,w,w,h,lastSeam,seam);
    int i;
    for(i=0;i<h;i++){
        printf("%d\n",seam[i]);
    }
    return;
}

/**
 * Find & carve out @p `n` minimal paths in @p `img`.
 * The image size stays the same, instead for every carved out path there is a
 * column of black pixels appended to the right.
 */
void find_and_carve_path(struct image *const img, int n)
{
    // TODO implement (assignment 3.3)
    /* implement and use the functions from assignment 3.2 and:
     * - `carve_path`
     * - `image_write_to_file`
     * in `image.c`.
     */
    int w = img->w;
    int h = img->h;
    int w_prime = img->w;
    // store energy
    uint32_t* energy = energy_init(w,h);
    for (int i=0; i<n; i++){
        
        calculate_energy(energy,  img, w_prime);
        int lastSeam = calculate_min_energy_column(energy,w,w_prime,h);
        uint32_t* seam = seam_init(h);
        calculate_optimal_path(energy,w,w_prime,h,lastSeam,seam);
        carve_path(img,w_prime,seam);
        w_prime = w_prime-1;
    }
    image_write_to_file(img, "out.ppm");
    return;
}

/**
 * Parse the arguments and call the appropriate functions as specified by the
 * arguments.
 */
int main(int const argc, char **const argv)
{
    // DO NOT EDIT
    bool show_min_path = false;
    bool show_statistics = false;
    int n_steps = -1;

    char const *const filename =
        parse_arguments(argc, argv, &show_min_path, &show_statistics, &n_steps);
    if (!filename)
        return EXIT_FAILURE;

    struct image *img = image_read_from_file(filename);

    if (show_statistics)
    {
        statistics(img);
        image_destroy(img);
        return EXIT_SUCCESS;
    }

    if (show_min_path)
    {
        find_print_min_path(img);
    }
    else
    {
        if (n_steps < 0 || n_steps > img->w)
            n_steps = img->w;

        find_and_carve_path(img, n_steps);
    }

    image_destroy(img);
    return EXIT_SUCCESS;
}
