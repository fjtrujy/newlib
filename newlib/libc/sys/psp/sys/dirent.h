#ifndef _SYS_DIRENT_H
#define _SYS_DIRENT_H

typedef struct __dirdesc {
	int dd_fd;		/* directory file */
    int dd_loc;		/* position in buffer */
    int dd_seek;
    char *dd_buf;	/* buffer */
    int dd_len;		/* buffer length */
    int dd_size;	/* amount of data in buffer */
} DIR;

# define __dirfd(dp)	((dp)->dd_fd)

#include <sys/stat.h>

#undef  MAXNAMLEN	/* from unistd.h */
#define MAXNAMLEN	255

#define d_ino	d_fileno	/* compatibility */

struct dirent {
	struct stat d_stat;
	unsigned long	d_fileno;
	unsigned short	d_reclen;
	unsigned short	d_namlen;
	char		d_name[MAXNAMLEN + 1];
};

#endif