#include "x86/core/task_state_segment.h"
#include "x86/core/segment_selector.h"
#include "x86/core/global_descriptor_table.h"
#include "logger/logger.h"

namespace iposix
{
namespace arch
{
namespace x86
{

task_state_segment::task_state_segment()
	:	link(0), 
	ESP0(&emergency_stack[EMERGENCY_STACK_SIZE]), SS0(0),
	ESP1(&emergency_stack[EMERGENCY_STACK_SIZE]), SS1(0),
	ESP2(&emergency_stack[EMERGENCY_STACK_SIZE]), SS2(0),
	CR3(0), EIP(0), EFLAGS(0),
	EAX(0), ECX(0), EDX(0), EBX(0), ESP(0), EBP(0), ESI(0), EDI(0),
	ES(0), CS(0), SS(0),
	DS(0), FS(0), GS(0),
	LDTR(0), reserved(0), IOPB_offset( sizeof( task_state_segment ) )
{
	segment_selector data_segment_selector;
	segment_selector code_segment_selector;
	segment_selector switch_data_segment_selector;

	switch_data_segment_selector =
		segment_selector(
				global_descriptor_table::GLOBAL_KERNEL_DATA_SEGMENT_NR
				);

	data_segment_selector =
		segment_selector(
				global_descriptor_table::GLOBAL_KERNEL_DATA_SEGMENT_NR,
				privilege::user_privilege_level
				);

	code_segment_selector =
		segment_selector(
				global_descriptor_table::GLOBAL_KERNEL_CODE_SEGMENT_NR,
				privilege::user_privilege_level
				);

	this->SS0	= switch_data_segment_selector.bits;
	this->SS1	= switch_data_segment_selector.bits;
	this->SS2	= switch_data_segment_selector.bits;
	this->DS	= data_segment_selector.bits;
	this->ES	= data_segment_selector.bits;
	this->SS	= data_segment_selector.bits;
	this->FS	= data_segment_selector.bits;

	this->GS	= data_segment_selector.bits;

	this->CS	= code_segment_selector.bits;

	this->LDTR = ( global_descriptor_table::INTERRUPT_TASK_LDT_NR << 3 ) + 3;

	this->IOPB_offset = sizeof(task_state_segment);
}

task_state_segment::~task_state_segment()
{ }

void task_state_segment::dump()
{
	#define dmp_ptr( value )\
		logger << '\t' << #value << "=\"" \
			<< hex << reinterpret_cast<uintptr_t>( this->value ) \
			<< "\"" << endl

	#define dmp( value ) \
		logger << '\t' << #value << "=\"" \
		<< hex << this->value \
		<< dec << "\"" << endl

	logger << message::MORE_VERBOSE << __func__
		<< ": Dumping Task State Segment at "
		<< hex << reinterpret_cast<uintptr_t>( this )
		<< endl;

	dmp( link );
	dmp_ptr( ESP0 );
	dmp( SS0 );
	dmp_ptr( ESP1 );
	dmp( SS1 );
	dmp_ptr( ESP2 );
	dmp( SS2 );
	dmp( CR3 );
	dmp( EIP );
	dmp( EFLAGS );
	dmp( EAX );
	dmp( ECX );
	dmp( EDX );
	dmp( EBX );
	dmp( ESP );
	dmp( EBP );
	dmp( ESI );
	dmp( EDI );
	dmp( ES );
	dmp( CS );
	dmp( SS );
	dmp( DS );
	dmp( FS );
	dmp( GS );
	dmp( LDTR );
	dmp( reserved );
	dmp( IOPB_offset );
}

} //namespace x86
} //namespace arch
} //namespace iposix
