#ifndef _KERN_ARCH_X86_CORE_ISR_H_
#define _KERN_ARCH_X86_CORE_ISR_H_

#include <string>
#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Descripes the different execption types which can occur.
 */
enum exceptions
{
	/**
	 * Uninitilized Interrupt. This is not a real exception just a
	 * default in iPosix.
	 */
	unintialized_isr = -2,

	/**
	 * this is not a exception
	 */
	not_exp = -1,

	/**
	 * Raised when a program tries to divide by 0.
	 */
	divide_error_exp = 0,

	/**
	 * Raised when the *T* flag of *eflags* is set (for a step-by-step execution
	 * of a debugged program) or when the address of an instruction or operand
	 * falls within the range of an active debug register.
	 */
	debug_exp = 1,

	/**
	 * Reserved for NonMaskable Interrupts (those which use the NMI pin).
	 */
	nmi_exp = 2, /* unused */

	/**
	 * Caused by int3 (breakpoint) instruction (inserted by a debugger).
	 */
	breakpoint_exp = 3,

	/**
	 * An *into* (check for overflow) instruction has been executed when the
	 * *OF* flag of *eflags* is set.
	 */
	overflow_exp = 4,

	/**
	 * A *bound* (check on address bound) instruction has been executed with the
	 * oper- and outside of the valid address bounds.
	 */
	bounds_check_exp = 5,

	/**
	 * The CPU exectuion unit has detected an invalid opcode (instruction).
	 */
	invalid_opcode_exp = 6,

	/**
	 * An ESCPAE or MMX instruction has been executed with the *TS* flag of
	 * *cr0* set (to save the floating point registers).
	 */
	device_not_available_exp = 7,

	/**
	 * Raised when a second exception is deteced which cannot be handled
	 * serially.
	 */
	double_fault_exp = 8,

	/**
	 * Problems with the external mathematical coprocessor (applies only to old
	 * microprocessors).
	 */
	coprocessor_segment_overrun_exp = 9,

	/**
	 * The CPU has attempted a context switch to a process having an invalid
	 * Task State Segment.
	 */
	invalid_tss_exp = 10,

	/**
	 * A reference was made to a segment not present in memory (one in which the 
	 * *Segment-Present* flag of the Segment Descriptor is cleared).
	 */
	segment_not_present_exp = 11,

	/**
	 * The instruction attempted ti exceed the stack segment limit, or the
	 * segment identified by *ss* is not present in memory.
	 */
	stack_exp = 12,

	/**
	 * One of the protection rules in the protectedmode of the Intel 80x86 has
	 * been violated.
	 */
	general_protection_exp = 13,

	/**
	 * The addressed page is not present in memory, the corresponding page table
	 * entry is null, or a violation of the paging protection mechanism has
	 * occurred.
	 */
	page_fault_exp = 14,

	/**
	 * Unknown
	 */
	intel_reserved_exp = 15,

	/**
	 * The floating point unit integrated into the CPU chip has signaled an
	 * error condition, such as numeric overflow or division by 0.
	 */
	floating_point_error_exp = 16,

	/**
	 * The address of an operand is not correctly aligned (for instance the
	 * address of a long integer is not a multiple of 4).
	 */
	alignment_check_exp = 17,

	/**
	 * TODO find out
	 */
	machine_check_exp = 18,

	/** Intel reserved for future development */ res_19_exp = 19,
	/** Intel reserved for future development */ res_20_exp = 20,
	/** Intel reserved for future development */ res_21_exp = 21,
	/** Intel reserved for future development */ res_22_exp = 22,
	/** Intel reserved for future development */ res_23_exp = 23,
	/** Intel reserved for future development */ res_24_exp = 24,
	/** Intel reserved for future development */ res_25_exp = 25,
	/** Intel reserved for future development */ res_26_exp = 22,
	/** Intel reserved for future development */ res_27_exp = 27,
	/** Intel reserved for future development */ res_28_exp = 28,
	/** Intel reserved for future development */ res_29_exp = 29,
	/** Intel reserved for future development */ res_30_exp = 20,
	/** Intel reserved for future development */ res_31_exp = 31,

	/**
	 * User-Level-Interrupt (Syscall)
	 */
	syscall_exp = 0x80
};

inline const ::std::string& exception_to_string(exceptions e)
{
	static const ::std::string NO_SUCH   = "no_such_exception";

	static const ::std::string EXC_NR_0  = "divide_by_zero_exception";
	static const ::std::string EXC_NR_1  = "debug";
	static const ::std::string EXC_NR_2  = "nmi";
	static const ::std::string EXC_NR_3  = "breakpoint";
	static const ::std::string EXC_NR_4  = "overflow";
	static const ::std::string EXC_NR_5  = "bounds_check";
	static const ::std::string EXC_NR_6  = "invalid_opcode";
	static const ::std::string EXC_NR_7  = "device_not_available";
	static const ::std::string EXC_NR_8  = "double_fault";
	static const ::std::string EXC_NR_9  = "coprocessor_segment_overrun";
	static const ::std::string EXC_NR_10 = "invalid_tss";
	static const ::std::string EXC_NR_11 = "segment_not_present";
	static const ::std::string EXC_NR_12 = "stack_exception";
	static const ::std::string EXC_NR_13 = "general_protection";
	static const ::std::string EXC_NR_14 = "page_fault";
	static const ::std::string EXC_NR_15 = "intel_reserved";
	static const ::std::string EXC_NR_16 = "floating_point_error";
	static const ::std::string EXC_NR_17 = "alignment_check";
	static const ::std::string EXC_NR_18 = "machine_check";

	static const ::std::string EXC_NR_80 = "syscall";
	
	switch (e)
	{
		case divide_error_exp:                  return EXC_NR_0;
		case debug_exp:                         return EXC_NR_1;
		case nmi_exp:                           return EXC_NR_2;
		case breakpoint_exp:                    return EXC_NR_3;
		case overflow_exp:                      return EXC_NR_4;
		case bounds_check_exp:                  return EXC_NR_5;
		case invalid_opcode_exp:                return EXC_NR_6;
		case device_not_available_exp:          return EXC_NR_7;
		case double_fault_exp:                  return EXC_NR_8;
		case coprocessor_segment_overrun_exp:   return EXC_NR_9;
		case invalid_tss_exp:                   return EXC_NR_10;
		case segment_not_present_exp:           return EXC_NR_11;
		case stack_exp:                         return EXC_NR_12;
		case general_protection_exp:            return EXC_NR_13;
		case page_fault_exp:                    return EXC_NR_14;
		case intel_reserved_exp:                return EXC_NR_15;
		case floating_point_error_exp:          return EXC_NR_16;
		case alignment_check_exp:               return EXC_NR_17;
		case machine_check_exp:                 return EXC_NR_18;
		case syscall_exp:                       return EXC_NR_80;
		default:                                return NO_SUCH;
	}
}

enum irqs
{
	unintialized_irq = -2,
	not_irq = -1,
	timer_irq = 0,
	keyboard_irq = 1,
	slave_irq = 2,
	serial_port_com2_com4_irq = 3,
	serial_port_com1_com3_irq = 4,
	parallel_port_lpt2_irq = 5,
	floppy_disk_irq = 6,
	parallel_port_lpt1_irq = 7,
	real_time_clock_irq = 8,
	free1_irq = 9,
	free2_irq = 10,
	free3_irq = 11,
	mouse_irq = 12,
	mathmatic_coprocessor_irq = 13,
	hard_disk_controller1_irq = 14,
	hard_disk_controller2_irq = 15
};

inline const ::std::string& irq_to_string(irqs i)
{
	static const ::std::string NO_SUCH   = "no_such_irq";

	static const ::std::string IRQ_NR_0  = "timer";
	static const ::std::string IRQ_NR_1  = "keyboard";
	static const ::std::string IRQ_NR_2  = "slave";
	static const ::std::string IRQ_NR_3  = "serial_port_com2_com4";
	static const ::std::string IRQ_NR_4  = "serial_port_com1_com3";
	static const ::std::string IRQ_NR_5  = "parallel_port_lpt2";
	static const ::std::string IRQ_NR_6  = "floppy_disk";
	static const ::std::string IRQ_NR_7  = "parallel_port_lpt1";
	static const ::std::string IRQ_NR_8  = "real_time_clock";
	static const ::std::string IRQ_NR_9  = "free1";
	static const ::std::string IRQ_NR_10 = "free2";
	static const ::std::string IRQ_NR_11 = "free3";
	static const ::std::string IRQ_NR_12 = "mouse";
	static const ::std::string IRQ_NR_13 = "mathmatic_coprocessor";
	static const ::std::string IRQ_NR_14 = "hard_disk_controller1";
	static const ::std::string IRQ_NR_15 = "hard_disk_controller2";
	
	switch (i)
	{
		case timer_irq:                   return IRQ_NR_0;
		case keyboard_irq:                return IRQ_NR_1;
		case slave_irq:                   return IRQ_NR_2;
		case serial_port_com2_com4_irq:   return IRQ_NR_3;
		case serial_port_com1_com3_irq:   return IRQ_NR_4;
		case parallel_port_lpt2_irq:      return IRQ_NR_5;
		case floppy_disk_irq:             return IRQ_NR_6;
		case parallel_port_lpt1_irq:      return IRQ_NR_7;
		case real_time_clock_irq:         return IRQ_NR_8;
		case free1_irq:                   return IRQ_NR_9;
		case free2_irq:                   return IRQ_NR_10;
		case free3_irq:                   return IRQ_NR_11;
		case mouse_irq:                   return IRQ_NR_12;
		case mathmatic_coprocessor_irq:   return IRQ_NR_13;
		case hard_disk_controller1_irq:   return IRQ_NR_14;
		case hard_disk_controller2_irq:   return IRQ_NR_15;
		default:                          return NO_SUCH;
	}
}

/**
 * Struct containing some registers and other stuff which may needed by
 * an interrupt handler
 */
struct isr_registers
{
	/**
	 * Page fault register
	 */
	uint32_t cr2;

	/**
	 * The old date segment selector.
	 */
	uint32_t data_segment_selector;

	/** A general purpose register saved by pushad/popad */
	uint32_t edi;
	/** A general purpose register saved by pushad/popad */
	uint32_t esi;
	/** A general purpose register saved by pushad/popad */
	uint32_t ebp;
	/** A general purpose register saved by pushad/popad */
	uint32_t esp;
	/** A general purpose register saved by pushad/popad */
	uint32_t ebx;
	/** A general purpose register saved by pushad/popad */
	uint32_t edx;
	/** A general purpose register saved by pushad/popad */
	uint32_t ecx;
	/** A general purpose register saved by pushad/popad */
	uint32_t eax;

	/**
	 * The exception number 0-255.
	 */
	union exception_number
	{
		int32_t number;
		exceptions exception;
	} exception_interrupt;

	/**
	 * The irq number 0-255.
	 */
	union irq_number
	{
		int32_t number;
		irqs irq;
	} irq_interrupt;

	/**
	 * The error code set by the interrupt in most cases it is 0.
	 */
	uint32_t error_code;

	/**
	 * The old Instruktion Pointer.
	 */
	uint32_t eip;

	/**
	 * The old Code Segment Selector.
	 */
	uint32_t code_segment_selector;

	/**
	 * The content of the EFLAGS Register.
	 */
	uint32_t eflags;

	/**
	 * The old Stack Pointer Register. This Value could be in user (s
	 * virutal address) space.
	 */
	//uint32_t user_esp;

	/**
	 * The old Stack Segment Selector.
	 */
	//uint32_t stack_segment_selector;
}; 


extern "C" void asm_isr_uninitilized();
extern "C" void asm_isr_0();
extern "C" void asm_isr_1();
extern "C" void asm_isr_2();
extern "C" void asm_isr_3();
extern "C" void asm_isr_4();
extern "C" void asm_isr_5();
extern "C" void asm_isr_6();
extern "C" void asm_isr_7();
extern "C" void asm_isr_8();
extern "C" void asm_isr_9();
extern "C" void asm_isr_10();
extern "C" void asm_isr_11();
extern "C" void asm_isr_12();
extern "C" void asm_isr_13();
extern "C" void asm_isr_14();
extern "C" void asm_isr_15();
extern "C" void asm_isr_16();
extern "C" void asm_isr_17();
extern "C" void asm_isr_18();
extern "C" void asm_isr_19();
extern "C" void asm_isr_20();
extern "C" void asm_isr_21();
extern "C" void asm_isr_22();
extern "C" void asm_isr_23();
extern "C" void asm_isr_24();
extern "C" void asm_isr_25();
extern "C" void asm_isr_26();
extern "C" void asm_isr_27();
extern "C" void asm_isr_28();
extern "C" void asm_isr_29();
extern "C" void asm_isr_30();
extern "C" void asm_isr_31();
extern "C" void asm_isr_32();
extern "C" void asm_isr_33();
extern "C" void asm_isr_34();
extern "C" void asm_isr_35();
extern "C" void asm_isr_36();
extern "C" void asm_isr_37();
extern "C" void asm_isr_38();
extern "C" void asm_isr_39();
extern "C" void asm_isr_40();
extern "C" void asm_isr_41();
extern "C" void asm_isr_42();
extern "C" void asm_isr_43();
extern "C" void asm_isr_44();
extern "C" void asm_isr_45();
extern "C" void asm_isr_46();
extern "C" void asm_isr_47();
extern "C" void asm_isr_48();
extern "C" void asm_isr_49();
extern "C" void asm_isr_50();
extern "C" void asm_isr_51();
extern "C" void asm_isr_52();
extern "C" void asm_isr_53();
extern "C" void asm_isr_54();
extern "C" void asm_isr_55();
extern "C" void asm_isr_56();
extern "C" void asm_isr_57();
extern "C" void asm_isr_58();
extern "C" void asm_isr_59();
extern "C" void asm_isr_60();
extern "C" void asm_isr_61();
extern "C" void asm_isr_62();
extern "C" void asm_isr_63();
extern "C" void asm_isr_64();
extern "C" void asm_isr_65();
extern "C" void asm_isr_66();
extern "C" void asm_isr_67();
extern "C" void asm_isr_68();
extern "C" void asm_isr_69();
extern "C" void asm_isr_70();
extern "C" void asm_isr_71();
extern "C" void asm_isr_72();
extern "C" void asm_isr_73();
extern "C" void asm_isr_74();
extern "C" void asm_isr_75();
extern "C" void asm_isr_76();
extern "C" void asm_isr_77();
extern "C" void asm_isr_78();
extern "C" void asm_isr_79();
extern "C" void asm_isr_80();
extern "C" void asm_isr_81();
extern "C" void asm_isr_82();
extern "C" void asm_isr_83();
extern "C" void asm_isr_84();
extern "C" void asm_isr_85();
extern "C" void asm_isr_86();
extern "C" void asm_isr_87();
extern "C" void asm_isr_88();
extern "C" void asm_isr_89();
extern "C" void asm_isr_90();
extern "C" void asm_isr_91();
extern "C" void asm_isr_92();
extern "C" void asm_isr_93();
extern "C" void asm_isr_94();
extern "C" void asm_isr_95();
extern "C" void asm_isr_96();
extern "C" void asm_isr_97();
extern "C" void asm_isr_98();
extern "C" void asm_isr_99();
extern "C" void asm_isr_100();
extern "C" void asm_isr_101();
extern "C" void asm_isr_102();
extern "C" void asm_isr_103();
extern "C" void asm_isr_104();
extern "C" void asm_isr_105();
extern "C" void asm_isr_106();
extern "C" void asm_isr_107();
extern "C" void asm_isr_108();
extern "C" void asm_isr_109();
extern "C" void asm_isr_110();
extern "C" void asm_isr_111();
extern "C" void asm_isr_112();
extern "C" void asm_isr_113();
extern "C" void asm_isr_114();
extern "C" void asm_isr_115();
extern "C" void asm_isr_116();
extern "C" void asm_isr_117();
extern "C" void asm_isr_118();
extern "C" void asm_isr_119();
extern "C" void asm_isr_120();
extern "C" void asm_isr_121();
extern "C" void asm_isr_122();
extern "C" void asm_isr_123();
extern "C" void asm_isr_124();
extern "C" void asm_isr_125();
extern "C" void asm_isr_126();
extern "C" void asm_isr_127();
extern "C" void asm_isr_128();
extern "C" void asm_isr_129();
extern "C" void asm_isr_130();
extern "C" void asm_isr_131();
extern "C" void asm_isr_132();
extern "C" void asm_isr_133();
extern "C" void asm_isr_134();
extern "C" void asm_isr_135();
extern "C" void asm_isr_136();
extern "C" void asm_isr_137();
extern "C" void asm_isr_138();
extern "C" void asm_isr_139();
extern "C" void asm_isr_140();
extern "C" void asm_isr_141();
extern "C" void asm_isr_142();
extern "C" void asm_isr_143();
extern "C" void asm_isr_144();
extern "C" void asm_isr_145();
extern "C" void asm_isr_146();
extern "C" void asm_isr_147();
extern "C" void asm_isr_148();
extern "C" void asm_isr_149();
extern "C" void asm_isr_150();
extern "C" void asm_isr_151();
extern "C" void asm_isr_152();
extern "C" void asm_isr_153();
extern "C" void asm_isr_154();
extern "C" void asm_isr_155();
extern "C" void asm_isr_156();
extern "C" void asm_isr_157();
extern "C" void asm_isr_158();
extern "C" void asm_isr_159();
extern "C" void asm_isr_160();
extern "C" void asm_isr_161();
extern "C" void asm_isr_162();
extern "C" void asm_isr_163();
extern "C" void asm_isr_164();
extern "C" void asm_isr_165();
extern "C" void asm_isr_166();
extern "C" void asm_isr_167();
extern "C" void asm_isr_168();
extern "C" void asm_isr_169();
extern "C" void asm_isr_170();
extern "C" void asm_isr_171();
extern "C" void asm_isr_172();
extern "C" void asm_isr_173();
extern "C" void asm_isr_174();
extern "C" void asm_isr_175();
extern "C" void asm_isr_176();
extern "C" void asm_isr_177();
extern "C" void asm_isr_178();
extern "C" void asm_isr_179();
extern "C" void asm_isr_180();
extern "C" void asm_isr_181();
extern "C" void asm_isr_182();
extern "C" void asm_isr_183();
extern "C" void asm_isr_184();
extern "C" void asm_isr_185();
extern "C" void asm_isr_186();
extern "C" void asm_isr_187();
extern "C" void asm_isr_188();
extern "C" void asm_isr_189();
extern "C" void asm_isr_190();
extern "C" void asm_isr_191();
extern "C" void asm_isr_192();
extern "C" void asm_isr_193();
extern "C" void asm_isr_194();
extern "C" void asm_isr_195();
extern "C" void asm_isr_196();
extern "C" void asm_isr_197();
extern "C" void asm_isr_198();
extern "C" void asm_isr_199();
extern "C" void asm_isr_200();
extern "C" void asm_isr_201();
extern "C" void asm_isr_202();
extern "C" void asm_isr_203();
extern "C" void asm_isr_204();
extern "C" void asm_isr_205();
extern "C" void asm_isr_206();
extern "C" void asm_isr_207();
extern "C" void asm_isr_208();
extern "C" void asm_isr_209();
extern "C" void asm_isr_210();
extern "C" void asm_isr_211();
extern "C" void asm_isr_212();
extern "C" void asm_isr_213();
extern "C" void asm_isr_214();
extern "C" void asm_isr_215();
extern "C" void asm_isr_216();
extern "C" void asm_isr_217();
extern "C" void asm_isr_218();
extern "C" void asm_isr_219();
extern "C" void asm_isr_220();
extern "C" void asm_isr_221();
extern "C" void asm_isr_222();
extern "C" void asm_isr_223();
extern "C" void asm_isr_224();
extern "C" void asm_isr_225();
extern "C" void asm_isr_226();
extern "C" void asm_isr_227();
extern "C" void asm_isr_228();
extern "C" void asm_isr_229();
extern "C" void asm_isr_230();
extern "C" void asm_isr_231();
extern "C" void asm_isr_232();
extern "C" void asm_isr_233();
extern "C" void asm_isr_234();
extern "C" void asm_isr_235();
extern "C" void asm_isr_236();
extern "C" void asm_isr_237();
extern "C" void asm_isr_238();
extern "C" void asm_isr_239();
extern "C" void asm_isr_240();
extern "C" void asm_isr_241();
extern "C" void asm_isr_242();
extern "C" void asm_isr_243();
extern "C" void asm_isr_244();
extern "C" void asm_isr_245();
extern "C" void asm_isr_246();
extern "C" void asm_isr_247();
extern "C" void asm_isr_248();
extern "C" void asm_isr_249();
extern "C" void asm_isr_250();
extern "C" void asm_isr_251();
extern "C" void asm_isr_252();
extern "C" void asm_isr_253();
extern "C" void asm_isr_254();
extern "C" void asm_isr_255();

extern "C" void asm_irq_0();
extern "C" void asm_irq_1();
extern "C" void asm_irq_2();
extern "C" void asm_irq_3();
extern "C" void asm_irq_4();
extern "C" void asm_irq_5();
extern "C" void asm_irq_6();
extern "C" void asm_irq_7();
extern "C" void asm_irq_8();
extern "C" void asm_irq_9();
extern "C" void asm_irq_10();
extern "C" void asm_irq_11();
extern "C" void asm_irq_12();
extern "C" void asm_irq_13();
extern "C" void asm_irq_14();
extern "C" void asm_irq_15();

} //namespace x86
} //namespace arch
} //namespace iposix


#endif /* !_KERN_ARCH_X86_CORE_ISR_H_ */
