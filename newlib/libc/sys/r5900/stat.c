#include <sys/types.h>
#include <sys/stat.h>
#include <reent.h>
#include <errno.h>

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

extern int is_mc_mode(const char *);
extern mode_t fio_to_c_mode(int,mode_t);

extern int fioGetstat(const char *, io_stat_t *buf);
extern int fileXioGetStat(const char *, io_stat_t *buf);

/* __LARGE64_FILES
int _stat64_r (struct _reent *reent, const char *path, struct stat64 *st)
{
}
*/
typedef struct {
  unsigned char reserved0;
  unsigned char sec;
  unsigned char min;
  unsigned char hour;
  unsigned char day;   //1-31
  unsigned char mon;   //1-12
  unsigned short year; //4-digits
} ps2time;

int _stat_r (struct _reent *reent, const char *path, struct stat *st)
{
  int ret;
  long long high;
  int mc = 0;
  io_stat_t f_st;
  ps2time *ps2tm;
  struct tm loctm;

#ifdef USE_FILEXIO
  if ((ret = fileXioGetStat(path,&f_st)) < 0) {
#else
  if ((ret = fioGetstat(path,&f_st)) < 0) {
#endif
    reent->_errno = ret * -1;    
    return -1;
  }

  mc = is_mc_mode(path);

  /* Type */
  st->st_mode = fio_to_c_mode(mc,f_st.mode);

  /* Size */
  st->st_size = f_st.size;

  if (f_st.hisize) {
    high = f_st.hisize;
    st->st_size = st->st_size + (high << 32);
  }

  ps2tm = (ps2time*)f_st.ctime;
  loctm.tm_year =  ps2tm->year-1900;
  loctm.tm_mon  =  ps2tm->mon-1;
  loctm.tm_mday =  ps2tm->day;
  loctm.tm_hour =  ps2tm->hour;
  loctm.tm_min  =  ps2tm->min;
  loctm.tm_sec  =  ps2tm->sec;
  st->st_ctime  = mktime(&loctm);

  ps2tm = (ps2time*)f_st.atime;
  loctm.tm_year =  ps2tm->year-1900;
  loctm.tm_mon  =  ps2tm->mon-1;
  loctm.tm_mday =  ps2tm->day;
  loctm.tm_hour =  ps2tm->hour;
  loctm.tm_min  =  ps2tm->min;
  loctm.tm_sec  =  ps2tm->sec;
  st->st_atime  = mktime(&loctm);

  ps2tm = (ps2time*)f_st.mtime;
  loctm.tm_year =  ps2tm->year-1900;
  loctm.tm_mon  =  ps2tm->mon-1;
  loctm.tm_mday =  ps2tm->day;
  loctm.tm_hour =  ps2tm->hour;
  loctm.tm_min  =  ps2tm->min;
  loctm.tm_sec  =  ps2tm->sec;
  st->st_mtime  = mktime(&loctm);

  if ((st->st_ctime == -1) || (st->st_atime == -1) || (st->st_mtime == -1))
    return -1;

  return 0;
}
