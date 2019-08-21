#include <sys/types.h>
#include <fcntl.h>

#include <reent.h>

extern int fioOpen(const char *, int);
extern int fioDopen(const char *);
extern int fileXioOpen(const char *, int, ...);
extern int fileXioDopen(const char *);

extern int c_to_fio_flags(int);
extern int is_mc_mode(const char *);

/* __LARGE64_FILES
int _open64_r(struct _reent *reent, const char *name, int flags, ...)
{
}
*/
int _open_r(struct _reent *reent, const char *name, int flags, int mode)
{
#ifdef USE_FILEXIO
  int mc;
  int fio_mode;
#endif
  int fio_flags;
  int ret;

  fio_flags = c_to_fio_flags(flags);

#ifdef USE_FILEXIO
  mc = is_mc_mode(name);

  fio_mode = c_to_fio_mode(mc,mode);

  if ((ret = fileXioOpen(name,fio_flags,fio_mode)) < 0) {
#else
  if ((ret = fioOpen(name,fio_flags)) < 0) {
#endif
    reent->_errno = ret * -1;
    return -1;
  }

  return ret;
}
