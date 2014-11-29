#include "syscalls/syscall_dispatcher.h"
#include "exceptions/exceptions.h"
#include "fs/i_fs_node.h"
#include "fs/i_fs.h"
#include "fs/filehandle.h"
#include "elf/i_elf_loader.h"
#include "scheduler/i_scheduler.h"
#include "scheduler/i_process.h"
#include "syscalls.h"
#include "logger/logger.h"
#include "mm/kmalloc.h"
#include "die.h"

#include <exception>

namespace iposix
{
namespace syscalls
{

using ::iposix::scheduler::scheduler_type;
using ::iposix::scheduler::process_type;
using ::iposix::fs::filehandle_ptr;
using ::iposix::fs::i_fs_node_ptr;
using ::iposix::fs::i_fs;
using ::iposix::elf::elf_loader_type;

syscall_dispatcher::syscall_dispatcher()
{ }

syscall_dispatcher::~syscall_dispatcher()
{ }

inline void real_dispatch_syscall( syscall_message& msg )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Got "
		<< syscall_type_to_string( msg.call.type ).c_str() << endl;

	process_type* cur_process =
		::iposix::utils::Singleton< scheduler_type >::instance().get_current_process();

	if ( cur_process == 0 )
	{
		logger << message::ERROR << __func__
			<< ": Should handle syscall but there is no current process. "
			<< "Dont how to handle this. iPosix will die now..."
			<< endl;
		die();
	}

	msg.error.type = syscalls::none;
	
	switch ( msg.call.type )
	{
		case syscalls::test_call:
			{
				static const char* answer = "HELLO Userlevel!";
				logger << message::VERBOSE
					<< msg.testcall_data.request << endl;

				for (int i = 0; i < 16; i++)
				{
					msg.testcall_data.response[i] = answer[i];
				}
			}
			break;

		case syscalls::open_call:
			{
				//get the file
				i_fs_node_ptr file = i_fs::open_node( msg.open_data.path_name );

				//add the file handle
				if ( file )
				{
					msg.open_data.file_descriptor = cur_process->push_filehandle(
							file,
						msg.open_data.flags,
							msg.open_data.create_mode );
				}
				else
				{
					msg.error.type = syscalls::e_no_such_entry;
				}

			}
			break;

		case syscalls::close_call:
			{
				//remove the handle and get the file
				i_fs_node_ptr file =
					cur_process->remove_filehandle( msg.close_data.file_descriptor );

				if ( file )
				{
					i_fs::close_node( file );
				}
				else
				{
					msg.error.type = syscalls::e_bad_filedescriptor;
				}
			}
			break;

		case syscalls::read_call:
			{
				//get the filehandle
				filehandle_ptr fd =
					cur_process->get_filehandle( msg.read_data.file_descriptor );
	
				if ( fd )
				{
					fd->read( msg.read_data.buffer, msg.read_data.length );
				}
				else
				{
					msg.error.type = syscalls::e_bad_filedescriptor;
				}
			}
			break;

		case syscalls::write_call:
			{
				//get the filehandle
				filehandle_ptr fd =
					cur_process->get_filehandle( msg.write_data.file_descriptor );

				if ( fd )
				{
					fd->write( msg.read_data.buffer, msg.write_data.length );
				}
				else
				{
					msg.error.type = syscalls::e_bad_filedescriptor;
				}
			}
			break;

		case syscalls::lseek_call:
			{
				//get the filehandle
				filehandle_ptr fd =
					cur_process->get_filehandle( msg.lseek_data.file_descriptor );

				if ( fd )
				{
					bool success = fd->seek( msg.lseek_data.offset, msg.lseek_data.seek.number );
					if ( !success )
					{
						msg.error.type = syscalls::e_illegal_seek;
					}
				}
				else
				{
					msg.error.type = syscalls::e_bad_filedescriptor;
				}
			}
			break;

		case syscalls::fstat_call:
			{
				//get the filehandle
				filehandle_ptr fd =
					cur_process->get_filehandle( msg.fstat_data.file_descriptor );

				if ( fd )
				{
					bool success = fd->stat( msg.fstat_data.st );
					if ( !success )
					{
						msg.error.type = syscalls::e_no_such_entry;
					}
				}
				else
				{
					msg.error.type = syscalls::e_bad_filedescriptor;
				}
			}
			break;

		case syscalls::sbrk_call:
			{
				process_type * proc = cur_process;

				if( proc->heap_top == 0 )
				{
					logger << message::MORE_VERBOSE << __func__
						<< ": sbrk call failed: heap_top == 0" << endl;
					msg.error.type = syscalls::e_no_mem; //TODO
					break;
				}

				uintptr_t heap_top = reinterpret_cast< uintptr_t >( proc->heap_top );
				uintptr_t mod = heap_top % process_type::page_size;

				//speicher wird nur in page_size einheiten geholt

				if( msg.sbrk_data.incr > 0 )
				{
					if( mod != 0 ) //es ist noch bereits allozierter speicher verfuegbar
					{
						uintptr_t avail = process_type::page_size - mod;

						if( avail >= msg.sbrk_data.incr )
						{
							heap_top += msg.sbrk_data.incr;

							if( (
										proc->heap_max && ( heap_top > reinterpret_cast< uintptr_t >( proc->heap_max ) ) ) || ( heap_top < reinterpret_cast< uintptr_t >( proc->heap_top ) ) )
							{
								msg.error.type = syscalls::e_no_mem;
								logger << message::MORE_VERBOSE << __func__
									<< ": sbrk call failed: no memory" << endl;
							}

							msg.sbrk_data.addr = proc->heap_top;
							proc->heap_top = reinterpret_cast< void * >( heap_top );
							break;
						}

						msg.sbrk_data.incr -= avail;
						heap_top += avail;
					}
					else
					{
						logger << message::MORE_VERBOSE << __func__
							<< ": Reusing available memory " << endl;
					}

					mod = msg.sbrk_data.incr % process_type::page_size;
					if( mod == 0 )
					{
						mod = process_type::page_size;
					}
					void * vaddr = hlmemory::vmalloc( msg.sbrk_data.incr + process_type::page_size - mod, *cur_process, reinterpret_cast< void * >( heap_top ) );
					if( vaddr == 0 )
					{
						msg.error.type = syscalls::e_no_mem;
					}

					if( !vaddr )
					{
						uintptr_t val( 0 );
						val = ~val; //is maybe -1?
						vaddr = reinterpret_cast< void * >( val );
					}

					msg.sbrk_data.addr = vaddr;
					proc->heap_top = reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( proc->heap_top ) + msg.sbrk_data.incr );
				}
				else
				{
					auto decr = -msg.sbrk_data.incr;

					msg.sbrk_data.addr = 0;

					if( decr < mod )
					{
						proc->heap_top = reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( proc->heap_top ) - decr );
					}
					else
					{
						uintptr_t heap_top = reinterpret_cast< uintptr_t >( proc->heap_top );

						do
						{
							heap_top -= mod;
							decr -= mod;
							hlmemory::kfree( reinterpret_cast< void * >( heap_top ), false );
							mod = decr > 4096 ? 4096 : decr;
						}
						while( mod == 4096 );

						heap_top -= decr;

						proc->heap_top = reinterpret_cast< void * >( heap_top );
					}
				}
			}
			break;

		case syscalls::getpid_call:
			{
				msg.getpid_data.pid = cur_process->pid;
			}
			break;

		case syscalls::fork_call:
			{
				uint32_t child_pid = 0;
				bool success = ::iposix::utils::Singleton< scheduler_type >::instance().fork_process( false, child_pid );

				if ( !success )
				{
					msg.error.type = syscalls::e_no_mem;
					msg.fork_data.child_pid = -1;
				}
				else
				{
					msg.fork_data.child_pid = child_pid;
				}
			}
			break;

		case syscalls::execve_call:
			{
				elf_loader_type& el = ::iposix::utils::Singleton< elf_loader_type >::instance();

				bool success = el.load_elf(
						msg.execve_data.file_name,
						msg.execve_data.argv
						);

				if ( !success )
				{
					msg.error.type = syscalls::e_no_exec;
				}
			}
			break;
		
		case syscalls::pipe_call:
			{

				
			
			}
			break;
					
				
		case  syscalls::exit_call:
			{
				bool success = ::iposix::utils::Singleton< scheduler_type >::instance().exit_process();
				if ( !success )
				{
					logger << message::VERBOSE << __func__
						<< ": Exit without success internal error. iPosix will die now..."
						<< endl;
					die();
				}
			}
			break;

		default:
			logger << message::VERBOSE << __func__
				<< ": syscall "
				<< syscall_type_to_string( msg.call.type ).c_str()
				<< " (" << msg.call.number << ") unhandled" << endl;

			msg.error.type = syscalls::e_no_syscall;
			break;
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": syscall "
		<< syscall_type_to_string( msg.call.type ).c_str()
		<< " exits with "
		<< error_type_to_string( msg.error.type ).c_str()
		<< " (" << msg.error.number << ")" << endl;
}

void syscall_dispatcher::dispatch_syscall( syscall_message& msg )
{
	try
	{
		try
		{
			real_dispatch_syscall( msg );
		}
		catch ( ::std::exception& e )
		{
			logger << message::ERROR << __func__
				<< ": Exception: " << e.what()
				<< " . iPosix will die now..." << endl;
			die();
		}
	}
	catch ( ... )
	{
		die();
	}
}

inline void real_dispatch_exception( exception_message& msg )
{
	switch ( msg.exception_number )
	{
		default:
			logger << message::VERBOSE << __func__
				<< ": exception " << msg.message.c_str()
				<< " (" << dec << msg.exception_number
				<< ") with error code " << msg.error_code
				<< " at insruction " << hex << msg.instruction_address
				<< " and page_fault address " << hex << msg.memory_address
				<< dec << " unhandled" << endl;
			logger << message::VERBOSE << __func__
				<< ": Since it is unhandled iPosix will die now..." << endl;
			die();
			break;
	}
}

void syscall_dispatcher::dispatch_exception( exception_message& msg )
{
	try
	{
		try
		{
			real_dispatch_exception( msg );
		}
		catch ( ::std::exception& e )
		{
			logger << message::ERROR << __func__
				<< ": Exception: " << e.what()
				<< " . iPosix will die now..." << endl;
			die();
		}
	}
	catch ( ... )
	{
		die();
	}
}

} //namespace syscalls
} //namespace iposix
