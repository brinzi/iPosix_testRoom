#ifndef _KERN_ARCH_X86_DEVICES_SOUND_H_
#define _KERN_ARCH_X86_DEVICES_SOUND_H_

#include <stdint.h>
#include "x86/devices/timer.h"
#include "utils/singleton.hpp"

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Controls the PC Speaker
 */
class sound
	: private timer,
	  public ::iposix::utils::Singleton< sound >
{
	friend class ::iposix::utils::Singleton< sound >;

	public:
		// i_sound

		/**
		 * Makes a single beep sound
		 */
		void beep();

		/**
		 * Makes a sound in a given frequency
		 * @param[in] frequency the frequency of the sound which has to be
		 * played
		 */
		void play_sound( const uint32_t frequency);

		/**
		 * Stops playing the current sound
		 */
		void stop_sound();

		// ~i_sound

	protected:
		/**
		 * Constructor for the pc speaker.
		 */
		sound();

		/**
		 * Destructor.
		 */
		~sound();

		/**
		 * The Port which controlle the pc speaker.
		 */
		static const uint16_t SOUND_PORT = 0x61;

		/**
		 * The Or-Mask which activates the pc speaker.
		 */
		static const uint8_t SOUND_PLAY_MASK = 0x3;

		/**
		 * The And-Mask which deactivates the pc speaker.
		 */
		static const uint8_t SOUND_STOP_MASK = 0xFC;
		
	private:
		/**
		 * The Mode used for the pc speaker timer.
		 */
		static const timer_mode SOUND_TIMER_MODE;
};

} //namespace x86
}
}

#endif /* !_KERN_ARCH_X86_DEVICES_SOUND_H_ */
