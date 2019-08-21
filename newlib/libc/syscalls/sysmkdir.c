/* connector for mkdir */

#include <reent.h>
#include <sys/stat.h>
#include <sys/types.h>

int
mkdir (const char *name, mode_t flags)
{
  int ret;

  ret = _mkdir_r (_REENT, name, flags);
  return ret;
}
