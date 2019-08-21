#ifndef _SYS_DIRENT_H_
#define _SYS_DIRENT_H_

#include <sys/types.h>

/** @addtogroup unistd
    @{
*/

#ifdef __cplusplus
extern "C" {
#endif

struct dirent
{
	char d_name[256];
};

typedef struct DIR
{
	int  d_fd;
	char d_dir[256];
} DIR;

/** Open a directory.

    @param path  Directory to open

    @return      A pointer to a directory
    @retval NULL Failure

    @warning     Only a single open directory is supported
*/
DIR *opendir(const char *path);

/** Read entry from open directory

    @param  d    Pointer to directory opened by opendir

    @return      A pointer to a directory entry.
    @retval NULL Failure
*/
struct dirent *readdir(DIR *d);

/** Rewind directory to beginning

    @param d Pointer to directory to rewind

    @note    Rewind is done by closing and opening directory.

    @return  If an error occurs, the directory structure at d will no longer
	     hold a valid file descriptor.
*/
void rewinddir(DIR *d);

/** Close directory

    @param d   Open directory to close
    
    @retval 0  Success
    @retval <0 Failure
*/
int closedir(DIR *d);

/** End of addtogroup unistd
    @}
*/

#ifdef __cplusplus
}
#endif

#endif /* _SYS_DIRENT_H_ */
