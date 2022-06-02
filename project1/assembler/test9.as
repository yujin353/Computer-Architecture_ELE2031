	lw 0 1 three	reg 1 has a 3
	lw 0 2 three	reg 2 has a 3
	lw 0 3 one	reg 3 has a 1
	nor 1 2 4	reg 4 has a 5 nor 3
	add 4 3 4	reg 4 is a reverse of reg 1 value
done	halt
three	.fill 3
three	.fill 3
one	.fill 1
