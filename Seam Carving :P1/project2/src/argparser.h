#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Parse the arguments and fill in the values on the bool pointers @p
 * `show_min_path`, @p `show_statistics` and @p `n_steps`.
 */
char const* parse_arguments(int argc, char** argv, bool* show_min_path,
                            bool* show_statistics, int* n_steps);

#endif
