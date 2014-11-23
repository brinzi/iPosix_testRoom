#include "iposix/iposix_kernel.h"
#include "logger/logger.h"
#include "logger/level_filter.h"
#include "fs/i_fs.h"
#include "fs/dev_fs.h"
#include "devices/i_console.h"
#include "devices/i_core_holder.h"

namespace iposix {

using ::iposix::arch::core_holder_type;
using ::iposix::fs::i_fs;
using ::iposix::fs::i_fs_type_t;

template<>
void iposix_kernel< ARCHITECTURE, MEMORY, SCHEDULER >::stop()
{
	this->running = false;
}

template<>
void iposix_kernel< ARCHITECTURE, MEMORY, SCHEDULER >::boot()
{
	this->video.clear();

	logger << message::HIGHEST << "Booting iPosix!" << endl;

	/* Boot procedure start */

	logger << message::VERBOSE << "Initializing virtual filesystem..." << endl;
	i_fs::i_fs_init();

	logger << message::VERBOSE << "Setting up DevFS... " << endl;
	::iposix::utils::Singleton< ::iposix::fs::dev_fs >::instance().init();

	logger << message::VERBOSE << "Setting up CPU Cores... " << endl;
	::iposix::utils::Singleton< core_holder_type >::instance().init();

	logger << message::VERBOSE << "Setting up Architecture... " << endl;
	this->set_up_architecture();

	logger << message::VERBOSE << "Setting up Console..." << endl;
	::iposix::arch::i_console::init();

	/* add additional here */

	/* Boot procduer end */

	logger << message::HIGHEST << "Adding Kernel Process..." << endl;
	::iposix::utils::Singleton< scheduler_type >::instance().put_process0( this );

	logger << message::HIGHEST << "Mounting dummy root node..." << endl;
	i_fs::mount_root_dummy();

	logger << message::HIGHEST << "Mounting device filesystem..." << endl;
	i_fs::mount( i_fs_type_t::fs_dev_fs, "/dev" );

	logger << message::HIGHEST << "Opening stdin, stderr, stdout..." << endl;
	i_fs_node_ptr io = i_fs::open_node( "/dev/stdin" );
	this->push_filehandle( io );
	io = i_fs::open_node( "/dev/stderr" );
	this->push_filehandle( io );
	io = i_fs::open_node( "/dev/stdout" );
	this->push_filehandle( io );

	logger << message::HIGHEST << "Activating Paging..." << endl;
	this->prepare_running();
	::iposix::utils::Singleton< memory_type >::instance().paging( true );

	logger << message::HIGHEST << "Booting finished!" << endl;
}

template<>
void iposix_kernel< ARCHITECTURE, MEMORY, SCHEDULER >::run()
{
	logger << message::HIGHEST << "Running!" << endl;

	while(this->running)
	{
	}
}

template<>
void iposix_kernel< ARCHITECTURE, MEMORY, SCHEDULER >::shutdown()
{
	logger << message::HIGHEST << "Shutting down!" << endl;

	/* shutdown start */


	/* add additional here */



	logger << message::VERBOSE << "Shutting down Architecture... " << endl;
	this->set_down_architecture();

	logger << message::VERBOSE << "Shuting down CPU Cores..." << endl;
	::iposix::utils::Singleton< core_holder_type >::instance().init();

	/* shutdown end */
	logger << message::HIGHEST << "Good bye!" << endl;
}

} //iposix
