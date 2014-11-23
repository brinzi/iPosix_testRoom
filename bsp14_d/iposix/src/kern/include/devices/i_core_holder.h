#ifndef _KERN_INCLUDE_DEVICES_I_CORE_HOLDER_H_
#define _KERN_INCLUDE_DEVICES_I_CORE_HOLDER_H_

#include "config/cpu_core.h"
#include "utils/singleton.hpp"

#include <vector>
#include <stdint.h>

namespace iposix {
namespace arch {

template< typename Cpu_Core >
class i_core_holder
	: public ::iposix::utils::Singleton< i_core_holder< Cpu_Core > >
{
	friend class ::iposix::utils::Singleton< i_core_holder < Cpu_Core > >;

	public:
		/**
		 * Initializes all cpu cores
		 */
		void init();

		/**
		 * Deregisters all cpu cores
		 */
		void finish();

		/**
		 * Returns the current core
		 * @return the current core
		 */
		Cpu_Core& current_core();

	private:
		/** Constructor */
		i_core_holder();

		/** Destructor */
		~i_core_holder();

		/** Array for all cpu_cores */
		::std::vector< Cpu_Core* > cores;
};

typedef i_core_holder< CPU_CORE > core_holder_type;

typedef CPU_CORE core_type;

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_DEVICES_I_CORE_HOLDER_H_ */
