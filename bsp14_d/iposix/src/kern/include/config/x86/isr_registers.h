#ifndef _KERN_INCLUDE_CONFIG_X86_ISR_REGISTERS_H_
#define _KERN_INCLUDE_CONFIG_X86_ISR_REGISTERS_H_

/*
 * There is no include needed but there has to be a predefine and a typedef
 * ::iposix::arch::isr_regsiters linked to the real isr_regsiters struct in
 * ::iposix::arch::x86::isr_regsiters
 */

namespace iposix
{
namespace arch
{
namespace x86
{

/* Predefine */
struct isr_registers;

} //namespace x86

/* Link it */
typedef ::iposix::arch::x86::isr_registers isr_registers;

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_CONFIG_X86_ISR_REGISTERS_H_ */
