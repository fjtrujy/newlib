#include <sys/times.h>
#include <reent.h>
#include <errno.h>

clock_t _times_r (struct _reent *reent, struct tms *buf)
{
  reent->_errno = ENOSYS;
  return ((clock_t)-1);
}
