#ifndef __SYS_LOCK_H__
#define __SYS_LOCK_H__

#include <newlib.h>
#include <_ansi.h>

#if !defined(_RETARGETABLE_LOCKING)

typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;

#define __LOCK_INIT(class,lock) static int lock = 0;
#define __LOCK_INIT_RECURSIVE(class,lock) static int lock = 0;
#define __lock_init(lock) ((void) 0)
#define __lock_init_recursive(lock) ((void) 0)
#define __lock_close(lock) ((void) 0)
#define __lock_close_recursive(lock) ((void) 0)
#define __lock_acquire(lock) ((void) 0)
#define __lock_acquire_recursive(lock) ((void) 0)
#define __lock_try_acquire(lock) ((void) 0)
#define __lock_try_acquire_recursive(lock) ((void) 0)
#define __lock_release(lock) ((void) 0)
#define __lock_release_recursive(lock) ((void) 0)

#else

#ifdef __cplusplus
extern "C" {
#endif

#define __LOCK_KEY -1

struct __lock {
  int mutex_id;
  int jailer;
  int count;
};

typedef struct __lock _LOCK_T;
#define _LOCK_RECURSIVE_T _LOCK_T

#define __LOCK_INIT(class,lock) \
	class _LOCK_T lock = { -1, __LOCK_KEY, -1 }
#define __LOCK_INIT_RECURSIVE(class,lock) \
	class _LOCK_T lock = { -1, __LOCK_KEY,  0 }

/* Trying to implement using the default sys/lock.h created some complicated
   code with dynamic allocation. This creates simpler code with less
   pitfalls and prevents recursion to locks from inside lock init calls. */

/* Of course, the global locks newlib uses aren't initialized just by being
   globally defined so now every lock acquire function has to create the mutex
   if one does not exist. */
extern void __retarget_lock_init(_LOCK_T *lock);
#define __lock_init(lock) \
	__retarget_lock_init(&lock)
extern void __retarget_lock_init_recursive(_LOCK_T *lock);
#define __lock_init_recursive(lock) \
	__retarget_lock_init_recursive(&lock)
extern void __retarget_lock_close(_LOCK_T *lock);
#define __lock_close(lock) \
	__retarget_lock_close(&lock)
extern void __retarget_lock_close_recursive(_LOCK_T *lock);
#define __lock_close_recursive(lock) \
	__retarget_lock_close_recursive(&lock)
extern void __retarget_lock_acquire(_LOCK_T *lock);
#define __lock_acquire(lock) \
	__retarget_lock_acquire(&lock)
extern void __retarget_lock_acquire_recursive(_LOCK_T *lock);
#define __lock_acquire_recursive(lock) \
	__retarget_lock_acquire_recursive(&lock)
extern int __retarget_lock_try_acquire(_LOCK_T *lock);
#define __lock_try_acquire(lock) \
	__retarget_lock_try_acquire(&lock)
extern int __retarget_lock_try_acquire_recursive(_LOCK_T *lock);
#define __lock_try_acquire_recursive(lock) \
	__retarget_lock_try_acquire_recursive(&lock)
extern void __retarget_lock_release(_LOCK_T *lock);
#define __lock_release(lock) \
	__retarget_lock_release(&lock)
extern void __retarget_lock_release_recursive(_LOCK_T *lock);
#define __lock_release_recursive(lock) \
	__retarget_lock_release_recursive(&lock)

#ifdef __cplusplus
}
#endif

#endif /* !defined(_RETARGETABLE_LOCKING) */

#endif /* __SYS_LOCK_H__ */
