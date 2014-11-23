; Have a look at
; http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#boot_002eS 
; http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Example-OS-code
; for information about multiboot.
;

; ------------------
; ----- loader -----
; ------------------
[BITS 32]								; protected mode

[section .text]							; we definitly want this in .text section

[global _start]							; make visible to outside
[global die]

[extern _init]							; the runtime init code
[extern _fini]							; the runtime shutdown code

[extern memory_init]					; the memory init function
[extern kernel_main]					; our main in c++ code
[extern multiboot_init]					; the multiboot init code

[extern __end_executable__]				; defined by the linker
										; end of the executable section
[extern _end]							; defined by the linker
										; end of the kernel


%define ASM
%ifidni __OUTPUT_FORMAT__,elf			; if __OUTPUT_FORMAT__==elf
	%define __ELF__						; define __ELF__
%endif

; -------------------------------------------
;  Duplicated from MULTIBOOT.H 
; -------------------------------------------

; The magic number for the Multiboot header. */
%define MULTIBOOT_HEADER_MAGIC          0x1BADB002

; The magic number passed from a Multiboot compatible Bootloader
%define MULTIBOOT_BOOTLOADER_MAGIC		0x2BADB002

; The Boot-Loader should load the kernel page aligned 
%define MULTIBOOT_PAGE_ALIGN 			1<<0

; The Boot-Loader should give us information about the memory
%define MULTIBOOT_MEMORY_INFO			1<<1

; Additional Informations musst be given to the boot loader if
; this is not an ELF file
%define MULTIBOOT_AOUT_KLUDGE			1<<16

; The flags for the Multiboot header.
%ifdef __ELF__
	%define MULTIBOOT_HEADER_FLAGS		\
		(								\
			MULTIBOOT_PAGE_ALIGN |		\
			MULTIBOOT_MEMORY_INFO		\
		)
%else
	%define MULTIBOOT_HEADER_FLAGS		\
		(								\
			MULTIBOOT_PAGE_ALIGN |		\
			MULTIBOOT_MEMORY_INFO |		\
			MULTIBOOT_AOUT_KLUDGE 		\
		)
%endif

; The Checksum used for validating the Multiboot header
%define MULTIBOOT_CHECKSUM 				\
		-(								\
			MULTIBOOT_HEADER_MAGIC +	\
			MULTIBOOT_HEADER_FLAGS		\
		)

; The size of our stack (4KB).
%define STACK_SIZE                      0x1000

; -------------------------------------------
;  END of MULTIBOOT.H Duplication
; -------------------------------------------


_start:									; the special start entry symbol
	jmp multiboot_entry					; jump over the multiboot header


; ----------------------------------------
; ----- Multiboot Header Starts Here -----
; ----------------------------------------
; Multiboot Header is needed for running with a common bootloader
; like grub. So we do some magic here

align 4
multiboot_header:
	; The GRUB Multiboot header - some kind of boot signature
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM

	; some physikal addresses mostly filled by linker - only needed if not ELF
%ifndef __ELF__
	dd multiboot_header					; where to find the header
	dd _start							; begin of section which should be loaded to ram
										; this part is executable
	dd __end_executable__				; end of the executable section
	dd _end								; end of the section whic should
										; be loaded to ram
	dd multiboot_entry					; entry address
%endif ;__ELF__

	; graphic mode
	; just define is FLAGS[2]  is set
	GRAPHIC_MODE	equ 1				; 0 := linaer graphic,
										; 1 := EGA text mdoe
	GRAPHIC_WIDTH	equ 80				; mode=0 => pixels
										; mode=1 => columns
	GRAPHIC_HEIGHT	equ 43				; mode=0 => pixels
										; mode=1 => columns
	GRAPHIC_DEPTH	equ 0				; mode=0 => bits/pixel
										;		0:=nothing prefered
										; mode=1 => 0
	dd GRAPHIC_MODE
	dd GRAPHIC_WIDTH
	dd GRAPHIC_HEIGHT
	dd GRAPHIC_DEPTH


; --------------------------------------
; ----- Multiboot Header Ends Here -----
; --------------------------------------



; --------------------------------------
; ----- Get the magic information ------
; --------------------------------------

multiboot_entry:
	mov esp, _sys_stack					; sets stackpointer
	push esp							; push the stackpointer

	push 0								; reset EFLAGS
	popf								; pop stack top to EFLAGS register

	push ebx							; push the pointer to the multiboot information
	push eax							; push the magic value

	jmp boot_routine

; --------------------------------------
; ----- end of getting information -----
; --------------------------------------



; --------------------------------------
; ---------- Own startup stuff ---------
; --------------------------------------

boot_routine:
	call multiboot_init					; initialize the multiboot information
										; memory init depends on this

	pop eax								; pop the multiboot information
										; they are still on the stack
	pop ebx								; pop the multiboot information

	call memory_init					; initialize the memory
										; since we first bring up the memory we
										; should use everything afterwards

	call _init							; do the runtime starttup

	call kernel_main					; calls our main-methode
										; void kernel_main( uint32_t initial_stack_pointer )
										; the parameter is generated above

	call _fini							; do the runtime shutdown

	cli									; interrupts off is
										; nessearay for halt command

die:									; this label could be used as the die()
										; function

halt_loop:
	hlt									; halt
	jmp halt_loop						; endless retry ;)

; ---------------------------
; ------ Own stuff end ------
; ---------------------------



; ----------------------
; ----- STACK HERE -----
; ----------------------

section .bss
align 32
	resb STACK_SIZE						; reserved Stack
_sys_stack:								; musst be behind 'cause stack
										; grows backwards
