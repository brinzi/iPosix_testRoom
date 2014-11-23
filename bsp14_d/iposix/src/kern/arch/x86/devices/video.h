#ifndef _KERN_ARCH_X86_DEVICES_VIDEO_H_
#define _KERN_ARCH_X86_DEVICES_VIDEO_H_

#include <stdint.h>
#include "devices/i_video.h"
#include "utils/noncopyable.h"

namespace iposix {
namespace arch {
namespace x86 {

struct video
	: virtual public ::iposix::utils::Noncopyable
{
		friend class ::iposix::arch::i_video< video >; //only i_video can use this

		/**
		 * Method for clearing Video
		 */
		void clear();

		/**
		 * Method for writing a single char to Video
		 * @param[in] c the char to write
		 */
		 void write(const char c);

	private:
		/**
		 * Konstruktor for the Video-Interface
		 */
		video();

		/**
		 * Destructor for the Video-Interface
		 */
		virtual ~video();

		/**
		 * X-Cord from Video-Memory
		 */
		unsigned int cursor_x;

		/**
		 * Y-Cord from Video-Memory
		 */
		unsigned int cursor_y;

		/**
		 * Save current colors (back- and foreground)
		 */
		 unsigned int colors;

		/**
		 * pointer to the Video-Memory
		 */
		unsigned short *videomem;

		/**
		 * Scrolls the Screen on line up if needed.
		 */
		void scroll();
		
		/**
		 * Sets the Hardware Cursor to the position where the next
		 * Character will be placed.
		 */
		void move_hardware_cursor();

		/**
		 * Method for writing a single char to Video
		 * @param[in] c the char to write
		 */
		void put(const char c);

		/**
		 * Sets the Color Attributes for Text-Output.
		 * @param[in] backcolor The backgroundcolor.
		 * @param[out] forecolor The foregroundcolor.
		 */
		void set_textcolor(const int backcolor, const int forecolor);
};

}}} //iposix::arch::x86

#endif /* !_KERN_ARCH_X86_DEVICES_VIDEO_H_ */
