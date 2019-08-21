#include <reent.h>
#include <errno.h>

extern int fioClose(int fd);
extern int fileXioClose(int fd);

int _close_r(struct _reent *reent, int fd)
{
  int err = 0;

#ifdef USE_FILEXIO
  err = fileXioClose(fd);
#else
  err = fioClose(fd);
#endif

  if (err < 0) {
    reent->_errno = err * -1;
    return -1;
  }

  return 0;
}
