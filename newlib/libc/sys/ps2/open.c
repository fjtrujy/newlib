
int fio_open( const char *, int  );

int open(const char *buf, int flags, int mode)
{
	return fioOpen( buf, flags );	
}
