#include <reent.h>
#include <errno.h>

int _kill_r (struct _reent *reent, int fd, int sig)
{
  reent->_errno = ENOSYS;
  return -1;
}
