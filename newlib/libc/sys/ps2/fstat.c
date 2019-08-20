#include <sys/stat.h>

int fstat(int fd, struct stat *buf)
{
	buf->st_mode = S_IFCHR;       /* Always pretend to be a tty */
	buf->st_blksize = 0;

	return (0);
}
