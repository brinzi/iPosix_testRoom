/* link_bin.ld */
OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x00100000; 
SECTIONS
{
	.text phys : AT(phys)
	{
		code = .;
		_code = .;
		__code = .;
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}

	.data : 
	/*AT(phys + (data - code))*/
	{
		/* include constructors */
		start_ctors = .;
		*(.ctor*)
		end_ctors = .;

		/* include destructors */
		start_dtors = .;
		*(.dtor*)
		end_dtors = .;

		data = .;
		_data = .;
		__data = .;
		*(.data)
		. = ALIGN(4096);
	}

	.bss : 
	/*AT(phys + (bss - code))*/
	{
		bss = .;
		_bss = .;
		__bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

    /*
	 *.rodata : AT(phys + (rodata - code))
	 *{
	 *    rodata = .;
	 *    _rodata = .;
	 *    __rodata = .;
	 *    *(.rodata)
	 *    . = ALIGN(4096);
	 *}
     */


	end = .;
	_end = .;
	__end = .;
}
