	lw 0 1 one
	lw 0 2 n1024
twice	add 1 1 3 // insert 2 noop to avoid branch hazard
	add 3 3 1 // insert 3 noop to avoid branch hazard
	beq 1 2 done // insert 3 noop to avoid branch hazard
	beq 0 0 twice
done halt
one .fill 1
n1024 .fill 1024
