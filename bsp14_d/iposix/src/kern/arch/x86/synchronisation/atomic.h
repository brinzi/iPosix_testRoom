#ifndef _KERN_ARCH_X86_SYNCHRONISATION_ATOMIC_H_
#define _KERN_ARCH_X86_SYNCHRONISATION_ATOMIC_H_

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Describes atomic variables which can be used for synchronisation.
 */
class atomic
{
	public:
		/**
		 * Creates a Atmoic Variable.
		 * @param[in] initial_count the initial value of the atmoic variable
		 */
		atomic( const uint32_t initial_count )
			:	counter( initial_count )
		{
		};

		/**
		 * Destructor
		 */
		~atomic()
		{
		};

		/**
		 * Atomic-Increments a Variable and returns if the result was
		 * _less then or equal_ zero.
		 * @param[in] var the atmoic variable
		 * @return true if result was less then or equal zero.
		 */
		static inline bool inc_and_test_leq_zero( atomic& var );

		/**
		 * Atomic-Decrements a Variable and returns if the result was
		 * _less then_ zero.
		 * @param[in] var the atmoic variable
		 * @return true if result was less then zero.
		 */
		static inline bool dec_and_test_less_zero( atomic& var );
		
	private:
		/**
		 * The Atmoic Count. This musst be volatile to prevent GCC from
		 * optimising.
		 */
		volatile uint32_t counter;
};

/* ********************** *
 * USEFULL ASM-OPERATIONS *
 * ********************** *
 * lock			locks the memory bus for all cpus (make atomic)
 *
 * inc %0		increments %0
 * dec %0		decrements %0
 * add %0,%1	adds %1 to %0
 * sub %0,%1	subs %1 from %0
 * mov %0,%1	moves %1 to %0
 *
 * xchg %0,%1	exchanges %0 with %1 and vice versa (automatical locked see lock)
 *
 * setl %0		sets %0 to 1 if last result was less then zero else 0
 * setle %0		sets %0 to 1 if last result was zero or less else 0
 * setz %0		sets %0 to 1 if last result was zero else 0
 * setge %0		sets %0 to 1 if last result was zero or greater else 0
 * setg %0		sets %0 to 1 if last result was greater then zero else 0
 */

bool atomic::inc_and_test_leq_zero( atomic var& )
{
	uint8_t leq;

	asm volatile(
				"lock incl %0; setle %1"
			:	"m" ( var.counter ),
				"=qm" ( leq ),
			:	
			:	"memory"
		);

	return ( leq == 1 );
}

bool atomic::dec_and_test_less_zero( atomic var& )
{
	uint8_t less;

	asm volatile(
				"lock decl %0; setl %1"
			:	"m" ( var.counter ),
				"=qm" ( less )
			:	
			:	"memory"
		);

	return ( less == 1 );
}

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_SYNCHRONISATION_ATOMIC_H_ */
