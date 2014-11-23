#include "x86/core/memory_descriptor.h"
#include "logger/logger.h"

namespace iposix
{
namespace arch
{
namespace x86
{

memory_descriptor::memory_descriptor(
		segment_type type,
		bool present,
		uint32_t base,
		uint32_t limit,
		bool high_granularity
		)
:	limit_0_to_15( ( limit & 0xFFFF ) ),
	base_0_to_15( ( base & 0xFFFF ) ),
	base_16_to_23( ( base >> 16 ) & 0xFF ),
	type(0), //later on
	system(1),
	discriptor_privilege_level(0), //later on
	segment_present( present ? 1 : 0 ),
	limit_16_to_19( ( limit >> 16 ) & 0xF ),
	AVL(0), //unused
	reserved(0), //allways 0
	data_or_code( 0 ),
	granularity( high_granularity ? 0 : 1 ),
	base_24_to_31( ( base >> 24) & 0xFF )
{
	switch(type)
	{
		case null_pointer_segment_descriptor:
			this->limit_0_to_15 = 0;
			this->base_0_to_15 = 0;
			this->base_16_to_23 = 0;
			this->type = 0;
			this->system = 0;
			this->discriptor_privilege_level = 0;
			this->segment_present = 0;
			this->limit_16_to_19 = 0;
			this->AVL = 0;
			this->reserved = 0;
			this->data_or_code = 0;
			this->granularity = 0;
			this->base_24_to_31 = 0;
			break;

		case kernel_code_segment_descriptor:
			this->type = 0xA;
			this->discriptor_privilege_level = 0; // system_privilege_level
			this->data_or_code = 1;
			break;

		case kernel_data_segment_descriptor:
			this->type = 0x2;
			this->discriptor_privilege_level = 0; // system_privilege_level
			this->data_or_code = 1;
			break;

		case user_code_segment_descriptor:
			this->type = 0xA;
			this->discriptor_privilege_level = 3; // user_privilege_level
			this->data_or_code = 1;
			break;

		case user_data_segment_descriptor:
			this->type = 0x2;
			this->discriptor_privilege_level = 3; // user_privilege_level
			this->data_or_code = 1;
			break;

		case task_state_segment_descriptor:
			//9 indicates a task witch is not busy
			//11 indicates a task witch is busy
			this->type = 0x9; //TODO keep in mind
			this->system = 0;
			this->discriptor_privilege_level = 0; // system_privilege_level
			this->data_or_code = 1;
			break;

		case local_descriptor_table_descriptor:
			this->type = 0x2;
			this->system = 0;
			this->discriptor_privilege_level = 0; // system_privilege_level
			this->data_or_code = 1;
			break;
	}
}

uint32_t memory_descriptor::base()
{
	return ( base_24_to_31 << 24 ) + ( base_16_to_23 << 16 ) + ( base_0_to_15 );
}

uint32_t memory_descriptor::limit()
{
	return ( limit_16_to_19 << 16 ) + ( limit_0_to_15 );
}

void memory_descriptor::dump()
{
	uint32_t b = base();
	uint32_t l = limit();
	uint32_t t = type;
	uint32_t dpl = discriptor_privilege_level;

	logger << "\tMD"
		<< '\t' << hex << t
		<< '\t' << hex << dpl
		<< '\t'
		<< ( system ? "S" : "-" )
		<< ( segment_present ? "P" : "-" )
		<< ( data_or_code ? "32" : "16" )
		<< ( granularity ? "L" : "H" )
		<< '\t' << hex << b
		<< '\t' << hex << l
		<< endl;
}

} //namespace x86
} //namespace arch
} //namespace iposix
