#include <stdlib.h>
#include <string.h>

#include "energy.h"
#include "image.h"
#include "indexing.h"
#include "test_common.h"

struct image* create_small2() {
    struct image* img = image_init(3, 3);

    img->pixels[0].r = 255;
    img->pixels[0].g = 255;
    img->pixels[0].b = 0;
    img->pixels[1].r = 0;
    img->pixels[1].g = 0;
    img->pixels[1].b = 255;
    img->pixels[2].r = 0;
    img->pixels[2].g = 255;
    img->pixels[2].b = 0;

    img->pixels[3].r = 255;
    img->pixels[3].g = 255;
    img->pixels[3].b = 0;
    img->pixels[4].r = 255;
    img->pixels[4].g = 0;
    img->pixels[4].b = 0;
    img->pixels[5].r = 255;
    img->pixels[5].g = 255;
    img->pixels[5].b = 0;

    img->pixels[6].r = 0;
    img->pixels[6].g = 0;
    img->pixels[6].b = 255;
    img->pixels[7].r = 0;
    img->pixels[7].g = 0;
    img->pixels[7].b = 1;
    img->pixels[8].r = 0;
    img->pixels[8].g = 0;
    img->pixels[8].b = 1;

    return img;
}

struct image* create_carved_small2() {
    struct image* img = image_init(3, 3);

    img->pixels[0].r = 0;
    img->pixels[0].g = 0;
    img->pixels[0].b = 255;
    img->pixels[1].r = 0;
    img->pixels[1].g = 255;
    img->pixels[1].b = 0;
    img->pixels[2].r = 0;
    img->pixels[2].g = 0;
    img->pixels[2].b = 0;

    img->pixels[3].r = 255;
    img->pixels[3].g = 0;
    img->pixels[3].b = 0;
    img->pixels[4].r = 255;
    img->pixels[4].g = 255;
    img->pixels[4].b = 0;
    img->pixels[5].r = 0;
    img->pixels[5].g = 0;
    img->pixels[5].b = 0;

    img->pixels[6].r = 0;
    img->pixels[6].g = 0;
    img->pixels[6].b = 255;
    img->pixels[7].r = 0;
    img->pixels[7].g = 0;
    img->pixels[7].b = 1;
    img->pixels[8].r = 0;
    img->pixels[8].g = 0;
    img->pixels[8].b = 0;

    return img;
}

struct image* create_wide() {
    struct image* img = image_init(10, 2);

    img->pixels[0].r = 42;
    img->pixels[1].r = 42;
    img->pixels[2].r = 42;
    img->pixels[3].r = 42;
    img->pixels[4].r = 42;
    img->pixels[5].r = 42;
    img->pixels[6].r = 42;
    img->pixels[7].r = 42;
    img->pixels[8].r = 42;
    img->pixels[9].r = 42;

    img->pixels[0].g = 7;
    img->pixels[1].g = 7;
    img->pixels[2].g = 7;
    img->pixels[3].g = 7;
    img->pixels[4].g = 7;
    img->pixels[5].g = 7;
    img->pixels[6].g = 7;
    img->pixels[7].g = 7;
    img->pixels[8].g = 7;
    img->pixels[9].g = 7;

    img->pixels[0].b = 200;
    img->pixels[1].b = 200;
    img->pixels[2].b = 200;
    img->pixels[3].b = 200;
    img->pixels[4].b = 200;
    img->pixels[5].b = 200;
    img->pixels[6].b = 200;
    img->pixels[7].b = 200;
    img->pixels[8].b = 200;
    img->pixels[9].b = 200;

    //
    img->pixels[10].r = 12;  // 30^2
    img->pixels[11].r = 22;  // 20^2 + 10^2
    img->pixels[12].r = 5;   // 37^2 + 17^2
    img->pixels[13].r = 90;  // 48^2 + 85^2
    img->pixels[14].r = 8;   // 34^2 + 82^2
    img->pixels[15].r = 12;  // 30^2 + 16
    img->pixels[16].r = 22;
    img->pixels[17].r = 5;
    img->pixels[18].r = 90;
    img->pixels[19].r = 8;

    img->pixels[10].g = 8;   // 1
    img->pixels[11].g = 12;  // 5^2 + 16
    img->pixels[12].g = 22;  // 15^2 + 10^2
    img->pixels[13].g = 5;   // 4    + 17^2
    img->pixels[14].g = 90;  // 83^2 + 85^2
    img->pixels[15].g = 8;   // 1   + 82^2
    img->pixels[16].g = 12;
    img->pixels[17].g = 22;
    img->pixels[18].g = 5;
    img->pixels[19].g = 90;

    img->pixels[10].b = 5;   // 195^2
    img->pixels[11].b = 90;  // 110^2 + 85^2
    img->pixels[12].b = 8;   // 192^2 + 82^2
    img->pixels[13].b = 12;  // 188^2 + 16
    img->pixels[14].b = 22;  // 178^2 + 10^2
    img->pixels[15].b = 5;   // 195^2 + 17^2
    img->pixels[16].b = 90;
    img->pixels[17].b = 8;
    img->pixels[18].b = 12;
    img->pixels[19].b = 22;

    return img;
}

uint32_t* energy_init(const int w, const int h) {
    return calloc(w * h, sizeof(uint32_t));
}

uint32_t* seam_init(const int h) { return calloc(h, sizeof(uint32_t)); }

result_t brightness_small2_test(const char* test) {
    (void)test;
    struct image* img = create_small2();
    uint8_t bright = image_brightness(img);
    uint8_t exp_bright = 94;
    result_t res = SUCCESS;
    if (bright != exp_bright) {
        printf("expected brightness: %d, but got %d\n", exp_bright, bright);
        res = FAILURE;
    }
    image_destroy(img);
    return res;
}

result_t diff_color_test(const char* test) {
    (void)test;
    struct pixel a, b;
    a.r = 10;
    a.g = 50;
    a.b = 101;
    b.r = 12;
    b.g = 55;
    b.b = 0;
    uint32_t res = diff_color(a, b);
    uint32_t exp_res = 10230;
    if (res != exp_res) {
        printf("expected color diff: %d, but got %d\n", exp_res, res);
        return FAILURE;
    }
    return SUCCESS;
}

uint32_t* create_energy_small2() {
    uint32_t* energy = energy_init(3, 3);

    energy[0] = 0;
    energy[1] = 195075;
    energy[2] = 130050;

    energy[3] = 0;
    energy[4] = 195075;
    energy[5] = 260100;

    energy[6] = 195075;
    energy[7] = 129542;
    energy[8] = 325126;

    return energy;
}

uint32_t* create_energy_wide() {
    uint32_t* energy = energy_init(10, 2);

    energy[0] = 0;
    energy[1] = 0;
    energy[2] = 0;
    energy[3] = 0;
    energy[4] = 0;
    energy[5] = 0;
    energy[6] = 0;
    energy[7] = 0;
    energy[8] = 0;
    energy[9] = 0;

    energy[10] = 30 * 30 + 1 + 195 * 195;
    energy[11] = 19866;
    energy[12] = 37 * 37 + 15 * 15 + 192 * 192 + 17 * 17 + 100 + 82 * 82;
    energy[13] = 48 * 48 + 4 + 188 * 188 + 85 * 85 + 17 * 17 + 16;
    energy[14] = 34 * 34 + 83 * 83 + 178 * 178 + 82 * 82 + 85 * 85 + 100;
    energy[15] = 30 * 30 + 1 + 195 * 195 + 16 + 82 * 82 + 17 * 17;
    energy[16] = 19866;
    energy[17] = 37 * 37 + 15 * 15 + 192 * 192 + 17 * 17 + 100 + 82 * 82;
    energy[18] = 48 * 48 + 4 + 188 * 188 + 85 * 85 + 17 * 17 + 16;
    energy[19] = 34 * 34 + 83 * 83 + 178 * 178 + 82 * 82 + 85 * 85 + 100;

    return energy;
}

uint32_t* create_energy_tall() {
    uint32_t* energy = energy_init(3, 6);

    energy[0] = 0;
    energy[1] = 7;
    energy[2] = 15;

    energy[3] = 14;
    energy[4] = 10;
    energy[5] = 0;

    energy[6] = 25;
    energy[7] = 21;
    energy[8] = 0;

    energy[9] = 1;
    energy[10] = 2;
    energy[11] = 0;

    energy[12] = 5;
    energy[13] = 50;
    energy[14] = 28;

    energy[15] = 7;
    energy[16] = 0;
    energy[17] = 15;

    return energy;
}

uint32_t* create_energy_twelve() {
    uint32_t* energy = energy_init(4, 3);

    energy[0] = 2;
    energy[1] = 0;
    energy[2] = 4;
    energy[3] = 7;

    energy[4] = 1;
    energy[5] = 2;
    energy[6] = 4;
    energy[7] = 0;

    energy[8] = 0;
    energy[9] = 2;
    energy[10] = 1;
    energy[11] = 5;

    return energy;
}

result_t energy_small2_test(const char* test) {
    (void)test;
    const uint32_t w = 3;
    const uint32_t h = 3;
    struct image* img = create_small2();
    uint32_t* energy = energy_init(w, h);
    calculate_energy(energy, img, w);
    uint32_t* ref_energy = create_energy_small2();

    result_t res = SUCCESS;
    for (uint32_t i = 0; i < w * h; i++) {
        if (energy[i] != ref_energy[i]) {
            printf("energy at %d: %d\nref energy: %d", i, energy[i],
                   ref_energy[i]);
            res = FAILURE;
            break;
        }
    }
    image_destroy(img);
    free(energy);
    free(ref_energy);
    return res;
}

result_t min_energy_wide_1_test(const char* test) {
    (void)test;
    uint32_t w = 10;
    uint32_t h = 2;
    uint32_t* energy = create_energy_wide();
    uint32_t col = calculate_min_energy_column(energy, w, w, h);
    uint32_t exp_col = 1;
    result_t res = SUCCESS;
    if (col != exp_col) {
        printf("expected min energy column index %d, but got %d\n", exp_col,
               col);
        res = FAILURE;
    }
    free(energy);
    return res;
}

result_t optimal_path_tall_test(const char* test) {
    (void)test;
    const uint32_t w = 3;
    const uint32_t h = 6;
    uint32_t* seam = seam_init(h);
    uint32_t* energy = create_energy_tall();

    calculate_optimal_path(energy, w, w, h, 1, seam);

    uint32_t* exp_seam = seam_init(h);
    exp_seam[5] = 1;
    exp_seam[4] = 0;
    exp_seam[3] = 0;
    exp_seam[2] = 1;
    exp_seam[1] = 2;
    exp_seam[0] = 1;

    result_t res = SUCCESS;
    for (int i = 5; i >= 0; i--) {
        if (seam[i] != exp_seam[i]) {
            printf("at row %d: expected seam column %d, but got %d", i,
                   exp_seam[i], seam[i]);
            res = FAILURE;
            break;
        }
    }
    free(energy);
    free(exp_seam);
    free(seam);
    return res;
}

result_t energy_wide_test(const char* test) {
    (void)test;
    const uint32_t w = 10;
    const uint32_t h = 2;
    struct image* img = create_wide();
    uint32_t* energy = energy_init(w, h);
    calculate_energy(energy, img, w);
    uint32_t* ref_energy = create_energy_wide();

    result_t res = SUCCESS;
    for (int i = 0; i < 20; i++) {
        if (energy[i] != ref_energy[i]) {
            printf("energy at %d: %d\nref energy: %d\n", i, energy[i],
                   ref_energy[i]);
            res = FAILURE;
            break;
        }
    }
    image_destroy(img);
    free(energy);
    free(ref_energy);
    return res;
}

result_t carve_path_small2_test(const char* test) {
    (void)test;
    const uint32_t w = 3;
    const uint32_t h = 3;
    struct image* img = create_small2();
    uint32_t* seam = seam_init(h);
    seam[0] = 0;
    seam[1] = 0;
    seam[2] = 1;
    carve_path(img, w, seam);

    struct image* exp_img = create_carved_small2();
    result_t res = SUCCESS;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            int i = yx_index(y, x, w);
            struct pixel p = img->pixels[i];
            struct pixel exp_p = exp_img->pixels[i];
            printf("Indian");
            if (p.r != exp_p.r || p.g != exp_p.g || p.b != exp_p.b) {
                printf(
                    "at row %d, column %d: expected %d %d %d, but got %d %d %d",
                    y, x, exp_p.r, exp_p.g, exp_p.b, p.r, p.g, p.b);
                res = FAILURE;
            }
        }
    }
    image_destroy(exp_img);
    image_destroy(img);
    free(seam);
    return res;
}

test_fun_t get_test(const char* test) {
    TEST("public.statistics.brightness_small2", brightness_small2_test);
    TEST("public.min_path.diff_color", diff_color_test);
    TEST("public.min_path.energy_small2", energy_small2_test);
    TEST("public.min_path.energy_wide", energy_wide_test);
    TEST("public.min_path.min_energy_wide_1", min_energy_wide_1_test);
    TEST("public.min_path.optimal_path_tall", optimal_path_tall_test);
    TEST("public.carve.carve_path_small2", carve_path_small2_test);
    return NULL;
}
