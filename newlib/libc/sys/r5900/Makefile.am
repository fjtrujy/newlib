## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = cygnus

INCLUDES = $(NEWLIB_CFLAGS) $(CROSS_CFLAGS) $(TARGET_CFLAGS)

AM_CCASFLAGS = $(INCLUDES)

noinst_LIBRARIES = lib.a libcrt0.a

lib_a_SOURCES = close.c dirent.c execve.c fcntl.c fork.c fstat.c \
		general.c getpid.c gettimeofday.c isatty.c kill.c \
		link.c lock.c lseek.c mkdir.c open.c read.c rmdir.c sbrk.c \
		stat.c times.c unlink.c wait.c write.c
lib_a_CCASFLAGS = $(AM_CCASFLAGS)
lib_a_CFLAGS = $(AM_CFLAGS)

libcrt0_a_SOURCES = crt0.S

all-local: crt0.o

ACLOCAL_AMFLAGS = -I ../../.. -I ../../../..
CONFIG_STATUS_DEPENDENCIES = $(newlib_basedir)/configure.host
