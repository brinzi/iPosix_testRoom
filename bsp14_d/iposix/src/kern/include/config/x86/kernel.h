#ifndef _KERN_INCLUDE_CONFIG_X86_KERNEL_H_
#define _KERN_INCLUDE_CONFIG_X86_KERNEL_H_

#include "config/x86/memory.h"
#include "config/x86/keyboard.h"
#include "config/x86/timer.h"
#include "config/x86/sound.h"
#include "config/x86/ata.h"
#include "config/x86/video.h"

#include "x86/x86_kernel.hpp"
#define ARCHITECTURE ::iposix::arch::x86::x86_kernel< \
	TIMER, \
	VIDEO_DISPLAY, \
	KEYBOARD, \
	SOUND, \
	VIDEO_SERIAL, \
	MEMORY, \
	ATA >

#endif /* !_KERN_INCLUDE_CONFIG_X86_KERNEL_H_ */
