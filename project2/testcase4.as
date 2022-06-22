	lw 0 1 one
	lw 0 2 one
	lw 0 4 eghty9
	noop
	noop
pibo	add 1 2 3
	noop
	noop
	noop
	add 3 1 2
	noop
	noop
	noop
	add 2 3 1
	noop
	noop
	noop
	beq 4 1 done
	beq 0 0 pibo
done halt
one .fill 1
eghty9 .fill 89
