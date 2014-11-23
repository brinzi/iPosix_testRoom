; here we define the part of an interrupt service routing which can not (or
; properply can not, or should not or we simply do not want to) made in C++

[extern isr_handler]

%macro ISR_WITHOUT_ERROR_CODE 1		; 1 parameter accessed by 
									;	%1 the isr number
	[global asm_isr_%1]				; this funktion is globally known
	align 16						; should be 16byte alligned
	asm_isr_%1:						; the function entry

		cli							; at first switch off interrupts

		push 0						; push a dummy error code on the stack - cause
									; this ISR does not push it's own

		push -1						; we push a -1 for not irq

		push %1						; push the ISR number

		jmp asm_isr_common_stuff	; jump to general ISR stuff
%endmacro

%macro ISR_WITH_ERROR_CODE 1		; 1 parameter accessed by %1
									;	%1 the isr number
	[global asm_isr_%1]				; this funktion is globally known
	align 16						; should be 16byte alligned
	asm_isr_%1:						; the function entry

		cli							; at first switch off interrupts
									; this ISR pushes it's own error code

		push -1						; we push a -1 for not irq

		push %1						; push the ISR number

		jmp asm_isr_common_stuff	; jump to general ISR stuff
%endmacro

%macro IRQ_WITHOUT_ERROR_CODE 2		; 2 parameters accessed by 
									;	%1 the isr number
									;	%2 the irq number
	[global asm_irq_%2]				; this funktion is globally known
	align 16						; should be 16byte alligned
	asm_irq_%2:						; the function entry

		cli							; at first switch off interrupts
									; this ISR pushes it's own error code

		push 0						; push a dummy error code on the stack - cause
									; a IRQ does not have one

		push %2						; we push the IRQ number

		push -1						; we push a -1 for no exception

		jmp asm_isr_common_stuff	; jump to general ISR stuff
%endmacro

; Here is what every ISR has to do after an interrupt occuered.
; 1. Save all Registers and Segment Selector
; 2. Load new Segment Selectors
; 3. Call the high-level Interrupt Service Routine
; 4. Reload old Segment Selectors
; 5. Switch on interrupts
; 6. Jump back to interrupted procedure
asm_isr_common_stuff:
	pushad					; Nice method for pushing edi, esi, ebp, esp, ebx, 
							; edx, ecx and eax
	mov eax, 0x0			; setting register content to zero

	mov ax, ds				; load data segement selector to ax (lower 16bit of
							; eax)

	push eax				; push the data segment selector

	mov eax, 0x10			; load the kernel data segment selector
	mov ds, ax				; to all data selector registers
	mov es, ax				; there has to be a better way cause this is
	mov fs, ax				; hardcoded and does not care about the value
	mov gs, ax				; in the GDT
							; SS was set through the hardware

	mov eax, cr2			; get the page fault register
	push eax				; push the page fault register

	call isr_handler		; call the higher level Interrupt Service Routine

	add esp, 4

	pop eax					; reload the old data segment selector
	mov ds, ax				; to all data selector registers
	mov es, ax
	mov fs, ax
	mov gs, ax
							; TODO what about SS

	popa					; Nice method again for poping edi, esi, ebp, esp,
							; ebx, edx, ecs and eax
	add esp, 12				; set the stackpointer so that the pushed error code 
							; and pushed ISR number is cleaned up
							; actually we push 3 additional values of 4byte
							; so we set the stack pointer 3*4=12byte back
	sti						; switch on interrupts
	iret					; pops: CS, EIP, EFLAGS, SS, and ESP
							; and return from interrupt service routing 
							; to the interrupted procedure

; following ISRs

[global asm_isr_uninitilized]	; this funktion is globally known
align 16						; should be 16byte alligned
asm_isr_uninitilized:			; the function entry

	cli							; at first switch off interrupts

	push 0						; push a dummy error code on the stack - couse
								; this ISR does not push it's own

	push 0						; we push a 0 for Exceptions

	push -1						; push the ISR number

	jmp asm_isr_common_stuff	; jump to general ISR stuff

; create code for all ISRs (256) 
; 	 0-18  are _must_ have
;	19-31  are  must  have
;	32-256 are should have
; ISR_WITHOUT_ERROR_CODE ( isr_number)
; ISR_WITH_ERROR_CODE ( isr_number )
ISR_WITHOUT_ERROR_CODE 0	;  0 - Division by zero exception
ISR_WITHOUT_ERROR_CODE 1	;  1 - Debug exception
ISR_WITHOUT_ERROR_CODE 2	;  2 - Non maskable interrupt
ISR_WITHOUT_ERROR_CODE 3	;  3 - Breakpoint exception
ISR_WITHOUT_ERROR_CODE 4	;  4 - 'Into detected overflow'
ISR_WITHOUT_ERROR_CODE 5	;  5 - Out of bounds exception
ISR_WITHOUT_ERROR_CODE 6	;  6 - Invalid opcode exception
ISR_WITHOUT_ERROR_CODE 7	;  7 - No coprocessor exception
ISR_WITH_ERROR_CODE 8		;  8 - Double fault (pushes an error code)
ISR_WITHOUT_ERROR_CODE 9	;  9 - Coprocessor segment overrun
ISR_WITH_ERROR_CODE 10		; 10 - Bad TSS (pushes an error code)
ISR_WITH_ERROR_CODE 11		; 11 - Segment not present (pushes an error code)
ISR_WITH_ERROR_CODE 12		; 12 - Stack fault (pushes an error code)
ISR_WITH_ERROR_CODE 13		; 13 - General protection fault (pushes an error code)
ISR_WITH_ERROR_CODE 14		; 14 - Page fault (pushes an error code)
ISR_WITHOUT_ERROR_CODE 15	; 15 - Unknown interrupt exception
ISR_WITHOUT_ERROR_CODE 16	; 16 - Coprocessor fault
ISR_WITHOUT_ERROR_CODE 17	; 17 - Alignment check exception
ISR_WITHOUT_ERROR_CODE 18	; 18 - Machine check exception

; Something reserved
ISR_WITHOUT_ERROR_CODE 19	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 20	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 21	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 22	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 23	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 24	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 25	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 26	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 27	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 28	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 29	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 30	; 19-31 - Reserved
ISR_WITHOUT_ERROR_CODE 31	; 19-31 - Reserved

; irq handler
; IRQ_WITHOUT_ERROR_CODE ( isr_number , irq_number )
IRQ_WITHOUT_ERROR_CODE 32,  0	; Intel 8253/8254 Programmable Interval Timer
IRQ_WITHOUT_ERROR_CODE 33,  1	; Intel 8042 Keyboard Controller
IRQ_WITHOUT_ERROR_CODE 34,  2	; cascaded to slave 8259 INT line
IRQ_WITHOUT_ERROR_CODE 35,  3	; 8250 UART serial port COM2 + COM4
IRQ_WITHOUT_ERROR_CODE 36,  4	; 8250 UART serial port COM1 + COM3
IRQ_WITHOUT_ERROR_CODE 37,  5	; Intel 8255 Parallel Port LPT2
IRQ_WITHOUT_ERROR_CODE 38,  6	; Intel 82072A Floppy Disk Controller
IRQ_WITHOUT_ERROR_CODE 39,  7	; Intel 8255 Parallel Port LPT1
IRQ_WITHOUT_ERROR_CODE 40,  8	; real-time-clock (RTC)
IRQ_WITHOUT_ERROR_CODE 41,  9	; no common
IRQ_WITHOUT_ERROR_CODE 42, 10	; no common
IRQ_WITHOUT_ERROR_CODE 43, 11	; no common
IRQ_WITHOUT_ERROR_CODE 44, 12	; Intel 8042 PS/2 Mouse Controller
IRQ_WITHOUT_ERROR_CODE 45, 13	; math coprocessor
IRQ_WITHOUT_ERROR_CODE 46, 14	; hard disk controller 1
IRQ_WITHOUT_ERROR_CODE 47, 15	; hard disk controller 2

ISR_WITHOUT_ERROR_CODE 128	; syscall interrupt

; more common ISRs
;ISR_WITHOUT_ERROR_CODE 48
;ISR_WITHOUT_ERROR_CODE 49
;ISR_WITHOUT_ERROR_CODE 50
;ISR_WITHOUT_ERROR_CODE 51
;ISR_WITHOUT_ERROR_CODE 52
;ISR_WITHOUT_ERROR_CODE 53
;ISR_WITHOUT_ERROR_CODE 54
;ISR_WITHOUT_ERROR_CODE 55
;ISR_WITHOUT_ERROR_CODE 56
;ISR_WITHOUT_ERROR_CODE 57
;ISR_WITHOUT_ERROR_CODE 58
;ISR_WITHOUT_ERROR_CODE 59
;ISR_WITHOUT_ERROR_CODE 60
;ISR_WITHOUT_ERROR_CODE 61
;ISR_WITHOUT_ERROR_CODE 62
;ISR_WITHOUT_ERROR_CODE 63
;ISR_WITHOUT_ERROR_CODE 64
;ISR_WITHOUT_ERROR_CODE 65
;ISR_WITHOUT_ERROR_CODE 66
;ISR_WITHOUT_ERROR_CODE 67
;ISR_WITHOUT_ERROR_CODE 68
;ISR_WITHOUT_ERROR_CODE 69
;ISR_WITHOUT_ERROR_CODE 70
;ISR_WITHOUT_ERROR_CODE 71
;ISR_WITHOUT_ERROR_CODE 72
;ISR_WITHOUT_ERROR_CODE 73
;ISR_WITHOUT_ERROR_CODE 74
;ISR_WITHOUT_ERROR_CODE 75
;ISR_WITHOUT_ERROR_CODE 76
;ISR_WITHOUT_ERROR_CODE 77
;ISR_WITHOUT_ERROR_CODE 78
;ISR_WITHOUT_ERROR_CODE 79
;ISR_WITHOUT_ERROR_CODE 80
;ISR_WITHOUT_ERROR_CODE 81
;ISR_WITHOUT_ERROR_CODE 82
;ISR_WITHOUT_ERROR_CODE 83
;ISR_WITHOUT_ERROR_CODE 84
;ISR_WITHOUT_ERROR_CODE 85
;ISR_WITHOUT_ERROR_CODE 86
;ISR_WITHOUT_ERROR_CODE 87
;ISR_WITHOUT_ERROR_CODE 88
;ISR_WITHOUT_ERROR_CODE 89
;ISR_WITHOUT_ERROR_CODE 90
;ISR_WITHOUT_ERROR_CODE 91
;ISR_WITHOUT_ERROR_CODE 92
;ISR_WITHOUT_ERROR_CODE 93
;ISR_WITHOUT_ERROR_CODE 94
;ISR_WITHOUT_ERROR_CODE 95
;ISR_WITHOUT_ERROR_CODE 96
;ISR_WITHOUT_ERROR_CODE 97
;ISR_WITHOUT_ERROR_CODE 98
;ISR_WITHOUT_ERROR_CODE 99
;ISR_WITHOUT_ERROR_CODE 100
;ISR_WITHOUT_ERROR_CODE 101
;ISR_WITHOUT_ERROR_CODE 102
;ISR_WITHOUT_ERROR_CODE 103
;ISR_WITHOUT_ERROR_CODE 104
;ISR_WITHOUT_ERROR_CODE 105
;ISR_WITHOUT_ERROR_CODE 106
;ISR_WITHOUT_ERROR_CODE 107
;ISR_WITHOUT_ERROR_CODE 108
;ISR_WITHOUT_ERROR_CODE 109
;ISR_WITHOUT_ERROR_CODE 110
;ISR_WITHOUT_ERROR_CODE 111
;ISR_WITHOUT_ERROR_CODE 112
;ISR_WITHOUT_ERROR_CODE 113
;ISR_WITHOUT_ERROR_CODE 114
;ISR_WITHOUT_ERROR_CODE 115
;ISR_WITHOUT_ERROR_CODE 116
;ISR_WITHOUT_ERROR_CODE 117
;ISR_WITHOUT_ERROR_CODE 118
;ISR_WITHOUT_ERROR_CODE 119
;ISR_WITHOUT_ERROR_CODE 120
;ISR_WITHOUT_ERROR_CODE 121
;ISR_WITHOUT_ERROR_CODE 122
;ISR_WITHOUT_ERROR_CODE 123
;ISR_WITHOUT_ERROR_CODE 124
;ISR_WITHOUT_ERROR_CODE 125
;ISR_WITHOUT_ERROR_CODE 126
;ISR_WITHOUT_ERROR_CODE 127

;ISR_WITHOUT_ERROR_CODE 129
;ISR_WITHOUT_ERROR_CODE 130
;ISR_WITHOUT_ERROR_CODE 131
;ISR_WITHOUT_ERROR_CODE 132
;ISR_WITHOUT_ERROR_CODE 133
;ISR_WITHOUT_ERROR_CODE 134
;ISR_WITHOUT_ERROR_CODE 135
;ISR_WITHOUT_ERROR_CODE 136
;ISR_WITHOUT_ERROR_CODE 137
;ISR_WITHOUT_ERROR_CODE 138
;ISR_WITHOUT_ERROR_CODE 139
;ISR_WITHOUT_ERROR_CODE 140
;ISR_WITHOUT_ERROR_CODE 141
;ISR_WITHOUT_ERROR_CODE 142
;ISR_WITHOUT_ERROR_CODE 143
;ISR_WITHOUT_ERROR_CODE 144
;ISR_WITHOUT_ERROR_CODE 145
;ISR_WITHOUT_ERROR_CODE 146
;ISR_WITHOUT_ERROR_CODE 147
;ISR_WITHOUT_ERROR_CODE 148
;ISR_WITHOUT_ERROR_CODE 149
;ISR_WITHOUT_ERROR_CODE 150
;ISR_WITHOUT_ERROR_CODE 151
;ISR_WITHOUT_ERROR_CODE 152
;ISR_WITHOUT_ERROR_CODE 153
;ISR_WITHOUT_ERROR_CODE 154
;ISR_WITHOUT_ERROR_CODE 155
;ISR_WITHOUT_ERROR_CODE 156
;ISR_WITHOUT_ERROR_CODE 157
;ISR_WITHOUT_ERROR_CODE 158
;ISR_WITHOUT_ERROR_CODE 159
;ISR_WITHOUT_ERROR_CODE 160
;ISR_WITHOUT_ERROR_CODE 161
;ISR_WITHOUT_ERROR_CODE 162
;ISR_WITHOUT_ERROR_CODE 163
;ISR_WITHOUT_ERROR_CODE 164
;ISR_WITHOUT_ERROR_CODE 165
;ISR_WITHOUT_ERROR_CODE 166
;ISR_WITHOUT_ERROR_CODE 167
;ISR_WITHOUT_ERROR_CODE 168
;ISR_WITHOUT_ERROR_CODE 169
;ISR_WITHOUT_ERROR_CODE 170
;ISR_WITHOUT_ERROR_CODE 171
;ISR_WITHOUT_ERROR_CODE 172
;ISR_WITHOUT_ERROR_CODE 173
;ISR_WITHOUT_ERROR_CODE 174
;ISR_WITHOUT_ERROR_CODE 175
;ISR_WITHOUT_ERROR_CODE 176
;ISR_WITHOUT_ERROR_CODE 177
;ISR_WITHOUT_ERROR_CODE 178
;ISR_WITHOUT_ERROR_CODE 179
;ISR_WITHOUT_ERROR_CODE 180
;ISR_WITHOUT_ERROR_CODE 181
;ISR_WITHOUT_ERROR_CODE 182
;ISR_WITHOUT_ERROR_CODE 183
;ISR_WITHOUT_ERROR_CODE 184
;ISR_WITHOUT_ERROR_CODE 185
;ISR_WITHOUT_ERROR_CODE 186
;ISR_WITHOUT_ERROR_CODE 187
;ISR_WITHOUT_ERROR_CODE 188
;ISR_WITHOUT_ERROR_CODE 189
;ISR_WITHOUT_ERROR_CODE 190
;ISR_WITHOUT_ERROR_CODE 191
;ISR_WITHOUT_ERROR_CODE 192
;ISR_WITHOUT_ERROR_CODE 193
;ISR_WITHOUT_ERROR_CODE 194
;ISR_WITHOUT_ERROR_CODE 195
;ISR_WITHOUT_ERROR_CODE 196
;ISR_WITHOUT_ERROR_CODE 197
;ISR_WITHOUT_ERROR_CODE 198
;ISR_WITHOUT_ERROR_CODE 199
;ISR_WITHOUT_ERROR_CODE 200
;ISR_WITHOUT_ERROR_CODE 201
;ISR_WITHOUT_ERROR_CODE 202
;ISR_WITHOUT_ERROR_CODE 203
;ISR_WITHOUT_ERROR_CODE 204
;ISR_WITHOUT_ERROR_CODE 205
;ISR_WITHOUT_ERROR_CODE 206
;ISR_WITHOUT_ERROR_CODE 207
;ISR_WITHOUT_ERROR_CODE 208
;ISR_WITHOUT_ERROR_CODE 209
;ISR_WITHOUT_ERROR_CODE 210
;ISR_WITHOUT_ERROR_CODE 211
;ISR_WITHOUT_ERROR_CODE 212
;ISR_WITHOUT_ERROR_CODE 213
;ISR_WITHOUT_ERROR_CODE 214
;ISR_WITHOUT_ERROR_CODE 215
;ISR_WITHOUT_ERROR_CODE 216
;ISR_WITHOUT_ERROR_CODE 217
;ISR_WITHOUT_ERROR_CODE 218
;ISR_WITHOUT_ERROR_CODE 219
;ISR_WITHOUT_ERROR_CODE 220
;ISR_WITHOUT_ERROR_CODE 221
;ISR_WITHOUT_ERROR_CODE 222
;ISR_WITHOUT_ERROR_CODE 223
;ISR_WITHOUT_ERROR_CODE 224
;ISR_WITHOUT_ERROR_CODE 225
;ISR_WITHOUT_ERROR_CODE 226
;ISR_WITHOUT_ERROR_CODE 227
;ISR_WITHOUT_ERROR_CODE 228
;ISR_WITHOUT_ERROR_CODE 229
;ISR_WITHOUT_ERROR_CODE 230
;ISR_WITHOUT_ERROR_CODE 231
;ISR_WITHOUT_ERROR_CODE 232
;ISR_WITHOUT_ERROR_CODE 233
;ISR_WITHOUT_ERROR_CODE 234
;ISR_WITHOUT_ERROR_CODE 235
;ISR_WITHOUT_ERROR_CODE 236
;ISR_WITHOUT_ERROR_CODE 237
;ISR_WITHOUT_ERROR_CODE 238
;ISR_WITHOUT_ERROR_CODE 239
;ISR_WITHOUT_ERROR_CODE 240
;ISR_WITHOUT_ERROR_CODE 241
;ISR_WITHOUT_ERROR_CODE 242
;ISR_WITHOUT_ERROR_CODE 243
;ISR_WITHOUT_ERROR_CODE 244
;ISR_WITHOUT_ERROR_CODE 245
;ISR_WITHOUT_ERROR_CODE 246
;ISR_WITHOUT_ERROR_CODE 247
;ISR_WITHOUT_ERROR_CODE 248
;ISR_WITHOUT_ERROR_CODE 249
;ISR_WITHOUT_ERROR_CODE 250
;ISR_WITHOUT_ERROR_CODE 251
;ISR_WITHOUT_ERROR_CODE 252
;ISR_WITHOUT_ERROR_CODE 253
;ISR_WITHOUT_ERROR_CODE 254
;ISR_WITHOUT_ERROR_CODE 255

;%undefine ISR_WITHOUT_ERROR_CODE
;%undefine ISR_WITH_ERROR_CODE
