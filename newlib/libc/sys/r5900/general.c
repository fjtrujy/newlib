#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


/* fcntl */
#define IO_RDONLY	0x0001
#define IO_WRONLY	0x0002
#define IO_RDWR		0x0003
#define IO_NONBLOCK	0x0010
#define IO_APPEND	0x0100
#define IO_CREAT	0x0200
#define IO_TRUNC	0x0400
#define	IO_EXCL		0x0800

/* pfs mode values */
#define IO_S_IFMT		070000
#define IO_S_IFLNK		040000
#define IO_S_IFREG		020000
#define IO_S_IFDIR		010000

#define IO_S_ISUID		04000
#define IO_S_ISGID		02000
#define IO_S_ISVTX		01000

#define IO_S_IRWXU		0700
#define IO_S_IRUSR		0400
#define IO_S_IWUSR		0200
#define IO_S_IXUSR		0100

#define IO_S_IRWXG		0070
#define IO_S_IRGRP		0040
#define IO_S_IWGRP		0020
#define IO_S_IXGRP		0010

#define IO_S_IRWXO		0007
#define IO_S_IROTH		0004
#define IO_S_IWOTH		0002
#define IO_S_IXOTH		0001

#define IO_S_IRWXA	(IO_S_IRWXU|IO_S_IRWXG|IO_S_IRWXO)
#define IO_S_IRALL	(IO_S_IRUSR|IO_S_IRGRP|IO_S_IROTH)
#define IO_S_IWALL	(IO_S_IWUSR|IO_S_IWGRP|IO_S_IWOTH)
#define IO_S_IXALL	(IO_S_IXUSR|IO_S_IXGRP|IO_S_IXOTH)

#define IO_S_IMASK	(IO_S_ISUID|IO_S_ISGID|IO_S_VTX|IO_S_IRWXA)

#define IO_S_ISLNK(m)	(((m) & IO_S_IFMT) == IO_S_IFLNK)
#define IO_S_ISREG(m)	(((m) & IO_S_IFMT) == IO_S_IFREG)
#define IO_S_ISDIR(m)	(((m) & IO_S_IFMT) == IO_S_IFDIR)

/* Simple filesystem mode values */
#define IO_MC_IFMT		0x0030
#define IO_MC_IFDIR		0x0020
#define IO_MC_IFREG		0x0010

#define IO_MC_RWXC		0x000F
#define IO_MC_R			0x0001
#define IO_MC_W			0x0002
#define IO_MC_X			0x0004
#define IO_MC_C			0x0008

#define IO_MC_ISREG(m)	(((m) & IO_MC_IFMT) == IO_MC_IFREG)
#define IO_MC_ISDIR(m)	(((m) & IO_MC_IFMT) == IO_MC_IFDIR)

/* Determines whether to use MC mode. */
int is_mc_mode(const char *path)
{
  if ((path[0] == 'p') && (path[1] == 'f') && (path[2] == 's'))
    return 0;
  if ((path[0] == 'h') && (path[1] == 'd') && (path[2] == 'd'))
    return 0;

  return 1;
}

/* Converts standard C flags to our flags. */
int c_to_fio_flags(int flags)
{
  int fixed = 0;

  /* O_RDONLY is generally 0. */
  if (!flags)
    fixed |= IO_RDONLY;

  /* Read or write */
  if (flags & O_WRONLY)
    fixed |= IO_WRONLY;

  /* Read and write */
  if (flags & O_RDWR)
    fixed = IO_RDWR;

  /* Creation */
  if (flags & O_CREAT)
    fixed |= IO_CREAT;
  if (flags & O_EXCL)
    fixed |= IO_EXCL;

  if (flags & O_TRUNC)
    fixed |= IO_TRUNC;
  if (flags & O_APPEND)
    fixed |= IO_APPEND;

  return fixed;
}

/* Converts newlib's POSIX file mode to our mode. */
mode_t c_to_fio_mode(int mc, mode_t mode)
{
  mode_t fixed = 0;
  mode_t tmp_mode = 0;

  if (mc)
  {
    if (S_ISDIR(mode))
      fixed = IO_MC_IFDIR;
    if (S_ISREG(mode))
      fixed = IO_MC_IFREG;
  }
  else
  {
    if (S_ISLNK(mode))
      fixed = IO_S_IFLNK;
    if (S_ISREG(mode))
      fixed = IO_S_IFREG;
    if (S_ISDIR(mode))
      fixed = IO_S_IFDIR;
  }

  /* Access */
  if (mode & S_IROTH)
    tmp_mode |= IO_S_IROTH;
  if (mode & S_IWOTH)
    tmp_mode |= IO_S_IWOTH;
  if (mode & S_IXOTH)
    tmp_mode |= IO_S_IXOTH;

  if (mode & S_IRGRP)
    tmp_mode |= IO_S_IRGRP;
  if (mode & S_IWGRP)
    tmp_mode |= IO_S_IWGRP;
  if (mode & S_IXGRP)
    tmp_mode |= IO_S_IXGRP;

  if (mode & S_IRUSR)
    tmp_mode |= IO_S_IRUSR;
  if (mode & S_IWUSR)
    tmp_mode |= IO_S_IWUSR;
  if (mode & S_IXUSR)
    tmp_mode |= IO_S_IXUSR;

  /* Not sure if this should be here. */
  if (mc) {
    if (tmp_mode & IO_S_IRALL)
      fixed |= IO_MC_R;
    if (tmp_mode & IO_S_IWALL)
      fixed |= IO_MC_W;
    if (tmp_mode & IO_S_IXALL)
      fixed |= IO_MC_X;
  }

  return fixed;
}

/* Converts our mode to newlib's POSIX mode. */
mode_t fio_to_c_mode(int mc, mode_t mode)
{
  mode_t fixed = 0;

  if (mc)
  {
    if (IO_MC_ISREG(mode))
      fixed = S_IFREG;
    if (IO_MC_ISDIR(mode)) {
      fixed = S_IFDIR;
      fixed |= (S_IXUSR | S_IXGRP | S_IXOTH);
    }

    if (mode & IO_MC_R)
      fixed |= (S_IRUSR | S_IRGRP | S_IROTH);
    if (mode & IO_MC_W)
      fixed |= S_IWUSR;
    if ((mode & IO_MC_X) && IO_MC_ISDIR(mode))
      fixed |= S_IXUSR;
  }
  else
  {
    if (IO_S_ISLNK(mode))
      fixed = S_IFLNK;
    if (IO_S_ISREG(mode))
      fixed = S_IFREG;
    if (IO_S_ISDIR(mode))
      fixed = S_IFDIR;

    /* Access */
    if (mode & IO_S_IROTH)
      fixed |= S_IROTH;
    if (mode & IO_S_IWOTH)
      fixed |= S_IWOTH;
    if (mode & IO_S_IXOTH)
      fixed |= S_IXOTH;

    if (mode & IO_S_IRGRP)
      fixed |= S_IRGRP;
    if (mode & IO_S_IWGRP)
      fixed |= S_IWGRP;
    if (mode & IO_S_IXGRP)
      fixed |= S_IXGRP;

    if (mode & IO_S_IRUSR)
      fixed |= S_IRUSR;
    if (mode & IO_S_IWUSR)
      fixed |= S_IWUSR;
    if (mode & IO_S_IXUSR)
      fixed |= S_IXUSR;
  }

  return fixed;
}
