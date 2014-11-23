#ifndef _KERN_ARCH_X86_CORE_GATE_DESCRIPTOR_H_
#define _KERN_ARCH_X86_CORE_GATE_DESCRIPTOR_H_

#include "x86/core/privilege.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/* Predefine */
union segment_selector;

typedef void (*gate_ptr)();

struct __attribute__ ((packed)) gate_descriptor 
{
	/**
	 * Defines the handling type.
	 */
	enum descriptor_type
	{
		/**
		 * Used for Interrupt Handling.
		 */
		interrupt_gate_descriptor,

		/**
		 * Used for Exception Handling.
		 */
		trap_gate_descriptor
	};

	/**
	 * The lower 16bit of the address of the handler.
	 */
	uint16_t offset_0_to_15;

	/**
	 * Which Segment Describtor should be used (see
	 * global_descriptor_table) when this gate is used. If the current
	 * code will be interrupted and the interrupt service routine (ISR)
	 * is call which is the segment the ISR-Code is in (normally the
	 * Kernel Code Segment)
	 */
	uint16_t segment_selector_bits;

	/**
	 * Unknown
	 */
	uint8_t reserved;

	/**
	 * Type field indicate whether this is a interrupt or exception
	 * handler.
	 */
	unsigned type:5;

	/**
	 * Privilege level of this handler
	 */
	unsigned dpl:2;

	/**
	 * Is this Descriptor present or active.
	 */
	unsigned present:1;

	/**
	 * The higher 16bit of the address of the handler
	 */
	uint16_t  offset_16_to_31;

	/**
	 * TODO: black magic
	 */
	static const int TASK_GATE_DESRIPTOR_CODE = 0x05;

	/**
	 * TODO: black magic
	 */
	static const int INTERRUPT_GATE_DESCRIPTOR_CODE = 0x0E;

	/**
	 * TODO: black magic
	 */
	static const int TRAP_GATE_DESCRIPTOR_CODE = 0x0F;

	/**
	 * Constructor of a handler descriptor.
	 * @param[in] offset of the interrupt handler routine
	 * @param[in] type type of the descriptor
	 * @param[in] present present or active flag
	 * @param[in] lvl privilege level
	 */
	gate_descriptor(
			gate_ptr offset = 0,
			descriptor_type type = interrupt_gate_descriptor,
			bool present = false,
			privilege::level lvl = privilege::unused_level_1
			);

	/**
	 * Constructor of a handler descriptor.
	 * @param[in] offset of the interrupt handler routine
	 * @param[in] type type of the descriptor
	 * @param[in] present present or active flag
	 * @param[in] lvl privilege level
	 * @param[in] seg_sel a segment selector which should be used in case of a
	 * trap
	 */
	gate_descriptor(
			gate_ptr offset,
			descriptor_type type,
			bool present,
			privilege::level lvl,
			segment_selector seg_sel
			);
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_GATE_DESCRIPTOR_H_ */
