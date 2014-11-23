#ifndef _KERN_ARCH_X86_DEVICES_KEYBOARD_AT_SCANCODES_H_
#define _KERN_ARCH_X86_DEVICES_KEYBOARD_AT_SCANCODES_H_

const char keyboard_german[128][4] = 
{
//normal	//shift		//alt-gr	//escaped
{0		,	0		,	0		,	0		},	// 0x0	- unknown
{0		,	0		,	0		,	0		},	// 0x1	- ESCAPE
{'1'	,	'!'		,	0		,	0		},	// 0x2
{'2'	,	'"'		,	0		,	0		},	// 0x3
{'3'	,	0		,	0		,	0		},	// 0x4
{'4'	,	'$'		,	0		,	0		},	// 0x5
{'5'	,	'%'		,	0		,	0		},	// 0x6
{'6'	,	'&'		,	0		,	0		},	// 0x7
{'7'	,	'/'		,	'{'		,	0		},	// 0x8
{'8'	,	'('		,	'['		,	0		},	// 0x9
{'9'	,	')'		,	']'		,	0		},	// 0xA
{'0'	,	'='		,	'}'		,	0		},	// 0xB
{0		,	'?'		,	'\\'	,	0		},	// 0xC	-ss
{0		,	'`'		,	0		,	0		},	// 0xD	- unknown
{'\b'	,	'\b'	,	0		,	0		},	// 0xE	- backspace
{'\t'	,	0		,	0		,	0		},	// 0xF	- tab
{'q'	,	'Q'		,	'@'		,	0		},	// 0x10
{'w'	,	'W'		,	0		,	0		},	// 0x11
{'e'	,	'E'		,	0		,	0		},	// 0x12
{'r'	,	'R'		,	0		,	0		},	// 0x13
{'t'	,	'T'		,	0		,	0		},	// 0x14
{'z'	,	'Z'		,	0		,	0		},	// 0x15
{'u'	,	'U'		,	0		,	0		},	// 0x16
{'i'	,	'I'		,	0		,	0		},	// 0x17
{'o'	,	'O'		,	0		,	0		},	// 0x18
{'p'	,	'P'		,	0		,	0		},	// 0x19
{0		,	0		,	0		,	0		},	// 0x10	- ue
{'+'	,	'*'		,	'~'		,	0		},	// 0x1B
{'\n'	,	0		,	0		,	'\n'	},	// 0x1C	- return
{0		,	0		,	0		,	0		},	// 0x1D	- left ctrl
{'a'	,	'A'		,	0		,	0		},	// 0x1E
{'s'	,	'S'		,	0		,	0		},	// 0x1F
{'d'	,	'D'		,	0		,	0		},	// 0x20
{'f'	,	'F'		,	0		,	0		},	// 0x21
{'g'	,	'G'		,	0		,	0		},	// 0x22
{'h'	,	'H'		,	0		,	0		},	// 0x23
{'j'	,	'J'		,	0		,	0		},	// 0x24
{'k'	,	'K'		,	0		,	0		},	// 0x25
{'l'	,	'L'		,	0		,	0		},	// 0x26
{0		,	0		,	0		,	0		},	// 0x27	- oe
{0		,	0		,	0		,	0		},	// 0x28	- ae
{'^'	,	0		,	0		,	0		},	// 0x29
{0		,	0		,	0		,	0		},	// 0x2A
{'#'	,	'\''	,	0		,	0		},	// 0x2B
{'y'	,	'Y'		,	0		,	0		},	// 0x2C
{'x'	,	'X'		,	0		,	0		},	// 0x2D
{'c'	,	'C'		,	0		,	0		},	// 0x2E
{'v'	,	'V'		,	0		,	0		},	// 0x2F
{'b'	,	'B'		,	0		,	0		},	// 0x30
{'n'	,	'N'		,	0		,	0		},	// 0x31
{'m'	,	'M'		,	0		,	0		},	// 0x32
{','	,	';'		,	0		,	0		},	// 0x33
{'.'	,	':'		,	0		,	0		},	// 0x34
{'-'	,	'_'		,	0		,	'/'		},	// 0x35	-			ESC:gray div
{0		,	0		,	0		,	0		},	// 0x36	- unknown
{'*'	,	0		,	0		,	0		},	// 0x37	- gray mul	ESC:print
{0		,	0		,	0		,	0		},	// 0x38	- left alt	ESC:alt gr
{' '	,	' '		,	0		,	0		},	// 0x39	- Space
{0		,	0		,	0		,	0		},	// 0x3A	- Caps Lock
{0		,	0		,	0		,	0		},	// 0x3B	- F1
{0		,	0		,	0		,	0		},	// 0x3C	- F2
{0		,	0		,	0		,	0		},	// 0x3D	- F3
{0		,	0		,	0		,	0		},	// 0x3E	- F4
{0		,	0		,	0		,	0		},	// 0x3F	- F5
{0		,	0		,	0		,	0		},	// 0x40	- F6
{0		,	0		,	0		,	0		},	// 0x41	- F6
{0		,	0		,	0		,	0		},	// 0x42	- F8
{0		,	0		,	0		,	0		},	// 0x43	- F9
{0		,	0		,	0		,	0		},	// 0x44	- F10
{0		,	0		,	0		,	0		},	// 0x45	- break / num
{0		,	0		,	0		,	0		},	// 0x46	- scroll lock
{'7'	,	0		,	0		,	0		},	// 0x47	- gray 7		ESC:pos1		
{'8'	,	0		,	0		,	0		},	// 0x48	- gray 8        ESC:up		
{'9'	,	0		,	0		,	0		},	// 0x49	- gray 9        ESC:page up	
{'-'	,	0		,	0		,	0		},	// 0x4A	- gray minus    ESC:unknown	
{'4'	,	0		,	0		,	0		},	// 0x4B	- gray 4        ESC:left		
{'5'	,	0		,	0		,	0		},	// 0x4C	- gray 5        ESC:unknown	
{'6'	,	0		,	0		,	0		},	// 0x4D	- gray 6        ESC:right		
{'+'	,	0		,	0		,	0		},	// 0x4E	- gray plus     ESC:unknown	
{'1'	,	0		,	0		,	0		},	// 0x4F	- gray 1        ESC:end		
{'2'	,	0		,	0		,	0		},	// 0x50	- gray 2        ESC:down		
{'3'	,	0		,	0		,	0		},	// 0x51	- gray 3        ESC:page down	
{'0'	,	0		,	0		,	0		},	// 0x52	- gray 0        ESC:insert	
{','	,	0		,	0		,	0		},	// 0x53	- gray ,		ESC:delete	
{0		,	0		,	0		,	0		},	// 0x54	- unknown
{0		,	0		,	0		,	0		},	// 0x55	- unknown
{'<'	,	'>'		,	'|'		,	0		},	// 0x56
{0		,	0		,	0		,	0		},	// 0x57	- F11
{0		,	0		,	0		,	0		},	// 0x58	- F12
{0		,	0		,	0		,	0		},	// 0x59	- unknown
{0		,	0		,	0		,	0		},	// 0x5A	- unknown
{0		,	0		,	0		,	0		},	// 0x5B	- meta L
{0		,	0		,	0		,	0		},	// 0x5C	- meta R
{0		,	0		,	0		,	0		},	// 0x5D	- pop up
{0		,	0		,	0		,	0		},	// 0x5E	- unknown
{0		,	0		,	0		,	0		},	// 0x5F	- unknown
{0		,	0		,	0		,	0		},	// 0x60	- unknown
{0		,	0		,	0		,	0		},	// 0x61	- unknown
{0		,	0		,	0		,	0		},	// 0x62	- unknown
{0		,	0		,	0		,	0		},	// 0x63	- unknown
{0		,	0		,	0		,	0		},	// 0x64	- unknown
{0		,	0		,	0		,	0		},	// 0x65	- unknown
{0		,	0		,	0		,	0		},	// 0x66	- unknown
{0		,	0		,	0		,	0		},	// 0x67	- unknown
{0		,	0		,	0		,	0		},	// 0x68	- unknown
{0		,	0		,	0		,	0		},	// 0x69	- unknown
{0		,	0		,	0		,	0		},	// 0x60	- unknown
{0		,	0		,	0		,	0		},	// 0x6A	- unknown
{0		,	0		,	0		,	0		},	// 0x6B	- unknown
{0		,	0		,	0		,	0		},	// 0x6C	- unknown
{0		,	0		,	0		,	0		},	// 0x6D	- unknown
{0		,	0		,	0		,	0		},	// 0x6E	- unknown
{0		,	0		,	0		,	0		}	// 0x6F	- unknown
};

#endif /* !_KERN_ARCH_X86_DEVICES_KEYBOARD_AT_SCANCODES_H_ */
