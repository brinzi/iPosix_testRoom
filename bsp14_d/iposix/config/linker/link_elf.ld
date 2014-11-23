/* link_elf.ld */
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)
phys = 0x00100000;
SECTIONS
{
	. = phys;
	.text :
	{
		/* normal code */
		PROVIDE( __section_init__ = . );
		*(.init)

		. = ALIGN(4);
		PROVIDE( __section_code__ = . );
		*(.text)

		. = ALIGN(4);
		PROVIDE( __section_fini__ = . );
		*(.fini)

		/* include constructors */
		. = ALIGN(4);
		PROVIDE( __CTOR_LIST__ = . );
		/*LONG( ( __CTOR_END__ - __CTOR_LIST__ ) / 4 )*/ /* 4 is only correct for 32bit */
		LONG( -1 ) /* so we give no informations */
		*(.ctor*)
		LONG(0)
		PROVIDE( __CTOR_END__ = . );

		/* include destructors */
		. = ALIGN(4);
		PROVIDE( __DTOR_LIST__ = . );
		LONG(-1)
		*(.dtor*)
		LONG(0)
		PROVIDE( __DTOR_END__ = . );

		/* end executable section */
		PROVIDE( __end_executable__ = . );

		. = ALIGN(4096);
	}

	.rdata :
	{
		/* read only data */
		PROVIDE( __section_rdata__ = . );
		*(.rdata)
		*(.rodata)
		. = ALIGN(4096);
	}

	.eh_frame :
	{
		/* Exception Frame */
		PROVIDE( __EH_FRAME_BEGIN__ = . );	
		*(.eh_frame)
		PROVIDE( __EH_FRAME_END__ = . );
		. = ALIGN(4096);
	}

	.data : 
	{
		/* normal data */
		PROVIDE( __section_data__ = . );
		
		*(.data)
		. = ALIGN(4);
		*(.gcc_except_table)
		. = ALIGN(4);
		*(.gcc_exc)
		. = ALIGN(4);
		*(.sdata)
		. = ALIGN(4096);
	}

	.bss : 
	{
		PROVIDE( __section_bss__ = . );
		
		PROVIDE( _edata = . );
		PROVIDE( __edata = . );
		PROVIDE( fbss = . );
		PROVIDE( _fbss = . );

		PROVIDE( __bss_start = . );
		*(.sbss)

		. = ALIGN(4);
		*(.scommon)
		
		*(.bss)
		. = ALIGN(4);

		*(COMMON)
		. = ALIGN(4);

		PROVIDE( __bss_end = . );
	}

	. = ALIGN(4);
	PROVIDE( end = . );
	PROVIDE( _end = . );
	PROVIDE( __end = . );
}
