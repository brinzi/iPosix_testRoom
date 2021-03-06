#ifndef _COMMON_INCLUDE_SYSCALLS_SYSCALL_STRUCTS_H_
#define _COMMON_INCLUDE_SYSCALLS_SYSCALL_STRUCTS_H_

#include <stdint.h>
#include <signal.h>
#include <stdio.h>

namespace iposix
{
namespace syscalls
{

/* TEST_CALL */
struct testcall_data_t
{
	const char* request;
	unsigned int req_length;
	char* response;
	unsigned int resp_length;
};

/* EXIT */
struct exit_data_t
{
	int exit_code;
};

/* CLOSE */
struct close_data_t
{
	 int file_descriptor;
};

struct fork_data_t
{
	unsigned int child_pid;
};

/* EXECVE */
struct execve_data_t
{
	const char* file_name;
	char * const* argv;
	char * const* env;
};

/* FORK */

/* FSTAT */
struct fstat_data_t
{
	int file_descriptor;
	struct stat* st;
};

/* GETPID */
struct getpid_data_t
{
	unsigned int pid;
};

/* ISATTY */
struct isatty_data_t
{
	 int file_descriptor;
	int is_a_tty;
};

/* KILL */
enum signal_type
{
	signal_none = 0,
	signal_hangup = SIGHUP,
	signal_interrupt = SIGINT,
	signal_quit = SIGQUIT,
	signal_illegal_instruction = SIGILL,
	signal_trace_trap = SIGTRAP,
	signal_iot_instruction = SIGIOT,
	signal_abort = SIGABRT,
	signal_emt_instruction = SIGEMT,
	signal_floating_point_exception = SIGFPE,
	signal_kill = SIGKILL,
	signal_bus_error = SIGBUS,
	signal_segmentation_violation = SIGSEGV,
	signal_bad_arg_to_syscall = SIGSYS,
	signal_write_pipe_no_read = SIGPIPE,
	signal_alarm_clock = SIGALRM,
	signal_software_termination = SIGTERM
};

struct kill_data_t
{
	unsigned int pid;
	union __attribute__ ((packed))
	{
		int number;
		signal_type type;
	} signal;
};

/* LINK */
struct link_data_t
{
	const char* from;
	const char* to;
	//TODO may add lengths here
};

/* LSEEK */
enum seek_type
{
	set = SEEK_SET,
	reltive_to_current = SEEK_CUR,
	relative_to_end = SEEK_END

};

struct lseek_data_t
{
	int file_descriptor;
	int64_t offset;
	union __attribute__ ((packed))
	{
		int32_t number;
		seek_type type;
	} seek;
};

/* OPEN */
struct open_data_t
{	
	int file_descriptor;
	const char* path_name;
	unsigned int flags;
	unsigned int create_mode;
};

/* READ */
struct read_data_t
{
	int file_descriptor;
	char* buffer;
	uint32_t length;
};

/* SBRK */
struct sbrk_data_t
{
	unsigned int incr;
	void * addr;
};

/* STAT */
struct stat_data_t
{
	const char* path_name;
	struct stat* st;
};

/* TIMES */
struct times_data_t
{
	struct tms* buf;
	clock_t clock;
};

/* UNLINK */
struct unlink_data_t
{
	const char* file_name;
};

/* WAIT */
struct wait_data_t
{
	unsigned int status;
	unsigned int pid;
};

/* WRITE */
struct write_data_t
{
	int file_descriptor;
	const char* buffer;
	uint32_t length;
};

/* GETTIMEOFDAY */
struct gettimeofday_data_t
{
	struct timeval* p;
	void* timezone;
};

struct pipe_data_t
{
	int read_file_descriptor;
	int write_file_descriptor;
	unsigned int pid;
	unsigned int child_pid;
	char* buffer;
};
	

} //namespace syscall
} //namespace iposix

#endif /* !_COMMON_INCLUDE_SYSCALLS_SYSCALL_STRUCTS_H_ */
