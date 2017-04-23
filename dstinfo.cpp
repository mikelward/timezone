/*
 * dstinfo - print information about upcoming daylight savings transitions
 * (c) 2003-2014 Mikel Ward
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "timezone.h"           // Prototypes for nextdst(), printoff()

#define NUM_TRANSITIONS 2      // Number of DST transitions to print

/**
 * Prints information about upcoming daylight savings transitions.
 */
int main(int argc, char *argv[])
{
    time_t t = time(0);
    printoff(t);

    if (!dsttype()) {
        std::cout << "Timezone does not have daylight savings" << std::endl;
        exit(EXIT_SUCCESS);
    }

    for (int i = 0; i < NUM_TRANSITIONS; i++) {
        t = nextdst(t);
        printoff(t);
    }
    exit(EXIT_SUCCESS);
}

// vi: set sw=4 et:
