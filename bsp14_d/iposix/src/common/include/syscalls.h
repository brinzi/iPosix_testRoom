#ifndef _COMMON_INCLUDE_SYSCALLS_H_
#define _COMMON_INCLUDE_SYSCALLS_H_

/* add syscall headers here */
#include "syscalls/syscall_structs.h"

#include <stdint.h>
/* note these headers are all provided by newlib - you don't need to provide
 * them */
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <stdio.h>
#include <string>

#include <errno.h>
#undef errno

namespace iposix
{
namespace syscalls
{

/**
 * This Enumeration should include every type of syscall message (one or more
 * for each call).
 */
enum syscall_type
{
	test_call,			/**< a test call */
	exit_call,			/**< an exit call */
	close_call,			/**< a close call */
	execve_call,		/**< a execve call */
	fork_call,			/**< a fork call */
	fstat_call,			/**< a fstat call */
	getpid_call,		/**< a getpid call */
	isatty_call,		/**< a is a tty call */
	kill_call,			/**< a kill call */
	link_call,			/**< a link call */
	lseek_call,			/**< a lseek call */
	open_call,			/**< an open call */
	read_call,			/**< a read call */
	sbrk_call,			/**< a sbrk call */
	stat_call,			/**< a stat call */
	times_call,			/**< a times call */
	unlink_call,		/**< a unlink call */
	wait_call,			/**< a wait call */
	write_call,			/**< a write call */
	gettimeofday_call,	/**< a get time of day call */
	pipe_call              /**<pipe call*/
};

/**
 * Converts a syscall type to a string message
 * @param[in] st the syscall type
 * @return the message
 */
inline const ::std::string& syscall_type_to_string( syscall_type st )
{
	static const ::std::string NOSUCH 	= "No such syscall";

	static const ::std::string TEST_CALL			= "Test call";
	static const ::std::string EXIT_CALL			= "Exit call";
	static const ::std::string CLOSE_CALL			= "Close call";
	static const ::std::string EXECVE_CALL			= "Execve call";
	static const ::std::string FORK_CALL			= "Fork call";
	static const ::std::string FSTAT_CALL			= "Fstat call";
	static const ::std::string GETPID_CALL			= "Getpid call";
	static const ::std::string ISATTY_CALL			= "is a tty call";
	static const ::std::string KILL_CALL			= "kill call";
	static const ::std::string LINK_CALL			= "link call";
	static const ::std::string LSEEK_CALL			= "lseek call";
	static const ::std::string OPEN_CALL			= "open call";
	static const ::std::string READ_CALL			= "read call";
	static const ::std::string SBRK_CALL			= "sbrk call";
	static const ::std::string STAT_CALL			= "stat call";
	static const ::std::string TIMES_CALL			= "times call";
	static const ::std::string UNLINK_CALL			= "unlink call";
	static const ::std::string WAIT_CALL			= "wait call";
	static const ::std::string PIPE_CALL			=  "pipe call "; //added by brinzi
	static const ::std::string WRITE_CALL			= "write call";
	static const ::std::string GETTIMEOFDAY_CALL	= "get time of day call";

	switch ( st )
	{
		case test_call:				return TEST_CALL;
		case exit_call:				return EXIT_CALL;
		case close_call:			return CLOSE_CALL;
		case execve_call:			return EXECVE_CALL;
		case fork_call:				return FORK_CALL;
		case fstat_call:			return FSTAT_CALL;
		case getpid_call:			return GETPID_CALL;
		case isatty_call:			return ISATTY_CALL;
		case kill_call:				return KILL_CALL;
		case link_call:				return LINK_CALL;
		case lseek_call:			return LSEEK_CALL;
		case open_call:				return OPEN_CALL;
		case read_call:				return READ_CALL;
		case sbrk_call:				return SBRK_CALL;
		case stat_call:				return STAT_CALL;
		case times_call:			return TIMES_CALL;
		case unlink_call:			return UNLINK_CALL;
		case wait_call:				return WAIT_CALL;
		case write_call:			return WRITE_CALL;
		case gettimeofday_call:		return GETTIMEOFDAY_CALL;
		case pipe_call:				return PIPE_CALL; // added by Brinzi
 	
		default:					return NOSUCH;
	}
}

/**
 * This Enumeration should include every type of error code which could answer a
 * syscall.
 */
enum error_type
{
	none					= 0,			/**< no error */
	e_no_such_entry			= ENOENT,		/**< no such entry error */
	e_no_syscall			= ENOSYS,		/**< no such syscall error */
	e_bad_filedescriptor	= EBADF,		/**< bad filedescriptor error */
	e_no_exec 				= ENOEXEC,		/**< not executable error */
	e_no_mem 				= ENOMEM,		/**< no memory error */
	e_illegal_seek 			= ESPIPE		/**< illegal seek error */
};

/**
 * Converts an error type to a string
 * @param[in] et the error type
 * @return the string
 */
inline const ::std::string& error_type_to_string( error_type et )
{
	static const ::std::string NOSUCH				= "no such error";

	static const ::std::string NONE					= "no error";
	static const ::std::string E_NO_SUCH_ENTRY		= "no such entry";
	static const ::std::string E_NO_SYSCALL			= "no syscall";
	static const ::std::string E_BAD_FILEDESCRIPTOR	= "bad filedescriptor";
	static const ::std::string E_NO_EXEC			= "no exec";
	static const ::std::string E_NO_MEM				= "no mem";
	static const ::std::string E_ILLEGAL_SEEK		= "illegal seek";

	switch ( et )
	{
		case none:					return NONE;
		case e_no_such_entry:		return E_NO_SUCH_ENTRY;
		case e_no_syscall:			return E_NO_SYSCALL;
		case e_bad_filedescriptor:	return E_BAD_FILEDESCRIPTOR;
		case e_no_exec: 			return E_NO_EXEC;
		case e_no_mem: 				return E_NO_MEM;
		case e_illegal_seek: 		return E_ILLEGAL_SEEK;
		default:					return NOSUCH;
	}
}

/**
 * This Struct should include every data for a syscall (one or more request
 * structs and one or more response structs for each syscall).
 */
struct syscall_message
{
	/** the syscall */
	union __attribute__ ((packed))
	{
		uint32_t number;		/**< the syscall number */
		syscall_type type;		/**< the syscall type */
	} call;

	/** the data */
	union __attribute__ ((packed))
	{
		struct testcall_data_t testcall_data;			/**< test data */
		/* add more request structs here */
		struct exit_data_t exit_data;					/**< exit data */
		struct close_data_t close_data;					/**< close data */
		struct fork_data_t fork_data;					/**< fork data */
		struct execve_data_t execve_data;				/**< execve data */
		struct fstat_data_t fstat_data;					/**< fstat data */
		struct getpid_data_t getpid_data;				/**< getpid data */
		struct isatty_data_t isatty_data;				/**< is a tty data */
		struct kill_data_t kill_data;					/**< kill data */
		struct link_data_t link_data;					/**< link data */
		struct lseek_data_t lseek_data;					/**< lseek data */
		struct open_data_t open_data;					/**< open data */
		struct read_data_t read_data;					/**< read data */
		struct sbrk_data_t sbrk_data;					/**< sbrk data */
		struct stat_data_t stat_data;					/**< stat data */
		struct times_data_t times_data;					/**< times data */
		struct unlink_data_t unlink_data;				/**< unlink data */
		struct wait_data_t wait_data;					/**< wait data */
		struct write_data_t write_data;					/**< write data */
		struct gettimeofday_data_t gettimeofday_data;	/**< get time of day data */
		struct pipe_data_t pipe_data;               /**<pipe call*/ //added by brinzi
		
	};

	/** the error */
	union __attribute__ ((packed))
	{
		uint32_t number;		/**< the error number */
		error_type type;		/**< the error type */
	} error;
};

/* add syscall methodes here */

/**
 * The first test call
 * @param[in] request the request message
 * @param[in] req_length the length of the request message
 * @param[in] response the response message
 * @param[in] resp_lenght the length of the response message
 */
extern "C" void testcall(
		const char* request, 
		int req_length, 
		char* response, 
		int resp_length);


/**
 * Terminates the current running Prozess/Thread
 * @param[in] exit_code the exit code
 */
extern "C" void _exit( int exit_code ) __attribute__ ((noreturn));

/**
 * Close a file.
 * @param[in] file the filehandle number
 */
extern "C" int close(int file);

/**
 * Transfer control to a new process
 * @param[in] name the executable name
 * @param[in] argc the arguments
 */
extern "C" int execv(char *name, char **argv );

/**
 * Transfer control to a new process.
 * @param[in] name the executable name
 * @param[in] argv the arguments
 * @param[in] env the environment
 */
extern "C" int execve(char *name, char **argv, char **env);

/**
 * Create a new process.
 * @param[in] 
 */
extern "C" int fork(void);

/**
 * Status of an open file.
 * @param[in] 
 */
extern "C" int fstat(int file, struct stat *st);

/**
 * Process-ID
 * @param[in] 
 */
extern "C" int getpid(void);

/**
 * Query whether output stream is a terminal.
 * @param[in]
 */
extern "C" int isatty(int file);

/**
 * Send a signal.
 * @param[in]
 */
extern "C" int kill(int pid, int sig);

/**
 * Establish a new name for an existing file.
 * @param[in]
 */
extern "C" int link(char *from, char *to);

/**
 * Set position in a file.
 * @param[in]
 */
extern "C" int lseek(int file, int ptr, int dir);

/**
 * Open a file.
 * @param[in]
 */
//int open(const char *name, int flags, int mode) 
extern "C" int open(const char *name, int flags, mode_t mode = 0);

/**
 * Read from a file.
 * @param[in]
 */
extern "C" int read(int file, char *ptr, uint32_t len);

/**
 * Increase program data space. Used by malloc for example.
 * @param[in]
 */
extern "C" void* sbrk(int incr);

/**
 * Status of a file (by name).
 * @param[in]
 */
extern "C" int stat(const char *file, struct stat *st);

/**
 * Timing information for current process.
 * @param[in]
 */
extern "C" clock_t times(struct tms *buf);

/**
 * Remove a file's directory entry.
 * @param[in]
 */
extern "C" int unlink(char *name);

/**
 * Wait for a child process.
 * @param[in]
 */
extern "C" int wait(int *status);

/**
 * Write to a file. libc subroutines will use this system routine for output to all files, including stdout.
 * @param[in]
 */
extern "C" int write(int file, char *ptr, uint32_t len);

//TODO implement and checkout if needed
extern "C" int gettimeofday(struct timeval *p, void *tz);

//////////////////////
// additional syscalls
//////////////////////


/**
 * Creaetes a new directory with name path
 * @param[in]
 */
extern "C" int mkdir( const char* path, mode_t mode );

/**
 * Removes a directory
 * @param[in]
 */
extern "C" int rmdir( const char* path );

/**
 * Changes the current working directory
 * @param[in]
 */
extern "C" int chdir( char* );

/**
 * Gets the current working directory
 * @param[in]
 */
extern "C" char* getcwd( char* buffer, size_t size);

/**
 * Syncs the dirty buffers to disc
 * @param[in]
 */
extern "C" void sync(void);

/**
 * Halts the system
 * @param[in]
 */
extern "C" void halt(void);

/**
 * Creates a pipe
 * @param[in]
 */
extern "C" int pipe( int fd[] );

/**
 * Duplicates a filehandle
 * @param[in]
 */
extern "C" int dup2( int oldfd, int newfd );

/**
 * Set an environment variable
 * @param[in]
 */
extern "C" int putenv( char * string );

} //namespace syscall
} //namespace iposix

#endif /* !_COMMON_INCLUDE_SYSCALLS_H_ */
