#include "syscalls.h"
#include "syscalls/syscaller.h"

using iposix::syscalls::syscaller;

void iposix::syscalls::testcall(
		const char* request,
		int req_length,
		char* response,
		int resp_length)
{
	syscall_message msg;

	msg.call.type = syscalls::test_call;
	msg.testcall_data.request = &request[0];
	msg.testcall_data.req_length = req_length;
	msg.testcall_data.response = &response[0];
	msg.testcall_data.resp_length = resp_length;

	syscaller::call_system(msg);
}

void iposix::syscalls::_exit( int exit_code )
{
	syscall_message msg;

	msg.call.type = syscalls::exit_call;
	msg.exit_data.exit_code = exit_code;

	syscaller::call_system(msg);

	//should _never_ come here

	while( true );
}

int iposix::syscalls::close(int file)
{
	syscall_message msg;

	msg.call.type = syscalls::close_call;
	msg.close_data.file_descriptor = file;

	syscaller::call_system(msg);

	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return 0;	
	}
		
}

int iposix::syscalls::execv( char* name, char** argv )
{
	return ::iposix::syscalls::execve( name, argv, 0 );
}

int iposix::syscalls::execve(char *name, char **argv, char **env)
{
	syscall_message msg;

	msg.call.type = syscalls::execve_call;
	msg.execve_data.file_name = &name[0];
	msg.execve_data.argv = &argv[0];
	msg.execve_data.env = &env[0];

	syscaller::call_system(msg);

	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;
	}
}

int iposix::syscalls::fork(void)
{
	syscall_message msg;

	msg.call.type = syscalls::fork_call;

	syscaller::call_system(msg);

	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;
	}
	else{
		return msg.fork_data.child_pid;
	}
}

int iposix::syscalls::fstat(int file, struct stat *st)
{
	syscall_message msg;

	msg.call.type = syscalls::fstat_call;
	msg.fstat_data.file_descriptor = file;
	msg.fstat_data.st = st;

	syscaller::call_system(msg);

	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return 0;	
	}
		
}

int iposix::syscalls::getpid(void)
{
	syscall_message msg;

	msg.call.type = syscalls::getpid_call;

	syscaller::call_system(msg);


	return msg.getpid_data.pid;
}

int iposix::syscalls::isatty(int file)
{
	syscall_message msg;

	msg.call.type = syscalls::isatty_call;
	msg.isatty_data.file_descriptor = file;

	syscaller::call_system(msg);

	return msg.isatty_data.is_a_tty;
}

int iposix::syscalls::kill(int pid, int sig)
{
	syscall_message msg;

	msg.call.type = syscalls::kill_call;
	msg.kill_data.pid = pid;
	msg.kill_data.signal.number = sig;

	syscaller::call_system(msg);
	
	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;
	}
	else{
		return 0;
	}
}

int iposix::syscalls::link(char *from, char *to)
{
	syscall_message msg;

	msg.call.type = syscalls::link_call;
	msg.link_data.from = &from[0];
	msg.link_data.to = &to[0];

	syscaller::call_system(msg);
	
	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return 0;	
	}
		
}

int iposix::syscalls::lseek(int file, int offset, int seek_type)
{
	syscall_message msg;

	msg.call.type = syscalls::lseek_call;
	msg.lseek_data.file_descriptor = file;
	msg.lseek_data.offset = offset;
	msg.lseek_data.seek.number = seek_type;

	syscaller::call_system(msg);
	
	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return msg.lseek_data.offset;	
	}
		
}

int iposix::syscalls::open(const char *path, int oflags, mode_t mode)
{
	syscall_message msg;

	msg.call.type = syscalls::open_call;
	msg.open_data.path_name = &path[0];
	msg.open_data.flags = oflags;
	msg.open_data.create_mode = mode;

	syscaller::call_system(msg);
	
	if(msg.error.number!=0)
		*__errno() = msg.error.number;

	if(msg.error.number==0) 
		return msg.open_data.file_descriptor;
	else return -1;
}

int iposix::syscalls::read(int file, char *ptr, uint32_t len){

	syscall_message msg;

	msg.call.type = syscalls::read_call;
	msg.read_data.file_descriptor = file;
	msg.read_data.buffer = &ptr[0];
	msg.read_data.length = len;

	syscaller::call_system(msg);
	
	if(msg.error.number==0){
		return len;}
	else {
		*__errno() = msg.error.number;

		return -1;
	}
}
void* iposix::syscalls::sbrk(int incr)
{
	syscall_message msg;

	msg.call.type = syscalls::sbrk_call;
	msg.sbrk_data.incr = incr;
	msg.sbrk_data.addr = 0;

	syscaller::call_system(msg);

	return msg.sbrk_data.addr;
}

int iposix::syscalls::stat(const char *file, struct stat *st)
{
	syscall_message msg;

	msg.call.type = syscalls::stat_call;
	msg.stat_data.path_name = &file[0];
	msg.stat_data.st = st;

	syscaller::call_system(msg);
	
	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return 0;	
	}
		
}

clock_t iposix::syscalls::times(struct tms *buf)
{
	syscall_message msg;

	msg.call.type = syscalls::times_call;
	msg.times_data.buf = buf;
	msg.times_data.clock = 0;

	syscaller::call_system(msg);

	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return msg.times_data.clock;	
	}
}

int iposix::syscalls::unlink(char *name)
{
	syscall_message msg;

	msg.call.type = syscalls::unlink_call;
	msg.unlink_data.file_name = &name[0];

	syscaller::call_system(msg);
	

	return msg.error.number;
}

int iposix::syscalls::wait(int *status)
{
	syscall_message msg;

	msg.call.type = syscalls::wait_call;
	msg.wait_data.status = 0;
	msg.wait_data.pid = 0;

	syscaller::call_system(msg);

	
	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return msg.wait_data.pid;	
	}
}
		

int iposix::syscalls::write(int file, char *ptr, uint32_t len)
{
	syscall_message msg;

	msg.call.type = syscalls::write_call;
	msg.write_data.file_descriptor = file;
	msg.write_data.buffer = &ptr[0];
	msg.write_data.length = len;

	syscaller::call_system(msg);


	if(msg.error.number==0) {
		return len;}
	else{ 

		*__errno() = msg.error.number;
		return -1;
		}
}

int iposix::syscalls::gettimeofday(struct timeval *p, void *tz)
{
	syscall_message msg;

	msg.call.type = syscalls::gettimeofday_call;
	msg.gettimeofday_data.p = p;
	msg.gettimeofday_data.timezone = tz;

	syscaller::call_system(msg);
	
	if(msg.error.type != 0){
		*__errno() = msg.error.number;
		return -1;}
	else{
		return 0;	
	}
		
}

//////////////////////
// additional syscalls
//////////////////////

int iposix::syscalls::mkdir( const char* , mode_t )
{
	//TODO
	return 0;
}

int iposix::syscalls::rmdir( const char* )
{
	//TODO
	return 0;
}

char* iposix::syscalls::getcwd( char*, size_t)
{
	//TODO
	return 0;
}

int iposix::syscalls::chdir( char* )
{
	//TODO
	return -1;
}

void iposix::syscalls::sync()
{
	//TODO
}

void iposix::syscalls::halt()
{
	//TODO
}

int iposix::syscalls::pipe( int pipefd[2], int flags )
{

	syscall_message msg;

	msg.pipe_data.read_file_descriptor=pipefd[0];
	msg.write_file_descriptor.pipe_data=pipefd[1];
	char* buff;
	unsigned int child_pid=this->fork();

	





	return -1;
}

int iposix::syscalls::dup2( int /*oldfd*/, int /*newfd*/ )
{
	//TODO
	return -1;
}

int iposix::syscalls::putenv( char* /*string*/ )
{
	return -1;
}
