#include <reent.h>
#include <errno.h>

extern int GetThreadId(void);

int _getpid_r(struct _reent *reent)
{
  return GetThreadId();
}
