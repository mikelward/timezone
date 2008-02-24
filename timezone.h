/*
 * timezone - timezone information functions
 * (c) 2003-2004 Michael Wardle
 */

#include <time.h>

int dsttype();

long dstoffset();
long dstoffset(time_t clock);

int isdst();
int isdst(time_t clock);

time_t nextdst();
time_t nextdst(time_t clock);

void printoff();
void printoff(time_t clock);

long utcoffset();
long utcoffset(time_t clock);
