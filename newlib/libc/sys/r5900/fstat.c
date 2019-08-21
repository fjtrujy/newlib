#include <sys/types.h>
#include <sys/stat.h>
#include <reent.h>
#include <errno.h>

/* __LARGE64_FILES
int _fstat64_r (struct _reent *reent, int fd, struct stat64 *buf)
{
  reent->_errno = ENOSYS;
  return -1;
}
*/

int _fstat_r(struct _reent *reent, int fd, struct stat *buf)
{
  reent->_errno = ENOSYS;
  return -1;
}
