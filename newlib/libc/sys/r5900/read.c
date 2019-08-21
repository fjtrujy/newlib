#include <sys/types.h>
#include <reent.h>

extern int fioRead(int, void *, int);
extern int fileXioRead(int, void *, int);

ssize_t _read_r(struct _reent *reent, int fd, void *buf, size_t count)
{
#ifdef USE_FILEXIO
  ssize_t ret = (ssize_t)fileXioRead(fd, buf, count);
#else
  ssize_t ret = (ssize_t)fioRead(fd, buf, count);
#endif

  if (ret < 0) {
    reent->_errno = ret * -1;
    return -1;
  }

  return ret;
}
