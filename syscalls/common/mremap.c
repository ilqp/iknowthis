#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <glib.h>
#include <asm/unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>

#include "sysfuzz.h"
#include "typelib.h"
#include "iknowthis.h"

// Re-map a virtual memory address.
// void *mremap(void *old_address, size_t old_size, size_t new_size, int flags);
SYSFUZZ(mremap, __NR_mremap, SYS_NONE, CLONE_DEFAULT, 0)
{
	gint        retcode;
	guintptr    address;
	gint        newaddr;
	gsize       oldsize;
	gsize       newsize;

	typelib_get_vma(this, &address, &oldsize);
	
	newsize = g_random_int_range(0, 0x10000);
	retcode = spawn_syscall_lwp(this, &newaddr, __NR_mremap,                                // void *
	                            address,                                                    // void *old_address
	                            oldsize,                                                    // size_t old_size
	                            newsize,                                                    // size_t new_size
	                            typelib_get_integer_mask(MREMAP_FIXED | MREMAP_MAYMOVE),    // int flags
                                typelib_get_integer());                                     // unsigned long new_addr

    if (retcode == ESUCCESS) {
    	// FIXME: Do something like this.
    	// typelib_vma_moved(this, address, newaddr, newsize);
        typelib_vma_stale(this, address);
        typelib_vma_new(this, newaddr, newsize, VMA_NONE);
    }

    return retcode;
}

