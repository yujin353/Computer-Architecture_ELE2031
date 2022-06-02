	lw 0 1 one
	lw 0 2 n1024
two	add 1 1 3
	add 3 3 1
	beq 1 2 done
	beq 0 0 two
done halt
one .fill 1
n1024 .fill 1024
