
int fio_read( int fd, void *buff, int buff_size );

int read(int fd, void *buf, int nbytes)
{
	return fioRead( fd, buf, nbytes );
}
