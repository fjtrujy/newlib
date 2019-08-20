#include <errno.h>
#include <sys/types.h>

caddr_t ps2_sbrk( int incr );

caddr_t sbrk( int incr )
{
	return ps2_sbrk( incr );
}
