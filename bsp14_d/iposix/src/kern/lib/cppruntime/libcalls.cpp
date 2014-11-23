#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>

#include <stdio.h>
#include <errno.h>
#undef errno
extern "C" int errno;

#include "mm/kmalloc.h"

/*
 * This file contains calls from the c/c++ library which are normally handeld by
 * a syscall so the OS can handel these, because we are the OS we don't need a
 * syscall we can handel them directly.
 */

namespace iposix
{

using hlmemory::kmalloc;
using hlmemory::kfree;

/**
 * This Namespace containts every Code which depends on the used C++ Runtime
 * Librarys. For example Startup Code.
 *
 * \todo TODO Think about it: These Functions should not be called from
 * userlevel/syscall cause may be we should handle them in a different way.
 *
 * \todo TODO Current these libcalls are dummies and they should be implemented.
 *
 * \todo TODO Documentation!
 */
namespace cppruntime
{

/*
 * These Function should be visible as they are
 */
extern "C"
{
	/**
	 * Exits the current Thread.
	 */
	void _exit();

	/**
	 * Close a file.
	 */
	int close(int file);

	/**
	 * Transfer control to a new process.
	 */
	int execve(char *name, char **argv, char **env);

	/**
	 * Create a new process.
	 */
	int fork(void);

	/**
	 * Status of an open file.
	 */
	int fstat(int file, struct stat *st);

	/**
	 * Process-ID
	 */
	int getpid(void);

	/**
	 * Query whether output stream is a terminal.
	 */
	int isatty(int file);

	/**
	 U
	 * Send a signal.
	 */
	int kill(int pid, int sig);

	/**
	 * Establish a new name for an existing file.
	 */
	int link(char *from, char *to);
	/**
	 * Set position in a file.
	 */
	int lseek(int file, int offset, int seek_type);

	/**
	 * Open a file.
	 */
	int open(const char *path, int oflags, mode_t mode);

	/**
	 * Read from a file.
	 */
	int read(int file, char *ptr, int len);

	/**
	 * Increase program data space. Used by malloc for example.
	 */
	//caddr_t sbrk(int incr);

	/**
	 * Reentrant version of realloc. Since we don't want sbrk we need to provide
	 * this.
	 */
	void* _realloc_r(struct _reent*, void *, size_t);

	/**
	 * Reentrant version of malloc. Since we don't want sbrk we need to provide
	 * this.
	 */
	void* _malloc_r(struct _reent*, size_t);

	/**
	 * Reentrant version of free. Since we don't want sbrk we need to provide
	 * this.
	 */
	void _free_r(struct _reent*, void*);

	/**
	 * Status of a file (by name).
	 */
	int stat(const char *file, struct stat *st);

	/**
	 * Timing information for current process.
	 */
	clock_t times(struct tms *buf);

	/**
	 * Remove a file's directory entry.
	 */
	int unlink(char *name);

	/**
	 * Wait for a child process.
	 */
	int wait(int *status);

	/**
	 * Write to a file. libc subroutines will use this system routine for output
	 * to all files, including stdout.
	 */
	int write(int file, char *ptr, int len);

	/**
	 * Get the current time of day.
	 */
	int gettimeofday(struct timeval *p, void *tz);
}

void debug_help()
{
	
}

/* [> what is this good for <]
 * #undef errno
 * extern int errno;
 *
 * char *__env[1] = { 0 };
 * char **environ = __env;
*/

void _exit()
{
	debug_help();
}

int close(int)
{
	debug_help();
	return -1;
}

int execve(char* , char** , char** )
{
	debug_help();
	return ENOMEM;
}

int fork(void)
{
	debug_help();
	return EAGAIN;
}

int fstat(int , struct stat* st)
{
	debug_help();
	st->st_mode = S_IFCHR;
	return 0;
}

int getpid(void)
{
	debug_help();
	return 1;
}

int isatty(int )
{
	debug_help();
	return 1;
}

int kill(int , int )
{
	debug_help();
	return EINVAL;
}

int link(char *, char *)
{
	debug_help();
	return EMLINK;
}

int lseek(int , int , int )
{
	debug_help();
	return 0;
}

int open(const char* , int , mode_t )
{
	debug_help();
	return -1;
}

int read(int , char *, int )
{
	debug_help();
	return 0;
}

/*
 *caddr_t sbrk(int )
 *{
 *    debug_help();
 *        //extern char _end;		[> Defined by the linker <]
 *        //static char *heap_end;
 *        //char *prev_heap_end;
 *        //static char *stack_ptr = 0; //not a good idea but save
 *
 *        //if (heap_end == 0)
 *        //{
 *            //heap_end = &_end;
 *        //}
 *
 *        //prev_heap_end = heap_end;
 *
 *        //if (heap_end + incr > stack_ptr)
 *        //{
 *            //write (1, "Heap and stack collision\n", 25);
 *            //abort ();
 *        //}
 *
 *        //heap_end += incr;
 *
 *        //return (caddr_t) prev_heap_end;
 *
 *    return 0;
 *}
 */

void* _realloc_r(struct _reent* re, void * old, size_t size)
{
	if( old == 0 && size != 0 )
	{
		return _malloc_r( re, size );
	}
	else if( old == 0 && size == 0 )
	{
		_free_r( re, old );
		return 0;
	}
	else
	{
		::errno = ENOMEM;
		return 0;
	}
}

void* _malloc_r(struct _reent*, size_t size)
{
	void * ret = kmalloc( size, true );
	if( ret == 0 )
	{
		::errno = ENOMEM;
	}

	return ret;
}

void _free_r(struct _reent*, void* addr)
{
	kfree( addr, true );
}

int stat(const char* , struct stat* st)
{
	debug_help();
	st->st_mode = S_IFCHR;
	return 0;
}

clock_t times(struct tms* )
{
	debug_help();
	return -1;
}

int unlink(char* )
{
	debug_help();
	return ENOENT;
}

int wait(int* )
{
	debug_help();
	return ECHILD;
}

int write(int , char* , int )
{
	debug_help();
	return 0;
}

int gettimeofday(struct timeval* , void* )
{
	debug_help();
	return 0;
}

} //namespace cppruntime
} //namespace iposix
