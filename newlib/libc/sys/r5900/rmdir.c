#include <reent.h>
#include <unistd.h>

/* Not in newlib. */
extern int fioRmdir(const char *dirname);
extern int fileXioRmdir(const char *dirname);

int _rmdir_r(struct _reent *reent, const char *path)
{
#ifdef USE_FILEXIO
  int ret = fileXioRmdir(path);
#else
  int ret = fioRmdir(path);
#endif

  if (ret < 0) {
    reent->_errno = ret * -1;
    return -1;
  }

  return 0;
}

int
rmdir (const char *path)
{
  int ret;

  ret = _rmdir_r (_REENT, path);
  return ret;
}
