	lw 0 1 Nteen	Load 19 to reg 1
	lw 0 2 neg3	load -3 to reg 2
	lw 0 3 three	load 3 to reg 3
	lw 0 4 one	load 1 to reg 4
start	add 1 2 1	reg1 = reg1 + reg2
	add 4 3 4	reg3 = reg4 + reg3
	beq 4 1 done	if reg4 == reg1 done
	beq 0 0 start	else goto start
done halt 
zero .fill 0 
one .fill 1 
Nteen .fill 19
neg3 .fill -3
three .fill 3
