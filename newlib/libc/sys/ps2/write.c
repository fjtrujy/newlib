
int fio_write( int fd, void *buf, int nbytes );

int write(int fd, void *buf, int nbytes)
{
	return fioWrite( fd, buf, nbytes);
}
