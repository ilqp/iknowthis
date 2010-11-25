#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include <glib.h>
#include <asm/unistd.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "sysfuzz.h"
#include "typelib.h"
#include "iknowthis.h"

// Duplicate a file descriptor.
// int dup2(int oldfd, int newfd);
SYSFUZZ(dup2, __NR_dup2, SYS_DISABLED, CLONE_DEFAULT, 0)
{
    gint    retcode;
    gint    result;

    retcode = spawn_syscall_lwp(this, &result, __NR_dup2,                                   // int
                                typelib_get_resource(this, NULL, RES_FILE, RF_NONE),        // int oldfd
                                typelib_get_resource(this, NULL, RES_FILE, RF_NONE));       // int newfd

    return retcode;
}

