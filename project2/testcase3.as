	lw 0 1 one // r1 = 1
	lw 0 2 n1024 // r2 = 1024
twice	add 1 1 3 // insert 2 noop to avoid branch hazard r3 = r1 + r1
	add 3 3 1 // insert 3 noop to avoid branch hazard r1 = r3 + r3
	beq 1 2 done // insert 3 noop to avoid branch hazard if r1 = r2 then done
	beq 0 0 twice // else goto twice
done halt
one .fill 1
n1024 .fill 1024
