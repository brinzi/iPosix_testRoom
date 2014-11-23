#include "x86/devices/video.h"
#include "x86/io_ports/io_ports.h"

namespace iposix {
namespace arch {
namespace x86 {

video::video()
:	cursor_x(0),
	cursor_y(0),
	colors(0x0F00),
	videomem(reinterpret_cast<unsigned short*>(0xb8000))
{
}

video::~video()
{
}

void video::clear()
{
	//writing nothing to the full screen
	for(unsigned int i = 0; i < (80*25); ++i)
	{
		this->videomem[i] = ' ' | this->colors;	
	}

	//reseting position
	this->cursor_y = 0;
	this->cursor_x = 0;
}

void video::write(const char c)
{
	put(c);
}

void video::put(const char c)
{
	if ( c == 0x08 ) // backspace
	{
		//setting cursor one step back 
		if (this->cursor_x != 0)
		{
			--this->cursor_x;
		}
        /*
		 *else if (this->cursor_y != 0) //and may at line end
		 *{
		 *    --this->cursor_y;
		 *}
         */
	}
	else if ( c == 0x09 ) // tabulator
	{
		this->cursor_x = (this->cursor_x + 8) & ~(7); //set lowest bits zero
	}
	else if ( c == '\r' ) // carriage return
	{
		this->cursor_x = 0; // bring back to line begining
	}
	else if ( c == '\n' ) // newline
	{
		this->cursor_x = 0;
		++this->cursor_y;
	}
	else if (c >= ' ') // all other character will be printet out
	{
		// set in video buffer
		this->videomem[this->cursor_y * 80 + this->cursor_x] = c | this->colors;
		++this->cursor_x; // and advance posistion
	}

	if (this->cursor_x >= 80)
	{
		this->cursor_x = 0;
		++this->cursor_y;
	}

	this->scroll(); // scrool screen if needed
	this->move_hardware_cursor(); // and move hardware pointer
}

void video::scroll()
{
	if (this->cursor_y >= 25)
	{
		//moving videomem content on line up
		for(unsigned int i = 0; i < (80*24); ++i)
		{
			this->videomem[i] = this->videomem[i+80];
		}

		//clearing last line
		for (int x = 0; x < 80; ++x)
		{
			videomem[(80*24) + x]= ' ' | this->colors;
		}

		//setting cursor one line back
		this->cursor_y = 24;
	}
}

void video::move_hardware_cursor()
{
	uint16_t index = this->cursor_y * 80 + this->cursor_x;

	io_ports::write_port8( 0x3D4, 14 );
	io_ports::write_port8( 0x3D5, (index >> 8) & 0xFF );
	io_ports::write_port8( 0x3D4, 15 );
	io_ports::write_port8( 0x3D5, (index & 0xFF) );
}

void video::set_textcolor(const int backcolor, const int forecolor)
{
	// backcolor bit 15-12
	// forecolor bit 11-8
	// character bit 7-0
	// Value  	Color
	// 0 		BLACK
	// 1 		BLUE
	// 2 		GREEN
	// 3 		CYAN
	// 4 		RED
	// 5 		MAGENTA
	// 6 		BROWN
	// 7 		LIGHT GREY
	// 8 		DARK GREY
	// 9 		LIGHT BLUE
	// 10 		LIGHT GREEN
	// 11 		LIGHT CYAN
	// 12 		LIGHT RED
	// 13 		LIGHT MAGENTA
	// 14 		LIGHT BROWN
	// 15 		WHITE
	this->colors = (((backcolor & 0x0F) << 4) | (forecolor & 0x0F)) << 8;
}

}}} //iposix::arch::x86
