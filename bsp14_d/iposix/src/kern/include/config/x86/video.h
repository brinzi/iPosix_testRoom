#ifndef _KERN_INCLUDE_CONFIG_X86_VIDEO_H_
#define _KERN_INCLUDE_CONFIG_X86_VIDEO_H_

#include "devices/i_video.h"

#include "x86/devices/video.h"
#define VIDEO_DISPLAY ::iposix::arch::i_video< ::iposix::arch::x86::video >

#include "x86/devices/serial_port.h"
#define VIDEO_SERIAL ::iposix::arch::i_video< ::iposix::arch::x86::serial_port >

#endif /* !_KERN_INCLUDE_CONFIG_X86_VIDEO_H_ */
