/*
 * timezone - get and display timezone related information
 * (c) 2003-2004 Michael Wardle
 */

#include <cstdlib>

#include "timezone.h"           // Prototypes for nextdst(), printoff()

/**
 * Prints information about the local time zone's relationship to the universal
 * time zone.
 */
int main(int argc, char *argv[])
{
    time_t clock;               // Temporary system time variable
    int i;                      // Loop counter

    clock = time(0);            // Determine current system time
    printoff(clock);            // Print current DST offset

    for (i = 0; i < 2; i++)     // Find and print the next two DST transitions
    {
        clock = nextdst(clock);
        printoff(clock);
    }
    
    exit(EXIT_SUCCESS);
}
