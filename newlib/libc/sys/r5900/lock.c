#ifndef __SINGLE_THREAD__

#include <sys/lock.h>
#include <reent.h>
#include <stdio.h>

#define __LOCK_KEY    -1

extern int GetThreadId(void);
extern int CreateSema(void *sema);
extern int DeleteSema(int);
extern int WaitSema(int);
extern int SignalSema(int);
extern int PollSema(int);

typedef struct {
  signed int   count;
  signed int   max_count;
  signed int   init_count;
  signed int   wait_threads;
  unsigned int attr;
  unsigned int option;
} ee_sema_t;

int
__create_mutex(void)
{
  ee_sema_t mutex;

  mutex.init_count = 1;
  mutex.max_count = 1;

  return CreateSema(&mutex);
}

void
__retarget_lock_init(_LOCK_T *lock)
{
  if (lock->mutex_id < 0) {
    lock->mutex_id = __create_mutex();
  }
  lock->jailer = __LOCK_KEY;
  lock->count = -1;
}

void
__retarget_lock_init_recursive(_LOCK_T *lock)
{
  if (lock->mutex_id < 0) {
    lock->mutex_id = __create_mutex();
  }
  lock->jailer = __LOCK_KEY;
  lock->count = 0;
}

void
__retarget_lock_close(_LOCK_T *lock)
{
  if (lock->mutex_id != -1)
    DeleteSema(lock->mutex_id);

  lock->mutex_id = -1;
  lock->jailer = __LOCK_KEY;
  lock->count = -1;
}

void
__retarget_lock_close_recursive(_LOCK_T *lock)
{
  if (lock->mutex_id != -1)
    DeleteSema(lock->mutex_id);

  lock->mutex_id = -1;
  lock->jailer = __LOCK_KEY;
  lock->count = 0;
}

void
__retarget_lock_acquire(_LOCK_T *lock)
{
  if (lock->mutex_id < 0) {
    lock->mutex_id = __create_mutex();
  }

  WaitSema(lock->mutex_id);
  lock->jailer = GetThreadId();
}

void
__retarget_lock_acquire_recursive(_LOCK_T *lock)
{
  if (lock->mutex_id < 0) {
    lock->mutex_id = __create_mutex();
  }

  if (lock->jailer == GetThreadId()) {
    if (lock->count >= 0)
      lock->count++;
  }
  else {
    WaitSema(lock->mutex_id);
    lock->jailer = GetThreadId();

    if (lock->count >= 0)
      lock->count = 1;
  }
}

/* The trylocks don't seem to be used in newlib. */
int
__retarget_lock_try_acquire(_LOCK_T *lock)
{
  if (lock->mutex_id < 0) {
    lock->mutex_id = __create_mutex();
  }

  if (lock->jailer == GetThreadId())
    return 1;

  /* I could use PollSema()->SignalSema()->WaitSema(), but that does a lock,
     unlock, lock chain which may mess with the semaphore's thread queue. */

  /* A jailer has returned the key. */
  if (lock->jailer == __LOCK_KEY) {
    WaitSema(lock->mutex_id);
    lock->jailer = GetThreadId();
    return 1;
  }

  return 0;
}

int
__retarget_lock_try_acquire_recursive(_LOCK_T *lock)
{
  if (lock->mutex_id < 0) {
    lock->mutex_id = __create_mutex();
  }

  if (lock->jailer == GetThreadId()) {
    if (lock->count >= 0) {
      lock->count++;
    }
    return 1;
  }

  /* A jailer has returned the key. */
  if (lock->jailer == __LOCK_KEY) {
    WaitSema(lock->mutex_id);
    lock->jailer = GetThreadId();

    if (lock->count >= 0)
      lock->count = 1;

    return 1;
  }

  return 0;
}

void
__retarget_lock_release(_LOCK_T *lock)
{
  lock->jailer = __LOCK_KEY;
  SignalSema(lock->mutex_id);
}

void
__retarget_lock_release_recursive(_LOCK_T *lock)
{
  if (lock->count > 0)
    lock->count--;

  if (!lock->count) {
    lock->jailer = __LOCK_KEY;
    SignalSema(lock->mutex_id);
  }
}

#endif /*__SINGLE_THREAD__*/
