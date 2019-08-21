#include <reent.h>
#include <errno.h>

int _isatty_r(struct _reent *reent, int fd)
{
  if (fd == 1 || fd == 2)
  	return 1;

  reent->_errno = ENOTTY;

  return 0;  
}
