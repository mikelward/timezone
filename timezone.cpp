/*
 * timezone - timezone information functions
 * (c) 2003-2004 Michael Wardle
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <time.h>               // Prototypes for time(), localtime(), gmtime()

#include "timezone.h"           // Prototypes for utcoffset(), nextdst()

extern long timezone;           // UTC offset in seconds West of UTC
                                // (not allowing for daylight savings time)

/**
 * Determines daylight savings time offset at current time.
 *
 * @return                      daylight savings time offset in seconds or 0 if error
 */
long dstoffset()
{
    time_t now;

    now = time(0);
    if (now < 0)
    {
        perror("Unable to determine system time");
        return 0;
    }
    return dstoffset(now);
}


/**
 * Determines daylight savings time offset at clock.
 *
 * @param clock                 time to determine daylight savings offset
 * @return                      daylight savings time offset in seconds or 0 if error
 */
long dstoffset(time_t clock)
{
    struct tm *tm;              // Temporary calendar time variable
    int isdst;                  // Whether DST applies at clock
    time_t lclock;              // Local system time
    long offset;                // Daylight savings time offset in seconds

    if (clock < 0)              // Ensure parameter is sane
    {
        perror("System time is invalid");
        return 0;
    }

    tm = (struct tm *)malloc(sizeof(struct tm));
    if (!tm)
    {
        perror("Unable to allocate memory");
        return 0;
    }
 
    tzset();                    // Ensure timezone is set
    tm = localtime(&clock);     // Create local calendar time
    if (!tm)
    {
        perror("Unable to convert system time to calendar time");
        return 0;
    }
    isdst = tm->tm_isdst;       // Store DST status
    offset = 0;                 // Default to no offset

#if defined(_WIN32)
    lclock = clock - timezone;  // Determine local standard system time
                                // (UTC plus standard time zone offset)
    tm = gmtime(&lclock);       // Create local calendar time with no time zone
    //tm->tm_isdst = daylight;  // Reinstate DST
    tm->tm_isdst = isdst;       // Reinstate DST if it applied at clock
    clock = mktime(tm);         // Convert local time back to UTC system time
    if (clock < 0)
    {
        perror("Unable to convert calendar time to system time");
        return 0;
    }
    offset = (long)lclock + timezone - clock;
#else // POSIX
    tm->tm_isdst = 0;
    lclock = mktime(tm);        // Determine universal system time plus DST offset
    if (lclock < 0)
    {
        perror("Unable to convert calendar time to system time");
        return 0;
    }
    offset = (long)lclock - clock;
#endif // _WIN32
    return offset;
}

/**
 * Determine the type of daylight savings time adjustment used in the current
 * time zone.
 *
 * @return                      >0 if DST is used, 0 if not
 */
int dsttype()
{
    tzset();
    return daylight;
}

/**
 * Determines whether daylight savings time applies at current time.
 *
 * @return                      >0 if DST applies, 0 if not
 */
int isdst()
{
    time_t now;

    now = time(0);
    if (now < 0)
    {
        perror("Unable to determine system time");
        return -1;
    }
    return isdst(now);
}

/**
 * Determine whether daylight savings time applies at clock.
 *
 * @param clock                 time to test whether DST applies
 * @return                      >0 if DST applies, 0 if not
 */
int isdst(time_t clock)
{
    struct tm *tm;              // Temporary calendar time
    int dst;                    // Whether daylight savings applies at clock

    tm = (struct tm *)malloc(sizeof(struct tm));
    if (!tm)
    {
        perror("Unable to allocate memory");
        return -1;
    }

    tm = localtime(&clock);     // Determine local calendar time for clock
    dst = tm->tm_isdst;         // Store daylight savings time status at clock
    return dst;
}

/**
 * Find first daylight savings time transition after current time.
 *
 * @return                      system time of next DST transition
 */
time_t nextdst()
{
    time_t now;

    now = time(0);              // Determine current system time
    if (now < 0)
    {
        perror("Unable to determine system time");
        return -1;
    }
    return nextdst(now);
}

/**
 * Find first daylight savings time transition after clock.
 *
 * @param clock                 system time from which to begin search
 * @return                      system time of next DST transition
 */
time_t nextdst(time_t clock)
{
    int dst;                    // Whether daylight savings applies at clock

    dst = isdst(clock);         // Determine current DST status
    clock -= (clock % 60);      // Round time down to nearest minute
    while (isdst(clock) == dst) // Find the next time where DST is different
    {
        clock += 60;            // Skip 1 minute
    }
    return clock;
}

/**
 * Prints the number of hours difference between local time and universal time
 * now.
 *
 */
void printoff()
{
    time_t now;

    now = time(0);
    if (time < 0)
    {
        perror("Unable to determine system time");
        return;
    }
    printoff(now);
}

/**
 * Prints the number of hours difference between local time and universal time
 * at clock.
 *
 * @param clock                 system time at which the offset applies
 * @see time
 * @see utcoffset
 */
void printoff(time_t clock)
{
    long offset;

    if (clock < 0)
    {
        perror("Invalid system time");
        return;
    }
    offset = utcoffset(clock);
    if (offset == -1)
    {
        perror("Invalid offset");
        return;
    }

    std::string time;
    time = ctime(&clock);
    time[time.length()-1] = 0;

    std::cout << "Difference between local time and universal time"
              << " at " << time
              << " is " << offset/60.0/60.0 << " hours"
              << std::endl;
}

/**
 * Determines the difference between local time and universal time (UTC)
 * at the current time.
 *
 * @return                      seconds East of UTC
 */
long utcoffset()
{
    time_t now;                 // Current system time

    now = time(0);              // Get current system time
    if (now < 0)
    {
        perror("Unable to determine system time");
        return -1;
    }
    return utcoffset(now);
}

/**
 * Determines the difference between local time and universal time (UTC)
 * at the specified time.
 *
 * @param clock                 system time the offset should be determined for
 * @return                      seconds East of UTC, -1 if error
 * @see                         dstoffset
 */
long utcoffset(time_t clock)
{
    int isdst;                  // Whether daylight savings applies at clock
    long offset;                // Seconds East of UTC in local time zone
    struct tm *tm;              // Temporary calendar time variable

    tm = (struct tm *)malloc(sizeof(struct tm));
    if (!tm)
    {
        perror("Unable to allocate memory");
        return -1;
    }

    tzset();                    // Ensure timezone is set
    tm = localtime(&clock);     // Get current calendar time
    if (!tm)
    {
        perror("Unable to convert system time into calendar time");
        return -1;
    }
    isdst = tm->tm_isdst;       // Save daylight savings time status at clock
#if defined(_BSD_SOURCE)
    offset = tm->tm_gmtoff;     // Retrieve UTC offset from calendar time
#elif defined(_GNU_SOURCE)
    lclock = timegm(tm);        // Determine local system time at clock
    offset = (long)(lclock - clock); // Offset is local subtract universal
#else // POSIX || WIN32
    offset = -timezone;         // Determine standard local time offset
    offset += dstoffset(clock); // Allow for daylight savings time
#endif // _BSD_SOURCE || _GNU_SOURCE
    return offset;
}
