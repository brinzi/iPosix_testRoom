#ifndef _KERN_ARCH_X86_CORE_TASK_STATE_SEGMENT_H_
#define _KERN_ARCH_X86_CORE_TASK_STATE_SEGMENT_H_

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Desriptor for a Hardware task
 */
struct __attribute__ ((packed)) task_state_segment
{
	//0x00	
	uint32_t link; //previous task segment

	//0x04	
	uint32_t* ESP0; //Stack Pointer when entering Privilege Level 0

	//0x08	
	uint32_t SS0; //Stack Segment when entering Privilege Level 0

	//0x0C	
	uint32_t* ESP1; //Stack Pointer when entering Privilege Level 1

	//0x10	
	uint32_t SS1; //Stack Segment when entering Privilege Level 1

	//0x14	
	uint32_t* ESP2; //Stack Pointer when entering Privilege Level 2

	//0x18	
	uint32_t SS2; //Stack Segment when entering Privilege Level 2

	//0x1C	
	uint32_t CR3; //Page Table Directory Register

	//0x20	
	uint32_t EIP; //Instruction Pointer

	//0x24	
	uint32_t EFLAGS; //Flags Register

	//0x28	
	uint32_t EAX; //general purpose register

	//0x2C	
	uint32_t ECX; //general purpose register

	//0x30	
	uint32_t EDX; //general purpose register

	//0x34	
	uint32_t EBX; //general purpose register

	//0x38	
	uint32_t ESP; //stack pointer

	//0x3C	
	uint32_t EBP; //general purpose register

	//0x40	
	uint32_t ESI; //general purpose register

	//0x44	
	uint32_t EDI; //general purpose register

	//0x48	
	uint32_t ES; //Segment Selector

	//0x4C	
	uint32_t CS; //Segment Selector

	//0x50	
	uint32_t SS; //Segment Selector

	//0x54	
	uint32_t DS; //Segment Selector

	//0x58	
	uint32_t FS; //Segment Selector

	//0x5C	
	uint32_t GS; //Segment Selector

	//0x60	
	uint32_t LDTR; //Local Descriptor Table Register

	//0x64	
	/**
	 * IO Port Permission Bitmap.
	 * A pointer to an IO Port Permission Bitmap witch can grant access
	 * to IO Ports for this task.
	 */
	uint16_t reserved;
	uint16_t IOPB_offset;

	/**
	 * Since a Task State Segment must be greater then 103Byte
	 * we reserve some space here. Good Idea to have the Emergency Stack
	 * here.
	 */
	const static int EMERGENCY_STACK_SIZE = 0x0380; //4KiB
	uint32_t emergency_stack[EMERGENCY_STACK_SIZE + 1];

	/**
	 * Constructor
	 */
	task_state_segment();

	/** Destructor */
	~task_state_segment();

	/**
	 * Dumps the Task state segment
	 */
	void dump();

} __attribute__ ((aligned (128)));

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_TASK_STATE_SEGMENT_H_ */
