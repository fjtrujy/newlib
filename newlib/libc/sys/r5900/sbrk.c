#include <reent.h>
#include <errno.h>

extern unsigned int end;
extern void *EndOfHeap(void);

void *_sbrk_r(struct _reent *reent, ptrdiff_t incr)
{
   static unsigned char *heap_end = (unsigned char*)&end;
   unsigned char *prev_heap_end;

   prev_heap_end = heap_end;
   heap_end += incr;

   if (((ptrdiff_t)heap_end) <= ((ptrdiff_t)EndOfHeap()))
     return prev_heap_end;

   heap_end = prev_heap_end;

   reent->_errno = ENOMEM;

   return ((void*)-1);
}

