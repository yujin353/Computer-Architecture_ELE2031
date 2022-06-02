	lw 0 1 two	reg 1 has a 2
	lw 0 2 three	reg 2 has a 3
	lw 0 3 one	reg 3 has a 1
	add 1 2 1	reg 1 has a 5
	nor 1 2 4	reg 4 has a 5 nor 3
	add 4 3 4	reg 4 is a reverse of reg 1 value
done	halt
two	.fill 2
three	.fill 3
one	.fill 1
