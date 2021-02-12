	.orig x3050
	add r0, r0, #5   ; r0 = 5
	add r1, r0, r0  ; r1 = 10
	and r0, r0, #1  ; r0 = 11
	add r0, r0, #14 ; r0 = 15
	add r1, r1, #-11 ; r1 = -1
	and r1, r1, r0  ;  r1 = 11
	br  nother      ;  
	lea r1, label   ;  r1 = pc + 10
nother	add r2, r1, r0  ;  r2 = pc +18
	lea r1, label   ;  r1 = pc +6
	jmp r1          ;  jumping to label
	add r4, r4, #10 ; r4 = 10
label	add r2, r2, #15 ;  r2 = r2 + 15
	lea r4, numb    ; r4 = address of x 3050
	ldb r5, r4, #0  ; r5 = x30
	ldw r6, r4, #0  ; r6 = x3050 
	and r1, r1, #0  ; r1 = 0
	add r1, r1, #14 ; r1 = 14
	lshf r1, r1, #1 ; r1 = 28
	rshfl r1, r1, #1; r1 = 14
	and r1, r1, #0  ; r1 = 0
	add r1, r1, #-1 ; r1 = -1
	rshfl r1, r1, #1; r1 = x7fff
	lshf r1, r1, #1 ; r1 =-1 = xffff
	rshfa r1, r1, #1; r1 = xffff = -1
	and r1, r1, #0  ; r1 = 0
	not r1, r1      ; r1 = xffff
	lea r2, word    ; r2 = address of x4000
	ldw r2, r2, #0  ; r2 = x4000
	lea r3, byte    ; r3 = address of x 4008
	ldw r3, r3, #0  ; r3 = x4008
	stb r4, r2, #0  ; x4000 = address of x3050
	stw r4, r3, #0  ; x4008 = address of x3050
	lea r2, sub     ; r2 = pc + 6
	jsrr r2         ; next instruction will be and r4
	add r1,r1, #4   ; gets skipped then performs after jsrr
sub	and r4, r4, #0  ; r4 = 0
	and r1, r1, #0  ; r1 = 0
	and r2, r2, #0  ; r2 = 0
	ret             ; jumps to add r1, r1, #4
	halt            ; pc = 0
numb	.fill x3050
word	.fill x4000
byte	.fill x4008
	.end
