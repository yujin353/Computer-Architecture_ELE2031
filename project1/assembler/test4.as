	lw 0 1 one
	lw 0 2 one
	lw 0 4 eghty9
pibo	add 1 2 3
	add 3 1 2
	add 2 3 1
	beq 4 1 done
	beq 0 0 pibo
done halt
one .fill 1
eghty9 .fill 89
