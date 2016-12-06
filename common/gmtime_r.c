#ifndef HAVE_GMTIME_R
/* Mimic thread-unsafe gmtime_r(3) */

#include <time.h>
#include <string.h>

struct tm * gmtime_r(const time_t *clock, struct tm *result) 
{
int i;
struct tm *t;

t = gmtime(clock);

/* Emulate bcopy(3) */
for (i=0; i < sizeof(struct tm); i++)
  *(((unsigned char *) result)+i) = *(((unsigned char *) t)+i);

return t;
}
#endif /* !HAVE_GMTIME_R */

