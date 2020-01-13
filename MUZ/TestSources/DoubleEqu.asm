; test double définition d'un EQU
kFirst .EQU 0
kFirst .EQU 1
	.org kFirst
	db 0
	.end