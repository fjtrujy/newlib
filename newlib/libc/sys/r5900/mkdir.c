#include <sys/types.h>
#include <reent.h>

/* The reentrant version exists but not the connecting function that's
   supposed to be in the syscalls dir. */
extern int fioMkdir(const char *path);
extern int fileXioMkdir(const char *path, int mode);

extern int is_mc_mode(const char *path);
extern int c_to_fio_flags(int,mode_t);

int _mkdir_r(struct _reent *reent, const char *path, int mode)
{
#ifdef USE_FILEXIO
  int mc;
  int fio_mode;
#endif
  int ret;

#ifdef USE_FILEXIO
  mc = is_mc_mode(path);

  fio_mode = c_to_fio_mode(mc,mode);

  if ((ret = fileXioMkdir(path,fio_mode)) < 0 ) {
#else
  if ((ret = fioMkdir(path)) < 0) {
#endif
    reent->_errno = ret * -1;
    return -1;
  }

  return 0;
}

int
mkdir (const char *path, mode_t flags)
{
  int ret;

  ret = _mkdir_r (_REENT, path, flags);
  return ret;
}
