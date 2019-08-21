#include <sys/types.h>
#include <reent.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#define IO_SEEK_SET 0
#define IO_SEEK_CUR 1
#define IO_SEEK_END 2

extern int fioLseek(int,int,int);
extern int fileXioLseek(int,int,int);

/* fileXioLseek64 can support __LARGE_64_FILES
off64_t _lseek64_r (struct _reent *reent, int fd, off64_t offset, int whence)
{
}
*/

off_t _lseek_r(struct _reent *reent, int fd, off_t offset, int whence)
{
  off_t ret = 0;

#ifdef USE_FILEXIO
  if (whence == SEEK_SET)
    ret = (off_t)fileXioLseek(fd,offset,IO_SEEK_SET);

  if (whence == SEEK_CUR)
    ret = (off_t)fileXioLseek(fd,offset,IO_SEEK_CUR);

  if (whence == SEEK_END)
    ret = (off_t)fileXioLseek(fd,offset,IO_SEEK_END);
#else
  if (whence == SEEK_SET)
    ret = (off_t)fioLseek(fd,offset,IO_SEEK_SET);

  if (whence == SEEK_CUR)
    ret = (off_t)fioLseek(fd,offset,IO_SEEK_CUR);

  if (whence == SEEK_END)
    ret = (off_t)fioLseek(fd,offset,IO_SEEK_END);
#endif

  if (ret < 0) {
    reent->_errno = (ret * -1);
    return ((off_t)-1);
  }

  return ret;
}
