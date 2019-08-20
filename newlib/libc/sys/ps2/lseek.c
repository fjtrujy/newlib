#include <sys/types.h>
#include <errno.h>

int fio_lseek( int fd, off_t offset, int mode );

off_t lseek(int fd, off_t offset, int whence)
{
	return fioLseek( fd, offset, whence );
}
