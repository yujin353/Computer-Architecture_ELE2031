	lw 0 1 one	reg1 = 1//always 1
	lw 0 2 one	reg2 = 1//upto
	lw 0 3 one	reg3 = 1
	lw 0 4 neg1	reg4 = -1//count
	lw 0 5 zero reg5 = 0//result
	lw 0 6 n120	reg6 = 120//desire
mult	add 3 5 5	reg5 += reg3
	add 4 1 4	reg4++
	beq 4 2 fact	if reg4 == reg2 fact
	beq 0 0 mult	else mult
fact	beq 5 6 done
	add 2 1 2	reg2++
	add 0 0 4	reg4 = 0
	add 5 0 3	reg3 = reg5
	beq 0 0 mult
done halt
neg1 .fill -1
zero .fill 0 
one .fill 1 
n120 .fill 120
