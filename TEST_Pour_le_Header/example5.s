.global main
.global data_global
.text
	mov r0, #0
main:
	mov r0, #1
	b data_global
main2:
	mov r0, #3
	bl suite
main3:
	mov r0, #5
	bl suite2
main4:	
	nop
suite:
	mov r0, #4
	mov pc, lr
suite2:
	mov r0, #6
	b fin
.ltorg
	.word suite
	.word suite2
	.word main
	.word main2
	.word main3
	.word main4
	.word data_local
	.word data_global
.data
	.skip 8
data_local:
	.word suite
	.word main
	.word data_local
data_global:
	mov r0, #2
	b main2
fin:
