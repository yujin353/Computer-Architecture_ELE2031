	lw 0 1 one		// r1 = 1
	lw 0 2 two		// r2 = 2
	add 1 2 1		// r1 = r1 + r2, data hazard you add 3 noop between lw and add to solve data hazard
	halt
one	.fill 1
two	.fill 2
