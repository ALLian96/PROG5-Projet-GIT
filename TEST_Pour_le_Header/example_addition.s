## somme jusqu'à 100
.global main
.global data
.global	boucle
.text
main:
	mov r1, #0		@resultat
	mov r2, #1		@le nombre a additionner
	mov r3, #10	@jusqu'où aller
boucle:
	cmp r2, r3
	beq fin
	bgt addition

addition:
	add r1, r1, r2
	add r2, r2, #1
	bl boucle

.ltorg
	.word main
	.byte boucle
	.hword addition
	
.data
	
fin :

