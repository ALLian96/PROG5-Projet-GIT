## PGCD
.global main
.global data
.text
main:
	ldr r1, =data1
	ldr r1, [r1]
	ldr r2, =data2
	ldr r2, [r2]
boucle:
	cmp r1, r2
	beq fin
	bgt r1sup
r2sup:
	sub r2, r2, r1
	bl boucle
r1sup:
	sub r1, r1, r2
	bl boucle

.ltorg
	.word main
	.word boucle
	.word r2sup
	.word r1sup
	.word fin
	
.data
data1:
	.word 0x45
data2:
	.word 0x31
	
fin :
