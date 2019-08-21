#include <dirent.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>

extern int fioDopen(const char *path);
extern int fioDread(int fd, void *buf);
extern int fioDclose(int fd);
extern int fileXioDopen(const char *path);
extern int fileXioDread(int fd, void *buf);
extern int fileXioDclose(int fd);

struct dirent dirent_internal__;

typedef struct {
	unsigned int	mode;
	unsigned int	attr;
	unsigned int	size;
	unsigned char	ctime[8];
	unsigned char	atime[8];
	unsigned char	mtime[8];
	unsigned int	hisize;
	unsigned int	private_0;
	unsigned int	private_1;
	unsigned int	private_2;
	unsigned int	private_3;
	unsigned int	private_4;
	unsigned int	private_5;
} io_stat_t;

typedef struct {
	io_stat_t	stat;
	char		name[256];
	unsigned int	private_0;
} io_dirent_t;

typedef struct {
	unsigned int	mode;
	unsigned int	attr;
	unsigned int	size;
	unsigned char	ctime[8];
	unsigned char	atime[8];
	unsigned char	mtime[8];
	unsigned int	hisize;
} fio_stat_t;

typedef struct {
	fio_stat_t	stat;
	char		name[256];
	unsigned int	private_0;
	unsigned int	unused[6];
} fio_dirent_t;

int is_fio_dirent(const char *path)
{
  if ((path[0] == 'm') && (path[1] == 'c'))
    return 1;

  return 0;
}

DIR *opendir(const char *path)
{
  DIR *dir;
  int fd;

#ifdef USE_FILEXIO
  if ((fd = fileXioDopen(path)) < 0) {
#else
  if ((fd = fioDopen(path)) < 0) {
#endif
    return NULL;
  }

  dir = (DIR*)malloc(sizeof(DIR));

  strncpy(dir->d_dir, path, 256);

  return dir;
}

struct dirent *readdir(DIR *d)
{
  io_dirent_t io_entry_;
  fio_dirent_t fio_entry_;

  if (d == NULL)
    return NULL;

  if (d->d_fd < 0)
    return NULL;

  /* Some device drivers use a different dirent. I update ps2sdk so
     only memory cards are affected. */
  if (is_fio_dirent(d->d_dir)) {
#ifdef USE_FILEXIO
    if (fileXioDread(d->d_fd, &fio_entry_) < 1) {
#else
    if (fioDread(d->d_fd, &fio_entry_) < 1) {
#endif
      return NULL;
    }
    strncpy(dirent_internal__.d_name, fio_entry_.name, 256);
  }
  else {
#ifdef USE_FILEXIO
    if (fileXioDread(d->d_fd, &io_entry_) < 1) {
#else
    if (fioDread(d->d_fd, &io_entry_) < 1) {
#endif
      return NULL;
    }
    strncpy(dirent_internal__.d_name, io_entry_.name, 256);
  }

  return &dirent_internal__;
}

int closedir(DIR *d)
{
  if (d == NULL)
    return -1;

  if (d->d_fd < 0)
    return -1;

#ifdef USE_FILEXIO
  if (fileXioDclose(d->d_fd) < 0) {
#else
  if (fioDclose(d->d_fd) < 0) {
#endif
    return -1;
  }
  else {
    d->d_fd = -1;
    return 0;
  }
}

void rewinddir(DIR *d)
{
  if (d == NULL)
    return;

  if (d->d_fd < 0)
    return;

  /* Reinitialize by closing and opening. */
#ifdef USE_FILEXIO
  if (fileXioDclose(d->d_fd) < 0)
    return;

  d->d_fd = fileXioDopen(d->d_dir);
#else
  if (fioDclose(d->d_fd) < 0)
    return;

  d->d_fd = fioDopen(d->d_dir);
#endif

  return;
}
