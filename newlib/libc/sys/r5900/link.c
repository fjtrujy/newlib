#include <reent.h>
#include <errno.h>

int _link_r(struct _reent *reent, const char *old, const char *new)
{
  reent->_errno = ENOSYS;
  return -1;
}
