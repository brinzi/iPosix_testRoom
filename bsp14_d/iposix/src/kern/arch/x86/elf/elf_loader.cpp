#include "x86/elf/elf_loader.h"
#include "x86/core/segment_selector.h"
#include "x86/core/global_descriptor_table.h"
#include "scheduler/i_process.h"
#include "logger/logger.h"

#include <stdint.h>

//#define DUMP_VM

namespace iposix
{
namespace arch
{
namespace x86
{

using ::iposix::scheduler::process_type;

elf_loader::elf_loader()
{ }

elf_loader::~elf_loader()
{ }

bool elf_loader::jump_in(
		process_type& process,
		uintptr_t stack_pointer,
		uint32_t entry_point,
		char* const* /*args*/
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Jump to " << hex << entry_point
		<< " with stack pointer " << hex << stack_pointer << dec
		<< endl;

#ifdef DUMP_VM
	process.dump_vm();
#endif

	//TODO copy arguments to stack and note that we should free the memory used
	//by the args

	asm volatile(
			"mov %0, %%cr3"
			:	//no outputs
			:	"r" ( process.page_table_ptr() )
			:	//no clobber list
			);


	segment_selector data_segment_selector = 
		segment_selector(
				global_descriptor_table::GLOBAL_USER_DATA_SEGMENT_NR,
				privilege::user_privilege_level);

	segment_selector code_segment_selector =
		segment_selector(
				global_descriptor_table::GLOBAL_USER_CODE_SEGMENT_NR,
				privilege::user_privilege_level);

	asm volatile(
			"cli					# switch of interrupts\n\t"
			"mov %0, %%ds			# save this to every segment register\n\t"
			"mov %0, %%es			\n\t"
			"mov %0, %%fs			\n\t"
			"mov %0, %%gs			# but not to stack segment selector\n\t"
			"push %0				# now we push the stack segment selector\n\t"
			"push %3				# we push the stack pointer\n\t"
			"pushf					# we push the eflags register\n\t"
			"popl %%eax				# we pop the eflags again\n\t"
			"or $0x200, %%eax		# activate interrupt enable flag\n\t"
			"push %%eax			# after modfiy push it again\n\t"
			"push %1				# now we push the code segment selector\n\t"
			"push %2				# push the jump destination\n\t"
			"iret					# jump okay we pretend to be an interrupt\n"
			: /* no outputs */
			:	"m" ( data_segment_selector.bits ), 
				"m" ( code_segment_selector.bits ),
				"r" ( entry_point ),
				"r" ( stack_pointer )
			:	"eax" 
			);

	// We should never never come here
	return false;
}

} //namespace x86
} //namespace arch
} //namespace iposix
