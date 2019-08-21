#include <fcntl.h>
#include <errno.h>
#include <reent.h>

int _fcntl_r(struct _reent *reent, int fd, int cmd, int arg)
{
  reent->_errno = ENOSYS;
  return -1;
}
