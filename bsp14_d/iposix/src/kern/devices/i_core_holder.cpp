#include "devices/i_core_holder.h"
#include "x86/core/cpu_core.h"
#include "logger/logger.h"

namespace iposix {
namespace arch {

template<>
core_holder_type::i_core_holder()
	: cores()
{ }

template<>
core_holder_type::~i_core_holder()
{ }

template<>
void core_holder_type::init()
{
	core_type* core = new core_type();

	if ( core )
	{
		core->init();

		this->cores.push_back( core );
	}
	else
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Can not create CPU Cores..." << endl;
	}
}

template<>
void core_holder_type::finish()
{
	//TODO
}

template<>
core_type& core_holder_type::current_core()
{
	core_type* core = this->cores[0];

	if ( core == 0 )
	{
		logger << message::CRITICAL << __func__
			<< ": Making dereferencing a null pointer" << endl;
	}

	return *core;
}

} //namespace arch
} //namespace iposix
