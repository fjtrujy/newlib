/* connector for rmdir */

#include <reent.h>
#include <unistd.h>

int
rmdir (const char *path)
{
  int ret;

  ret = _rmdir_r (_REENT, path);
  return ret;
}
