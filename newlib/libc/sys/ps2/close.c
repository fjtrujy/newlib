
int fio_close( int fd );

int close(int fd)
{
	return fioClose( fd );
}
