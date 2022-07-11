#include "argparser.h"

#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Print the usage of the program.
 */
static void usage(char const* const name) {
    fprintf(stderr, "usage: %s [-n <count>] [-p] [-s] <image file>\n", name);
}

/**
 * Parse the arguments and fill in the values on the bool pointers @p
 * `show_min_path`, @p `show_statistics` and @p `n_steps`.
 */
char const* parse_arguments(int const argc, char** const argv,
                            bool* show_min_path, bool* show_statistics,
                            int* n_steps) {
    for (;;) {
        switch (getopt(argc, argv, "n:ps")) {
            case -1:
                if (argc - optind != 1) {
                    usage(argv[0]);
                    return NULL;
                }
                return argv[optind];

            case 'n': {
                char* end;
                *n_steps = strtoul(optarg, &end, 0);
                if (end == optarg || *end != '\0')
                    errx(EXIT_FAILURE, "invalid iteration count '%s'", optarg);
                break;
            }

            case 'p':
                *show_min_path = true;
                break;

            case 's':
                *show_statistics = true;
                break;

            case '?':
                usage(argv[0]);
                return NULL;
        }
    }
}
