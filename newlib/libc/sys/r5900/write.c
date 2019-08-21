#include <sys/types.h>
#include <reent.h>

extern int fioWrite(int, const void *, int);
extern int fileXioWrite(int, const void *, int);

ssize_t _write_r (struct _reent *reent, int fd, const void *buf, size_t count)
{
#ifdef USE_FILEXIO
  ssize_t ret = (ssize_t)fileXioWrite(fd, buf, count);
#else
  ssize_t ret = (ssize_t)fioWrite(fd, buf, count);
#endif

  if (ret < 0) {
    reent->_errno = ret * -1;
    return -1;
  }

  return ret;
}
