#include "x86/core/gate_descriptor.h"
#include "x86/core/global_descriptor_table.h"
#include "x86/core/segment_selector.h"

namespace iposix
{
namespace arch
{
namespace x86
{

gate_descriptor::gate_descriptor(
		const gate_ptr offset,
		const gate_descriptor::descriptor_type type,
		const bool present,
		const privilege::level lvl)
:	offset_0_to_15( reinterpret_cast<uint32_t>(offset) & 0xFFFF ),
	segment_selector_bits(
			segment_selector(
			global_descriptor_table::GLOBAL_KERNEL_CODE_SEGMENT_NR ).bits
			),
	reserved( 0 ),
	type( 0 ), // later on
	dpl ( 0 ), //later on
	present ( present ? 1 : 0 ),
	offset_16_to_31( ( reinterpret_cast<uint32_t>(offset) >> 16 ) & 0xFFFF)

{
	switch (lvl)
	{
		case privilege::system_privilege_level:
			this->dpl = 0;
			break;

		case privilege::unused_level_1:
			this->dpl = 1;
			break;

		case privilege::unused_level_2:
			this->dpl = 2;
			break;

		case privilege::user_privilege_level:
			this->dpl = 3;
			break;
	}

	switch (type)
	{
		case interrupt_gate_descriptor:
			this->type = INTERRUPT_GATE_DESCRIPTOR_CODE;
			break;

		case trap_gate_descriptor:
			this->type = TRAP_GATE_DESCRIPTOR_CODE;
			break;
	}
}

gate_descriptor::gate_descriptor(
		const gate_ptr offset,
		const gate_descriptor::descriptor_type type,
		const bool present,
		const privilege::level lvl,
		const segment_selector seg_sel)
:	offset_0_to_15( reinterpret_cast<uint32_t>(offset) & 0xFFFF ),
	segment_selector_bits( seg_sel.bits ),
	reserved( 0 ),
	type( 0 ), // later on
	dpl ( 0 ), //later on
	present ( present ? 1 : 0 ),
	offset_16_to_31( ( reinterpret_cast<uint32_t>(offset) >> 16 ) & 0xFFFF)

{
	switch (lvl)
	{
		case privilege::system_privilege_level:
			this->dpl = 0;
			break;

		case privilege::unused_level_1:
			this->dpl = 1;
			break;

		case privilege::unused_level_2:
			this->dpl = 2;
			break;

		case privilege::user_privilege_level:
			this->dpl = 3;
			break;
	}

	switch (type)
	{
		case interrupt_gate_descriptor:
			this->type = INTERRUPT_GATE_DESCRIPTOR_CODE;
			break;

		case trap_gate_descriptor:
			this->type = TRAP_GATE_DESCRIPTOR_CODE;
			break;
	}
}

} //namespace x86
} //namespace arch
} //namespace iposix
