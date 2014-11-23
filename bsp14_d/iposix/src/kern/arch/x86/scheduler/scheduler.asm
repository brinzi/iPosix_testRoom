; this function is just an proof of concept and is not tested
; only use this function if you know what it _trys_ to do
;void switch_to( uintptr_t* next_esp_ptr, uintptr_t prev_esp_ptr );
[GLOBAL switch_to]
switch_to:
	push ebx				; preserve ebx
	pushf					; get eflags for setting interrupts on in case it
							; was set befor

	cli						; interrupts off

	mov edx, cr0			; get cr0
	and edx, 0x7fffffff		; disable paging bit
	mov cr0, edx			; set cr0

	mov ebx, [esp + 12]		; get nexts pointer to esp
	mov ecx, [esp + 16]		; get previous pointer to esp

	;pusha			; push all general purpose registers


	;mov edx, .after			; create the jmp label
	;push edx				; push the jmp label

	mov edx, cr3			; get the cr3 reg
	push edx				; push the cr3 reg

	mov [ecx], esp			; save old esp
	nop
	mov esp, [ebx]			; restore new esp
	nop

	; restore pag
	pop edx					; get new cr3
	mov cr3, edx			; set new cr3


	mov edx, cr0			; get cr0
	or  edx, 0x80000000		; set paging bit
	mov cr0, edx			; set cr0

	popf					; restore eflags
	pop ebx					; restore ebx

	ret

;uintptr_t get_eip();
[GLOBAL get_eip]
get_eip:
	pop eax
	jmp eax
