#include <reent.h>
#include <errno.h>

int _unlink_r (struct _reent *reent, const char *path)
{
  reent->_errno = ENOSYS;
  return -1;
}
