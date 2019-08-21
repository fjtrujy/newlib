#include <reent.h>
#include <errno.h>

int _wait_r(struct _reent *reent, int *status)
{
  reent->_errno = ENOSYS;
  return -1;
}
