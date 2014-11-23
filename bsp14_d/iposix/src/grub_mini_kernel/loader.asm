; Have a look at
; http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#boot_002eS 
; http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Example-OS-code
; for information about multiboot.
;

; ------------------
; ----- loader -----
; ------------------
[BITS 32]										; protected mode

[global start]									; predefine
[extern main]									; our main in c++ code
[extern start_ctors]							; defined by the linker
[extern end_ctors]								; defined by the linker
[extern start_dtors]							; defined by the linker
[extern end_dtors]								; defined by the linker

start: 
_start:
	jmp multiboot_entry




; ----------------------------------------
; ----- Multiboot Header Starts Here -----
; ----------------------------------------
; Multiboot Header is needed for running with a common bootloader
; like grub. So we do some magic here

align 4
multiboot_header:
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002		; magic number for the Multiboot header -- "one bad boor"

	MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
;    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
;    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO

    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

	MULTIBOOT_BOOTLOADER_MAGIC equ 0x2BADB002	; The magic number passed by a 
												; Multiboot-compliant boot loader.

    EXTERN code, bss, end


	; The GRUB Multiboot header - some kind of boot signature
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM

	; some physikal addresses mostly filled by linker
	dd multiboot_header
	dd code
	dd bss
	dd end
	dd multiboot_entry

; --------------------------------------
; ----- Multiboot Header Ends Here -----
; --------------------------------------



; --------------------------------------
; ----- Get the magic information ------
; --------------------------------------

multiboot_entry:
	mov esp, _sys_stack							; sets stackpointer

	push 0										; reset EFLAGS
	popf										; pop stack top to EFLAGS register

	push ebx									; push the pointer to the multiboot information
	push eax									; push the magic value

	jmp boot_routine

; --------------------------------------
; ----- end of getting information -----
; --------------------------------------



; --------------------------------------
; ---------- Own startup stuff ---------
; --------------------------------------

boot_routine:
static_ctors_loop:
	mov ebx, start_ctors
	jmp .test
.body:
	call [ebx]
	add ebx, 4
.test:
	cmp ebx, end_ctors
	jb .body

	call main							; calls our main-methode int main(void)

static_dtors_loop:
	mov ebx, start_dtors
	jmp .test
.body:
	call [ebx]
	add ebx, 4
.test:
	cmp ebx, end_dtors
	jb .body

	cli									; interrupts off is
										; nessearay for halt command
halt_loop:
	hlt									; halt
	jmp halt_loop						; endless retry ;)

; ---------------------------
; ------ Own stuff end ------
; ---------------------------



; ----------------------
; ----- STACK HERE -----
; ----------------------

SECTION .bss
	resb 16384							; 8KiB reserved Stack
_sys_stack:								; musst be behind 'cause stack
										; grows backwards
