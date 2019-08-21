#include <sys/time.h>
#include <sys/reent.h>
#include <errno.h>

int _gettimeofday_r(struct _reent *reent, struct timeval *tv,
		    struct timezone *tz)
{
  reent->_errno = ENOSYS;
  return -1;
}
