#ifndef _KERN_ARCH_X86_ATA_ATA_H_
#define _KERN_ARCH_X86_ATA_ATA_H_

#include "utils/singleton.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

namespace iposix
{
namespace arch
{
namespace x86
{
namespace ata
{

/* Predefine */
class ata_drive;
class ata_controller;

/**
 * Implements an ata bus
 */
class ata
	: public ::iposix::utils::Singleton< ::iposix::arch::x86::ata::ata >
{
	friend class ::iposix::utils::Singleton< ata >;

	typedef ::std::shared_ptr< ata_drive > ata_drive_ptr;
	typedef ::std::shared_ptr< ata_controller > ata_controller_ptr;

	public:
		/**
		 * Initializes the ata bus and add the probed devices to the drive 
		 * holder
		 */
		void init();

	protected:
		/** The normal primary bus base port */
		static const uint16_t PRIMARY_BUS_BASE_PORT			= 0x1F0;
		/** The normal primary bus alternativ port */
		static const uint16_t PRIMARY_BUS_ALT_PORT			= 0x3F6;

		/** The normal secondary bus base port */
		static const uint16_t SECONDARY_BUS_BASE_PORT		= 0x170;
		/** The normal secondary bus alternativ port */
		static const uint16_t SECONDARY_BUS_ALT_PORT		= 0x376;

		/** The normal third bus base port */
		static const uint16_t THIRD_BUS_BASE_PORT			= 0x1E8;
		/** The normal third bus alternativ port */
		static const uint16_t THIRD_BUS_ALT_PORT			= 0x3E6;

		/** The normal fourth bus base port */
		static const uint16_t FOURTH_BUS_BASE_PORT			= 0x168;
		/** The normal fourth bus alternativ port */
		static const uint16_t FOURTH_BUS_ALT_PORT			= 0x366;

	private:
		/** Constructor */
		ata();

		/** Destructor */
		~ata();

		/** Holds the bus controllers */
		::std::vector< ata_controller_ptr > controllers;

		/** Holds the drives */
		::std::vector< ata_drive_ptr > drives;
};

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_ATA_ATA_H_ */
